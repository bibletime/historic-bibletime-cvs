/***************************************************************************
                          cbtconfig.h  -  description
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

#ifndef CBTCONFIG_H
#define CBTCONFIG_H

#include <qstring.h>
#include <qfont.h>

/**This class is the interface to the config object of BibleTime
  *@author The BibleTime team
  */

class CBTConfig {
public:

	enum strings{
	};
	enum fonts{
	};
	enum colors{
	};
	enum bools{
	};

  /** No descriptions */
  static QString get( CBTConfig::strings );
  /** No descriptions */
  static QFont get(CBTConfig::fonts );
};

#endif
