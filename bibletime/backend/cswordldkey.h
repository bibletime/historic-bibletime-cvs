/***************************************************************************
                          cswordldkey.h  -  description
                             -------------------
    begin                : Thu Oct 5 2000
    copyright            : (C) 2000 by The BibleTime team
    email                : info@bibletime.de
 ***************************************************************************

 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CSWORDLDKEY_H
#define CSWORDLDKEY_H


//own includes
#include "cswordkey.h"
#include "cswordmoduleinfo.h"

//Qt includes
#include <qstring.h>

//Sword includes
#include <swkey.h>

/**
	*	This class is the implementation of CKey used for dictionaries and lexicons.
	*
	*	CSwordLDKey is the implementation of CKey for Lexicons and dictionaries.
	* It provides a simple interface to set the current key,
	* to get the text for the key and functions to get the next and previous items
	* of the used module in comparision to the current key.<BR>
	* Here's an example how to use this class:<BR>
	* @code
	*		CSwordLexiconModuleInfo* m_module = new CSwordLexiconModuleInfo( sword_module );
	*		CSwordLDKey* ldKey = new CSwordLDKey(m_module);
	*		ldKey->getPreviousEntry( ldKey );
	*		qDebug( QString("The current key is: %1").arg(QString::fromLocal8Bit((const char*)*ldKey)));
	* @endcode
	*
	* Please not, that the result will be invalid if use the operator const char*
	* on the adress of the object, use something like this
  *
	* @code
	*		CSwordLDKey* key = new CSwordLDKey( lexicon_module );
	*		const QString keyname = key->getKey();
	* @endcode
	*
  *	@author The BibleTime team
  *	@version $Id$
  */

class CSwordLDKey : public SWKey, public CSwordKey {
public: 
	/**
	* Constructor of CSwordLDKey
	*/
	CSwordLDKey( CSwordModuleInfo* module );
	/**
	* Copy constructor for this key class.
	*/
	CSwordLDKey( const CSwordLDKey &k );
	virtual ~CSwordLDKey();
  /**
  * Clones this object by copying the members.
  */
  virtual CSwordLDKey* clone() const;
  /**
  * Uses the parameter to returns the next entry afer this key.
  */
  void NextEntry( void );
  /**
  * Uses the parameter to returns the previous entry afer this key.
  */
  void PreviousEntry( void );  	
  /**
  * Sets the module of this key.
  */
  virtual CSwordModuleInfo* module( CSwordModuleInfo* module = 0 );
  /**
  * Returns the current key as a QString
  */
  virtual const QString key( const QString& newKey = QString::null);
  /**
  * Returns the current key as a QString
  */
  virtual void key( const char* );
};


#endif

