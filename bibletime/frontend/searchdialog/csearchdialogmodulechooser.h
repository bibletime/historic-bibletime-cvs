/***************************************************************************
                          csearchdialogmodulechooser.h  -  description
                             -------------------
    begin                : Fri Mar 30 2001
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

#ifndef CSEARCHDIALOGMODULECHOOSER_H
#define CSEARCHDIALOGMODULECHOOSER_H

#include "../../backend/cswordmoduleinfo.h"
#include "../cpointers.h"

//Qt includes
#include <qwidget.h>
#include <qptrdict.h>

class CGroupManager;
class QListBox;

/** A widget to select the modules in which you want to search.
  * @author The BibleTime team
  */
class CSearchDialogModuleChooser : public QWidget, public CPointers  {
   Q_OBJECT
public: 
	CSearchDialogModuleChooser(QWidget *parent, const char *name=0);
	~CSearchDialogModuleChooser();
  /**
  * Sets the chosen modules for this object.
  */
  void setChosenModules( ListCSwordModuleInfo& modules );
  /**
  * Returns the modules chosen in this widget.
  */
  ListCSwordModuleInfo getChosenModules();

private:
	CGroupManager* m_moduleIndex;
	QListBox* m_moduleList;
	QPtrDict<char> m_itemsDict;
	bool m_initialized;
	
private slots: // Private slots
  /**
  * Removes the selected item from the module list
  */
  void removeCurrentItem();
  /**
  * Adds the selected item to the list
  */
  void addCurrentItem();

signals: // Signals
  /**
  * No descriptions
  */
  void chosenModulesChanged();
};

#endif
