/***************************************************************************
                          cbookpresenter.cpp  -  description
                             -------------------
    begin                : Thu Jan 24 2002
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
#include "cbookpresenter.h"
#include "../keychooser/ckeychooser.h"
#include "cmodulechooserbar.h"
#include "../ctoolclass.h"
#include "../chtmlwidget.h"
#include "../../backend/cswordtreekey.h"
#include "../../backend/chtmlentrydisplay.h"
#include "../../backend/cswordbackend.h"

//KDE includes
#include <kaccel.h>

CBookPresenter::CBookPresenter(ListCSwordModuleInfo useModules, QWidget *parent, const char *name )
	: CSwordPresenter(useModules,parent,name)
{
	m_key = dynamic_cast<CSwordTreeKey*>(CSwordKey::createInstance( useModules.first() ));
	
	//please note that book display windows only support one module!
	initView();
	show();
	initConnections();
	
	m_key->key("/");
	lookup(m_key);
}

CBookPresenter::~CBookPresenter(){
	if (m_key)
		delete m_key;
	m_key = 0;
}

/** Initializes the interface of this presenter. */
void CBookPresenter::initView(){
	m_mainToolBar = new KToolBar(this);
	m_keyChooser = CKeyChooser::createInstance(m_moduleList.first(), m_key, m_mainToolBar);
	m_mainToolBar->insertWidget(0,m_keyChooser->sizeHint().width(),m_keyChooser);
	m_mainToolBar->setItemAutoSized(0);

	addToolBar(m_mainToolBar);			
	
	m_htmlWidget = new CHTMLWidget(true, this);	
	setCentralWidget(m_htmlWidget);
}

/** Initializes the Signal / Slot connections */
void CBookPresenter::initConnections(){
	connect(m_htmlWidget, SIGNAL(referenceClicked(const QString&, const QString&)),
		this, SLOT(lookup(const QString&, const QString&))); 	
	connect(m_htmlWidget, SIGNAL(referenceDropped(const QString&)),
		this, SLOT(referenceDropped(const QString&)));

 	connect(m_keyChooser, SIGNAL(keyChanged(CSwordKey*)),
 		this, SLOT(lookup(CSwordKey*)));
		
//	connect(m_popup, SIGNAL(aboutToShow()),
//		SLOT(popupAboutToShow()));
//	connect(m_moduleChooserBar, SIGNAL( sigChanged() ),
//		SLOT(modulesChanged() ));
//	connect(m_displaySettingsButton, SIGNAL( sigChanged() ),	
//		SLOT(optionsChanged() ));
}

void CBookPresenter::modulesChanged(){
  m_moduleList = m_moduleChooserBar->getModuleList();
  if (!m_moduleList.count()) {
  	close();
  }
  else {
//		m_displaySettingsButton->reset(m_moduleList);
//    refreshFeatures()
//	  m_key->module(m_moduleList.first());
//	  m_keyChooser->setModule(m_moduleList.first());	
		m_htmlWidget->setModules(m_moduleList);
	  	
//	  lookup(m_key);
	}
}

void CBookPresenter::lookup(CSwordKey* key) {
	qWarning("CBookPresenter::lookup(CSwordKey*)");
	CSwordTreeKey* treeKey = dynamic_cast<CSwordTreeKey*>(key);
	ASSERT(treeKey);
  qWarning(treeKey->key().latin1());

	setUpdatesEnabled(false);	

//	m_moduleList.first()->module()->SetKey(*treeKey);
	CSwordModuleInfo* m = m_moduleList.first();
	if (((SWKey*)*m->module()) != treeKey) {
		m->module()->SetKey(treeKey);
	  qWarning("CBookPresenter::lookup have set key!");
	}
	
		
	if (m->getDisplay()) {
		m->getDisplay()->Display( m );
	  qWarning("CBookPresenter::lookup: Displayed!");
	  qWarning(m->getDisplay()->getHTML().latin1());
#warning "Comment out?"
		m_htmlWidget->setText(m->getDisplay()->getHTML());
	}	
	if (m_key != treeKey) {
		m_key->key(treeKey->key());
	  qWarning("CBookPresenter::lookup: setKey");			
	}
	setUpdatesEnabled(true);
	setCaption( windowCaption() );

  qWarning("CBookPresenter::lookup: finished!");			
}

/** No descriptions */
void CBookPresenter::lookup( const QString& module, const QString& key){
	CSwordModuleInfo* m = backend()->findModuleByName(module);
	if (m && m_moduleList.containsRef(m)) {
		if (!key.isEmpty())
			m_key->key(key);
		m_keyChooser->setKey(m_key); //the key chooser does send an update signal	
	}
	else {
		emit lookupInModule(module, key);
	}
}

/** Initializes keyboard accelerators. */
void CBookPresenter::initAccels(){
	CSwordPresenter::initAccels();
	m_accel->setConfigGroup("Book window");	
//	m_accel->insertItem(i18n("Next entry"), "Next entry", 0);
//	m_accel->connectItem("Next entry", this, SLOT(nextEntry()));
//		
//	m_accel->insertItem(i18n("Previous entry"), "Previous entry", 0);		
//	m_accel->connectItem("Previous entry", this, SLOT(previousEntry()));
		
	m_accel->readSettings();
}

void CBookPresenter::insertKeyboardActions(KAccel* a){
	a->setConfigGroup("Book window");	
//	a->insertItem(i18n("Next entry"), "Next entry", 0);
//	a->insertItem(i18n("Previous entry"), "Previous entry", 0);	
}
