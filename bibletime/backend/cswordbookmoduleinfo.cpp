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

//Sword includes
#include <treekeyidx.h>
#include <treekey.h>

CSwordBookModuleInfo::CSwordBookModuleInfo( CSwordBackend* backend, SWModule* module )
	: CSwordModuleInfo(backend, module),
	m_depth(-1)
{
	//checking if it works as expected
	qWarning("CSwordBookModuleInfo: checking if it's valid");
	TreeKeyIdx* treeKey = getTree();
	if (treeKey) {
		TreeKeyIdx root = *treeKey;
  	root.root();
		printTree(root, treeKey, 0);
	}
	else
		qWarning("treeKey not valid");
	qWarning("depth: %i", depth());
}

CSwordBookModuleInfo::CSwordBookModuleInfo( const CSwordModuleInfo& m ) : CSwordModuleInfo(m){

}	

CSwordBookModuleInfo::~CSwordBookModuleInfo(){
}

void CSwordBookModuleInfo::printTree(TreeKeyIdx treeKey, TreeKeyIdx* target, int level ){
	qWarning("CSwordBookModuleInfo::printTree(TreeKeyIdx treeKey, TreeKeyIdx* target, int level )");
	if (!target)
		target = &treeKey;
	ASSERT(target);

  unsigned long currentOffset = target->getOffset();
  for (int i = 0; i < level; i++)
  	cout << "    ";
  cout << treeKey.getLocalName() << endl;
  if (treeKey.firstChild()) {
    printTree(treeKey, target, level+1);
    treeKey.parent();
  }
  if (treeKey.nextSibling())
		printTree(treeKey, target, level);
}

const int CSwordBookModuleInfo::depth() {
	if (m_depth == -1) {
  	TreeKeyIdx* treeKey = getTree();
  	ASSERT(treeKey);
  	if (treeKey) {
    	TreeKeyIdx root = *treeKey;
    	root.root();		
			computeDepth(root, treeKey, m_depth=0);
		}
	}
	return m_depth;
}

void CSwordBookModuleInfo::computeDepth(TreeKeyIdx treeKey, TreeKeyIdx* target, int& level ){
	qWarning("CSwordBookModuleInfo::printTree(TreeKeyIdx treeKey, TreeKeyIdx* target, int level )");
	if (!target)
		target = &treeKey;
	ASSERT(target);

  if (treeKey.firstChild()) {
    int newLevel = level+1;
    computeDepth(treeKey, target, newLevel);
    if (newLevel > level)
    	level = newLevel;
    treeKey.parent();
  }
  if (treeKey.nextSibling()) {
		int newLevel = level;
		computeDepth(treeKey, target, newLevel);
		if (newLevel > level)
			level = newLevel;
	}
}

/** Returns a treekey filled with the structure of this module */
TreeKeyIdx* CSwordBookModuleInfo::getTree(){
	TreeKeyIdx* treeKey = (TreeKeyIdx*)((SWKey*)*(module()));
	if (treeKey) {
		return treeKey;
	}
	return 0;
}
