/***************************************************************************
                          cmoduleinfo.h  -  description
                             -------------------
    begin                : Wed Oct 11 2000
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

#ifndef CMODULEINFO_H
#define CMODULEINFO_H

#include <qlist.h>

/**	The base class for all CModuleInfo derived classes. It does only contain the
	* methods being available in all modules you can think of.
	*
  * @short CModuleInfo is the base class for all modules
  *	@author The BibleTime team
  * $Id
  */
class CModuleInfo {
public: 
	CModuleInfo() {};
	~CModuleInfo() {};
};

typedef QList<CModuleInfo> ListCModuleInfo;

#endif
