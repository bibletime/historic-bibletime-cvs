/***************************************************************************
                          cswordlexiconmoduleinfo.h  -  description
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

#ifndef CSWORDLEXICONMODULEINFO_H
#define CSWORDLEXICONMODULEINFO_H

//own includes
#include "cswordmoduleinfo.h"

//Qt includes
#include <qstringlist.h>

/**
	* The implementation of CModuleInfo for the Sword lexiccons and citionaries.
  * @author The BibleTime team
  * @version $Id$
  */
class CSwordLexiconModuleInfo : public CSwordModuleInfo {
public: 
	/**
	* The standard constructor fot this object.
	* A default constructor doesn't exist. Use this one.
	*/
	CSwordLexiconModuleInfo( CSwordBackend* backend, SWModule* module );
	/**
	* Te copy constructor
	*/
	CSwordLexiconModuleInfo( const CSwordLexiconModuleInfo& m );
	/**
	*
	*/	
	~CSwordLexiconModuleInfo();	
  /**
 	* Returns the entries of the module.
 	* This function returns the entries of the modules represented by this object.
 	* If this function is called for the first time the list is load from disk and stored in a list which cahes it.
 	* If the function is called again, the cached list is returned so we have a major speed improvement.
 	*/
  QStringList* getEntries();
  /**
  * Reimplementation.
  */
  virtual const CSwordModuleInfo::type getType() const;

private:
	/**
	* This is the list which caches the entres of the module.
	*/
	QStringList* m_entryList;
};

inline const CSwordModuleInfo::type CSwordLexiconModuleInfo::getType() const {
	return CSwordModuleInfo::Lexicon;
}

#endif
