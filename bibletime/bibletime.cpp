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
#include "backend/cmoduleinfo.h"
#include "backend/sword_backend/cswordmoduleinfo.h"
#include "backend/sword_backend/cswordbiblemoduleinfo.h"
#include "backend/sword_backend/cswordcommentarymoduleinfo.h"
#include "backend/sword_backend/cswordlexiconmoduleinfo.h"
#include "backend/sword_backend/chtmlentrydisplay.h"
#include "backend/sword_backend/chtmlchapterdisplay.h"
#include "backend/sword_backend/cswordversekey.h"
#include "backend/sword_backend/cswordldkey.h"
#include "frontend/presenters/cswordpresenter.h"
#include "frontend/presenters/cbiblepresenter.h"
#include "frontend/presenters/ccommentarypresenter.h"
#include "frontend/presenters/clexiconpresenter.h"
#include "frontend/keychooser/ckeychooser.h"

//KDE includes
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
	ASSERT(m_groupmanager);	
	ASSERT(m_mdi);
	ASSERT(m_config);
	ASSERT(m_keyAccel);
	
	saveMainWindowSettings(m_config);
//	if (groupmanager)
//		groupmanager->saveSettings();	
	if (m_mdi)
		m_mdi->saveSettings();	

	if (m_keyAccel)
		m_keyAccel->writeSettings(m_config);	
	{
		KConfigGroupSaver groupSaver(m_config,"General");
//		m_config->writeEntry("maingeom", size());
		m_config->writeEntry("showFootnotes", m_viewFootnotes_action->isChecked());
		m_config->writeEntry("showStrongs", m_viewStrongs_action->isChecked());
		m_config->writeEntry("show toolbar", m_viewToolbar_action->isChecked());
		m_config->writeEntry("show main index", m_viewGroupManager_action->isChecked());		
		if (m_viewGroupManager_action->isChecked())	//only save changes when the groupmanager is visible
			m_config->writeEntry("splitterSizes", m_splitter->sizes());
	}
	{
		KConfigGroupSaver groupSaver(m_config, "MDI");	
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
	}
	//backend specific things
		if (m_important->swordBackend->m_entryDisplay) {
			KConfigGroupSaver groupSaver(m_config, "Colors");	
			m_config->writeEntry("Highlighted Verse",
				QColor(m_important->swordBackend->m_entryDisplay->m_highlightedVerseColor));
		}
}

/** Reads the settings from the configfile and sets the right properties. */
void BibleTime::readSettings(){
	qDebug("BibleTime::readSettings()");
	
	applyMainWindowSettings(m_config);
	
	m_keyAccel->readSettings(m_config);
	ASSERT(m_viewToolbar_action);	
	{
		KConfigGroupSaver groupsaver(m_config, "General");
		m_viewFootnotes_action->setChecked( m_config->readBoolEntry("showFootnotes", true));
		slotToggleFootnotes();
	
		m_viewStrongs_action->setChecked( m_config->readBoolEntry("showStrongs", true));
		slotToggleStrongs();

		m_viewToolbar_action->setChecked( m_config->readBoolEntry("show toolbar", true) );
		slotToggleToolbar();
		
		m_viewGroupManager_action->setChecked( m_config->readBoolEntry("show main index", true) );
		slotToggleGroupManager();
		
		m_splitter->setSizes( m_config->readIntListEntry("splitterSizes") );		
	}
	
	{
		KConfigGroupSaver groupSaver(m_config, "MDI");
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
	//backend specific things
		if (m_important->swordBackend->m_entryDisplay) {
			KConfigGroupSaver groupSaver(m_config, "Colors");
			QColor tempColor;
	
			tempColor = m_config->readColorEntry("Highlighted Verse", &red);
			m_important->swordBackend->m_entryDisplay->m_highlightedVerseColor = tempColor.name();
			tempColor = tempColor = m_config->readColorEntry("Versenumber/URL");
			m_important->swordBackend->m_entryDisplay->m_linkColor = tempColor.name();
			tempColor = tempColor = m_config->readColorEntry("Normal Text");
			m_important->swordBackend->m_entryDisplay->m_textColor = tempColor.name();
			
			m_config->setGroup("Fonts");
			QFont dummy =  m_config->readFontEntry("Presenter");
			m_important->swordBackend->m_entryDisplay->setStandardFont( dummy.family(), CToolClass::makeLogicFontSize( dummy.pointSize() ) );			
		}
		if (m_important->swordBackend->m_chapterDisplay) {
			KConfigGroupSaver groupSaver(m_config, "Colors");
			QColor tempColor;
	
			tempColor = m_config->readColorEntry("Highlighted Verse", &red);
			m_important->swordBackend->m_chapterDisplay->m_highlightedVerseColor = tempColor.name();
			tempColor = tempColor = m_config->readColorEntry("Versenumber/URL");
			m_important->swordBackend->m_chapterDisplay->m_linkColor = tempColor.name();
			tempColor = tempColor = m_config->readColorEntry("Normal Text");
			m_important->swordBackend->m_chapterDisplay->m_textColor = tempColor.name();
			
			m_config->setGroup("Fonts");
			QFont dummy =  m_config->readFontEntry("Presenter");
			m_important->swordBackend->m_chapterDisplay->setStandardFont( dummy.family(), CToolClass::makeLogicFontSize( dummy.pointSize() ));			
		}	
}

/** Creates a new presenter in the MDI area according to the type of the module. */
void BibleTime::createNewSwordPresenter(ListCSwordModuleInfo modules, const QString key) {
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
		connect(presenter, SIGNAL(createSwordPresenter(CSwordModuleInfo*, const QString)),
			this, SLOT(createNewSwordPresenter(CSwordModuleInfo*, const QString)));
		if (presenter->isA("CBiblePresenter")) {
			connect(presenter->getKeyChooser(), SIGNAL(keyChanged(CKey*)),
				m_mdi, SLOT(syncCommentaries(CKey*)));		
		}
		presenter->lookup(key);		
	}
			
	kapp->restoreOverrideCursor();
	presenter->setFocus();
}


/** Creates a new presenter in the MDI area according to the type of the module. */
void BibleTime::createNewSwordPresenter(CSwordModuleInfo* module, const QString key) {
	ListCSwordModuleInfo list;
	list.append(module);
	
	createNewSwordPresenter(list, key);		
}

/** Refreshes all presenter supporting at least in of the features given as parameter.*/
void BibleTime::refreshPresenters( int useFeatures ) {
	/*
	*	Iterate through all features and test their support by the psenters.
	*	If the feature is supported refresh the presenter and continue.
	*/		
	unsigned int index;				
	for ( index = 0; index < m_mdi->windowList().count(); index++) {
		CSwordPresenter* myPresenter = (CSwordPresenter*)m_mdi->windowList().at(index);
		if (myPresenter)
   		myPresenter->refresh(useFeatures);
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
	return true;
}

/** No descriptions */
void BibleTime::show(){
	KMainWindow::show();
	
	//if we show BibleTime for the first time we are ready for processing
	//but not before this point.
	m_initialized = true;
}

/** Reimplementation from QWidget. Sets the correct plain caption. */
void BibleTime::setPlainCaption(const QString& c){
	qWarning(c.local8Bit());
	if (c.isEmpty())
		KMainWindow::setPlainCaption(QString("BibleTime %1").arg(VERSION));
	else
		KMainWindow::setPlainCaption(QString("%1").arg(c));
}
