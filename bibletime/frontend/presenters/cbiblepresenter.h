/***************************************************************************
                          cbiblepresenter.h  -  description
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

#ifndef CBIBLEPRESENTER_H
#define CBIBLEPRESENTER_H

#include <qwidget.h>
#include "cswordpresenter.h"

class CSwordVerseKey;

/**
	* The presenter used for Sword Bibles.
  * @author The BibleTime team
  */
class CBiblePresenter : public CSwordPresenter  {
   Q_OBJECT
public: 	
	CBiblePresenter(ListCSwordModuleInfo useModules, QWidget *parent=0, const char *name=0);
	~CBiblePresenter();
  /**
  * Reimplementation from CSwordPresenter.
  */
  virtual void lookup(const QString&, const QString&);
  /**
  * Reimplementation.
	*/
  virtual void refresh( );
  /**
  * Inserts the actions used by this window class into the given KAccel object.
  */
  static void insertKeyboardActions(KAccel* a);
  /** Applies some Bible window specific setttings. */
  virtual void applySettings( CProfileWindow* window );
  /** Stores some Bible window dpecific setttings in the profile. */
  virtual void storeSettings( CProfileWindow* setttings );

protected:
  /**
  * Initializes the view (central widget, toolbars etc) of this presenter
  */
  virtual void initView();
  /**
  * Initializes the Signal / Slot connections
	*/
  void initConnections();
  /**
  * Initializes the accelerator object.
  */
	virtual void initAccels();

protected slots:
  void modulesChanged();
	void optionsChanged();
  void lookup( CSwordKey* );
	void nextBook();
	void previousBook();
	void nextChapter();		
	void previousChapter();
	void nextVerse();
	void previousVerse();
		
private slots:
  void popupAboutToShow();
  void copyVerse();
  void copyVerseText();
  void copyVerseAndText();
  void printVerseAndText();
  void printChapter();
  void saveVerseAndText();

private:
	CSwordVerseKey* m_key;
};

#endif
