/***************************************************************************
                          cbooktreechooser.cpp  -  description
                             -------------------
    begin                : Sat Jan 26 2002
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

#include "cbooktreechooser.h"
#include "../../backend/cswordtreekey.h"
#include "../../backend/cswordbookmoduleinfo.h"

//Qt includes
#include <qlayout.h>
#include <qheader.h>
#include <qlistview.h>

////////////

CBookTreeChooser::TreeItem::TreeItem(QListViewItem* parent, QListViewItem* after, const QString caption, const QString key)
	: KListViewItem(parent, after, caption) {	
	m_key = key;
};

CBookTreeChooser::TreeItem::TreeItem(QListViewItem* parent,const QString caption, const QString key)
	: KListViewItem(parent, caption) {	
	m_key = key;
};

CBookTreeChooser::TreeItem::TreeItem(QListView* view, QListViewItem* after, const QString caption, const QString key)
	: KListViewItem(view,after, caption) {	
	m_key = key;
};

const QString& CBookTreeChooser::TreeItem::key() const {
	return m_key;
};	

////////////

CBookTreeChooser::CBookTreeChooser(CSwordModuleInfo *module, CSwordKey *key, QWidget *parent, const char *name)
	: CKeyChooser(module, key, parent,name) {
	if ( module && (module->type() == CSwordModuleInfo::GenericBook) ) {
		m_module = dynamic_cast<CSwordBookModuleInfo*>(module);		
		m_key = dynamic_cast<CSwordTreeKey*>(key);
	}
	else {
		m_module = 0;
		m_key = 0;
	}		

	//now setup the keychooser widgets
	QHBoxLayout* layout = new QHBoxLayout(this);
	m_treeView = new KListView(this);
	layout->addWidget(m_treeView);
	
	connect( m_treeView, SIGNAL(executed(QListViewItem*)), SLOT(itemClicked(QListViewItem*)));
	m_treeView->addColumn("Tree");	
	m_treeView->header()->resizeSection( 0,m_treeView->sizeHint().width());
	m_treeView->header()->setResizeEnabled(-1);
	m_treeView->header()->hide();
	m_treeView->setSorting(-1);
	m_treeView->setRootIsDecorated(true);
	
	setModule(module);
}

CBookTreeChooser::~CBookTreeChooser(){
}

/** Sets a new key to this keychooser */
void CBookTreeChooser::setKey(CSwordKey* newKey){
}

/** Sets a new key to this keychooser */
void CBookTreeChooser::setKey(CSwordKey* newKey, const bool emitSignal){
	if (m_key != newKey )
		m_key = dynamic_cast<CSwordTreeKey*>(newKey);
	
	const QString key = m_key->key();
	
	QStringList siblings;
	if (m_key && !key.isEmpty())
		siblings = QStringList::split("/",key,false);
	
	//find the right listview item
	QListViewItem* parent = 0;
	const int count = siblings.count();
	int index = 0;
	QString currentSibling = siblings[index];
	
  QListViewItem* child = m_treeView->firstChild();
  while( child && index < count ) {
//  	qWarning("trying to find %s", currentSibling.latin1() );

  	if (child->text(0) == currentSibling) { //found parent of our item
  		//found right entry?
  		TreeItem* i = dynamic_cast<TreeItem*>(child);
  		if (!i || i->key() == key) {
//  			qWarning("found!");
  			break;
  		}
  		child = child->firstChild();
  		currentSibling = siblings[++index];
  	}
  	else
			child = child->nextSibling();
  }
	
  m_treeView->setCurrentItem( child );
  m_treeView->ensureItemVisible(child);
	if (emitSignal)
		emit keyChanged(m_key);
}

/** Returns the key of this kechooser. */
CSwordKey* CBookTreeChooser::key(){
	return m_key;
}

/** Sets another module to this keychooser */
void CBookTreeChooser::setModule(CSwordModuleInfo* module){
	m_module = dynamic_cast<CSwordBookModuleInfo*>(module);
	
	if (m_module && m_key) {
		m_treeView->clear();
	
		m_key->root();
		m_key->firstChild();
		setupTree(0,0,m_key);
		
		updateKey(m_key);
	}
}

/** Refreshes the content. */
void CBookTreeChooser::refreshContent(){
	if (m_key)
		updateKey( m_key ); //refresh with current key
}

/** Set up the tree with the current level of key. */
void CBookTreeChooser::setupTree( QListViewItem* parent, QListViewItem* after, CSwordTreeKey* key ){
//	ASSERT(key);
	QListViewItem* item = 0;
	if (parent)
		item = new TreeItem(parent, after, key->getLocalName(), key->key());
	else
		item = new TreeItem(m_treeView, after, key->getLocalName(), key->key());
	
	if (key->firstChild()) {
		setupTree(item, 0, key);
		key->parent();
	}
	if (key->nextSibling())
		setupTree(parent, item, key);
}

/** No descriptions */
void CBookTreeChooser::itemClicked( QListViewItem* item ){
	TreeItem* i = dynamic_cast<TreeItem*>(item);
	ASSERT(i);
	if (!i)
		return;

	const QString key = i->key();
	qWarning(key.latin1());
	m_key->key(key);
	
	emit keyChanged(m_key);
}

/** No descriptions */
void CBookTreeChooser::updateKey( CSwordKey* key ){	
	setKey(key, false);
}
