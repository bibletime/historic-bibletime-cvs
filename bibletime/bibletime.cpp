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

//frontend includes
#include "frontend/chtmldialog.h"
#include "frontend/cmdiarea.h"
#include "frontend/kstartuplogo.h"
#include "frontend/mainindex/cmainindex.h"
#include "frontend/displaywindow/cdisplaywindow.h"
#include "frontend/displaywindow/creadwindow.h"
#include "frontend/displaywindow/cwritewindow.h"
#include "frontend/keychooser/ckeychooser.h"
#include "frontend/cbtconfig.h"

#include "util/ctoolclass.h"
#include "util/cpointers.h"

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

BibleTime::BibleTime()
  : KMainWindow(0,0, WType_TopLevel /*| WDestructiveClose*/),
	  m_initialized(false),
	  m_moduleList(0),
	  m_progress(0),
	  m_currentProfile(0),
    m_splitter(0),
    m_mdi(0),
    m_profileMgr(),
    m_backend(0),
    m_printer(0),
    m_mainIndex(0)
{
	setHelpMenuEnabled(false);

  initBackends();
	initPrinter();
	initView();
	initActions();

	createGUI("bibletimeui.rc");
	initMenubar();
	initConnections();

	readSettings();

	setPlainCaption("BibleTime " VERSION);

  // we don't save the geometry, it's stored in the startup profile
  setAutoSaveSettings(QString::fromLatin1("MainWindow"), true);
}

/** Saves the properties of BibleTime to the application wide configfile  */
void BibleTime::saveSettings(){
	if (m_mdi) {
		m_mdi->saveSettings();
  }

  accel()->writeSettings();

 	CBTConfig::set(CBTConfig::toolbar, m_viewToolbar_action->isChecked());
 	CBTConfig::set(CBTConfig::mainIndex, m_viewMainIndex_action->isChecked());

 	if (m_viewMainIndex_action->isChecked()) {	//only save changes when the groupmanager is visible
 		CBTConfig::set(CBTConfig::splitterSizes, m_splitter->sizes());
  }

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
		if (CProfile* p = m_profileMgr.startupProfile()) {
			saveProfile(p);
    }
	}
}

/** Reads the settings from the configfile and sets the right properties. */
void BibleTime::readSettings(){
	accel()->readSettings(KGlobal::config());

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
CDisplayWindow* BibleTime::createReadDisplayWindow(ListCSwordModuleInfo modules, const QString& key) {
  kapp->setOverrideCursor( waitCursor );

  qWarning("will create now displayWindow (bibletime)");
  CDisplayWindow* displayWindow = CDisplayWindow::createReadInstance(modules, m_mdi);
  qWarning("created displayWindow (bibletime)");
  if ( displayWindow ) {
  	displayWindow->init(key);
		displayWindow->show();
	}

  kapp->restoreOverrideCursor();  
	return displayWindow;
}


/** Creates a new presenter in the MDI area according to the type of the module. */
CDisplayWindow* BibleTime::createReadDisplayWindow(CSwordModuleInfo* module, const QString& key) {
	ListCSwordModuleInfo list;
	list.append(module);

	return createReadDisplayWindow(list, key);
}

CDisplayWindow* BibleTime::createWriteDisplayWindow(CSwordModuleInfo* module, const QString& key, const CDisplayWindow::WriteWindowType& type) {
  kapp->setOverrideCursor( waitCursor );

	ListCSwordModuleInfo modules;
	modules.append(module);

  CDisplayWindow* displayWindow = CDisplayWindow::createWriteInstance(modules, m_mdi, type);
  if ( displayWindow ) {
  	displayWindow->init(key);
		displayWindow->show();
	}

  kapp->restoreOverrideCursor();
	return displayWindow;
}

/** Refreshes all presenters.*/
void BibleTime::refreshDisplayWindows() {
	unsigned int index;				
	for ( index = 0; index < m_mdi->windowList().count(); index++) {
		if (CDisplayWindow* window = dynamic_cast<CDisplayWindow*>(m_mdi->windowList().at(index))) {
   		window->reload();
    }
	}
}

/** Called before quit. */
bool BibleTime::queryExit(){
  if (!m_initialized) {
  	return false;
  }
	saveSettings();
	return true;
}

/** Called before a window is closed */
bool BibleTime::queryClose(){
  bool ret = true;
	for ( unsigned int index = 0; index < m_mdi->windowList().count(); ++index) {
		if (CDisplayWindow* window = dynamic_cast<CDisplayWindow*>(m_mdi->windowList().at(index))) {
   		ret = ret && window->queryClose();
    }
	}	
	return ret;
}

/** Reimplementation used for sessions management. */
void BibleTime::saveProperties(KConfig* /*myConfig*/){

}

/** Reimplementation used for session management. */
void BibleTime::readProperties(KConfig* /*myConfig*/){

}

/** Restores the workspace if the flag for this is set in the config. */
void BibleTime::restoreWorkspace(){	
	if (CProfile* p = m_profileMgr.startupProfile()) {
		loadProfile(p);
  }
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

  if (CBTConfig::get(CBTConfig::crashedLastTime) || CBTConfig::get(CBTConfig::restoreWorkspace)) {
    if (!CBTConfig::get(CBTConfig::crashedTwoTimes) && !args->isSet("ignore-session")) { //restore workspace if it crashed ony once
      restoreWorkspace();
    }
  }

  //open every time a module if the command line option was set.
  if ( args->isSet("open-default-bible") && !CBTConfig::get(CBTConfig::crashedLastTime) && !CBTConfig::get(CBTConfig::crashedTwoTimes)) { //restore workspace if it crashed ony once
    QString bibleKey = args->getOption("open-default-bible");    
    CSwordModuleInfo* bible = CPointers::backend()->findModuleByDescription(CBTConfig::get(CBTConfig::standardBible));
    if (bibleKey == "<random>") {
      CSwordVerseKey vk(0);
      const int maxIndex = 32400;

      KRandomSequence rs;
      int newIndex = rs.getLong(maxIndex);
      vk.setPosition(sword::TOP);
      vk.Index(newIndex);
      bibleKey = vk.key();
    }
    createReadDisplayWindow(bible, bibleKey);
//    if (isVisible())
      m_mdi->myTile();//we are sure only one window is open, which should be displayed fullscreen in the working area
  }
}

void BibleTime::polish(){
  m_initialized = true;
  KMainWindow::polish();
  applyMainWindowSettings(KGlobal::config(), QString::fromLatin1("MainWindow"));
}

