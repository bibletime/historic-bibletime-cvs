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

//own includes
#include "structdef.h"

#include <pthread.h>

//forward class declarations
class CSwordModuleInfo;
typedef QList<CSwordModuleInfo> ListCSwordModuleInfo;

class CGroupManager;
class CMDIArea;
class CModuleInfo;

//KDE classes
class KConfig;
class KToggleAction;
class KAccel;
class KPopupMenu;

//QT classes
class QPopupMenu;
class QProgressDialog;
class QSplitter;

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
 	*	The frontend contains the classes which interact with the user. For example the main index,
 	* the display widnows, the searchdialog or the other parts.
 	* The main index is implemented in the class CGroupManager, the items of the main index are implemented in the class CGroupManagerItem.
 	* Each CGroupManagerItem has a type() function which returns the type of the object (Module, Bookmark or Group).<BR>
 	* The display windows are all derived from the base class CPresenter. The display windows which handle Sword modules are all derived from the CSwordPresenter class.
 	* The display windows which provide functionality are CBiblePresenter for Bibles, CCommentaryPresenter for commentaries and CLexiconPresenter for lexicon and dictionaries.
 	* CSwordPresenter provides the essential base functions which are reimplemented in the derived classes (for example CSwordPresenter::lookup).<BR>
 	* Another important part of the frontend are the keychoosers. They provide an interface to choose a key of a module. The interface for different module types is different.
 	* The base class is CKeyChooser which is the factory for the derived classes. Use the function CKeyChooser::createInstance to get the correct
 	* keychooser implementation for the desired module.<BR>
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
  /**
  * Reimplementation used for session management.
  */
  void readProperties(KConfig* config);
  /**
  * Reimplementation used for sessions management.
  */
  void saveProperties(KConfig* myConfig);

	QPopupMenu* m_windowMenu;
	QPopupMenu* m_editMenu;

  /** FILE menu actions */
	KAction* m_fileClearQueue_action;	
	KAction* m_filePrint_action;	
	KAction* m_fileQuit_action;

  /** Groupmanager menu actions */
  KAction* m_GMReset_action;
	KAction* m_GMSearchModules_action;
	KAction* m_GMDeleteItems_action;		

  /** VIEW menu actions */
  KToggleAction*	m_viewFootnotes_action;
	KToggleAction*	m_viewStrongs_action;		
	KToggleAction*	m_viewToolbar_action;		
	KToggleAction*	m_viewGroupManager_action;			
	
	/** SETTINGS menu actions */
	KAction*	m_settingsOptions_action;		
	KAction*	m_settingsToolbar_action;		
	
	/** WINDOW menu actions */
	KAction*	m_windowCascade_action;
	KAction*	m_windowTile_action;	
	KToggleAction*	m_windowAutoCascade_action;
	KToggleAction*	m_windowAutoTile_action;
	KAction*	m_windowCloseAll_action;

	/** HELP menu actions */	
	KAction*	m_helpContents_action;
	KAction*	m_helpWhatsThis_action;
	KAction*	m_helpBugReport_action;	
	KAction*	m_helpDailyTip_action;	
	KAction*	m_helpAboutBibleTime_action;	
	KAction*	m_helpAboutKDE_action;			
		
	KHelpMenu* m_helpMenu;
		
	KConfig* m_config;
	KAccel* m_keyAccel;
	QSplitter* m_splitter;
	CGroupManager* m_groupmanager;
	CMDIArea* m_mdi;
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

private slots: // Private slots
  /**
  * No descriptions
  */
  void slotPrintedEntry( const QString& key, const int index);
  /**
  * Printing was finished
  */
  void slotPrintingFinished();
  /**
  * Aborts the printing
  */
  void slotAbortPrinting();
  /**
  * Printing was started
  */
  void slotPrintingStarted();

private:
	QProgressDialog* m_progress;
	pthread_mutex_t progress_mutex;	
};
#endif
