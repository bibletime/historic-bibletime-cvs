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

#include <qwidget.h>
#include "../../backend/sword_backend/cswordmoduleinfo.h"

class CImportantClasses;

/** A widget to select the modules in which you want to search.
  * @author The BibleTime team
  */
class CSearchDialogModuleChooser : public QWidget  {
   Q_OBJECT
public: 
	CSearchDialogModuleChooser(CImportantClasses* importantClasses, QWidget *parent, const char *name=0);
	~CSearchDialogModuleChooser();
  /**
  * Sets the chosen modules for this object.
  */
  void setModuleList( ListCSwordModuleInfo* modules);
  /**
  * Returns the modules chosen in this widget.
  */
  ListCSwordModuleInfo* getModuleList();

private:
	CImportantClasses* m_importantClasses;
	ListCSwordModuleInfo* m_moduleList;
};

#endif
