/***************************************************************************
                          creaddisplay.h  -  description
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

#ifndef CREADDISPLAY_H
#define CREADDISPLAY_H

#include "cdisplay.h"

class QPopupMenu;
class QWidget;

/**The base class for all read-only widgets like KHTMLView.
  *@author The BibleTime team
  */

class CReadDisplay :  public CDisplay {
public:
  /**
  * Installs the popup which should be opened when the right mouse button was pressed.
  */
  void installPopup( QPopupMenu* popup );
  /**
  * Returns the popup menu which was set by installPopupMenu()
  */
  QPopupMenu* const installedPopup();
  /**
  * Returns true if the display has an active anchor.
  */
  const bool hasActiveAnchor();
  /**
  * Returns the current active anchor.
  */
  const QString& activeAnchor();
  /**
  * Moves the widget to the given anchor.
  */
  virtual void moveToAnchor( const QString& ) = 0;
  virtual void print(const CDisplay::TextPart);

protected: // Protected methods
	friend class CDisplay;
 	friend class CHTMLReadDisplay;
 	friend class CHTMLReadDisplayView;

	CReadDisplay( CReadWindow* readWindow );
	~CReadDisplay();

  /**
  * Sets the current anchor to the parameter.
  */
  void setActiveAnchor( const QString& );

private: // Public attributes
  /**
  * The member which hols the current anchor.
  */
  QPopupMenu* m_popup;
  QString m_activeAnchor;
};

#endif
