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
	m_clonedModule = false;	
//	if (module)
//		m_module = module->clone();
	m_searchResult.ClearList();

//	qWarning((const char*)SWVersion::currentVersion);
//	ASSERT(backend());	
	if (backend()) {
		if (hasVersion() && (requiredSwordVersion() > SWVersion::currentVersion)) {
		 	qWarning("The module \"%s\" requires a newer Sword library. Please update to \"Sword %s\".", name().latin1(), (const char*)requiredSwordVersion());
		}
	}
}

CSwordModuleInfo::CSwordModuleInfo( const CSwordModuleInfo& m ) {
	m_module = m.m_module; //clone Sword module, don't forget to delete it later
	m_clonedModule = false;
	m_searchResult = m.m_searchResult;
}


/** No descriptions */
CSwordModuleInfo* CSwordModuleInfo::clone(){
	return new CSwordModuleInfo(*this);
}

CSwordModuleInfo::~CSwordModuleInfo(){
	m_searchResult.ClearList();
	if (m_clonedModule) {
		qWarning("delete cloned module");
		delete m_module;
	}
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
	if (isEncrypted() && cipherKey().isEmpty())
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


/** Returns the cipher key if the module is encrypted, if the key is not set return QString::empty, if the module is not encrypted retur QString::null. */
const QString CSwordModuleInfo::cipherKey() const {
	if (!isEncrypted())
		return QString::null;
		
	const string key = (*backend()->getConfig())[name().latin1()]["CipherKey"];		
	if (key.length())
		return QString::fromLocal8Bit( key.c_str() );
	else
		return QString::null;
}

/** Returns the description of the module */
const QString CSwordModuleInfo::description() const {
	return QString::fromLocal8Bit( m_module->Description() );
}

/** Returns the about information of this module. */
const QString CSwordModuleInfo::aboutInformation() const {
	const string about = (*backend()->getConfig())[name().latin1()]["About"];
	
	QString ret = QString::null;
	if (strlen(about.c_str())) {	
		RTFHTML filter;
		const int len = about.length()+600;
		char dummy[len];
		strcpy(dummy, about.c_str());
		
		filter.ProcessText(dummy,len,0);
		ret = QString::fromLocal8Bit(dummy);
	}
	return ret;
}

/** Returns the version number of this module. */
const QString CSwordModuleInfo::version() const{
	const string version = (*backend()->getConfig())[name().latin1()]["Version"];
	if (version.length())
		return QString::fromLocal8Bit( version.c_str() );
	return  QString::null;
}

const bool CSwordModuleInfo::hasVersion() const {
	const string version = (*backend()->getConfig())[name().latin1()]["Version"];
	return version.length();
}


/** Returns the path to this module. */
const QString CSwordModuleInfo::path() const {
	const string path = (*backend()->getConfig())[name().latin1()]["DataPath"];
	if (strlen(path.c_str()))
		return QString::fromLocal8Bit(path.c_str());
	else
		return QString::null;
}

/** Returns true if something was found, otherwise return false. */
const bool CSwordModuleInfo::search( const QString searchedText, const int searchOptions, ListKey scope, void (*percentUpdate)(char, void*) ) {
	int searchType = 0;
 	int searchFlags = REG_ICASE;
	
	//work around Swords thread insafety
//	VerseKey k;
	m_module->SetKey(VerseKey());
	
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
const SWVersion CSwordModuleInfo::requiredSwordVersion(){
	const string version = (*backend()->getConfig())[name().latin1()]["MinimumVersion"];	
	if (!version.length())	//no special version required
		return SWVersion("0.0"); //version not set
	return SWVersion(version.c_str());
//	const float swordVersion = QString::fromLatin1( version.c_str() ).toFloat();	
//	return swordVersion;
}

/** Returns the name of the module. */
const QString CSwordModuleInfo::name() const {
	return QString::fromLatin1( module()->Name() );
}
/** Returns true if this module is Unicode encoded. False if the charset is iso8859-1. */

const bool CSwordModuleInfo::isUnicode(){
	return (module()->isUnicode());
}
