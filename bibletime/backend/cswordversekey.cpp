/***************************************************************************
                          cswordversekey.cpp  -  description
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

//BibleTime includes
#include "cswordversekey.h"
#include "cswordbiblemoduleinfo.h"
#include "cswordcommentarymoduleinfo.h"

//Qt includes
#include <qstringlist.h>

//Sword includes
#include <swmodule.h>

CSwordVerseKey::CSwordVerseKey( CSwordModuleInfo* module ) : CSwordKey(module) {
  if ( CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(module) ) {
    key( bible->lowerBound().key() );
  }
}

CSwordVerseKey::CSwordVerseKey( const CSwordVerseKey& k ) : VerseKey(k),CSwordKey(k) {

}

CSwordVerseKey::CSwordVerseKey( const VerseKey* k, CSwordModuleInfo* module) : VerseKey(*k),CSwordKey(module) {
}

/** Clones this object. */
CSwordKey* CSwordVerseKey::copy() const {
	return new CSwordVerseKey(*this);
}

/** Sets the module for this key */
CSwordModuleInfo* const CSwordVerseKey::module( CSwordModuleInfo* const newModule ){
	if (newModule && (newModule->type() == CSwordModuleInfo::Bible || newModule->type() == CSwordModuleInfo::Commentary) ) {
		const QString& oldKey	= key();
		m_module = newModule;
		key(oldKey);
	}
	return dynamic_cast<CSwordBibleModuleInfo*>(m_module);
}

/** Returns the current book as Text, not as integer. */
const QString CSwordVerseKey::book( const QString& newBook ) {
//  qWarning("CSwordVerseKey::book( const QString& newBook )");
	int min = 0;
	int max = 1;
	
	if (CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(module())) {
		const bool hasOT = bible->hasTestament(CSwordBibleModuleInfo::OldTestament);		
		const bool hasNT = bible->hasTestament(CSwordBibleModuleInfo::NewTestament);
		if (hasOT && hasNT) {
			min = 0;
			max = 1;
		}
		else if (hasOT && !hasNT) {
			min = 0;
			max = 0;
		}
		else if (!hasOT && hasNT) {
			min = 1;
			max = 1;
		}
		else if (!hasOT && !hasNT) {
			min = 0;
			max = -1; //no loop
		}
  }

	if (!newBook.isEmpty()) {  				
		bool finished = false;
		for (int testament = min; testament <= max && !finished; ++testament) {
			for (int book = 0; book < BMAX[testament] && !finished; ++book) {
				if ( !strcmp((const char*)newBook.local8Bit(),books[testament][book].name ) ) {
					Testament(testament+1);
					Book(book+1);
					finished = true;
				}
			}
		}
	}
	if ( Testament() >= min+1 && Testament() <= max+1 && Book() <= BMAX[min] )
		return QString::fromLocal8Bit( books[Testament()-1][Book()-1].name );
//  qWarning("return standard book: %s", books[min][0].name);
	return QString::fromLocal8Bit( books[min][0].name ); //return the first book, i.e. Genesis
}

/** Sets the key we use to the parameter. */
const QString CSwordVerseKey::key(){	
	return QString::fromLocal8Bit((const char*)*this); //don't use fromUtf8 here!
}

void CSwordVerseKey::key( const QString& newKey ) {
	if (newKey.isEmpty()) {
    if ( CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(module()) ) {
      VerseKey::operator = ((const char*)bible->lowerBound().key().local8Bit());
    }
  }
  else
    VerseKey::operator = ((const char*)newKey.local8Bit());
}

void CSwordVerseKey::key( const char* newKey ){
  if (newKey && strlen(newKey)>0) {
		VerseKey::operator = (newKey);
	}
  else if (!strlen(newKey)) {
    if ( CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(module()) ) {
      VerseKey::operator = ((const char*)bible->lowerBound().key().local8Bit());
    }
  }
}

const bool CSwordVerseKey::next( const JumpType type ) {
//  const QString& oldKey = key();
	switch (type) {
		case UseBook: {
			if (Book() <= 0 || Book() >= BMAX[Testament()-1] && Testament() > 1)
				return false;		
			Book(Book()+1);
			break;
		}
		case UseChapter: {
			Chapter(Chapter()+1);		
			break;
		}
		case UseVerse: {
    	if (m_module && m_module->module()) {
    		m_module->module()->SetKey(this);	//use this key as base for the next one!	
    		(*(m_module->module()) )++;
    		if (!m_module->module()->Error())		
    			key( QString::fromLocal8Bit(m_module->module()->KeyText()) );//don't use fromUtf8
    		else {    			
	    	  Verse(Verse()+1);
	    	  break;
	    	}
    	}
    	else
    	  Verse(Verse()+1);
    	break;
		}
		default:	
			return false;
	};

  if ( CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(module()) ) {
    bool ret = true;
    if (_compare(bible->lowerBound()) < 0 ) {
      key( bible->lowerBound() );
      ret = false;
    }
    if (_compare(bible->upperBound()) > 0 ) {
      key( bible->upperBound() );
      ret = false;
    }
    return ret;
  }
  return true;
};

const bool CSwordVerseKey::previous( const JumpType type ) {
//  const QString& oldKey = key();
	switch (type) {
		case UseBook: {
			if (Book()<=1 || Book() > BMAX[Testament()-1] && Testament() > 1)
				return false;
			Book(Book()-1);
			break;
		}
		case UseChapter: {
			Chapter(Chapter()-1);
			break;
		}
		case UseVerse: {
    	if (m_module && m_module->module()) {
    		m_module->module()->SetKey(this);	//use this key as base for the next one!			
    		( *( m_module->module() ) )--;
    		if (!m_module->module()->Error())
    			key( QString::fromLocal8Bit(m_module->module()->KeyText()) );//don't use fromUtf8
    		else
	    	  Verse(Verse()-1);
    	}
    	else {
    		Verse(Verse()-1);
      }
      break;
		}
		default:	
			return false;
	};

  if ( CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(module()) ) {
    qWarning("lower bound: %s", bible->lowerBound().key().latin1());
    qWarning("upper bound: %s", bible->upperBound().key().latin1());

    bool ret = true;
    if (_compare(bible->lowerBound()) < 0 ) {
      key( bible->lowerBound() );
      ret = false;
    }
    if (_compare(bible->upperBound()) > 0 ) {
      key( bible->upperBound() );
      ret = false;
    }
    return ret;
  }
  return true;
};

/** Assignment operator for more ease of use. */
CSwordVerseKey& CSwordVerseKey::operator = (const QString& keyname){
  key(keyname);
  return *this;
}
