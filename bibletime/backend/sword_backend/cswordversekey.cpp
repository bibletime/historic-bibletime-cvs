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
//	if (!(m_module = dynamic_cast<CSwordBibleModuleInfo*>(module)) ) {	//bad module
//		throw EBadModule();
//	}
	m_module = dynamic_cast<CSwordBibleModuleInfo*>(module);
}

CSwordVerseKey::~CSwordVerseKey(){
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
const QString CSwordVerseKey::getRenderedText() const{
	m_module->module()->SetKey(*this->clone());
	return QString::fromLocal8Bit( (const char*)*m_module->module() );
}

/** Returns the stripped down text of this verse, */
const QString CSwordVerseKey::getStrippedText() const{
	m_module->module()->SetKey(*this->clone());
	return QString::fromLocal8Bit( m_module->module()->StripText() );
}

/** Returns the current book as Text, no as integer. */
void CSwordVerseKey::setBook( const QString newBook ) {
//	if(!setKey(QString("%1 %2:%3").arg(newBook).arg((int)Chapter()).arg((int)Verse())))
//		qWarning("CSwordVerseKey::setBook: Invalid key!");	
	for (int testament = 0; testament < 2; ++testament) {
		for (int book = 0; book < BMAX[testament]; ++book) {
			if ( !strcmp(newBook.local8Bit(),books[testament][book].name ) ) {
				qDebug("booknumber == %i", book);			
				Testament(testament+1);
				Book(book+1);
				qWarning("VerseKey::setBook: set book to %s",books[testament][book].name);
				return;
			}			
		}
	}
}
