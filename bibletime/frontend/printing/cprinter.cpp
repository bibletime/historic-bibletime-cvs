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
	//create this item with heading text and one child item which has the content
	m_alternativeContent = QString::fromLatin1("%1").arg(key);
	childList()->append( new KeyTreeItem(key, module, KeyTreeItem::Settings()) );
}

CPrinter::Item::Item(const QString& startKey, const QString& stopKey, CSwordModuleInfo* module, const Settings settings)
 : CDisplayRendering::KeyTreeItem(startKey, module, settings),
	 m_stopKey(stopKey)
{
	//use the start and stop key to ceate our child items
// 	Q_ASSERT(childList());
	
	if (module->type() == CSwordModuleInfo::Bible) {
		CSwordVerseKey start(module);
		start = startKey;
		
		CSwordVerseKey stop(module);
		stop = stopKey;
		
		if (!key().isEmpty()  && !m_stopKey.isEmpty()) {
			while ((start < stop) || (start == stop) ) {
				childList()->append( new KeyTreeItem(start.key(), module, KeyTreeItem::Settings()) );
				
				start.next(CSwordVerseKey::UseVerse);
			}	
		}
	}
	else if ((module->type() == CSwordModuleInfo::Lexicon) || (module->type() == CSwordModuleInfo::Commentary) ) {		
		childList()->append( new KeyTreeItem(startKey, module, KeyTreeItem::Settings()) );
	}
	else if (module->type() == CSwordModuleInfo::GenericBook) {
		childList()->append( new KeyTreeItem(startKey, module, KeyTreeItem::Settings()) );
	}
				
	m_alternativeContent = QString::fromLatin1("%1 - %2").arg(startKey).arg(stopKey);	
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
	
	m_htmlPart->setJScriptEnabled(false);
	m_htmlPart->setJavaEnabled(false);
	m_htmlPart->setMetaRefreshEnabled(false);
	m_htmlPart->setPluginsEnabled(false);
	m_htmlPart->view()->resize(500,500);
/*	m_htmlPart->view()->setMarginWidth(20);
	m_htmlPart->view()->setMarginHeight(20);*/
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
		
		return QString::number(vk.Verse());
	}
	
	return item.key();
}

const QString CPrinter::renderEntry( const KeyTreeItem& i) {
	const CPrinter::Item* printItem = dynamic_cast<const CPrinter::Item*>(&i);	
	Q_ASSERT(printItem);
	if (printItem && printItem->hasAlternativeContent()) 
	{
		QString ret = QString::fromLatin1("<div class=\"entry\"><div class=\"rangeheading\">%1</div>").arg(printItem->getAlternativeContent());
		
		if (i.hasChildItems()) {
			KeyTree const * tree = i.childList();
			KeyTree::const_iterator end = tree->end();
			
			for ( KeyTree::const_iterator it = tree->begin(); it != end; ++it ) {
				ret += CDisplayRendering::renderEntry( **it );
			}
		}

		ret += "</div>";
		
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

	CDisplayTemplateMgr tMgr;	
	return tMgr.fillTemplate(CBTConfig::get(CBTConfig::displayStyle), text, settings);
}
