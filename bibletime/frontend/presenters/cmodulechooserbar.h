/***************************************************************************
                          cmodulechooserbar.h  -  description
                             -------------------
    begin                : Sat Mar 17 2001
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

#ifndef CMODULECHOOSERBAR_H
#define CMODULECHOOSERBAR_H

#include "../../backend/sword_backend/cswordmoduleinfo.h"
#include "../../structdef.h"

#include <qwidget.h>
#include <ktoolbar.h>

class CModuleChooserButton;

/**
  *@author The BibleTime team
  */

class CModuleChooserBar : public KToolBar  {
   Q_OBJECT
public: 
	CModuleChooserBar(CImportantClasses* important, ListCSwordModuleInfo useModules,CSwordModuleInfo::type type,  QWidget *parent=0, const char *name=0);
	~CModuleChooserBar();
  /**
  * Returns a list of selected modules.
  */
  ListCSwordModuleInfo getModuleList();
  /**
  * Sets the number of the maximum count of buttons.
  */
  void setButtonLimit( const int limit);
	
private:
	CImportantClasses* m_important;
	QList<CModuleChooserButton> m_buttonList;
	CSwordModuleInfo::type m_moduleType;
	int m_idCounter;
	int m_deleteID;
	int m_buttonLimit;

protected slots: // Protected slots
  /**
  * Adds a button to the toolbar
  */
  void addButton();
	/**
	* Removes a button from the toolbar
	*/
	void removeButton( const int ID );
  /**
  * No descriptions
  */
  void deleteButton();

signals: // Signals
  /**
  * This signal is send if the modules changed.
  */
  void sigChanged();
};

#endif
