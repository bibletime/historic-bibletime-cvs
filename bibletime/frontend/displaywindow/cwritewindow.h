/***************************************************************************
                          cwritewindow.h  -  description
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

#ifndef CWRITEWINDOW_H
#define CWRITEWINDOW_H

//BibleTime includes
#include "cdisplaywindow.h"

//Qt includes
#include <qwidget.h>

/**The base class for all write-only display windows.
  *@author The BibleTime team
  */

class CWriteWindow : public CDisplayWindow  {
	Q_OBJECT
public: 
	CWriteWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name=0);
	~CWriteWindow();
};

#endif
