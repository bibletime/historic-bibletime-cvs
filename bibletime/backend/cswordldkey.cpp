/***************************************************************************
                          cswordldkey.cpp  -  description
                             -------------------
    begin                : Thu Oct 5 2000
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
#include "cswordldkey.h"
#include "cswordlexiconmoduleinfo.h"

//Qt includes

//Sword includes
#include <swmodule.h>
#include <swld.h>
#include <utilstr.h>


CSwordLDKey::CSwordLDKey( CSwordModuleInfo* module ) {
	if ((m_module = dynamic_cast<CSwordLexiconModuleInfo*>(module))) {
//    *(m_module->module()) = TOP;	
  }
  SWKey::operator = (" ");
}

/** No descriptions */
CSwordLDKey::CSwordLDKey( const CSwordLDKey &k ) : SWKey((const char*)k), CSwordKey(k) {
}

/** No descriptions */
CSwordLDKey::CSwordLDKey( const SWKey *k, CSwordModuleInfo* module) : SWKey(*k), CSwordKey(module) {
}

/** Clones this object by copying the members. */
CSwordLDKey* CSwordLDKey::copy() const {
	return new CSwordLDKey(*this);
}

/** Sets the module of this key. */
CSwordModuleInfo* const CSwordLDKey::module(CSwordModuleInfo* const newModule){	
	if (newModule && newModule->type() == CSwordModuleInfo::Lexicon) {
		const QString oldKey = key();	
		m_module = newModule;
		key(oldKey);
	}	
	return m_module;
}

/** Sets the key of this instance */
const QString CSwordLDKey::key(){
// 	return QString::fromLocal8Bit((const char*)*this);//don't use fromUtf8
 	return QString::fromUtf8((const char*)*this);
}

void CSwordLDKey::key( const QString& newKey ){
//	SWKey::operator = ((const char*)newKey.local8Bit());		
	SWKey::operator = ((const char*)newKey.utf8());		
	m_module->module()->SetKey(this);
	m_module->snap();
	SWKey::operator = (m_module->module()->KeyText());
}

/** Uses the parameter to returns the next entry afer this key. */
CSwordLDKey* CSwordLDKey::NextEntry(){
	m_module->module()->SetKey(this);	//use this key as base for the next one!
  m_module->module()->setSkipConsecutiveLinks(true);  
	( *( m_module->module() ) )++;
  m_module->module()->setSkipConsecutiveLinks(false);    
	key(m_module->module()->KeyText());
	SWKey::operator = (m_module->module()->KeyText());	
	
	return this;
}

/** Uses the parameter to returns the next entry afer this key. */
CSwordLDKey* CSwordLDKey::PreviousEntry(){
	m_module->module()->SetKey(this);	//use this key as base for the next one!		
  m_module->module()->setSkipConsecutiveLinks(true);
  ( *( m_module->module() ) )--;
  m_module->module()->setSkipConsecutiveLinks(false);
	SWKey::operator = (m_module->module()->KeyText());	
	
	return this;
}

/** Sets the key of this instance */
void CSwordLDKey::key( const char* newKey ){
	if (newKey) {
		SWKey::operator = (newKey);

		m_module->module()->SetKey(this);
		m_module->snap();
		SWKey::operator = (m_module->module()->KeyText());
	}
}

/** Assignment operator for more ease of use of this class. */
CSwordLDKey& CSwordLDKey::operator = (const QString& keyname ){
  key(keyname);
  return *this;
}
