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
	m_data = QString::null;	
	if ( !(m_module = dynamic_cast<CSwordLexiconModuleInfo*>(module)) ) {
		throw EBadModule();
	}		
}

CSwordLDKey::~CSwordLDKey(){
	m_data = QString::null;
}

/** Stores the data of this key in the member m_data. */
void CSwordLDKey::getData(){
	m_module->module()->SetKey(*this->clone());
	m_data = QString::fromLocal8Bit((const char*)*m_module->module());
}

/** Sets the key of this instance */
bool CSwordLDKey::setKey( const QString key ){
	SWKey::operator = ((const char*)key.local8Bit());	
	m_module->module()->SetKey(*this->clone());			
	m_data = QString::null;
	
	return !(bool)error;
}

/** Uses the parameter to returns the next entry afer this key. */
void CSwordLDKey::NextEntry(){
	m_module->module()->SetKey(*this->clone());	//use this key as base for the next one!		
	( *( m_module->module() ) )++;
	setKey(m_module->module()->KeyText());
}

/** Uses the parameter to returns the next entry afer this key. */
void CSwordLDKey::PreviousEntry(){
	m_module->module()->SetKey(*this->clone());	//use this key as base for the next one!		
	( *( m_module->module() ) )--;
	setKey(m_module->module()->KeyText());
}

/** Sets the module of this key. */
void CSwordLDKey::setModule( CSwordModuleInfo* module ){
	const QString oldKey = QString::fromLocal8Bit( (const char*)*this );
	if (module && module->getType() == CSwordModuleInfo::Lexicon) {
		m_module = module;	
		setKey(oldKey);
	}
}
