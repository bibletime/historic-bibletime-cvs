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
	m_data = QString::null;
	if (!(m_module = dynamic_cast<CSwordBibleModuleInfo*>(module)) ) {	//bad module
		throw EBadModule();
	}
}

CSwordVerseKey::~CSwordVerseKey(){
	m_data = QString::null;
}

/** Stores the data of this key in the member m_data */
void CSwordVerseKey::getData(){
	m_module->module()->SetKey(*this->clone());
	m_data = QString::fromLocal8Bit( (const char*)*m_module->module() );
}

/** Sets the key we use to the parameter. */
const bool CSwordVerseKey::setKey( QString key ){	
	error = 0;	
	VerseKey::operator = ((const char*)key.local8Bit());		
	//clear data
	m_data = QString::null;	
	return !(bool)error;
}

/**  */
const bool CSwordVerseKey::NextVerse(){	
	if (m_module->getType() == CSwordModuleInfo::Commentary) {
		m_module->module()->SetKey(*this->clone());	//use this key as base for the next one!
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
		
		m_module->module()->SetKey(*this->clone());	//use this key as base for the next one!		
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
//this is bad for commentary modules because they do not have all keys
//#warning Implement some special thing for commentaries	
	if (Chapter()<0)
		return false;		
	Chapter(Chapter()+1);	
	return true;
}

/**  */
const bool CSwordVerseKey::PreviousChapter(){
//this is bad for commentary modules because tey do not have all keys
//#warning Implement some special thing for commentaries		
	if (Chapter()<=0)
		return false;	
	Chapter(Chapter()-1);
	return true;
}

/**  */
const bool CSwordVerseKey::NextBook(){
	//this is bad for commentary modules because tey do not have all keys
//#warning Implement some special thing for commentaries					
	if (Book()<0)
		return false;
	Book(Book()+1);		
	return true;
}

/**  */
const bool CSwordVerseKey::PreviousBook(){
	//this is bad for commentary modules because tey do not have all keys
//#warning Implement some special thing for commentaries		
	if (Book()<=0)
		return false;
		
	Book(Book()-1);

	return true;
}

/** Returns the current book as Text, no as integer. */
const QString CSwordVerseKey::getBook() const {
	const int curBook = Book();
	const int testament = Testament();

	QString text = QString::null;
	if ( testament > 0 && curBook <= BMAX[testament-1] ) {
		text = QString::fromLocal8Bit( books[testament-1][curBook-1].name );
	}
	else {
		text = QString::fromLocal8Bit(books[0][0].name);
	}
	return text;
}

/** Returns the current book as Text, no as integer. */
void CSwordVerseKey::setBook( const QString newBook ) {
	const QString newKey = QString("%1 %2:%3").arg(newBook).arg((int)Chapter()).arg((int)Verse());
	if(!setKey(newKey))
		qWarning("Invalid key!");
}

/** Sets the module for this key */
void CSwordVerseKey::setModule( CSwordModuleInfo* module ){
	const QString oldKey = QString::fromLocal8Bit((const char*)*this);
	if (module && (module->getType() == CSwordModuleInfo::Bible || module->getType() == CSwordModuleInfo::Commentary) ) {	
		m_module = module;
		setKey(oldKey);
	}
}

/** Returns the key as a QString. */
const QString CSwordVerseKey::getKey() const {
	return QString::fromLocal8Bit((const char*)*this);
}

/** Sets the key using a versekey object of Sword. */
void CSwordVerseKey::setKey( VerseKey& key ){
	setKey(QString::fromLocal8Bit((const char*)key));
}
