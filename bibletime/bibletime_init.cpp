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
#include "bibletime.h"
#include "resource.h"

#include "backend/cswordbackend.h"

#include "frontend/chtmldialog.h"
#include "frontend/cprofilemgr.h"
#include "frontend/cprofile.h"
#include "frontend/cmdiarea.h"
#include "frontend/kstartuplogo.h"
#include "frontend/mainindex/cmainindex.h"

#include "printing/cprinter.h"

#include "whatsthisdef.h"
#include "config.h"
#include "frontend/cbtconfig.h"


#include <stdlib.h>

//QT includes
#include <qpopupmenu.h>
#include <qsplitter.h>
#include <qguardedptr.h>
#include <qlistview.h>

//KDE includes
#include <kaboutdata.h>
#include <kconfigbase.h>
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
	KStartupLogo::setStatusMessage(i18n("Creating BibleTime's GUI") + QString::fromLatin1("..."));	
	
	m_splitter = new QSplitter(this, "mainsplitter");
	setCentralWidget(m_splitter);	
	
	m_mainIndex = new CMainIndex( centralWidget() );
  m_mainIndex->initTree();
	m_mainIndex->setFocusPolicy(WheelFocus);

	m_mdi = new CMDIArea(centralWidget(), "mdiarea" );
	m_mdi->setFocusPolicy(ClickFocus);

	m_helpMenu = new KHelpMenu(this, KGlobal::instance()->aboutData(), true, actionCollection());
}


/** Initializes the action objects of the GUI */
void BibleTime::initActions() {
	KStartupLogo::setStatusMessage(i18n("Initializing menu- and toolbars") + QString::fromLatin1("..."));

	KAction* action = 0;
		
	m_fileClearQueue_action = new KAction(i18n("Clear printing queue"), ICON_FILE_CLEAR_QUEUE ,0,
		m_printer, SLOT(clearQueue()), actionCollection(), "fileClearQueue_action");	
	m_fileClearQueue_action->setEnabled(false);	
	m_fileClearQueue_action->setToolTip( TT_FILE_CLEAR_QUEUE );
	m_fileClearQueue_action->setWhatsThis( WT_FILE_CLEAR_QUEUE );
	m_fileClearQueue_action->plugAccel( m_keyAccel );	
	
	m_filePrint_action = KStdAction::print(this, SLOT( slotFilePrint() ), actionCollection());
	m_filePrint_action->setEnabled(false);		
	m_filePrint_action->setToolTip( TT_FILE_PRINT );	
	m_filePrint_action->setWhatsThis( WT_FILE_PRINT );
	m_filePrint_action->plugAccel( m_keyAccel );	
	
	action = KStdAction::quit(this, SLOT( slotFileQuit() ), actionCollection());
	action->setToolTip( TT_FILE_QUIT );	
	action->setWhatsThis( WT_FILE_QUIT );
	action->plugAccel( m_keyAccel );
												
//	action =  new KAction(i18n("Reset main index"), ICON_MAININDEX_RESET, IDK_GM_RESET,
//		m_groupmanager, SLOT(slotReset()), actionCollection(), "GMreset_action");	
//	action->setToolTip( TT_GM_RESET );			
//	action->setWhatsThis( WT_GM_RESET );		
//	action->plugAccel( m_keyAccel );
	
//	action =  new KAction(i18n("Search in module(s)"), ICON_MAININDEX_SEARCH, IDK_GM_MODULES_SEARCH,
//		m_groupmanager, SLOT(slotSearchSelectedModules()), actionCollection(), "GMsearch_action");	
//  action->setToolTip( TT_GM_SEARCH_MODULES );
//  action->setWhatsThis( WT_GM_SEARCH_MODULES );
//	action->plugAccel( m_keyAccel );
	
//	action =  new KAction(i18n("Remove selected item(s)"),ICON_MAININDEX_DELETE_ITEMS, IDK_GM_ITEMS_DELETE,
//		m_groupmanager, SLOT(slotDeleteSelectedItems()), actionCollection(), "GMdelete_action");	
//	action->setToolTip( TT_GM_DELETE_ITEMS );	
//	action->setWhatsThis( WT_GM_DELETE_ITEMS );
//	action->plugAccel( m_keyAccel );

	m_viewToolbar_action = KStdAction::showToolbar(this, SLOT( slotToggleToolbar() ), actionCollection());
	m_viewToolbar_action->setToolTip( TT_VIEW_TOOLBAR );	
	m_viewToolbar_action->setWhatsThis( WT_VIEW_TOOLBAR );
	m_viewToolbar_action->plugAccel( m_keyAccel );

	m_viewMainIndex_action = new KToggleAction(i18n("&Show main index"), ICON_VIEW_MAININDEX, IDK_VIEW_GROUPMANAGER,
		this, SLOT(slotToggleGroupManager()), actionCollection(), "viewGroupManager_action");
	m_viewMainIndex_action->setToolTip( TT_VIEW_GROUPMANAGER );	
	m_viewMainIndex_action->setWhatsThis( WT_VIEW_GROUPMANAGER );
	
	action = KStdAction::preferences(this, SLOT( slotSettingsOptions() ), actionCollection());
	action->setToolTip( TT_SETTINGS_OPTIONS );
	action->setWhatsThis( WT_SETTINGS_OPTIONS );
	action->plugAccel( m_keyAccel );

	action = KStdAction::configureToolbars(this, SLOT( slotSettingsToolbar() ), actionCollection());
	action->setToolTip( TT_SETTINGS_EDIT_TOOLBAR );	
	action->setWhatsThis( WT_SETTINGS_EDIT_TOOLBAR );
	action->plugAccel( m_keyAccel );

	m_windowCascade_action = new KAction(i18n("&Cascade"), ICON_WINDOW_CASCADE, IDK_WINDOW_CASCADE,
		m_mdi, SLOT(cascade()), actionCollection(), "windowCascade_action");
	m_windowCascade_action->setToolTip( TT_WINDOW_CASCADE );	
	m_windowCascade_action->setWhatsThis( WT_WINDOW_CASCADE );
	m_windowCascade_action->plugAccel( m_keyAccel);	
	
	m_windowTile_action = new KAction(i18n("&Tile"), ICON_WINDOW_TILE,
																IDK_WINDOW_TILE, m_mdi, SLOT(tile()), actionCollection(), "windowTile_action");
	m_windowTile_action->setToolTip( TT_WINDOW_TILE );	
	m_windowTile_action->setWhatsThis( WT_WINDOW_TILE );
	m_windowTile_action->plugAccel( m_keyAccel );	
	
	m_windowAutoCascade_action 	= new KToggleAction(i18n("&Auto cascade"), ICON_WINDOW_CASCADE_AUTO,
																IDK_WINDOW_AUTO_CASCADE, this, SLOT(slotAutoCascade()), actionCollection(), "windowAutoCascade_action");
	m_windowAutoCascade_action->setToolTip( TT_WINDOW_AUTO_CASCADE );	
	m_windowAutoCascade_action->setWhatsThis( WT_WINDOW_AUTO_CASCADE );
	m_windowAutoCascade_action->plugAccel( m_keyAccel );	
																	
	m_windowAutoTile_action = new KToggleAction(i18n("A&uto Tile"),ICON_WINDOW_TILE_AUTO,
																IDK_WINDOW_AUTO_TILE, this, SLOT(slotAutoTile()), actionCollection(), "windowAutoTile_action");
	m_windowAutoTile_action->setToolTip( TT_WINDOW_AUTO_TILE );	
	m_windowAutoTile_action->setWhatsThis( WT_WINDOW_AUTO_TILE );
	m_windowAutoTile_action->plugAccel( m_keyAccel );	
	
	m_windowCloseAll_action = new KAction(i18n("&Close all"), ICON_WINDOW_CLOSE_ALL,
																IDK_WINDOW_CLOSE_ALL, m_mdi, SLOT(deleteAll()), actionCollection(),"windowCloseAll_action");
	m_windowCloseAll_action->setToolTip( TT_WINDOW_CLOSE_ALL );	
	m_windowCloseAll_action->setWhatsThis( WT_WINDOW_CLOSE_ALL );
	m_windowCloseAll_action->plugAccel( m_keyAccel );	
	
	m_windowSaveProfile_action = new KActionMenu(i18n("&Save profile"), ICON_WINDOW_SAVE_PROFILE, actionCollection(),"windowSaveProfile_action");
	m_windowSaveProfile_action->setToolTip( TT_WINDOW_SAVE_PROFILE );	
	m_windowSaveProfile_action->setWhatsThis( WT_WINDOW_SAVE_PROFILE );
	m_windowSaveProfile_action->plugAccel( m_keyAccel );	

	m_windowLoadProfile_action = new KActionMenu(i18n("&Load profile"), ICON_WINDOW_SAVE_PROFILE, actionCollection(),"windowLoadProfile_action");
	m_windowLoadProfile_action->setToolTip( TT_WINDOW_SAVE_PROFILE );	
	m_windowLoadProfile_action->setWhatsThis( WT_WINDOW_SAVE_PROFILE );
	m_windowLoadProfile_action->plugAccel( m_keyAccel );

	m_windowEditProfiles_action = new KAction(i18n("&Configure profiles"), ICON_WINDOW_EDIT_PROFILES,
																IDK_WINDOW_EDIT_PROFILES, this, SLOT(editProfiles()), actionCollection(),"windowEditProfiles_action");
	m_windowEditProfiles_action->setToolTip( TT_WINDOW_SAVE_PROFILE );	
	m_windowEditProfiles_action->setWhatsThis( WT_WINDOW_SAVE_PROFILE );
	m_windowEditProfiles_action->plugAccel( m_keyAccel );	

	m_windowFullscreen_action = new KToggleAction(i18n("&Fullscreen mode"), ICON_WINDOW_FULLSCREEN,
																IDK_WINDOW_FULLSCREEN, this, SLOT(toggleFullscreen()), actionCollection(),"windowFullscreen_action");
	m_windowFullscreen_action->setToolTip( TT_WINDOW_FULLSCREEN );	
	m_windowFullscreen_action->setWhatsThis( WT_WINDOW_FULLSCREEN );
	m_windowFullscreen_action->plugAccel( m_keyAccel );			
		
	QPtrList<CProfile> profiles = m_profileMgr.profiles();		
	KPopupMenu* loadPopup = m_windowLoadProfile_action->popupMenu();
	KPopupMenu* savePopup = m_windowSaveProfile_action->popupMenu();	
	connect(loadPopup, SIGNAL(activated(int)), SLOT(loadProfile(int)));
	connect(savePopup, SIGNAL(activated(int)), SLOT(saveProfile(int)));
	for (CProfile* p = profiles.first(); p; p = profiles.next()) {
		savePopup->insertItem(p->name());			
		loadPopup->insertItem(p->name());
	}
	
	if ( actionCollection()->action("help_contents") )	 //delete help action if KDE created it
		actionCollection()->remove(actionCollection()->action("help_contents"));
		
	action = new KAction(i18n("&Handbook"), ICON_HELP_CONTENTS, 0, this,
		SLOT(openOnlineHelp_Handbook()), actionCollection(), "help_handbook");
  action->setToolTip( TT_HELP_HANDBOOK );
  action->setWhatsThis( WT_HELP_HANDBOOK  );
	action->plugAccel( m_keyAccel );

	action = new KAction(i18n("&Installation"), ICON_HELP_CONTENTS, 0, this,
		SLOT(openOnlineHelp_Install()), actionCollection(), "help_install");
  action->setToolTip( TT_HELP_INSTALLATION );
  action->setWhatsThis( WT_HELP_INSTALLATION  );
	action->plugAccel( m_keyAccel );

	action = new KAction(i18n("&Bible Study Howto"), ICON_HELP_CONTENTS, 0, this,
		SLOT(openOnlineHelp_Howto()), actionCollection(), "help_howto");
  action->setToolTip( TT_HELP_BIBLESTUDY );
  action->setWhatsThis( WT_HELP_BIBLESTUDY  );
	action->plugAccel( m_keyAccel );

	action = KStdAction::whatsThis(this, SLOT(whatsThis()), actionCollection());
	action->setToolTip(TT_HELP_WHATSTHIS);
	action->setWhatsThis(WT_HELP_WHATSTHIS);	
	
	action = KStdAction::reportBug(m_helpMenu, SLOT(reportBug()), actionCollection());	
	action->setToolTip(TT_HELP_BUGREPORT);
	action->setWhatsThis(WT_HELP_BUGREPORT);
	action->setIcon(ICON_BUG_REPORT);
	
	action = new KAction(i18n("&Daily tip"), ICON_HELP_DAILY_TIP, IDK_HELP_DAILY_TIP, this,
		SLOT(slotHelpTipOfDay()), actionCollection(), "helpDailyTip_action");
	action->setToolTip(TT_HELP_DAILY_TIP);
	action->setWhatsThis(WT_HELP_DAILY_TIP);	

	action = KStdAction::aboutApp(m_helpMenu, SLOT(aboutApplication()), actionCollection());	
	action->setToolTip(TT_HELP_ABOUT);
	action->setWhatsThis(WT_HELP_ABOUT);	
	
	//delete About KDE action if KDE created it already
	if ( actionCollection()->action("help_about_kde") )	 //delete About KDE action if KDE created it
		actionCollection()->remove(actionCollection()->action("help_about_kde"));	
	action = KStdAction::aboutKDE(m_helpMenu, SLOT(aboutKDE()), actionCollection());	
	action->setToolTip(TT_HELP_ABOUT_KDE);
	action->setWhatsThis(WT_HELP_ABOUT_KDE);

//	action = KStdAction::keybindings(m_helpMenu, SLOT(aboutApplication()), actionCollection());	
//	action->setToolTip(TT_HELP_ABOUT);
//	action->setWhatsThis(WT_HELP_ABOUT);	
}

/** Initializes the menubar of BibleTime. */
void BibleTime::initMenubar(){
	//get the window and edit menus using the actions and their properties
	m_windowMenu = dynamic_cast<QPopupMenu*>(m_windowTile_action->container(0));	
}

/** Initializes the SIGNAL / SLOT connections */
void BibleTime::initConnections(){
	connect(m_mdi, SIGNAL(sigSetToplevelCaption(const QString&)),
		this, SLOT(setPlainCaption(const QString&)));
	connect(m_mdi, SIGNAL(createDisplayWindow(ListCSwordModuleInfo, const QString&)),
		this, SLOT(createDisplayWindow(ListCSwordModuleInfo, const QString&)));
		
	if (m_windowMenu) {
		connect(m_windowMenu, SIGNAL(aboutToShow()),
			this, SLOT(slotWindowMenuAboutToShow()));
		connect(m_windowMenu, SIGNAL(activated(int)),
			this, SLOT(slotWindowMenuActivated(int)));		
	}
	else
		qWarning("can't find window menu");
		
 	connect(m_mainIndex, SIGNAL(modulesChosen(ListCSwordModuleInfo, const QString&)),
		this, SLOT(createDisplayWindow(ListCSwordModuleInfo,const QString&)));
	
	//connect to the signals of the printer object
	connect(m_printer, SIGNAL(addedFirstQueueItem()),
		this, SLOT(slotSetPrintingStatus()));
	connect(m_printer, SIGNAL(printingStarted()),
		this, SLOT(slotPrintingStarted()));		
	connect(m_printer, SIGNAL(printingFinished()),
		this, SLOT(slotSetPrintingStatus()));		
	connect(m_printer, SIGNAL(printingFinished()),
		this, SLOT(slotPrintingFinished()));				
	connect(m_printer, SIGNAL(printingInterrupted()),
		this, SLOT(slotSetPrintingStatus()));				
	connect(m_printer, SIGNAL(printingInterrupted()),
		this, SLOT(slotPrintingFinished()));						
	connect(m_printer, SIGNAL(queueCleared()),
		this, SLOT(slotSetPrintingStatus()));
	connect(m_printer, SIGNAL(percentCompleted(const int)),
		this, SLOT(slotPrintedPercent(const int)));		
}

/** Initializes the backend */
void BibleTime::initBackends(){
	KStartupLogo::setStatusMessage(i18n("Initializing Sword")+QString::fromLatin1("..."));
	
	m_backend = new CSwordBackend();	
	CPointers::setBackend(m_backend);
	const CSwordBackend::LoadError errorCode = m_backend->initModules();
//	qWarning("ErrorCode = %i", errorCode);

	m_moduleList = 0;		
	if ( errorCode == CSwordBackend::NoError ) {	//no error
		m_moduleList = &(m_backend->moduleList());
	} else {
		m_moduleList = 0;
		//show error message that initBackend failed		
		switch (errorCode) {
			case CSwordBackend::NoSwordConfig: //mods.d or mods.conf missing
			{
				KStartupLogo::hideSplash();
				CHTMLDialog dlg(HELPDIALOG_NO_SWORD_MODULE_CONFIG_DIR);
				dlg.exec();				
				KStartupLogo::showSplash();				
				break;
	    }
	
			case CSwordBackend::NoModules: //no modules installed, but config exists
			{
				KStartupLogo::hideSplash();
				CHTMLDialog dlg(HELPDIALOG_NO_SWORD_MODULES);
				dlg.exec();
				KStartupLogo::showSplash();				
				break;
		  }
		
			default: //unknown error
			{
				KStartupLogo::hideSplash();
				CHTMLDialog dlg(HELPDIALOG_INITBACKEND_FAILED);
				dlg.exec();
				KStartupLogo::showSplash();				
				break;
		  }
		}
	}
	m_backend->booknameLanguage( CBTConfig::get(CBTConfig::language) );
}

/** Initializes the CPrinter object. */
void BibleTime::initPrinter() {
	KStartupLogo::setStatusMessage(i18n("Initializing printing system") + QString::fromLatin1("..."));
	CPointers::setPrinter( (m_printer = new CPrinter(this)) );
}

/** Apply the settings given by the profile p*/
void BibleTime::applyProfileSettings( CProfile* p ){
	if (!p)
		return;

	if (p->fullscreen()) { //we can set only fullscreen OR geometry
		m_windowFullscreen_action->setChecked( true );
		toggleFullscreen();
	}
	else {
		setGeometry( p->geometry() );
		m_windowFullscreen_action->setChecked( false );
		toggleFullscreen();		
	}
}

/** Stores the settings of the mainwindow in the profile p */
void BibleTime::storeProfileSettings( CProfile* p ){
	p->setFullscreen(m_windowFullscreen_action->isChecked());
	p->setGeometry(geometry());
}
