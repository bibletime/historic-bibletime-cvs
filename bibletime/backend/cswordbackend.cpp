/***************************************************************************
                          cswordbackend.cpp  -  description
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
#include "cswordbackend.h"
#include "centrydisplay.h"
#include "cswordbiblemoduleinfo.h"
#include "cswordcommentarymoduleinfo.h"
#include "cswordlexiconmoduleinfo.h"
#include "cswordbookmoduleinfo.h"
#include "frontend/cbtconfig.h"

#include "bt_thmlhtml.h"
#include "bt_osishtml.h"
#include "bt_gbfhtml.h"

#include <dirent.h>
#include <unistd.h>
#include <ctype.h>

//Qt includes
#include <qdir.h>
#include <qfileinfo.h>

//KDE includes
#include <klocale.h>
#include <kstringhandler.h>

//Sword includes
#include <swdisp.h>
#include <swfiltermgr.h>
#include <encfiltmgr.h>
#include <plainhtml.h>
#include <rawgbf.h>
#include <rtfhtml.h>
#include <filemgr.h>
#include <utilstr.h>

using std::string;

char* BTStringMgr::upperUTF8(char* text, const unsigned int maxlen) {
	const int max = (maxlen>0) ? maxlen : strlen(text);
	
	if (isUtf8(text)) {
		strncpy(text, (const char*)QString::fromUtf8(text).upper().utf8(), max);
	
		return text;
	}
	else {
// 		strncpy(text, (const char*)QString::fromLatin1(text).upper().latin1(), max);
		char* ret = text;	
		while (*text) {
			*text = toupper(*text);
			text++;
		}
		
		return ret;
	}
	
		
	return text;
}

char* BTStringMgr::upperLatin1(char* text) {
// 	strncpy(text, (const char*)QString::fromLatin1(text).upper().latin1(), strlen(text));

	char* ret = text;	
	while (*text) {
		*text++ = toupper(*text);
	}
	
	return ret;
}

const bool BTStringMgr::supportsUnicode() const {
	return true;
}

const bool BTStringMgr::isUtf8(const char *buf) {
  int i, n;
  register unsigned char c;
  bool gotone = false;

#define F 0   /* character never appears in text */
#define T 1   /* character appears in plain ASCII text */
#define I 2   /* character appears in ISO-8859 text */
#define X 3   /* character appears in non-ISO extended ASCII (Mac, IBM PC) */

  static const unsigned char text_chars[256] = {
        /*                  BEL BS HT LF    FF CR    */
        F, F, F, F, F, F, F, T, T, T, T, F, T, T, F, F,  /* 0x0X */
        /*                              ESC          */
        F, F, F, F, F, F, F, F, F, F, F, T, F, F, F, F,  /* 0x1X */
        T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,  /* 0x2X */
        T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,  /* 0x3X */
        T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,  /* 0x4X */
        T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,  /* 0x5X */
        T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, T,  /* 0x6X */
        T, T, T, T, T, T, T, T, T, T, T, T, T, T, T, F,  /* 0x7X */
        /*            NEL                            */
        X, X, X, X, X, T, X, X, X, X, X, X, X, X, X, X,  /* 0x8X */
        X, X, X, X, X, X, X, X, X, X, X, X, X, X, X, X,  /* 0x9X */
        I, I, I, I, I, I, I, I, I, I, I, I, I, I, I, I,  /* 0xaX */
        I, I, I, I, I, I, I, I, I, I, I, I, I, I, I, I,  /* 0xbX */
        I, I, I, I, I, I, I, I, I, I, I, I, I, I, I, I,  /* 0xcX */
        I, I, I, I, I, I, I, I, I, I, I, I, I, I, I, I,  /* 0xdX */
        I, I, I, I, I, I, I, I, I, I, I, I, I, I, I, I,  /* 0xeX */
        I, I, I, I, I, I, I, I, I, I, I, I, I, I, I, I   /* 0xfX */
  };

  /* *ulen = 0; */
  for (i = 0; (c = buf[i]); i++) {
    if ((c & 0x80) == 0) {        /* 0xxxxxxx is plain ASCII */
      /*
       * Even if the whole file is valid UTF-8 sequences,
       * still reject it if it uses weird control characters.
       */

      if (text_chars[c] != T)
        return false;

    } else if ((c & 0x40) == 0) { /* 10xxxxxx never 1st byte */
      return false;
    } else {                           /* 11xxxxxx begins UTF-8 */
      int following;

    if ((c & 0x20) == 0) {             /* 110xxxxx */
      following = 1;
    } else if ((c & 0x10) == 0) {      /* 1110xxxx */
      following = 2;
    } else if ((c & 0x08) == 0) {      /* 11110xxx */
      following = 3;
    } else if ((c & 0x04) == 0) {      /* 111110xx */
      following = 4;
    } else if ((c & 0x02) == 0) {      /* 1111110x */
      following = 5;
    } else
      return false;

      for (n = 0; n < following; n++) {
        i++;
        if (!(c = buf[i]))
          goto done;

        if ((c & 0x80) == 0 || (c & 0x40))
          return false;
      }
      gotone = true;
    }
  }
done:
  return gotone;   /* don't claim it's UTF-8 if it's all 7-bit */
}

#undef F
#undef T
#undef I
#undef X


//static class-wide members
static QMap<QString, QString> moduleDescriptionMap;

CSwordBackend::CSwordBackend()
	: sword::SWMgr(0, 0, false, new sword::EncodingFilterMgr( sword::ENC_UTF8 ))
{	
	m_displays.entry = 0;
	m_displays.chapter = 0;
	m_displays.book = 0;

	m_filters.gbf = 0;
	m_filters.thml = 0;
	m_filters.osis = 0;
	m_filters.plain = 0;
}

CSwordBackend::CSwordBackend(const QString& path)
	: sword::SWMgr((!path.isEmpty() ? (const char*)path.local8Bit() : 0), false, new sword::EncodingFilterMgr( sword::ENC_UTF8 ))
{	
	m_displays.entry = 0;
	m_displays.chapter = 0;
	m_displays.book = 0;

	m_filters.gbf = 0;
	m_filters.thml = 0;
	m_filters.osis = 0;
	m_filters.plain = 0;
}

CSwordBackend::~CSwordBackend(){
	shutdownModules();
	delete m_filters.gbf;
	delete m_filters.plain;
	delete m_filters.thml;
	delete m_filters.osis;

	delete m_displays.book;
	delete m_displays.chapter;
	delete m_displays.entry;
}

/** Initializes the Sword modules. */
const CSwordBackend::LoadError CSwordBackend::initModules() {
//  qWarning("globalSwordConfigPath is %s", globalConfPath);
	LoadError ret = NoError;

 	shutdownModules(); //remove previous modules
 	m_moduleList.clear();	

	sword::ModMap::iterator end = Modules.end();
	ret = LoadError( Load() );
	for (sword::ModMap::iterator it = Modules.begin(); it != end; it++) {
		sword::SWModule* const curMod = (*it).second;
		CSwordModuleInfo* newModule = 0;
				
		if (!strcmp(curMod->Type(), "Biblical Texts")) {
			newModule = new CSwordBibleModuleInfo(curMod, this);
			newModule->module()->Disp(m_displays.chapter ? m_displays.chapter : (m_displays.chapter = new CChapterDisplay));
		} else if (!strcmp(curMod->Type(), "Commentaries")) {
			newModule = new CSwordCommentaryModuleInfo(curMod, this);
			newModule->module()->Disp(m_displays.entry ? m_displays.entry : (m_displays.entry = new CEntryDisplay));
		} else if (!strcmp(curMod->Type(), "Lexicons / Dictionaries")) {
			newModule = new CSwordLexiconModuleInfo(curMod, this);
			newModule->module()->Disp(m_displays.entry ? m_displays.entry : (m_displays.entry = new CEntryDisplay));
		} else if (!strcmp(curMod->Type(), "Generic Books")) {
			newModule = new CSwordBookModuleInfo(curMod, this);
			newModule->module()->Disp(m_displays.book ? m_displays.book : (m_displays.book = new CBookDisplay));
		}
		
		if (newModule) {	//append the new modules to our list
			m_moduleList.append( newModule );
		}
	}

	for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
		moduleDescriptionMap.insert(m_moduleList.current()->config(CSwordModuleInfo::Description), m_moduleList.current()->name());
	}

	//unlock modules if keys are present
	for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
		if ( m_moduleList.current()->isEncrypted() ){
			const QString unlockKey = CBTConfig::getModuleEncryptionKey(m_moduleList.current()->name()).latin1();
			if (!unlockKey.isNull()){
  			setCipherKey( m_moduleList.current()->name().latin1(), unlockKey.latin1() );
			}
		}
	}

	return ret;
}

void CSwordBackend::AddRenderFilters(sword::SWModule *module, sword::ConfigEntMap &section) {
	sword::SWBuf sourceformat;
	sword::SWBuf moduleDriver;
	sword::ConfigEntMap::iterator entry;
	bool noDriver = true;

	sourceformat = ((entry = section.find("SourceType")) != section.end()) ? (*entry).second : (sword::SWBuf) "";
	moduleDriver = ((entry = section.find("ModDrv")) != section.end()) ? (*entry).second : (sword::SWBuf) "";

	if (sourceformat == "GBF") {
		if (!m_filters.gbf) {
			m_filters.gbf = new BT_GBFHTML();
		}
		module->AddRenderFilter(m_filters.gbf);
		noDriver = false;
	}

	if (sourceformat == "PLAIN") {
		if (!m_filters.plain) {
			m_filters.plain = new sword::PLAINHTML();	
		}
		module->AddRenderFilter(m_filters.plain);
		noDriver = false;
	}

	if (sourceformat == "ThML") {
		if (!m_filters.thml) {
			m_filters.thml = new BT_ThMLHTML();
		}
		module->AddRenderFilter(m_filters.thml);
		noDriver = false;
	}

	if (sourceformat == "OSIS") {
		if (!m_filters.osis) {
			m_filters.osis = new BT_OSISHTML();
		}
		module->AddRenderFilter(m_filters.osis);
		noDriver = false;
	}

	if (noDriver){ //no driver found
		if ( (moduleDriver == "RawCom") || (moduleDriver == "RawLD") ) {
			if (!m_filters.plain) {
				m_filters.plain = new sword::PLAINHTML();
			}
			module->AddRenderFilter(m_filters.plain);
			noDriver = false;
		}
	}
}

/** This function deinitializes the modules and deletes them. */
const bool CSwordBackend::shutdownModules(){
	for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
		CSwordModuleInfo* current = m_moduleList.current();
    m_moduleList.removeRef(current);
		
		delete current;
	}
	m_moduleList.clear();

  //BT  mods are deleted now, delete Sword mods, too.
  DeleteMods();
  
	return true;
}

/** Returns true if the given option is enabled. */
const bool CSwordBackend::isOptionEnabled( const CSwordBackend::FilterTypes type) {
	return (getGlobalOption( optionName(type).latin1() ) == "On");
}

/** Sets the given options enabled or disabled depending on the second parameter. */
void CSwordBackend::setOption( const CSwordBackend::FilterTypes type, const int state ){
  sword::SWBuf value;
	switch (type) {
		case textualVariants:
      if (state == 0) {
        value = "Primary Reading";
			}
      else if (state == 1) {
        value = "Secondary Reading";
			}
      else {
        value = "All Readings";
			}
			break;

// 		case transliteration:
//      if (useICU()) {
//         sword::StringList options = transliterator()->getOptionValues();
//         
// 				sword::StringList::iterator it = options.begin();
// 				sword::StringList::iterator end = options.end();
//         for (int index = state; (index > 0) && (it != end); ++it) {
//           --index;
//         }
//         value = it->c_str();
// 				//qWarning("setting transliteration to %s", value.c_str());
//       }
//       break;

		default:
			value = state ? "On": "Off";
			break;
	};
	if (value.length())
    setGlobalOption(optionName(type).latin1(), value.c_str());
}

void CSwordBackend::setFilterOptions( const CSwordBackend::FilterOptions options){
  setOption( footnotes, 					options.footnotes );
  setOption( strongNumbers, 			options.strongNumbers );
  setOption( headings, 						options.headings );
  setOption( morphTags, 					options.morphTags );
	setOption( lemmas, 							options.lemmas );
	setOption( hebrewPoints, 				options.hebrewPoints );
	setOption( hebrewCantillation, 	options.hebrewCantillation );
	setOption( greekAccents, 				options.greekAccents );
	setOption( redLetterWords,			options.redLetterWords );	
  setOption( textualVariants,			options.textualVariants );	
// 	setOption( transliteration,			options.transliteration );	
	setOption( scriptureReferences,	options.scriptureReferences);	
}

void CSwordBackend::setDisplayOptions( const CSwordBackend::DisplayOptions ){
/*  if (m_displays.entry) {
		m_displays.entry->setDisplayOptions(options);	
	}
  if (m_displays.chapter) {
		m_displays.chapter->setDisplayOptions(options);	
	}
  if (m_displays.book) {
		m_displays.book->setDisplayOptions(options);
	}
	*/
}

/** This function searches for a module with the specified description */
CSwordModuleInfo* const CSwordBackend::findModuleByDescription(const QString& description){
  if (m_moduleList.count()) {
    for ( m_moduleList.first();m_moduleList.current();m_moduleList.next() ) {
      if ( m_moduleList.current()->config(CSwordModuleInfo::Description) == description ) {
        return m_moduleList.current();
			}
		}
	}
  return 0;
}

/** This function searches for a module with the specified description */
const QString CSwordBackend::findModuleNameByDescription(const QString& description){
	if (moduleDescriptionMap.contains(description)) {
		return moduleDescriptionMap[description];
  }
	
  return QString::null;
}

/** This function searches for a module with the specified name */
CSwordModuleInfo* const CSwordBackend::findModuleByName(const QString& name){
  if (m_moduleList.count()) {
    for ( m_moduleList.first(); m_moduleList.current(); m_moduleList.next() ) {
      if ( m_moduleList.current()->name() == name ) {
        return m_moduleList.current();
      }
    }
  }
	
  return 0;
}

CSwordModuleInfo* const CSwordBackend::findSwordModuleByPointer(const sword::SWModule* const swmodule){
	if (swmodule) {
		for ( m_moduleList.first(); m_moduleList.current(); m_moduleList.next() ) {
			if ( m_moduleList.current()->module() == swmodule ) {
				return m_moduleList.current();
			}
		}
	}
	return 0;
}

CSwordModuleInfo* const CSwordBackend::findModuleByPointer(const CSwordModuleInfo* const module){
	if (module) {
		for ( m_moduleList.first(); m_moduleList.current(); m_moduleList.next() ) {
			if ( m_moduleList.current() == module ) {
				return m_moduleList.current();
			}
		}
	}
	return 0;
}

/** Returns our local config object to store the cipher keys etc. locally for each user. The values of the config are merged with the global config. */
const bool CSwordBackend::moduleConfig(const QString& module, sword::SWConfig& moduleConfig) {
	sword::SectionMap::iterator section;
	DIR *dir = opendir(configPath);
	struct dirent *ent;
	
	bool foundConfig = false;	
	QString modFile;	
	if (dir) {    // find and update .conf file
		rewinddir(dir);
		while ((ent = readdir(dir)) && !foundConfig) {
			if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {								
				modFile = QString::fromLocal8Bit(configPath);
				modFile += QString::fromLatin1("/");
				modFile += QString::fromLocal8Bit(ent->d_name);
				moduleConfig = sword::SWConfig( (const char*)modFile.local8Bit() );
				section =	moduleConfig.Sections.find( (const char*)module.local8Bit() );
				foundConfig = ( section != moduleConfig.Sections.end() );
			}
		}
		closedir(dir);
	}
	else { //try to read mods.conf
		//moduleConfig = SWConfig( configPath + "/mods.conf" );
		moduleConfig = sword::SWConfig("");//global config		
		section =	config->Sections.find( (const char*)module.local8Bit() );		
		foundConfig = ( section != config->Sections.end() );		
		
		sword::ConfigEntMap::iterator entry;
		if (foundConfig) { //copy module section
			for (entry = (*section).second.begin(); entry != (*section).second.end(); entry++) {
					moduleConfig.Sections[(*section).first].insert(sword::ConfigEntMap::value_type((*entry).first, (*entry).second));
			}
		}			
	}
	
	if (!foundConfig && configType != 2) { //search in $HOME/.sword/
		QString myPath = QString::fromLatin1("%1/.sword/mods.d").arg(getenv("HOME"));
		dir = opendir(myPath.latin1());
		if (dir) {
			rewinddir(dir);
			while ((ent = readdir(dir)) && !foundConfig) {
				if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {								
					modFile = myPath;
					modFile += "/";
					modFile += ent->d_name;
					moduleConfig = sword::SWConfig( (const char*)modFile.local8Bit() );
					section =	moduleConfig.Sections.find( (const char*)module.local8Bit() );
					foundConfig = ( section != moduleConfig.Sections.end() );
				}
			}
			closedir(dir);
		}
	}
	return foundConfig;
}

/** Returns the text used for the option given as parameter. */
const QString CSwordBackend::optionName( const CSwordBackend::FilterTypes option ){
	switch (option) {
		case CSwordBackend::footnotes:
			return QString::fromLatin1("Footnotes");
		case CSwordBackend::strongNumbers:
			return QString::fromLatin1("Strong's Numbers");
		case CSwordBackend::headings:
			return QString::fromLatin1("Headings");
		case CSwordBackend::morphTags:
			return QString::fromLatin1("Morphological Tags");
  	case CSwordBackend::lemmas:
			return QString::fromLatin1("Lemmas");
		case CSwordBackend::hebrewPoints:
			return QString::fromLatin1("Hebrew Vowel Points");
		case CSwordBackend::hebrewCantillation:
			return QString::fromLatin1("Hebrew Cantillation");
		case CSwordBackend::greekAccents:
			return QString::fromLatin1("Greek Accents");
		case CSwordBackend::redLetterWords:
			return QString::fromLatin1("Words of Christ in Red");
    case CSwordBackend::textualVariants:
			return QString::fromLatin1("Textual Variants");	
		case CSwordBackend::scriptureReferences:
			return QString::fromLatin1("Cross-references");
		case CSwordBackend::transliteration:
			return QString::fromLatin1("Transliteration");
	}
	return QString::null;	
}

/** Returns the translated name of the option given as parameter. */
const QString CSwordBackend::translatedOptionName(const CSwordBackend::FilterTypes option){
	switch (option) {
		case CSwordBackend::footnotes:
			return i18n("Footnotes");
		case CSwordBackend::strongNumbers:
			return i18n("Strong's Numbers");
		case CSwordBackend::headings:
			return i18n("Headings");
		case CSwordBackend::morphTags:
			return i18n("Morphological Tags");
  	case CSwordBackend::lemmas:
			return i18n("Lemmas");
		case CSwordBackend::hebrewPoints:
			return i18n("Hebrew Vowel Points");
		case CSwordBackend::hebrewCantillation:
			return i18n("Hebrew Cantillation");
		case CSwordBackend::greekAccents:
			return i18n("Greek Accents");
		case CSwordBackend::redLetterWords:
			return i18n("Red letter words");
		case CSwordBackend::textualVariants:
			return i18n("Textual Variants");
		case CSwordBackend::scriptureReferences:
			return i18n("Scripture Cross-references");	
		case CSwordBackend::transliteration:
			return i18n("Transliteration between scripts");	
	}
	return QString::null;
}


const QString CSwordBackend::configOptionName( const CSwordBackend::FilterTypes option ){
	switch (option) {
		case CSwordBackend::footnotes:
			return QString::fromLatin1("Footnotes");         
		case CSwordBackend::strongNumbers:
			return QString::fromLatin1("Strongs");
		case CSwordBackend::headings:
			return QString::fromLatin1("Headings");
		case CSwordBackend::morphTags:
			return QString::fromLatin1("Morph");
  	case CSwordBackend::lemmas:
			return QString::fromLatin1("Lemma");
		case CSwordBackend::hebrewPoints:
			return QString::fromLatin1("HebrewPoints");
		case CSwordBackend::hebrewCantillation:
			return QString::fromLatin1("Cantillation");
		case CSwordBackend::greekAccents:
			return QString::fromLatin1("GreekAccents");
		case CSwordBackend::redLetterWords:
			return QString::fromLatin1("RedLetterWords");
		case CSwordBackend::textualVariants:
			return QString::fromLatin1("Variants");
		case CSwordBackend::scriptureReferences:
			return QString::fromLatin1("Scripref");
    default:
      return QString::null;
	}
	return QString::null;	
}

const QString CSwordBackend::booknameLanguage( const QString& language ) {
	if (!language.isNull())
		sword::LocaleMgr::getSystemLocaleMgr()->setDefaultLocaleName( language.local8Bit() );
	return QString::fromLatin1(sword::LocaleMgr::getSystemLocaleMgr()->getDefaultLocaleName());
}


/** Reload all Sword modules. */
void CSwordBackend::reloadModules(){
  shutdownModules();
  
  //delete Sword's config to make Sword reload it!
  if (myconfig) { // force reload on config object because we may have changed the paths
    delete myconfig;
    config = myconfig = 0;
    loadConfigDir(configPath);
  }
  else if (config) {
    config->Load();
  }
  initModules();
}

const QStringList CSwordBackend::swordDirList(){
	QStringList ret;
	const QString home = getenv("HOME");
	
	//return a list of used Sword dirs. Useful for the installer
	QString configPath;
	if (!home.isEmpty()) {
		configPath = home + "/.sword/sword.conf";
	}
	else {
		configPath = globalConfPath; //e.g. /etc/sword.conf, /usr/local/etc/sword.conf
	}
	
	
	QStringList configs = QStringList::split(":", configPath);
	for (QStringList::const_iterator it = configs.constBegin(); it != configs.constEnd(); ++it) {
		if (!QFileInfo(*it).exists()) {
			continue;
		}
	
		//get all DataPath and AugmentPath entries from the config file and add them to the list
		sword::SWConfig conf( (*it).latin1() );
	
		ret << conf["Install"]["DataPath"].c_str();
		sword::ConfigEntMap group = conf["Install"];
		sword::ConfigEntMap::iterator start = group.equal_range("AugmentPath").first;
		sword::ConfigEntMap::iterator end = group.equal_range("AugmentPath").second;
	
		for (sword::ConfigEntMap::const_iterator it = start; it != end; ++it) {
			ret << it->second.c_str(); //added augment path
		}
	}
	
	if (!home.isEmpty()) {
		ret << home + "/.sword/";
	}
	
	return ret;
}
