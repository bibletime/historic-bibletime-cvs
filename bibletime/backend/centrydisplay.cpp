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

#include "frontend/cbtconfig.h"

#include "util/scoped_resource.h"

//Qt includes
#include <qapplication.h>

CEntryDisplay::CEntryDisplay(){
}

CEntryDisplay::~CEntryDisplay(){
}

/** Returns the rendered text using the modules in the list and using the key parameter. The displayoptions and filter options are used, too. */
const QString CEntryDisplay::text( QPtrList<CSwordModuleInfo> modules, const QString& keyName, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions ) {
  backend()->setDisplayOptions( displayOptions );
  backend()->setFilterOptions( filterOptions );

  const QString entry = entryText(modules, keyName);
  return finishText( entry, modules, keyName );
}

/** Returns a preview for the given module and key. This is useful for the seatchdialog and perhaps the tooltips. */
const QString CEntryDisplay::previewText( CSwordModuleInfo*  module, const QString& keyName, const QString& headerText, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions){
  backend()->setDisplayOptions( displayOptions );
  backend()->setFilterOptions( filterOptions );

  util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
  key->key(keyName);

  QString css = QString::null;
  for (int t = MinType; t <= MaxType; ++t) {
    css += "\t" + cssString( static_cast<CEntryDisplay::StyleType>(t) ) + "\n\n";
  }

  const QString pageStart = QString::fromLatin1("<HTML><HEAD><STYLE type=\"text/css\">%1</STYLE></HEAD><BODY>%2<HR>")
    .arg(css)
    .arg(headerText);

  const QString text = QString::fromLatin1("<DIV %1>%2</DIV>")
    .arg(module->textDirection() == CSwordModuleInfo::RightToLeft ? QString::fromLatin1("dir=\"rtl\"") : QString::null)
//    .arg(module->isUnicode() ? QString::fromLatin1("class=\"unicodetext\"") : QString::null)
    .arg(key->renderedText());

  const QString pageEnd = QString::fromLatin1("</BODY></HTML>");
  return pageStart + text + pageEnd;
}

/** Renders one entry using the given modules and the key. This makes chapter rendering more easy. */
const QString CEntryDisplay::entryText( QPtrList<CSwordModuleInfo> modules, const QString& keyName){
//	Q_ASSERT(modules.first());
  util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(modules.first()) );
  key->key(keyName);
  QString renderedText = QString::null;

  QFont moduleFont;
  QString tdStyle;
  
  renderedText = QString::fromLatin1("<TR valign=\"top\">");
  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    moduleFont = CBTConfig::get( m->language() );
    tdStyle = QString::fromLatin1("style=\"%1 %2 font-family:%3; font-size:%4pt;\"")
      .arg(((modules.at()+1) < modules.count()) ? QString::fromLatin1("padding-right: 2mm; border-right:thin solid black;") : QString::null)
      .arg(((modules.at()>0) && ((modules.at()+1) <= modules.count() )) ? QString::fromLatin1("padding-left:2mm;") : QString::null)
      .arg(moduleFont.family())
      .arg(moduleFont.pointSize());

    key->module(m);
    key->key(keyName);

    renderedText += QString::fromLatin1("<TD %1 valign=\"top\"><SPAN>%2</SPAN></TD>")
                      .arg(tdStyle)
                      .arg(key->renderedText());
  }
  renderedText += QString::fromLatin1("</TR>");
  return renderedText;
}

/** Returns the color of the given type. */
const QColor CEntryDisplay::color( const CEntryDisplay::ColorType type ) {
  switch (type) {
    case ReferenceColor:
      return CBTConfig::get(CBTConfig::swordRefColor);
    case TextColor:
      return CBTConfig::get(CBTConfig::textColor);
    case HighlightedTextColor:
      return CBTConfig::get(CBTConfig::highlightedVerseColor);
    case BackgroundColor:
      return CBTConfig::get(CBTConfig::backgroundColor);
    case FootnoteColor:
      return CBTConfig::get(CBTConfig::footnotesColor);
    case StrongsNumberColor:
      return CBTConfig::get(CBTConfig::strongsColor);
    case MorphCodeColor:
      return CBTConfig::get(CBTConfig::morphsColor);
    case JesusWordColor:
      return CBTConfig::get(CBTConfig::jesuswordsColor);
    default:
      return Qt::white;
  }
}

/** Returns the font of the given type. */
const QFont CEntryDisplay::font( const CLanguageMgr::Language& lang ) {
  return CBTConfig::get(lang);
}

/** Adds the right headers and footers to the page and returns them together. */
const QString CEntryDisplay::finishText( const QString text, QPtrList <CSwordModuleInfo> modules, const QString& keyName){
  CSwordModuleInfo* module = modules.first();
  Q_ASSERT(module); //shouldn't happen
  util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(modules.first()) );
  key->key(keyName);

  QString css = "table.maintable {width:100%;} td.tableheading {border-bottom: thin solid black;}";
  for (int t = MinType; t <= MaxType; ++t) {
    css += "\t" + cssString( static_cast<CEntryDisplay::StyleType>(t) ) + "\n\n";
  }

  const int columnWidth = (int)((float)100 / (float)modules.count());

  QString pageStart = QString::fromLatin1("<HTML><HEAD><STYLE type=\"text/css\">%1</STYLE></HEAD><BODY attr1=\"working!\"><TABLE cellspacing=\"0\" class=\"maintable\"><TR>")
    .arg(css);

  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key->module(m);
    const QString newKeyName = key->key();
    pageStart += QString::fromLatin1("<TD class=\"tableheading\" width=\"%1%\"><CENTER><B>%2</B> %3</CENTER></TD>")
      .arg(columnWidth)
      .arg(m->name())
      .arg(!newKeyName.isEmpty() ? QString::fromLatin1("(%1)").arg(htmlReference(m, newKeyName, newKeyName, newKeyName)) : QString::null);
  }
  pageStart += QString::fromLatin1("</TR>");

  const QString pageEnd = QString::fromLatin1("</TABLE></BODY></HTML>");

  return pageStart + text + pageEnd;
}

const QString CEntryDisplay::cssString( const CEntryDisplay::StyleType type ){
  const QString bgColor = color(BackgroundColor).name();

  const QString textColor = color(TextColor).name();
  const QString highlightColor = color(HighlightedTextColor).name();
  const QString swordRefColor = color(ReferenceColor).name();
  const QString footnotesColor = color(FootnoteColor).name();

  const QString strongsColor = color(StrongsNumberColor).name();
  const QString morphsColor = color(MorphCodeColor).name();
//  const QString lemmaColor = color(LemmaColor).name();

  const QString jesusWordsColor = color(JesusWordColor).name();


  QString text;
  switch(type) {
    case Body:
//      text =  QString::fromLatin1("body {%1; color: %2; font-size: %3pt; font-family: %4;}")
//                .arg(QString::fromLatin1("background-color: %1").arg(bgColor))
//                .arg(textColor)
//                .arg(font(StandardFont).pointSize())
//                .arg(font(StandardFont).family());
      text =  QString::fromLatin1("body {%1; color: %2;}")
                .arg(QString::fromLatin1("background-color: %1").arg(bgColor))
                .arg(textColor);
      break;
    case Link:
      text = QString::fromLatin1("a:link {text-decoration:none;}");
//              .arg(swordRefColor);
      break;
    case LinkHover:
      text = QString::fromLatin1("a:hover {text-decoration:none;}");
      break;
    case Background:
      text = QString::fromLatin1(".background1 {background-color:%1;}")
              .arg(bgColor);
      break;
//    case Background2:
//      text = QString::fromLatin1(".background2 {background-color:%1;}")
//              .arg(bgColor2);
//      break;
    case HighlightedText:
      text = QString::fromLatin1(".highlighted {color: %1;}")
              .arg(highlightColor);
      break;
    case UnicodeText:
//      text = QString::fromLatin1(".unicodetext { font-family: %1; font-size:%2pt; }")
//                .arg(font(UnicodeFont).family())
//                .arg(font(UnicodeFont).pointSize());
      break;
    case Reference:
      text = QString::fromLatin1(".reference { color: %1; font-decoration: none; font-weight:bold; }")
              .arg(swordRefColor);
      break;
    case MorphCode:
      text = QString::fromLatin1(".morphcode {font-size: smaller; color: %1; font-decoration:none;}")
              .arg(morphsColor);
      break;
    case StrongNumber:
      text = QString::fromLatin1(".strongnumber {font-size: smaller; color: %1; font-decoration:none;}")
              .arg(strongsColor);
      break;
    case Lemma:
//      text = QString::fromLatin1(".lemma {font-size: smaller; color: %1; font-decoration:none;}")
//              .arg(lemmaColor);
      break;
    case Footnote:
      text = QString::fromLatin1(".footnote  {font-size: smaller; color: %1; font-style:italic;}")
              .arg(footnotesColor);
      break;
    case FootnotePre:
      text = QString::fromLatin1(".footnotepre {font-size: smaller; color: %1; font-weight: bolder;}")
              .arg(footnotesColor);
      break;
    case Poetry:
      text = QString::fromLatin1(".poetry {font-weight: lighter; alignment: justify;}");
      break;
    case Quotation:
      text = QString::fromLatin1(".quotation {font-style: italic;}");
      break;
    case JesusWords:
      text = QString::fromLatin1(".jesuswords {font-style: italic; color: %1;}")
              .arg(jesusWordsColor);
      break;
    case BookTitle:
      text = QString::fromLatin1(".booktitle {font-weight: x-bold; font-size: x-large; color: %1; margin-top:1mm; margin-bottom:1mm;}")
              .arg(textColor);
      break;
    case SectionTitle:
      text = QString::fromLatin1(".sectiontitle {font-size: larger; font-weight: bold; color: %1;}")
              .arg(textColor);
      break;
    default:      
      break;
  };
  return QString::fromLatin1("\n\n%1\n\n").arg(text);
}

void CEntryDisplay::setDisplayOptions(const CSwordBackend::DisplayOptions options) {
  m_displayOptions = options;
}

/** Returns the right reference text which can be incluced in the HTML */
const QString CEntryDisplay::htmlReference( CSwordModuleInfo* module, const QString& keyName, const QString linkText, const QString& anchorText ) {
  if (linkText.isEmpty())
    return QString::fromLatin1("<A NAME=\"%1\"></A>").arg(anchorText);
  else
    return QString::fromLatin1("<A NAME=\"%1\" HREF=\"%2\">%3</A>")
      .arg(anchorText)
      .arg(CReferenceManager::encodeHyperlink(module->name(), keyName, CReferenceManager::typeFromModule(module->type()) ))
      .arg(linkText);
}

/* ----------------------- new class: CChapterDisplay ------------------- */

/** Returns the rendered text using the modules in the list and using the key parameter. The displayoptions and filter options are used, too. */
const QString CChapterDisplay::text( QPtrList <CSwordModuleInfo> modules, const QString& keyName, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions ) {	
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
  
	for (key.Verse(1); key.Testament() == currentTestament && key.Book() == currentBook && key.Chapter() == currentChapter && ok && !module->module()->Error(); ok = key.next(CSwordVerseKey::UseVerse)) {
    text += entryText(modules, key.key(), keyName);
	}
  return finishText(text, modules, QString::null);
}

/** Renders one entry using the given modules and the key. This makes chapter rendering more easy. */
const QString CChapterDisplay::entryText( QPtrList<CSwordModuleInfo> modules, const QString& keyName, const QString& chosenKey ){
  CSwordVerseKey key(modules.first());
  QString renderedText = (modules.count() > 1) ? QString::fromLatin1("<TR valign=\"top\">") : QString::null;

	// Only insert the table stuff if we are displaying parallel.
  // Otherwise, strip out he table stuff -> the whole chapter will be rendered in one cell!

  //declarations out of the lopp for optimization
  const QString colStyle = QString::fromLatin1("style=\"border-bottom:thin solid black; padding-bottom:2px; padding-top:2px;");
  QString tdStyle;
  QString entry;
  QString keyText;
  
  QFont font;
  bool isRTL;

  const QString lineBreakString = ((modules.count() == 1) && m_displayOptions.lineBreaks) ? QString::fromLatin1("<BR>") : QString::fromLatin1(" ");
  
  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key.module(m);
    key.key(keyName);
    keyText = key.key();
    isRTL = (m->textDirection() == CSwordModuleInfo::RightToLeft);

    tdStyle = colStyle + QString::fromLatin1("%1 %2\"")
      .arg((modules.at()+1 < modules.count()) ? QString::fromLatin1("padding-right: 2mm; border-right:thin solid black;") : QString::null)
      .arg((modules.at()>0 && modules.at()+1 <= modules.count()) ? QString::fromLatin1("padding-left:2mm;") : QString::null);

    font = CBTConfig::get(m->language());
    
    entry =
      QString::fromLatin1("<SPAN %1 STYLE=\"font-family:%2; font-size:%3pt;\"><SPAN dir=\"%4\">%5%6</SPAN></SPAN>")
        .arg((keyText == chosenKey) ? QString::fromLatin1("class=\"highlighted\"") : QString::null)
        .arg(font.family())
        .arg(font.pointSize())
        .arg(isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr"))
        .arg(m_displayOptions.verseNumbers //if we shuld show the verse numbers
            ? QString::fromLatin1("<SPAN STYLE=\"vertical-align:text-top; font-size:%1pt;\">%2</SPAN> ")
                .arg(int(float(font.pointSize()) / 1.5))
                .arg(htmlReference(m, keyText, QString::number(key.Verse()), keyText))
            : htmlReference(m, QString::null, QString::null, keyText) )
        .arg(key.renderedText() + lineBreakString);

  	if (modules.count() == 1)
			renderedText += entry;
  	else
	    renderedText += QString::fromLatin1("<TD class=\"background1\" %1 dir=\"%2\" valign=\"top\">%3</TD>")
                        .arg(tdStyle)
                        .arg(isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr"))
											  .arg(entry);
  }

  if (modules.count() > 1){
		renderedText += QString::fromLatin1("</TR>");
	}

  return renderedText;
}

const QString CChapterDisplay::finishText( const QString text, QPtrList <CSwordModuleInfo> modules, const QString& keyName) {
  util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(modules.first()) );
  key->key(keyName);

  QString css = "table.maintable {width:100%;} td.tableheading {border-bottom: thin solid black;}";
  for (int t = MinType; t <= MaxType; ++t) {
    css += "\t" + cssString( static_cast<CEntryDisplay::StyleType>(t) ) + "\n\n";
  }

  const int columnWidth = (int)((float)100 / (float)modules.count());

  QString pageStart = QString::fromLatin1("<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\"><HTML><HEAD><STYLE type=\"text/css\">%1</STYLE></HEAD><BODY><TABLE cellspacing=\"0\" class=\"maintable\" %2><TR>")
    .arg(css)
    .arg(((modules.count() == 1) && (modules.first()->textDirection() == CSwordModuleInfo::RightToLeft)) ? "dir=\"rtl\"" : "");

  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key->module(m);
    const QString newKeyName = key->key();

    pageStart += QString::fromLatin1("<TD class=\"tableheading\" width=\"%1%\"><CENTER><B>%2</B> %3</CENTER></TD>")
      .arg(columnWidth)
      .arg(m->name())
      .arg(!keyName.isEmpty() ? htmlReference(m, newKeyName, newKeyName, newKeyName) : QString::null);
  }
  pageStart += QString::fromLatin1("</TR>");

  QString pageEnd = QString::fromLatin1("</TABLE></BODY></HTML>");

	if (modules.count() == 1) // render everything into one cell. entryText leaves out the table tags.
		return pageStart  + QString::fromLocal8Bit("<TR><TD>")
                      + text
					    			  + QString::fromLocal8Bit("</TD></TR>") + pageEnd;
  else  // use many cells. entryText inserts the necessary table tags.
	 	return pageStart + text + pageEnd;
}

/* ----------------------- new class: CBookDisplay ------------------- */

/** Returns the rendered text using the modules in the list and using the key parameter. The displayoptions and filter options are used, too. */
const QString CBookDisplay::text( QPtrList <CSwordModuleInfo> modules, const QString& keyName, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions ) {
  backend()->setDisplayOptions( displayOptions );
  backend()->setFilterOptions( filterOptions );

	CSwordBookModuleInfo* book = dynamic_cast<CSwordBookModuleInfo*>(modules.first());

// the number of levels which should be display together, 1 means display no entries together
  const int displayLevel = book->config( CSwordModuleInfo::DisplayLevel ).toInt();

  util::scoped_ptr<CSwordTreeKey> key( dynamic_cast<CSwordTreeKey*>( CSwordKey::createInstance(book) ) );
  key->key(keyName); //set the key to position we'd like to get

  //standard of DisplayLevel, display nothing together
  //if the current key is the root entry don't display anything together!
  if (displayLevel <= 1 || (key->key().isEmpty() || key->key() == "/")) {
    return finishText( entryText(modules, keyName), modules, keyName );
  };

  /**
  * Check whether displaying displayLevel levels together is possible. For this count the childs and parents
  * of the required position
  */

  int possibleLevels = 1; //we start with the default value of displayLevel, which means no entries together
  while( key->parent() && key->key() != "/" && !key->key().isEmpty() ) {//add parents
    ++possibleLevels;
  };
  key->key(keyName); //set the key to the start position
  while( key->firstChild( )) { //add childs
    ++possibleLevels;
  };

  if (possibleLevels < displayLevel) { //too few levels available!
    //display current level, we could also decide to display the available levels together
//    qWarning("too few levels available!");
    return finishText( entryText(modules, keyName), modules, keyName );
  };

  // at this point we're sure that we can display the required levels toogether
  // at the moment we're at the lowest level, so we only have to go up!
  for (int currentLevel = 1; currentLevel < displayLevel; ++currentLevel) { //we start again with 1 == standard of displayLevel
    if (!key->parent()) { //something went wrong althout we checked before! Be safe and return entry's text
      return finishText( entryText(modules, keyName), modules, keyName );
    };
  };

  // no we can display all sub levels together! We checked before that this is possible!
  m_text = entryText(modules,key->key(),0, key->key() == keyName);

  const bool hasToplevelText = !key->strippedText().isEmpty();
  
  key->firstChild(); //go to the first sibling on the same level
  m_chosenKey = keyName;
  printTree(*key, modules, hasToplevelText ? 1 : 0 ); //if the top level entry has text ident the other text
  
	key->key(keyName);
  return finishText(m_text, modules, keyName);
}

/** Renders one entry using the given modules and the key. This makes chapter rendering more easy. */
const QString CBookDisplay::entryText( QPtrList<CSwordModuleInfo> modules, const QString& keyName, const int level, const bool activeKey){
	CSwordBookModuleInfo* book = dynamic_cast<CSwordBookModuleInfo*>(modules.first());
  util::scoped_ptr<CSwordTreeKey> key( dynamic_cast<CSwordTreeKey*>( CSwordKey::createInstance(book) ) );

  key->key(keyName);

  return QString::fromLatin1("<TR><TD STYLE=\"padding-left: %1px;\"><SUP>%2</SUP> %3</TD></TR>")
    .arg(level*30)
    .arg(htmlReference(book, keyName, key->getLocalName(), !key->key().isEmpty() ? key->key() : "/" ))
    .arg(
      QString::fromLatin1("<SPAN %1 STYLE=\"font-family:%2; font-size:%3pt;\">%4</SPAN>")
      .arg(activeKey ? "class=\"highlighted\"" : "")
      .arg(CBTConfig::get(book->language()).family())
      .arg(CBTConfig::get(book->language()).pointSize())
      .arg(key->renderedText())
    );
}

void CBookDisplay::printTree(CSwordTreeKey treeKey, QPtrList<CSwordModuleInfo> modules, const int levelPos){
  m_text += entryText(modules, treeKey.getFullName(), levelPos, (m_chosenKey == treeKey.getFullName()));

  if (treeKey.hasChildren()) { //print tree for the child items
    treeKey.firstChild();
    printTree(treeKey, modules, levelPos+1);
    treeKey.parent();
  }

  if (treeKey.nextSibling()) //print tree for next entry on the same depth
		printTree(treeKey, modules, levelPos);
}

const QString CBookDisplay::finishText( const QString text, QPtrList <CSwordModuleInfo> modules, const QString& keyName){
 	CSwordBookModuleInfo* book = dynamic_cast<CSwordBookModuleInfo*>(modules.first());
  util::scoped_ptr<CSwordTreeKey> key( dynamic_cast<CSwordTreeKey*>( CSwordKey::createInstance(book) ) );
  key->key(keyName);

  QString css = "table.maintable {width:100%;} td.tableheading {border-bottom: thin solid black;}";
  for (int t = MinType; t <= MaxType; ++t) {
    css += "\t" + cssString( static_cast<CEntryDisplay::StyleType>(t) ) + "\n\n";
  }

  const int columnWidth = (int)((float)100 / (float)modules.count());

  QString pageStart = QString::fromLatin1("<HTML><HEAD><STYLE type=\"text/css\">%1</STYLE></HEAD><BODY><TABLE cellspacing=\"0\" class=\"maintable\"><TR>")
    .arg(css);

  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key->module(m);
    const QString newKeyName = !key->key().isEmpty() ? key->key() : "/";
    pageStart += QString::fromLatin1("<TD class=\"tableheading\" width=\"%1%\"><CENTER><B>%2</B> %3</CENTER></TD>")
      .arg(columnWidth)
      .arg(m->name())
      .arg(!newKeyName.isEmpty() ? QString::fromLatin1("(%1)").arg(htmlReference(m, newKeyName, newKeyName, QString::null)) : QString::null);
  }
  pageStart += QString::fromLatin1("</TR>");

  const QString pageEnd = QString::fromLatin1("</TABLE></BODY></HTML>");

  return pageStart + text + pageEnd;
}
