/***************************************************************************
                          cswordlexiconmoduleinfo.cpp  -  description
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
#include "cswordlexiconmoduleinfo.h"

//Qt includes

//Sword includes
#include <swmodule.h>


CSwordLexiconModuleInfo::CSwordLexiconModuleInfo( CSwordBackend* backend, SWModule* module) : CSwordModuleInfo(backend, module) {
	m_entryList = 0;
}

CSwordLexiconModuleInfo::~CSwordLexiconModuleInfo(){
	if (m_entryList)
		delete m_entryList;
}

/** Returns the entries of the module. */
QStringList* CSwordLexiconModuleInfo::getEntries(){
	qDebug("CSwordLexiconModuleInfo::getEntries()");
	ASSERT(module());
	ASSERT(m_entryList);
	
	if (!m_entryList) {
		m_entryList = new QStringList();
		
		if (!module())
			return 0;
		SWKey	old = (*(SWKey*)*module());
		qDebug((const char*)old);
		
		int counter = 0;
		QString currentKey;
		module()->SetKey(" ");
		do {
			currentKey = QString::fromLocal8Bit(module()->KeyText());
			(*module())++;
			if ((counter == 0) && (!currentKey.stripWhiteSpace().isEmpty())) {
				m_entryList->append(currentKey);
			}
			if (currentKey == QString::fromLocal8Bit(module()->KeyText()) )
				counter++;
			else
				counter = 0;
				
		} while (counter < 5 );
		module()->SetKey(" ");
	}
	ASSERT(m_entryList);
	return m_entryList;
}

/** Reimplementation. */
CSwordModuleInfo::type CSwordLexiconModuleInfo::getType(){
	return CSwordModuleInfo::Lexicon;
}
