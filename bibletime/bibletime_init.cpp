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

#include "backend/cswordbackend.h"

#include "frontend/mainindex/cmainindex.h"
#include "frontend/cprofilemgr.h"
#include "frontend/cprofile.h"
#include "frontend/cmdiarea.h"
#include "frontend/kstartuplogo.h"
#include "frontend/cswordsetupdialog.h"
#include "frontend/cprinter.h"

#include "backend/clanguagemgr.h"


#include "config.h"
#include "frontend/cbtconfig.h"
#include "frontend/cinfodisplay.h"

#include "util/cpointers.h"
#include "util/cresmgr.h"

//system includes
#include <stdlib.h>

//QT includes
#include <qpopupmenu.h>
#include <qsplitter.h>
#include <qguardedptr.h>
#include <qlistview.h>

//KDE includes
#include <kaboutdata.h>
#include <kapplication.h>
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

//Sword includes
#include <swlog.h>


/**Initializes the view of this widget*/
void BibleTime::initView(){
	KStartupLogo::setStatusMessage(i18n("Creating BibleTime's GUI") + QString::fromLatin1("..."));	

	m_splitter = new QSplitter(this, "mainsplitter");
//  m_splitter->setOpaqueResize( true );
	setCentralWidget(m_splitter);	

	QSplitter* mainIndexSplitter = new QSplitter(Qt::Vertical, m_splitter);
	
	m_mainIndex = new CMainIndex(mainIndexSplitter);
	m_mainIndex->initTree();
	m_mainIndex->setFocusPolicy(WheelFocus);       	

	m_infoDisplay = new CInfoDisplay(mainIndexSplitter);
	CPointers::setInfoDisplay(m_infoDisplay);
	
	m_mdi = new CMDIArea(m_splitter, "mdiarea" );
	m_mdi->setFocusPolicy(ClickFocus);

	m_helpMenu = new KHelpMenu(this, KGlobal::instance()->aboutData(), true, actionCollection());
}

/** Initializes the action objects of the GUI */
void BibleTime::initActions() {
	KStartupLogo::setStatusMessage(i18n("Initializing menu- and toolbars") + QString::fromLatin1("..."));
	KAction* action = 0;

  action = KStdAction::quit(kapp, SLOT( quit() ), actionCollection());
	action->setToolTip( CResMgr::mainMenu::file::quit::tooltip );
	action->setWhatsThis( CResMgr::mainMenu::file::quit::whatsthis );
  #if KDE_VERSION_MINOR < 1
    action->plugAccel( accel() );
  #endif

  action = new KAction(i18n("Search in open module(s)"),
    CResMgr::mainMenu::mainIndex::search::icon,
    CResMgr::mainMenu::mainIndex::search::accel,
    this, SLOT(slotSearchModules()), actionCollection(),
    CResMgr::mainMenu::mainIndex::search::actionName
  );
  action->setToolTip( CResMgr::mainMenu::mainIndex::search::tooltip );
  action->setWhatsThis( CResMgr::mainMenu::mainIndex::search::whatsthis );
  #if KDE_VERSION_MINOR < 1
    action->plugAccel( accel() );
  #endif

  action = new KAction(i18n("Search in default bible"),
    CResMgr::mainMenu::mainIndex::searchdefaultbible::icon,
    CResMgr::mainMenu::mainIndex::searchdefaultbible::accel,
    this, SLOT(slotSearchDefaultBible()), actionCollection(),
    CResMgr::mainMenu::mainIndex::searchdefaultbible::actionName
  );
  action->setToolTip( CResMgr::mainMenu::mainIndex::searchdefaultbible::tooltip );
  action->setWhatsThis( CResMgr::mainMenu::mainIndex::searchdefaultbible::whatsthis );
  #if KDE_VERSION_MINOR < 1
  	action->plugAccel( accel() );
  #endif

  m_viewToolbar_action = KStdAction::showToolbar(this, SLOT( slotToggleToolbar() ), actionCollection());
	m_viewToolbar_action->setToolTip( CResMgr::mainMenu::view::showToolBar::tooltip );
	m_viewToolbar_action->setWhatsThis( CResMgr::mainMenu::view::showToolBar::whatsthis );
  #if KDE_VERSION_MINOR < 1
  	m_viewToolbar_action->plugAccel( accel() );
  #endif


  m_viewMainIndex_action = new KToggleAction(i18n("&Show main index"),
    CResMgr::mainMenu::view::showMainIndex::icon,
    CResMgr::mainMenu::view::showMainIndex::accel,
		this, SLOT(slotToggleMainIndex()), actionCollection(),
    CResMgr::mainMenu::view::showMainIndex::actionName);
	m_viewMainIndex_action->setToolTip( CResMgr::mainMenu::view::showMainIndex::tooltip );
	m_viewMainIndex_action->setWhatsThis( CResMgr::mainMenu::view::showMainIndex::whatsthis );
  #if KDE_VERSION_MINOR < 1
    m_viewMainIndex_action->plugAccel( accel() );
  #endif

  m_viewInfoDisplay_action = new KToggleAction(i18n("&Show info display"),
    CResMgr::mainMenu::view::showInfoDisplay::icon,
    CResMgr::mainMenu::view::showInfoDisplay::accel,
		this, SLOT(slotToggleInfoDisplay()), actionCollection(),
    CResMgr::mainMenu::view::showInfoDisplay::actionName);
	m_viewMainIndex_action->setToolTip( CResMgr::mainMenu::view::showInfoDisplay::tooltip );
	m_viewMainIndex_action->setWhatsThis( CResMgr::mainMenu::view::showInfoDisplay::whatsthis );
  #if KDE_VERSION_MINOR < 1
    m_viewInfoDisplay_action->plugAccel( accel() );
  #endif

  action = KStdAction::preferences(this, SLOT( slotSettingsOptions() ), actionCollection());
	action->setToolTip( CResMgr::mainMenu::settings::optionsDialog::tooltip );
	action->setWhatsThis( CResMgr::mainMenu::settings::optionsDialog::whatsthis );
  #if KDE_VERSION_MINOR < 1
  	action->plugAccel( accel() );
  #endif

  action = new KAction(i18n("Configure &Sword"),
    CResMgr::mainMenu::settings::swordSetupDialog::icon,
    CResMgr::mainMenu::settings::swordSetupDialog::accel,
		this, SLOT( slotSwordSetupDialog() ), actionCollection(),
    CResMgr::mainMenu::settings::swordSetupDialog::actionName
  );
	action->setToolTip( CResMgr::mainMenu::settings::swordSetupDialog::tooltip );
	action->setWhatsThis( CResMgr::mainMenu::settings::swordSetupDialog::whatsthis );
  #if KDE_VERSION_MINOR < 1
    action->plugAccel( accel() );
  #endif

  action = KStdAction::configureToolbars(this, SLOT( slotSettingsToolbar() ), actionCollection());
	action->setToolTip( CResMgr::mainMenu::settings::editToolBar::tooltip );
	action->setWhatsThis( CResMgr::mainMenu::settings::editToolBar::whatsthis );
  #if KDE_VERSION_MINOR < 1
    action->plugAccel( accel() );
  #endif

  m_windowCascade_action = new KAction(i18n("&Cascade"),
    CResMgr::mainMenu::window::cascade::icon,
    CResMgr::mainMenu::window::cascade::accel,
		m_mdi, SLOT(myCascade()), actionCollection(),
    CResMgr::mainMenu::window::cascade::actionName
  );
	m_windowCascade_action->setToolTip( CResMgr::mainMenu::window::cascade::tooltip );
	m_windowCascade_action->setWhatsThis( CResMgr::mainMenu::window::cascade::whatsthis );
  #if KDE_VERSION_MINOR < 1
  	m_windowCascade_action->plugAccel( accel());
  #endif


  m_windowTile_action = new KAction(i18n("&Tile"),
    CResMgr::mainMenu::window::tile::icon,
    CResMgr::mainMenu::window::tile::accel,
    m_mdi, SLOT(myTile()), actionCollection(),
    CResMgr::mainMenu::window::tile::actionName
	);
	m_windowTile_action->setToolTip( CResMgr::mainMenu::window::tile::tooltip );
	m_windowTile_action->setWhatsThis( CResMgr::mainMenu::window::tile::whatsthis );
  #if KDE_VERSION_MINOR < 1
  	m_windowTile_action->plugAccel( accel() );
  #endif


	m_windowAutoCascade_action 	= new KToggleAction(i18n("&Auto cascade"),
    CResMgr::mainMenu::window::autoCascade::icon,
		CResMgr::mainMenu::window::autoCascade::accel,
    this, SLOT(slotAutoCascade()), actionCollection(),
    CResMgr::mainMenu::window::autoCascade::actionName
  );
	m_windowAutoCascade_action->setToolTip( CResMgr::mainMenu::window::autoCascade::tooltip );
	m_windowAutoCascade_action->setWhatsThis( CResMgr::mainMenu::window::autoCascade::whatsthis );
  #if KDE_VERSION_MINOR < 1
  	m_windowAutoCascade_action->plugAccel( accel() );
  #endif


	m_windowAutoTile_action = new KToggleAction(i18n("A&uto Tile"),
    CResMgr::mainMenu::window::autoTile::icon,
		CResMgr::mainMenu::window::autoTile::accel,
    this, SLOT(slotAutoTile()), actionCollection(),
    CResMgr::mainMenu::window::autoTile::actionName
  );
	m_windowAutoTile_action->setToolTip( CResMgr::mainMenu::window::autoTile::tooltip );
	m_windowAutoTile_action->setWhatsThis( CResMgr::mainMenu::window::autoTile::whatsthis );
  #if KDE_VERSION_MINOR < 1
  	m_windowAutoTile_action->plugAccel( accel() );
  #endif


  m_windowCloseAll_action = new KAction(i18n("&Close all"),
    CResMgr::mainMenu::window::closeAll::icon,
    CResMgr::mainMenu::window::closeAll::accel,
    m_mdi, SLOT(deleteAll()), actionCollection(),
    CResMgr::mainMenu::window::closeAll::actionName
  );
	m_windowCloseAll_action->setToolTip( CResMgr::mainMenu::window::closeAll::tooltip );
	m_windowCloseAll_action->setWhatsThis( CResMgr::mainMenu::window::closeAll::whatsthis );
  #if KDE_VERSION_MINOR < 1
  	m_windowCloseAll_action->plugAccel( accel() );
  #endif


	m_windowSaveProfile_action = new KActionMenu(i18n("&Save session"),
    CResMgr::mainMenu::window::saveProfile::icon,
    actionCollection(),
    CResMgr::mainMenu::window::saveProfile::actionName
  );
	m_windowSaveProfile_action->setToolTip( CResMgr::mainMenu::window::saveProfile::tooltip );
	m_windowSaveProfile_action->setWhatsThis( CResMgr::mainMenu::window::saveProfile::whatsthis );
  #if KDE_VERSION_MINOR < 1                           
  	m_windowSaveProfile_action->plugAccel( accel() );
  #endif


  m_windowSaveToNewProfile_action = new KAction(i18n("&Save as new session"),
    CResMgr::mainMenu::window::saveToNewProfile::icon,
    CResMgr::mainMenu::window::saveToNewProfile::accel,
    this, SLOT(saveToNewProfile()), actionCollection(),
    CResMgr::mainMenu::window::saveToNewProfile::actionName
  );
	m_windowSaveToNewProfile_action->setToolTip( CResMgr::mainMenu::window::saveToNewProfile::tooltip );
	m_windowSaveToNewProfile_action->setWhatsThis( CResMgr::mainMenu::window::saveToNewProfile::whatsthis );
  #if KDE_VERSION_MINOR < 1
  	m_windowSaveToNewProfile_action->plugAccel( accel() );
  #endif
  

	m_windowLoadProfile_action = new KActionMenu(i18n("&Load session"),
    CResMgr::mainMenu::window::loadProfile::icon,
    actionCollection(),
    CResMgr::mainMenu::window::loadProfile::actionName
  );
	m_windowLoadProfile_action->setToolTip( CResMgr::mainMenu::window::loadProfile::tooltip );
	m_windowLoadProfile_action->setWhatsThis( CResMgr::mainMenu::window::loadProfile::whatsthis );
  #if KDE_VERSION_MINOR < 1
  	m_windowLoadProfile_action->plugAccel( accel() );
  #endif


	m_windowEditProfiles_action = new KAction(i18n("&Configure sessions"),
    CResMgr::mainMenu::window::setupProfiles::icon,
    CResMgr::mainMenu::window::setupProfiles::accel,
    this, SLOT(editProfiles()), actionCollection(),
    CResMgr::mainMenu::window::setupProfiles::actionName
  );
	m_windowEditProfiles_action->setToolTip( CResMgr::mainMenu::window::setupProfiles::tooltip );
	m_windowEditProfiles_action->setWhatsThis( CResMgr::mainMenu::window::setupProfiles::whatsthis );
  #if KDE_VERSION_MINOR < 1
  	m_windowEditProfiles_action->plugAccel( accel() );
  #endif


	m_windowFullscreen_action = new KToggleAction(i18n("&Fullscreen mode"),
    CResMgr::mainMenu::window::showFullscreen::icon,
		CResMgr::mainMenu::window::showFullscreen::accel,
    this, SLOT(toggleFullscreen()), actionCollection(),
    CResMgr::mainMenu::window::showFullscreen::actionName
  );
	m_windowFullscreen_action->setToolTip( CResMgr::mainMenu::window::showFullscreen::tooltip );
	m_windowFullscreen_action->setWhatsThis( CResMgr::mainMenu::window::showFullscreen::whatsthis );
	m_windowFullscreen_action->setShortcutConfigurable(true);
  #if KDE_VERSION_MINOR < 1
  	m_windowFullscreen_action->plugAccel( accel() );
  #endif

	QPtrList<CProfile> profiles = m_profileMgr.profiles();
	KPopupMenu* loadPopup = m_windowLoadProfile_action->popupMenu();
	KPopupMenu* savePopup = m_windowSaveProfile_action->popupMenu();
	connect(loadPopup, SIGNAL(activated(int)), SLOT(loadProfile(int)));
	connect(savePopup, SIGNAL(activated(int)), SLOT(saveProfile(int)));
	for (CProfile* p = profiles.first(); p; p = profiles.next()) {
		savePopup->insertItem(p->name());
		loadPopup->insertItem(p->name());
	}

	if ( actionCollection()->action( KStdAction::stdName(KStdAction::HelpContents) )) {	 //delete help action if KDE created it
		actionCollection()->remove(actionCollection()->action(KStdAction::stdName(KStdAction::HelpContents)) );
  }

  action = new KAction(i18n("&Handbook"),
    CResMgr::mainMenu::help::handbook::icon,
    CResMgr::mainMenu::help::handbook::accel,
    this, SLOT(openOnlineHelp_Handbook()), actionCollection(),
    CResMgr::mainMenu::help::handbook::actionName
  );
  action->setToolTip( CResMgr::mainMenu::help::handbook::tooltip );
  action->setWhatsThis( CResMgr::mainMenu::help::handbook::whatsthis  );
  #if KDE_VERSION_MINOR < 1
	  action->plugAccel( accel() );
  #endif

	action = new KAction(i18n("&Bible Study Howto"),
    CResMgr::mainMenu::help::bibleStudyHowTo::icon,
    CResMgr::mainMenu::help::bibleStudyHowTo::accel,
    this, SLOT(openOnlineHelp_Howto()), actionCollection(),
    CResMgr::mainMenu::help::bibleStudyHowTo::actionName
  );
  action->setToolTip( CResMgr::mainMenu::help::bibleStudyHowTo::tooltip );
  action->setWhatsThis( CResMgr::mainMenu::help::bibleStudyHowTo::whatsthis  );
  #if KDE_VERSION_MINOR < 1
	  action->plugAccel( accel() );
  #endif

	action = KStdAction::whatsThis(this, SLOT(whatsThis()), actionCollection());
	action->setToolTip(CResMgr::mainMenu::help::whatsThis::tooltip);
	action->setWhatsThis(CResMgr::mainMenu::help::whatsThis::whatsthis);
  #if KDE_VERSION_MINOR < 1
	action->plugAccel( accel() );
  #endif

	action = KStdAction::reportBug(m_helpMenu, SLOT(reportBug()), actionCollection());
	action->setToolTip(CResMgr::mainMenu::help::bugreport::tooltip);
	action->setWhatsThis(CResMgr::mainMenu::help::bugreport::whatsthis);
  #if KDE_VERSION_MINOR < 1
	  action->plugAccel( accel() );
  #endif
//	action->setIcon(ICON_BUG_REPORT);

	action = new KAction(i18n("&Daily tip"),
    CResMgr::mainMenu::help::dailyTip::icon,
    CResMgr::mainMenu::help::dailyTip::accel,
    this, SLOT(slotHelpTipOfDay()), actionCollection(),
    CResMgr::mainMenu::help::dailyTip::actionName
  );
	action->setToolTip(CResMgr::mainMenu::help::dailyTip::tooltip);
	action->setWhatsThis(CResMgr::mainMenu::help::dailyTip::whatsthis);
  #if KDE_VERSION_MINOR < 1
	  action->plugAccel( accel() );
  #endif
  
	if ( actionCollection()->action( KStdAction::stdName( KStdAction::AboutApp ) ) ) {	 //delete About KDE action if KDE created it
		actionCollection()->remove(actionCollection()->action(KStdAction::stdName( KStdAction::AboutApp )));
  }
  action = KStdAction::aboutApp(m_helpMenu, SLOT(aboutApplication()), actionCollection());
	action->setToolTip(CResMgr::mainMenu::help::aboutBibleTime::tooltip);
	action->setWhatsThis(CResMgr::mainMenu::help::aboutBibleTime::whatsthis);

	//delete About KDE action if KDE created it already
	if ( actionCollection()->action( KStdAction::stdName( KStdAction::AboutKDE ) ) ) {	 //delete About KDE action if KDE created it
		actionCollection()->remove(actionCollection()->action(KStdAction::stdName( KStdAction::AboutKDE )));
  }
	action = KStdAction::aboutKDE(m_helpMenu, SLOT(aboutKDE()), actionCollection());
	action->setToolTip(CResMgr::mainMenu::help::aboutKDE::tooltip);
	action->setWhatsThis(CResMgr::mainMenu::help::aboutKDE::whatsthis);
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
	else {
		qWarning("Main window: can't find window menu");
  }


	connect(m_mainIndex, SIGNAL(createReadDisplayWindow(ListCSwordModuleInfo, const QString&)),
		this, SLOT(createReadDisplayWindow(ListCSwordModuleInfo,const QString&)));
	connect(m_mainIndex, SIGNAL(createWriteDisplayWindow(CSwordModuleInfo*, const QString&, const CDisplayWindow::WriteWindowType&)),
		this, SLOT(createWriteDisplayWindow(CSwordModuleInfo*,const QString&, const CDisplayWindow::WriteWindowType&)));
    

//   //connect to the signals of the printer object
//   connect(m_printer, SIGNAL(addedFirstQueueItem()),
// 		this, SLOT(slotSetPrintingStatus()));
// 	connect(m_printer, SIGNAL(printingFinished()),
// 		this, SLOT(slotPrintingFinished()));
// 	connect(m_printer, SIGNAL(queueCleared()),
// 		this, SLOT(slotSetPrintingStatus()));

  //if we use KDE 3.1 we do not use our own printing status window, KDE >= 3.1 has a better one
// #ifdef KDE_VERSION_MINOR
// #if KDE_VERSION_MINOR < 1
//   connect(m_printer, SIGNAL(printingStarted()),
// 		this, SLOT(slotPrintingStarted()));		
// 	connect(m_printer, SIGNAL(printingFinished()),
// 		this, SLOT(slotSetPrintingStatus()));		
//   connect(m_printer, SIGNAL(printingInterrupted()),
// 		this, SLOT(slotPrintingFinished()));
// 	connect(m_printer, SIGNAL(percentCompleted(const int)),
// 		this, SLOT(slotPrintedPercent(const int)));
// #endif
// #endif

// 	connect(m_printer, SIGNAL(printingInterrupted()),
// 		this, SLOT(slotSetPrintingStatus()));

}

/** Initializes the backend */
void BibleTime::initBackends(){
	KStartupLogo::setStatusMessage(i18n("Initializing Sword") + QString::fromLatin1("..."));

//  qWarning("backend constructed with a path argument! %s", path.latin1());
	StringMgr::setSystemStringMgr( new BTStringMgr() );
	SWLog::getSystemLog()->setLogLevel(0);
	
	m_backend = new CSwordBackend();
	CPointers::setBackend(m_backend);
	const CSwordBackend::LoadError errorCode = m_backend->initModules();

	m_moduleList = 0;
	if ( errorCode == CSwordBackend::NoError ) {	//no error
		m_moduleList = &(m_backend->moduleList());
	}
	else {
		m_moduleList = 0;
		//show error message that initBackend failed
		switch (errorCode) {
			case CSwordBackend::NoSwordConfig: //mods.d or mods.conf missing
			{
				KStartupLogo::hideSplash();
        CSwordSetupDialog dlg;
        dlg.showPart( CSwordSetupDialog::Sword );
        dlg.exec();
				break;
	    }

			case CSwordBackend::NoModules: //no modules installed, but config exists
			{
				KStartupLogo::hideSplash();
        CSwordSetupDialog dlg;
        dlg.showPart( CSwordSetupDialog::Install );
        dlg.exec();
				break;
		  }

			default: //unknown error
			{
				KStartupLogo::hideSplash();
        CSwordSetupDialog dlg;
        dlg.showPart( CSwordSetupDialog::Sword );
       	dlg.exec();
				break;
		  }
		}
	}
	m_backend->booknameLanguage( CBTConfig::get(CBTConfig::language) );
}

/** Initializes the CPrinter object. */
void BibleTime::initPrinter() {
	KStartupLogo::setStatusMessage(i18n("Initializing print system") + QString::fromLatin1("..."));
	CPointers::setPrinter( (m_printer = new Printing::CPrinter(this)) );
}

/** Apply the settings given by the profile p*/
void BibleTime::applyProfileSettings( CProfile* p ){
	if (!p) {
		return;
  }

	if (m_initialized) //on startup KDE sets the main geometry
    setGeometry( p->geometry() );
	m_windowFullscreen_action->setChecked( p->fullscreen() );  //set the fullscreen button state
	toggleFullscreen();  
}

/** Stores the settings of the mainwindow in the profile p */
void BibleTime::storeProfileSettings( CProfile* p ){
  if (!p || !m_windowFullscreen_action) {
    return;
  }
  p->setFullscreen( m_windowFullscreen_action->isChecked() );
	p->setGeometry(geometry());
}
