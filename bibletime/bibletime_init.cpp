/***************************************************************************
                          bibletime_init.cpp  -  functions to initialize BibleTime on startup
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

//BibleTime includes
#include "printing/cprinter.h"
#include "bibletime.h"
#include "ressource.h"
#include "frontend/chtmldialog.h"
#include "frontend/ctoolclass.h"
#include "ressource.h"
#include "whatsthisdef.h"
#include "config.h"

#include <stdlib.h>

//QT includes
#include <qpopupmenu.h>
#include <qsplitter.h>
#include <qguardedptr.h>
#include <qlistview.h>

//KDE includes
#include <kaboutdata.h>
#include <kconfigbase.h>
#include <kconfig.h>
#include <ktoolbar.h>
#include <kiconloader.h>
#include <kstddirs.h>
#include <kstdaction.h>
#include <kmenubar.h>
#include <kaccel.h>
#include <klocale.h>
#include <kaction.h>
#include <khelpmenu.h>
#include <kglobal.h>
#include <kpopupmenu.h>

/**Initializes the view of this widget*/
void BibleTime::initView(){
	qDebug("BibleTime::initView");
	ASSERT(m_moduleList);
	ASSERT(m_important->swordBackend);
	m_splitter = new QSplitter(this, "mainsplitter");
	
	m_groupmanager = new CGroupManager(m_important, m_splitter, "groupmanager", m_moduleList );
//	QWhatsThis::add(m_groupmanager,WT_GM_WIDGET);
	
	m_mdi = new CMDIArea(m_important, m_splitter, "mdiarea" );
//	QWhatsThis::add(m_groupmanager,WT_GM_WIDGET);	
	m_mdi->setFocusPolicy( StrongFocus );

	m_helpMenu = new KHelpMenu(this, KGlobal::instance()->aboutData(), true, actionCollection());
	
	
	setCentralWidget(m_splitter);	
}


/** Initializes the action objects of the GUI */
void BibleTime::initActions() {
	qDebug("BibleTime::initActions()");

	m_fileClearQueue_action = new KAction(i18n("Clear printing queue"), ICON_FILE_CLEAR_QUEUE ,0,
		m_important->printer, SLOT(clearQueue()), actionCollection(), "fileClearQueue_action");	
	m_fileClearQueue_action->setEnabled(false);	
	m_fileClearQueue_action->setToolTip( TT_FILE_CLEAR_QUEUE );
	m_fileClearQueue_action->setWhatsThis( WT_FILE_CLEAR_QUEUE );
	
	m_filePrint_action = KStdAction::print(this, SLOT( slotFilePrint() ), actionCollection());
	m_filePrint_action->setEnabled(false);		
	m_filePrint_action->setToolTip( TT_FILE_PRINT );	
	m_filePrint_action->setWhatsThis( WT_FILE_PRINT );
	
	m_fileQuit_action = KStdAction::quit(this, SLOT( slotFileQuit() ), actionCollection());
	m_fileQuit_action->setToolTip( TT_FILE_QUIT );	
	m_fileQuit_action->setWhatsThis( WT_FILE_QUIT );
												
	m_GMReset_action =  new KAction(i18n("Reset main index"), ICON_MAININDEX_RESET, IDK_GM_RESET,
		m_groupmanager, SLOT(slotReset()), actionCollection(), "GMreset_action");	
	m_GMReset_action->setToolTip( TT_GM_RESET );			
	m_GMReset_action->setWhatsThis( WT_GM_RESET );		
	
	m_GMSearchModules_action =  new KAction(i18n("Search in module(s)"), ICON_MAININDEX_SEARCH, IDK_GM_MODULES_SEARCH,
		m_groupmanager, SLOT(slotSearchSelectedModules()), actionCollection(), "GMsearch_action");	
  m_GMSearchModules_action->setToolTip( TT_GM_SEARCH_MODULES );
  m_GMSearchModules_action->setWhatsThis( WT_GM_SEARCH_MODULES );
	
	m_GMDeleteItems_action =  new KAction(i18n("Remove selected item(s)"),ICON_MAININDEX_DELETE_ITEMS, IDK_GM_ITEMS_DELETE,
		m_groupmanager, SLOT(slotDeleteSelectedItems()), actionCollection(), "GMdelete_action");	
	m_GMDeleteItems_action->setToolTip( TT_GM_DELETE_ITEMS );	
	m_GMDeleteItems_action->setWhatsThis( WT_GM_DELETE_ITEMS );

	m_viewFootnotes_action = new KToggleAction(i18n("&Show footnotes"),ICON_VIEW_FOOTNOTES, IDK_VIEW_FOOTNOTES,
		this, SLOT(slotToggleFootnotes()), actionCollection(), "viewFootnotes_action");
	m_viewFootnotes_action->setToolTip( TT_VIEW_FOOTNOTES );	
	m_viewFootnotes_action->setWhatsThis( WT_VIEW_FOOTNOTES );
	
	m_viewStrongs_action = new KToggleAction(i18n("S&how Strong's numbers"),ICON_VIEW_STRONGS, IDK_VIEW_STRONGS,
		this, SLOT(slotToggleStrongs()), actionCollection(), "viewStrongs_action");	
	m_viewStrongs_action->setToolTip( TT_VIEW_STRONGS );	
	m_viewStrongs_action->setWhatsThis( WT_VIEW_STRONGS );
 	
	m_viewToolbar_action = KStdAction::showToolbar(this, SLOT( slotToggleToolbar() ), actionCollection());
	m_viewToolbar_action->setToolTip( TT_VIEW_TOOLBAR );	
	m_viewToolbar_action->setWhatsThis( WT_VIEW_TOOLBAR );

	m_viewGroupManager_action = new KToggleAction(i18n("&Show main index"), ICON_VIEW_MAININDEX, IDK_VIEW_GROUPMANAGER,
		this, SLOT(slotToggleGroupManager()), actionCollection(), "viewGroupManager_action");
	m_viewGroupManager_action->setWhatsThis( WT_VIEW_GROUPMANAGER );
	
	m_settingsOptions_action = KStdAction::preferences(this, SLOT( slotSettingsOptions() ), actionCollection());
	m_settingsOptions_action->setToolTip( TT_SETTINGS_OPTIONS );
	m_settingsOptions_action->setWhatsThis( WT_SETTINGS_OPTIONS );

	m_settingsToolbar_action = KStdAction::configureToolbars(this, SLOT( slotSettingsToolbar() ), actionCollection());
	m_settingsToolbar_action->setToolTip( TT_SETTINGS_EDIT_TOOLBAR );	
	m_settingsToolbar_action->setWhatsThis( WT_SETTINGS_EDIT_TOOLBAR );

	m_windowCascade_action = new KAction(i18n("&Cascade"), ICON_WINDOW_CASCADE,
																IDK_WINDOW_CASCADE, m_mdi, SLOT(cascade()), actionCollection(), "windowCascade_action");
	m_windowCascade_action->setToolTip( TT_WINDOW_CASCADE );	
	m_windowCascade_action->setWhatsThis( WT_WINDOW_CASCADE );
	
	m_windowTile_action = new KAction(i18n("&Tile"), ICON_WINDOW_TILE,
																IDK_WINDOW_TILE, m_mdi, SLOT(tile()), actionCollection(), "windowTile_action");
	m_windowTile_action->setToolTip( TT_WINDOW_TILE );	
	m_windowTile_action->setWhatsThis( WT_WINDOW_TILE );
	
	m_windowAutoCascade_action 	= new KToggleAction(i18n("&Auto cascade"), ICON_WINDOW_CASCADE_AUTO,
																IDK_WINDOW_AUTO_CASCADE, this, SLOT(slotAutoCascade()), actionCollection(), "windowAutoCascade_action");
	m_windowAutoCascade_action->setToolTip( TT_WINDOW_AUTO_CASCADE );	
	m_windowAutoCascade_action->setWhatsThis( WT_WINDOW_AUTO_CASCADE );
																	
	m_windowAutoTile_action = new KToggleAction(i18n("A&uto Tile"),ICON_WINDOW_TILE_AUTO,
																IDK_WINDOW_AUTO_TILE, this, SLOT(slotAutoTile()), actionCollection(), "windowAutoTile_action");
	m_windowAutoTile_action->setToolTip( TT_WINDOW_AUTO_TILE );	
	m_windowAutoTile_action->setWhatsThis( WT_WINDOW_AUTO_TILE );
	
	m_windowCloseAll_action = new KAction(i18n("&Close all"), ICON_WINDOW_CLOSE_ALL,
																IDK_WINDOW_CLOSE_ALL, m_mdi, SLOT(deleteAll()), actionCollection(),"windowCloseAll_action");
	m_windowCloseAll_action->setToolTip( TT_WINDOW_CLOSE_ALL );	
	m_windowCloseAll_action->setWhatsThis( WT_WINDOW_CLOSE_ALL );
	

	if ( !(m_helpContents_action = actionCollection()->action("help_contents")) )	
		m_helpContents_action = KStdAction::helpContents(m_helpMenu, SLOT(appHelpActivated()), actionCollection() );
	
	if ( !(m_helpWhatsThis_action = actionCollection()->action("help_whats_this")) )		
		m_helpWhatsThis_action = KStdAction::whatsThis(this, SLOT(whatsThis()), actionCollection());
	
	if ( !(m_helpBugReport_action = actionCollection()->action("help_report_bug")) )		
		m_helpBugReport_action = KStdAction::reportBug(m_helpMenu, SLOT(reportBug()), actionCollection());	
	
	m_helpDailyTip_action = new KAction(i18n("&Daily tip"), ICON_HELP_DAILY_TIP, IDK_HELP_DAILY_TIP, this, SLOT(slotHelpTipOfDay()), actionCollection(), "helpDailyTip_action");
	if ( !(m_helpAboutBibleTime_action = actionCollection()->action("help_about_app")) )		
		m_helpAboutBibleTime_action = KStdAction::aboutApp(m_helpMenu, SLOT(aboutApplication()), actionCollection());	
	
	if ( !(m_helpAboutKDE_action = actionCollection()->action("help_about_kde")) )
		m_helpAboutKDE_action = KStdAction::aboutKDE(m_helpMenu, SLOT(aboutKDE()), actionCollection());	
}

/** Initializes the menubar of BibleTime. */
void BibleTime::initMenubar(){
	//get the window and edit menus using the actions and their properties
	if ( m_windowTile_action->container(0) && m_windowTile_action->container(0)->inherits("QPopupMenu") )
		m_windowMenu = (QPopupMenu*)m_windowTile_action->container(0);
	else
		m_windowMenu = 0;
}

/** Initializes the SIGNAL / SLOT connections */
void BibleTime::initConnections(){
	connect(m_mdi, SIGNAL(sigSetToplevelCaption(const QString&)),
		this, SLOT(setPlainCaption(const QString&)));
	
	ASSERT(m_windowMenu);
	if (m_windowMenu) {
		qDebug("window menu exists!");
		connect(m_windowMenu, SIGNAL(aboutToShow()),
			this, SLOT(slotWindowMenuAboutToShow()));
		connect(m_windowMenu, SIGNAL(activated(int)),
			this, SLOT(slotWindowMenuActivated(int)));		
	}
	else
		warning("can't find window menu");
		
	connect(m_groupmanager, SIGNAL(createSwordPresenter(CSwordModuleInfo*, const QString)),
		this, SLOT(createNewSwordPresenter(CSwordModuleInfo*,const QString)));
	connect(m_groupmanager, SIGNAL(createSwordPresenter(ListCSwordModuleInfo, const QString)),
		this, SLOT(createNewSwordPresenter(ListCSwordModuleInfo,const QString)));
	
	//connect to the signals of the printer object
	connect(m_important->printer, SIGNAL(addedFirstQueueItem()),
		this, SLOT(slotSetPrintingStatus()));
	connect(m_important->printer, SIGNAL(printingStarted()),
		this, SLOT(slotPrintingStarted()));		
	connect(m_important->printer, SIGNAL(printingFinished()),
		this, SLOT(slotSetPrintingStatus()));		
	connect(m_important->printer, SIGNAL(printingFinished()),
		this, SLOT(slotPrintingFinished()));				
	connect(m_important->printer, SIGNAL(printingInterrupted()),
		this, SLOT(slotSetPrintingStatus()));				
	connect(m_important->printer, SIGNAL(printingInterrupted()),
		this, SLOT(slotPrintingFinished()));						
	connect(m_important->printer, SIGNAL(queueCleared()),
		this, SLOT(slotSetPrintingStatus()));
	connect(m_important->printer, SIGNAL(printedOneItem(const QString&, const int)),
		this, SLOT(slotPrintedEntry(const QString&, const int)));		
}

/** Initializes the keyboard accelerators */
void BibleTime::initKeyAccels(){
	m_keyAccel = new KAccel(this);

	m_fileQuit_action->plugAccel( m_keyAccel );
	
	m_GMSearchModules_action->plugAccel( m_keyAccel );
	m_GMReset_action->plugAccel( m_keyAccel );
	m_GMDeleteItems_action->plugAccel( m_keyAccel );
	
	m_viewFootnotes_action->plugAccel( m_keyAccel );
	m_viewStrongs_action->plugAccel( m_keyAccel );	

	m_settingsOptions_action->plugAccel( m_keyAccel );
	m_settingsToolbar_action->plugAccel( m_keyAccel );
	
	m_windowCascade_action->plugAccel( m_keyAccel);	
	m_windowTile_action->plugAccel( m_keyAccel );	
	m_windowAutoCascade_action->plugAccel( m_keyAccel );	
	m_windowAutoTile_action->plugAccel( m_keyAccel );	
	m_windowCloseAll_action->plugAccel( m_keyAccel );
}

/** Initializes the backend */
void BibleTime::initBackends(){
	qDebug("CBibleTime::initBackends()");
	m_moduleList = 0;
	
	m_important->swordBackend = new CSwordBackend();
	
	CSwordBackend::errorCode errorCode = m_important->swordBackend->initModules();
	if ( errorCode == CSwordBackend::noError ) {	//no error
		m_moduleList = m_important->swordBackend->getModuleList();
		ASSERT(m_moduleList);
	} else {
		//show error message that initBackend failed		
		switch (errorCode) {
			case CSwordBackend::noModulesAvailable:			
				HTML_DIALOG(HELPDIALOG_NO_SWORD_MODULES);
				break;
			case CSwordBackend::noSwordConfigFile:
				HTML_DIALOG(HELPDIALOG_NO_SWORD_CONFIG);
				break;
			case CSwordBackend::noSwordModuleDirectory:
				HTML_DIALOG(HELPDIALOG_NO_SWORD_MODULES_DIR);
				break;		
			case CSwordBackend::noSwordModuleConfigDirectory:
				HTML_DIALOG(HELPDIALOG_NO_SWORD_MODULE_CONFIG_DIR);				
				break;						
			default:
				HTML_DIALOG(HELPDIALOG_INITBACKEND_FAILED);
				break;
		}
	}
	//initialize international bookname language
	KConfigGroupSaver dummy(m_config, "SWORD");
	const QString language = m_config->readEntry("Language", QString::fromLatin1(getenv("LANG")));
	m_important->swordBackend->setBooknameLanguage(language);
}

/** Initializes the CPrinter object. */
void BibleTime::initPrinter(){
	qDebug("BibleTime::initPrinter()");
	ASSERT(m_important->swordBackend);
	m_important->printer = new CPrinter(m_important, this);
}

