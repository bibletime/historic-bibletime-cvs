/***************************************************************************
                          bibletime_slots.cpp  -  description
                             -------------------
    begin                : Wed Jan 19 2000
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
#include "bibletime.h"
#include "config.h"

#include "backend/cswordversekey.h"

#include "util/ctoolclass.h"

#include "frontend/cmdiarea.h"
#include "frontend/cprofilemgr.h"
#include "frontend/cprofile.h"
#include "frontend/cprofilewindow.h"                       	
#include "frontend/coptionsdialog.h"
#include "frontend/cswordsetupdialog.h"
#include "frontend/cbtconfig.h"
#include "frontend/cinputdialog.h"
#include "frontend/cinfodisplay.h"
#include "frontend/mainindex/cmainindex.h"
#include "frontend/mainindex/cindexitem.h"
#include "frontend/displaywindow/cdisplaywindow.h"
#include "frontend/displaywindow/cbiblereadwindow.h"
#include "frontend/searchdialog/csearchdialog.h"

#include <errno.h>

//QT includes
#include <qprogressdialog.h>
#include <qlistview.h>
#include <qwhatsthis.h>
#include <qvaluelist.h>
#include <qclipboard.h>
#include <qinputdialog.h>

//KDE includes
#include <kaction.h>
#include <kapplication.h>
#include <kaboutkde.h>
#include <kaboutdialog.h>
#include <kbugreport.h>
#include <kaboutapplication.h>
#include <kstandarddirs.h>
#include <kmenubar.h>
#include <kaboutdata.h>
#include <kglobal.h>
#include <klocale.h>
#include <kedittoolbar.h>
#include <kpopupmenu.h>
#include <khelpmenu.h>
#include <ktip.h>

//Sword includes
#include <versekey.h>

/** Opens the optionsdialog of BibleTime. */
void BibleTime::slotSettingsOptions(){
	COptionsDialog *dlg = new COptionsDialog(this, "COptionsDialog", accel());
  connect(dlg, SIGNAL(signalSettingsChanged()), SLOT(slotSettingsChanged()) );
	
	dlg->exec();
	dlg->delayedDestruct();
}

/** Is called when settings in the optionsdialog were changed (ok or apply) */
void BibleTime::slotSettingsChanged(){
 	const QString language = CBTConfig::get(CBTConfig::language);
 	m_backend->booknameLanguage(language);

 	QListViewItemIterator it( m_mainIndex );
 	CItemBase* item = 0;
 	for ( ; it.current(); ++it ) {
 		if ( (item = dynamic_cast<CItemBase*>(it.current())) ) {
      item->update();
 		}
  }

  refreshDisplayWindows();
  refreshProfileMenus();
}

/** Opens the sword setup dialog of BibleTime. */
void BibleTime::slotSwordSetupDialog(){
	InstallationManager::CSwordSetupDialog *dlg = new InstallationManager::CSwordSetupDialog(this, "CSwordSetupDialog");
  connect(dlg, SIGNAL(signalSwordSetupChanged()), SLOT(slotSwordSetupChanged()) );

	dlg->exec();
	dlg->delayedDestruct();
}

/** Is called when settings in the sword setup dialog were changed (ok or apply) */
void BibleTime::slotSwordSetupChanged(){
  /*
    Refresh everything here what might have changed
    these are the mainindex, the searchdialog, the displaywindows
    But at first we have to reset the Sword backend to reload the modules
  */
  m_mainIndex->saveBookmarks();

	CPointers::deleteBackend();
	m_backend = new CSwordBackend();
	CPointers::setBackend(m_backend);
	m_backend->initModules();

  m_mainIndex->reloadSword();

//  refresh display windows
  refreshDisplayWindows();
}

/** Shows the daily tip */
void BibleTime::slotHelpTipOfDay(){
	KTipDialog::setShowOnStart( CBTConfig::get(CBTConfig::tips) );
	KTipDialog::showTip(this, "bibletime/tips", true);
}

/** Is called just before the window menu is ahown. */
void BibleTime::slotWindowMenuAboutToShow(){
	if (!m_windowMenu) {
		return;
  }

	if ( m_windowSaveProfile_action->isPlugged() )
		m_windowSaveProfile_action->unplug(m_windowMenu);
	if ( m_windowLoadProfile_action->isPlugged() )
		m_windowLoadProfile_action->unplug(m_windowMenu);
	if ( m_windowEditProfiles_action->isPlugged() )
		m_windowEditProfiles_action->unplug(m_windowMenu);
	if ( m_windowFullscreen_action->isPlugged() )
		m_windowFullscreen_action->unplug(m_windowMenu);
				
	if ( m_windowCascade_action->isPlugged() )
		m_windowCascade_action->unplug(m_windowMenu);
	if ( m_windowTile_action->isPlugged() )
		m_windowTile_action->unplug(m_windowMenu);
	if ( m_windowArrangementMenu->isPlugged() )
		m_windowArrangementMenu->unplug(m_windowMenu);
	if ( m_windowCloseAll_action->isPlugged() )
		m_windowCloseAll_action->unplug(m_windowMenu);

	m_windowMenu->clear();
			
	m_windowSaveProfile_action->plug(m_windowMenu);	
	m_windowSaveToNewProfile_action->plug(m_windowMenu);	
	m_windowLoadProfile_action->plug(m_windowMenu);
	m_windowEditProfiles_action->plug(m_windowMenu);	
	m_windowMenu->insertSeparator();
	
	m_windowFullscreen_action->plug(m_windowMenu);
	m_windowMenu->insertSeparator();
	
	m_windowArrangementMenu->plug(m_windowMenu);
	m_windowCascade_action->plug(m_windowMenu);
	m_windowTile_action->plug(m_windowMenu);
	m_windowCloseAll_action->plug(m_windowMenu);	
	
	if ( m_mdi->windowList().isEmpty() ) {
		m_windowCascade_action->setEnabled(false);
		m_windowTile_action->setEnabled(false);
		m_windowCloseAll_action->setEnabled(false);
	}
	else if (m_mdi->windowList().count() == 1) {
		m_windowTile_action->setEnabled( false );
		m_windowCascade_action->setEnabled( false );
		m_windowCloseAll_action->setEnabled( true );
		m_windowMenu->insertSeparator();
	} 
	else {
		slotUpdateWindowArrangementActions(0); //update the window tile/cascade states
		m_windowCloseAll_action->setEnabled( true );
		m_windowMenu->insertSeparator();
	}
	
	QWidgetList windows = m_mdi->windowList();
	int i, id;
	for ( i = 0; i < int(windows.count()); ++i ) {
    QString caption = windows.at(i)->caption();
		
		id = m_windowMenu->insertItem(QString::fromLatin1("&%1 ").arg(i+1) + caption,
			this, SLOT(slotWindowMenuActivated( int )) );
		m_windowMenu->setItemParameter( id, i );
	  m_windowMenu->setItemChecked( id, m_mdi->activeWindow() == windows.at(i) );
	}
}

/** This slot is connected with the windowAutoTile_action object */
void BibleTime::slotUpdateWindowArrangementActions( KAction* clickedAction ){
	
	/* If a toggle action was clicked we see if it checked ot unchecked and
	* enable/disable the simple cascade and tile options accordingly 
	*/
	m_windowTile_action->setEnabled( m_windowManualMode_action->isChecked() );
	m_windowCascade_action->setEnabled( m_windowManualMode_action->isChecked() );
	
	if (clickedAction) {
		m_windowManualMode_action->setEnabled( m_windowManualMode_action != clickedAction );
		m_windowAutoTileVertical_action->setEnabled( m_windowAutoTileVertical_action != clickedAction );
		m_windowAutoTileHorizontal_action->setEnabled( m_windowAutoTileHorizontal_action != clickedAction );
		m_windowAutoCascade_action->setEnabled( m_windowAutoCascade_action != clickedAction );
	}
	
	if (clickedAction == m_windowManualMode_action) {
		m_windowAutoTileVertical_action->setChecked(false);
		m_windowAutoTileHorizontal_action->setChecked(false);
		m_windowAutoCascade_action->setChecked(false);
		
		m_mdi->setGUIOption( CMDIArea::Nothing );
	}
	else if (clickedAction == m_windowAutoTileVertical_action) {		
		m_windowManualMode_action->setChecked(false);
		m_windowAutoTileHorizontal_action->setChecked(false);
		m_windowAutoCascade_action->setChecked(false);
		
		m_mdi->setGUIOption( CMDIArea::autoTileVertical );
	}
	else if (clickedAction == m_windowAutoTileHorizontal_action) {
		m_windowManualMode_action->setChecked(false);
		m_windowAutoTileVertical_action->setChecked(false);
		m_windowAutoCascade_action->setChecked(false);
		
		m_mdi->setGUIOption( CMDIArea::autoTileHorizontal );
	}
	else if (clickedAction == m_windowAutoCascade_action) {
		m_windowManualMode_action->setChecked(false);
		m_windowAutoTileHorizontal_action->setChecked(false);
		m_windowAutoTileVertical_action->setChecked(false);
		
		m_mdi->setGUIOption( CMDIArea::autoCascade );
	}
	else if (clickedAction == m_windowCascade_action) {
		m_mdi->setGUIOption( CMDIArea::Nothing );
		m_mdi->myCascade();
	}
	else if (clickedAction == m_windowTile_action) {
		m_mdi->setGUIOption( CMDIArea::Nothing );
 		m_mdi->myTileVertical();
	}
}

void BibleTime::slotManualArrangementMode() {
	slotUpdateWindowArrangementActions( m_windowManualMode_action );
}

/** This slot is connected with the windowAutoTile_action object */
void BibleTime::slotAutoTileHorizontal(){
	slotUpdateWindowArrangementActions( m_windowAutoTileHorizontal_action );
}

/** This slot is connected with the windowAutoTile_action object */
void BibleTime::slotAutoTileVertical(){
	slotUpdateWindowArrangementActions( m_windowAutoTileVertical_action );
}

void BibleTime::slotCascade() { 
	slotUpdateWindowArrangementActions( m_windowCascade_action );
}

void BibleTime::slotTileVertical() {
	slotUpdateWindowArrangementActions( m_windowTile_action );
}

/** This slot is connected with the windowAutoCascade_action object */
void BibleTime::slotAutoCascade(){
	slotUpdateWindowArrangementActions( m_windowAutoCascade_action );
/*	if (m_windowAutoCascade_action->isChecked()) {
		m_windowAutoTileVertical_action->setChecked(false);
		m_windowAutoTileHorizontal_action->setChecked(false);
		m_mdi->setGUIOption( CMDIArea::autoCascade );
	}
	else if (!m_windowAutoTileVertical_action->isChecked()) { //tile and cascade are enabled/disbled in  slotWindowMenuAboutToShow
		m_mdi->setGUIOption( CMDIArea::Nothing );    
  }*/
}

void BibleTime::slotWindowMenuActivated( int id ) {
  if (!m_windowMenu) {
  	return;
	}
  	
  QWidget* const window = m_mdi->windowList().at( id );
  if ( window ) {
		window->setFocus();
	}
}


/** Shows/hides the toolbar */
void BibleTime::slotToggleToolbar(){
	if (m_viewToolbar_action->isChecked()) {
		toolBar("mainToolBar")->show();
	}
	else {
		toolBar("mainToolBar")->hide();
	}
}

/** Shows or hides the groupmanager. */
void BibleTime::slotToggleMainIndex() {
	if (m_viewMainIndex_action->isChecked()) {
		m_mainIndex->parentWidget()->show();
  }
	else {
		m_mainIndex->parentWidget()->hide();
  }
}

/** Shows or hides the groupmanager. */
void BibleTime::slotToggleInfoDisplay() {
	if (m_viewInfoDisplay_action->isChecked()) {
		m_infoDisplay->show();
  }
	else {
		m_infoDisplay->hide();
  }
}

/** Opens a toolbar editor */
void BibleTime::slotSettingsToolbar(){
	KEditToolbar dlg(actionCollection());
	if (dlg.exec()) {
		createGUI();
  }
}

void BibleTime::slotSearchModules() {
  //get the modules of the open windows
  ListCSwordModuleInfo modules;
  
	QWidgetList windows = m_mdi->windowList();
	for ( int i = 0; i < static_cast<int>(windows.count()); ++i ) {
    if (CDisplayWindow* w = dynamic_cast<CDisplayWindow*>(windows.at(i))) {
      ListCSwordModuleInfo windowModules = w->modules();
			
			ListCSwordModuleInfo::iterator end_it = windowModules.end();
			for (ListCSwordModuleInfo::iterator it(windowModules.begin()); it != end_it; ++it) {
        modules.append(*it);
      };
    };
  };
  
  CSearchDialog::openDialog(modules, QString::null);
}

/* Search default Bible slot
 * Call CSearchDialog::openDialog with only the default bible module
 */
void BibleTime::slotSearchDefaultBible() {
  ListCSwordModuleInfo module;
	CSwordModuleInfo* bible = CBTConfig::get(CBTConfig::standardBible);
  if (bible) {
    module.append(bible);
	}	

  CSearchDialog::openDialog(module, QString::null);
}

void BibleTime::openOnlineHelp_Handbook() {
   kapp->invokeHelp("", "bibletime/handbook");
}

void BibleTime::openOnlineHelp_Howto() {
  kapp->invokeHelp("", "bibletime/howto");
}

/** Saves the current settings into the currently activatred profile. */
void BibleTime::saveProfile(int ID){
	m_mdi->setUpdatesEnabled(false);

  KPopupMenu* popup = m_windowSaveProfile_action->popupMenu();
	if ( CProfile* p = m_profileMgr.profile(popup->text(ID)) ) {
		saveProfile(p);
  }

	m_mdi->setUpdatesEnabled(true);	
}

void BibleTime::saveProfile(CProfile* profile){
	if (!profile) {
		return;
  }

	//save mainwindow settings
	storeProfileSettings(profile);
			
	QWidgetList windows = m_mdi->windowList();
	QPtrList<CProfileWindow> profileWindows;
	for (QWidget* w = windows.first(); w; w = windows.next()) {
		CDisplayWindow* displayWindow = dynamic_cast<CDisplayWindow*>(w);
		if (!displayWindow) {
			continue;
    }

		CProfileWindow* profileWindow = new CProfileWindow();
		displayWindow->storeProfileSettings(profileWindow);
		profileWindows.append(profileWindow);
	}
	profile->save(profileWindows);

//clean up memory - delete all created profile windows
	profileWindows.setAutoDelete(true);
	profileWindows.clear();
}

void BibleTime::loadProfile(int ID){	
	KPopupMenu* popup = m_windowLoadProfile_action->popupMenu();
  if ( CProfile* p = m_profileMgr.profile( popup->text(ID) ) ) {
    m_mdi->deleteAll();

    loadProfile(p);
  }
}

void BibleTime::loadProfile(CProfile* p){
	if (!p) {
		return;
	}
    
  QPtrList<CProfileWindow> windows = p->load();

	//load mainwindow setttings
	applyProfileSettings(p);

	m_mdi->setUpdatesEnabled(false);//don't auto tile or auto cascade, this would mess up everything!!

  QWidget* focusWindow = 0;
  
  for (CProfileWindow* w = windows.first(); w; w = windows.next()) {
		const QString key = w->key();
		QStringList usedModules = w->modules();
		
		ListCSwordModuleInfo modules;
		for ( QStringList::Iterator it = usedModules.begin(); it != usedModules.end(); ++it ) {
			if (CSwordModuleInfo* m = m_backend->findModuleByName(*it)) {
				modules.append(m);
      }
    }
    if (!modules.count()) { //are the modules still installed? If not continue wih next session window
      continue;
    }

    //is w->isWriteWindow is false we create a write window, otherwise a read window
    CDisplayWindow* displayWindow = 0;
    if (w->writeWindowType() > 0) { //create a write window
      displayWindow = createWriteDisplayWindow(modules.first(), key, CDisplayWindow::WriteWindowType(w->writeWindowType()) );
		}
    else { //create a read window
      displayWindow = createReadDisplayWindow(modules, key);
		}
    
    if (displayWindow) { //if a window was created initialize it.
      if (w->hasFocus()) {
        focusWindow = displayWindow;
      }

      displayWindow->applyProfileSettings(w);
    };
	}

	m_mdi->setUpdatesEnabled(true);
}

void BibleTime::toggleFullscreen(){
	if (m_windowFullscreen_action->isChecked()) {
 		showFullScreen();
  }
	else if (isVisible()){
    showNormal();
  }
	
	m_mdi->triggerWindowUpdate();
}

void BibleTime::editProfiles(){
	COptionsDialog *dlg = new COptionsDialog(this, "COptionsDialog", accel() );
  connect(dlg, SIGNAL(signalSettingsChanged()), SLOT(slotSettingsChanged()) );
	dlg->showPart(COptionsDialog::ViewProfiles);	
	dlg->exec();

	dlg->delayedDestruct();	
}

/** Saves current settings into a new profile. */
void BibleTime::saveToNewProfile(){
  bool ok = false;
  const QString name = QInputDialog::getText(i18n("Session name:"), i18n("Please enter a name for the new session."), QLineEdit::Normal, QString::null, &ok, this);
  if (ok && !name.isEmpty()) {
    CProfile* profile = m_profileMgr.create(name);
    saveProfile(profile);
  };
  refreshProfileMenus();
}

/** Slot to refresh the save profile and load profile menus. */
void BibleTime::refreshProfileMenus(){
 	//refresh the load profile and save profile menus
	m_profileMgr.refresh();
 	KPopupMenu* savePopup = m_windowSaveProfile_action->popupMenu();
 	KPopupMenu* loadPopup = m_windowLoadProfile_action->popupMenu();
 	savePopup->clear();
 	loadPopup->clear();
 	
	QPtrList<CProfile> profiles = m_profileMgr.profiles();
 	for (CProfile* p = profiles.first(); p; p = profiles.next()) {
		savePopup->insertItem(p->name());
		loadPopup->insertItem(p->name());
 	}
}
