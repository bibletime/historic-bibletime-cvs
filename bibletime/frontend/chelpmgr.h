/***************************************************************************
                          chelpmgr.h  -  description
                             -------------------
    begin                : Mon Sep 16 2002
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

#ifndef CHELPMGR_H
#define CHELPMGR_H

//Qt includes
#include <qstring.h>

//KDE includes

/**Provides static functions to easily access the Tooltip and WhatsThis texts for all the frontend parts.
  *@author The BibleTime team
  */

class CHelpMgr {
public: 
  /**
  * Returns the correct Tooltip help text for the given part of the GUI
  */
//  static const QString tooltip();

  /**
  * Returns the correct WhatsThus? help text for the given part of the GUI
  */
//  static const QString whatsthis();

private:
  //constructor and destructor shouldn't be accessible, since we provide only static functions
  CHelpMgr() {};
//	~CHelpMgr() {};
};

#endif
