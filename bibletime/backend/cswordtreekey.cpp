/***************************************************************************
                          cswordtreekeyidx.cpp  -  description
                             -------------------
    begin                : Thu Jan 24 2002
    copyright            : (C) 2002 by The BibleTime team
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

#include "cswordtreekey.h"
#include "cswordbookmoduleinfo.h"

CSwordTreeKey::CSwordTreeKey( const CSwordTreeKey& k ) : CSwordKey(k), TreeKeyIdx(k) {
  
}

CSwordTreeKey::CSwordTreeKey( const TreeKeyIdx *k, CSwordModuleInfo* module ) : CSwordKey(module), TreeKeyIdx(*k) {
  
}

CSwordTreeKey* CSwordTreeKey::copy() const {
	return new CSwordTreeKey(*this);
}

/** Sets the key of this instance */
const QString CSwordTreeKey::key() const {
//   return QString::fromLocal8Bit( getFullName() ); //don't use fromUtf8
	return QString::fromUtf8( getFullName() ); //don't use fromUtf8
}

void CSwordTreeKey::key( const QString& newKey ){
  if (newKey.isEmpty()) {
    root();
  }
  else
    TreeKeyIdx::operator = ((const char*)newKey.local8Bit());		//don't use Utf8! Doesn't work with umlauts!	
 	
  if (Error())
 		root();
}

void CSwordTreeKey::key( const char* newKey ){
	if (newKey) {
		TreeKeyIdx::operator = (newKey);
	}
}

CSwordModuleInfo* const CSwordTreeKey::module( CSwordModuleInfo* const newModule ) {
	if (newModule && (newModule != m_module) && (newModule->type() == CSwordModuleInfo::GenericBook) ) {
		m_module = newModule;

 		const QString oldKey = key();
		
		CSwordBookModuleInfo* newBook = dynamic_cast<CSwordBookModuleInfo*>(newModule);
		copyFrom( *(newBook->tree()) );

 		key(oldKey); //try to restore our old key
		
		//set the key to the root node
 		root();
		firstChild();
	}
	
	return m_module;
}

/** Assignment operator. */
CSwordTreeKey& CSwordTreeKey::operator = (const QString& keyname ){
  key(keyname);
  return *this;
}
