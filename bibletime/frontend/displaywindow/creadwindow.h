/***************************************************************************
                          creadwindow.h  -  description
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

#ifndef CREADWINDOW_H
#define CREADWINDOW_H

//BibleTime includes
#include "cdisplaywindow.h"

#include "frontend/display/cdisplay.h"
#include "frontend/display/creaddisplay.h"

//Qt includes
#include <qwidget.h>
#include <qpopupmenu.h>

class KPopupMenu;

/** The base class for all read-only display windows.
  * @author The BibleTime team
  */

class CReadWindow : public CDisplayWindow  {
   Q_OBJECT
public: 
	static void insertKeyboardActions( KAccel* const a );
 	
  CReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name=0);
	virtual ~CReadWindow();
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

protected: // Protected methods
  /**
  * Sets the display widget of this display window.
  */
  virtual void setDisplayWidget( CReadDisplay* newDisplay );
  /**
  * Returns the display widget of this window.
  */
  virtual CReadDisplay* const displayWidget();
  /**
  * Setup the entries of the popup menu.
  */
	virtual void initKeyboardActions();

protected slots:
  virtual void lookup( CSwordKey* );
  /**
  * Update the status of the popup menu entries.
  */
  virtual void copyDisplayedText();
  
private:
	CReadDisplay* m_displayWidget;
};

#endif
