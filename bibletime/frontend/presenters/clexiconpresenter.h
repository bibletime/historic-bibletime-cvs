/***************************************************************************
                          clexiconpresenter.h  -  description
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

#ifndef CLEXICONPRESENTER_H
#define CLEXICONPRESENTER_H

#include <qwidget.h>
#include "cswordpresenter.h"

class CSwordLDKey;

/**
 	* Presenter used for lexicons
  * @author The BibleTime team
  */
class CLexiconPresenter : public CSwordPresenter  {
   Q_OBJECT
public: 
	/**
	* constructor
	*/
	CLexiconPresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent=0, const char *name=0);
	/**
	* destructor
	*/
	virtual ~CLexiconPresenter();
  /**
  * Inserts the used keyboard actions into the given KAccel object.
  */
  static void insertKeyboardActions(KAccel* a);
	
public slots: // Public slots
  /**
  * No descriptions
  */
  virtual void lookup(const QString&);
  /**
  * Refreshes all parts decsribed by the parameter.
  */
  virtual void refresh( const int events);

protected: // Protected methods
  /**
  * Initializes keyboard accelerators.
  */
  virtual void initAccels();

private slots:
  void copyEntry();
  void copyEntryText();
  void copyEntryAndText();
  void printEntryAndText();
  void modulesChanged();
	void optionsChanged();
  void popupAboutToShow();
  void lookup(CSwordKey*);
  void nextEntry();
  void previousEntry();

private: // Private methods
  void initConnections();
  void initView();

  CSwordLDKey* m_key;
};

#endif
