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
	CBiblePresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent=0, const char *name=0);
	~CBiblePresenter();
  /**
  * Reimplementation from CSwordPresenter.
  */
  virtual void lookup(const QString&);
  /**
  * Reimplementation.
	* Refreshes the things which are described by the event integer.
	*/
  virtual void refresh( const int events );

protected:
  /**
  * Initializes the view (central widget, toolbars etc) of this presenter
  */
  virtual void initView();
  /**
  * Initializes the Signal / Slot connections
	*/
  void initConnections();

protected slots: // Protected slots
  /**
  * This slot is called when the modules selected in
	* the module chooserbar have changed.
	*/
  void modulesChanged();
  /**
  * Displays the chapter using the aparameter.
  */
  void lookup( CKey* );

private slots: // Private slots
  /**
  ** No descriptions
  */
  void popupAboutToShow();

//copy slots
  /**
  * Copies the highlighted text into clipboard.
  */
  void copyVerse();
	/**
  * Copies the highlighted text into clipboard.
  */
  void copyVerseText();
	/**
  * Copies the highlighted text into clipboard.
  */
  void copyVerseAndText();

//print slots
  void printVerseAndText();
  /**
  * Copies the highlighted text into clipboard.
  */
  void printChapter();

//save slots
	/**
  * Copies the highlighted text into clipboard.
  */
  void saveVerseAndText();


private:
	CSwordVerseKey* m_key;
};

#endif
