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

CSwordTreeKey::CSwordTreeKey( const CSwordTreeKey& k ) : TreeKeyIdx(k), CSwordKey(k) {
	qWarning("copy constructor of CSwordTreeKey");
	Persist(0);
}

CSwordTreeKey::CSwordTreeKey( const TreeKeyIdx *k, CSwordModuleInfo* module ) : TreeKeyIdx(*k), CSwordKey() {
	m_module = module;
	Persist(0);
}

CSwordTreeKey* CSwordTreeKey::copy() const {
	qWarning("CSwordTreeKey* CSwordTreeKey::clone() const");
	return new CSwordTreeKey(*this);
}

CSwordTreeKey::~CSwordTreeKey() {
	qWarning("destructor of CSwordTreeKey of object %p", this);
}

/** Sets the key of this instance */
const QString CSwordTreeKey::key( const QString& newKey ){
//	qWarning("CSwordTreeKey::key( const QString& newKey )");
	if (!newKey.isNull()) {
		TreeKeyIdx::operator = ((const char*)newKey.local8Bit());		//don't use Utf8! Doesn't work with umlauts!
		m_module->module()->SetKey(this);
	//snap here to entry?
		TreeKeyIdx::operator = (m_module->module()->KeyText());
	}
// 	qWarning(getFullName());
 	return QString::fromLocal8Bit( getFullName() );//don't use fromUtf8
}

void CSwordTreeKey::key( const char* newKey ){
	if (newKey) {
		TreeKeyIdx::operator = (newKey);
	}
}

