/***************************************************************************
                          clexiconreadwindow.h  -  description
                             -------------------
    begin                : Don Mai 9 2002
    copyright            : (C) 2002 by The BibleTime team
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

#ifndef CLEXICONREADWINDOW_H
#define CLEXICONREADWINDOW_H

//BibleTime includes
#include "creadwindow.h"

//Qt includes
#include <qwidget.h>

//KDE includes
#include <kaction.h>

class CSwordKey;
class CSwordLDKey;
/**
  *@author The BibleTime team
  */

class CLexiconReadWindow : public CReadWindow  {
   Q_OBJECT
public: 
	CLexiconReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name=0);
	virtual ~CLexiconReadWindow();
  /**
  * Store the settings of this window in the given CProfileWindow object.
  */
  virtual void storeProfileSettings( CProfileWindow* profileWindow );
  /**
  * Store the settings of this window in the given profile window.
  */
  virtual void applyProfileSettings( CProfileWindow* profileWindow );
  /** Reimplementation. */
  static void insertKeyboardActions( KAccel* a );

protected:
	virtual void initKeyboardActions();
  virtual void initConnections();
  virtual void initView();
  virtual void updatePopupMenu();
  virtual void setupPopupMenu();

private:
  struct {
    KAction* selectAll;
    KActionMenu* copyMenu;
  	struct {
			KAction* entryOnly;
   		KAction* textOnly;
     	KAction* entryAndText;
      KAction* selectedText;
   	} copy;

    KActionMenu* saveMenu;
    struct {
   		KAction* entryAsPlain;
     	KAction* entryAsHTML;
    } save;

    KActionMenu* printMenu;
    struct {
			KAction* entryAndText;
		} print;
  } m_actions;
};

#endif
