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

//CSwordTreeKey::CSwordTreeKey( CSwordModuleInfo* module ){
//	m_module = module;
//	if (m_module)
//		SWKey::operator = (m_module->module()->KeyText());
//}

CSwordTreeKey::CSwordTreeKey( TreeKey *k, CSwordModuleInfo* module ) : TreeKey(*k), CSwordKey() {
	m_module = module;
}


CSwordTreeKey::~CSwordTreeKey() {

}
