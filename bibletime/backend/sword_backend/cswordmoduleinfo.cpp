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

CSwordModuleInfo::CSwordModuleInfo( CSwordBackend* backend, SWModule* module ){
	m_backend = backend;
	m_module = module;
}

CSwordModuleInfo::~CSwordModuleInfo(){
	m_searchResult.ClearList();
}

/** Sets the unlock key of the modules and writes the key into the cofig file.*/
const CSwordModuleInfo::unlockErrorCode CSwordModuleInfo::unlock( const QString unlockKey ){
	CSwordModuleInfo::unlockErrorCode	ret = CSwordModuleInfo::noError;
	SWConfig moduleConfig("");
	if ( m_backend->getModuleConfig(m_module->Name(), moduleConfig) ) {
		moduleConfig[m_module->Name()]["CipherKey"] = unlockKey.local8Bit();	
		m_backend->setCipherKey(m_module->Name(), unlockKey.local8Bit());	
		moduleConfig.Save();
		*(m_backend->getConfig())+=moduleConfig;
	}	
	else
		CSwordModuleInfo::wrongUnlockKey;
	return ret;
}

/** Returns the display object for this module. */
CHTMLEntryDisplay* CSwordModuleInfo::getDisplay() const {
	return dynamic_cast<CHTMLEntryDisplay*>(m_module->Disp());
}

/** This function returns true if this module is locked, otherwise return false. */
const bool CSwordModuleInfo::isLocked() {
	if (isEncrypted() && getCipherKey().isEmpty())
		return true;
	return false;
}

/** This functions returns true if this module is encrypted (locked or unlocked). */
const bool CSwordModuleInfo::isEncrypted() const {
	/* if we have the CipherKey entry the module
		* is encrypted but not necessary locked
		*/		
	ConfigEntMap config	= m_backend->getConfig()->Sections.find( m_module->Name() )->second;;
	ConfigEntMap::iterator it = config.find("CipherKey");
	if (it != config.end())
		return true;
	return false;
}


/** Returns the cipher key if the module is encrypted, if the key is not set return QString::empty, if the module is not encrypted retur QString::null. */
const QString CSwordModuleInfo::getCipherKey() const {
	if (!isEncrypted())
		return QString::null;
		
	string key = (*m_backend->getConfig())[m_module->Name()]["CipherKey"];		
	if (strlen(key.c_str()))
		return QString::fromLocal8Bit( key.c_str() );
	else
		return QString::null;
}

/** Returns the description of the module */
const QString CSwordModuleInfo::getDescription() const {
	return QString::fromLocal8Bit( m_module->Description() );
}

/** Returns the about information of this module. */
const QString CSwordModuleInfo::getAboutInformation() const {
	const string about = (*m_backend->getConfig())[m_module->Name()]["About"];
	
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
const QString CSwordModuleInfo::getVersion() const{
	const string version = (*m_backend->getConfig())[m_module->Name()]["Version"];
	if (strlen( version.c_str() ))
		return QString::fromLocal8Bit( version.c_str() );
	else 	
		return  QString::null;
}

/** Returns the path to this module. */
const QString CSwordModuleInfo::getPath() const {
	const string path = (*m_backend->getConfig())[m_module->Name()]["DataPath"];
	if (strlen(path.c_str()))
		return QString::fromLocal8Bit(path.c_str());
	else
		return QString::null;
}

/** Returns true if something was found, otherwise return false. */
const bool CSwordModuleInfo::search( const QString searchedText, const int searchOptions, ListKey scope, void (*percentUpdate)(char, void*) ) {
	//workaround for thread-insafety
#warning WORKAROUND!
	SWKey* k = module()->CreateKey();
	module()->SetKey(*k);
	delete k;
	
	int searchType = 0;
 	int searchFlags = REG_ICASE;
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
		searchScope = &m_searchResult;
    m_searchResult = m_module->Search((const char*)searchedText.local8Bit(), searchType, searchFlags, searchScope, 0, percentUpdate);
	}
	else if (searchOptions & CSwordModuleSearch::useScope) {
		searchScope = &scope;  	
  	m_searchResult = m_module->Search((const char*)searchedText.local8Bit(), searchType, searchFlags, getType() != Lexicon ? searchScope : 0, 0, percentUpdate);
	}
  else  {
  	m_searchResult = m_module->Search((const char*)searchedText.local8Bit(), searchType, searchFlags, 0, 0, percentUpdate);
  }
	return m_searchResult.Count();
}

/** Returns the last search result for this module. */
const ListKey& CSwordModuleInfo::getSearchResult() {
	return m_searchResult;
}

/** Clears the last search result. */
void CSwordModuleInfo::clearSearchResult(){
	m_searchResult.ClearList();
}

/** This interupts the search if this module is being searched. */
void CSwordModuleInfo::interruptSearch(){
	module()->terminateSearch = true;
}

/** Returns true if the given type i supported by this module. */
const bool CSwordModuleInfo::supportsFeature( const CSwordBackend::moduleOptions type){
	bool ret = false;
	
	ConfigEntMap config = m_backend->getConfig()->Sections.find( m_module->Name() )->second;	
	ConfigEntMap::iterator start 	= config.lower_bound("GlobalOptionFilter");
	ConfigEntMap::iterator end 		= config.upper_bound("GlobalOptionFilter");		
	QString text = QString::null;
	
	switch (type) {
		case CSwordBackend::footnotes:
			text = "Footnotes";
			break;
		case CSwordBackend::strongNumbers:
			text = "Strongs";
			break;			
	}	
	
	for (; start != end; start++) {
		QString option = (*start).second.c_str();
		qDebug( (const char*)option.local8Bit() );
		if ( option.contains(text) > 0 ) {
			ret = true;
			break;
		}
	}	
	return ret;
}

/** Used to find out the module specific font */
const QFont CSwordModuleInfo::getFont(){
  const string familyString = (*m_backend->getConfig())[m_module->Name()]["Font"];
  const string sizeString = (*m_backend->getConfig())[m_module->Name()]["Font size"];
	
  QString family = QString::null;
  int size = 12;
  if (strlen(familyString.c_str()))
  	family = QString::fromLocal8Bit( familyString.c_str() );
  if (strlen(sizeString.c_str()))
  	size = QString::fromLocal8Bit( sizeString.c_str() ).toInt();
  QFont f( family, size );
  f.setCharSet(QFont::AnyCharSet);

  return f;
}


/** Used to set the module specific font */
void CSwordModuleInfo::setFont(const QFont &font){
	(*m_backend->getConfig())[m_module->Name()]["Font"] = font.family().local8Bit();
	(*m_backend->getConfig())[m_module->Name()]["Font size"] = QString::number(font.pointSize()).local8Bit();
	m_backend->getConfig()->Save();
}

/** Used to find out if the module has a specific font */
const bool CSwordModuleInfo::hasFont(){
	const string font = (*m_backend->getConfig())[m_module->Name()]["Font"];
	if (strlen(font.c_str()))
		return true;
	return false;
}

/** Returns the backend. */
CSwordBackend* CSwordModuleInfo::backend() const {
	return m_backend;
}
