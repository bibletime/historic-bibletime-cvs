/***************************************************************************
                          cswordbookmoduleinfo.cpp  -  description
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

//BibleTime includes
#include "cswordbookmoduleinfo.h"
#include "cswordtreekey.h"

//Sword includes
#include <treekeyidx.h>
#include <treekey.h>

#include <iostream>
#include <string>
using std::cout;
using std::endl;

CSwordBookModuleInfo::CSwordBookModuleInfo( sword::SWModule* module, CSwordBackend* const usedBackend ) 
	: CSwordModuleInfo(module, usedBackend),
		m_depth(-1) 
{

}

CSwordBookModuleInfo::CSwordBookModuleInfo( const CSwordBookModuleInfo& module )
	: CSwordModuleInfo(module)
{
	m_depth = module.m_depth;
}	

CSwordBookModuleInfo::~CSwordBookModuleInfo(){
}

const int CSwordBookModuleInfo::depth() {
	if (m_depth == -1) {
  	sword::TreeKeyIdx* key = tree();
  	if (key) {
    	key->root();
			computeDepth(key, 0);
		}
	}
	return m_depth;
}

void CSwordBookModuleInfo::computeDepth(sword::TreeKeyIdx* key, int level ){
  std::string savedKey;
  savedKey = key->getFullName();
  if (level > m_depth) {
		m_depth = level;
  }

  if (key->hasChildren()) {
    key->firstChild();
    computeDepth(key, level+1);
    
    key->setText( savedKey.c_str() );//return to the initial value    
  }
  if (key->nextSibling()) {
		computeDepth(key, level);
  }
}

/** Returns a treekey filled with the structure of this module */
sword::TreeKeyIdx* const  CSwordBookModuleInfo::tree() const {
	sword::TreeKeyIdx* treeKey = dynamic_cast<sword::TreeKeyIdx*>((sword::SWKey*)*(module()));
  Q_ASSERT(treeKey);
	return treeKey;
}
