/***************************************************************************
                          ccommentarypresenter.h  -  description
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

#ifndef CCOMMENTARYPRESENTER_H
#define CCOMMENTARYPRESENTER_H

#include <qwidget.h>
#include "cswordpresenter.h"

class CSwordVerseKey;

class KToggleAction;
class KToolBar;

/** The presenter used for Sword commentaries.
  * @author The BibleTime team
  */
class CCommentaryPresenter : public CSwordPresenter  {
   Q_OBJECT

public:
	/**
	* Default constructor
	*/
	CCommentaryPresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent=0, const char *name=0);
	/**
	* Default destructor
	*/	
	~CCommentaryPresenter();
  /**
  * Synchronizes to the given key if sync is enabled.
  */
  void synchronize( CSwordKey* syncKey );
  /**
  * Inserts the actions used by this display window type into the given KAccel object.
  */
  static void insertKeyboardActions(KAccel* a);
	
public slots: // Public slots
  /**
  * Saves the given text in the module.
  */
  void saveText(const QString);
  /**
  * Reimplementation.
  */
  virtual void lookup(const QString&);
  /**
  * No descriptions
  */
  virtual void refresh( const int events);

protected: // Protected methods
  /**
  * Init keyboard accelartors.
  */
  virtual void initAccels();

protected slots: // Protected slots
  /**
  * renders text and set it to the HTML widget
  */
  void lookup(CSwordKey*);
  /**
  * Is called when the selected modules changed.
  */
  void modulesChanged();
	void optionsChanged();
  /**
  * Deletes the displayed and edited text.
  */
  void deleteText();
	void editComment();
	/**
	* Print the current item displayed in the presenter.
	*/
	void printHighlightedVerse();
	void nextBook();
	void previousBook();
	void nextChapter();
	void previousChapter();
	void nextVerse();
	void previousVerse();	
  /**
  * Toggles the synchronize button.
  */
  void toggleSynchronize();

private:
  void initConnections();
  void initView();
  /**
  * Checks for changes and saves the text.
  */
  void checkChanges();
	KToggleAction *presenterEdit_action;
	KToggleAction *presenterSync_action;
  CSwordVerseKey* m_key;
	KToolBar* m_editToolBar;
		
private slots:
  void popupAboutToShow();
  void syncToggled();
  void insertReference(const QString&);
  void copyEntry();
  void copyEntryText();
  void copyEntryAndText();
  void printEntryAndText();
  void beforeKeyChange(const QString&);
};

#endif
