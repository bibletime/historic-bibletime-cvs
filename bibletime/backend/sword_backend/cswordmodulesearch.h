/***************************************************************************
                          cswordmodulesearch.h  -  description
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

#ifndef CSWORDMODULESEARCH_H
#define CSWORDMODULESEARCH_H

//BibleTime includes
#include "cswordmoduleinfo.h"

//Qt includes
#include <qlist.h>
#include <qstring.h>

//System includes
#include <pthread.h>

class ListKey;

/**
	* CSwordModuleSearch manages the search on Sword modules. It manages the thread(s)
	* and manages the different modules.
  *
  * @author The BibleTime team
  * @version $Id$
  */
class CSwordModuleSearch {
public:
	enum scopeType {
		Scope_NoScope,
		Scope_LastSearch,
		Scope_Bounds
	};
  enum searchOptions {
  	exactPhrase		= 0x000000001,
  	multipleWords	= 0x000000002,
  	regExp				= 0x000000004,
  	caseSensitive	= 0x000000008,  	
  	useLastResult	= 0x000000010,
  	useScope	 		= 0x000000020
	};
	enum percentType {
  	currentModule	= 0x000000001,
		allModules		= 0x000000002
	};

	CSwordModuleSearch();
	/**
	* The destructor of this class. It cleans uop memory before it's deleted.
	*/
	~CSwordModuleSearch();
  /**
 	* Sets the text which should be search in the modules.
 	*/
  void setSearchedText( const QString );
  /**
 	* Starts the search for the search text.
 	*/
  bool startSearch();
  /**
 	* This function sets the modules which should be searched.
 	*/
  void setModules( ListCSwordModuleInfo* );
  /**
 	* Sets the search scope.
 	*/
  void setSearchScope( ListKey scope );
  /**
 	*	Calls with true if you want to use the last searchresult as search scope.
 	*/
  void useLastSearchResult( bool useIt );
  /**
 	* Sets the seaech scope back.
 	*/
  void resetSearchScope();
  /**
 	* Interrupts the current search.
 	*/
  void interruptSearch();
  /**
 	* @return "true" if in the last search the searcher found items, if no items were found return "false"
 	*/
  bool foundItems();
  /**
 	* Returns true if the search is still in progress, otherwise return false.
 	*/
  bool isSearching();
  /**
 	* This functions starts the search: First it creates a new thread for the search and it calles the the
 	* function to start the search.
 	*/
	void startSearchThread(void);
  /**
 	* Sets the options for this search. Options include the
	* flags and search types of the Sword searc interface.
	*/
  void setSearchOptions( int options );
  /**
 	* Returns the percent for the given type.
 	*/
  int getPercent( percentType type );
	void percentUpdate(char percent, void *p);

protected:
	QString m_searchedText;
	ListKey m_searchScope;
	ListCSwordModuleInfo*	m_moduleList;

	int m_searchOptions;

	bool m_useLastSearchResult;
	bool m_foundItems;
	bool m_isSearching;
	bool m_terminateSearch;

	int cms_currentProgress;
	int cms_overallProgress;
	int cms_module_count;
	int cms_module_current;

private:
	pthread_mutex_t percentage_mutex;
};

#endif
