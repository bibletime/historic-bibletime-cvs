/***************************************************************************
                          cswordkey.cpp  -  description
                             -------------------
    begin                : Thu May 24 2001
    copyright            : (C) 2001 by The BibleTime team
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


//own includes
#include "cswordkey.h"
#include "cswordmoduleinfo.h"

#include "cswordversekey.h"
#include "cswordldkey.h"
#include "cswordtreekey.h"

//Sword includes
#include <swmodule.h>
#include <swkey.h>
#include <versekey.h>
#include <treekey.h>
#include <treekeyidx.h>


CSwordKey::CSwordKey(CSwordModuleInfo* module) : m_module(module) {
}

CSwordKey::CSwordKey(const CSwordKey& k) {
	qWarning("copy constructor of CSwordKey");
	m_module = k.m_module;
//	ASSERT(m_module);
//	qWarning(m_module->name().latin1());
}

CSwordModuleInfo* CSwordKey::module(CSwordModuleInfo* newModule) {
	if (newModule)
		m_module = newModule;
	return m_module;
}

const QString CSwordKey::renderedText() {
	if (!m_module)
		return QString::null;
	SWKey* k = dynamic_cast<SWKey*>(this);
	if (k)
		m_module->module()->SetKey(k);	
	return QString::fromUtf8((const char*)*(m_module->module()));
}

const QString CSwordKey::strippedText() {
	if (!m_module)
		return QString::null;
	SWKey* k = dynamic_cast<SWKey*>(this);
	if (k)
		m_module->module()->SetKey(k);
	return QString::fromUtf8(m_module->module()->StripText());	
}

/** This will create a proper key object from a given module */
CSwordKey* CSwordKey::createInstance( CSwordModuleInfo *module ){
	if (!module)
		return 0;
		
	switch( module->type() ){
		case CSwordModuleInfo::Bible:
		case CSwordModuleInfo::Commentary:
			return new CSwordVerseKey( (VerseKey *) ( (SWKey *)(*module->module()) ), module );
		case CSwordModuleInfo::Lexicon:
			return new CSwordLDKey( (SWKey *)(*module->module()), module);
		case CSwordModuleInfo::GenericBook:
			CSwordTreeKey* key =  new CSwordTreeKey( (TreeKeyIdx*)((SWKey *)(*module->module())), module );
			qWarning("Persist? %s", key->Persist() ? "yes" : "no");
			return key;
	}
	return 0;
}
