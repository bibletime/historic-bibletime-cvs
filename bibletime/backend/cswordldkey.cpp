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
	m_module = dynamic_cast<CSwordLexiconModuleInfo*>(module);
	if (m_module)
		SWKey::operator = (m_module->module()->KeyText());
}

/** No descriptions */
CSwordLDKey::CSwordLDKey( const CSwordLDKey &k ) : SWKey(k), CSwordKey(k) {
}

/** No descriptions */
CSwordLDKey::CSwordLDKey( const SWKey *k, CSwordModuleInfo* module) : SWKey(*k), CSwordKey() {
	m_module = module;
}

CSwordLDKey::~CSwordLDKey(){
}

/** Clones this object by copying the members. */
CSwordLDKey* CSwordLDKey::copy() const {
	return new CSwordLDKey(*this);
}

/** Sets the module of this key. */
CSwordModuleInfo* CSwordLDKey::module(CSwordModuleInfo* newModule){
	const QString oldKey = key();	
	if (newModule && newModule->getType() == CSwordModuleInfo::Lexicon) {
		m_module = newModule;
		key(oldKey);
	}	
	return m_module;
}

/** Sets the key of this instance */
const QString CSwordLDKey::key( const QString& newKey ){
//	qWarning("const QString CSwordLDKey::key( const QString& newKey )");
	if (!newKey.isNull()) {
		SWKey::operator = ((const char*)newKey.local8Bit());		
		m_module->module()->SetKey(this);
		(const char*)*(m_module->module()); //snap to entry
		SWKey::operator = (m_module->module()->KeyText());
	}
 	return QString::fromLocal8Bit((const char*)*this);//don't use fromUtf8
}

/** Uses the parameter to returns the next entry afer this key. */
void CSwordLDKey::NextEntry(){
	m_module->module()->SetKey(this);	//use this key as base for the next one!		
	( *( m_module->module() ) )++;
	key(m_module->module()->KeyText());
	SWKey::operator = (m_module->module()->KeyText());	
}

/** Uses the parameter to returns the next entry afer this key. */
void CSwordLDKey::PreviousEntry(){
	m_module->module()->SetKey(this);	//use this key as base for the next one!		
	( *( m_module->module() ) )--;
	SWKey::operator = (m_module->module()->KeyText());	
}

/** Sets the key of this instance */
void CSwordLDKey::key( const char* newKey ){
	if (newKey) {
		SWKey::operator = (newKey);

		m_module->module()->SetKey(this);
		(const char*)*(m_module->module()); //snap to entry
		SWKey::operator = (m_module->module()->KeyText());
	}
}

