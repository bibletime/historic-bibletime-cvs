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

#include "frontend/mainindex/cmainindex.h"
#include "frontend/chtmldialog.h"
#include "frontend/cprofilemgr.h"
#include "frontend/cprofile.h"
#include "frontend/cmdiarea.h"
#include "frontend/kstartuplogo.h"
#include "frontend/chelpmgr.h"

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
  qWarning("BibleTime::initView()");
	KStartupLogo::setStatusMessage(i18n("Creating BibleTime's GUI") + QString::fromLatin1("..."));	
	
	m_splitter = new QSplitter(this, "mainsplitter");
	setCentralWidget(m_splitter);	
	
	m_mainIndex = new CMainIndex( centralWidget() );	
	m_mainIndex->initTree();
	m_mainIndex->setFocusPolicy(WheelFocus);       	
	

	m_mdi = new CMDIArea(centralWidget(), "mdiarea" );
	m_mdi->setFocusPolicy(ClickFocus);

	m_helpMenu = new KHelpMenu(this, KGlobal::instance()->aboutData(), true, actionCollection());
	
	m_selectDialog = 0;
}


/** Initializes the action objects of the GUI */
void BibleTime::initActions() {
	KStartupLogo::setStatusMessage(i18n("Initializing menu- and toolbars") + QString::fromLatin1("..."));
	KAction* action = 0;

  
	m_fileClearQueue_action = new KAction(i18n("Clear printing queue"),
    CHelpMgr::mainMenu::file::clearQueue::icon,
    CHelpMgr::mainMenu::file::clearQueue::accel,
    m_printer, SLOT(clearQueue()), actionCollection(),
    CHelpMgr::mainMenu::file::clearQueue::actionName
  );
	m_fileClearQueue_action->setEnabled(false);	
	m_fileClearQueue_action->setToolTip( CHelpMgr::mainMenu::file::clearQueue::tooltip );
	m_fileClearQueue_action->setWhatsThis( CHelpMgr::mainMenu::file::clearQueue::whatsthis );
	m_fileClearQueue_action->plugAccel( m_keyAccel );	
	

  m_filePrint_action = KStdAction::print(this, SLOT( slotFilePrint() ), actionCollection());
	m_filePrint_action->setEnabled(false);		
	m_filePrint_action->setToolTip( CHelpMgr::mainMenu::file::print::tooltip );
	m_filePrint_action->setWhatsThis( CHelpMgr::mainMenu::file::print::whatsthis );
	m_filePrint_action->plugAccel( m_keyAccel );	
	

  action = KStdAction::quit(this, SLOT( slotFileQuit() ), actionCollection());
	action->setToolTip( CHelpMgr::mainMenu::file::quit::tooltip );	
	action->setWhatsThis( CHelpMgr::mainMenu::file::quit::whatsthis );
	action->plugAccel( m_keyAccel );
												

  action =  new KAction(i18n("Search in module(s)"),
    CHelpMgr::mainMenu::mainIndex::search::icon,
    CHelpMgr::mainMenu::mainIndex::search::accel,
    this, SLOT(slotSearchSelectedModules()), actionCollection(),
    CHelpMgr::mainMenu::mainIndex::search::actionName
  );
  action->setToolTip( CHelpMgr::mainMenu::mainIndex::search::tooltip );
	action->setWhatsThis( CHelpMgr::mainMenu::mainIndex::search::whatsthis );


  m_viewToolbar_action = KStdAction::showToolbar(this, SLOT( slotToggleToolbar() ), actionCollection());
	m_viewToolbar_action->setToolTip( CHelpMgr::mainMenu::view::showToolBar::tooltip );	
	m_viewToolbar_action->setWhatsThis( CHelpMgr::mainMenu::view::showToolBar::whatsthis );
	m_viewToolbar_action->plugAccel( m_keyAccel );


  m_viewMainIndex_action = new KToggleAction(i18n("&Show main index"),
    CHelpMgr::mainMenu::view::showMainIndex::icon,
    CHelpMgr::mainMenu::view::showMainIndex::accel,
		this, SLOT(slotToggleGroupManager()), actionCollection(),
    CHelpMgr::mainMenu::view::showMainIndex::actionName);
	m_viewMainIndex_action->setToolTip( CHelpMgr::mainMenu::view::showMainIndex::tooltip );	
	m_viewMainIndex_action->setWhatsThis( CHelpMgr::mainMenu::view::showMainIndex::whatsthis );
	

  action = KStdAction::preferences(this, SLOT( slotSettingsOptions() ), actionCollection());
	action->setToolTip( CHelpMgr::mainMenu::settings::optionsDialog::tooltip );
	action->setWhatsThis( CHelpMgr::mainMenu::settings::optionsDialog::whatsthis );
	action->plugAccel( m_keyAccel );


  action = KStdAction::configureToolbars(this, SLOT( slotSettingsToolbar() ), actionCollection());
	action->setToolTip( CHelpMgr::mainMenu::settings::editToolBar::tooltip );	
	action->setWhatsThis( CHelpMgr::mainMenu::settings::editToolBar::whatsthis );
	action->plugAccel( m_keyAccel );


  m_windowCascade_action = new KAction(i18n("&Cascade"),
    CHelpMgr::mainMenu::window::cascade::icon,
    CHelpMgr::mainMenu::window::cascade::accel,
		m_mdi, SLOT(cascade()), actionCollection(),
    CHelpMgr::mainMenu::window::cascade::actionName
  );
	m_windowCascade_action->setToolTip( CHelpMgr::mainMenu::window::cascade::tooltip );
	m_windowCascade_action->setWhatsThis( CHelpMgr::mainMenu::window::cascade::whatsthis );
	m_windowCascade_action->plugAccel( m_keyAccel);	
	

  m_windowTile_action = new KAction(i18n("&Tile"),
    CHelpMgr::mainMenu::window::tile::icon,
    CHelpMgr::mainMenu::window::tile::accel,
    m_mdi, SLOT(tile()), actionCollection(),
    CHelpMgr::mainMenu::window::tile::actionName);
	m_windowTile_action->setToolTip( CHelpMgr::mainMenu::window::tile::tooltip );	
	m_windowTile_action->setWhatsThis( CHelpMgr::mainMenu::window::tile::whatsthis );
	m_windowTile_action->plugAccel( m_keyAccel );	

  	
	m_windowAutoCascade_action 	= new KToggleAction(i18n("&Auto cascade"),
    CHelpMgr::mainMenu::window::autoCascade::icon,
		CHelpMgr::mainMenu::window::autoCascade::accel,
    this, SLOT(slotAutoCascade()), actionCollection(),
    CHelpMgr::mainMenu::window::autoCascade::actionName
  );
	m_windowAutoCascade_action->setToolTip( CHelpMgr::mainMenu::window::autoCascade::tooltip );	
	m_windowAutoCascade_action->setWhatsThis( CHelpMgr::mainMenu::window::autoCascade::whatsthis );
	m_windowAutoCascade_action->plugAccel( m_keyAccel );	


	m_windowAutoTile_action = new KToggleAction(i18n("A&uto Tile"),
    CHelpMgr::mainMenu::window::autoTile::icon,
		CHelpMgr::mainMenu::window::autoTile::accel,
    this, SLOT(slotAutoTile()), actionCollection(),
    CHelpMgr::mainMenu::window::autoTile::actionName
  );
	m_windowAutoTile_action->setToolTip( CHelpMgr::mainMenu::window::autoTile::tooltip );	
	m_windowAutoTile_action->setWhatsThis( CHelpMgr::mainMenu::window::autoTile::whatsthis );
	m_windowAutoTile_action->plugAccel( m_keyAccel );	
	

  m_windowCloseAll_action = new KAction(i18n("&Close all"),
    CHelpMgr::mainMenu::window::closeAll::icon,
    CHelpMgr::mainMenu::window::closeAll::accel,
    m_mdi, SLOT(deleteAll()), actionCollection(),
    CHelpMgr::mainMenu::window::closeAll::actionName
  );
	m_windowCloseAll_action->setToolTip( CHelpMgr::mainMenu::window::closeAll::tooltip );	
	m_windowCloseAll_action->setWhatsThis( CHelpMgr::mainMenu::window::closeAll::whatsthis );
	m_windowCloseAll_action->plugAccel( m_keyAccel );	

  
	m_windowSaveProfile_action = new KActionMenu(i18n("&Save profile"),
    CHelpMgr::mainMenu::window::saveProfile::icon,
    actionCollection(),
    CHelpMgr::mainMenu::window::saveProfile::actionName
  );
	m_windowSaveProfile_action->setToolTip( CHelpMgr::mainMenu::window::saveProfile::tooltip );	
	m_windowSaveProfile_action->setWhatsThis( CHelpMgr::mainMenu::window::saveProfile::whatsthis );
	m_windowSaveProfile_action->plugAccel( m_keyAccel );	

	m_windowLoadProfile_action = new KActionMenu(i18n("&Load profile"),
    CHelpMgr::mainMenu::window::loadProfile::icon,
    actionCollection(),
    CHelpMgr::mainMenu::window::loadProfile::actionName
  );
	m_windowLoadProfile_action->setToolTip( CHelpMgr::mainMenu::window::loadProfile::tooltip );	
	m_windowLoadProfile_action->setWhatsThis( CHelpMgr::mainMenu::window::loadProfile::whatsthis );
	m_windowLoadProfile_action->plugAccel( m_keyAccel );

  
	m_windowEditProfiles_action = new KAction(i18n("&Configure profiles"),
    CHelpMgr::mainMenu::window::setupProfiles::icon,
    CHelpMgr::mainMenu::window::setupProfiles::accel,
    this, SLOT(editProfiles()), actionCollection(),
    CHelpMgr::mainMenu::window::setupProfiles::actionName
  );
	m_windowEditProfiles_action->setToolTip( CHelpMgr::mainMenu::window::setupProfiles::tooltip );	
	m_windowEditProfiles_action->setWhatsThis( CHelpMgr::mainMenu::window::setupProfiles::whatsthis );
	m_windowEditProfiles_action->plugAccel( m_keyAccel );	

  
	m_windowFullscreen_action = new KToggleAction(i18n("&Fullscreen mode"),
    CHelpMgr::mainMenu::window::showFullscreen::icon,
		CHelpMgr::mainMenu::window::showFullscreen::accel,
    this, SLOT(toggleFullscreen()), actionCollection(),
    CHelpMgr::mainMenu::window::showFullscreen::actionName
  );
	m_windowFullscreen_action->setToolTip( CHelpMgr::mainMenu::window::showFullscreen::tooltip );	
	m_windowFullscreen_action->setWhatsThis( CHelpMgr::mainMenu::window::showFullscreen::whatsthis );
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
  action->setToolTip( CHelpMgr::mainMenu::help::handbook::tooltip );
  action->setWhatsThis( CHelpMgr::mainMenu::help::handbook::whatsthis  );
	action->plugAccel( m_keyAccel );

	action = new KAction(i18n("&Installation"), ICON_HELP_CONTENTS, 0, this,
		SLOT(openOnlineHelp_Install()), actionCollection(), "help_install");
  action->setToolTip( CHelpMgr::mainMenu::help::installation::tooltip );
  action->setWhatsThis( CHelpMgr::mainMenu::help::installation::whatsthis  );
	action->plugAccel( m_keyAccel );

	action = new KAction(i18n("&Bible Study Howto"), ICON_HELP_CONTENTS, 0, this,
		SLOT(openOnlineHelp_Howto()), actionCollection(), "help_howto");
  action->setToolTip( CHelpMgr::mainMenu::help::bibleStudyHowTo::tooltip );
  action->setWhatsThis( CHelpMgr::mainMenu::help::bibleStudyHowTo::whatsthis  );
	action->plugAccel( m_keyAccel );

	action = KStdAction::whatsThis(this, SLOT(whatsThis()), actionCollection());
	action->setToolTip(CHelpMgr::mainMenu::help::whatsThis::tooltip);
	action->setWhatsThis(CHelpMgr::mainMenu::help::whatsThis::whatsthis);	
	
	action = KStdAction::reportBug(m_helpMenu, SLOT(reportBug()), actionCollection());	
	action->setToolTip(CHelpMgr::mainMenu::help::bugreport::tooltip);
	action->setWhatsThis(CHelpMgr::mainMenu::help::bugreport::whatsthis);
	action->setIcon(ICON_BUG_REPORT);
	
	action = new KAction(i18n("&Daily tip"), ICON_HELP_DAILY_TIP, IDK_HELP_DAILY_TIP, this,
		SLOT(slotHelpTipOfDay()), actionCollection(), "helpDailyTip_action");
	action->setToolTip(CHelpMgr::mainMenu::help::dailyTip::tooltip);
	action->setWhatsThis(CHelpMgr::mainMenu::help::dailyTip::whatsthis);	

	action = KStdAction::aboutApp(m_helpMenu, SLOT(aboutApplication()), actionCollection());	
	action->setToolTip(CHelpMgr::mainMenu::help::aboutBibleTime::tooltip);
	action->setWhatsThis(CHelpMgr::mainMenu::help::aboutBibleTime::whatsthis);	
	
	//delete About KDE action if KDE created it already
	if ( actionCollection()->action("help_about_kde") )	 //delete About KDE action if KDE created it
		actionCollection()->remove(actionCollection()->action("help_about_kde"));	
	action = KStdAction::aboutKDE(m_helpMenu, SLOT(aboutKDE()), actionCollection());	
	action->setToolTip(CHelpMgr::mainMenu::help::aboutKDE::tooltip);
	action->setWhatsThis(CHelpMgr::mainMenu::help::aboutKDE::whatsthis);
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
	connect(m_mdi, SIGNAL(createReadDisplayWindow(ListCSwordModuleInfo, const QString&)),
		this, SLOT(createReadDisplayWindow(ListCSwordModuleInfo, const QString&)));
		
	if (m_windowMenu) {
		connect(m_windowMenu, SIGNAL(aboutToShow()),
			this, SLOT(slotWindowMenuAboutToShow()));
		connect(m_windowMenu, SIGNAL(activated(int)),
			this, SLOT(slotWindowMenuActivated(int)));		
	}
	else
		qWarning("can't find window menu");
		 	
	
	connect(m_mainIndex, SIGNAL(createReadDisplayWindow(ListCSwordModuleInfo, const QString&)),
		this, SLOT(createReadDisplayWindow(ListCSwordModuleInfo,const QString&)));
	connect(m_mainIndex, SIGNAL(createWriteDisplayWindow(CSwordModuleInfo*, const QString&)),
		this, SLOT(createWriteDisplayWindow(CSwordModuleInfo*,const QString&)));
    
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
