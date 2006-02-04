/********* Read the file LICENSE for license details. *********/

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
	/*enum scopeType {
		Scope_NoScope,
	//	Scope_LastSearch,
		Scope_Bounds
};*/
	/*enum searchOptions {
		exactPhrase  = 0x000000001,
		multipleWords = 0x000000002,
		regExp    = 0x000000004,
		entryAttribs  = 0x000000008,
		cluceneIndex  = 0x000000010,

		caseSensitive = 0x000000020,
		//useLastResult = 0x000000040,
		useScope    = 0x000000080
};*/
	/*enum PercentType {
		currentModule,
		allModules
};*/
	
	/**
	* This is used to show the selection in the searchscope dialog
	*/
	//enum SearchScope   { NoScope, LastSearch, Bounds };

	/** This is used to so show the selection of the searchdialog
	*
	*/
	//enum SearchType   { MultiWord, ExactPhrase, RegEx };


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
	* @return "true" if in the last search the searcher found items, if no items were found return "false"
	*/
	const bool foundItems() const;
	/**
	* Sets the options for this search. Options include the
	* flags and search types of the Sword searc interface.
	*/
//	void setSearchOptions( const int options );
	/**
	* Returns a copy of the used search scope.
	*/
	const sword::ListKey& searchScope() const;

	void connectFinished( QObject * receiver, const char * member );
	void searchFinished();

	/**
	* Returns true if all of the specified modules have indices already built.
	*/
	const bool modulesHaveIndices( const ListCSwordModuleInfo& );
	/**
	* Indexes the specified modules if there is no existing index for the module.
	*/
//	void indexModules( const ListCSwordModuleInfo& );

protected:
	QString m_searchedText;
	sword::ListKey m_searchScope;
	ListCSwordModuleInfo m_moduleList;

	int m_searchOptions;

	bool m_foundItems;

private:
 	QSignal m_finishedSig;
	static CSwordModuleSearch* searcher;
};

#endif
