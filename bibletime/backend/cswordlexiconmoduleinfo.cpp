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
#include "frontend/cbtconfig.h"

//Qt includes
#include <qfile.h>
#include <qdatastream.h>

//Sword includes
#include <swmodule.h>

#include <kglobal.h>
#include <kstandarddirs.h>

//Change it once the format changed to make all
//systems rebuild their caches
#define CACHE_FORMAT "2"

CSwordLexiconModuleInfo::CSwordLexiconModuleInfo( sword::SWModule* module) : CSwordModuleInfo(module) {
	m_entryList = 0;
}

CSwordLexiconModuleInfo::CSwordLexiconModuleInfo( const CSwordLexiconModuleInfo& m ) : CSwordModuleInfo(m) {
	delete m_entryList;
	m_entryList = 0;	
		
	if (m.m_entryList) {
		m_entryList = new QStringList();
		*m_entryList = *m.m_entryList;//copy list items
	}
}

CSwordLexiconModuleInfo::~CSwordLexiconModuleInfo(){
	delete m_entryList;
	m_entryList = 0;
}

/** Returns the entries of the module. */
QStringList* const CSwordLexiconModuleInfo::entries(){
//  qWarning("QStringList* const CSwordLexiconModuleInfo::entries()");
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
  			QString mod_ver, prog_ver;
        s >> mod_ver;
        s >> prog_ver;
  			if (mod_ver == config(ModuleVersion) && prog_ver == CACHE_FORMAT) {
  				s >> *m_entryList;
  				read = true;
  			}
  			f1.close();
      }
		}

    if (!read || !m_entryList->count()){
//      module()->setSkipConsecutiveLinks(true);
			(*module()) = sword::TOP;
      snap(); //snap to top entry
  		do {
        if ( isUnicode() )
     			m_entryList->append(QString::fromUtf8(module()->KeyText()));
        else //latin1 is a lot faster than UTF8, use it because latin1 modules won't contain unicode keys
          m_entryList->append(QString::fromLatin1(module()->KeyText()));
  			(*module())++;
  		} while ( !module()->Error() );
			(*module()) = sword::TOP;
//      module()->setSkipConsecutiveLinks(false);
      
      if (m_entryList->count()) {
        m_entryList->first().simplifyWhiteSpace();
    		if (m_entryList->first().stripWhiteSpace().isEmpty())
  	  		m_entryList->remove( m_entryList->begin() );			
      }

			if (lexiconCache && m_entryList->count()){
  			//create cache
		 		QString dir = KGlobal::dirs()->saveLocation("data", "bibletime/cache/");
        QFile f2( QString::fromLatin1("%1/%2").arg(dir).arg( name() ) );
        if (f2.open( IO_WriteOnly )){
          QDataStream s( &f2 );
  				s << config(CSwordModuleInfo::ModuleVersion); //store module version
					s << QString::fromLatin1(CACHE_FORMAT); //store BT version -- format may change
  				s << *m_entryList;
  			  f2.close();
        }
			}
		}
	}	
	return m_entryList;
}

/** Jumps to the closest entry in the module.  */
const bool CSwordLexiconModuleInfo::snap(){
	if(module()->getRawEntry()){	//snap to the current entry
		return true;
	}
	return false;
}

/** No descriptions */
CSwordModuleInfo* CSwordLexiconModuleInfo::clone(){
	return new CSwordLexiconModuleInfo(*this);
}
