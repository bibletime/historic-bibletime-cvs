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
#include "../ckey.h"
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
class CSwordVerseKey :  public VerseKey, public CKey  {
public: 
	/**
		* Constructor of this class.
		*
		* This function will construct a versekey with the current module position
		* and it will setup the m_module members.
		*
		*	If you specifiy a module as parameter, which is not verse based the exception EBadModule
		* is thrown to show this problem to the creator of this class.
		*
		*/
	CSwordVerseKey( CSwordModuleInfo* module );
	/**
		* Destructor of this class.
		*
		* Clean up the data variables and delete objects used and
		* created by this class.
		*/
	~CSwordVerseKey();
  /**
  	* Get the data for the this object in the member variable m_data.
  	*
  	* You have to manually access the m_data variable to get the text.
  	*/
  void getData();
  /**
  	* Sets the key we use to the parameter.
  	*
  	* This will also set the position of the module to the new key.
  	*/
  bool setKey( QString key );
  /**
  	* Gets the key with the next verse in comparision with the current verse and stores
  	* the key in the parameter key.
  	*
  	* If this object and the parameter are the same we'll directly modify this key object.
  	*/
  bool NextVerse();
  /**
  	* Gets the key with the previous verse in comparision with the current verse and stores
  	* the key in the parameter key.
  	*
  	* If this object and the parameter are the same we'll directly modify this key object.
  	*/
  bool PreviousVerse();
  /**
  	* Gets the key with the next chapter in comparision with the current chapter and stores
  	* the key in the parameter key.
  	*
  	* If this object and the parameter are the same we'll directly modify this key object.
  	*/
  bool NextChapter();
  /**
  	* Gets the key with the previous chapter in comparision with the current chapter and stores
  	* the key in the parameter key.
  	* If this object and the parameter are the same we'll directly modify this key object.
  	*/
  bool PreviousChapter();
  /**
  	* Gets the key with the next book in comparision with the current book and stores
  	* the key in the parameter key.
  	*
  	* If this object and the parameter are the same we'll directly modify this key object.
  	*/
  bool NextBook();
  /**
  	* Gets the key with the previous chapter in comparision with the current book and stores
  	* the key in the parameter key.
  	*
  	* If this object and the parameter are the same we'll directly modify this key object.
  	*/
  bool PreviousBook();
  /**
  	* This functions returns the current book as localised text, not as book numer.
  	*
  	* Use "char Book()" to retrieve the book number of the current book.
  	* @return The name of the current book
  	*/
  QString getBook() const;
  /**
  	* Sets the book for this key. It gets a text parameter, not a booknumber.
  	*
  	* Use "Book(char)" to set the number using a number.
  	* @param newBook The QString object which contains the new book for this key.
  	*/
  void setBook(const QString newBook);
  /**
  	* This is our data member, which contains the data for this key.
  	*
  	* Access it to get the content for the current key.
  	* @see setKey() clears the content of this variable, use the function @see getData() to fill it again.
  	*/
  QString m_data;

protected:
  /**
  	* This is the pointer to the module we use.
  	*/
  CSwordModuleInfo*	m_module;
};

#endif
