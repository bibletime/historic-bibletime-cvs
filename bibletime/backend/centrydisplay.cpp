/***************************************************************************
                          centrydisplay.cpp  -  description
                             -------------------
    begin                : Fre Mai 31 2002
    copyright            : (C) 2002 by The BibleTime team
    email                : info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//BibleTime includes
#include "centrydisplay.h"
#include "cswordkey.h"
#include "cswordversekey.h"
#include "cswordbookmoduleinfo.h"
#include "creferencemanager.h"
#include "cdisplaytemplatemgr.h"

#include "frontend/cbtconfig.h"

#include "util/scoped_resource.h"

//Qt includes
#include <qapplication.h>

CEntryDisplay::CEntryDisplay(){
}

CEntryDisplay::~CEntryDisplay(){
}

/** Returns the rendered text using the modules in the list and using the key parameter. The displayoptions and filter options are used, too. */
const QString CEntryDisplay::text( QPtrList<CSwordModuleInfo> modules, const QString& keyName, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions, const QString& displayTemplateName )
{
  backend()->setDisplayOptions( displayOptions );
  backend()->setFilterOptions( filterOptions );

	QString text = entryText( modules, keyName );
	if (modules.count() > 1) {
		QString header;

		for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
			header += QString::fromLatin1("<th width=\"%1%\">%2</th>")
					.arg(100 / modules.count())
					.arg(m->name());
		}
		text = "<table><tr>" + header + "</tr>" + text + "</table>";
	}

	const QString langAbbrev = ((modules.count() == 1) && modules.first()->language().isValid()) 
		? modules.first()->language().abbrev() 
		: "unknown";
	
	CDisplayTemplateMgr tMgr;
	return tMgr.fillTemplate(displayTemplateName, "title", text, langAbbrev);
}

/** Returns a preview for the given module and key. This is useful for the seatchdialog and perhaps the tooltips. */
/*const QString CEntryDisplay::previewText( CSwordModuleInfo*  module, const QString& keyName, const QString& headerText, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions)
{
  backend()->setDisplayOptions( displayOptions );
  backend()->setFilterOptions( filterOptions );

  util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
  key->key(keyName);

	CDisplayTemplateMgr tMgr;
	return tMgr.fillTemplate( CBTConfig::get(CBTConfig::displayStyle), QString::null, key->renderedText() );
}*/

/** Renders one entry using the given modules and the key. This makes chapter rendering more easy. */
const QString CEntryDisplay::entryText( QPtrList<CSwordModuleInfo> modules, const QString& keyName){
  util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(modules.first()) );
  key->key(keyName);
  
	//declarations out of the loop for optimization
  QString entry;
	QString renderedText;
  QString keyText;
  bool isRTL;

  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key->module(m);
    key->key(keyName);
    keyText = key->key();
    isRTL = (m->textDirection() == CSwordModuleInfo::RightToLeft);
		
		entry = QString::null;

/*		key.renderedText();
		int pvHeading = 0;
		do { //add sectiontitle before we add the versenumber
			QString preverseHeading = QString::fromUtf8(
m->module()->getEntryAttributes()["Heading"]["Preverse"][QString::number(pvHeading++).latin1()].c_str());
			if (!preverseHeading.isEmpty()) {
				entry += QString::fromLatin1("<div lang=\"%1\" class=\"sectiontitle\">%1</div>")
					.arg(m->language().isValid() 
						? QString::fromLatin1("lang=\"%1\"").arg(m->language().abbrev()) 
						: QString::null
					)
					.arg(preverseHeading);
			}
			else {
				break;
			}
		} while (true);
*/

		entry += QString::fromLatin1("<div %1 %2 dir=\"%3\">%4</div>")
			.arg(m->language().isValid() 
				? QString::fromLatin1("lang=\"%1\"").arg(m->language().abbrev()) 
				: QString::null
			)
			.arg((modules.count() == 1) ? QString::fromLatin1("class=\"entry\"") : QString::null)
			.arg(isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr"))
			.arg(key->renderedText());
		
  	if (modules.count() == 1) {
			renderedText += entry;
		}
  	else {
	    renderedText += QString::fromLatin1("<td class=\"entry\" %1 dir=\"%2\">%3</td>")
				.arg(m->language().isValid() 
					? QString::fromLatin1("lang=\"%1\"").arg(m->language().abbrev()) 
					: QString::null
				)
				.arg(isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr"))
				.arg(entry);
		}
	}
 	if (modules.count() > 1) {
		renderedText += QString::fromLatin1("</tr>");
	}
  return renderedText;
}


void CEntryDisplay::setDisplayOptions(const CSwordBackend::DisplayOptions options) {
  m_displayOptions = options;
}

/** Returns the right reference text which can be incluced in the HTML */
const QString CEntryDisplay::htmlReference( CSwordModuleInfo* module, const QString& keyName, const QString linkText, const QString& anchorText ) {
  if (linkText.isEmpty()) {
    return QString::fromLatin1("<a name=\"%1\"/>").arg(anchorText);
  }
  else {
    return QString::fromLatin1("<a name=\"%1\" href=\"%2\">%3</a>")
      .arg(anchorText)
      .arg(CReferenceManager::encodeHyperlink(module->name(), keyName, CReferenceManager::typeFromModule(module->type()) ))
      .arg(linkText);
  }
}

/* ----------------------- new class: CChapterDisplay ------------------- */

/** Returns the rendered text using the modules in the list and using the key parameter. The displayoptions and filter options are used, too. */
const QString CChapterDisplay::text( QPtrList <CSwordModuleInfo> modules, const QString& keyName, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions, const QString& displayTemplateName ) {
  backend()->setDisplayOptions( displayOptions );
  backend()->setFilterOptions( filterOptions );
  QString text = QString::null;

  CSwordVerseKey key(0);
  key = keyName;

  const int currentTestament = key.Testament();
	const int currentBook = key.Book();
	const int currentChapter = key.Chapter();

  CSwordModuleInfo* module = modules.first();
  bool ok = true;

	QPtrList<CSwordKey*> keyList;
	for (key.Verse(1); key.Testament() == currentTestament && key.Book() == currentBook && key.Chapter() == currentChapter && ok && !module->module()->Error(); ok = key.next(CSwordVerseKey::UseVerse) && !key.Error() ) {
		keyList.append( key.copy() );		
	}

/*	if (modules.count() > 1) {
//		text = QString::fromLatin1("<table>%1</table>").arg(text);
		QString header;

		for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
			header += QString::fromLatin1("<th width=\"%1%\">%2</th>")
					.arg(100 / modules.count())
					.arg(m->name());
		}
		text = "<table><tr>" + header + "</tr>" + text + "</table>";
	}

	QString langAbbrev = ((modules.count() == 1) && modules.first()->language().isValid()) ? modules.first()->language().abbrev() : "unknown";
	
	CDisplayTemplateMgr tMgr;
	return tMgr.fillTemplate(displayTemplateName, "title", text, langAbbrev);
	*/
	
	return renderList( modules, keyList );
}

/** Renders one entry using the given modules and the key. This makes chapter rendering more easy. */
/*const QString CChapterDisplay::entryText( QPtrList<CSwordModuleInfo> modules, const QString& keyName, const QString& chosenKey ) {

  CSwordVerseKey key(modules.first());
  QString renderedText = (modules.count() > 1) ? QString::fromLatin1("<tr>") : QString::null;

	// Only insert the table stuff if we are displaying parallel.
  // Otherwise, strip out he table stuff -> the whole chapter will be rendered in one cell!


  //declarations out of the loop for optimization
  QString entry;
  QString keyText;
  bool isRTL;

  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key.module(m);
    key.key(keyName);
    keyText = key.key();
    isRTL = (m->textDirection() == CSwordModuleInfo::RightToLeft);
    //font = CBTConfig::get(m->language()).second;
		entry = QString::null;

		key.renderedText();
		int pvHeading = 0;
		do { //add sectiontitle before we add the versenumber
			QString preverseHeading = QString::fromUtf8(
m->module()->getEntryAttributes()["Heading"]["Preverse"][QString::number(pvHeading++).latin1()].c_str());
			if (!preverseHeading.isEmpty()) {
				entry += QString::fromLatin1("<div %1 class=\"sectiontitle\">%1</div>")
					.arg(m->language().isValid() 
						? QString::fromLatin1("lang=\"%1\"").arg(m->language().abbrev()) 
						: QString::null
					)
					.arg(preverseHeading);
			}
			else {
				break;
			}
		} while (true);

		entry += QString::fromLatin1("<%1 %2 %3 dir=\"%4\">") //linebreaks = div, without = span
    	.arg(m_displayOptions.lineBreaks ? QString::fromLatin1("div") : QString::fromLatin1("span"))
			.arg((modules.count() == 1) 
				? ((keyText == chosenKey) ? QString::fromLatin1("class=\"currentverse\"") : QString::fromLatin1("class=\"verse\"")) 
				: "") //insert only the class if we're not in a td
			.arg(m->language().isValid() 
				? QString::fromLatin1("lang=\"%1\"").arg(m->language().abbrev()) 
				: QString::null
			)
			.arg(isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr"));

		entry += QString::fromLatin1("<span dir=\"%1\" class=\"versenum\">%2</span>")
			.arg(isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr"))
			.arg( m_displayOptions.verseNumbers 
				? htmlReference(m, keyText, QString::number(key.Verse()), keyText) 
				: htmlReference(0, QString::null, QString::null, keyText)
			);
		
		//entry += QString::fromLatin1("<span>%1</span>").arg(key.renderedText());
		entry += key.renderedText();
		entry +=  (m_displayOptions.lineBreaks 
			? QString::fromLatin1("</div>") 
			: QString::fromLatin1("</span>"));
		
  	if (modules.count() == 1) {
			renderedText += entry;
		}
  	else {
	    renderedText += QString::fromLatin1("<td class=\"%1\" %2 dir=\"%3\">%4</td>")
				.arg((keyText == chosenKey) ? QString::fromLatin1("currentverse") : QString::fromLatin1("verse"))
				.arg(m->language().isValid() 
					? QString::fromLatin1("lang=\"%1\"").arg(m->language().abbrev()) 
					: QString::null
				)
				.arg(isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr"))
				.arg(entry);
		}
	}
 	if (modules.count() > 1) {
		renderedText += QString::fromLatin1("</tr>");
	}
  return renderedText;
}*/

const QString CChapterDisplay::renderEntry(ListCSwordModuleInfo const* list, CSwordKey* key) {
	return QString::null;
}

/* ----------------------- new class: CBookDisplay ------------------- */

/** Returns the rendered text using the modules in the list and using the key parameter. The displayoptions and filter options are used, too. */
const QString CBookDisplay::text( QPtrList <CSwordModuleInfo> modules, const QString& keyName, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions, const QString& displayTemplateName ) {
  backend()->setDisplayOptions( displayOptions );
  backend()->setFilterOptions( filterOptions );

	CDisplayTemplateMgr tMgr;

	CSwordBookModuleInfo* book = dynamic_cast<CSwordBookModuleInfo*>(modules.first());

  // the number of levels which should be display together, 1 means display no entries together
  int displayLevel = book->config( CSwordModuleInfo::DisplayLevel ).toInt();

  util::scoped_ptr<CSwordTreeKey> key(
		dynamic_cast<CSwordTreeKey*>( CSwordKey::createInstance(book) )
	);
  key->key(keyName); //set the key to position we'd like to get

  // standard of DisplayLevel, display nothing together
  // if the current key is the root entry don't display anything together!
  if ((displayLevel <= 1) || (key->key().isEmpty() || (key->key() == "/") )) {
		QString ret = tMgr.fillTemplate(CBTConfig::get(CBTConfig::displayStyle), QString::null, entryText(modules, key));
  	key->key(keyName); //restore before we return so make sure it doesn't break anything
    return ret;
  };

  /**
  * Check whether displaying displayLevel levels together is possible.
	* For this count the childs and parents
  * of the required position
  */

  int possibleLevels = 1; //we start with the default value of displayLevel, which means no entries together
  while( key->parent() && (key->key() != "/") && !key->key().isEmpty() ) {//add parents
    ++possibleLevels;
  };
  key->key(keyName); //set the key to the start position
  while( key->firstChild( )) { //add childs
    ++possibleLevels;
  };

  if (possibleLevels < displayLevel) { //too few levels available!
    //display current level, we could also decide to display the available levels together
		return tMgr.fillTemplate(CBTConfig::get(CBTConfig::displayStyle), QString::null, entryText(modules, key));
  };
  if ((displayLevel > 2) && (displayLevel == possibleLevels)) { //fix not to diplay the whole module
    --displayLevel;
  }

  // at this point we're sure that we can display the required levels toogether
  // at the moment we're at the lowest level, so we only have to go up!
  for (int currentLevel = 1; currentLevel < displayLevel; ++currentLevel) { //we start again with 1 == standard of displayLevel
    if (!key->parent()) { //something went wrong althout we checked before! Be safe and return entry's text
			return tMgr.fillTemplate(CBTConfig::get(CBTConfig::displayStyle), QString::null, entryText(modules, key));
    };
  };

  // no we can display all sub levels together! We checked before that this is possible!
  m_text = entryText(modules, key, 0, (key->key() == keyName));

  const bool hasToplevelText = !key->strippedText().isEmpty();

  key->firstChild(); //go to the first sibling on the same level
  m_chosenKey = keyName;

  printTree(key, modules, hasToplevelText); //if the top level entry has text ident the other text

	key->key(keyName); //restore key
	return tMgr.fillTemplate(displayTemplateName, QString::null, m_text);
}

/** Renders one entry using the given modules and the key. This makes chapter rendering more easy. */
const QString CBookDisplay::entryText( QPtrList<CSwordModuleInfo> modules, CSwordTreeKey* const key, const int level, const bool activeKey){
  /**
  * We have to be careful that we don't change the value of the key! We pass pointers for optimizations reasons,
  * since entryText is called many times!
  * creating copies of the key object takes too long
  */
	CSwordBookModuleInfo* book = dynamic_cast<CSwordBookModuleInfo*>(modules.first());
  const QString& keyName = key->getFullName();
  const bool isRTL = (book->textDirection() == CSwordModuleInfo::RightToLeft);

  return QString::fromLatin1("<%1 %2 class=\"%3\" dir=\"%4\"><span class=\"entryname\" dir=\"%5\">%6</span> %7</%8>")
    .arg( m_displayOptions.lineBreaks  ? "div" : "span" )
    .arg( book->language().isValid() 
			? QString::fromLatin1("lang=\"%1\"").arg(book->language().abbrev()) 
			: QString::null
		)
		.arg( activeKey ? "currententry" : "entry" )
		.arg( isRTL ? "rtl" : "ltr")
		.arg( isRTL ? "rtl" : "ltr")
    .arg( htmlReference(book, keyName, key->getLocalName(), (!keyName.isEmpty() ? keyName : "/") ))
		.arg( key->renderedText() )
		.arg( m_displayOptions.lineBreaks ? "div" : "span" );
}

void CBookDisplay::printTree(CSwordTreeKey* const treeKey, QPtrList<CSwordModuleInfo> modules, const int levelPos){
  // make sure we don't change the value of the key!

  //static for performance reasons, static is faster because the
  //initialization isn't executed more than one time
  static QString fullKeyName;
  fullKeyName = treeKey->getFullName();

  m_text += entryText(modules, treeKey, levelPos, (m_chosenKey == fullKeyName));

  if (treeKey->hasChildren()) { //print tree for the child items
    treeKey->firstChild();
    printTree(treeKey, modules, levelPos+1); //doesn't change the value of the key! (this function)
    treeKey->key(fullKeyName); //go back where we came from
  }

  if (treeKey->nextSibling()) { //print tree for next entry on the same depth
		printTree(treeKey, modules, levelPos);
    treeKey->key(fullKeyName); //return to the value we had at the beginning of this block!
  }
}




/*!
    \fn CTextRendering::renderKeyList( ListCSwordModuleInfo* modules, QList<CSwordKey*>keyList )
 */
const QString CTextRendering::renderKeyList( ListCSwordModuleInfo const* modules, QPtrList<CSwordKey*> keyList ) {

	QString text;
	for ( key = keyList.first(); key; key = keyList.next() ) {
		text += renderEntry( modules, key );
	}
	
	return finishText( text );
}


/*!
    \fn CTextRendering::renderKeyRange( ListCSwordModuleInfo, const QString& start, const QString& stop )
 */
const QString CTextRendering::renderKeyRange( ListCSwordModuleInfo const* modules, const QString& start, const QString& stop ) {

	//make the range to a list of keys and call renderKeyList
	
	return QString::null;
}


/*!
    \fn CTextRendering::renderSingleKey( ListCSwordModuleInfo const*, CSwordKey* )
 */
const QString CTextRendering::renderSingleKey( ListCSwordModuleInfo const* modules, CSwordKey* key )
{
	m_text = renderEntry( modules, key );
	return finishText( m_text );
    /// @todo implement me
}


/*!
    \fn CTextRendering::finishText()
 */
//void CTextRendering::finishText( const QString& text ) {
//	return CDisplayTemplateMgr::fillTemplate(CBTConfig::get(CBTConfig::displayStyle), QString::null, text);
    /// @todo implement me
//}
