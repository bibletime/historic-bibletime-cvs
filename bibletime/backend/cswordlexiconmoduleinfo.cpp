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
#include <qfile.h>
#include <qdatastream.h>

//Sword includes
#include <swmodule.h>


CSwordLexiconModuleInfo::CSwordLexiconModuleInfo( CSwordBackend* backend, SWModule* module) : CSwordModuleInfo(backend, module) {
	m_entryList = 0;
}

CSwordLexiconModuleInfo::CSwordLexiconModuleInfo( const CSwordLexiconModuleInfo& m ) : CSwordModuleInfo(m) {
	if (m_entryList)
		delete m_entryList;
	m_entryList = 0;	
		
	if (m.m_entryList) {
		m_entryList = new QStringList();
		*m_entryList = *m.m_entryList;//copy list items
	}
}

CSwordLexiconModuleInfo::~CSwordLexiconModuleInfo(){
	if (m_entryList)
		delete m_entryList;
}

/** Returns the entries of the module. */
QStringList* CSwordLexiconModuleInfo::getEntries(){
	if (!m_entryList) {
		m_entryList = new QStringList();		
		if (!module()) return 0;
		module()->KeyText(" ");

		QFile f1( QString("/tmp/%1").arg( name() ) );
		bool read = false;
    if ( f1.open( IO_ReadOnly ) ){
      QDataStream s( &f1 );
			QString version;
      s >> version;
			if (version == (getVersion()==QString::null?QString("0"):getVersion())){
				qDebug("chache used");
				s >> *m_entryList;
				read = true;
			}
			f1.close();
    }


		if (!read){
  		do {
  			if (encoding() == QFont::Unicode)
    			m_entryList->append(QString::fromUtf8(module()->KeyText()));
        else
    			m_entryList->append(QString::fromLocal8Bit(module()->KeyText()));
  			(*module())++;
  		} while (!module()->Error());
  		if (m_entryList->first().stripWhiteSpace().isEmpty())
	  		m_entryList->remove( m_entryList->begin() );			

			// Open the file.
      QFile f2( QString("/tmp/%1").arg( name() ) );
      if (f2.open( IO_WriteOnly )){
        QDataStream s( &f2 );
				qDebug("cache created");
				s << (getVersion()==QString::null?QString("0"):getVersion());
				s << *m_entryList;
			  f2.close();
      }
		}

		module()->KeyText(" ");
	}	
	return m_entryList;
}
