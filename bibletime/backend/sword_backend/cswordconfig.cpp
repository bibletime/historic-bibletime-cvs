/***************************************************************************
                          cswordconfig.cpp  -  description
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

#include "cswordconfig.h"

CSwordConfig::CSwordConfig(const char* file, SWConfig* globalConfig) : SWConfig(file) {
	m_globalConfig = globalConfig;
	if (m_globalConfig)
		*m_globalConfig += *this;
}

CSwordConfig::~CSwordConfig(){
}

/** A reimplementation of SWConfig. This does automatically merge the keys of this config with the global config. */
void CSwordConfig::Save(){
	SWConfig::Save();	
	if (m_globalConfig)
		*m_globalConfig += *this;
}
