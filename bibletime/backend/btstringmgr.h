//
// C++ Interface: btstringmgr
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef BTSTRINGMGR_H
#define BTSTRINGMGR_H

//Sword includes
#include <stringmgr.h>

//Qt includes
#include <qstring.h>

using namespace sword;

class BTStringMgr : public StringMgr {
public:
	/** Converts the param to an upper case Utf8 string
	* @param The text encoded in utf8 which should be turned into an upper case string
	*/	
	virtual char* upperUTF8(char*, const unsigned int maxlen = 0);
	
	/** Converts the param to an uppercase latin1 string
	* @param The text encoded in latin1 which should be turned into an upper case string
	*/	
	virtual char* upperLatin1(char*);

protected:
	virtual const bool supportsUnicode() const;
	
	/** CODE TAKEN FROM KDELIBS 3.2
	* This function checks whether a string is utf8 or not.
	*
	* It was taken from kdelibs so we do not depend on KDE 3.2.
	*/
	const bool isUtf8(const char *buf);
};

#endif
