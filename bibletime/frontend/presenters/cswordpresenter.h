/***************************************************************************
                          cswordpresenter.h  -  description
                             -------------------
    begin                : Sun Mar 18 2001
    copyright            : (C) 2001 by The BibleTime team
    email                : info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CSWORDPRESENTER_H
#define CSWORDPRESENTER_H

//BibleTime includes
#include "../../backend/cswordmoduleinfo.h"
#include "../../backend/cswordbackend.h"
#include "../../structdef.h"
#include "../cprofilewindow.h"

//Qt includes
#include <qwidget.h>

//KDE includes
#include <kmainwindow.h>

class KPopupMenu;
class KToolBar;

class CKeyChooser;
class CHTMLWidget;

class CModuleChooserBar;
class CDisplaySettingsButton;

class CSwordKey;
class KAccel;

/** Base class for all Sword based presneters
	* The base class for all Sword based presenters.
	*
  * @author The BibleTime team
  */
class CSwordPresenter : public KMainWindow  {
   Q_OBJECT
public:
	/**
	* Default constructor
	*/
	CSwordPresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses, QWidget *parent=0, const char *name=0);
  /**
  * Returns the features used by this presenter.
  */
  int getFeatures();
  /**
  * Refreshes the presenter.
  */
  virtual void refresh();
  /**
  * Returns the listof used modules.
  */
  ListCSwordModuleInfo& getModuleList();
  /**
  * Returns the caption for this Sword display window.
  */
  const QString windowCaption();
  /**
  * Stores the settings of this window in the CProfileWindow object given as parameter.
  */
  virtual void storeSettings( CProfileWindow* p );
  /**
  * Applies the settings given in the parameter to the window.
  */
  virtual void applySettings( CProfileWindow* settings );

public slots: // Public slots
  /**
  * Look up the key given as parameter.
  */
  virtual void lookup(const QString&);
  /**
  * Returns the used keychooser object of this presenter.
  */
  CKeyChooser* getKeyChooser() const;
  /**
  * Sets the caption of this display window
  */
  virtual void setCaption(const QString&);
  /**
  * Inserts the action used by this display window in the given KAccel object.
  */
  static void insertKeyboardActions( KAccel* accel );

protected: // Protected methods
  /**
  * Prints the key given as parameter.
  */
  void printKey(CSwordKey*, CSwordKey*, CSwordModuleInfo*);
  /**
  * Refreshes the supported features.
  */
//  void refreshFeatures();
  /**
  * Is called when the presenter should be closed. To delete the presenter it emits "close(CPresenter*)".
  */
  virtual void closeEvent(QCloseEvent*e);
  /** Initializes the accelerators used by this windows.
	* Derived classes have to call this function too if the reimplemented this function.
	* e.g.
	*  @code
	*			//init own additional accelerators
	* 	 CSwordPresenter::initAccel();
  */
  virtual void initAccels();

	KToolBar* m_mainToolBar;
	
	KPopupMenu* m_popup;
	KPopupMenu* m_savePopup;
	KPopupMenu* m_copyPopup;	
	KPopupMenu* m_printPopup;	
		
	QPopupMenu* m_lexiconPopup;
	CKeyChooser* m_keyChooser;
	CHTMLWidget* m_htmlWidget;	

  CModuleChooserBar* m_moduleChooserBar;
	CDisplaySettingsButton* m_displaySettingsButton;

  ListCSwordModuleInfo m_moduleList;
 	CImportantClasses* m_important;

 	QString m_highlightedVerse;

 	KAccel* m_accel;

	CSwordBackend::moduleOptionsBool m_moduleOptions;
	CSwordBackend::displayOptionsBool m_displayOptions;
 	 	  	
protected slots: // Protected slots
  void lookupWord(int ID);
  /**
  * Initilizes widget before shown and after constructor.
  */
  virtual void polish();
  /**
  * Is called when this display window looses the focus.
  */
  virtual void focusInEvent( QFocusEvent* e );
  /**
  * Is called when this display window looses the focus.
  */
  virtual void focusOutEvent( QFocusEvent* e );


private:
	int m_features;

signals: // Signals
  void lookupInLexicon(const QString& text, const QString& lexicon);
  /**
  * Is emitted when this presenter should be closed.
  */
  void closePresenter(CSwordPresenter*);

};

#endif
