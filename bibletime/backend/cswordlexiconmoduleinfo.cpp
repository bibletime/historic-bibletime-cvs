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


CSwordLexiconModuleInfo::CSwordLexiconModuleInfo( SWModule* module) : CSwordModuleInfo(module) {
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
//	if (m_entryList)
		delete m_entryList;
}

/** Returns the entries of the module. */
QStringList* const CSwordLexiconModuleInfo::entries(){
	if (!module())
		return 0;

	if (!m_entryList) {
		m_entryList = new QStringList();

    const bool lexiconCache = CBTConfig::get(CBTConfig::lexiconCache);
		bool read = false;

		if (lexiconCache){
  		QFile f1(
  			QString::fromLatin1("%1/%2")
  				.arg(KGlobal::dirs()->saveLocation("data", "bibletime/cache/"))
  				.arg( name() )
  		);
  		
      if ( f1.open( IO_ReadOnly ) ){
        QDataStream s( &f1 );
  			QString v;
        s >> v;
  			if (v == config(ModuleVersion) ) {
  				s >> *m_entryList;
  				read = true;
  			}
  			f1.close();
      }
		}


		if (!read){
			(*module()) = TOP;  		
  		do {
//   			m_entryList->append(QString::fromUtf8(module()->KeyText()));
   			m_entryList->append(QString::fromLocal8Bit(module()->KeyText())); //UTF8, Latin1 or Local8Bit??
  			(*module())++;
  		} while (!module()->Error());

  		if (m_entryList->first().stripWhiteSpace().isEmpty())
	  		m_entryList->remove( m_entryList->begin() );			

			if (lexiconCache){
  			// create cache
		 		QString dir = KGlobal::dirs()->saveLocation("data", "bibletime/cache/");
        QFile f2( QString::fromLatin1("%1/%2").arg(dir).arg( name() ) );
        if (f2.open( IO_WriteOnly )){
          QDataStream s( &f2 );
  				s << config(CSwordModuleInfo::ModuleVersion);
  				s << *m_entryList;
  			  f2.close();
        }
			}
			(*module()) = TOP;			
		}
	}	
	return m_entryList;
}

/** Jumps to the closest entry in the module.  */
const bool CSwordLexiconModuleInfo::snap(){
	bool ret = false;
	if(module()->getRawEntry()){	//snap to the current entry
		ret = true;
	}
	return ret;
}

/** No descriptions */
CSwordModuleInfo* CSwordLexiconModuleInfo::clone(){
	return new CSwordLexiconModuleInfo(*this);
}
