/***************************************************************************
                          cswordversekey.h  -  description
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

#ifndef CSWORDVERSEKEY_H
#define CSWORDVERSEKEY_H

//own includes
#include "cswordkey.h"
#include "cswordmoduleinfo.h"

//Qt includes
#include <qstring.h>

//Sword includes
#include <versekey.h>

/**
	* The CKey implementation for verse based modules (Bibles and Commentaries)
	*
	*	This class is the implementation of CKey for verse based modules like
	*	Bibles and commentaries.
	* This class provides the special functions to work with the verse based modules.
	*
	* Useful functions are
	*	@see NextBook()
	* @see PreviousBook()
	* @see NextChapter()
	* @see PreviousChapter()
	* @see NextVerse()
	* @see PreviousVerse().
	*
	* Call the constructor only with a valid verse based modules, otherwise this key will be invalid
	* and the application will probably crash.
	*
	*	@version $Id$
  *	@author The BibleTime team
  */
class CSwordVerseKey :  public VerseKey, public CSwordKey  {
public: 
	/**
	* Constructor of this class.
	*
	* This function will construct a versekey with the current module position
	* and it will setup the m_module members.
	*
	*/
	CSwordVerseKey( CSwordModuleInfo* module );
  /**
  * Copy constructor.
  */
  CSwordVerseKey( const CSwordVerseKey& k );
	/**
	* Destructor of this class.
	*
	* Clean up the data variables and delete objects used and
	* created by this class.
	*/
	virtual ~CSwordVerseKey();
  /**
  * Set/get the key. If the parameter is not set (means equal to QString::null)
  * the used key is returned. Otherwise the key is set and the new on ei returned.
  */
  virtual const QString key( const QString& key = QString::null );
  /**
  * Set/get the key. If the parameter is not set (means equal to QString::null)
  * the used key is returned. Otherwise the key is set and the new on ei returned.
  */
	virtual void key( const char* key );
  /**
  * Gets the key with the next verse in comparision with the current verse and stores
  * the key in the parameter key.
  *
  * If this object and the parameter are the same we'll directly modify this key object.
  */
  const bool NextVerse();
  /**
  * Gets the key with the previous verse in comparision with the current verse and stores
  * the key in the parameter key.
  *
  * If this object and the parameter are the same we'll directly modify this key object.
  */
  const bool PreviousVerse();
  /**
  * Gets the key with the next chapter in comparision with the current chapter and stores
  * the key in the parameter key.
  *
  * If this object and the parameter are the same we'll directly modify this key object.
  */
  const bool NextChapter();
  /**
  * Gets the key with the previous chapter in comparision with the current chapter and stores
  * the key in the parameter key.
  * If this object and the parameter are the same we'll directly modify this key object.
  */
  const bool PreviousChapter();
  /**
  * Gets the key with the next book in comparision with the current book and stores
  * the key in the parameter key.
  *
  * If this object and the parameter are the same we'll directly modify this key object.
  */
  const bool NextBook();
  /**
  * Gets the key with the previous chapter in comparision with the current book and stores
  * the key in the parameter key.
  *
  * If this object and the parameter are the same we'll directly modify this key object.
  */
  const bool PreviousBook();
  /**
  * This functions returns the current book as localised text, not as book numer.
  *
  * Use "char Book()" to retrieve the book number of the current book.
  * @return The name of the current book
  */
  const QString book(const QString& newBook = QString::null);
  /**
  * Sets the module for this key
  */
  virtual CSwordModuleInfo* module( CSwordModuleInfo* newModule = 0 );
  /**
  * Clones this object.
  */
  virtual CSwordVerseKey* clone() const;
};

#endif
