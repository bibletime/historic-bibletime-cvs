/***************************************************************************
                          cswordbiblemoduleinfo.cpp  -  description
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
#include "cswordbiblemoduleinfo.h"

//Qt includes
#include <qstringlist.h>

//Sword includes
#include <versekey.h>

CSwordBibleModuleInfo::CSwordBibleModuleInfo( CSwordBackend* backend, SWModule* module ) : CSwordModuleInfo(backend, module) {
	qDebug("Constructor of CSwordBibleModuleInfo");
	m_bookList = 0;
	m_cachedLocale = "unknown";
}

CSwordBibleModuleInfo::~CSwordBibleModuleInfo(){
	qDebug("Destructor of CSwordBibleModuleInfo");
	qDebug(module()->Name());
	if (m_bookList)
		delete m_bookList;
}

/** Returns the books available in this module */
QStringList* CSwordBibleModuleInfo::getBooks() {
	qDebug("CSwordBibleModuleInfo::getBooks()");
	ASSERT(m_bookList);
	qDebug((const char*)m_cachedLocale.local8Bit());
	if (m_cachedLocale != backend()->getCurrentBooknameLanguage()){	//if the locale has changed
		if (m_bookList)
			delete m_bookList;
		m_bookList = 0;
	}
		
	if (!m_bookList) {
		m_bookList = new QStringList();	
		VerseKey key;
		for ( int i = 0; i < 2; i++) {
			for ( int j = 0; j < key.BMAX[i]; j++) {
				m_bookList->append( QString::fromLocal8Bit(key.books[i][j].name) );
			}
		}
		m_cachedLocale = backend()->getCurrentBooknameLanguage();		
	}
	return m_bookList;
}

/** Returns the number of chapters for the given book. */
unsigned int CSwordBibleModuleInfo::getChapterCount(const unsigned int book) const {
	qDebug("CSwordBibleModuleInfo::getChapterCount( const unsigned int book ) const");
	
	VerseKey key;
	qDebug((const char*)QString("Book nnumber: %1").arg(book).local8Bit());
	
	int result = 0;
	if ( (book >= 1) && book <= (unsigned int)key.BMAX[0]) {		//Is the book in the old testament?
		result = (key.books[0][book-1].chapmax);
	}
	else if ( (book >= 1) && (book - key.BMAX[0]) <= (unsigned int)key.BMAX[1] ) {	//is the book in the new testament?
		qDebug("new testament");
	 	result = (key.books[1][book-1-key.BMAX[0]].chapmax);
	}
	
	qDebug((const char*)QString::number(result).local8Bit());
	return result;
}

/** Returns the number of verses  for the given chapter. */
unsigned int CSwordBibleModuleInfo::getVerseCount( const unsigned int book, const unsigned int chapter ) const {
	qDebug("CSwordBibleModuleInfo::getVerseCount( const unsigned int book, const unsigned int chapter ) const");	
	qDebug((const char*)QString::number(book).local8Bit());
	qDebug((const char*)QString::number(chapter).local8Bit());
	VerseKey key;
	unsigned int result = 0;
	if ((book>=1) && (book <= (unsigned int)key.BMAX[0]) ) { //Is the book in te old testament?
		if (chapter <= getChapterCount(book) )	//does the chapter exist?
			result = (key.books[0][book-1].versemax[chapter-1]);
	}
	else if ((book>=1) && (book - key.BMAX[0]) <= (unsigned int)key.BMAX[1]) {	//is the book in the new testament?
		if (chapter <= getChapterCount(book) )	//does the chapter exist?
			result = key.books[1][book-1-key.BMAX[0]].versemax[chapter-1];
	}
	
	qDebug((const char*)QString::number(result).local8Bit());
	return result;
}

/** Reimplementation, Returns the type */
CSwordModuleInfo::type CSwordBibleModuleInfo::getType(){
	return CSwordModuleInfo::Bible;
}
