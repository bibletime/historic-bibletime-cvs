/***************************************************************************
                          cbtconfig.cpp  -  description
                             -------------------
    begin                : Mon Nov 19 2001
    copyright            : (C) 2001 by The BibleTime team
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

#include "cbtconfig.h"

#include <kconfig.h>

/* 	No constructor and destructor, because this class only contains static methods.
		It won't be instantiated. */


/** No descriptions */
QString CBTConfig::get( CBTConfig::strings ){
}
/** No descriptions */
QFont CBTConfig::get(CBTConfig::fonts ){
}
