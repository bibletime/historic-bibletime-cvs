/***************************************************************************
                          cswordconfig.h  -  description
                             -------------------
    begin                : Thu Mar 15 2001
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

#ifndef CSWORDCONFIG_H
#define CSWORDCONFIG_H

#include <swconfig.h>

/**This is a small extension to SWConfig, which updates the global config object if a key was updated locally.
  *@author The BibleTime team
  */

class CSwordConfig : public SWConfig  {
public: 
	CSwordConfig(const char* file, SWConfig* globalConfig);
	~CSwordConfig();
  /** A reimplementation of SWConfig. This does automatically merge the keys of this config with the global config. */
  virtual void Save();
private:
	SWConfig* m_globalConfig;
};

#endif
