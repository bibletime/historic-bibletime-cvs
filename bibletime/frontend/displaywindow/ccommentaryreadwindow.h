/***************************************************************************
                          ccommentaryreadwindow.h  -  description
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

#ifndef CCOMMENTARYREADWINDOW_H
#define CCOMMENTARYREADWINDOW_H

//BibleTime includes
#include "cbiblereadwindow.h"

//Qt includes
#include <qwidget.h>

/**
  *@author The BibleTime team
  */

class CCommentaryReadWindow : public CBibleReadWindow  {
   Q_OBJECT
public: 
	CCommentaryReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name=0);
	~CCommentaryReadWindow();
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
};

#endif
