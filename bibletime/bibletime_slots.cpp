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
#include "frontend/optionsdialog/coptionsdialog.h"
//#include "frontend/presenters/cpresenter.h"
#include "frontend/ctoolclass.h"
#include "printing/cprinterdialog.h"
#include "frontend/ctipwindow.h"
#include "backend/sword_backend/cswordversekey.h"
#include "backend/sword_backend/chtmlentrydisplay.h"
#include "backend/sword_backend/chtmlchapterdisplay.h"
#include "config.h"

//QT includes
#include <qwhatsthis.h>
#include <qvaluelist.h>
#include <qclipboard.h>
#include <qxembed.h>

//KDE includes
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
#include <kconfig.h>

//Sword includes
#include <versekey.h>

/** Closes the current apploication window */
void BibleTime::slotFileQuit(){
	saveSettings();		
	// close the first window, the list makes the next one the first again.
	// This ensures that queryClose() is called on each window to ask for closing
	
#if KDE_VERSION >= 193
	#warning You are using the latest KDE CVS!
	KMainWindow* w;
#else
	#warning You are using a KDE Beta release!
	KTMainWindow* w;
#endif

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
	COptionsDialog *dlg = new COptionsDialog(m_important, this, "COptionsDialog", m_keyAccel);
	dlg->setMinimumSize(dlg->sizeHint());
	dlg->resize(dlg->sizeHint());
	
	if ( dlg->exec() ) {			
		if (m_important->swordBackend->m_entryDisplay) {
			KConfigGroupSaver groupSaver(m_config, "Colors");
			QColor tempColor = m_config->readColorEntry("Highlighted Verse");			
			m_important->swordBackend->m_entryDisplay->m_highlightedVerseColor = tempColor.name();
			
			m_config->setGroup("Fonts");
			QFont dummy =  m_config->readFontEntry("Presenter");
			m_important->swordBackend->m_entryDisplay->setStandardFont( dummy.family(), CToolClass::makeLogicFontSize( dummy.pointSize() ) );			
		}
		if (m_important->swordBackend->m_chapterDisplay) {
			KConfigGroupSaver groupSaver(m_config, "Colors");
			QColor tempColor = tempColor = m_config->readColorEntry("Highlighted Verse");
			m_important->swordBackend->m_chapterDisplay->m_highlightedVerseColor = tempColor.name();
			
			m_config->setGroup("Fonts");
			QFont dummy =  m_config->readFontEntry("Presenter");
			m_important->swordBackend->m_chapterDisplay->setStandardFont( dummy.family(), CToolClass::makeLogicFontSize( dummy.pointSize() ) );			
		}				
		const int changedSettings = dlg->getChangedSettings();
		if (changedSettings & CSwordPresenter::language) {	//the language changed
			KConfigGroupSaver dummy(m_config, "SWORD");
			const QString language = m_config->readEntry("Language", "");
			m_important->swordBackend->setBooknameLanguage(language);		
		}
		for ( int index = 0; index < m_mdi->windowList().count(); index++) {
			CSwordPresenter* myPresenter = (CSwordPresenter*)m_mdi->windowList().at(index);
			ASSERT(myPresenter);
			if (myPresenter) {
				myPresenter->refresh(changedSettings);
			}
		}		
		
		//refresh the bookmark items in the groupmanager		
		QListViewItemIterator it( m_groupmanager );
		CGroupManagerItem* dummy = 0;
		for ( ; it.current(); ++it ) {
			if ( (dummy = dynamic_cast<CGroupManagerItem*>(it.current())) ) {
				if (dummy->type() == CGroupManagerItem::Bookmark) {
					if (dummy->getBookmarkKey()) {
						CSwordVerseKey* vKey = 0;
						if ( (vKey = dynamic_cast<CSwordVerseKey*>(dummy->getBookmarkKey())) ) {
							vKey->setLocale( (const char*)m_important->swordBackend->getCurrentBooknameLanguage().local8Bit());
							dummy->update();
						}
					}
				}
			}
		}
	}
	
	delete dlg;
}

/** Shows the daily tip */
void BibleTime::slotHelpTipOfDay(){
	CTipWindow* tipDlg = new CTipWindow();
	tipDlg->exec();
	delete tipDlg;	
}

/** Switches footnotes on or off */
void BibleTime::slotToggleFootnotes(){
	qDebug("BibleTime::slotToggleFootnotes");		
	m_important->swordBackend->setOption(CSwordBackend::footnotes, m_viewFootnotes_action->isChecked() );
	
	refreshPresenters( CSwordPresenter::footnotes );
}

/** Switches displaying of strong number on or off */
void BibleTime::slotToggleStrongs(){
	qDebug("BibleTime::slotToggleStrongs");
	m_important->swordBackend->setOption(CSwordBackend::strongNumbers, m_viewStrongs_action->isChecked());	
	
	refreshPresenters( CSwordPresenter::strongNumbers );
}

/** Is called just before the window menu is ahown. */
void BibleTime::slotWindowMenuAboutToShow(){
	if (!m_windowMenu) {
		ASSERT(m_windowMenu);
		qDebug("return");
		return;
	}
		
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
	qDebug("%d entries in list",windows.count());
	int i, id;
	for ( i = 0; i < int(windows.count()); ++i ) {
		id = m_windowMenu->insertItem(QString("&%1 ").arg(i+1)+windows.at(i)->caption(),
			this, SLOT( slotWindowMenuActivated( int ) ) );
		m_windowMenu->setItemParameter( id, i );
	  m_windowMenu->setItemChecked( id, m_mdi->activeWindow() == windows.at(i) );
	}
}

/** This slot is connected with the windowAutoTile_action object */
void BibleTime::slotAutoTile(){
	qDebug("BibleTime::slotAutoTile");
	if (m_windowAutoTile_action->isChecked()) {
		m_windowAutoCascade_action->setChecked(false);
		m_mdi->setGUIOption( CMDIArea::autoTile );
//		m_mdi->tile();
	}
	else if (!m_windowAutoCascade_action->isChecked())
		m_mdi->setGUIOption( CMDIArea::Nothing );
}

/** This slot is connected with the windowAutoCascade_action object */
void BibleTime::slotAutoCascade(){
	qDebug("BibleTime::slotAutoCascade");
	if (m_windowAutoCascade_action->isChecked()) {
		m_windowAutoTile_action->setChecked(false);
		m_mdi->setGUIOption( CMDIArea::autoCascade );
//		m_mdi->cascade();
	}
	else if (!m_windowAutoTile_action->isChecked())
		m_mdi->setGUIOption( CMDIArea::Nothing );
}

void BibleTime::slotWindowMenuActivated( int id ) {
  qDebug("BibleTime::slotWindowMenuActivated( int id )");
  if (!m_windowMenu)
  	return;
  	
  QString dummy;
  QWidget* w = m_mdi->windowList().at( id );
//	qDebug( dummy.setNum(id) );
	ASSERT(w);
  if ( w )
		w->setFocus();
}


/** Shows/hides the toolbar */
void BibleTime::slotToggleToolbar(){
	qDebug("BibleTime::slotToggleToolbar");
	
	if (m_viewToolbar_action->isChecked()) {
		toolBar("mainToolBar")->show();
		qDebug("show");
		
	}
	else {
		toolBar("mainToolBar")->hide();
		qDebug("hide");
	}
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
	if (dlg.exec()) {
		createGUI();
	}
}

/** The last window was closed! */
void BibleTime::lastWindowClosed(){
	qDebug("BibleTime::lastWindowClosed()");
	qDebug(m_initialized ? "true" : "false");
}


/** Opens the print dialog. */
void BibleTime::slotFilePrint(){
	qDebug("BibleTime::slotFilePrint()");
	m_important->printer->setup(this);	//opens the printer dialog
}

/** Enables the "Clear printer queue" action */
void BibleTime::slotSetClearQueueStatus(){
	m_fileClearQueue_action->setEnabled( m_important->printer->getPrintQueue()->count() >0 );
}

/** Reimplementation. Sets the caption and takes care of special captions set by the MDI window */
void BibleTime::setCaption(const QString& caption){
	KMainWindow::setCaption(caption);
}
