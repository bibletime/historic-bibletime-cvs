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

#define NO_SWORD_NAMESPACE


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

protected:
	CSwordKey(CSwordModuleInfo* module = 0); //protected constructor, because CSwordKey shouldn't be used (it's an abstract base class).
	CSwordKey(const CSwordKey&); //copy constructor

public:
	virtual ~CSwordKey() {};
	
	
	//pure virtual functions		
	virtual const QString key(const QString& = QString::null) = 0;
	virtual void key(const char*) = 0;
	virtual CSwordKey* copy() const = 0;	
	
	//implemented functions
	virtual CSwordModuleInfo* module(CSwordModuleInfo* newModule = 0);
	virtual const QString renderedText();
	virtual const QString strippedText();
  /**
  * This returns a new object of the right CSwordKey* implementation (e.g. CSwordVerseKey or CSwordLDKey)
  * The type is determined by the type of the module.
  * @see CSwordModuleInfo, CSwordBibleModuleInfo, CSwordCommentaryModuleInfo, CSwordLexiconModukleInfo
  */
  static CSwordKey* createInstance(CSwordModuleInfo * const module);

protected:
	CSwordModuleInfo* m_module; //module pointer used by all keys
};

#endif
