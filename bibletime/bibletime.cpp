/***************************************************************************
                          bibletime.cpp  -  The main class of BibleTime
                             -------------------
    begin                : Mit Jan 19 12:57:15 CET 2000
    copyright            : (C) 2000 by The BibleTime Team
    email                : Info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//local includes
#include "config.h"
#include "bibletime.h"
#include "ressource.h"

//frontend includes
#include "frontend/chtmldialog.h"
#include "frontend/ctoolclass.h"
#include "frontend/cmdiarea.h"
#include "frontend/presenters/cswordpresenter.h"
#include "frontend/presenters/cbiblepresenter.h"
#include "frontend/presenters/ccommentarypresenter.h"
#include "frontend/presenters/clexiconpresenter.h"
#include "frontend/presenters/cbookpresenter.h"
#include "frontend/keychooser/ckeychooser.h"
#include "frontend/cbtconfig.h"
#include "frontend/cpointers.h"

//backend includes
#include "backend/cswordmoduleinfo.h"
#include "backend/cswordbiblemoduleinfo.h"
#include "backend/cswordcommentarymoduleinfo.h"
#include "backend/cswordlexiconmoduleinfo.h"
#include "backend/cswordbookmoduleinfo.h"
#include "backend/chtmlentrydisplay.h"
#include "backend/chtmlchapterdisplay.h"
#include "backend/cswordversekey.h"
#include "backend/cswordldkey.h"

//printing includes
#include "printing/cprinter.h"

//Qt includes
#include <qsplitter.h>

//KDE includes
#include <kaction.h>
#include <klocale.h>
#include <kaccel.h>
#include <kmenubar.h>
#include <ktoolbar.h>
#include <kaccel.h>

BibleTime::BibleTime() : KMainWindow() {
	m_initialized = false;
	m_moduleList  = 0;
	m_progress = 0;
	m_currentProfile = 0;
	
	m_keyAccel = new KAccel(this);

	connect(kapp, SIGNAL(lastWindowClosed()), SLOT(lastWindowClosed()));

	initBackends();
	initPrinter();
	
	initView();
	initActions();
	setHelpMenuEnabled(false);
	createGUI("bibletimeui.rc");
	initMenubar();
	initConnections();
	readSettings();

	setPlainCaption("BibleTime " VERSION);
}

BibleTime::~BibleTime() {
	saveSettings();
}

/** Saves the properties of BibleTime to the application wide configfile  */
void BibleTime::saveSettings(){
	saveMainWindowSettings(KGlobal::config());
	if (m_mdi)
		m_mdi->saveSettings();	
	if (m_keyAccel)
		m_keyAccel->writeSettings();

 	CBTConfig::set(CBTConfig::toolbar, m_viewToolbar_action->isChecked());
 	CBTConfig::set(CBTConfig::mainIndex, m_viewGroupManager_action->isChecked());

 	if (m_viewGroupManager_action->isChecked())	//only save changes when the groupmanager is visible
 		CBTConfig::set(CBTConfig::splitterSizes, m_splitter->sizes());

 	if (m_windowAutoTile_action->isChecked())	{
 		CBTConfig::set(CBTConfig::autoTile, true);
 		CBTConfig::set(CBTConfig::autoCascade, false);	
 	}
 	else if ( m_windowAutoTile_action->isChecked() ) {
		CBTConfig::set(CBTConfig::autoTile, false);
 		CBTConfig::set(CBTConfig::autoCascade, true);	
 	}
 	else {
		CBTConfig::set(CBTConfig::autoTile, false);
 		CBTConfig::set(CBTConfig::autoCascade, false);	
 	}

	if ( CBTConfig::get(CBTConfig::restoreWorkspace) ) {
		CProfile* p = m_profileMgr.startupProfile();
		if (p)
			saveProfile(p);
	}
}

/** Reads the settings from the configfile and sets the right properties. */
void BibleTime::readSettings(){
	qDebug("BibleTime::readSettings()");
	
//	applyMainWindowSettings(m_config);
	
	m_keyAccel->readSettings(KGlobal::config());

 	m_viewToolbar_action->setChecked( CBTConfig::get(CBTConfig::toolbar) );
 	slotToggleToolbar();
		
 	m_viewGroupManager_action->setChecked( CBTConfig::get(CBTConfig::mainIndex) );
 	slotToggleGroupManager();
		
 	m_splitter->setSizes( CBTConfig::get(CBTConfig::splitterSizes) );		

 	if ( CBTConfig::get(CBTConfig::autoTile) ) {
 		m_windowAutoTile_action->setChecked( true );
 		m_windowAutoCascade_action->setChecked( false );
 		m_mdi->setGUIOption( CMDIArea::autoTile );
 	}
 	else if ( CBTConfig::get(CBTConfig::autoCascade) ) {
 		m_windowAutoCascade_action->setChecked(true);
 		m_windowAutoTile_action->setChecked(false);
 		m_mdi->setGUIOption( CMDIArea::autoCascade );
 	}
 	else {
 		m_mdi->setGUIOption( CMDIArea::Nothing );		
 		m_windowAutoTile_action->setChecked(false);
 		m_windowAutoCascade_action->setChecked(false);		
 	}
}

/** Creates a new presenter in the MDI area according to the type of the module. */
CSwordPresenter* BibleTime::createNewSwordPresenter(ListCSwordModuleInfo modules, const QString& key) {
	kapp->setOverrideCursor( waitCursor );
	
	CSwordPresenter* presenter = 0;
	switch (modules.first()->type()) {
		case CSwordModuleInfo::Bible:
			presenter = new CBiblePresenter(modules, m_mdi);
			break;
		case CSwordModuleInfo::Commentary:
			presenter = new CCommentaryPresenter(modules, m_mdi);
			break;
		case CSwordModuleInfo::Lexicon:
			presenter = new CLexiconPresenter(modules, m_mdi);
			break;
		case CSwordModuleInfo::GenericBook:			
			presenter = new CBookPresenter(modules, m_mdi);
			break;
		default:
			presenter = 0;
			qWarning("unknown module type");
			break;
	}	
	if (presenter) {
		qWarning("CREATE NEW PRESENTER: %s", key.latin1());
		connect(presenter, SIGNAL(lookupInLexicon(const QString&, const QString&)),
			m_mdi, SLOT(lookupInLexicon(const QString&, const QString&)));				
		connect(presenter, SIGNAL(lookupInModule(const QString&, const QString&)),
			m_mdi, SLOT(lookupInModule(const QString&, const QString&)));							
		connect(presenter, SIGNAL(closePresenter(CSwordPresenter*)),
			m_mdi, SLOT(closePresenter(CSwordPresenter*)));							
		if (presenter->isA("CBiblePresenter")) {
			connect(presenter->keyChooser(), SIGNAL(keyChanged(CSwordKey*)),
				m_mdi, SLOT(syncCommentaries(CSwordKey*)));		
		}
		presenter->lookup(modules.first()->name(),key);		
	}
			
	kapp->restoreOverrideCursor();
	presenter->setFocus();
	
	return presenter;
}


/** Creates a new presenter in the MDI area according to the type of the module. */
CSwordPresenter* BibleTime::createNewSwordPresenter(CSwordModuleInfo* module, const QString& key) {
	ListCSwordModuleInfo list;
	list.append(module);
	
	return createNewSwordPresenter(list, key);		
}

/** Refreshes all presenters.*/
void BibleTime::refreshPresenters() {
	unsigned int index;				
	for ( index = 0; index < m_mdi->windowList().count(); index++) {
		CSwordPresenter* myPresenter = dynamic_cast<CSwordPresenter*>(m_mdi->windowList().at(index));
		if (myPresenter)
   		myPresenter->refresh();
	}
}

/** Called before quit. */
bool BibleTime::queryExit(){
  if (!m_initialized)
  	return false;
	saveSettings();
	return true;
}

/** Called before a window is closed */
bool BibleTime::queryClose(){
//	qWarning("BibleTime::queryClose()");
	bool ret = true;
	for ( int index = 0; index < m_mdi->windowList().count(); ++index) {
		CSwordPresenter* myPresenter = dynamic_cast<CSwordPresenter*>(m_mdi->windowList().at(index));
		if (myPresenter)
   		ret = myPresenter->queryClose() && ret;
	}	
	return ret;
}

/** No descriptions */
void BibleTime::show(){
	KMainWindow::show();	
	//if we show BibleTime for the first time we are ready for processing
	//but not before this point.
	m_initialized = true;
}

/** Reimplementation used for sessions management. */
void BibleTime::saveProperties(KConfig* /*myConfig*/){

}

/** Reimplementation used for session management. */
void BibleTime::readProperties(KConfig* /*myConfig*/){

}

/** Restores the workspace if the flaf for this is set in the config. */
void BibleTime::restoreWorkspace(){	
	CProfile* p = m_profileMgr.startupProfile();
	if (p)
		loadProfile(p);
}

/** Sets the caption of the mainwindow */
void BibleTime::setCaption( const QString& ){
	KMainWindow::setPlainCaption( KApplication::kApplication()->makeStdCaption( m_mdi->currentApplicationCaption() ) );
}

/** Sets the plain caption of the main window */
void BibleTime::setPlainCaption( const QString& ){
	KMainWindow::setPlainCaption( KApplication::kApplication()->makeStdCaption( m_mdi->currentApplicationCaption() ) );
}
