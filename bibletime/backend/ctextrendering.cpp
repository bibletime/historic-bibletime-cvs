//
// C++ Implementation: ctextrendering
//
// Description:
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "ctextrendering.h"

//BibleTime includes
#include "backend/cswordkey.h"
#include "backend/cswordversekey.h"
#include "backend/cswordmoduleinfo.h"
#include "backend/cdisplaytemplatemgr.h"
#include "backend/creferencemanager.h"

#include "util/scoped_resource.h"
#include "util/ctoolclass.h"

//Sword includes
#include <swkey.h>

//Qt includes
#include <qregexp.h>

//KDE includes
#include <klocale.h>

CTextRendering::KeyTreeItem::KeyTreeItem(const QString& key, CSwordModuleInfo const * mod, const Settings settings ) 
	: m_settings(settings),
		m_moduleList(),
		m_key(key),
		m_childList( 0 )
{
	m_moduleList.append( mod );
}

CTextRendering::KeyTreeItem::KeyTreeItem(const QString& key, ListCSwordModuleInfo& mods, const Settings settings ) 
	: m_settings(settings),
		m_moduleList( mods ),
		m_key(key),
		m_childList( 0 )
{
	
}

CTextRendering::KeyTreeItem::KeyTreeItem()
	: m_key(QString::null),
		m_childList( 0 )
{
}


CTextRendering::KeyTreeItem::~KeyTreeItem() {
	delete m_childList;
}

CTextRendering::KeyTree* CTextRendering::KeyTreeItem::childList() const {
	if (!m_childList) {
		m_childList = new KeyTree();
	}
		
	return m_childList;
};

ListCSwordModuleInfo CTextRendering::KeyTree::collectModules() {
	//collect all modules which are available and used by child items
	ListCSwordModuleInfo modules;
	KeyTree::const_iterator it;
	for (it = begin(); it != end(); ++it) {
		ListCSwordModuleInfo childMods = (*it).modules();
		for (ListCSwordModuleInfo::const_iterator c_it = childMods.begin(); c_it != childMods.end(); ++c_it) {
			if (!modules.contains(*c_it)) {
				modules.append(*c_it);
			}
		}
	}
	
	return modules;
}

CTextRendering::CTextRendering() {
}


CTextRendering::~CTextRendering() {

}

const QString CTextRendering::renderKeyTree( KeyTree& tree ) {
	initRendering();	
	
	QString t;
	for (KeyTree::const_iterator it = tree.begin(); it != tree.end(); ++it) {
		t += renderEntry( *it );	
	}
	
	return finishText(t, tree);
}

const QString CTextRendering::renderKeyRange( const QString& start, const QString& stop, ListCSwordModuleInfo modules ) {
	CSwordModuleInfo* module = modules.first();
	util::scoped_ptr<CSwordKey> lowerBound( CSwordKey::createInstance(module) );
	lowerBound->key(start);
	
	util::scoped_ptr<CSwordKey> upperBound( CSwordKey::createInstance(module) );
	upperBound->key(stop);

	sword::SWKey* sw_start = dynamic_cast<sword::SWKey*>(lowerBound.get());
	sword::SWKey* sw_stop = dynamic_cast<sword::SWKey*>(upperBound.get());
	
	Q_ASSERT(sw_start && sw_stop);
	
	if (*sw_start == *sw_stop) { //same key, render single key
		return renderSingleKey(lowerBound->key(), modules);
	}
	else if (*sw_start < *sw_stop) { // Render range
		KeyTree tree;
		KeyTreeItem::Settings settings;
		
		CSwordVerseKey* vk_start = dynamic_cast<CSwordVerseKey*>(lowerBound.get());
		CSwordVerseKey* vk_stop = dynamic_cast<CSwordVerseKey*>(upperBound.get());
		
		Q_ASSERT(vk_start && vk_stop);
//		qWarning("render range: %s - %s", vk_start->key().latin1(), vk_stop->key().latin1());
		
		while ((*vk_start < *vk_stop) || (*vk_start == *vk_stop)) {
			tree.append( KeyTreeItem(vk_start->key(), modules,settings) );
			
			vk_start->next(CSwordVerseKey::UseVerse);
		}
		
		return renderKeyTree(tree);
	}
	
	return QString::null;
}

const QString CTextRendering::renderSingleKey( const QString& key, ListCSwordModuleInfo moduleList ) {
	KeyTree tree;
	KeyTreeItem::Settings settings;
	tree += KeyTreeItem(key, moduleList, settings);
	
	return renderKeyTree(tree);
}


CHTMLExportRendering::CHTMLExportRendering(const CHTMLExportRendering::Settings& settings, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions) 
	: m_settings(settings),
		m_displayOptions(displayOptions),
		m_filterOptions(filterOptions)
{

}

CHTMLExportRendering::~CHTMLExportRendering() {

}

const QString CHTMLExportRendering::renderEntry( const KeyTreeItem& i ) {
	ListCSwordModuleInfo modules = i.modules();	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(modules.first()) );
  
	QString renderedText = (modules.count() > 1) ? QString::fromLatin1("<tr>") : QString::null;

//	qWarning("renderEntry  %s", i.key().latin1());
	// Only insert the table stuff if we are displaying parallel.
  // Otherwise, strip out he table stuff -> the whole chapter will be rendered in one cell!

  //declarations out of the loop for optimization
  QString entry;
  QString keyText;
  bool isRTL;
	
	//taken out of the loop for optimization
	QString preverseHeading;
	CLanguageMgr::Language lang;
	QString langAttr;
	
  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key->module(m);
    key->key( i.key() );
    keyText = key->key();
    isRTL = (m->textDirection() == CSwordModuleInfo::RightToLeft);
		entry = QString::null;

		lang =  m->language();
		langAttr = lang.isValid()
			? QString::fromLatin1("lang=\"%1\"").arg(lang.abbrev()) 
			: QString::null;
		
		const QString key_renderedText = key->renderedText();
		int pvHeading = 0;
		do { //add sectiontitle before we add the versenumber
			preverseHeading = QString::fromUtf8(
m->module()->getEntryAttributes()["Heading"]["Preverse"][QString::number(pvHeading++).latin1()].c_str());
			if (!preverseHeading.isEmpty()) {
				entry += QString::fromLatin1("<div %1 class=\"sectiontitle\">%1</div>")
					.arg(langAttr)
					.arg(preverseHeading);
			}
			else {
				break;
			}
		} while (true);

		entry += QString::fromLatin1("<%1 %2 %3 dir=\"%4\">") //linebreaks = div, without = span
    	.arg(m_displayOptions.lineBreaks ? QString::fromLatin1("div") : QString::fromLatin1("span"))
			.arg((modules.count() == 1) //insert only the class if we're not in a td
				? (i.settings().highlight 
					? QString::fromLatin1("class=\"currententry\"") 
					: QString::fromLatin1("class=\"entry\"")) 
				: QString::null
			)
			.arg(langAttr)
			.arg(isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr"));

		entry += QString::fromLatin1("<span dir=\"%1\" class=\"entryname\">%2</span>")
			.arg(isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr"))
			.arg(m_displayOptions.verseNumbers 
				? entryLink(i, m)
				: QString::null
			);
		
		if (m_settings.addText) {
			entry += key_renderedText; //key->renderedText();
		}
		
		if (i.childList()) {
			KeyTree const * tree = i.childList();
			for ( KeyTree::const_iterator it = tree->begin(); it != tree->end(); ++it ) {
				entry += renderEntry( *it );
			}
		}
		
		entry += (m_displayOptions.lineBreaks 
			? QString::fromLatin1("</div>") 
			: QString::fromLatin1("</span>"));
		
  	if (modules.count() == 1) {
			renderedText += entry;
		}
  	else {
	    renderedText += QString::fromLatin1("<td class=\"%1\" %2 dir=\"%3\">%4</td>")
 				.arg(i.settings().highlight ? QString::fromLatin1("currententry") : QString::fromLatin1("entry"))
				.arg(langAttr)
				.arg(isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr"))
				.arg(entry);
		}
	}
 	
	if (modules.count() > 1) {
		renderedText += QString::fromLatin1("</tr>");
	}
	
  return renderedText;	
}

void CHTMLExportRendering::initRendering() {
//  CPointers::backend()->setDisplayOptions( m_displayOptions );
  CPointers::backend()->setFilterOptions( m_filterOptions );
}

const QString CHTMLExportRendering::finishText( const QString& text, KeyTree& tree ) {
	ListCSwordModuleInfo modules = tree.collectModules();
	
	CLanguageMgr::Language lang = modules.first()->language();
	
	CDisplayTemplateMgr tMgr;
	CDisplayTemplateMgr::Settings settings;
	settings.modules = modules;
	settings.langAbbrev = ((modules.count() == 1) && lang.isValid())
		?	lang.abbrev() 
		: "unknown";

	return tMgr.fillTemplate(i18n("Export"), text, settings);
}


/*!
    \fn CHTMLExportRendering::entryLink( KeyTreeItem& item )
 */
const QString CHTMLExportRendering::entryLink( const KeyTreeItem& item, CSwordModuleInfo*  module ) {
//	Q_ASSERT(module);		
	return item.key();
}

CDisplayRendering::CDisplayRendering(CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions)
	: CHTMLExportRendering(CHTMLExportRendering::Settings(true), displayOptions, filterOptions) 
{

}


const QString CDisplayRendering::entryLink( const KeyTreeItem& item, CSwordModuleInfo*  module ) {
//	Q_ASSERT(module);		
	QString linkText;
	
	if (module && module->type() == CSwordModuleInfo::Bible) {
		CSwordVerseKey vk(module);
		vk = item.key();
		
		linkText = QString::number(vk.Verse());
	}
	else {
		linkText = item.key();
	}
	
  if (linkText.isEmpty()) {
    return QString::fromLatin1("<a name=\"%1\"/>").arg(item.key());
  }
  else {
    return QString::fromLatin1("<a name=\"%1\" href=\"%2\">%3</a>")
      .arg(item.key())
      .arg(
				CReferenceManager::encodeHyperlink(module->name(), 
				item.key(), 
				CReferenceManager::typeFromModule(module->type()) )
			)
      .arg(linkText);
  }
	
	return QString::null;
}


const QString CDisplayRendering::finishText( const QString& oldText, KeyTree& tree ) {
	ListCSwordModuleInfo modules = tree.collectModules();
	
	
	//marking words is very slow, we have to find a better solution	

/*
	//mark all words by spans
	
	QString text = oldText; 
	
	QRegExp re("(\\b)(?=\\w)"); //word begin marker
	int pos = text.find(re, 0);

	while (pos != -1) { //word begin found		
		//qWarning("found word at %i in %i", pos, text.length());
		int endPos = pos + 1;
		if (!CToolClass::inHTMLTag(pos+1, text)) { //the re has a positive look ahead which matches one char before the word start
			//qWarning("matched %s", text.mid(pos+1, 4).latin1());
			
			//find end of word and put a marker around it
			endPos = text.find(QRegExp("\\b|[,.:]"), pos+1);
			if ((endPos != -1) && !CToolClass::inHTMLTag(endPos, text) && (endPos - pos >= 3)) { //reuire wordslonger than 3 chars
				text.insert(endPos, "</span>");
				text.insert(pos, "<span class=\"word\">");
				
				endPos += 26;
			}
		}		
		pos = text.find(re, endPos);		
	}
*/
	
	CLanguageMgr::Language lang = modules.first()->language();
	
	CDisplayTemplateMgr tMgr;
	CDisplayTemplateMgr::Settings settings;
	settings.modules = modules;
	settings.langAbbrev = ((modules.count() == 1) && lang.isValid())
		?	lang.abbrev() 
		: QString::null;

	return tMgr.fillTemplate(CBTConfig::get(CBTConfig::displayStyle), oldText, settings);
}



CPlainTextExportRendering::CPlainTextExportRendering(const CPlainTextExportRendering::Settings& settings, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions)
	: CHTMLExportRendering(settings, displayOptions, filterOptions)
{
}

CPlainTextExportRendering::~CPlainTextExportRendering() {
}

const QString CPlainTextExportRendering::renderEntry( const KeyTreeItem& i ) {
	if (!m_settings.addText) {
		return QString("%1\n").arg(i.key());
	}
		
	ListCSwordModuleInfo modules = i.modules();	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(modules.first()) );
	QString renderedText = QString("%1:\n").arg(i.key());
		
  QString entry;
  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key->module(m);
    key->key( i.key() );
		
		entry += key->strippedText() + "\n\n";
		//entry.simplifyWhiteSpace();
		//entry.replace(QRegExp("^\\b+"), "");
		
		renderedText += entry;
	}
	return renderedText;	
}

const QString CPlainTextExportRendering::finishText( const QString& oldText, KeyTree& tree ) {
	return oldText;
}
