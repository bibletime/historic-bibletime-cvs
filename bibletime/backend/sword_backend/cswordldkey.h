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
#include "../ckey.h"
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
	*		const char* keyname = (const char*)*key;
	* @endcode
	*
  *	@author The BibleTime team
  *	@version $Id$
  */

class CSwordLDKey : public SWKey, public CKey {
public: 
	/**
		* Constructor of CSwordLDKey
		*/
	CSwordLDKey(  CSwordModuleInfo* module );
	~CSwordLDKey();
  /**
  	* Stores the data of this key in the member m_data.
  	*/
//  virtual void getData();
  /**
  	* Sets the key for this key object. The variable m_data will be cleared.
  	* Use @ref getData() to fill the m_data variable again.
  	*/
  virtual bool setKey( const QString );
  /**
  	* Uses the parameter to returns the next entry afer this key.
  	*/
  virtual void NextEntry( void );
  /**
  	* Uses the parameter to returns the previous entry afer this key.
  	*/
  virtual void PreviousEntry( void );  	
  /** Sets the module of this key. */
  void setModule( CSwordModuleInfo* module );
  /** Returns the current key as a QString */
  const QString getKey() const;
  /** Reimplementation of the cast operator to const char* */
  virtual  operator const char*();
  /** Returns the stripped down text of this entry. */
  const QString getStrippedText() const;
  /** Returns the rendered text of this entry. */
  const QString getRenderedText() const;
  /**
  	* This is out data member, which contains the data for this key.
  	*/
  QString m_data;
  /**
	 	* This is the pointer to the module we use.
  	*/
  CSwordModuleInfo*	m_module;
};

#endif
