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
#include "resource.h"

//frontend includes
#include "frontend/chtmldialog.h"
#include "frontend/ctoolclass.h"
#include "frontend/cmdiarea.h"
#include "frontend/displaywindow/cdisplaywindow.h"
#include "frontend/displaywindow/creadwindow.h"
#include "frontend/keychooser/ckeychooser.h"
#include "frontend/cbtconfig.h"
#include "frontend/cpointers.h"

//backend includes
#include "backend/cswordmoduleinfo.h"
#include "backend/cswordbiblemoduleinfo.h"
#include "backend/cswordcommentarymoduleinfo.h"
#include "backend/cswordlexiconmoduleinfo.h"
#include "backend/cswordbookmoduleinfo.h"
#include "backend/cswordversekey.h"
#include "backend/cswordldkey.h"

//printing includes
#include "printing/cprinter.h"

//Qt includes
#include <qsplitter.h>

//KDE includes
#include <kaction.h>
#include <kapplication.h>
#include <kconfig.h>
#include <kcmdlineargs.h>
#include <klocale.h>
#include <kaccel.h>
#include <kmenubar.h>
#include <ktoolbar.h>
#include <krandomsequence.h>

BibleTime::BibleTime() : KMainWindow(0,0, WType_TopLevel /*| WDestructiveClose*/) {
	m_initialized = false;
	m_moduleList  = 0;
	m_progress = 0;
	m_currentProfile = 0;
	
	m_keyAccel = accel(); //new KAccel(this);

	connect(KApplication::kApplication(), SIGNAL(lastWindowClosed()), SLOT(lastWindowClosed()));

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
	setAutoSaveSettings(QString::fromLatin1("MainWindow"), false);

	initDefaultModules();
}

BibleTime::~BibleTime() {
	saveSettings();
}

/** Saves the properties of BibleTime to the application wide configfile  */
void BibleTime::saveSettings(){
	if (m_mdi)
		m_mdi->saveSettings();	
	if (m_keyAccel)
		m_keyAccel->writeSettings();

 	CBTConfig::set(CBTConfig::toolbar, m_viewToolbar_action->isChecked());
 	CBTConfig::set(CBTConfig::mainIndex, m_viewMainIndex_action->isChecked());

 	if (m_viewMainIndex_action->isChecked())	//only save changes when the groupmanager is visible
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
		if (CProfile* p = m_profileMgr.startupProfile())
			saveProfile(p);
	}
}

/** Reads the settings from the configfile and sets the right properties. */
void BibleTime::readSettings(){
	applyMainWindowSettings(KGlobal::config(), QString::fromLatin1("MainWindow"));
	
	m_keyAccel->readSettings(KGlobal::config());

 	m_viewToolbar_action->setChecked( CBTConfig::get(CBTConfig::toolbar) );
 	slotToggleToolbar();
		
 	m_viewMainIndex_action->setChecked( CBTConfig::get(CBTConfig::mainIndex) );
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
CDisplayWindow* BibleTime::createDisplayWindow(ListCSwordModuleInfo modules, const QString& key) {
//  qWarning("BibleTime::createDisplayWindow: key is %s", key.latin1());
  kapp->setOverrideCursor( waitCursor );

 	CDisplayWindow* displayWindow = CDisplayWindow::createReadInstance(modules, m_mdi);	
  if (displayWindow) {
  	displayWindow->init(key);
		displayWindow->show();
	}
	kapp->restoreOverrideCursor();	
	return displayWindow;
}


/** Creates a new presenter in the MDI area according to the type of the module. */
CDisplayWindow* BibleTime::createDisplayWindow(CSwordModuleInfo* module, const QString& key) {
	ListCSwordModuleInfo list;
	list.append(module);
	
	return createDisplayWindow(list, key);		
}

/** Refreshes all presenters.*/
void BibleTime::refreshPresenters() {
	unsigned int index;				
	for ( index = 0; index < m_mdi->windowList().count(); index++) {
		CDisplayWindow* window = dynamic_cast<CDisplayWindow*>(m_mdi->windowList().at(index));
		if (window)
   		window->refresh();
	}
}

/** Called before quit. */
bool BibleTime::queryExit(){
//	qWarning("BibleTime::queryExit()");
	return true;

  if (!m_initialized)
  	return false;
	saveSettings();
	return true;
}

/** Called before a window is closed */
bool BibleTime::queryClose(){
//	qWarning("BibleTime::queryClose()");
	return true;

	bool ret = true;
	for ( unsigned int index = 0; index < m_mdi->windowList().count(); ++index) {
		if (CDisplayWindow* window = dynamic_cast<CDisplayWindow*>(m_mdi->windowList().at(index)))
   		ret = window->queryClose() && ret;
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

/** Restores the workspace if the flag for this is set in the config. */
void BibleTime::restoreWorkspace(){	
	if (CProfile* p = m_profileMgr.startupProfile())
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

/** Processes the commandline options given to BibleTime. */
void BibleTime::processCommandline(){
	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();

  if (CBTConfig::get(CBTConfig::restoreWorkspace) && !args->isSet("ignore-startprofile"))
    restoreWorkspace();
  QString bibleKey = args->getOption("open-default-bible");
  if (!bibleKey.isNull() && args->isSet("open-default-bible")) {
    CSwordModuleInfo* bible = CPointers::backend()->findModuleByDescription(CBTConfig::get(CBTConfig::standardBible));
    Q_ASSERT(bible);
    if (bibleKey == "<random>") {
//      qWarning("Open #random# key in default bible!");
      CSwordVerseKey vk(0);
      const int maxIndex = 32400;

      KRandomSequence rs;
      int newIndex = rs.getLong(maxIndex);
      vk.setPosition(TOP);
      vk.Index(newIndex);
      bibleKey = vk.key();
    }
//    else {
//      qWarning("Open given key!");
//    }
    createDisplayWindow(bible, bibleKey);
  }
}
