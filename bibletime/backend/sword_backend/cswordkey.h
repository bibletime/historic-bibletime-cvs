/***************************************************************************
                          cswordkey.h  -  description
                             -------------------
    begin                : Thu May 24 2001
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

#ifndef CSWORDKEY_H
#define CSWORDKEY_H

//Own includes
#include "../ckey.h"

//Qt includes
#include <qstring.h>


class CSwordModuleInfo;
/**
 * The base class for all Sword based keys.
 * @author The BibleTime team
 * @version $Id$
 */
class CSwordKey : public CKey  {
public:
	//pure virtual functions
	virtual const QString key(const QString& newKey = QString::null) = 0;
	virtual const QString renderedText() = 0;
	virtual const QString strippedText() = 0;
	virtual const CSwordKey* clone() const = 0;	
	
	//implemented functions
	virtual CSwordModuleInfo* module(CSwordModuleInfo* newModule = 0);

protected:
	CSwordModuleInfo* m_module;

protected:
	CSwordKey();
};

#endif
