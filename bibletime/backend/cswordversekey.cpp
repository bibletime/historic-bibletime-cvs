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

CSwordVerseKey::CSwordVerseKey( CSwordModuleInfo* module ) {
	m_module = module;
}

/** No descriptions */
CSwordVerseKey::CSwordVerseKey( const CSwordVerseKey& k ) : VerseKey(k),CSwordKey(k) {
// qWarning("copy constructor of CSwordVerseKey");
}

/** No descriptions */
CSwordVerseKey::CSwordVerseKey( const VerseKey* k, CSwordModuleInfo* module) : VerseKey(*k),CSwordKey() {
	m_module = module;	
}

CSwordVerseKey::~CSwordVerseKey(){
}

/** Clones this object. */
CSwordKey* CSwordVerseKey::copy() const {
	CSwordVerseKey* k = new CSwordVerseKey(*this);
	return k;
}

/** Sets the module for this key */
CSwordModuleInfo* CSwordVerseKey::module( CSwordModuleInfo* newModule ){
	if (newModule && (newModule->getType() == CSwordModuleInfo::Bible || newModule->getType() == CSwordModuleInfo::Commentary) ) {
		const QString& oldKey = key();
		m_module = newModule;
		key(oldKey);
	}
	return m_module;
}

/** Returns the current book as Text, not as integer. */
const QString CSwordVerseKey::book( const QString& newBook ) {
	if (!newBook.isEmpty()) {
		int min = 0;
		int max = 1;
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
	if ( Testament() && Book() <= BMAX[Testament()-1] )
		return QString::fromLocal8Bit( books[Testament()-1][Book()-1].name );
	return QString::fromLocal8Bit(books[0][0].name); //return the first book, i.e. Genesis
}

/**  */
const bool CSwordVerseKey::NextVerse(){	
	m_module->module()->SetKey(this);	//use this key as base for the next one!
	( *( m_module->module() ) )++;
	key( QString::fromLocal8Bit(m_module->module()->KeyText()) );//don't use fromUtf8
	
	return true;
}

/**  */
const bool CSwordVerseKey::PreviousVerse(){
	m_module->module()->SetKey(this);	//use this key as base for the next one!		
	( *( m_module->module() ) )--;

	key( QString::fromLocal8Bit(m_module->module()->KeyText()) );//don't use fromUtf8
	
	return true;
}

/** Jump to the next chapter */
const bool CSwordVerseKey::NextChapter(){
//This moves to the next chapter. Sword sets the Verse to 1, but this is ok here
	Chapter(Chapter()+1);	
	return true;
}

/**  */
const bool CSwordVerseKey::PreviousChapter(){
//This moves to the next chapter. Sword sets the Verse to 1, but this is ok here	
	Chapter(Chapter()-1);
	return true;
}

/**  */
const bool CSwordVerseKey::NextBook(){
//This moves to the next chapter. Sword sets the chapter and verse to 1, but this is ok here	
	if (Book() <= 0 || Book() >= BMAX[Testament()-1] && Testament() > 1)
		return false;		
	Book(Book()+1);			
	return true;
}

/**  */
const bool CSwordVerseKey::PreviousBook(){
//This moves to the next chapter. Sword sets the chapter and verse to 1, but this is ok here		
	if (Book()<=1 || Book() > BMAX[Testament()-1] && Testament() > 1)
		return false;
	Book(Book()-1);
	return true;
}

/** Sets the key we use to the parameter. */
const QString CSwordVerseKey::key( const QString& newKey ){	
	if (!newKey.isNull()) {
		VerseKey::operator = ((const char*)newKey.local8Bit());
	}
	return QString::fromLocal8Bit((const char*)*this);//don't use fromUtf8 here!
}

void CSwordVerseKey::key( const char* newKey ){
	if (newKey) {
		VerseKey::operator = (newKey);
	}
}
