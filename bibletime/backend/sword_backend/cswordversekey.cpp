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
	if (!module)
		return;
	VerseKey* vk = (VerseKey*)(SWKey*)*(module->module());	
	m_oldKey = QString::null;
	if (vk) {
		m_oldKey = QString::fromLocal8Bit((const char*)*vk);		
		key(QString::fromLocal8Bit((const char*)*vk));
	}
}

/** No descriptions */
CSwordVerseKey::CSwordVerseKey( const CSwordVerseKey& k ) : VerseKey(k),CSwordKey() {
	m_module = k.m_module;	
}

CSwordVerseKey::~CSwordVerseKey(){
//	if (module() && module()->module()) {
//		VerseKey k(m_oldKey.local8Bit());
//		module()->module()->SetKey(k);
//	}
}

/** Clones this object. */
CSwordVerseKey* CSwordVerseKey::clone() const {
	return new CSwordVerseKey(*this);
}

/** Sets the module for this key */
CSwordModuleInfo* CSwordVerseKey::module( CSwordModuleInfo* newModule ){
	if (newModule && (newModule->getType() == CSwordModuleInfo::Bible || newModule->getType() == CSwordModuleInfo::Commentary) ) {
		const QString oldKey = key();

		m_module = newModule;
		key(oldKey);
	}
	return m_module;
}

/** Returns the current book as Text, no as integer. */
const QString CSwordVerseKey::book( const QString& newBook ) {
//	qDebug("const QString CSwordVerseKey::book( const QString& newBook )");
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
	return QString::fromLocal8Bit(books[0][0].name);
}

/**  */
const bool CSwordVerseKey::NextVerse(){	
//	if (m_module->getType() == CSwordModuleInfo::Commentary) {
//		qDebug("CSwordVerseKey::NextVerse()");
		m_module->module()->SetKey(this);	//use this key as base for the next one!
		( *( m_module->module() ) )++;
		key(m_module->module()->KeyText());		
//	}
//	else
//		Verse(Verse()+1);
	return true;
}

/**  */
const bool CSwordVerseKey::PreviousVerse(){
//	if (m_module->getType() == CSwordModuleInfo::Commentary) {		
//		qDebug("CSwordVerseKey::PreviousVerse()");		
		m_module->module()->SetKey(this);	//use this key as base for the next one!		
		( *( m_module->module() ) )--;
		key(m_module->module()->KeyText());		
//	}
//	else
//		Verse(Verse()-1);
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

/** Sets the key we use to the parameter. */
const QString CSwordVerseKey::key( const QString& newKey ){	
//	qDebug("const QString CSwordVerseKey::key( const QString& newKey )");
	if (!newKey.isNull()) {
		qDebug(newKey.latin1());
		error = 0;
		VerseKey::operator = ((const char*)newKey.local8Bit());
	}
	return QString::fromLocal8Bit(VerseKey::operator const char*());
}

void CSwordVerseKey::key( const char* newKey ){
//	qDebug("const char* CSwordVerseKey::key( const char* newKey )");
	if (newKey) {
		qDebug(newKey);
		error = 0;
		VerseKey::operator = (newKey);
	}
}
