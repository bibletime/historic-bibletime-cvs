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
	m_htmlText = header();
	
	if (!module || !key) {
		return 0;
	}	
	int displayLevel = book->config( CSwordModuleInfo::DisplayLevel ).toInt();
	key->key(book->module()->KeyText());
  updateSettings();	
	if (displayLevel <= 1) {
	 	m_htmlText = key->renderedText();
	 	return 0;
	}	
	
  --displayLevel; //better handling if 1 is to concat the last level
  const QString oldKey = key->key();			
	int moved = 0;
	
	while (key->firstChild())
		++moved; //down

	for (int i = 1; i < displayLevel; i++) {
		if (!key->parent() || key->key() == "/" || key->key().isEmpty()) {
			break;		
		}
		--moved; //up
	};
	
	if (moved <= 1) { //display entries together
		while(key->previousSibling()); //first entry of it's parent		
		printTree(*key, book);	
		key->key(oldKey);
		return 1;	
	}
	else { //do not display entries together
		key->key(oldKey.isNull() ? "/" : oldKey);
		m_htmlText = key->renderedText();
		return 1;
	}	
}

void CHTMLBookDisplay::printTree(CSwordTreeKey treeKey, CSwordBookModuleInfo* module, const int levelPos){
	if (levelPos>0) {
		m_htmlText += QString::fromLatin1("<DIV STYLE=\"padding-left: %1px;\">").arg(levelPos*10+10);
	}

  m_htmlText += QString::fromLatin1("<A NAME=\"%1\" HREF=\"%2\">%3</A>: %4<BR>")
  	.arg(treeKey.getFullName())
  	.arg(CReferenceManager::encodeHyperlink(module->name(), treeKey.getFullName(), CReferenceManager::GenericBook))
  	.arg(treeKey.getLocalName())
  	.arg(treeKey.renderedText());  	

  if (treeKey.hasChildren()) {
//    m_htmlText += QString::fromLatin1("<H3>%1</H3>").arg(treeKey.getFullName());
    treeKey.firstChild();
    printTree(treeKey, module, levelPos+1);
    treeKey.parent();
  }

	if (levelPos>0)
		m_htmlText += QString::fromLatin1("</DIV>");

  if (treeKey.nextSibling())
		printTree(treeKey, module, levelPos);
}
