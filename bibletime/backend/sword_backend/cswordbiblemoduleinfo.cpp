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
	m_bookList = 0;
	m_cachedLocale = "unknown";
}

CSwordBibleModuleInfo::~CSwordBibleModuleInfo(){
	if (m_bookList)
		delete m_bookList;
}

/** Returns the books available in this module */
QStringList* CSwordBibleModuleInfo::getBooks() {
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
const unsigned int CSwordBibleModuleInfo::getChapterCount(const unsigned int book) const {
	VerseKey key;	
	int result = 0;
	if ( (book >= 1) && book <= (unsigned int)key.BMAX[0]) {		//Is the book in the old testament?
		result = (key.books[0][book-1].chapmax);
	}
	else if ( (book >= 1) && (book - key.BMAX[0]) <= (unsigned int)key.BMAX[1] ) {	//is the book in the new testament?
	 	result = (key.books[1][book-1-key.BMAX[0]].chapmax);
	}
	return result;
}

/** Returns the number of verses  for the given chapter. */
const unsigned int CSwordBibleModuleInfo::getVerseCount( const unsigned int book, const unsigned int chapter ) const {
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
	return result;
}

/** Reimplementation, Returns the type */
const CSwordModuleInfo::type CSwordBibleModuleInfo::getType(){
	return CSwordModuleInfo::Bible;
}

/** @return the book number, values starting with 1; 0 if not found */
const unsigned int CSwordBibleModuleInfo::getBookNumber(const QString &book){
  if (!m_bookList)
    getBooks();
  return  (m_bookList->findIndex(book)+1);
}
