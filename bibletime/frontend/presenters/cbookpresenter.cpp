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
//#include "cdisplaysettingsbutton.h"
#include "../ctoolclass.h"
#include "../chtmlwidget.h"
//#include "../keychooser/ckeychooser.h"
//#include "../../ressource.h"
//#include "../../backend/cswordbiblemoduleinfo.h"
//#include "../../backend/cswordversekey.h"
//#include "../../backend/chtmlchapterdisplay.h"
#include "../../backend/cswordbackend.h"
//#include "../../backend/creferencemanager.h"
//#include "../cprofile.h"
//#include "../cprofilewindow.h"

CBookPresenter::CBookPresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent=0, const char *name=0 )
	: CSwordPresenter(useModules, importantClasses, parent,name)
{
	initView();
	show();
	initConnections();
}

CBookPresenter::~CBookPresenter(){
}

/** Initializes the interface of this presenter. */
void CBookPresenter::initView(){
	m_mainToolBar = new KToolBar(this);
	m_keyChooser = CKeyChooser::createInstance(m_moduleList.first(), 0, m_mainToolBar);
	m_mainToolBar->insertWidget(0,m_keyChooser->sizeHint().width(),m_keyChooser);
	m_mainToolBar->setItemAutoSized(0);

	addToolBar(m_mainToolBar);			
	
	m_htmlWidget = new CHTMLWidget(m_important, true, this);	
	setCentralWidget(m_htmlWidget);
}

/** Initializes the Signal / Slot connections */
void CBookPresenter::initConnections(){
//	connect(m_htmlWidget, SIGNAL(referenceClicked(const QString&)),
//		this, SLOT(lookup(const QString&))); 	
 	connect(m_keyChooser, SIGNAL(keyChanged(CSwordKey*)),
 		this, SLOT(lookup(CSwordKey*)));
//	connect(m_popup, SIGNAL(aboutToShow()),
//		SLOT(popupAboutToShow()));
	connect(m_moduleChooserBar, SIGNAL( sigChanged() ),
		SLOT(modulesChanged() ));
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

void CBookPresenter::lookup(CSwordKey*) {

}
