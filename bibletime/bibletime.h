/***************************************************************************
                          bibletime.h  -  description
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

#ifndef BIBLETIME_H
#define BIBLETIME_H

//KDE includes
#include <kapp.h>
#include <ktmainwindow.h>
#include <kaction.h>

//QT includes
#include <qsplitter.h>
#include <qguardedptr.h>

//own includes
#include "frontend/groupmanager/cgroupmanager.h"
#include "frontend/cmdiarea.h"
#include "frontend/presenters/cswordpresenter.h"
#include "backend/cmoduleinfo.h"
#include "backend/sword_backend/cswordbackend.h"
#include "printing/cprinter.h"
#include "structdef.h"

//forward class declarations

//KDE classes
class KConfig;
class KToggleAction;
class KAccel;
class KPopupMenu;

//QT classes
class QPopupMenu;

/**
 	*	@page backend The structure of the backend
 	*	First we have a backend structure, which could be extended
 	* by every type of backend in the future. Have a look at the class CBackend.
 	*
 	*	The backend implementation for Sword is called CSwordBackend, the classes we use
 	* to work with keys are called CSwordVerseKey and CSwordLDKey, both are derived from
 	* the class CKey.
 	* The CKey derived classes used for Sword do also inherit the classes VerseKey (by CSwordVerseKey)
 	*	and SWKey (by CSwordLDKey).
 	*	
 	*	The classes used to handle all module based stuff are derived from CModleInfo.
 	*	The module classes are: CSwordModuleInfo, CSwordBibleModuleInfo, CSwordCommentaryModuleInfo and
 	*	CSwordLexiconModuleInfo.
 	*	Have a look at the class documentation of the mentioned classes to learn how the
 	*	structure of them looks like and which class inherits which other class.
 	*
 	*	The first class which should be created is the backend (for Sword the class is called CSwordBackend).
 	*	Then create all the different module classes for the correct Sword modules.
 	*	Later you can work with them for example by using  the CSwordVerseKey class.
 	*/

/**
 	*	@page frontend The structure of the frontend
 	*	
 	*
 	*	The frontend contains the classes used for the GUI, for examples the presenters,
 	*	the groupmanager, the searchdialog, the mainwindow etc.
 	*/

/** @mainpage BibleTime 1.0 - sourcecode documentation
 *
 *	This is the sourcecode documentation of BibleTime, a Bible study tool for KDE/Linux.
 *	BibleTime is devided in two major parts, the backend and the frontend.<BR>
 *	Documentation for the backend: @ref backend<BR>
 *	Documentation for the frontend: @ref frontend.<BR>
 */

/** The main class of BibleTime. Here are the main widgets created.
	*
	* This is the main class of BibleTime! This class creates the GUI, the KAction objects
	* and connects to some slots. Please insert the creation of actions in initActions,
	* the creation of widgets into initView and the connect(...) calls into initConnections.
	* Reading from a config file on creation time should go into readSettings(), saving into
	* saveSettings().
	* This is the general way of all BibleTime classes.
	*/
class BibleTime : public KMainWindow {
	friend class CMDIArea;
	Q_OBJECT
public:
  /**
  * construtor of BibleTime
  */
  BibleTime();
  /**
  * Destructor of BibleTime
  */
  virtual ~BibleTime();
  /**
  * No descriptions
  */
  virtual void show();

public slots:
  /**
  * Shows the daily tip
  */
  void slotHelpTipOfDay();
  /**
  * Opens the optionsdialog of BibleTime.
  */
  void slotSettingsOptions();
  /**
  * The last window was closed!
  */
  void lastWindowClosed();
  /**
  	* Reimplementation from QWidget.
  	* Sets the correct plain caption.
  	*/
  virtual void setPlainCaption( const QString& );
  /**
 	* Reimplementation. Sets the caption and takes care of special captions set by the MDI window
 	*/
  virtual void setCaption(const QString&);

protected: // Protected methods
  /**
  * Initializes the view of this widget
  */
  void initView();
  /**
  * Initializes the menubar of BibleTime.
  */
  void initMenubar();
  /**
  * Initializes the SIGNAL / SLOT connections
  */
  void initConnections();
  /**
  * Initializes the keyboard accelerators
  */
  void initKeyAccels();
  /**
  * Initializes the backend
  */
  void initBackends();
  /**
  * Reads the settings from the configfile and sets the right properties.
  */
  void readSettings();
  /**
  * Saves the settings of this class
  */
  void saveSettings();
  /**
	* Initializes the action objects of the GUI
	*/
  void initActions();
  /**
  * Refreshes all presenter supporting at least in of the features given as parameter.
  */
  void refreshPresenters( int useFeatures );
  /**
  * Called before a window is closed
  */
  bool queryClose();
  /**
  * Called before quit.
  */
  bool queryExit();
  /**
  * Initializes the CPrinter object.
  */
  void initPrinter();

	QGuardedPtr<QPopupMenu> m_windowMenu;
	QGuardedPtr<QPopupMenu> m_editMenu;

  /** FILE menu actions */
	QGuardedPtr<KAction> m_fileClearQueue_action;	
	QGuardedPtr<KAction> m_filePrint_action;	
	QGuardedPtr<KAction> m_fileQuit_action;

  /** Groupmanager menu actions */
  QGuardedPtr<KAction> m_GMReset_action;
	QGuardedPtr<KAction> m_GMSearchModules_action;
	QGuardedPtr<KAction> m_GMDeleteItems_action;		

  /** VIEW menu actions */
  QGuardedPtr<KToggleAction>	m_viewFootnotes_action;
	QGuardedPtr<KToggleAction>	m_viewStrongs_action;		
	QGuardedPtr<KToggleAction>	m_viewToolbar_action;		
	QGuardedPtr<KToggleAction>	m_viewGroupManager_action;			
	
	/** SWORD menu actions*/
	
	/** SETTINGS menu actions */
	QGuardedPtr<KAction>	m_settingsOptions_action;		
	QGuardedPtr<KAction>	m_settingsToolbar_action;		
	
	/** WINDOW menu actions */
	QGuardedPtr<KAction>	m_windowCascade_action;
	QGuardedPtr<KAction>	m_windowTile_action;	
	QGuardedPtr<KToggleAction>	m_windowAutoCascade_action;
	QGuardedPtr<KToggleAction>	m_windowAutoTile_action;
	QGuardedPtr<KAction>	m_windowCloseAll_action;

	QGuardedPtr<KAction>	m_helpContents_action;
	QGuardedPtr<KAction>	m_helpWhatsThis_action;
	QGuardedPtr<KAction>	m_helpBugReport_action;	
	QGuardedPtr<KAction>	m_helpDailyTip_action;	
	QGuardedPtr<KAction>	m_helpAboutBibleTime_action;	
	QGuardedPtr<KAction>	m_helpAboutKDE_action;			
		
	QGuardedPtr<KHelpMenu> m_helpMenu;
		
	QGuardedPtr<KConfig> m_config;
	QGuardedPtr<KAccel> m_keyAccel;
	QGuardedPtr<QSplitter> m_splitter;
	QGuardedPtr<CGroupManager> m_groupmanager;
	QGuardedPtr<CMDIArea> m_mdi;
  /**
  * The list of installed SWORD modules
  */
  ListCSwordModuleInfo* m_moduleList;
  CImportantClasses* m_important;

  bool m_initialized;

protected slots:
  /**
 	* Quit BibleTime
 	*/
  void slotFileQuit();
  /**
 	* Creates a new presenter in the MDI area according to the type of the module.
 	*/
  void createNewSwordPresenter(ListCSwordModuleInfo,const QString);
  /**
  * No descriptions
  */
  void createNewSwordPresenter(CSwordModuleInfo*, const QString );
  /**
 	* Switches displaying of strong number on or off
 	*/
  void slotToggleStrongs();
  /**
 	* Switches footnotes on or off
 	*/
  void slotToggleFootnotes();
  /**
 	* Is called when the window menu is about to show ;-)
 	*/
  void slotWindowMenuAboutToShow();
  /**
 	* This slot is connected with the windowAutoTile_action object
 	*/
  void slotAutoTile();
  /**
 	* This slot is connected with the windowAutoCascade_action object
 	*/
  void slotAutoCascade();
	/**
	* Is called when a client was selected in the window menu
	*/
	void slotWindowMenuActivated( int );
  /**
 	* Shows/hides the toolbar
 	*/
  void slotToggleToolbar();
  /**
  * Opens a toolbar editor
  */
  void slotSettingsToolbar();
  /**
  * Shows or hides the groupmanager.
  */
  void slotToggleGroupManager();
  /**
  * Starts printing and opens the printerdialog at first.
  */
  void slotFilePrint();
  /**
 	* Enables the "Clear printer queue" action
 	*/
  void slotSetPrintingStatus();
};
#endif
