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
#include "cswordmoduleinfo.h"
#include "cswordbiblemoduleinfo.h"
#include "cswordcommentarymoduleinfo.h"
#include "cswordlexiconmoduleinfo.h"
//#include "../../ctoolclass.h"

//Qt includes
//#include <qstring.h>

//KDE includes
//#include <kglobal.h>
//#include <kconfig.h>
//#include <klocale.h>

//Sword includes
#include <swdisp.h>
#include <gbfhtml.h>
#include <rwphtml.h>
#include <plainhtml.h>
#include <rawgbf.h>
#include <thmlhtml.h>
#include <rtfhtml.h>
#include <localemgr.h>
#include <filemgr.h>
#include <utilstr.h>

CSwordBackend::CSwordBackend() : SWMgr(0,0,false) {	
	m_errorCode = noError;
	
	m_localConfig = new CSwordConfig( QString("%1/.sword/localopts").
		arg( getenv("HOME") ? QString::fromLocal8Bit(getenv("HOME")) : QString("~")).local8Bit(), config);
	
	//set variables to NULL
	m_entryDisplay = 0;
	m_chapterDisplay = 0;
	m_moduleList = 0;
	m_gbfFilter = m_rwpFilter = m_plainTextFilter = m_thmlFilter = 0;
		
	//Load Sword modules
	Load();	
}

CSwordBackend::~CSwordBackend(){
	qDebug("Destructor of CSwordBackend");
		
	//delete filters
	if (m_gbfFilter)
		delete m_gbfFilter;
	if (m_rwpFilter)
		delete m_rwpFilter;	
	if (m_plainTextFilter)
		delete m_plainTextFilter;	
	if (m_thmlFilter)
		delete m_thmlFilter;	
	
	if (m_localConfig) {
		m_localConfig->Save();
		delete m_localConfig;	
	}
	
	shutdownModules();	
}

#define CHECK_HTML_CHAPTER_DISLPAY \
	if (!m_chapterDisplay) \
		m_chapterDisplay = new CHTMLChapterDisplay();

#define CHECK_HTML_ENTRY_DISLPAY \
	if (!m_entryDisplay) \
		m_entryDisplay = new CHTMLEntryDisplay();
		
/** Initializes the Sword modules. */
CSwordBackend::errorCode CSwordBackend::initModules() {
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
			ASSERT(newModule);
			CHECK_HTML_CHAPTER_DISLPAY	//a macro to check the chapter display			
			newModule->module()->Disp(m_chapterDisplay);
		} else if (!strcmp(curMod->Type(), "Commentaries")) {
			newModule = new CSwordCommentaryModuleInfo(this, curMod);
			ASSERT(newModule);
			CHECK_HTML_ENTRY_DISLPAY	//a macro to check the entry display
			newModule->module()->Disp(m_entryDisplay);
		} else if (!strcmp(curMod->Type(), "Lexicons / Dictionaries")) {
			newModule = new CSwordLexiconModuleInfo(this, curMod);
			ASSERT(newModule);
			CHECK_HTML_ENTRY_DISLPAY	//a macro to check the entry display			
			newModule->module()->Disp(m_entryDisplay);
		}	
		if (newModule)	//append the new modules to our list
			m_moduleList->append( newModule );
	}	
	
	if (m_moduleList->count() == 0)
		m_errorCode = noModulesAvailable;
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

	if (!stricmp(sourceformat.c_str(), "GBF")) {
		if (!m_gbfFilter)
			m_gbfFilter = new GBFHTML();
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
			m_thmlFilter = new ThMLHTML();
		module->AddRenderFilter(m_thmlFilter);
		noDriver = false;
	}

	if (!stricmp(module->Name(), "RWP")) {
		if (!m_rwpFilter)
			m_rwpFilter = new GBFHTML();		
		module->AddRenderFilter(m_rwpFilter);
		noDriver = false;
	}

	if (noDriver){
		if (!stricmp(moduleDriver.c_str(), "RawCom")) {
			if (!m_plainTextFilter)
				m_plainTextFilter = new PLAINHTML();
			module->AddRenderFilter(m_plainTextFilter);
			noDriver = false;
		}
		if (!stricmp(moduleDriver.c_str(), "RawLD")) {
			if (!m_plainTextFilter)
				m_plainTextFilter = new PLAINHTML();
			module->AddRenderFilter(m_plainTextFilter);
			noDriver = false;
		}
	}
}

/**Returns The list of modules managed by this backend*/
ListCSwordModuleInfo* CSwordBackend::getModuleList(){
	ASSERT(m_moduleList);
	return m_moduleList;
}

/** This function deinitializes the modules and deletes them. */
bool CSwordBackend::shutdownModules(){
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
bool CSwordBackend::isOptionEnabled( CSwordBackend::moduleOptions type){
	char *optionName = 0;
	switch (type) {
		case CSwordBackend::footnotes:
			optionName = (char*)"Footnotes";
			break;
		case CSwordBackend::strongNumbers:
			optionName = (char*)"Strong's Numbers";
			break;			
	}	
	return (bool)getGlobalOption(optionName);
}

/** Sets the given options enabled or disabled depending on the second parameter. */
void CSwordBackend::setOption( CSwordBackend::moduleOptions type, bool enable){
	char *optionName = 0;
	switch (type) {
		case CSwordBackend::footnotes:
			optionName = (char*)"Footnotes";
			break;
		case CSwordBackend::strongNumbers:
			optionName = (char*)"Strong's Numbers";
			break;			
	}	
	//we have to clear the module cache of the modules which support the option
	for (m_moduleList->first(); m_moduleList->current(); m_moduleList->next()) {
		if (m_moduleList->current()->supportsFeature(type)) {
			m_moduleList->current()->clearCache();
		}
	}
	
	setGlobalOption(optionName, enable ? "On": "Off");
}

/** I copied this method from swmgr.cpp of SWORD. This is just a workaround
	* that BibleTime isn't closed when
	* mods.d wasn't found.
	*/
void CSwordBackend::Load() {
	if (!config) {	// If we weren't passed a config object at construction, find a config file
		if (!configPath)	// If we weren't passed a config path at construction...
			findConfig(&configType, &prefixPath, &configPath);
		if (configPath) {
			if (configType)
				loadConfigDir(configPath);
			else	config = myconfig = new SWConfig(configPath);
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
		else	config->Load();

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
		m_errorCode = noModulesAvailable;
	}
}

/** Returns the current language of the international booknames. */
QString CSwordBackend::getCurrentBooknameLanguage(){
	if (LocaleMgr::systemLocaleMgr.getDefaultLocaleName())
		return LocaleMgr::systemLocaleMgr.getDefaultLocaleName();	
	return QString::null;;
}

/** Sets the language for the international booknames of Sword. */
bool CSwordBackend::setBooknameLanguage( const QString language ){
	LocaleMgr::systemLocaleMgr.setDefaultLocaleName( language.local8Bit() );
	return true;
}

/** This function searches for a module with the specified description */
CSwordModuleInfo* CSwordBackend::findModuleByDescription(QString description){
  if (m_moduleList)
    for ( m_moduleList->first();m_moduleList->current();m_moduleList->next() )
      if ( m_moduleList->current()->getDescription() == description )
        return m_moduleList->current();
  return 0;
}

/** This function searches for a module with the specified name */
CSwordModuleInfo * CSwordBackend::findModuleByName(QString name){
  if (m_moduleList)
    for ( m_moduleList->first();m_moduleList->current();m_moduleList->next() )
      if ( (QString)m_moduleList->current()->module()->Name() == name )
        return m_moduleList->current();
  return 0;

}

/** Returns our local config object to store the cipher keys etc. locally for each user. The values of the config are merged with the global config. */
SWConfig* CSwordBackend::localConfig(){
	ASSERT(m_localConfig);
	return m_localConfig;
}
