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


//Qt includes
#include <qstring.h>


class CSwordModuleInfo;
/**
 * The base class for all Sword based keys.
 * @author The BibleTime team
 * @version $Id$
 */
class CSwordKey {
public:
	virtual ~CSwordKey() {};
	virtual const QString key(const QString& = QString::null) {return QString::null;};
	virtual void key(const char*) {};
	
	//pure virtual functions	
	virtual const CSwordKey* clone() const = 0;	
	
	//implemented functions
	virtual CSwordModuleInfo* module(CSwordModuleInfo* newModule = 0);
	virtual const QString renderedText();
	virtual const QString strippedText();
  /**
  * This will create a proper key object from a given module
  */
  static CSwordKey * createInstance( CSwordModuleInfo *module);

protected:
	CSwordModuleInfo* m_module;
	CSwordKey();
	QString m_oldKey;
};

#endif
