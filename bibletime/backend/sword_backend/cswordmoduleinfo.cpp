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
	m_cache = 0;
	m_backend = backend;
	m_module = module;
}

CSwordModuleInfo::~CSwordModuleInfo(){
	m_searchResult.ClearList();
	if (m_cache)
		delete m_cache;
}

/** Returns the module object so all objects can access the module. */
SWModule* CSwordModuleInfo::module() {
	ASSERT(m_module);
	return m_module;
}

/** Returns the backend. */
CSwordBackend* CSwordModuleInfo::backend(){
	ASSERT(m_backend);
	return m_backend;
}

/** Sets the unlock key of the modules and writes the key into the cofig file.*/
CSwordModuleInfo::unlockErrorCode CSwordModuleInfo::unlock( const QString unlockKey ){
	CSwordModuleInfo::unlockErrorCode	ret = CSwordModuleInfo::noError;
  (*m_backend->localConfig())[m_module->Name()]["CipherKey"] = unlockKey.local8Bit();	
	m_backend->setCipherKey( (const char*)m_module->Name(), unlockKey.local8Bit());
	
	
	m_backend->localConfig()->Save();	
	return ret;
}

/** Returns the display object for this module. */
CHTMLEntryDisplay* CSwordModuleInfo::getDisplay(){
	CHTMLEntryDisplay* ret = 0;
	if (m_module->Disp()) {
		if (dynamic_cast<CHTMLEntryDisplay*>(m_module->Disp()))
			ret = dynamic_cast<CHTMLEntryDisplay*>(m_module->Disp());
		else
			ret = 0;
	}
	return ret;
}

/** This function returns true if this module is locked, otherwise return false. */
bool CSwordModuleInfo::isLocked() {
	if (isEncrypted() && getCipherKey().isEmpty()) {
		debug("module is locked");		
		return true;
	}
	else {
		debug("module is NOT locked");
		return false;
	}
}

/** This functions returns true if this module is encrypted (locked or unlocked). */
bool CSwordModuleInfo::isEncrypted() const{
	qDebug("CSwordModuleInfo::isEncrypted()");
	/* if we have the CipherKey entry the module
		* is encrypted but not necessary locked
		*/		
	ConfigEntMap config	= m_backend->config->Sections.find( m_module->Name() )->second;;
	ConfigEntMap::iterator it = config.find("CipherKey");
	bool ret = false;
	if (it != config.end()) {
		ret = true;
	}
	qDebug("return now");
	
	return ret;
}


/** Returns the cipher key if the module is encrypted, if the key is not set return QString::empty, if the module is not encrypted retur QString::null. */
QString CSwordModuleInfo::getCipherKey() const {
	qDebug("m_backend->localConfig()->Save()");
	if (!isEncrypted())
		return;
		
	string key = (*m_backend->localConfig())[m_module->Name()]["CipherKey"];
	if (!strlen(key.c_str()))
		key = (*m_backend->config)[m_module->Name()]["CipherKey"];
		
	if (strlen(key.c_str()))
		return QString::fromLocal8Bit( key.c_str() );
	else
		return QString::null;
}

/** Returns the description of the module */
QString CSwordModuleInfo::getDescription() const {
	return QString::fromLocal8Bit( m_module->Description() );
}

/** Returns the about information of this module. */
QString CSwordModuleInfo::getAboutInformation() const {
	const string about = (*m_backend->config)[m_module->Name()]["About"];
	
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
QString CSwordModuleInfo::getVersion() const{
	const string version = (*m_backend->config)[m_module->Name()]["Version"];
	if (strlen( version.c_str() ))
		return QString::fromLocal8Bit( version.c_str() );
	else 	
		return  QString::null;
}

/** Returns the path to this module. */
QString CSwordModuleInfo::getPath() const {
	const string path = (*m_backend->config)[m_module->Name()]["DataPath"];
	if (strlen(path.c_str()))
		return QString::fromLocal8Bit(path.c_str());
	else
		return QString::null;
}

/** Returns true if something was found, otherwise return false. */
bool CSwordModuleInfo::search( const QString searchedText, int searchOptions, ListKey scope, void (*percentUpdate)(char, void*) ) {
	int searchType = 0;
 	int searchFlags = REG_ICASE;
	//setup variables required for Sword
	if (searchOptions & CSwordModuleSearch::caseSensitive)
		searchFlags = 0;

	if (searchOptions & CSwordModuleSearch::exactPhrase)
		searchType = -1; //-1 == exact phrase
	else if (searchOptions & CSwordModuleSearch::multipleWords)
		searchType = -2; //-2 == multiple words
	else if (searchOptions & CSwordModuleSearch::regExp)
		searchType = 0;	// 0 == regexp matching

	SWKey* searchScope = 0;
	if ((searchOptions & CSwordModuleSearch::useLastResult) && m_searchResult.Count()) {
    qDebug("use last search result");
		searchScope = &m_searchResult;
    m_searchResult = m_module->Search((const char*)searchedText.local8Bit(), searchType, searchFlags, searchScope, 0, percentUpdate);
//  	delete searchScope;
	}
	else if (searchOptions & CSwordModuleSearch::useScope) {
		qDebug("use own scope");
		searchScope = &scope;  	
  	m_searchResult = m_module->Search((const char*)searchedText.local8Bit(), searchType, searchFlags, searchScope, 0, percentUpdate);
//  	delete searchScope;
	}
  else  {
    qDebug("use no scope");
  	m_searchResult = m_module->Search((const char*)searchedText.local8Bit(), searchType, searchFlags, 0, 0, percentUpdate);
  }
	
	qDebug("finsished search");
	qDebug((const char*)QString("Number of found items: %1").arg(m_searchResult.Count()).local8Bit());
	
	return (bool)m_searchResult.Count();
}

/** Returns the last search result for this module. */
ListKey& CSwordModuleInfo::getSearchResult() {
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
bool CSwordModuleInfo::supportsFeature( CSwordBackend::moduleOptions type){
	bool ret = false;
	
	ConfigEntMap config = m_backend->config->Sections.find( m_module->Name() )->second;	
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

/** This function adds an item to the cache. */
void CSwordModuleInfo::addCacheItem( const QString key, const QString cachedText){
	qDebug("CSwordModuleInfo::addCacheItem( const QString key, const QString cachedText)");
	qDebug(key.local8Bit());
	qDebug(cachedText.local8Bit());
	ASSERT(m_cache);		
	if (!m_cache) {
		m_cache = new QMap<QString,QString>();
	}	
	if (!key.isEmpty() &&!cachedText.isEmpty()) {
		m_cache->insert(key,cachedText);
	}
}

/** Returns the cached text for the key. If the key is not cached return QString::null */
QString CSwordModuleInfo::getCacheItem( const QString key ){
	qDebug("CSwordModuleInfo::getCacheItem()");
	ASSERT(m_cache);	
	if (!m_cache) {
		m_cache = new QMap<QString, QString>();
	}
	
	if (!key.isEmpty()) {
  	QMap<QString, QString>::Iterator ret = m_cache->find(key);
  	if (ret != m_cache->end())
    	return ret.data();
  	else
  	  return QString::null;
	}
}

/** Used to find out the module specific font */
QFont CSwordModuleInfo::getFont(){
  qDebug("CSwordModuleInfo::getFont()");
  const string familyString = (*m_backend->localConfig())[m_module->Name()]["Font"];
  const string sizeString = (*m_backend->localConfig())[m_module->Name()]["Font size"];
	
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
  qDebug("CSwordModuleInfo::setFont()");	
	(*m_backend->localConfig())[m_module->Name()]["Font"] = font.family().local8Bit();
	(*m_backend->localConfig())[m_module->Name()]["Font size"] = QString::number(font.pointSize()).local8Bit();
	
	m_backend->localConfig()->Save();
}

/** Used to find out if the module has a specific font */
bool CSwordModuleInfo::hasFont(){
	const string font = (*m_backend->config)[m_module->Name()]["Font"];
	if (strlen(font.c_str()))
		return true;
	else
		return false;
}

/** Clears the cache of this module. */
void CSwordModuleInfo::clearCache(){
	qDebug("CSwordModuleInfo::clearCache()");
	if (!m_cache) {
		m_cache = new QMap<QString, QString>();
	}	
	m_cache->clear();
}


/** Returns the type of the module. */
CSwordModuleInfo::type CSwordModuleInfo::getType(){
	return CSwordModuleInfo::Unknown;
}
