/***************************************************************************
                          cswordmodulesearch.cpp  -  description
                             -------------------
    begin                : Fri Oct 13 2000
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
#include "cswordmodulesearch.h"
#include <pthread.h>

//Sword includes
#include <swmodule.h>
#include <swkey.h>
#include <listkey.h>


CSwordModuleSearch* searcher = 0;

void* dummy(void *p){
	if ((CSwordModuleSearch *)p)
		((CSwordModuleSearch *)p)->startSearch();

	qWarning("Finished the dummy function, we should now close the thread.");
	return NULL;
}

void percentUpdateDummy(char percent, void *p) {
	qDebug("void percentUpdate(char percent, void *p)");
	if (searcher) {
		searcher->percentUpdate(percent, p);
	}
	else
		qWarning("searcher is 0");
};

CSwordModuleSearch::CSwordModuleSearch(){
	qDebug("CSwordModuleSearch::CSwordModuleSearch()");
	m_isSearching = false;
	m_foundItems = false;
	m_terminateSearch = false;
//	m_useScope = false;
	m_searchedText = QString::null;

	m_moduleList = 0;
	m_searchOptions = 0;

	searcher = this;
}

CSwordModuleSearch::~CSwordModuleSearch(){
	qDebug("CSwordModuleSearch::~CSwordModuleSearch()");
}

void CSwordModuleSearch::percentUpdate(char percent, void *p){
//	if (percent != old_p){
		pthread_mutex_lock(&percentage_mutex);

//		old_p = percent;
		cms_currentProgress = (int)percent;
		qDebug((const char*)QString("%1 %").arg((int)percent).local8Bit());
		if (cms_module_count > 1)
		  cms_overallProgress = (int)((float)((cms_module_current - 1)*100+cms_currentProgress))/cms_module_count;
		else
		  cms_overallProgress = cms_currentProgress;

		pthread_mutex_unlock(&percentage_mutex);
//	}
}

/** This function sets the modules which should be searched. */
void CSwordModuleSearch::setModules( ListCSwordModuleInfo* list ){
	qDebug("CSwordModuleSearch::setModules( ListCSwordModuleInfo* list )");
	ASSERT(list);

	if (!m_moduleList)
		m_moduleList = new ListCSwordModuleInfo();

	if (list != m_moduleList)
		m_moduleList->clear();
	if (!list) {
		qDebug("return");
		return;
	}

	for (list->first(); list->current(); list->next() ) {
		m_moduleList->append( list->current() );
	}
}

/** Starts the search for the search text. */
bool CSwordModuleSearch::startSearch() {
	qDebug("CSwordModuleSearch::startSearch()");
//	ASSERT(m_searchScopeList);
//	if (!m_searchScopeList)
//		m_searchScopeList = new ListSearchScopeStruct;
//
//	m_foundItems	= false;
//	m_terminateSearch = false;

	pthread_mutex_lock(&percentage_mutex);

	cms_currentProgress = 0;
	cms_overallProgress = 0;
	cms_module_current = 0;
	cms_module_count = m_moduleList->count();

	pthread_mutex_unlock(&percentage_mutex);

	bool foundItems = false;

	for (m_moduleList->first(); m_moduleList->current(); m_moduleList->next()) {
		qDebug("Module is %s ",m_moduleList->current()->module()->Name());
		cms_module_current++;
		m_moduleList->current()->clearSearchResult();
		if ( m_moduleList->current()->search(m_searchedText, m_searchOptions, m_searchScope, &percentUpdateDummy) )
      foundItems = true;
	}

	pthread_mutex_lock(&percentage_mutex);
	cms_currentProgress=100;
	cms_overallProgress=100;
	pthread_mutex_unlock(&percentage_mutex);

	m_foundItems = foundItems;

	m_isSearching = false;

	return true;
}


void CSwordModuleSearch::startSearchThread(void){
	qDebug("CSwordModuleSearch::startSearchThread(void)");

	m_foundItems	= false;
	m_terminateSearch = false;
	m_isSearching = true;
		
	pthread_t thread;
	
	pthread_attr_t attr;
	pthread_attr_init(&attr);
//	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
	pthread_mutex_init(&percentage_mutex, NULL);	
	
	const int i = pthread_create( &thread, &attr, &dummy, this );	

  if (i == -1)	//an error occurred
		qWarning("pthread_create failed");
	qWarning( (const char*)QString("Thread: %1").arg(i).local8Bit() );
}

/** Sets the text which should be search in the modules. */
void CSwordModuleSearch::setSearchedText( const QString text ){
	m_searchedText = text;
}

/** Sets the search scope. */
void CSwordModuleSearch::setSearchScope( ListKey scope ) {
	m_searchScope = scope;
}

/** Sets the seaech scope back. */
void CSwordModuleSearch::resetSearchScope() {
	m_searchScope = NULL;
}

/** Calls with true if you want to use the last searchresult as search scope. */
void CSwordModuleSearch::useLastSearchResult( bool useIt ) {
	m_useLastSearchResult = useIt;
}

/** Interrupts the current search. */
void CSwordModuleSearch::interruptSearch() {
	if (m_isSearching)
		m_terminateSearch = true; //no other modules will be searched
	for (m_moduleList->first(); m_moduleList->current(); m_moduleList->next())
		m_moduleList->current()->interruptSearch(); //interrupt the current module
}

/** Returns true if the search is still in progress, otherwise return false. */
bool CSwordModuleSearch::isSearching() {
	return m_isSearching;
}

/** Returns true if in the last search the searcher found items, if no items were found return false. */
bool CSwordModuleSearch::foundItems() {
	return m_foundItems;
}

/** Sets the options for this search. Options include theflags and search types of the Sword searc interface. */
void CSwordModuleSearch::setSearchOptions( int options ){
	m_searchOptions = options;
}

/** Returns the percent for the given type. */
int CSwordModuleSearch::getPercent( percentType type ){
	int ret = 0;
	pthread_mutex_lock(&percentage_mutex);
	if (type == currentModule) {
		ret = cms_currentProgress;
	}
	else if (type == allModules){
		ret = cms_overallProgress;
	}
	pthread_mutex_unlock(&percentage_mutex);
	return ret;
}
