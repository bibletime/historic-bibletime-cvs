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
#include "backend/cswordtreekey.h"
#include "backend/cswordbookmoduleinfo.h"
#include "frontend/cbtconfig.h"

//Qt includes
#include <qlayout.h>
#include <qheader.h>
#include <qlistview.h>
#include <qcursor.h>

//KDE includes
#include <kapplication.h>

////////////

CBookTreeChooser::TreeItem::TreeItem(QListViewItem* parent, QListViewItem* after, CSwordTreeKey* key, const QString keyName)
	: KListViewItem(parent, after), m_key(key), m_keyName(keyName) {	
  m_key->key(m_keyName);
  setText(0, QString::fromLatin1(m_key->getLocalName()) );
};

CBookTreeChooser::TreeItem::TreeItem(QListViewItem* parent,CSwordTreeKey* key, const QString keyName)
	: KListViewItem(parent), m_key(key), m_keyName(keyName) {	
  m_key->key(m_keyName);
  setText(0, QString::fromLatin1(m_key->getLocalName()) );

};

CBookTreeChooser::TreeItem::TreeItem(QListView* view, QListViewItem* after,CSwordTreeKey* key, const QString keyName)
	: KListViewItem(view,after), m_key(key), m_keyName(keyName) {	
  m_key->key(m_keyName);
  setText(0, QString::fromLatin1(m_key->getLocalName()) );
};

const QString& CBookTreeChooser::TreeItem::key() const {
	return m_keyName;
};	

/** Initializes this item with the correct caption. */
void CBookTreeChooser::TreeItem::setup() {
  m_key->key(m_keyName);
  setExpandable(m_key->hasChildren());

  KListViewItem::setup();
}

void CBookTreeChooser::TreeItem::createChilds() {
  m_key->key(m_keyName);
  m_key->firstChild();
  QListViewItem* oldItem = 0;
  do {
    if (oldItem)
      oldItem = new TreeItem(this, oldItem, m_key, m_key->key());
    else
      oldItem = new TreeItem(this, m_key, m_key->key());
  } while (m_key->nextSibling());
}


void CBookTreeChooser::TreeItem::setOpen(bool o) {
  //setup the tree under this item
  if ((!isOpen() && o) && childCount() == 0) {
    listView()->viewport()->setCursor(WaitCursor);
    createChilds();
    listView()->viewport()->unsetCursor();
  }
  KListViewItem::setOpen(o);
}

//////////////////////////////////

CBookTreeChooser::CBookTreeChooser(ListCSwordModuleInfo modules, CSwordKey *key, QWidget *parent, const char *name)
	: CKeyChooser(modules, key, parent,name) {
  setModules(modules, false);	
  m_key = dynamic_cast<CSwordTreeKey*>(key);
  if (!modules.count()) {
		m_modules.clear();
		m_key = 0;
	}

	//now setup the keychooser widgets
	QHBoxLayout* layout = new QHBoxLayout(this);
	
  m_treeView = new KListView(this);
	layout->addWidget(m_treeView);	
//	connect( m_treeView, SIGNAL(executed(QListViewItem*)), SLOT(itemClicked(QListViewItem*)));
	connect( m_treeView, SIGNAL(selectionChanged(QListViewItem*)), SLOT(itemClicked(QListViewItem*)));  
	m_treeView->addColumn("Tree");	
	m_treeView->header()->resizeSection( 0,m_treeView->sizeHint().width());
	m_treeView->header()->setResizeEnabled(-1);
	m_treeView->header()->hide();
	m_treeView->setSorting(-1);
	m_treeView->setRootIsDecorated(true);
  m_treeView->setFullWidth(true);

  //Make sure the entries are displayed correctly.
	if ( m_modules.first()->isUnicode() ){
		m_treeView->setFont( CBTConfig::get(CBTConfig::unicode) );
		qWarning("Unicode Book detected");
	}
	else{
		m_treeView->setFont( CBTConfig::get(CBTConfig::standard) );
		qWarning("Standard Book detected");
	}

//  setModules(modules); //fill the tree
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
	const int count = siblings.count();
	int index = 0;
	QString currentSibling = siblings[index];
	
  QListViewItem* child = m_treeView->firstChild();
  while( child && index < count ) {
  	if (child->text(0) == currentSibling) { //found a parent of our item
  		//found right entry?
  		TreeItem* i = dynamic_cast<TreeItem*>(child);
  		if (!i || i->key() == key) {
  			break;
  		}
  		i->setOpen(true); //automatically creates childs
      child = i->firstChild();
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
CSwordKey* const CBookTreeChooser::key(){
	return m_key;
}

/** Sets another module to this keychooser */
void CBookTreeChooser::setModules(ListCSwordModuleInfo modules, const bool refresh){
  m_modules.clear();
  for (modules.first(); modules.current(); modules.next()) {
    if (CSwordBookModuleInfo* book = dynamic_cast<CSwordBookModuleInfo*>(modules.current())) {
      m_modules.append(book);
    }
  }
	
	if (refresh && m_modules.count() && m_key) {
    setupTree();
//		m_treeView->clear();
//	
//    const QString oldKey = m_key->key();
//		m_key->root();
//		m_key->firstChild();
//		setupTree(0,0,m_key);
//
//    m_key->key(oldKey);
//		
//		updateKey(m_key);

    //Make sure the entries are displayed correctly.
  	if ( m_modules.first()->isUnicode() ){
  		m_treeView->setFont( CBTConfig::get(CBTConfig::unicode) );
			qWarning("Unicode Book detected");
  	}
  	else{
  		m_treeView->setFont( CBTConfig::get(CBTConfig::standard) );
			qWarning("Standard Book detected");
  	}
	}
}

/** Refreshes the content. */
void CBookTreeChooser::refreshContent(){
	if (m_key)
		updateKey( m_key ); //refresh with current key
}

/** Set up the tree with the current level of key. */
void CBookTreeChooser::setupTree( QListViewItem* parent, QListViewItem* after, CSwordTreeKey* key ){
	QListViewItem* item = 0;
	if (parent)
		item = new TreeItem(parent, after, key, key->key());
	else
		item = new TreeItem(m_treeView, after, key, key->key());
	
//	if (key->firstChild()) {
//		setupTree(item, 0, key);
//		key->parent();
//	}
	if (key->nextSibling())
		setupTree(parent, item, key);
}

/** No descriptions */
void CBookTreeChooser::itemClicked( QListViewItem* item ){
	TreeItem* i = dynamic_cast<TreeItem*>(item);
	if (!i)
		return;
	m_key->key(i->key());	
	emit keyChanged(m_key);
}

/** No descriptions */
void CBookTreeChooser::updateKey( CSwordKey* key ){	
	setKey(key, false);
}

/** Reimplementationm to handle tree creation on show. */
void CBookTreeChooser::show(){
  CKeyChooser::show();
  if (!m_treeView->childCount()) {
    KApplication::setOverrideCursor(WaitCursor);
    setupTree(); //create the first level of the tree structure
    m_treeView->resize(m_treeView->sizeHint());
    KApplication::restoreOverrideCursor();
  }
}

/** Creates the first level of the tree structure. */
void CBookTreeChooser::setupTree(){
 	m_treeView->clear();
	
  const QString oldKey = m_key->key();
 	m_key->root();
 	m_key->firstChild();
 	setupTree(0,0,m_key);

  m_key->key(oldKey);
		
  updateKey(m_key);
}
