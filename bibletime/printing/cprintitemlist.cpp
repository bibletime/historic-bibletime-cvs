/***************************************************************************
                          cprintitemlist.cpp  -  description
                             -------------------
    begin                : Sun Aug 20 2000
    copyright            : (C) 2000 by The BibleTime team
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

//Own includes
#include "cprintitemlist.h"
//#include <../backend/cmoduleinfo.h>
//#include <../backend/ckey.h>

//Qt includes
#include <qstring.h>

//KDE includes
#include <klocale.h>

CPrintItemList::CPrintItemList( printItemList* items,  QWidget *parent, const char *name ) : KListView(parent,name) {
	qDebug("Constructor of CPrinItemList");
	m_items = items;
	initView();
	
	insertItems(m_items);
}

CPrintItemList::~CPrintItemList(){
	clear();
}

/** Initializes the view. */
void CPrintItemList::initView(){
	setAllColumnsShowFocus(true);
	addColumn(i18n("Module"),-1);
	addColumn(i18n("From"), -1);
	addColumn(i18n("To"), -1);	
	addColumn(i18n("Style"), -1);	
	
	setSelectionMode( QListView::Extended );
}

/** Inserts the items of the list into the tree. */
void CPrintItemList::insertItems( printItemList* itemList ) {
	for(itemList->last(); itemList->current(); itemList->prev() ) {
		if (itemList != m_items)
			m_items->append(itemList->current());
		itemList->current()->getListViewItem(this);	//insert the QListViewItem into the list
	}
}

/** Sets the tree to contain the items of the list. Other items will be removed. */
void CPrintItemList::setItems( printItemList* itemList ){
	clear();
	insertItems( itemList );
}

/** Inserts a page divider */
void CPrintItemList::newPage(){
}

/** Deletes the current item. */
void CPrintItemList::deleteSelectedItems(){
	QList<QListViewItem> items = selectedItems();
	if (!items.count())
		return;
	
	for (items.first(); items.current(); items.next()) {
		for (m_items->first(); m_items->current(); m_items->next() ) {
			if (m_items->current()->getListViewItem() == items.current()) {
				CPrintItem*	dummyItem = m_items->current();			
				dummyItem->deleteListViewItem();
				m_items->remove(dummyItem);
				if (!m_items->autoDelete()) {
					delete dummyItem;			
					dummyItem = 0;
					break;
				}
			}
		}	
	}		
}

/** Moves the item one item up. */
void CPrintItemList::moveUp(){
	if (currentItem() && currentItem()->itemAbove() &&  currentItem()->itemAbove()->itemAbove()) {
		currentItem()->moveItem( currentItem()->itemAbove()->itemAbove() );
	}
}

/** Moves the selected entry one entry down. */
void CPrintItemList::moveDown(){
	if (currentItem() && currentItem()->itemBelow()) {
		currentItem()->moveItem( currentItem()->itemBelow() );
	}
}

/** Applies the style to the selected items. */
void CPrintItemList::applyStyleToSelected( CStyle* style){
	QListViewItemIterator it( this );
	for ( ; it.current(); ++it) {
		if (it.current()->isSelected()) {
			//find right CPrintItem
			for ( m_items->first(); m_items->current(); m_items->next() ) {
				if ( m_items->current()->getListViewItem() == it.current() ) {
					//found right CPrintItem
					m_items->current()->setStyle(style);
					m_items->current()->updateListViewItem();
					break;
				}
			}
		}
	}
}

/** Reimplementation. */
void CPrintItemList::clear(){
	//deletes all listViewItems
	for (m_items->first(); m_items->current(); m_items->next()) {
		if (m_items->current())
			m_items->current()->deleteListViewItem();
	}
	KListView::clear();
}
