/***************************************************************************
                          cswordpresenter.cpp  -  description
                             -------------------
    begin                : Sun Mar 18 2001
    copyright            : (C) 2001 by The BibleTime team
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

#include "cswordpresenter.h"
#include "../chtmlwidget.h"
#include "../../printing/cprintitem.h"
#include "../../printing/cprinter.h"

//Qt includes
#include <qpopupmenu.h>

CSwordPresenter::CSwordPresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent, const char *name ) : CPresenter(parent,name) {	
	m_moduleList = useModules;
	
	m_important = importantClasses;
	
	m_htmlWidget = 0;
	m_keyChooser = 0;
	m_mainToolBar = 0;
	m_moduleChooserBar = 0;
	m_popup = 0;
	m_features = 0;

	m_lexiconPopup = new QPopupMenu(this);
	for (m_important->swordBackend->getModuleList()->first(); m_important->swordBackend->getModuleList()->current(); m_important->swordBackend->getModuleList()->next()) {
		if (m_important->swordBackend->getModuleList()->current()->getType() == CSwordModuleInfo::Lexicon) {
			m_lexiconPopup->insertItem( QString::fromLocal8Bit(m_important->swordBackend->getModuleList()->current()->module()->Name()) );
		}
	}	
	connect(m_lexiconPopup, SIGNAL(activated(int)),this, SLOT(lookupWord(int)));
		
	refreshFeatures();
}

CSwordPresenter::~CSwordPresenter(){
}

/** Returns the features used by this presenter. */
int CSwordPresenter::getFeatures(){
	return m_features;
}

/** Refreshes the presenter depending on the events given as parameter. */
void CSwordPresenter::refresh( const int events ){	
}

/** Prints the key given as parameter. */
void CSwordPresenter::printKey(CKey* start, CKey* stop, CSwordModuleInfo* module) {
	CPrintItem* printItem = new CPrintItem();	
	printItem->setModule(module);	
	printItem->setStartKey(start);
	printItem->setStopKey(stop);
	
	m_important->printer->addItemToQueue(printItem);
}

/** Refreshes the supported features. */
void CSwordPresenter::refreshFeatures(){
	m_features = 0;
	
	for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
		if (m_moduleList.current()->supportsFeature( CSwordBackend::strongNumbers )){
			if (m_features != 0)
				m_features |= strongNumbers;
			else
				m_features = strongNumbers;
		}
		if (m_moduleList.current()->supportsFeature( CSwordBackend::footnotes )){
			if (m_features != 0)
				m_features |= footnotes;
			else
				m_features = footnotes;
		}
	}
}

void CSwordPresenter::lookupWord(int moduleID){
	const QString module = m_lexiconPopup->text(moduleID);	
	const QString text = m_htmlWidget->selectedText();
	if (!text.isEmpty())
		emit lookupInLexicon(text,module);
}

/** Look up the key given as parameter. */
void CSwordPresenter::lookup(const QString&){
}

/** Returns the listof used modules. */
ListCSwordModuleInfo& CSwordPresenter::getModuleList() {
	return m_moduleList;
}

/** Returns the used keychooser object of this presenter. */
CKeyChooser* CSwordPresenter::getKeyChooser() const {
	return m_keyChooser;
}
