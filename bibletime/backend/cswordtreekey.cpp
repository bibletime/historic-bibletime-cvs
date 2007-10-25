/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2007 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/



#include "cswordtreekey.h"
#include "cswordbookmoduleinfo.h"

#include <qtextcodec.h>

CSwordTreeKey::CSwordTreeKey( const CSwordTreeKey& k ) : CSwordKey(k), TreeKeyIdx(k) {}

CSwordTreeKey::CSwordTreeKey( const TreeKeyIdx *k, CSwordModuleInfo* module ) : CSwordKey(module), TreeKeyIdx(*k) {}

CSwordTreeKey* CSwordTreeKey::copy() const {
	return new CSwordTreeKey(*this);
}

const QString CSwordTreeKey::getLocalNameUnicode() const
{
	Q_ASSERT(m_module);
	CSwordTreeKey* nonconst_this = const_cast<CSwordTreeKey*>(this);
	if (!m_module || m_module->isUnicode()) {
		return QString::fromUtf8(nonconst_this->getLocalName());
	} else {
		return QString::fromLatin1(nonconst_this->getLocalName());
	}
}

/** Returns the key of this instance */
const QString CSwordTreeKey::key() const {
	Q_ASSERT(m_module);
	if (!m_module || m_module->isUnicode()) {
		return QString::fromUtf8(getText());
	} else {
		QTextCodec *codec = QTextCodec::codecForName("CP1252");
		return codec->toUnicode(getText());
	}
}

const bool CSwordTreeKey::key( const QString& newKey ) {
	Q_ASSERT(m_module);
	if (!m_module || m_module->isUnicode()) {
		return key((const char*)newKey.utf8());
	} else {
		QTextCodec *codec = QTextCodec::codecForName("CP1252");
		return key((const char*)codec->fromUnicode(newKey));
	}
}

const bool CSwordTreeKey::key( const char* newKey ) {
	Q_ASSERT(newKey);

	if (newKey) {
		TreeKeyIdx::operator = (newKey);
	}
	else {
		root();
	}

	return !Error();
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
CSwordTreeKey& CSwordTreeKey::operator = (const QString& keyname ) {
			key(keyname);
			return *this;
		}
