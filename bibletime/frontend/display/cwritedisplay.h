/***************************************************************************
                          cwritedisplay.h  -  description
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

#ifndef CWRITEDISPLAY_H
#define CWRITEDISPLAY_H

#include "cdisplay.h"

class KActionCollection;
class KToolBar;

/**The base class for all read/write-display classes.
  *@author The BibleTime team
  */

class CWriteDisplay : public CDisplay  {
protected:
	friend class CDisplay;
 	friend class CPlainWriteDisplay;  
	CWriteDisplay( CWriteWindow* writeWindow );
	~CWriteDisplay();

public: // Public methods
  /**
  * Sets the current modified status of the widget.
  */
  virtual void setModified( const bool modified ) = 0;
  /**
  * Returns true if the current text was modified.
  */
  virtual const bool isModified() const = 0;
  /**
  * Returns the text of this edit widget.
  */
  virtual const QString plainText() = 0;
  /**
  * Creates the necessary action objects and puts them on the toolbar.
  */
  virtual void setupToolbar( KToolBar* bar, KActionCollection* actionCollection ) = 0;
};

#endif
