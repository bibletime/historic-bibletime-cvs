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

//Sword includes
#include <swmodule.h>
#include <swkey.h>
#include <listkey.h>


static CSwordModuleSearch* searcher = 0;

CSwordModuleSearch::CSwordModuleSearch() :
	m_searchedText(QString::null),
	m_searchOptions(0),
	m_foundItems(false),
	m_isSearching(false),
	m_terminateSearch(false)
{
	searcher = this;
}

CSwordModuleSearch::~CSwordModuleSearch(){
	searcher = 0;
}

void CSwordModuleSearch::percentUpdate(char percent, void *){
	Q_ASSERT(searcher);
	
	searcher->cms_currentProgress = (int)percent;
	if (searcher->cms_module_count > 1) {
	  searcher->cms_overallProgress = (int)((float)((searcher->cms_module_current - 1) * 100 + searcher->cms_currentProgress)) / searcher->cms_module_count;
	}
	else {
	  searcher->cms_overallProgress = searcher->cms_currentProgress;
	}
	searcher->m_updateSig.activate();	
}

/** This function sets the modules which should be searched. */
void CSwordModuleSearch::setModules( const ListCSwordModuleInfo& list ) {
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
		if ( m_moduleList.current()->search(m_searchedText, m_searchOptions, m_searchScope, &CSwordModuleSearch::percentUpdate) ) {
			foundItems = true;
		}
	}
	cms_currentProgress = 100;
	cms_overallProgress = 100;
	
	m_foundItems = foundItems;		
	m_isSearching = false;
	m_terminateSearch = false;

  m_finishedSig.activate();		
	return true;
}

void CSwordModuleSearch::startSearchThread(){
	startSearch();
}

/** Sets the text which should be search in the modules. */
void CSwordModuleSearch::setSearchedText( const QString& text ){
	m_searchedText = text;
}

/** Sets the search scope. */
void CSwordModuleSearch::setSearchScope( const sword::ListKey& scope ) {
  m_searchScope.copyFrom( scope );
  if (!strlen(scope.getRangeText())) { //we can't search with an empty search scope, would crash
    //reset the scope
    resetSearchScope();
    
    //disable searching with a scope!
  //  if (m_searchOptions | useScope) {
//      qWarning("using the scope!");
      //set back the scope flag
   // }
  }
}

/** Sets the search scope back. */
void CSwordModuleSearch::resetSearchScope() {
	m_searchScope.ClearList();
}

/** Interrupts the current search. */
void CSwordModuleSearch::interruptSearch() {
	if (m_isSearching) {
		m_terminateSearch = true; //no other modules will be searched
	}
	
	for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
		m_moduleList.current()->interruptSearch(); //interrupt the current module
	}
}

/** Returns true if in the last search the searcher found items, if no items were found return false. */
const bool CSwordModuleSearch::foundItems() const {
	return m_foundItems;
}

/** Sets the options for this search. Options include theflags and search types of the Sword searc interface. */
void CSwordModuleSearch::setSearchOptions( const int options ){
	m_searchOptions = options;
}

/** Returns the percent for the given type. */
const int CSwordModuleSearch::getPercent( const PercentType type ){
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
const sword::ListKey& CSwordModuleSearch::scope() const {
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
	m_finishedSig.activate();	
}
