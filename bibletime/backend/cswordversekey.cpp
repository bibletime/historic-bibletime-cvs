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
CSwordModuleInfo* CSwordVerseKey::module( CSwordModuleInfo* newModule ){
	if (newModule && (newModule->type() == CSwordModuleInfo::Bible || newModule->type() == CSwordModuleInfo::Commentary) ) {
		const QString& oldKey	= key();
		m_module = newModule;
		key(oldKey);
	}
	return m_module;
}

/** Returns the current book as Text, not as integer. */
const QString CSwordVerseKey::book( const QString& newBook ) {
	int min = 0;
	int max = 1;
	
	if (!newBook.isEmpty()) {
		CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(module());
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
	if ( Testament()> 0 && Testament() <=2 && Book() <= BMAX[Testament()-1] )
		return QString::fromLocal8Bit( books[Testament()-1][Book()-1].name );
	return QString::fromLocal8Bit( books[min][0].name ); //return the first book, i.e. Genesis
}

/** Sets the key we use to the parameter. */
const QString CSwordVerseKey::key( const QString& newKey ){	
	if (!newKey.isEmpty()) {
		VerseKey::operator = ((const char*)newKey.local8Bit());
	}
	return QString::fromLocal8Bit((const char*)*this);//don't use fromUtf8 here!
}

void CSwordVerseKey::key( const char* newKey ){
	if (newKey) {
		VerseKey::operator = (newKey);
	}
}

const bool CSwordVerseKey::next( const JumpType type ) {
	switch (type) {
		case UseBook: {
			if (Book() <= 0 || Book() >= BMAX[Testament()-1] && Testament() > 1)
				return false;		
			Book(Book()+1);
			return true;
		}
		case UseChapter: {
			Chapter(Chapter()+1);		
			return true;
		}
		case UseVerse: {
    	if (m_module && m_module->module()) {
    		m_module->module()->SetKey(this);	//use this key as base for the next one!	
    		(*(m_module->module()) )++;
    		if (!m_module->module()->Error())		
    			key( QString::fromLocal8Bit(m_module->module()->KeyText()) );//don't use fromUtf8
    		else {    			
	    	  Verse(Verse()+1);
	    	  return false;
	    	}
    	}
    	else
    	  Verse(Verse()+1);
    	return true;		
		}
		default:	
			return false;
	};
};

const bool CSwordVerseKey::previous( const JumpType type ) {
	switch (type) {
		case UseBook: {
			if (Book()<=1 || Book() > BMAX[Testament()-1] && Testament() > 1)
				return false;
			Book(Book()-1);
			return true;
		}
		case UseChapter: {
			Chapter(Chapter()-1);
			return true;		
		}
		case UseVerse: {
    	if (m_module && m_module->module()) {
    		m_module->module()->SetKey(this);	//use this key as base for the next one!			
    		( *( m_module->module() ) )--;
    		if (!m_module->module()->Error())
    			key( QString::fromLocal8Bit(m_module->module()->KeyText()) );//don't use fromUtf8
    		else {
	    	  Verse(Verse()-1);
	    	  return false;
	    	}
    	}
    	else
    		Verse(Verse()-1);
    	
    	return true;		
		}
		default:	
			return false;
	};
};
