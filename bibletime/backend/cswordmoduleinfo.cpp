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
#include "centrydisplay.h"
#include "cswordmodulesearch.h"
#include "cswordkey.h"
#include "util/scoped_resource.h"

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

CSwordModuleInfo::CSwordModuleInfo( SWModule* module ) {
	m_module = module;
	m_searchResult.ClearList();
	m_dataCache.name = QString::fromLatin1(module->Name());
	m_dataCache.isUnicode = m_module->isUnicode();

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
	m_module = 0; //the Sword module object is deleted by the backend
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
CEntryDisplay* const CSwordModuleInfo::getDisplay() const {
	return dynamic_cast<CEntryDisplay*>(m_module->Disp());
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
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(this) );
	SWKey* s = dynamic_cast<SWKey*>(key.get());
	if (s)
		m_module->SetKey(*s);
	
	//setup variables required for Sword
	if (searchOptions & CSwordModuleSearch::caseSensitive)
		searchFlags = 0;

	if (searchOptions & CSwordModuleSearch::multipleWords)
		searchType = -2; //multiple words	
	else if (searchOptions & CSwordModuleSearch::exactPhrase)
		searchType = -1; //exact phrase
	else if (searchOptions & CSwordModuleSearch::regExp)
		searchType = 0;	//regexp matching

	if ((searchOptions & CSwordModuleSearch::useLastResult) && m_searchResult.Count()) {		
		util::scoped_ptr<SWKey> searchScope( m_searchResult.clone() );
		m_searchResult = m_module->Search(searchedText.utf8(), searchType, searchFlags, searchScope, 0, percentUpdate);
	}
	else if (searchOptions & CSwordModuleSearch::useScope) {
		m_searchResult = m_module->Search(searchedText.utf8(), searchType, searchFlags, (type() != Lexicon && type() != GenericBook) ? &scope : 0, 0, percentUpdate);
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
	m_module->terminateSearch = true;
}

/** Returns the required Sword version for this module. Returns -1 if no special Sword version is required. */
const SWVersion CSwordModuleInfo::minimumSwordVersion(){
	return SWVersion( config(CSwordModuleInfo::MinimumSwordVersion).latin1() );
}

/** Returns the name of the module. */
const QString CSwordModuleInfo::name() const {
	return m_dataCache.name;
}

/** Returns true if this module is Unicode encoded. False if the charset is iso8859-1. */
const bool CSwordModuleInfo::isUnicode(){
	return m_dataCache.isUnicode;
}

const QString CSwordModuleInfo::config( const CSwordModuleInfo::ConfigEntry entry) {
	switch (entry) {
		case AboutInformation:
		{
			QString about = QString::fromLatin1(m_module->getConfigEntry("About"));
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
		case CipherKey:
			return QString::fromLatin1(m_module->getConfigEntry("CipherKey"));
		case AbsoluteDataPath:
			return QString::fromLatin1(m_module->getConfigEntry("AbsoluteDataPath"));
		case DataPath:
			return QString::fromLatin1(m_module->getConfigEntry("DataPath"));
		case Description:
			return QString::fromLocal8Bit(m_module->Description());			
		case ModuleVersion:
			return QString::fromLatin1(m_module->getConfigEntry("Version"));
		case MinimumSwordVersion: {
			const QString version = QString::fromLatin1(m_module->getConfigEntry("MinimumVersion"));
			return !version.isEmpty() ? version : QString::fromLatin1("0.0");
		}
		case TextDir: {
			const QString dir = QString::fromLatin1(m_module->getConfigEntry("Direction"));
			return !dir.isEmpty() ? dir : QString::fromLatin1("LtoR");
		}		
    case DisplayLevel: {
			const QString level = QString::fromLatin1(m_module->getConfigEntry("DisplayLevel"));
			return !level.isEmpty() ? level : QString::fromLatin1("1");
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
		case DailyDevotional:
			return m_module->getConfig().has("Feature", "DailyDevotion");		
		case Glossary:
			return m_module->getConfig().has("Feature", "Glossary");		
	}
	return 0;
}

const bool CSwordModuleInfo::has( const CSwordBackend::FilterOptions option ){
	//BAD workaround to see if the filter is GBF or ThML!	
  const QString name = backend()->configOptionName(option);
  if (m_module->getConfig().has("GlobalOptionFilter",QString::fromLatin1("GBF%1").arg(name).latin1()))
 		return true;
 	if (m_module->getConfig().has("GlobalOptionFilter",QString::fromLatin1("ThML%1").arg(name).latin1()))
 		return true;
 	if (m_module->getConfig().has("GlobalOptionFilter",QString::fromLatin1("UTF8%1").arg(name).latin1()))
 		return true; 		
 	if (m_module->getConfig().has("GlobalOptionFilter",name.latin1()))
 		return true;
 	
 	return false;
}

/** Returns the text direction of the module's text., */
const CSwordModuleInfo::TextDirection CSwordModuleInfo::textDirection(){
  if (config(TextDir) == "RtoL")
    return CSwordModuleInfo::RightToLeft;
  else
    return CSwordModuleInfo::LeftToRight;
}
