/***************************************************************************
                          chtmlbookdisplay.cpp  -  description
                             -------------------
    begin                : Mon Mar 11 2002
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

#include "chtmlbookdisplay.h"
#include "cswordbookmoduleinfo.h"
#include "cswordtreekey.h"
#include "creferencemanager.h"
#include "../util/scoped_resource.h"

char CHTMLBookDisplay::Display( CSwordModuleInfo* module ){
	CSwordBookModuleInfo* book = dynamic_cast<CSwordBookModuleInfo*>(module);
	util::scoped_ptr<CSwordTreeKey> key( dynamic_cast<CSwordTreeKey*>( CSwordKey::createInstance(book) ) );
	m_htmlText = QString::null;
	
	if (!module || !key) {		
		return 0;
	}	
	const int displayLevel = book->config( CSwordModuleInfo::DisplayLevel ).toInt();
	key->key(book->module()->KeyText());		
	updateSettings();	
	if (!displayLevel) {
		m_htmlText = key->renderedText();
		return 1;
	}
	
	bool ok = true;
	const QString oldKey = key->key();		
	while (key->firstChild()); //last entry
	for (int i = 1; i < displayLevel && ok; i++) {
		if (!key->parent())
			ok = false;
	};
	if (!ok) {
		return 0;
	}
	while(key->previousSibling());
	printTree(*key, book);
	return 1;
}

void CHTMLBookDisplay::printTree(CSwordTreeKey treeKey, CSwordBookModuleInfo* module){
  m_htmlText += QString::fromLatin1("<A NAME=\"%1\" HREF=\"%2\">%3</A>: %4")
  	.arg(treeKey.getLocalName())
  	.arg(CReferenceManager::encodeHyperlink(module->name(), treeKey.getFullName(), CReferenceManager::GenericBook))
  	.arg(treeKey.getLocalName())
  	.arg(treeKey.renderedText());  	
  	
  if (treeKey.firstChild()) {
    printTree(treeKey, module);
    treeKey.parent();
  }
  if (treeKey.nextSibling())
		printTree(treeKey, module);
}
