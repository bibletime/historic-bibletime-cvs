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
#include <qfile.h>

//Sword includes
#include <versekey.h>

static VerseKey staticKey;

CSwordBibleModuleInfo::CSwordBibleModuleInfo( CSwordBackend* backend, SWModule* module ) : CSwordModuleInfo(backend, module) {
	m_bookList = 0;
	m_cachedLocale = "unknown";
	m_hasOT = -1;
	m_hasNT = -1;
}

CSwordBibleModuleInfo::CSwordBibleModuleInfo( const CSwordBibleModuleInfo& m ) : CSwordModuleInfo(m) {
	m_bookList = 0;
	if (m.m_bookList) {
		m_bookList = new QStringList();
		*m_bookList = *m.m_bookList;
	}
	m_cachedLocale = m.m_cachedLocale;	
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
//		const QString modulePath = backend()->getModulePath(QString::fromLocal8Bit(module()->Name()));
//		if (modulePath.isEmpty()) {
			m_hasNT = m_hasOT = true;			
//		}
//		else {
//			if (m_hasOT == -1)
//				m_hasOT = QFile::exists(QString::fromLatin1("%1/ot.vss").arg(modulePath)) || QFile::exists(QString::fromLatin1("%1/ot.bzs").arg(modulePath));
//			if (m_hasNT == -1)
//				m_hasNT = QFile::exists(QString::fromLatin1("%1/nt.vss").arg(modulePath)) || QFile::exists(QString::fromLatin1("%1/nt.bzs").arg(modulePath));
//		}
	
		m_bookList = new QStringList();	
		int max = -1;
		int min = -1;
		//find out if we have ot and nt, only ot or only nt
		if (m_hasOT && m_hasNT) {
			min = 0;
			max = 1;
		}
		else if (m_hasOT && !m_hasNT) {
			min = 0;
			max = 0;
		}
		else if (!m_hasOT && m_hasNT) {
			min = 1;
			max = 1;
		}

		staticKey.setLocale(LocaleMgr::systemLocaleMgr.getDefaultLocaleName());
		for (int i = min; i <= max; ++i) {
			for ( int j = 0; j < staticKey.BMAX[i]; ++j) {
				m_bookList->append( QString::fromLocal8Bit(staticKey.books[i][j].name) );
			}
		}
		m_cachedLocale = backend()->getCurrentBooknameLanguage();		
	}
	return m_bookList;
}

/** Returns the number of chapters for the given book. */
const unsigned int CSwordBibleModuleInfo::getChapterCount(const unsigned int book) const {
	int result = 0;
	if ( (book >= 1) && book <= (unsigned int)staticKey.BMAX[0]) {		//Is the book in the old testament?
		result = (staticKey.books[0][book-1].chapmax);
	}
	else if ( (book >= 1) && (book - staticKey.BMAX[0]) <= (unsigned int)staticKey.BMAX[1] ) {	//is the book in the new testament?
	 	result = (staticKey.books[1][book-1-staticKey.BMAX[0]].chapmax);
	}
	return result;
}

/** Returns the number of verses  for the given chapter. */
const unsigned int CSwordBibleModuleInfo::getVerseCount( const unsigned int book, const unsigned int chapter ) const {
	unsigned int result = 0;
	if ((book>=1) && (book <= (unsigned int)staticKey.BMAX[0]) ) { //Is the book in te old testament?
		if (chapter <= getChapterCount(book) )	//does the chapter exist?
			result = (staticKey.books[0][book-1].versemax[chapter-1]);
	}
	else if ((book>=1) && (book - staticKey.BMAX[0]) <= (unsigned int)staticKey.BMAX[1]) {	//is the book in the new testament?
		if (chapter <= getChapterCount(book) )	//does the chapter exist?
			result = staticKey.books[1][book-1-staticKey.BMAX[0]].versemax[chapter-1];
	}
	return result;
}

const unsigned int CSwordBibleModuleInfo::getBookNumber(const QString &book){
	unsigned int bookNumber = 0;
	bool found = false;
	staticKey.setLocale(LocaleMgr::systemLocaleMgr.getDefaultLocaleName());
	for (int i = 0; i <= 1 && !found; ++i) {
		for ( int j = 0; j < staticKey.BMAX[i] && !found; ++j) {
			++bookNumber;
			if (book == QString::fromLocal8Bit( staticKey.books[i][j].name) )
				found = true;
		}
	}		
	return bookNumber;
}

/** Returns true if his module has the text of desired type of testament */
const bool CSwordBibleModuleInfo::hasTestament( CSwordBibleModuleInfo::Testament type ){
	switch (type) {
		case oldTestament:
			return m_hasOT;
		case newTestament:
			return m_hasNT;		
		default:
			return false;
	}
}
