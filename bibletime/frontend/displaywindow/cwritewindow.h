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
#include "frontend/display/cwritedisplay.h"
#include "cdisplaywindow.h"

//Qt includes
#include <qwidget.h>

/**The base class for all write-only display windows.
  *@author The BibleTime team
  */

class CWriteWindow : public CDisplayWindow  {
	Q_OBJECT
public:
	static void insertKeyboardActions( KAccel* const a );

	CWriteWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name=0);
	~CWriteWindow();
  /**
  * Reimplementation to use the popup menu.
  */
  virtual const bool init( const QString& key );
  /**
  * Store the settings of this window in the given CProfileWindow object.
  */
  virtual void storeProfileSettings(CProfileWindow * const settings);
  /**
  * Store the settings of this window in the given CProfileWindow object.
  */
  virtual void applyProfileSettings(CProfileWindow * const settings);
  /**
  * Returns the write display widget used by this window.
  */
  CWriteDisplay* const displayWidget();
  virtual void initConnections();
  virtual void initKeyboardActions();

public slots:
  /**
  * Look up the given key and display the text. In our case we offer to edit the text.
  */
  virtual void lookup( CSwordKey* key );

        
protected: // Protected methods
  /**
  * Saves the given text as text of the given key. Use this function as backend in each write window implementation.
  */
  void setDisplayWidget( CWriteDisplay* display );

private:
  CWriteDisplay* m_writeDisplay;  
};

#endif
