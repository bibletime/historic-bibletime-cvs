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
#include <kmainwindow.h>

//Frontend includes
#include "frontend/cprofilemgr.h"
#include "frontend/displaywindow/cdisplaywindow.h"

//Backend includes
#include "backend/cswordmoduleinfo.h"

//forward class declarations

//BT classes
class CMDIArea;
class CProfile;
class CDisplayWindow;
class CMainIndex;

//KDE classes
class KToggleAction;
class KAccel;
class KPopupMenu;
class KAction;
class KActionMenu;

//QT classes
class QPopupMenu;
class QProgressDialog;
class QSplitter;

/**
 	*	@page backend The structure of the backend
 	*
 	*	The backend implementation for Sword is called CSwordBackend, the classes we use
 	* to work with keys are called CSwordVerseKey and CSwordLDKey, both are derived from
 	* the class CSwordKey.
 	* The CSwordKey derived classes used for Sword do also inherit the classes VerseKey (CSwordVerseKey)
 	*	and SWKey (CSwordLDKey).
 	*	
 	*	The classes used to handle all module based stuff are derived from CModuleInfo.
 	*	The module classes are: CSwordModuleInfo (for Sword modules), CSwordBibleModuleInfo (for bibles), CSwordCommentaryModuleInfo (for commentaries) and
 	*	CSwordLexiconModuleInfo (for lexicons).
 	*	Have a look at the class documentation of the mentioned classes to learn how the
 	*	structure of them looks like and which class inherits which other class.
 	*
 	*	The first objects which should be created in the application is the backend (for Sword the class is called CSwordBackend).
 	*	Then create all the different module classes for the correct Sword modules. Have a look at
 	* BibleTime::initBackens() to see how it's done in BibleTime.@br
 	*	Later you can work with them for example by using the CSwordKey and CSwordModuleInfo derived class.
 	*/

/**
 	*	@page frontend The structure of the frontend
 	*	
 	*
 	*	The frontend contains the classes which interact with the user. For example the main index,
 	* the display windows, the searchdialog or the other parts.
 	* The main index is implemented in the class CGroupManager, the items of the main index are implemented in the class CGroupManagerItem.
 	* Each CGroupManagerItem has a type() function which returns the type of the object (Module, Bookmark or Group).<BR>
 	* The display windows are all derived from the base class CPresenter. The display windows which handle Sword modules are all derived from the CSwordPresenter class.
 	* The display windows which provide functionality are CBiblePresenter for Bibles, CCommentaryPresenter for commentaries and CLexiconPresenter for lexicon and dictionaries.
 	* CSwordPresenter provides the essential base functions which are reimplemented in the derived classes (for example CSwordPresenter::lookup).<BR>
 	* Another important part of the frontend are the keychoosers. They provide an interface to choose a key of a module. The interface for different module types is different.
 	* The base class is CKeyChooser which is the factory for the derived classes. Use the function CKeyChooser::createInstance to get the correct
 	* keychooser implementation for the desired module.<BR>
 	*/

/** @mainpage BibleTime - sourcecode documentation
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
	virtual void show();
	/**
	* Reads the settings from the configfile and sets the right properties.
	*/
	void readSettings();
	/**
	* Saves the settings of this class
	*/
	void saveSettings();
	/**
	* Restores the workspace if the flaf for this is set in the config.
	*/
	void restoreWorkspace();
	/**
	* Apply the settings given by the profile p
	*/
	void applyProfileSettings( CProfile* p );
	/**
	* Stores the settings of the mainwindow in the profile p
	*/
	void storeProfileSettings( CProfile* p );

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
	* Opens the handbook.
	*/
	void openOnlineHelp_Handbook();
	/**
	* Opens the bible study howto.
	*/
	void openOnlineHelp_Howto();
	/**
	* Sets the plain caption of the main window
	*/
	virtual void setPlainCaption( const QString& );
	/**
	* Sets the caption of the mainwindow
	*/
	virtual void setCaption(const QString&);
  /**
  * Processes the commandline options given to BibleTime.
  */
  void processCommandline();

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
	* Initializes the backend
	*/
	void initBackends();
	/**
	* Initializes the action objects of the GUI
	*/
	void initActions();
	/**
	* Refreshes all presenter supporting at least in of the features given as parameter.
	*/
	void refreshDisplayWindows();
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

protected slots:
	/**
 	* Quit BibleTime
 	*/
	void slotFileQuit();
	/**
 	* Creates a new presenter in the MDI area according to the type of the module.
 	*/
	CDisplayWindow* createReadDisplayWindow(ListCSwordModuleInfo, const QString&);
	CDisplayWindow* createReadDisplayWindow(CSwordModuleInfo*, const QString&);
	CDisplayWindow* createWriteDisplayWindow(CSwordModuleInfo*, const QString&, const CDisplayWindow::WriteWindowType&);  
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
	/**
	* Saves to the profile with the menu id ID
	*/
	void saveProfile(int ID);
	/**
	* Saves the current settings into the currently activatred profile.
	*/
	void saveProfile(CProfile* p);
	/**
	* Saves the current settings into the currently activatred profile.
	*/
	void editProfiles();
	/**
	* Loads the profile with the menu id ID
	*/
	void loadProfile(int ID);
	/**
	* Loads the profile with the menu ID id
	*/
	void loadProfile(CProfile* p);
	/**
	* Toggles between normal and fullscreen mode.
	*/
	void toggleFullscreen();
	/**
	* Is called when settings in the optionsdialog have been
	* changed (ok or apply)
	*/
	void slotSettingsChanged();
	/**
	 * Called when search button is pressed
	 **/
	void slotSearchModules();
	/**
	 * Called for search default bible
	 **/
	void slotSearchDefaultBible();
	/**
	 * Called when back button pressed
	 **/
//	void slotBack();
	/**
	 * Called when forward button pressed
	 **/
//	void slotForward();
  /**
   Saves current settings into a new profile.
  */
  void saveToNewProfile();
  /**
  * Slot to refresh the save profile and load profile menus.
  */
  void refreshProfileMenus();
	

private slots: // Private slots
	/**
	* No descriptions
	*/
	void slotPrintedPercent(const int index);
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
  QPopupMenu* m_windowMenu;
	QPopupMenu* m_editMenu;

	/** FILE menu actions */
	KAction* m_fileClearQueue_action;
	KAction* m_filePrint_action;

	/** VIEW menu actions */
	KToggleAction*	m_viewToolbar_action;
	KToggleAction*	m_viewMainIndex_action;

	/** WINDOW menu actions */
	KAction*	m_windowCascade_action;
	KAction*	m_windowTile_action;
	KToggleAction*	m_windowAutoCascade_action;
	KToggleAction*	m_windowAutoTile_action;
	KAction*	m_windowCloseAll_action;

	KActionMenu*	m_windowSaveProfile_action;
 	KAction*	m_windowSaveToNewProfile_action;
	KActionMenu*	m_windowLoadProfile_action;
	KAction*	m_windowEditProfiles_action;
	KToggleAction*	m_windowFullscreen_action;

	KHelpMenu* m_helpMenu;


	bool m_initialized;
	/**
	* The list of installed SWORD modules
	*/
  ListCSwordModuleInfo* m_moduleList;
	QProgressDialog* m_progress;

  CProfile* m_currentProfile;
	KAccel* m_keyAccel;
	QSplitter* m_splitter;
	CMDIArea* m_mdi;

	CProfileMgr m_profileMgr;
	CSwordBackend* m_backend;
	CPrinter* m_printer;

	CMainIndex* m_mainIndex;


};

#endif
