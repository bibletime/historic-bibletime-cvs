//
// C++ Implementation: cprinter
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "cprinter.h"

//BibleTime includes
#include "backend/cdisplaytemplatemgr.h"
#include "backend/cswordversekey.h"

//KDE includes
#include <khtmlview.h>

CPrinter::Item::Item(const QString& key, CSwordModuleInfo* module, const Settings settings) 
	: CDisplayRendering::KeyTreeItem(key, module, settings),
		m_stopKey(QString::null)
{
	Q_ASSERT(module);
	
	//create this item with heading text and one child item which has the content
	m_alternativeContent = QString::fromLatin1("%1 (%2)").arg(key).arg(module->name());
	childList()->append( new KeyTreeItem(key, module, KeyTreeItem::Settings(false, KeyTreeItem::Settings::NoKey)) );
}

CPrinter::Item::Item(const QString& startKey, const QString& stopKey, CSwordModuleInfo* module, const Settings settings)
 : CDisplayRendering::KeyTreeItem(startKey, module, settings),
	 m_stopKey(stopKey)
{
	Q_ASSERT(module);
	
	//use the start and stop key to ceate our child items
	
	if (module->type() == CSwordModuleInfo::Bible) {
		CSwordVerseKey start(module);
		start = startKey;
		
		CSwordVerseKey stop(module);
		stop = stopKey;
		
		if (!key().isEmpty()  && !m_stopKey.isEmpty()) { //we have a range of keys
			bool ok = true;
			
			while (ok && ((start < stop) || (start == stop)) ) { //range
				childList()->append( new KeyTreeItem(start.key(), module, KeyTreeItem::Settings(false, KeyTreeItem::Settings::SimpleKey)) );
				
				ok = start.next(CSwordVerseKey::UseVerse);
			}	
		}
	}
	else if ((module->type() == CSwordModuleInfo::Lexicon) || (module->type() == CSwordModuleInfo::Commentary) ) {		
		childList()->append( new KeyTreeItem(startKey, module, KeyTreeItem::Settings(false, KeyTreeItem::Settings::NoKey)) );
	}
	else if (module->type() == CSwordModuleInfo::GenericBook) {
		childList()->append( new KeyTreeItem(startKey, module, KeyTreeItem::Settings(false, KeyTreeItem::Settings::NoKey)) );
	}
				
	m_alternativeContent = QString::fromLatin1("%1 (%2)")
		.arg((startKey != stopKey) ? QString::fromLatin1("%1 - %2").arg(startKey).arg(stopKey) : startKey)
		.arg(module->name());
}

CPrinter::Item::Item(const CPrinter::Item& i) 
	: KeyTreeItem(i) 
{
	m_stopKey = i.m_stopKey;
	m_alternativeContent = i.m_alternativeContent;
}

/*!
    \fn CPrinter::Item::getAlternativeContent()
 */
const QString& CPrinter::Item::getAlternativeContent() const
{
	return m_alternativeContent;
}

/* Class: CPrinter */

CPrinter::CPrinter(QObject *parent, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions)
 : 	QObject(parent),
 		CDisplayRendering(displayOptions, filterOptions), 
		m_htmlPart(new KHTMLPart(0, 0, this))
{	
	m_filterOptions.footnotes = false;
	m_filterOptions.scriptureReferences = false;
	m_filterOptions.strongNumbers = false;
	m_filterOptions.morphTags = false;
	m_filterOptions.headings = false;

	m_htmlPart->setJScriptEnabled(false);
	m_htmlPart->setJavaEnabled(false);
	m_htmlPart->setMetaRefreshEnabled(false);
	m_htmlPart->setPluginsEnabled(false);
	m_htmlPart->view()->resize(500,500);
}

CPrinter::~CPrinter()
{
	delete m_htmlPart;
}

void CPrinter::printKeyTree( KeyTree& tree ) {
	m_htmlPart->begin();
	m_htmlPart->write(renderKeyTree(tree));
	m_htmlPart->end();
	m_htmlPart->view()->layout();
	m_htmlPart->view()->print();
}

const QString CPrinter::entryLink(const KeyTreeItem& item, CSwordModuleInfo* module)
{
	Q_ASSERT(module);
	
	if (module->type() == CSwordModuleInfo::Bible) {
		CSwordVerseKey vk(module);
		vk = item.key();
		
		switch (item.settings().keyRenderingFace) {
			case Item::Settings::CompleteShort:
				return QString::fromUtf8(vk.getShortText());
			
			case Item::Settings::CompleteLong:
				return vk.key();
			
			case Item::Settings::NoKey:
				return QString::null;
			
			case Item::Settings::SimpleKey: //fall through
			default:
				return QString::number(vk.Verse());
		}
		
	}
	
	return item.key();
}

const QString CPrinter::renderEntry( const KeyTreeItem& i, CSwordKey* ) {
	const CPrinter::Item* printItem = dynamic_cast<const CPrinter::Item*>(&i);	
	Q_ASSERT(printItem);
	
	if (printItem && printItem->hasAlternativeContent()) {
		QString ret = QString::fromLatin1("<div class=\"entry\"><div class=\"rangeheading\">%1</div>").arg(printItem->getAlternativeContent());
		
		if (i.hasChildItems()) {
			KeyTree const * tree = i.childList();
			
			for ( KeyTreeItem* c = tree->first(); c; c = tree->next() ) {
				ret.append( CDisplayRendering::renderEntry( *c ) );
			}
		}

		ret.append("</div>");
		return ret;
	}
	
	return CDisplayRendering::renderEntry(i);
}

const QString CPrinter::finishText(const QString& text, KeyTree& tree)
{
	ListCSwordModuleInfo modules = tree.collectModules();
	
	const CLanguageMgr::Language* const lang = modules.first()->language();
	
	CDisplayTemplateMgr::Settings settings;
	settings.modules = modules;
	settings.pageCSS_ID = "printer";
	settings.langAbbrev = ((modules.count() == 1) && lang->isValid())
		?	lang->abbrev() 
		: "unknown";

	CDisplayTemplateMgr* tMgr = CPointers::displayTemplateManager(); 	
	return tMgr->fillTemplate(CBTConfig::get(CBTConfig::displayStyle), text, settings);
}
