/***************************************************************************
                          ckey.h  -  description
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

#ifndef CKEY_H
#define CKEY_H
	
/**
	* CKey is the baseclass for all key used by backends. CKey does only contain the functions used
	*	by all backends (only few functions, e.g. @see #getData()).
	*	The classes derived from this baseclass should be handled manually.
	*
  *	@short The base class for all key objects.
  *	@author The BibleTime team
  *	@version $Id
  */
class CKey {

public: 
	/**
		* The constructor of this CKey class. In derived classes it should initialize the members.
		*/
	CKey() {};
	/**
		* The destructor of CKey, In derived classes this function should delete
		*	the memory allocated by this object.
		*/
	virtual ~CKey() {};
  /**
  	* This function gets the data for the current key (e.g. current verse, current map etc.)
  	*	and stores the data in the member m_data. m_data  has to be managed manually,
  	*	since it has no base type (use dynamic_cast).
  	*/
  virtual void getData() = 0;
	/**
		* Defintions for exception handling in the CKey based classes.
		*/	
	class EBadModule {}; //exception if a wring modle was given

};

#endif
