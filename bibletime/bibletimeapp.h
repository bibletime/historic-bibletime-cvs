/***************************************************************************
                          bibletimeapp.h  -  description
                             -------------------
    begin                : Sam Jul 12 2003
    copyright            : (C) 2003 by The BibleTime team
    email                : info@bibletime.info
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BIBLETIMEAPP_H
#define BIBLETIMEAPP_H

//BibleTime
#include "util/cpointers.h"

//KDE
#include <kapplication.h>

/** The BibleTimeApp class is used to clean up all instances of the backend and to delete all created module objects.
  * @author The BibleTime team
  */
class BibleTimeApp : public KApplication, public CPointers {
public: 
	BibleTimeApp();
	virtual ~BibleTimeApp();
};

#endif
