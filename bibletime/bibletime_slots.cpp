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
#include "ressource.h"
#include "frontend/ctoolclass.h"
//#include "frontend/ctipwindow.h"
#include "frontend/cmdiarea.h"
#include "frontend/cprofilemgr.h"
#include "frontend/cprofile.h"
#include "frontend/cprofilewindow.h"
#include "backend/cswordversekey.h"
#include "backend/chtmlentrydisplay.h"
#include "backend/chtmlchapterdisplay.h"
#include "printing/cprinter.h"
#include "frontend/chtmldialog.h"
#include "frontend/presenters/cswordpresenter.h"
#include "frontend/groupmanager/cgroupmanager.h"
#include "frontend/coptionsdialog.h"
#include "frontend/cbtconfig.h"
#include "config.h"

#include <errno.h>

//QT includes
#include <qprogressdialog.h>
#include <qlistview.h>
#include <qwhatsthis.h>
#include <qvaluelist.h>
#include <qclipboard.h>
#include <qxembed.h>

//KDE includes
#include <kaction.h>
#include <kaboutkde.h>
#include <kaboutdialog.h>
#include <kbugreport.h>
#include <kaboutapplication.h>
#include <kstddirs.h>
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

/** Closes the current apploication window */
void BibleTime::slotFileQuit(){
	saveSettings();		
	// close the first window, the list makes the next one the first again.
	// This ensures that queryClose() is called on each window to ask for closing
	
	KMainWindow* w;

	if(memberList){
		for(w = memberList->first(); w; w = memberList->first()){
			// only close the window if the closeEvent is accepted. If the user presses Cancel on the saveModified() dialog,
			// the window and the application stay open.
			if(!w->close())
				break;
		}
	}		
	kapp->quit();
}

/** Opens the optionsdialog of BibleTime. */
void BibleTime::slotSettingsOptions(){
	COptionsDialog *dlg = new COptionsDialog(this, "COptionsDialog", m_keyAccel);
  connect(dlg, SIGNAL(signalSettingsChanged()), SLOT(slotSettingsChanged()) );
	
	dlg->exec();
	dlg->delayedDestruct();
}

/** Is called when settings in the optionsdialog have been changed (ok or apply) */
void BibleTime::slotSettingsChanged(){

 		const QString language = CBTConfig::get(CBTConfig::language);
 		m_backend->setBooknameLanguage(language);		
 		//refresh the bookmark items in the groupmanager		
 		QListViewItemIterator it( m_groupmanager );
 		CGroupManagerItem* item = 0;
 		for ( ; it.current(); ++it ) {
 			if ( (item = dynamic_cast<CGroupManagerItem*>(it.current())) ) {
 				if (item->type() == CGroupManagerItem::Bookmark) {
						CSwordVerseKey* vKey = dynamic_cast<CSwordVerseKey*>(item->getBookmarkKey());
						if ( vKey ) {
							vKey->setLocale( (const char*)m_backend->getCurrentBooknameLanguage().local8Bit());
							item->update();
						}
 				}
 			}
 		}			
// 	}
 	for ( unsigned int index = 0; index < m_mdi->windowList().count(); index++) {
 		CSwordPresenter* myPresenter = dynamic_cast<CSwordPresenter*>(m_mdi->windowList().at(index));
 		if (myPresenter)
 			myPresenter->refresh();
 	}

 	//refresh the load profile and save profile menus
	m_profileMgr.refresh();
 	KPopupMenu* loadPopup = m_windowLoadProfile_action->popupMenu();
 	KPopupMenu* savePopup = m_windowSaveProfile_action->popupMenu();	
 	loadPopup->clear();
 	savePopup->clear();
 	QList<CProfile> profiles = m_profileMgr.profiles();  	
 	for (CProfile* p = profiles.first(); p; p = profiles.next()) {
			savePopup->insertItem(p->name());			
			loadPopup->insertItem(p->name());
 	}
}


/** Shows the daily tip */
void BibleTime::slotHelpTipOfDay(){
	KTipDialog::setShowOnStart( CBTConfig::get(CBTConfig::tips) );
	KTipDialog::showTip(this, "bibletime/tips", true);	
//	CTipWindow* tipDlg = new CTipWindow();
//	tipDlg->exec();
//	delete tipDlg;	
}


/** Is called just before the window menu is ahown. */
void BibleTime::slotWindowMenuAboutToShow(){
	if (!m_windowMenu)
		return;

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
	if ( m_windowAutoCascade_action->isPlugged() )
		m_windowAutoCascade_action->unplug(m_windowMenu);
	if ( m_windowAutoTile_action->isPlugged() )
		m_windowAutoTile_action->unplug(m_windowMenu);
	if ( m_windowCloseAll_action->isPlugged() )
		m_windowAutoTile_action->unplug(m_windowMenu);

	m_windowMenu->clear();
			
	m_windowSaveProfile_action->plug(m_windowMenu);	
	m_windowLoadProfile_action->plug(m_windowMenu);	
	m_windowEditProfiles_action->plug(m_windowMenu);	
	m_windowMenu->insertSeparator();	
	m_windowFullscreen_action->plug(m_windowMenu);				
	m_windowMenu->insertSeparator();	
	m_windowCascade_action->plug(m_windowMenu);
	m_windowTile_action->plug(m_windowMenu);
	m_windowAutoCascade_action->plug(m_windowMenu);
	m_windowAutoTile_action->plug(m_windowMenu);
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
	} else {
		m_windowTile_action->setEnabled( !m_windowAutoTile_action->isChecked() );
		m_windowCascade_action->setEnabled( !m_windowAutoCascade_action->isChecked() );
		m_windowCloseAll_action->setEnabled( true );
		m_windowMenu->insertSeparator();		
	}
	
	QWidgetList windows = m_mdi->windowList();
	int i, id;
	for ( i = 0; i < int(windows.count()); ++i ) {
		id = m_windowMenu->insertItem(QString::fromLatin1("&%1 ").arg(i+1)+windows.at(i)->caption(),
			this, SLOT( slotWindowMenuActivated( int ) ) );
		m_windowMenu->setItemParameter( id, i );
	  m_windowMenu->setItemChecked( id, m_mdi->activeWindow() == windows.at(i) );
	}
}

/** This slot is connected with the windowAutoTile_action object */
void BibleTime::slotAutoTile(){
	if (m_windowAutoTile_action->isChecked()) {
		m_windowAutoCascade_action->setChecked(false);
		m_mdi->setGUIOption( CMDIArea::autoTile );
	}
	else if (!m_windowAutoCascade_action->isChecked())
		m_mdi->setGUIOption( CMDIArea::Nothing );
}

/** This slot is connected with the windowAutoCascade_action object */
void BibleTime::slotAutoCascade(){
	if (m_windowAutoCascade_action->isChecked()) {
		m_windowAutoTile_action->setChecked(false);
		m_mdi->setGUIOption( CMDIArea::autoCascade );
	}
	else if (!m_windowAutoTile_action->isChecked())
		m_mdi->setGUIOption( CMDIArea::Nothing );
}

void BibleTime::slotWindowMenuActivated( int id ) {
  if (!m_windowMenu)
  	return;
  	
//  QString dummy;
  QWidget* w = m_mdi->windowList().at( id );
  if ( w )
		w->setFocus();
}


/** Shows/hides the toolbar */
void BibleTime::slotToggleToolbar(){
	if (m_viewToolbar_action->isChecked())
		toolBar("mainToolBar")->show();
	else
		toolBar("mainToolBar")->hide();
}

/** Shows or hides the groupmanager. */
void BibleTime::slotToggleGroupManager() {
	if (m_viewGroupManager_action->isChecked())
		m_groupmanager->show();
	else
		m_groupmanager->hide();
}

/** Opens a toolbar editor */
void BibleTime::slotSettingsToolbar(){	
	KEditToolbar dlg(actionCollection());
	if (dlg.exec())
		createGUI();
}

/** The last window was closed! */
void BibleTime::lastWindowClosed(){
}


/** Opens the print dialog. */
void BibleTime::slotFilePrint(){
	m_printer->setup(this);	//opens the printer dialog
}

/** Enables the "Clear printer queue" action */
void BibleTime::slotSetPrintingStatus(){
	m_filePrint_action->setEnabled( m_printer->getPrintQueue()->count()>0 );
	m_fileClearQueue_action->setEnabled( m_printer->getPrintQueue()->count()>0 );
}

/** Printing was started */
void BibleTime::slotPrintingStarted(){
	pthread_mutex_init(&progress_mutex, 0);

	m_progress = new QProgressDialog(i18n("Printing..."), i18n("Abort printing"),/*m_printer->getPrintQueue()->count()*/100,this, "progress", true);
	connect(m_progress, SIGNAL(cancelled()), SLOT(slotAbortPrinting()));
	m_progress->setProgress(0);
	m_progress->show();
}

/** Printing was finished */
void BibleTime::slotPrintingFinished(){
	if (pthread_mutex_trylock(&progress_mutex) == EBUSY)
		return;
		
	if (m_progress)
		delete m_progress;
	m_progress = 0;	
	pthread_mutex_unlock(&progress_mutex);	
	
	pthread_mutex_destroy(&progress_mutex);
}

/** No descriptions */
void BibleTime::slotPrintedPercent( const int percent ){
	if (pthread_mutex_trylock(&progress_mutex) == EBUSY)
		return;		
	if (m_progress) {
		m_progress->setProgress(percent);
//		m_progress->setLabelText(i18n("Printing %1").arg(key));
	}	
	pthread_mutex_unlock(&progress_mutex);	
}

/** Aborts the printing */
void BibleTime::slotAbortPrinting(){
	m_printer->abort();
	if (m_progress)
		slotPrintingFinished();
}

void BibleTime::openOnlineHelp_Handbook() {
//#ifdef STATIC_BUILD
//	CHTMLDialog dlg("bibletime/handbook/index.html");
//	dlg.exec();
//#else
	if (m_helpMenu)
//		m_helpMenu->appHelpActivated();
		kapp->invokeHTMLHelp("bibletime/handbook/index.html");
//#endif
}
void BibleTime::openOnlineHelp_Install() {
//#ifdef STATIC_BUILD
//	CHTMLDialog dlg("bibletime/install/index.html");
//	dlg.exec();
//#else
	kapp->invokeHTMLHelp("bibletime/install/index.html");
//#endif
}
void BibleTime::openOnlineHelp_Howto() {
//#ifdef STATIC_BUILD
//	CHTMLDialog dlg("bibletime/howto/index.html");
//	dlg.exec();
//#else
	kapp->invokeHTMLHelp("bibletime/howto/index.html");
//#endif
}

/** Saves the current settings into the currently activatred profile. */
void BibleTime::saveProfile(int ID){
	m_mdi->setUpdatesEnabled(false);
	KPopupMenu* popup = m_windowSaveProfile_action->popupMenu();
	CProfile* p = m_profileMgr.profile( popup->text(ID) );
	if (p)
		saveProfile(p);

	m_mdi->setUpdatesEnabled(true);	
}

void BibleTime::saveProfile(CProfile* profile){
	if (!profile)
		return;

	//save mainwindow settings
	storeProfileSettings(profile);
			
	QWidgetList windows = m_mdi->windowList();
	QList<CProfileWindow> profileWindows;
	for (QWidget* w = windows.first(); w; w = windows.next()) {
		CSwordPresenter* displayWindow = dynamic_cast<CSwordPresenter*>(w);
		if (!displayWindow)
			continue;

		CProfileWindow* profileWindow = new CProfileWindow();
		displayWindow->storeSettings(profileWindow);
		profileWindows.append(profileWindow);
	}
	profile->save(profileWindows);

//clean up memory - delete all created profile windows
	profileWindows.setAutoDelete(true);
	profileWindows.clear();
}

void BibleTime::loadProfile(int ID){
	m_mdi->setUpdatesEnabled(false);
	m_mdi->deleteAll();
	
	m_mdi->setUpdatesEnabled(false);
	KPopupMenu* popup = m_windowLoadProfile_action->popupMenu();
	CProfile* p = m_profileMgr.profile( popup->text(ID) );
	if (p)
		loadProfile(p);

	m_mdi->setUpdatesEnabled(true);	
}

void BibleTime::loadProfile(CProfile* p){
	if (!p)
		return;		
	QList<CProfileWindow> windows = p->load();

	//load mainwindow setttings
	applyProfileSettings(p);
		
	for (CProfileWindow* w = windows.first(); w; w = windows.next()) {
		const QString key = w->key();		
		QStringList usedModules = w->modules();
		ListCSwordModuleInfo modules;
		for ( QStringList::Iterator it = usedModules.begin(); it != usedModules.end(); ++it ) {
			CSwordModuleInfo* m = m_backend->findModuleByName(*it);
			if (m)
				modules.append(m);
		}
		if (!modules.count()) //are the modules still installed?
			continue;
		
		CSwordPresenter* displayWindow = createNewSwordPresenter(modules, key);
		if (displayWindow) {
//			qDebug("BibleTime::loadProfile: apply settings");
			displayWindow->applySettings(w);
		}
	}	
	
	m_mdi->setUpdatesEnabled(true);	
//	qWarning("finished");
}

void BibleTime::toggleFullscreen(){
	if (m_windowFullscreen_action->isChecked())
		showFullScreen();
	else
		showNormal();
}

void BibleTime::editProfiles(){
	COptionsDialog *dlg = new COptionsDialog(this, "COptionsDialog", m_keyAccel);
  connect(dlg, SIGNAL(signalSettingsChanged()), SLOT(slotSettingsChanged()) );
	dlg->showPart(COptionsDialog::ViewProfiles);	
	dlg->exec();

	dlg->delayedDestruct();	
}
