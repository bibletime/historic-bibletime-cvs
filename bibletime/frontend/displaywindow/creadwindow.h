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

//Qt includes
#include <qwidget.h>
#include <qpopupmenu.h>

class CReadDisplay;

class KPopupMenu;

/** The base class for all read-only display windows.
  * @author The BibleTime team
  */

class CReadWindow : public CDisplayWindow  {
   Q_OBJECT
public: 
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
  void setDisplayWidget( CReadDisplay* newDisplay );
  /**
  * Returns the display widget of this window.
  */
  CReadDisplay* const displayWidget();
  /**
  * Returns the installed popup menu.
  */
  KPopupMenu* const popup();
  /**
  * Setup the entries of the popup menu.
  */
  virtual void setupPopupMenu() = 0;

protected slots:
  virtual void lookup( CSwordKey* );
  /**
  * Update the status of the popup menu entries.
  */
  virtual void updatePopupMenu();

private:
	CReadDisplay* m_displayWidget;
 	KPopupMenu* m_popupMenu;
};

#endif
