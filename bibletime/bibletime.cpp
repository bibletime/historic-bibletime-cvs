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
	qDebug("destructor of BibleTime's mainwindow");
	saveSettings();
}

/** Saves the properties of BibleTime to the application wide configfile  */
void BibleTime::saveSettings(){
	qDebug("BibleTime::saveSettings()");
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
		m_config->writeEntry("show groupmanager", m_viewGroupManager_action->isChecked());		
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
		
		m_viewGroupManager_action->setChecked( m_config->readBoolEntry("show groupmanager", true) );
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
			
			m_config->setGroup("Fonts");
			QFont dummy =  m_config->readFontEntry("Presenter");
			m_important->swordBackend->m_entryDisplay->setStandardFont( dummy.family(), CToolClass::makeLogicFontSize( dummy.pointSize() ) );			
		}
		if (m_important->swordBackend->m_chapterDisplay) {
			KConfigGroupSaver groupSaver(m_config, "Colors");
			QColor tempColor;
	
			tempColor = m_config->readColorEntry("Highlighted Verse", &red);
			m_important->swordBackend->m_chapterDisplay->m_highlightedVerseColor = tempColor.name();
			
			m_config->setGroup("Fonts");
			QFont dummy =  m_config->readFontEntry("Presenter");
			m_important->swordBackend->m_chapterDisplay->setStandardFont( dummy.family(), CToolClass::makeLogicFontSize( dummy.pointSize() ));			
		}	
}

/** Creates a new presenter in the MDI area according to the type of the module. */
void BibleTime::createNewSwordPresenter(ListCSwordModuleInfo* modules, const QString key) {

}


/** Creates a new presenter in the MDI area according to the type of the module. */
void BibleTime::createNewSwordPresenter(CSwordModuleInfo* module, const QString key) {
	kapp->setOverrideCursor( waitCursor );		
	
	ListCSwordModuleInfo list;
	list.append(module);
	
	CSwordPresenter* presenter = 0;
	switch (module->getType()) {
		case CSwordModuleInfo::Bible:
			presenter = new CBiblePresenter(list, m_important, m_mdi);
			break;
		case CSwordModuleInfo::Commentary:
			presenter = new CCommentaryPresenter(list, m_important, m_mdi);
			break;
		case CSwordModuleInfo::Lexicon:
			presenter = new CLexiconPresenter(list, m_important, m_mdi);
			break;
		default:
			qWarning("unknown module type");
	}	
	if (presenter) {
		connect(presenter, SIGNAL(createSwordPresenter(CSwordModuleInfo*, const QString)),
			this, SLOT(createNewSwordPresenter(CSwordModuleInfo*, const QString)));
		presenter->lookup(key);
	}

			
	kapp->restoreOverrideCursor();
	

//	if (!key.isEmpty() && presenter) {	//set key to presenter
//		if ((CSwordModuleInfo*)(module_info)) { //it is a Sword module?
//			CSwordModuleInfo* dummy = (CSwordModuleInfo*)(module_info);
//			if (!dummy)
//				return;
//			if ( dynamic_cast<CSwordCommentaryModuleInfo*>(dummy)) {
//				CCommentaryPresenter* commentaryPresenter = dynamic_cast<CCommentaryPresenter*>(presenter);
//				ASSERT(commentaryPresenter);				
//				if (commentaryPresenter && commentaryPresenter->m_keyChooser) {
//					commentaryPresenter->m_key->setKey( key );
//					commentaryPresenter->m_keyChooser->setKey( commentaryPresenter->m_key ); //setting the key will also lookup
//				}
//			}
//			else if ( dynamic_cast<CSwordBibleModuleInfo*>(dummy)) {
//				CBiblePresenter* biblePresenter = dynamic_cast<CBiblePresenter*>(presenter);
//				ASSERT(biblePresenter);
//				if (biblePresenter && biblePresenter->m_keyChooser) {
//					biblePresenter->m_key->setKey( key );
//					biblePresenter->m_keyChooser->setKey( biblePresenter->m_key );	//setting the key will also lookup					
//				}
//			}			
//			else if ( dynamic_cast<CSwordLexiconModuleInfo*>(dummy)) {
//				CLexiconPresenter* lexiconPresenter = dynamic_cast<CLexiconPresenter*>(presenter);
//				lexiconPresenter->m_key->setKey( key );
//				lexiconPresenter->m_keyChooser->setKey( lexiconPresenter->m_key ); //setting the key will also lookup
//			}
//		}
//	}
	presenter->setFocus();
}

/** Refreshes all presenter supporting at least in of the features given as parameter.*/
void BibleTime::refreshPresenters( int useFeatures ) {
	/*
		Iterate through all features and test their support by the psenters.
		If the feature is supported refresh the presenter and continue.
	*/		
	unsigned int index;				
	for ( index = 0; index < m_mdi->windowList().count(); index++) {
		CSwordPresenter* myPresenter = (CSwordPresenter*)m_mdi->windowList().at(index);
		ASSERT(myPresenter);
		if (myPresenter)
     		myPresenter->refresh(useFeatures);
	}
}

/** Called before quit. */
bool BibleTime::queryExit(){
	qDebug("BibleTieme: queryExit");
  if (!m_initialized) {
  	qDebug("return false");
  	return false;
  }
  qDebug( (const char*)QString("m_initialized == %1").arg(m_initialized).local8Bit() );
	saveSettings();
	return true;
}

/** Called before a window is closed */
bool BibleTime::queryClose(){
	qDebug("BibleTime::queryClose()");
	return true;
}

/** No descriptions */
void BibleTime::show(){
	KMainWindow::show();
		
	qDebug("BibleTime::show()");
	//if we show BibleTime for the first time we are ready for processing
	//but not before this point.
	m_initialized = true;
}
