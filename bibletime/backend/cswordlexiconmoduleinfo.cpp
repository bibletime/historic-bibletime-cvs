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
#include "../frontend/cbtconfig.h"

//Qt includes
#include <qfile.h>
#include <qdatastream.h>

//Sword includes
#include <swmodule.h>

#include <kglobal.h>
#include <kstddirs.h>


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
		if (!module())
			return 0;
		m_entryList = new QStringList();
		module()->KeyText(" ");

    bool lexiconCache = CBTConfig::get(CBTConfig::lexiconCache);
		bool read = false;

		if (lexiconCache){
  		QFile f1(
  			QString::fromLatin1("%1/%2")
  				.arg(KGlobal::dirs()->saveLocation("data", "bibletime/cache/"))
  				.arg( name() )
  		);
  		
      if ( f1.open( IO_ReadOnly ) ){
        QDataStream s( &f1 );
  			QString version;
        s >> version;
  			if (version == ( (getVersion() == QString::null) ? QString("0") : getVersion())) {
  				qDebug("chache used");
  				s >> *m_entryList;
  				read = true;
  			}
  			f1.close();
      }
		}


		if (!read){
  		do {
#warning check!!
//  			if (encoding() == QFont::Unicode)
    			m_entryList->append(QString::fromUtf8(module()->KeyText()));
//        else
//    			m_entryList->append(QString::fromLocal8Bit(module()->KeyText()));
  			(*module())++;
  		} while (!module()->Error());
  		if (m_entryList->first().stripWhiteSpace().isEmpty())
	  		m_entryList->remove( m_entryList->begin() );			

			if (lexiconCache){
  			// create cache
		 		QString dir = KGlobal::dirs()->saveLocation("data", "bibletime/cache/");
        QFile f2( QString("%1/%2").arg(dir).arg( name() ) );
        if (f2.open( IO_WriteOnly )){
          QDataStream s( &f2 );
  				qDebug("cache created");
  				s << (getVersion()==QString::null?QString("0"):getVersion());
  				s << *m_entryList;
  			  f2.close();
        }
			}
		}

		module()->KeyText(" ");
	}	
	return m_entryList;
}
