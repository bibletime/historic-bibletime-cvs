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
	m_module = dynamic_cast<CSwordBibleModuleInfo*>(module);
//	Persist(0);
}

/** No descriptions */
CSwordVerseKey::CSwordVerseKey( const CSwordVerseKey& k ) : VerseKey(k),CKey() {
	m_module = k.m_module;
}

CSwordVerseKey::~CSwordVerseKey(){
}

/** Clones this object. */
CSwordVerseKey* CSwordVerseKey::clone() const {
	return new CSwordVerseKey(*this);
}

/** Sets the module for this key */
void CSwordVerseKey::setModule( CSwordModuleInfo* module ){
	const QString oldKey = QString::fromLocal8Bit((const char*)*this);
	if (module && (module->getType() == CSwordModuleInfo::Bible || module->getType() == CSwordModuleInfo::Commentary) ) {	
		m_module = module;
		setKey(oldKey);
	}
}

/** Returns the rendered text of this verse */
const QString CSwordVerseKey::getRenderedText() const {
	m_module->module()->SetKey(this);
	return QString::fromLocal8Bit( (const char*)*m_module->module() );
}

/** Returns the stripped down text of this verse, */
const QString CSwordVerseKey::getStrippedText() const{
	m_module->module()->SetKey(this);
	return QString::fromLocal8Bit( m_module->module()->StripText() );
}

/** Returns the current book as Text, no as integer. */
void CSwordVerseKey::setBook( const QString newBook ) {
	for (int testament = 0; testament < 2; ++testament) {
		for (int book = 0; book < BMAX[testament]; ++book) {
			if ( !strcmp(newBook.local8Bit(),books[testament][book].name ) ) {
				Testament(testament+1);
				Book(book+1);
				return;
			}			
		}
	}
}

/**  */
const bool CSwordVerseKey::NextVerse(){	
	if (m_module->getType() == CSwordModuleInfo::Commentary) {
		m_module->module()->SetKey(this);	//use this key as base for the next one!
		( *( m_module->module() ) )++;
		setKey(m_module->module()->KeyText());		
	}
	else {
		Verse(Verse()+1);
	}	
	return true;
}

/**  */
const bool CSwordVerseKey::PreviousVerse(){
	if (m_module->getType() == CSwordModuleInfo::Commentary) {
		
		m_module->module()->SetKey(this);	//use this key as base for the next one!		
		( *( m_module->module() ) )--;
		setKey(m_module->module()->KeyText());		
	}
	else {
		Verse(Verse()-1);
	}	
	return true;
}

/**  */
const bool CSwordVerseKey::NextChapter(){
	Chapter(Chapter()+1);	
	return true;
}

/**  */
const bool CSwordVerseKey::PreviousChapter(){
	Chapter(Chapter()-1);
	return true;
}

/**  */
const bool CSwordVerseKey::NextBook(){
	if (Book() <= 0 || Book() >= BMAX[Testament()-1] && Testament() > 1)
		return false;	
	
	Book(Book()+1);			
	return true;
}

/**  */
const bool CSwordVerseKey::PreviousBook(){
	if (Book()<=1 || Book() > BMAX[Testament()-1] && Testament() > 1)
		return false;
	Book(Book()-1);
	return true;
}

/** Returns the current book as Text, no as integer. */
const QString CSwordVerseKey::getBook() const {
	if ( Testament() && Book() <= BMAX[Testament()-1] )
		return QString::fromLocal8Bit( books[Testament()-1][Book()-1].name );
	return QString::fromLocal8Bit(books[0][0].name);
}

/** Sets the key we use to the parameter. */
const bool CSwordVerseKey::setKey( QString key ){	
	error = 0;	
	VerseKey::operator = ((const char*)key.local8Bit());		
	//clear data
	return !(bool)error;
}

/** Sets the key using a versekey object of Sword. */
void CSwordVerseKey::setKey( VerseKey& key ){
	setKey(QString::fromLocal8Bit((const char*)key));
}
