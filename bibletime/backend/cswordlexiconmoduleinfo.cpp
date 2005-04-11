/********* Read the file LICENSE for license details. *********/

//BibleTime includes
#include "cswordlexiconmoduleinfo.h"
//#include "frontend/cbtconfig.h"

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

CSwordLexiconModuleInfo::CSwordLexiconModuleInfo( sword::SWModule* module, CSwordBackend* const backend ) : CSwordModuleInfo(module, backend) {
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
	if (!module()) {
		return 0;
	}

  if (!m_entryList) {
		m_entryList = new QStringList();
		bool read = false;

		QString dir = KGlobal::dirs()->saveLocation("data", "bibletime/cache/");
		QFile f1(
			QString(dir)
				.append("/")
				.append(name())
		);

		if ( f1.open( IO_ReadOnly ) ){
			QDataStream s( &f1 );
			QString mod_ver, prog_ver;
			s >> mod_ver;
			s >> prog_ver;
			if ((mod_ver == config(ModuleVersion)) && (prog_ver == CACHE_FORMAT)) {
				s >> *m_entryList;
				read = true;
			}
			f1.close();
		}

    if (!read || !m_entryList->count()){
      module()->setSkipConsecutiveLinks(true);
			(*module()) = sword::TOP;
      snap(); //snap to top entry
  		
			do {
        if (isUnicode()) {
     			m_entryList->append(QString::fromUtf8(module()->KeyText()));
				}
        else { //for latin1 modules use fromLatin1 because of speed
//           m_entryList->append(QString::fromLatin1(module()->KeyText()));
					m_entryList->append(QString(module()->KeyText()));
				}  			
				(*module())++;
  		} while ( !module()->Error() );
			
			(*module()) = sword::TOP; //back to the first entry
      module()->setSkipConsecutiveLinks(false);

      if (m_entryList->count()) {
        m_entryList->first().simplifyWhiteSpace();
    		if (m_entryList->first().stripWhiteSpace().isEmpty()) {
  	  		m_entryList->remove( m_entryList->begin() );
				}
				
// 				m_entryList->sort(); //make sure the module is sorted by utf-8
      }

			if (m_entryList->count()){
  			//create cache
		 		QString dir = KGlobal::dirs()->saveLocation("data", "bibletime/cache/");
        //QFile f2( QString::fromLatin1("%1/%2").arg(dir).arg( name() ) );
				QFile f2( QString(dir).append("/").append(name()) );
				
				
        if (f2.open( IO_WriteOnly )){
          QDataStream s( &f2 );
  				s << config(CSwordModuleInfo::ModuleVersion); //store module version
					s << QString(CACHE_FORMAT); //store BT version -- format may change
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
	if(module()->getRawEntry()){ // Snap to the current entry
		return true;
	}
	
	return false;
}

/** No descriptions */
CSwordModuleInfo* CSwordLexiconModuleInfo::clone(){
	return new CSwordLexiconModuleInfo(*this);
}
