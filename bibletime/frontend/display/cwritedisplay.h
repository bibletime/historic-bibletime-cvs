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

/**The base class for all read/write-display classes.
  *@author The BibleTime team
  */

class CWriteDisplay : public CDisplay  {
protected:
	friend class CDisplay;
	CWriteDisplay( CWriteWindow* writeWindow );
	~CWriteDisplay();
};

#endif
