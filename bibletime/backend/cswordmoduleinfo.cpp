/***************************************************************************
                          cswordmoduleinfo.cpp  -  description
                             -------------------
    begin                : Wed Oct 11 2000
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
#include "cswordmoduleinfo.h"
#include "cswordbackend.h"
#include "chtmlentrydisplay.h"
#include "cswordmodulesearch.h"
#include "cswordkey.h"

#include <sys/types.h>
#include <unistd.h>
#include <stddef.h>
#include <dirent.h>
#include <regex.h>

//Qt includes

//Sword includes
#include <swmodule.h>
#include <swkey.h>
#include <listkey.h>
#include <versekey.h>
#include <swconfig.h>
#include <rtfhtml.h>

//static class wide objects
//static CSwordBackend searchModulesMgr;
	
CSwordModuleInfo::CSwordModuleInfo( SWModule* module ) {
	m_module = module;
	m_searchResult.ClearList();

	if (backend()) {
		if (hasVersion() && (minimumSwordVersion() > SWVersion::currentVersion)) {
		 	qWarning("The module \"%s\" requires a newer Sword library. Please update to \"Sword %s\".", name().latin1(), (const char*)minimumSwordVersion());
		}
	}
}

CSwordModuleInfo::CSwordModuleInfo( const CSwordModuleInfo& m ) {
	m_module = m.m_module;
	m_searchResult = m.m_searchResult;
}


/** No descriptions */
CSwordModuleInfo* CSwordModuleInfo::clone(){
	return new CSwordModuleInfo(*this);
}

CSwordModuleInfo::~CSwordModuleInfo(){
	m_searchResult.ClearList();
	m_module = 0; //the real Sword module is deleted by the backend
}

/** Sets the unlock key of the modules and writes the key into the cofig file.*/
const CSwordModuleInfo::UnlockErrorCode CSwordModuleInfo::unlock( const QString& unlockKey ){
	CSwordModuleInfo::UnlockErrorCode	ret = CSwordModuleInfo::noError;
	SWConfig moduleConfig("");
	if ( backend()->moduleConfig(name(), moduleConfig) ) {
		moduleConfig[name().latin1()]["CipherKey"] = unlockKey.local8Bit();	
		backend()->setCipherKey(name().latin1(), unlockKey.local8Bit());	
		(*backend()->getConfig()) += moduleConfig;
		(*backend()->getConfig())[name().latin1()]["CipherKey"] = moduleConfig[name().latin1()]["CipherKey"];
		moduleConfig.Save();
	}	
	else
		ret = CSwordModuleInfo::wrongUnlockKey;
	return ret;
}

/** Returns the display object for this module. */
CHTMLEntryDisplay* CSwordModuleInfo::getDisplay() const {
	return dynamic_cast<CHTMLEntryDisplay*>(m_module->Disp());
}

/** This function returns true if this module is locked, otherwise return false. */
const bool CSwordModuleInfo::isLocked() {
	if (isEncrypted() && config(CipherKey).isEmpty())
		return true;
	return false;
}

/** This functions returns true if this module is encrypted (locked or unlocked). */
const bool CSwordModuleInfo::isEncrypted() const {
	/**
	* If we have the CipherKey entry the module
	* is encrypted but not necessary locked
	*/		
	ConfigEntMap config	= backend()->getConfig()->Sections.find( name().latin1() )->second;
	ConfigEntMap::iterator it = config.find("CipherKey");
	if (it != config.end())
		return true;
	return false;
}

const bool CSwordModuleInfo::hasVersion() const {
	const string version = (*backend()->getConfig())[name().latin1()]["Version"];
	return version.length();
}


/** Returns true if something was found, otherwise return false. */
const bool CSwordModuleInfo::search( const QString searchedText, const int searchOptions, ListKey scope, void (*percentUpdate)(char, void*) ) {
	int searchType = 0;
 	int searchFlags = REG_ICASE;
	
	//work around Swords thread insafety for Bibles and Commentaries
	CSwordKey* key = CSwordKey::createInstance(this);
	SWKey* s = dynamic_cast<SWKey*>(key);
	m_module->SetKey(s);
	delete key;
	
	//setup variables required for Sword
	if (searchOptions & CSwordModuleSearch::caseSensitive)
		searchFlags = 0;

	if (searchOptions & CSwordModuleSearch::exactPhrase)
		searchType = -1; //exact phrase
	else if (searchOptions & CSwordModuleSearch::multipleWords)
		searchType = -2; //multiple words
	else if (searchOptions & CSwordModuleSearch::regExp)
		searchType = 0;	//regexp matching
		
	SWKey* searchScope = 0;
	if ((searchOptions & CSwordModuleSearch::useLastResult) && m_searchResult.Count()) {
		searchScope = m_searchResult.clone();
		m_searchResult = m_module->Search(searchedText.utf8(), searchType, searchFlags, searchScope, 0, percentUpdate);
		delete searchScope;
	}
	else if (searchOptions & CSwordModuleSearch::useScope) {
		searchScope = &scope;		
		m_searchResult = m_module->Search(searchedText.utf8(), searchType, searchFlags, (type() != Lexicon) ? searchScope : 0, 0, percentUpdate);
	}
  else
  	m_searchResult = m_module->Search(searchedText.utf8(), searchType, searchFlags, 0, 0, percentUpdate);
	return (m_searchResult.Count()>0);
}

/** Returns the last search result for this module. */
ListKey& CSwordModuleInfo::searchResult(const ListKey* newResult) {
	if (newResult)
		m_searchResult.copyFrom( *newResult );	
	return m_searchResult;
}

/** Clears the last search result. */
void CSwordModuleInfo::clearSearchResult(){
	m_searchResult.ClearList();
}

/** This interupts the search if this module is being searched. */
void CSwordModuleInfo::interruptSearch(){
//	searchModulesMgr.Modules[name()]->terminateSearch = true;
	m_module->terminateSearch = true;
}

/** Returns true if the given type i supported by this module. */
const bool CSwordModuleInfo::supportsFeature( const CSwordBackend::moduleOptions type){
	ConfigEntMap config = backend()->getConfig()->Sections.find( name().latin1() )->second;	
	ConfigEntMap::iterator start 	= config.lower_bound("GlobalOptionFilter");
	ConfigEntMap::iterator end 		= config.upper_bound("GlobalOptionFilter");		
	
	const QString text = backend()->configOptionName(type);
	for (; start != end; start++) {
		const QString option = QString::fromLatin1((*start).second.c_str());
		if ( option.contains(text) )
			return true;
	}	
	return false;
}

/** Returns the required Sword version for this module. Returns -1 if no special Sword version is required. */
const SWVersion CSwordModuleInfo::minimumSwordVersion(){
	return SWVersion( config(CSwordModuleInfo::MinimumSwordVersion).latin1() );
}

/** Returns the name of the module. */
const QString CSwordModuleInfo::name() const {
	return QString::fromLatin1( module()->Name() );
}
/** Returns true if this module is Unicode encoded. False if the charset is iso8859-1. */

const bool CSwordModuleInfo::isUnicode(){
	return (module()->isUnicode());
}

const QString CSwordModuleInfo::config( const CSwordModuleInfo::ConfigEntry entry) {
//	qWarning("CSwordModuleInfo::config called");
	
	switch (entry) {
		case AboutInformation:
		{
//			qWarning("--About");
			QString about = configEntry("About");	
			if (!about.isEmpty()) {	
				RTFHTML filter;
				const int len = about.length()+600;
				char dummy[len];
				strcpy(dummy, about.local8Bit());
		
				filter.ProcessText(dummy,len,0);
				about = QString::fromLocal8Bit(dummy);
			}			
			return about;
		}
		
		case CipherKey: {
//			qWarning("--Cipher key");		
			return configEntry("CipherKey");
//			const char* key = m_module->getConfigEntry("CipherKey");
//			return strlen(key) ? QString::fromLocal8Bit(key) : QString::null;
		}
			
		case AbsoluteDataPath: {
//			qWarning("--DataPath");
			return configEntry("AbsoluteDataPath");
//			const char* path = m_module->getConfigEntry("DataPath");
//			return strlen(path) ? QString::fromLocal8Bit(path) : QString::null;	
		}
		case DataPath: {
//			qWarning("--DataPath");
			return configEntry("DataPath");
//			const char* path = m_module->getConfigEntry("DataPath");
//			return strlen(path) ? QString::fromLocal8Bit(path) : QString::null;	
		}			
		case Description:
//			qWarning("--Description");			
			return QString::fromLocal8Bit(m_module->Description());
			
		case ModuleVersion: {
//			qWarning("--Module version");
			return configEntry("Version");
		}
			
		case MinimumSwordVersion:
		{
//			qWarning("--Minimum Sword version");
			QString version = configEntry("MinimumVersion");
			return !version.isEmpty() ? version : QString::fromLatin1("0.0");
		}
			
		default:
//			qWarning("--default");
			return QString::null;
	}
}

/** No descriptions */
const QString CSwordModuleInfo::configEntry(const QString& entry){
//	qWarning("CSwordModuleInfo::configEntry");
//	qWarning(name().latin1());
//	qWarning(entry.latin1());
//	if (!m_module)
//		return QString::null;
//	SWConfig config("");
//	if (!backend()->moduleConfig(name(), config))
//		return QString::null;
//	const string data = (config)[m_module->Name()][(const char*)entry.local8Bit()];
//	return data.length() ? QString::fromLocal8Bit(data.c_str()) : QString::null;
//	const char* data = m_module->getConfigEntry((const char*)entry.latin1());
//	qWarning("return now!");
//	return data ? QString::fromLatin1(data) : QString::null;
	return QString::fromLatin1( m_module->getConfigEntry(entry.latin1()) );
}
