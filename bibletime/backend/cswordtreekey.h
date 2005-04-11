/********* Read the file LICENSE for license details. *********/

#ifndef CSWORDTREEKEYIDX_H
#define CSWORDTREEKEYIDX_H

//BibleTime includes
#include "cswordkey.h"

//Sword includes
#include <treekeyidx.h>

class CSwordModuleInfo;

/** BibleTime's implementation of Sword's TreeKeyIdx class.
  * @author The BibleTime team
  */
class CSwordTreeKey : public CSwordKey, public sword::TreeKeyIdx  {
public:
	CSwordTreeKey( const CSwordTreeKey& k );
	CSwordTreeKey( const sword::TreeKeyIdx *k, CSwordModuleInfo* module );
	CSwordModuleInfo* const module( CSwordModuleInfo* const newModule );
	virtual CSwordTreeKey* copy() const;
  /**
  * Returns the current key
  */
  virtual const QString key() const;
  /**
  * Set the key. If the parameter is empty or null, the key will be set to "/"
  */
  virtual void key( const QString& key );
  /**
  * Set/get the key. If the parameter is not set (means equal to QString::null)
  * the used key is returned. Otherwise the key is set and the new on ei returned.
  */
	virtual void key( const char* key );
  /**
  * Assignment operator.
  */
  virtual CSwordTreeKey& operator = (const QString& keyname );
};

#endif
