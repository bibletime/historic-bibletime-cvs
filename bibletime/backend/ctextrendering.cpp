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
#include <kurl.h>

using namespace Rendering;

CTextRendering::KeyTreeItem::KeyTreeItem(const QString& key, CSwordModuleInfo const * mod, const Settings settings ) 
	: m_settings( settings ),
		m_moduleList(),
		m_key(key),
		m_childList( 0 )
{
	m_moduleList.append( mod );
// 	m_childList.setAutoDelete(true);
}

CTextRendering::KeyTreeItem::KeyTreeItem(const QString& key, const ListCSwordModuleInfo& mods, const Settings settings ) 
	: m_settings( settings ),
		m_moduleList( mods ),
		m_key( key ),
		m_childList( 0 )
{
// 	m_childList.setAutoDelete(true);
	
}

CTextRendering::KeyTreeItem::KeyTreeItem()
	:	m_settings(),
		m_moduleList(),
		m_key(QString::null),
		m_childList(0)
{
// 	m_childList.setAutoDelete(true);
}

CTextRendering::KeyTreeItem::KeyTreeItem(const KeyTreeItem& i) 
	:	m_moduleList(i.m_moduleList),
		m_key(i.m_key),
		m_childList(0)
{
	if (i.hasChildItems()) {
		m_childList = new KeyTree();
		*m_childList = *(i.childList()); //deep copy
	}
	
	m_settings = i.m_settings;
}

CTextRendering::KeyTreeItem::~KeyTreeItem() {
	delete m_childList;
	m_childList = 0;
}

ListCSwordModuleInfo CTextRendering::KeyTree::collectModules() {
	//collect all modules which are available and used by child items
	ListCSwordModuleInfo modules;

	for (KeyTreeItem* c = first(); c; c = next()) {
		Q_ASSERT(c);
		
		ListCSwordModuleInfo childMods = c->modules();
		
		const ListCSwordModuleInfo::const_iterator c_end = childMods.end();
		for (ListCSwordModuleInfo::const_iterator c_it = childMods.constBegin(); c_it != c_end; ++c_it) {
			if (!modules.contains(*c_it)) {
				modules.append(*c_it);
			}
		}
	}
	 
	return modules;
}

const QString CTextRendering::renderKeyTree( KeyTree& tree ) {
	initRendering();	
	
	ListCSwordModuleInfo modules = tree.collectModules();	
	QString t = QString::null;
	
	//opimization for entries with the same key
	util::scoped_ptr<CSwordKey> key( 
		(modules.count() == 1) ? CSwordKey::createInstance(modules.first()) : 0 
	);
	
	for (KeyTreeItem* c = tree.first(); c; c = tree.next()) {
		if (modules.count() == 1) { //this optimizes the rendering, only one key created for all items
			key->key( c->key() );
			t.append( renderEntry( *c, key) );
		}
		else {
			t.append( renderEntry( *c ) );	
		}
	}
	
	return finishText(t, tree);
}

const QString CTextRendering::renderKeyRange( const QString& start, const QString& stop, const ListCSwordModuleInfo& modules, const QString& highlightKey, const KeyTreeItem::Settings keySettings ) {

	CSwordModuleInfo* module = modules.getFirst();
	util::scoped_ptr<CSwordKey> lowerBound( CSwordKey::createInstance(module) );
	lowerBound->key(start);
	
	util::scoped_ptr<CSwordKey> upperBound( CSwordKey::createInstance(module) );
	upperBound->key(stop);

	sword::SWKey* sw_start = dynamic_cast<sword::SWKey*>(lowerBound.get());
	sword::SWKey* sw_stop = dynamic_cast<sword::SWKey*>(upperBound.get());
	
	if (*sw_start == *sw_stop) { //same key, render single key
		return renderSingleKey(lowerBound->key(), modules);
	}
	else if (*sw_start < *sw_stop) { // Render range
		KeyTree tree;
 		KeyTreeItem::Settings settings = keySettings;
		
		CSwordVerseKey* vk_start = dynamic_cast<CSwordVerseKey*>(lowerBound.get());
		CSwordVerseKey* vk_stop = dynamic_cast<CSwordVerseKey*>(upperBound.get());
		
		bool ok = true;
		while (ok && ((*vk_start < *vk_stop) || (*vk_start == *vk_stop))) {
			//make sure the key given by highlightKey gets marked as current key
			settings.highlight = (!highlightKey.isEmpty() ? (vk_start->key() == highlightKey) : false);
			
			tree.append( new KeyTreeItem(vk_start->key(), modules, settings) );
			
			ok = vk_start->next(CSwordVerseKey::UseVerse);
		}
		
		return renderKeyTree(tree);
	}
	
	return QString::null;
}

const QString CTextRendering::renderSingleKey( const QString& key, const ListCSwordModuleInfo& moduleList, const KeyTreeItem::Settings settings ) {
	KeyTree tree;
	tree.append( new KeyTreeItem(key, moduleList, settings) );
	
	return renderKeyTree(tree);
}


CHTMLExportRendering::CHTMLExportRendering(const CHTMLExportRendering::Settings& settings, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions) 
	: m_displayOptions(displayOptions),
		m_filterOptions(filterOptions),
		m_settings(settings)
{
}

CHTMLExportRendering::~CHTMLExportRendering() {
}

const QString CHTMLExportRendering::renderEntry( const KeyTreeItem& i, CSwordKey* k) {
	ListCSwordModuleInfo modules = i.modules();	
	util::scoped_ptr<CSwordKey> scoped_key( !k ? CSwordKey::createInstance(modules.first()) : 0 );
	
	CSwordKey* key = k ? k : scoped_key;
  
	QString renderedText = (modules.count() > 1) ? QString::fromLatin1("<tr>") : QString::null;

	// Only insert the table stuff if we are displaying parallel.
  // Otherwise, strip out he table stuff -> the whole chapter will be rendered in one cell!

  //declarations out of the loop for optimization
  QString entry;
  QString keyText;
  bool isRTL;
	
	//taken out of the loop for optimization
	QString preverseHeading;
	QString langAttr;
	
  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key->module(m);
    key->key( i.key() );
    keyText = key->key();
    isRTL = (m->textDirection() == CSwordModuleInfo::RightToLeft);
		entry = QString::null;

		langAttr = (m->language()->isValid())
			? QString::fromLatin1("xml:lang=\"%1\" lang=\"%2\"")
				.arg(m->language()->abbrev())//twice the same
				.arg(m->language()->abbrev()) 
			: QString::fromLatin1("xml:lang=\"%1\" lang=\"%2\"")
				.arg(m->module()->Lang()) //again twice times the same
				.arg(m->module()->Lang());
		
		const QString key_renderedText = key->renderedText();
		
		
		if (m_filterOptions.headings) {
			AttributeValue::const_iterator it =  m->module()->getEntryAttributes()["Heading"]["Preverse"].begin();		
			const AttributeValue::const_iterator end = m->module()->getEntryAttributes()["Heading"]["Preverse"].end();
			
			for (; it != end; ++it) {
				preverseHeading = QString::fromUtf8(it->second.c_str());
				
				if (!preverseHeading.isEmpty()) {
					entry += QString::fromLatin1("<div %1 class=\"sectiontitle\">%1</div>")
						.arg(langAttr)
						.arg(preverseHeading);
				}		
			}
		}
		
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
			
 		//keys should normally be left-to-right, but this doesn't apply in all cases
		entry += QString::fromLatin1("<span dir=\"ltr\" class=\"entryname\">%1</span>").arg(entryLink(i, m));
		
		if (m_settings.addText) {
			//entry.append( QString::fromLatin1("<span %1>%2</span>").arg(langAttr).arg(key_renderedText) );
			entry.append( key_renderedText );
		}
		
		if (i.hasChildItems()) {
			KeyTree const * tree = i.childList();
			
			for (KeyTreeItem* c = tree->first(); c; c = tree->next()) {
				entry.append( renderEntry(*c) );
			}
		}
		
		entry.append(m_displayOptions.lineBreaks
			? QString::fromLatin1("</div>\n") 
			: QString::fromLatin1("</span>\n")
		);
		
  	if (modules.count() == 1) {
			renderedText.append( entry );
		}
  	else {
	    renderedText += QString::fromLatin1("<td class=\"%1\" %2 dir=\"%3\">%4</td>\n")
 				.arg( i.settings().highlight ? QString::fromLatin1("currententry") : QString::fromLatin1("entry"))
				.arg( langAttr )
				.arg( isRTL ? QString::fromLatin1("rtl") : QString::fromLatin1("ltr") )
				.arg( entry );
		}
	}
 	
	if (modules.count() > 1) {
		renderedText += QString::fromLatin1("</tr>\n");
	}
	
  return renderedText;	
}

void CHTMLExportRendering::initRendering() {
  CPointers::backend()->setDisplayOptions( m_displayOptions );
  CPointers::backend()->setFilterOptions( m_filterOptions );
}

const QString CHTMLExportRendering::finishText( const QString& text, KeyTree& tree ) {
	ListCSwordModuleInfo modules = tree.collectModules();

	const CLanguageMgr::Language* const lang = modules.first()->language();

	CDisplayTemplateMgr* tMgr = CPointers::displayTemplateManager();
	CDisplayTemplateMgr::Settings settings;
	settings.modules = modules;
	settings.langAbbrev = ((modules.count() == 1) && lang->isValid())
		?	lang->abbrev() 
		: "unknown";

	return tMgr->fillTemplate(i18n("Export"), text, settings);
}

/*!
    \fn CHTMLExportRendering::entryLink( KeyTreeItem& item )
 */
const QString CHTMLExportRendering::entryLink( const KeyTreeItem& item, CSwordModuleInfo* ) {
	return item.key();
}

CDisplayRendering::CDisplayRendering(CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions)
	: CHTMLExportRendering(CHTMLExportRendering::Settings(true), displayOptions, filterOptions) 
{

}

const QString CDisplayRendering::entryLink( const KeyTreeItem& item, CSwordModuleInfo*  module ) {
	QString linkText;
	
	const bool isBible = module && (module->type() == CSwordModuleInfo::Bible);
	CSwordVerseKey vk(module); //only valid for bible modules, i.e. isBible == true
	if (isBible) {
		vk = item.key();
	}
		
	switch (item.settings().keyRenderingFace) {
		case KeyTreeItem::Settings::NoKey: {
			linkText = QString::null;
			break; //no key is valid for all modules
		}
		case KeyTreeItem::Settings::CompleteShort: {
			if (isBible) {
				linkText = QString::fromUtf8(vk.getShortText());
				break;
			}
			//fall through for non-Bible modules
		}
		case KeyTreeItem::Settings::CompleteLong: {
			if (isBible) {
				linkText = vk.key();
				break;
			}
			//fall through for non-Bible modules
		}
		case KeyTreeItem::Settings::SimpleKey: {
			if (isBible) {
				linkText = QString::number(vk.Verse());
				break;
			}
			//fall through for non-Bible modules
		}
		default: { //default behaviour to return the passed key
			linkText = item.key();
			break;
		}
	}	
	
  if (linkText.isEmpty()) {
    return QString::fromLatin1("<a name=\"%1\" />")
			.arg( keyToHTMLAnchor(item.key()) );
  }
  else {
    return QString::fromLatin1("<a name=\"%1\" href=\"%2\">%3</a>\n")
      .arg( keyToHTMLAnchor(item.key()) )
      .arg(
				CReferenceManager::encodeHyperlink(
					module->name(), 
					item.key(), 
					CReferenceManager::typeFromModule(module->type())
				)
			)
			.arg(linkText);
  }
	
	return QString::null;
}

const QString CDisplayRendering::keyToHTMLAnchor(const QString& key) {
	QString ret = key;
	ret = ret.stripWhiteSpace().remove(QRegExp("[^A-Za-z0-9]+"));
	ret = ret.remove(QRegExp("^\\d+|"));

	return ret;
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
	
	const CLanguageMgr::Language* const lang = modules.first()->language();
	
	CDisplayTemplateMgr* tMgr = CPointers::displayTemplateManager();
	CDisplayTemplateMgr::Settings settings;
	settings.modules = modules;
	settings.langAbbrev = ((modules.count() == 1) && lang->isValid())
		?	lang->abbrev() 
		: QString::null;

	return tMgr->fillTemplate(CBTConfig::get(CBTConfig::displayStyle), oldText, settings);
}



CPlainTextExportRendering::CPlainTextExportRendering(const CPlainTextExportRendering::Settings& settings, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions)
	: CHTMLExportRendering(settings, displayOptions, filterOptions)
{
}

CPlainTextExportRendering::~CPlainTextExportRendering() {
}

const QString CPlainTextExportRendering::renderEntry( const KeyTreeItem& i ) {
	if (!m_settings.addText) {
		return QString::fromLatin1("%1\n").arg(i.key());
	}
		
	ListCSwordModuleInfo modules = i.modules();	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(modules.first()) );
	QString renderedText = QString::fromLatin1("%1:\n").arg(i.key());
		
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

const QString CPlainTextExportRendering::finishText( const QString& oldText, KeyTree& ) {
	return oldText;
}

