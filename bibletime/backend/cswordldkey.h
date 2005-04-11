/********* Read the file LICENSE for license details. *********/#ifndef CSWORDLDKEY_H
#define CSWORDLDKEY_H


//own includes
#include "cswordkey.h"
#include "cswordmoduleinfo.h"

//Qt includes
#include <qstring.h>

//Sword includes
#include <swkey.h>

/**
	*	This class is the implementation of CKey used for dictionaries and lexicons.
	*
	*	CSwordLDKey is the implementation of CKey for Lexicons and dictionaries.
	* It provides a simple interface to set the current key,
	* to get the text for the key and functions to get the next and previous items
	* of the used module in comparision to the current key.<BR>
	* Here's an example how to use this class:<BR>
	* @code
	*		CSwordLexiconModuleInfo* m_module = new CSwordLexiconModuleInfo( sword_module );
	*		CSwordLDKey* ldKey = new CSwordLDKey(m_module);
	*		ldKey->getPreviousEntry( ldKey );
	*		qDebug( QString("The current key is: %1").arg(QString::fromLocal8Bit((const char*)*ldKey)));
	* @endcode
	*
	* Please not, that the result will be invalid if use the operator const char*
	* on the adress of the object, use something like this
  *
	* @code
	*		CSwordLDKey* key = new CSwordLDKey( lexicon_module );
	*		const QString keyname = key->getKey();
	* @endcode
	*
  *	@author The BibleTime team
  *	@version $Id$
  */

class CSwordLDKey : public CSwordKey, public sword::SWKey {
public: 
	/**
	* Constructor of CSwordLDKey
	*/
	CSwordLDKey( CSwordModuleInfo* module );
	/**
	* Copy constructor for this key class.
	*/
	CSwordLDKey( const CSwordLDKey &k );
	/**
	* Copy constructor for this key class.
	*/
	CSwordLDKey( const sword::SWKey *k, CSwordModuleInfo* module);
//	virtual ~CSwordLDKey();
  /**
  * Clones this object by copying the members.
  */
  virtual CSwordLDKey* copy() const;
  /**
  * Uses the parameter to returns the next entry afer this key.
  */
  CSwordLDKey* NextEntry( void );
  /**
  * Uses the parameter to returns the previous entry afer this key.
  */
  CSwordLDKey* PreviousEntry( void );  	
  /**
  * Sets the module of this key.
  */
  virtual CSwordModuleInfo* const module( CSwordModuleInfo* const module = 0 );
  /**
  * Returns the current key as a QString
  */
  virtual const QString key() const;
  /**
  * Set the current key
  */
  virtual void key( const QString& newKey );
  /**
  * Returns the current key as a QString
  */
  virtual void key( const char* );
  /** 
	* Assignment operator for more ease of use of this class. 
	*/
  virtual CSwordLDKey& operator = (const QString& keyname );
};


#endif

