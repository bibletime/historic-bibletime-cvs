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
	qDebug("constructor of CSwordLexiconModuleInfo");
	m_entryList = 0;
}

CSwordLexiconModuleInfo::CSwordLexiconModuleInfo( const CSwordLexiconModuleInfo& m ) : CSwordModuleInfo(m) {
	qDebug("CSwordLexiconModuleInfo::CSwordLexiconModuleInfo( const CSwordLexiconModuleInfo& m ");
	if (m_entryList)
		delete m_entryList;
	m_entryList = 0;	
		
	if (m.m_entryList) {
		m_entryList = new QStringList();
		*m_entryList = *m.m_entryList;//copy entries
	}
}

CSwordLexiconModuleInfo::~CSwordLexiconModuleInfo(){
	if (m_entryList)
		delete m_entryList;
}

/** Returns the entries of the module. */
QStringList* CSwordLexiconModuleInfo::getEntries(){
	qDebug("QStringList* CSwordLexiconModuleInfo::getEntries()");
	if (!m_entryList) {
		m_entryList = new QStringList();		
		if (!module())
			return 0;
		module()->KeyText(" ");
		do {
			m_entryList->append(QString::fromLocal8Bit(module()->KeyText()));
			(*module())++;
		} while (!module()->Error());
		module()->KeyText(" ");		
		//if the first entry is empty remove it (empty entry means "About module")
		if (m_entryList->first().stripWhiteSpace().isEmpty())
			m_entryList->remove( m_entryList->begin() );			
	}	
	return m_entryList;
}
