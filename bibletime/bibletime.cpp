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

//own includes
#include "config.h"
#include "bibletime.h"
#include "frontend/chtmldialog.h"
#include "ressource.h"
#include "frontend/ctoolclass.h"
#include "frontend/cmdiarea.h"
#include "backend/cswordmoduleinfo.h"
#include "backend/cswordbiblemoduleinfo.h"
#include "backend/cswordcommentarymoduleinfo.h"
#include "backend/cswordlexiconmoduleinfo.h"
#include "backend/chtmlentrydisplay.h"
#include "backend/chtmlchapterdisplay.h"
#include "backend/cswordversekey.h"
#include "backend/cswordldkey.h"
#include "frontend/presenters/cswordpresenter.h"
#include "frontend/presenters/cbiblepresenter.h"
#include "frontend/presenters/ccommentarypresenter.h"
#include "frontend/presenters/clexiconpresenter.h"
#include "frontend/keychooser/ckeychooser.h"

//Qt includes
#include <qsplitter.h>

//KDE includes
#include <kaction.h>
#include <klocale.h>
#include <kconfig.h>
#include <kaccel.h>
#include <kmenubar.h>
#include <ktoolbar.h>
#include <kaccel.h>

BibleTime::BibleTime() : KMainWindow() {
	m_initialized = false;
	m_moduleList  = 0;
	m_progress = 0;
	m_currentProfile = 0;

	m_config = KGlobal::config();
	connect(kapp, SIGNAL(lastWindowClosed()), SLOT(lastWindowClosed()));

	m_important = new CImportantClasses();
	initBackends();
	initPrinter();
	initView();
	initActions();
	setHelpMenuEnabled(false);
	createGUI("bibletimeui.rc");
	initMenubar();
	initKeyAccels();
	initConnections();
	readSettings();

	setPlainCaption("BibleTime " VERSION);
}

BibleTime::~BibleTime() {
	saveSettings();
}

/** Saves the properties of BibleTime to the application wide configfile  */
void BibleTime::saveSettings(){
	saveMainWindowSettings(m_config);
	if (m_mdi)
		m_mdi->saveSettings();	
	if (m_keyAccel)
		m_keyAccel->writeSettings();

	KConfigGroupSaver groupSaver(m_config,"General");	
// 	m_config->writeEntry("showFootnotes", m_viewFootnotes_action->isChecked());
// 	m_config->writeEntry("showStrongs", m_viewStrongs_action->isChecked());
// 	m_config->writeEntry("showMorphTags", m_viewMorphTags_action->isChecked());
// 	m_config->writeEntry("showHeadings", m_viewHeadings_action->isChecked());
						
 	m_config->writeEntry("show toolbar", m_viewToolbar_action->isChecked());
 	m_config->writeEntry("show main index", m_viewGroupManager_action->isChecked());

 	if (m_viewGroupManager_action->isChecked())	//only save changes when the groupmanager is visible
 		m_config->writeEntry("splitterSizes", m_splitter->sizes());

	m_config->setGroup("MDI"); 		
 	if (m_windowAutoTile_action->isChecked())	{
 		m_config->writeEntry("autoTile", true);
 		m_config->writeEntry("autoCascade", false);	
 	}
 	else if ( m_windowAutoTile_action->isChecked() ) {
			m_config->writeEntry("autoTile", false);
 		m_config->writeEntry("autoCascade", true);	
 	}
 	else {
			m_config->writeEntry("autoTile", false);
 		m_config->writeEntry("autoCascade", false);	
 	}

	m_config->setGroup("Startup");
	if (m_config->readBoolEntry("restore workspace", false)) {
		CProfile* p = m_profileMgr.startupProfile();
		if (p)
			saveProfile(p);
	}
}

/** Reads the settings from the configfile and sets the right properties. */
void BibleTime::readSettings(){
	qDebug("BibleTime::readSettings()");
	
	applyMainWindowSettings(m_config);
	
	m_keyAccel->readSettings(m_config);
	KConfigGroupSaver groupsaver(m_config, "General");
// 	m_viewFootnotes_action->setChecked(m_config->readBoolEntry("showFootnotes",true));
// 	slotToggleFootnotes();
//	
// 	m_viewStrongs_action->setChecked(m_config->readBoolEntry("showStrongs",false));
// 	slotToggleStrongs();
//
// 	m_viewHeadings_action->setChecked(m_config->readBoolEntry("showHeadings", true));
// 	slotToggleHeadings();
//
// 	m_viewMorphTags_action->setChecked(m_config->readBoolEntry("showMorphTags", false));
// 	slotToggleMorphTags();
				
 	m_viewToolbar_action->setChecked(m_config->readBoolEntry("show toolbar", true));
 	slotToggleToolbar();
		
 	m_viewGroupManager_action->setChecked( m_config->readBoolEntry("show main index", true) );
 	slotToggleGroupManager();
		
 	m_splitter->setSizes( m_config->readIntListEntry("splitterSizes") );		

 	m_config->setGroup("MDI");
 	if (m_config->readBoolEntry("autoTile", true)) {
 		m_windowAutoTile_action->setChecked( true );
 		m_windowAutoCascade_action->setChecked( false );
 		m_mdi->setGUIOption( CMDIArea::autoTile );
 	}
 	else if ( m_config->readBoolEntry("autoCascade", false) ) {
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
	switch (modules.first()->getType()) {
		case CSwordModuleInfo::Bible:
			presenter = new CBiblePresenter(modules, m_important, m_mdi);
			break;
		case CSwordModuleInfo::Commentary:
			presenter = new CCommentaryPresenter(modules, m_important, m_mdi);
			break;
		case CSwordModuleInfo::Lexicon:
			presenter = new CLexiconPresenter(modules, m_important, m_mdi);
			break;
		default:
			qWarning("unknown module type");
	}	
	if (presenter) {
		presenter->lookup(key);
		connect(presenter, SIGNAL(lookupInLexicon(const QString&, const QString&)),
			m_mdi, SLOT(lookupInLexicon(const QString&, const QString&)));				
		connect(presenter, SIGNAL(closePresenter(CSwordPresenter*)),
			m_mdi, SLOT(closePresenter(CSwordPresenter*)));							
		if (presenter->isA("CBiblePresenter")) {
			connect(presenter->getKeyChooser(), SIGNAL(keyChanged(CSwordKey*)),
				m_mdi, SLOT(syncCommentaries(CSwordKey*)));		
		}
	}
			
	kapp->restoreOverrideCursor();
	presenter->setFocus();
	
	return dynamic_cast<CSwordPresenter*>(presenter);
}


/** Creates a new presenter in the MDI area according to the type of the module. */
CSwordPresenter* BibleTime::createNewSwordPresenter(CSwordModuleInfo* module, const QString& key) {
	ListCSwordModuleInfo list;
	list.append(module);
	
	return createNewSwordPresenter(list, key);		
}

/** Refreshes all presenter supporting at least in of the features given as parameter.*/
void BibleTime::refreshPresenters( int useFeatures ) {
	/*
	*	Iterate through all features and test their support by the psenters.
	*	If the feature is supported refresh the presenter and continue.
	*/		
	unsigned int index;				
	for ( index = 0; index < m_mdi->windowList().count(); index++) {
		CSwordPresenter* myPresenter = dynamic_cast<CSwordPresenter*>(m_mdi->windowList().at(index));
		if (myPresenter)
   		myPresenter->refresh(useFeatures);
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
	return true;
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
