/***************************************************************************
                          cbackend.h  -  description
                             -------------------
    begin                : Thu Oct 5 2000
    copyright            : (C) 2000 by The BibleTime team
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

#ifndef CBACKEND_H
#define CBACKEND_H

/**
	*	This is the base class for all backends, It does only contain the functions
	*	used by all backends.
	*
	*	@short This is the base class for all backends.
  *	@author The BibleTime team
  *	@version $Id
  */
class CBackend {
public: 
	CBackend(){};
	~CBackend(){};
};

#endif
