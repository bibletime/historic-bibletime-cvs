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
}

CPrinter::Item::Item(const QString& startKey, const QString& stopKey, CSwordModuleInfo* module, const Settings settings)
 : CDisplayRendering::KeyTreeItem(startKey, module, settings),
	 m_stopKey(stopKey)
{
	//use the start and stop key to ceate our child items
	Q_ASSERT(childList());
	
	CSwordVerseKey start(module);
	start = startKey;
	
	CSwordVerseKey stop(module);
	stop = stopKey;
	
	if (!key().isEmpty()  && !m_stopKey.isEmpty()) {
		while ((start < stop) || (start == stop) ) {
			KeyTreeItem i(start.key(), module, KeyTreeItem::Settings());
			childList()->append( i );
			
			start.next(CSwordVerseKey::UseVerse);
		}	
	}	
}

CPrinter::Item::Item(const CPrinter::Item& i) 
	: KeyTreeItem(i) 
{
	m_stopKey = i.m_stopKey;
}


/* Class: CPrinter */

CPrinter::CPrinter(QObject *parent, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions)
 : 	QObject(parent),
 		CDisplayRendering(displayOptions, filterOptions), 
		m_htmlPart(new KHTMLPart(0, 0, this))
{	
}

CPrinter::~CPrinter()
{
	delete m_htmlPart;
}

void CPrinter::printKeyTree( KeyTree& tree ) {
	m_htmlPart->begin();
	m_htmlPart->write(renderKeyTree(tree));
	m_htmlPart->end();
	
	m_htmlPart->view()->print();
}

const QString CPrinter::renderKeyTree( KeyTree& tree ) {
	return CDisplayRendering::renderKeyTree(tree);
}

const QString CPrinter::entryLink(const KeyTreeItem& item, CSwordModuleInfo* module)
{
	return item.key();
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


