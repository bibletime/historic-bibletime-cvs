/***************************************************************************
                          cbookreadwindow.h  -  description
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

#ifndef CBOOKREADWINDOW_H
#define CBOOKREADWINDOW_H

//BibleTime includes
#include "clexiconreadwindow.h"
#include "backend/cswordkey.h"
#include "backend/cswordtreekey.h"
#include "backend/cswordversekey.h"

class KToggleAction;
class CBookTreeChooser;

/**
  * @author The BibleTime team
  */
class CBookReadWindow : public CLexiconReadWindow  {
  Q_OBJECT
public:
	static void insertKeyboardActions( KAccel* a );

	CBookReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name=0);
	virtual ~CBookReadWindow();
  /**
  * Store the settings of this window in the given CProfileWindow object.
  */
  virtual void storeProfileSettings( CProfileWindow* profileWindow );
  /**
  * Store the settings of this window in the given profile window.
  */
  virtual void applyProfileSettings( CProfileWindow* profileWindow );

protected:
	virtual void initKeyboardActions();
  virtual void initConnections();
  virtual void initView();
//  virtual void setupPopupMenu();

private:
  KToggleAction* m_treeAction;
  CBookTreeChooser* m_treeChooser;

private slots: // Private slots
  /**
  * Is called when the action was executed to toggle the tree view.
  */
  void treeToggled();

protected slots: // Protected slots
  /**
  * Reimplementation to take care of the tree chooser.
  */
  virtual void modulesChanged();
};

#endif
