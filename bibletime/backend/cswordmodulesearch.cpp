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
#include "cswordmoduleinfo.h"
#include "cswordbackend.h"
#include "../frontend/cbtconfig.h"

//System includes
#include <pthread.h>

//#include <qdatetime.h>

//Sword includes
#include <swmodule.h>
#include <swkey.h>
#include <listkey.h>


CSwordModuleSearch* searcher = 0;

void startSearchCallback(void *p){
	if (searcher)
		searcher->startSearch();
}

void percentUpdateDummy(char percent, void *p) {
	searcher->percentUpdate(percent, p);
};

CSwordModuleSearch::CSwordModuleSearch() :
	m_searchedText(QString::null),
	m_searchOptions(0),m_foundItems(false),m_isSearching(false),m_terminateSearch(false)
{
	searcher = this;
}

CSwordModuleSearch::~CSwordModuleSearch(){
	searcher = 0;
}

void CSwordModuleSearch::percentUpdate(char percent, void *){
	cms_currentProgress = (int)percent;
	if (cms_module_count > 1)
	  cms_overallProgress = (int)((float)((cms_module_current - 1)*100+cms_currentProgress))/cms_module_count;
	else
	  cms_overallProgress = cms_currentProgress;
	m_updateSig.activate();	
}

/** This function sets the modules which should be searched. */
void CSwordModuleSearch::setModules( ListCSwordModuleInfo& list ) {
	m_moduleList = list;
}

/** Starts the search for the search text. */
const bool CSwordModuleSearch::startSearch() {
	backend()->setFilterOptions ( CBTConfig::getFilterOptionDefaults() );	
	m_foundItems			= false;
	m_terminateSearch = false;
	m_isSearching 		= true;		
		
	cms_currentProgress = 0;
	cms_overallProgress = 0;
	cms_module_current = 0;
	cms_module_count = m_moduleList.count();
	
	bool foundItems = false;
	
	for (m_moduleList.first(); m_moduleList.current() && !m_terminateSearch; m_moduleList.next()) {
		cms_module_current++;
		if ( m_moduleList.current()->search(m_searchedText, m_searchOptions, m_searchScope, &percentUpdateDummy) )
			foundItems = true;
	}
	cms_currentProgress = 100;
	cms_overallProgress = 100;
	
	m_foundItems = foundItems;		
	m_isSearching = false;
	m_terminateSearch = false;

	m_finishedSig.activate();		
	return true;
}

void CSwordModuleSearch::startSearchThread(void){
//	pthread_t thread;
	
//	pthread_attr_t attr;
//	pthread_attr_init(&attr);
//	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	
//	pthread_mutex_init(&percentage_mutex, NULL);
//	pthread_mutex_init(&signal_mutex, NULL);			

//	percentage_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
	/*const int i =*/ //pthread_create( &thread, /*&attr*/NULL, &startSearchCallback, this );

	startSearch();
	
//	pthread_cond_wait(&finish_cond, &dummy_mutex);
//	pthread_mutex_lock(&signal_mutex);			
//	m_finishedSig.activate();	
//	pthread_mutex_unlock(&signal_mutex);				
}

/** Sets the text which should be search in the modules. */
void CSwordModuleSearch::setSearchedText( const QString text ){
	m_searchedText = text;
}

/** Sets the search scope. */
void CSwordModuleSearch::setSearchScope( ListKey scope ) {
	m_searchScope.copyFrom( scope );
}

/** Sets the search scope back. */
void CSwordModuleSearch::resetSearchScope() {
	m_searchScope.ClearList();
}

/** Interrupts the current search. */
void CSwordModuleSearch::interruptSearch() {
	if (m_isSearching)
		m_terminateSearch = true; //no other modules will be searched
	for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next())
		m_moduleList.current()->interruptSearch(); //interrupt the current module
}

/** Returns true if in the last search the searcher found items, if no items were found return false. */
const bool CSwordModuleSearch::foundItems() {
	return m_foundItems;
}

/** Sets the options for this search. Options include theflags and search types of the Sword searc interface. */
void CSwordModuleSearch::setSearchOptions( int options ){
	m_searchOptions = options;
}

/** Returns the percent for the given type. */
const int CSwordModuleSearch::getPercent( percentType type ){
	switch (type) {
		case currentModule:
			return cms_currentProgress;
		case allModules:
			return cms_overallProgress;
//		default:
//			return 0;
	};
	return 0;
}

/** Returns a copy of the used search scope. */
ListKey& CSwordModuleSearch::scope() {
	return m_searchScope;
}

void CSwordModuleSearch::connectPercentUpdate( QObject *receiver, const char *member ) {
	m_updateSig.connect(receiver, member);
};

void CSwordModuleSearch::connectFinished( QObject *receiver, const char *member ) {
	m_finishedSig.connect(receiver, member);
};

/** Should be called when the search finished. */
void CSwordModuleSearch::searchFinished(){
//	qWarning("CSwordModuleSearch::searchFinished!");
//	m_updateSig.block(true);	
//	pthread_mutex_lock(&signal_mutex);			
	m_finishedSig.activate();	
//	pthread_mutex_unlock(&signal_mutex);
//	m_updateSig.block(false);
}
