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

//BibleTime - backend
#include "cswordmoduleinfo.h"

//BibleTime - utils
#include "util/cpointers.h"

//Qt includes
#include <qptrlist.h>
#include <qstring.h>
#include <qsignal.h>

//System includes
#include <pthread.h>

//Sword includes
#include <listkey.h>

/**
	* CSwordModuleSearch manages the search on Sword modules. It manages the thread(s)
	* and manages the different modules.
  *
  * @author The BibleTime team
  * @version $Id$
  */
class CSwordModuleSearch: public CPointers {
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
  	entryAttribs  = 0x000000008,
  	cluceneIndex  = 0x000000010,

		caseSensitive	= 0x000000020,  	
  	useLastResult	= 0x000000040,
  	useScope	 		= 0x000000080
	};
	enum PercentType {
  	currentModule,
		allModules
	};
  /**
  * This is used to show the selection in the searchscope dialog
  */
  enum SearchScope 		{ NoScope, LastSearch, Bounds	};

  /** This is used to so show the selection of the searchdialog
  *
  */
  enum SearchType 		{ MultiWord, ExactPhrase, RegEx	};


	CSwordModuleSearch();
	/**
	* The destructor of this class. It cleans uop memory before it's deleted.
	*/
	virtual ~CSwordModuleSearch();
  /**
 	* Sets the text which should be search in the modules.
 	*/
  void setSearchedText( const QString& );
  /**
 	* Starts the search for the search text.
 	*/
  const bool startSearch();
  /**
 	* This function sets the modules which should be searched.
 	*/
  void setModules( const ListCSwordModuleInfo& );
  /**
 	* Sets the search scope.
 	*/
  void setSearchScope( const sword::ListKey& scope );
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
  const bool foundItems() const;
  /**
 	* This functions starts the search: First it creates a new thread for the search and it calles the the
 	* function to start the search.
 	*/
	void startSearchThread();
  /**
 	* Sets the options for this search. Options include the
	* flags and search types of the Sword searc interface.
	*/
  void setSearchOptions( const int options );
  /**
 	* Returns the percent for the given type.
 	*/
  inline const int getPercent( const PercentType type );
	inline static void percentUpdate(char percent, void *p);
  /**
  * Returns a copy of the used search scope.
  */
  const sword::ListKey& scope() const;
	void connectPercentUpdate( QObject * receiver, const char * member );
	void connectFinished( QObject * receiver, const char * member );
  void searchFinished();
	
protected:
	QString m_searchedText;
	sword::ListKey m_searchScope;
	ListCSwordModuleInfo m_moduleList;

	int m_searchOptions;

	bool m_foundItems;
	bool m_isSearching;
	bool m_terminateSearch;

	int cms_currentProgress;
	int cms_overallProgress;
	int cms_module_count;
	int cms_module_current;

private:
	QSignal m_updateSig;
	QSignal m_finishedSig;
	
	static CSwordModuleSearch* searcher;
};

/** Returns the percent for the given type. */
inline const int CSwordModuleSearch::getPercent( const PercentType type ){
	switch (type) {
		case currentModule:
			return cms_currentProgress;
		case allModules:
			return cms_overallProgress;
	};
	
	return 0;
}

inline void CSwordModuleSearch::percentUpdate(char percent, void *){
	searcher->cms_currentProgress = (int)percent;
	
	if (searcher->cms_module_count > 1) {
	  searcher->cms_overallProgress = (int)((float)((searcher->cms_module_current - 1) * 100 + searcher->cms_currentProgress)) / searcher->cms_module_count;
	}
	else {
	  searcher->cms_overallProgress = searcher->cms_currentProgress;
	}
	
	searcher->m_updateSig.activate();	
}



#endif
