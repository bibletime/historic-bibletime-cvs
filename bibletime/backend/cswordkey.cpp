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

//Sword includes
#include <swmodule.h>

CSwordKey::CSwordKey() {
	m_module = 0;	
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
	if (k) {
		m_module->module()->SetKey(k);	
	}
	
	switch (m_module->encoding()) {
		case QFont::Unicode:
			return QString::fromUtf8( (const char*)*(m_module->module()) );	
		default:
			return QString::fromLocal8Bit( (const char*)*(m_module->module()) );	
	}
}

const QString CSwordKey::strippedText() {
	if (!m_module)
		return QString::null;
	SWKey* k = dynamic_cast<SWKey*>(this);
	if (k)
		m_module->module()->SetKey(k);
		
	switch (m_module->encoding()) {
		case QFont::Unicode:
			return QString::fromUtf8( m_module->module()->StripText() );
		default:
			return QString::fromLocal8Bit( m_module->module()->StripText() );
	}
}
