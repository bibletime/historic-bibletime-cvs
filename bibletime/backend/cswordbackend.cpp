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
#include "chtmlentrydisplay.h"
#include "chtmlchapterdisplay.h"
#include "cswordbiblemoduleinfo.h"
#include "cswordcommentarymoduleinfo.h"
#include "cswordlexiconmoduleinfo.h"
#include "cswordbookmoduleinfo.h"

#include "bt_thmlhtml.h"
#include "bt_gbfhtml.h"
//#include "bt_rwphtml.h"

#include <dirent.h>
#include <unistd.h>

//Qt includes
#include <qdir.h>

//KDE includes
#include <klocale.h>

//Sword includes
#include <swdisp.h>
#include <swfiltermgr.h>
#include <encfiltmgr.h>
#include <plainhtml.h>
#include <rawgbf.h>
#include <rtfhtml.h>
#include <filemgr.h>
#include <utilstr.h>

//static class-wide members
static QMap<QString, QString> moduleDescriptionMap;

CSwordBackend::CSwordBackend()
	: SWMgr(0,0,false,new EncodingFilterMgr( ENC_UTF8 )),
	m_errorCode(noError),
	m_entryDisplay(0),
	m_chapterDisplay(0),
	m_moduleList(0),
	m_gbfFilter(0),
	m_plainTextFilter(0),
	m_thmlFilter(0)/*, m_rwpFilter(0)*/ {
	

}

CSwordBackend::~CSwordBackend(){
	shutdownModules();	
	//delete filters
	if (m_gbfFilter)
		delete m_gbfFilter;
	if (m_plainTextFilter)
		delete m_plainTextFilter;	
	if (m_thmlFilter)
		delete m_thmlFilter;	
//	if (m_rwpFilter)
//		delete m_rwpFilter;			
}

#define CHECK_HTML_CHAPTER_DISLPAY \
	if (!m_chapterDisplay) \
		m_chapterDisplay = new CHTMLChapterDisplay();

#define CHECK_HTML_ENTRY_DISLPAY \
	if (!m_entryDisplay) \
		m_entryDisplay = new CHTMLEntryDisplay();
		
/** Initializes the Sword modules. */
const CSwordBackend::errorCode CSwordBackend::initModules() {
	qDebug("CSwordBackend::initModules");
	ModMap::iterator it;
	SWModule*	curMod = 0;
	CSwordModuleInfo* newModule = 0;
	if (!m_moduleList)
		m_moduleList = new ListCSwordModuleInfo();
		 	
 	shutdownModules(); //remove previous modules
 	m_moduleList->clear();	

	for (it = Modules.begin(); it != Modules.end(); it++) {
		curMod = (*it).second;
		if (!strcmp(curMod->Type(), "Biblical Texts")) {
			newModule = new CSwordBibleModuleInfo(this, curMod);
			CHECK_HTML_CHAPTER_DISLPAY	//a macro to check the chapter display			
			newModule->module()->Disp(m_chapterDisplay);
		} else if (!strcmp(curMod->Type(), "Commentaries")) {
			newModule = new CSwordCommentaryModuleInfo(this, curMod);
			CHECK_HTML_ENTRY_DISLPAY	//a macro to check the entry display
			newModule->module()->Disp(m_entryDisplay);
		} else if (!strcmp(curMod->Type(), "Lexicons / Dictionaries")) {
			newModule = new CSwordLexiconModuleInfo(this, curMod);
			CHECK_HTML_ENTRY_DISLPAY	//a macro to check the entry display			
			newModule->module()->Disp(m_entryDisplay);
		} else if (!strcmp(curMod->Type(), "Generic Book")) {
			newModule = new CSwordBookModuleInfo(this, curMod);
			CHECK_HTML_ENTRY_DISLPAY	//a macro to check the entry display			
			newModule->module()->Disp(m_entryDisplay);
		}

		if (newModule)	//append the new modules to our list
			m_moduleList->append( newModule );
	}	
	if (m_moduleList->count() == 0)
		m_errorCode = noModulesAvailable;
		
//module are now available, fill the static lists
	for (m_moduleList->first(); m_moduleList->current(); m_moduleList->next()) {
		moduleDescriptionMap.insert(m_moduleList->current()->getDescription(), m_moduleList->current()->name());
	}

	return m_errorCode;
}
#undef CHECK_HTML_CHAPTER_DISPLAY
#undef CHECK_HTML_ENTRY_DISPLAY

void CSwordBackend::AddRenderFilters(SWModule *module, ConfigEntMap &section) {
	string sourceformat;
	string moduleDriver;
	ConfigEntMap::iterator entry;
	bool noDriver = true;

	sourceformat = ((entry = section.find("SourceType")) != section.end()) ? (*entry).second : (string) "";
	moduleDriver = ((entry = section.find("ModDrv")) != section.end()) ? (*entry).second : (string) "";

	qDebug(moduleDriver.c_str());
	qDebug(sourceformat.c_str());	
	if (!stricmp(sourceformat.c_str(), "GBF")) {
		if (!m_gbfFilter)
			m_gbfFilter = new BT_GBFHTML();
		module->AddRenderFilter(m_gbfFilter);
		noDriver = false;
	}

	if (!stricmp(sourceformat.c_str(), "PLAIN")) {
		if (!m_plainTextFilter)
			m_plainTextFilter = new PLAINHTML();	
		module->AddRenderFilter(m_plainTextFilter);
		noDriver = false;
	}

	if (!stricmp(sourceformat.c_str(), "ThML")) {
		if (!m_thmlFilter)
			m_thmlFilter = new BT_ThMLHTML();
		module->AddRenderFilter(m_thmlFilter);
		noDriver = false;
	}

	if (noDriver){
		if (!stricmp(moduleDriver.c_str(), "RawCom") || !stricmp(moduleDriver.c_str(), "RawLD")) {
			if (!m_plainTextFilter)
				m_plainTextFilter = new PLAINHTML();
			module->AddRenderFilter(m_plainTextFilter);
			noDriver = false;
		}
	}
}

/** This function deinitializes the modules and deletes them. */
const bool CSwordBackend::shutdownModules(){
	for (m_moduleList->first(); m_moduleList->current(); m_moduleList->next()) {
		if (m_moduleList->current()) {
			CSwordModuleInfo* current = m_moduleList->current();
			m_moduleList->removeRef(current);
			delete current;
		}		
	}
	m_moduleList->clear();	
	return true;
}

/** Returns true if the given option is enabled. */
const bool CSwordBackend::isOptionEnabled( const CSwordBackend::moduleOptions type) {
	const QString optionName = getOptionName(type);
	return (getGlobalOption(optionName.latin1()) == "On");
}

/** Sets the given options enabled or disabled depending on the second parameter. */
void CSwordBackend::setOption( const CSwordBackend::moduleOptions type, const bool enable){
	const QString optionName = getOptionName(type);
	setGlobalOption(optionName.latin1(), enable ? "On": "Off");
}

//const CSwordBackend::moduleOptionsBool CSwordBackend::getAllModuleOptions(){
//	CSwordBackend::moduleOptionsBool options;
//	options.footnotes = isOptionEnabled( CSwordBackend::footnotes );
//	options.strongNumbers = isOptionEnabled( CSwordBackend::strongNumbers );
//	options.headings = isOptionEnabled( CSwordBackend::headings );
//	options.morphTags = isOptionEnabled( CSwordBackend::morphTags );
//	options.lemmas = isOptionEnabled( CSwordBackend::lemmas );
//	options.hebrewPoints = isOptionEnabled( CSwordBackend::hebrewPoints );
//	options.hebrewCantillation = isOptionEnabled( CSwordBackend::hebrewCantillation );
//	options.greekAccents = isOptionEnabled( CSwordBackend::greekAccents );
//							
//	return options;
//}

void CSwordBackend::setAllModuleOptions( const CSwordBackend::moduleOptionsBool options){
  setOption( footnotes, 					options.footnotes );
  setOption( strongNumbers, 			options.strongNumbers );
  setOption( headings, 						options.headings );
  setOption( morphTags, 					options.morphTags );
	setOption( lemmas, 							options.lemmas );
	setOption( hebrewPoints, 				options.hebrewPoints );
	setOption( hebrewCantillation, 	options.hebrewCantillation );
	setOption( greekAccents, 				options.greekAccents);
}

void CSwordBackend::setAllDisplayOptions( const CSwordBackend::displayOptionsBool options){
  if (m_entryDisplay)
		m_entryDisplay->setAllDisplayOptions(options);	
  if (m_chapterDisplay)
		m_chapterDisplay->setAllDisplayOptions(options);	
}


/** I copied this method from swmgr.cpp of SWORD. This is just a workaround
	* that BibleTime isn't closed when
	* mods.d wasn't found.
	*/
void CSwordBackend::Load() {
	qDebug("CSwordBackend::Load");
	if (!config) {	// If we weren't passed a config object at construction, find a config file
		if (!configPath)	// If we weren't passed a config path at construction...
			findConfig(&configType, &prefixPath, &configPath);
		if (configPath) {
			if (configType)
				loadConfigDir(configPath);
			else	
				config = myconfig = new SWConfig(configPath);
		}
	}

	if (config) {
		SectionMap::iterator Sectloop, Sectend;
		ConfigEntMap::iterator Entryloop, Entryend;

		DeleteMods();

		for (Sectloop = config->Sections.lower_bound("Globals"), Sectend = config->Sections.upper_bound("Globals"); Sectloop != Sectend; Sectloop++) {		// scan thru all 'Globals' sections
			for (Entryloop = (*Sectloop).second.lower_bound("AutoInstall"), Entryend = (*Sectloop).second.upper_bound("AutoInstall"); Entryloop != Entryend; Entryloop++)	// scan thru all AutoInstall entries
				InstallScan((*Entryloop).second.c_str());		// Scan AutoInstall entry directory for new modules and install
		}		
		if (configType) {	// force reload on config object because we may have installed new modules
			delete myconfig;
			config = myconfig = 0;
			loadConfigDir(configPath);
		}
		else	
			config->Load();

		CreateMods();

//	augment config with ~/.sword/mods.d if it exists ---------------------
			char *envhomedir  = getenv ("HOME");
			if (envhomedir != NULL && configType != 2) { // 2 = user only
				string path = envhomedir;
				if ((envhomedir[strlen(envhomedir)-1] != '\\') && (envhomedir[strlen(envhomedir)-1] != '/'))
					path += "/";
				path += ".sword/";
				if (FileMgr::existsDir(path.c_str(), "mods.d")) {
					char *savePrefixPath = 0;
					char *saveConfigPath = 0;
					SWConfig *saveConfig = 0;
					stdstr(&savePrefixPath, prefixPath);
					stdstr(&prefixPath, path.c_str());
					path += "mods.d";
					stdstr(&saveConfigPath, configPath);
					stdstr(&configPath, path.c_str());
					saveConfig = config;
					config = myconfig = 0;
					loadConfigDir(configPath);

					CreateMods();

					stdstr(&prefixPath, savePrefixPath);
					delete []savePrefixPath;
					stdstr(&configPath, saveConfigPath);
					delete []saveConfigPath;
					(*saveConfig) += *config;
					delete myconfig;
					config = myconfig = saveConfig;
				}
			}
// -------------------------------------------------------------------------
	}
	else {
		if (!configPath)
			m_errorCode = noSwordModuleConfigDirectory;
		else
			m_errorCode = noModulesAvailable;	
		qWarning("BibleTime: Can't find 'mods.conf' or 'mods.d'. Please setup /etc/sword.conf! Read the documentation on www.bibletime.de how to do this!");
	}
//	if ( (access("/etc/sword.conf",R_OK) == -1) && !strlen(getenv("SWORD_PATH")))
//		m_errorCode = noSwordConfigFile;
}

/** This function searches for a module with the specified description */
CSwordModuleInfo* CSwordBackend::findModuleByDescription(const QString& description){
//  qDebug("CSwordBackend::findModuleByDescription(const QString&)");
  if (m_moduleList && m_moduleList->count())
    for ( m_moduleList->first();m_moduleList->current();m_moduleList->next() )
      if ( m_moduleList->current()->getDescription() == description )
        return m_moduleList->current();
  return 0;
}

/** This function searches for a module with the specified description */
const QString CSwordBackend::findModuleNameByDescription(const QString& description){
	if (moduleDescriptionMap.contains(description)) {
//		qWarning("findModuleNameByDescription: found!!");
		return moduleDescriptionMap[description];
	}
//	qWarning("findModuleNameByDescription: NOT found!!");	
//  if (m_moduleList && m_moduleList->count())
//    for ( m_moduleList->first();m_moduleList->current();m_moduleList->next() )
//      if ( m_moduleList->current()->getDescription() == description )
//        return m_moduleList->current();
  return QString::null;
}

/** This function searches for a module with the specified name */
CSwordModuleInfo* CSwordBackend::findModuleByName(const QString& name){
//  qDebug("CSwordBackend::findModuleByName(const QString&)");
  if (m_moduleList && m_moduleList->count())
    for ( m_moduleList->first(); m_moduleList->current(); m_moduleList->next() )
      if ( m_moduleList->current()->name() == name )
        return m_moduleList->current();
  return 0;
}

/** Returns our local config object to store the cipher keys etc. locally for each user. The values of the config are merged with the global config. */
const bool CSwordBackend::getModuleConfig(const QString& module, SWConfig& moduleConfig) {
	SectionMap::iterator section;
	DIR *dir = opendir(configPath);
	struct dirent *ent;
	
	bool foundConfig = false;	
	QString modFile;	
	if (dir) {    // find and update .conf file
		rewinddir(dir);
		while ((ent = readdir(dir)) && !foundConfig) {
			if ((strcmp(ent->d_name, ".")) && (strcmp(ent->d_name, ".."))) {								
				modFile = configPath;
				modFile += "/";
				modFile += ent->d_name;
				moduleConfig = SWConfig( (const char*)modFile.local8Bit() );
				section =	moduleConfig.Sections.find( (const char*)module.local8Bit() );
				foundConfig = ( section != moduleConfig.Sections.end() );
			}
		}
		closedir(dir);
	}
	else { //try to read mods.conf
		//moduleConfig = SWConfig( configPath + "/mods.conf" );
		moduleConfig = SWConfig("");//global config		
		section =	config->Sections.find( (const char*)module.local8Bit() );		
		foundConfig = ( section != config->Sections.end() );		
		
		ConfigEntMap::iterator entry;
		if (foundConfig) { //copy module section
			for (entry = (*section).second.begin(); entry != (*section).second.end(); entry++) {
					moduleConfig.Sections[(*section).first].insert(ConfigEntMap::value_type((*entry).first, (*entry).second));
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
					moduleConfig = SWConfig( (const char*)modFile.local8Bit() );
					section =	moduleConfig.Sections.find( (const char*)module.local8Bit() );
					foundConfig = ( section != moduleConfig.Sections.end() );
				}
			}
			closedir(dir);
		}
	}
	return foundConfig;
}

/** Returns the path of the module with the name "moduleName". If no path is found return QString::null */
const QString CSwordBackend::getModulePath( const QString moduleName ){
	QString path = QString::null;
	SWConfig c("");
	if (getModuleConfig(moduleName, c)) {
		path = QString::fromLocal8Bit( c[moduleName.latin1()]["DataPath"].c_str() );		
		//remove "./" fromt the beginning ...
		if (path.left(2) == "./")
			path = path.mid(2);
		if (QString::fromLatin1(c.filename.c_str()).left( QString("%1/.sword/").arg(getenv("HOME")).length() ) ==	QString("%1/.sword/").arg(getenv("HOME")) )
			path = path.prepend( QString("%1/.sword/").arg(getenv("HOME")) );
		else //global
			path.prepend(prefixPath);
	}
	return path;
}

/** Returns the text used for the option given as parameter. */
const QString CSwordBackend::getOptionName( const CSwordBackend::moduleOptions option ){
	switch (option) {
		case CSwordBackend::footnotes:
			return QString("Footnotes");
		case CSwordBackend::strongNumbers:
			return QString("Strong's Numbers");
		case CSwordBackend::headings:
			return QString("Headings");
		case CSwordBackend::morphTags:
			return QString("Morphological Tags");
  	case CSwordBackend::lemmas:
			return QString("Lemmas");
		case CSwordBackend::hebrewPoints:
			return QString("Hebrew Vowel Points");
		case CSwordBackend::hebrewCantillation:
			return QString("Hebrew Cantillation");
		case CSwordBackend::greekAccents:
			return QString("Greek Accents");
	}
	return QString::null;	
}
/** Returns the translated name of the option given as parameter. */
const QString CSwordBackend::getTranslatedOptionName(const CSwordBackend :: moduleOptions option){
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
	}
	return QString::null;	

}


const QString CSwordBackend::getConfigOptionName( const CSwordBackend::moduleOptions option ){
	switch (option) {
		case CSwordBackend::footnotes:
			return QString("Footnotes");
		case CSwordBackend::strongNumbers:
			return QString("Strongs");
		case CSwordBackend::headings:
			return QString("Headings");
		case CSwordBackend::morphTags:
			return QString("Morph");
  	case CSwordBackend::lemmas:
			return QString("Lemma");
		case CSwordBackend::hebrewPoints:
			return QString("HebrewPoints");
		case CSwordBackend::hebrewCantillation:
			return QString("Cantillation");
		case CSwordBackend::greekAccents:
			return QString("GreekAccents");
	}
	return QString::null;	
}
