/********* Read the file LICENSE for license details. *********/

//BibleTime includes
#include "cswordmoduleinfo.h"
#include "cswordbackend.h"
#include "cswordmodulesearch.h"
#include "cswordkey.h"
#include "centrydisplay.h"
#include "clanguagemgr.h"

#include "util/scoped_resource.h"
#include "frontend/cbtconfig.h"

#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <dirent.h>
#include <regex.h>

//Qt includes
#include <qregexp.h>

//KDE includes
#include <klocale.h>

//Sword includes
#include <swbuf.h>
#include <swkey.h>
#include <listkey.h>
#include <versekey.h>
#include <swconfig.h>
#include <rtfhtml.h>

using std::string;

CSwordModuleInfo::CSwordModuleInfo( sword::SWModule* module, CSwordBackend* const usedBackend) {
	m_module = module;
	m_searchResult.ClearList();
  m_backend = usedBackend;
	m_dataCache.name = module ? QString(module->Name()) : QString::null;
	m_dataCache.isUnicode = module ? module->isUnicode() : false;
	m_dataCache.category = UnknownCategory;
	m_dataCache.language = 0;
	m_dataCache.hasVersion = !QString( (*m_backend->getConfig())[name().latin1()]["Version"] ).isEmpty();

	if (backend()) {
		if (hasVersion() && (minimumSwordVersion() > sword::SWVersion::currentVersion)) {
		 	qWarning("The module \"%s\" requires a newer Sword library. Please update to \"Sword %s\".", name().latin1(), (const char*)minimumSwordVersion());
		}
	}
}

CSwordModuleInfo::CSwordModuleInfo( const CSwordModuleInfo& m ) {
	m_module = m.m_module;
	m_backend = m.m_backend;
	m_dataCache = m.m_dataCache;
	m_searchResult = m.m_searchResult;
}

/** No descriptions */
CSwordModuleInfo* CSwordModuleInfo::clone(){
	return new CSwordModuleInfo(*this);
}

CSwordModuleInfo::~CSwordModuleInfo(){
	m_searchResult.ClearList();
	m_module = 0; //the Sword module object is deleted by the backend
}

/** Sets the unlock key of the modules and writes the key into the cofig file.*/
const bool CSwordModuleInfo::unlock( const QString& unlockKey ){
  if (!isEncrypted()) {
    return false;
  }

  CBTConfig::setModuleEncryptionKey( name(), unlockKey );
  backend()->setCipherKey(m_module->Name(), unlockKey.latin1());
      
  return true;
}

/** This function returns true if this module is locked, otherwise return false. */
const bool CSwordModuleInfo::isLocked() {
  //still works, but the cipherkey is stored in CBTConfig.
	//Works because it is set in sword on program startup.
	if (isEncrypted() && config(CipherKey).isEmpty()) {
		return true;
	}
	return false;
}

/** This functions returns true if this module is encrypted (locked or unlocked). */
const bool CSwordModuleInfo::isEncrypted()/* const*/ {
	/**
	* If we have the CipherKey entry the module
	* is encrypted but not necessarily locked
	*/
	//This code is still right, though we do no longer write to the module config files any more
	sword::ConfigEntMap config	= backend()->getConfig()->Sections.find( name().latin1() )->second;
	sword::ConfigEntMap::iterator it = config.find("CipherKey");
	if (it != config.end()) {
		return true;
	}
	
	return false;
}

/** Returns true if something was found, otherwise return false. */
const bool CSwordModuleInfo::search( const QString searchedText, const int searchOptions, sword::ListKey scope, void (*percentUpdate)(char, void*) ) {
	int searchType = 0;
 	int searchFlags = REG_ICASE;

	//work around Swords thread insafety for Bibles and Commentaries
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(this) );
	sword::SWKey* s = dynamic_cast<sword::SWKey*>(key.get());
	if (s) {
		m_module->SetKey(*s);
  }

	//setup variables required for Sword
	if (searchOptions & CSwordModuleSearch::caseSensitive) {
		searchFlags = 0;
	}

	if (searchOptions & CSwordModuleSearch::cluceneIndex) {
		searchType = -4; //clucene search
  }
	else if (searchOptions & CSwordModuleSearch::entryAttribs) {
		searchType = -3; //Entry attributes
  }
	else if (searchOptions & CSwordModuleSearch::multipleWords) {
		searchType = -2; //multiple words
		
		if (m_module->hasSearchFramework() && m_module->isSearchOptimallySupported((const char*)searchedText.utf8(), -4, 0, 0)) {
			searchType = -4;
		}
  }
	else if (searchOptions & CSwordModuleSearch::exactPhrase) {
		searchType = -1; //exact phrase
  }
	else if (searchOptions & CSwordModuleSearch::regExp) {
		searchType = 0;	//regexp matching
  }

	if ((searchOptions & CSwordModuleSearch::useLastResult) && m_searchResult.Count()) {
		util::scoped_ptr<sword::SWKey> searchScope( m_searchResult.clone() );
		m_searchResult = m_module->search(searchedText.utf8(), searchType, searchFlags, searchScope, 0, percentUpdate);
	}
	else if (searchOptions & CSwordModuleSearch::useScope) {
		m_searchResult = m_module->search(searchedText.utf8(), searchType, searchFlags, (type() != Lexicon && type() != GenericBook) ? &scope : 0, 0, percentUpdate);
	}
  else {
  	m_searchResult = m_module->search(searchedText.utf8(), searchType, searchFlags, 0, 0, percentUpdate);
  }

	return (m_searchResult.Count() > 0);
}

/** Returns the last search result for this module. */
sword::ListKey& CSwordModuleInfo::searchResult(const sword::ListKey* newResult) {
	if (newResult) {
		m_searchResult.copyFrom( *newResult );
  }
	return m_searchResult;
}

/** Clears the last search result. */
void CSwordModuleInfo::clearSearchResult(){
	m_searchResult.ClearList();
}

/** This interupts the search if this module is being searched. */
void CSwordModuleInfo::interruptSearch(){
	m_module->terminateSearch = true;
}

/** Returns the required Sword version for this module. Returns -1 if no special Sword version is required. */
const sword::SWVersion CSwordModuleInfo::minimumSwordVersion(){
	return sword::SWVersion( config(CSwordModuleInfo::MinimumSwordVersion).latin1() );
}

const QString CSwordModuleInfo::config( const CSwordModuleInfo::ConfigEntry entry) {
	switch (entry) {
		case AboutInformation:
		{
			sword::SWBuf buf( m_module->getConfigEntry("About") );
			if (buf.length()) {	
				sword::RTFHTML filter;
				filter.processText(buf, 0, 0);
			}
			
			QString about;
			if (isUnicode())
				about = QString::fromUtf8(buf.c_str());
			else
				about.setLatin1(buf.c_str());
				
			return about;
		}
		case CipherKey: {
      if (CBTConfig::getModuleEncryptionKey(name()).isNull()) { //fall back!
  			return QString( m_module->getConfigEntry("CipherKey") );
      }
      else {
        return CBTConfig::getModuleEncryptionKey(name());
      };
		}
		case AbsoluteDataPath: {
			QString path = QString(m_module->getConfigEntry("AbsoluteDataPath"));
      path.replace(QRegExp("/./"), "/"); // make /abs/path/./modules/ looking better
			//make sure we have a trailing slash!
			if (path.right(1) != "/") {
				path.append('/');
			}
      return path;
    }
		case DataPath: { //make sure we remove the dataFile part if it's a Lexicon
			QString path(m_module->getConfigEntry("DataPath"));
			if ((type() == CSwordModuleInfo::GenericBook) || (type() == CSwordModuleInfo::Lexicon)) {
				int pos = path.findRev("/"); //last slash in the string
				if (pos != -1) {
					path = path.left(pos+1); //include the slash
				}
			}
			return path;
		}
		case Description:
			return QString(m_module->Description());
		case ModuleVersion: {
			QString version(m_module->getConfigEntry("Version"));
      if (version.isEmpty()) {
        version = "1.0";
			}
      return version;
    }        
		case MinimumSwordVersion: {
			const QString minimumVersion(m_module->getConfigEntry("MinimumVersion"));
			return !minimumVersion.isEmpty() ? minimumVersion : QString("0.0");
		}
		case TextDir: {
			const QString dir(m_module->getConfigEntry("Direction"));
			return !dir.isEmpty() ? dir : QString("LtoR");
		}		
    case DisplayLevel: {
			const QString level(m_module->getConfigEntry("DisplayLevel"));
			return !level.isEmpty() ? level : QString("1");
		}
    case GlossaryFrom: {
      if (!category() == Glossary) {
        return QString::null;
      };
      const QString lang(m_module->getConfigEntry("GlossaryFrom"));
			return !lang.isEmpty() ? lang : QString::null;
    }
    case GlossaryTo: {
      if (!category() == Glossary) {
        return QString::null;
      };
      const QString lang(m_module->getConfigEntry("GlossaryTo"));
			return !lang.isEmpty() ? lang : QString::null;
    }
		case Markup: {
			const QString markup(m_module->getConfigEntry("SourceType"));
			return !markup.isEmpty() ? markup : QString("Unknown");
		}		

		default:
			return QString::null;
	}
}

/** Returns true if the module supports the feature given as parameter. */
const bool CSwordModuleInfo::has( const CSwordModuleInfo::Feature feature ){
	switch (feature) {
		case StrongsNumbers:
			return m_module->getConfig().has("Feature", "StrongsNumber");
		case GreekDef:
			return m_module->getConfig().has("Feature", "GreekDef");		
		case HebrewDef:
			return m_module->getConfig().has("Feature", "HebrewDef");		
		case GreekParse:
			return m_module->getConfig().has("Feature", "GreekParse");		
		case HebrewParse:
			return m_module->getConfig().has("Feature", "HebrewParse");		
	}
	return false;
}

const bool CSwordModuleInfo::has( const CSwordModuleInfo::FilterTypes option ){
	//BAD workaround to see if the filter is GBF, OSIS or ThML!	
  const QString name = backend()->configOptionName(option);
 	if (m_module->getConfig().has("GlobalOptionFilter", QString("OSIS").append(name).latin1()))
 		return true;
  if (m_module->getConfig().has("GlobalOptionFilter", QString("GBF").append(name).latin1()))
 		return true;
 	if (m_module->getConfig().has("GlobalOptionFilter", QString("ThML").append(name).latin1()))
 		return true;
 	if (m_module->getConfig().has("GlobalOptionFilter", QString("UTF8").append(name).latin1()))
 		return true; 		
 	if (m_module->getConfig().has("GlobalOptionFilter", name.latin1()))
 		return true;
 	
 	return false;
}

/** Returns the text direction of the module's text., */
const CSwordModuleInfo::TextDirection CSwordModuleInfo::textDirection(){
  if (config(TextDir) == "RtoL") {
    return CSwordModuleInfo::RightToLeft;
	}
  else {
    return CSwordModuleInfo::LeftToRight;
	}
}

/** Writes the new text at the given position into the module. This does only work for writable modules. */
void CSwordModuleInfo::write( CSwordKey* key, const QString& newText ){
  module()->KeyText( (const char*)key->key().utf8() );
	
	//don't store a pointer to the const char* value somewhere because QCString doesn't keep the value of it
	module()->setEntry( isUnicode() ? (const char*)newText.utf8() : (const char*)newText.local8Bit() );
}

/** Deletes the current entry and removes it from the module. */
const bool CSwordModuleInfo::deleteEntry( CSwordKey* const key ){
  module()->KeyText( isUnicode() ? (const char*)key->key().utf8() : (const char*)key->key().local8Bit() );
  if (module()) {
    module()->deleteEntry();
    return true;
  };
	
  return false;
}

/** Returns the category of this module. See CSwordModuleInfo::Category for possible values. */
const CSwordModuleInfo::Category CSwordModuleInfo::category() const {
  if (m_dataCache.category == CSwordModuleInfo::UnknownCategory) {
		const QString cat(m_module->getConfigEntry("Category"));
		
		if (cat == "Cults / Unorthodox / Questionable Material") {
			m_dataCache.category = Cult;
		}
		else if (cat == "Daily Devotional" || m_module->getConfig().has("Feature", "DailyDevotion")) {
			m_dataCache.category = DailyDevotional;
		}
		else if (cat == "Glossaries" || m_module->getConfig().has("Feature", "Glossary")) { //alow both
			m_dataCache.category = Glossary;
		};
	}

	return m_dataCache.category;
}

/** Returns the display object for this module. */
Rendering::CEntryDisplay* const CSwordModuleInfo::getDisplay() const {
	return dynamic_cast<Rendering::CEntryDisplay*>(m_module->Disp());
}

QString CSwordModuleInfo::aboutText(){

	QString text;	
	
	text += ("<font size=\"-1\"><table>");
	
	if ( hasVersion() )
    text += QString( "<tr><td><b>%1</b></td><td>%2</td><tr>" )
    	.arg( i18n("Version") )
    	.arg( config( CSwordModuleInfo::ModuleVersion ));

	if ( !QString( m_module->getConfigEntry("SourceType") ).isEmpty() )
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg( i18n("Markup") )
			.arg( m_module->getConfigEntry("SourceType") );

	text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
		.arg( i18n("Location") )
		.arg( config(CSwordModuleInfo::AbsoluteDataPath) );
		
	text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
		.arg( i18n("Language") )
		.arg( language()->translatedName() );

	if ( m_module->getConfigEntry("Category") )
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg( i18n("Category") )
			.arg( m_module->getConfigEntry("Category") );
	
	if ( m_module->getConfigEntry("LCSH") )
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg( i18n("LCSH") )
			.arg( m_module->getConfigEntry("LCSH") );

	if ( isWritable() )
		text += QString("<tr><td><b>%1</b></td><td>%2</td></tr>")
			.arg( i18n("Writable") )
			.arg( i18n("yes") );

	if ( isEncrypted() )
		text += QString("<tr><td><b>%1</b></td><td>%2</td></tr>")
			.arg( i18n("Unlock key") )
			.arg( config(CSwordModuleInfo::CipherKey) );	

	QString options;
	unsigned int opts;
	for (opts = CSwordModuleInfo::filterTypesMIN; opts <= CSwordModuleInfo::filterTypesMAX; ++opts){
		if (has( static_cast<CSwordModuleInfo::FilterTypes>(opts) )){
  		if (!options.isEmpty())
  			options += QString::fromLatin1(", ");
  		options += CSwordBackend::translatedOptionName( static_cast<CSwordModuleInfo::FilterTypes>(opts) );
		}
	}
	if (!options.isEmpty())
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg(i18n( "Features") )
			.arg( options );
	
	text += ("</table><hr>");

  if ( category() == CSwordModuleInfo::Cult ) //clearly say the module contains cult/questionable materials
    text += QString( "<BR><B>%1</B><BR><BR>" )
              .arg( i18n("Take care, this work contains cult / questionable material!") );

	text += QString( "<b>%1:</b><br> <font size=\"-2\">%2</font>" )
						.arg( i18n("About") )
						.arg( config(CSwordModuleInfo::AboutInformation) );
						
	text += ("<hr> <table>");
	
	if ( !QString( m_module->getConfigEntry("DistributionLicense") ).isEmpty() )
		text += QString( "<tr><td><b>%1</b></td><td><b>%2</b></td></tr>" )
			.arg( i18n("Distribution license") )
			.arg( m_module->getConfigEntry("DistributionLicense") );
	
	if ( !QString( m_module->getConfigEntry("DistributionSource") ).isEmpty() )
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg( i18n("Distribution source") )
			.arg( m_module->getConfigEntry("DistributionSource") );

	if ( !QString( m_module->getConfigEntry("TextSource") ).isEmpty() )
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg( i18n("Text source") )
			.arg( m_module->getConfigEntry("TextSource") );
	
	if ( !QString( m_module->getConfigEntry("DistributionNotes") ).isEmpty() )
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg( i18n("Distribution notes") )
			.arg( m_module->getConfigEntry("DistributionNotes") );
	
	if ( !QString( m_module->getConfigEntry("CopyrightNotes") ).isEmpty() ){
		sword::SWBuf RTF_Buffer = SWBuf(m_module->getConfigEntry("CopyrightNotes"));
		sword::RTFHTML RTF_Filter;
		RTF_Filter.processText(RTF_Buffer, 0, 0);
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg( i18n("Copyright notes") )
			.arg( RTF_Buffer.c_str() );
	}

	if ( !QString( m_module->getConfigEntry("CopyrightHolder") ).isEmpty() )
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg( i18n("Copyright holder") )
			.arg( m_module->getConfigEntry("CopyrightHolder") );

	if ( !QString( m_module->getConfigEntry("CopyrightDate") ).isEmpty() )
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg( i18n("Copyright date") )
			.arg( m_module->getConfigEntry("CopyrightDate") );

	if ( !QString( m_module->getConfigEntry("CopyrightContactName") ).isEmpty() )
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg( i18n("Copyright contact name") )
			.arg( m_module->getConfigEntry("CopyrightContactName") );
	
	if ( !QString( m_module->getConfigEntry("CopyrightContactAddress") ).isEmpty() ){
		sword::SWBuf RTF_Buffer = SWBuf(m_module->getConfigEntry("CopyrightContactAddress"));
		sword::RTFHTML RTF_Filter;
		RTF_Filter.processText(RTF_Buffer, 0, 0);
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg( i18n("Copyright contact address") )
			.arg( RTF_Buffer.c_str() );
	}
	
	if ( !QString( m_module->getConfigEntry("CopyrightContactEmail") ).isEmpty() )
		text += QString( "<tr><td><b>%1</b></td><td>%2</td></tr>" )
			.arg( i18n("Copyright contact email") )
			.arg( m_module->getConfigEntry("CopyrightContactEmail") );

	text += "</table></font>";
	
  return text;
}
