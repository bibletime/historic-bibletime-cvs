/***************************************************************************
                          cplainwritewindow.h  -  description
                             -------------------
    begin                : Fre Okt 11 2002
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

#ifndef CPLAINWRITEWINDOW_H
#define CPLAINWRITEWINDOW_H

//BibleTime includes
#include "cwritewindow.h"

//Qt includes
#include <qwidget.h>

/** The write window class which offers a plain editor for source code editing.
  * @author The BibleTime team
  */
class CPlainWriteWindow : public CWriteWindow  {
   Q_OBJECT
public: 
	CPlainWriteWindow( ListCSwordModuleInfo modules, CMDIArea* parent, const char *name = 0 );
	~CPlainWriteWindow();

protected: // Protected methods
  /**
  * Initialize the state of this widget.
  */
  virtual void initView();
  virtual void initConnections();
};

#endif
