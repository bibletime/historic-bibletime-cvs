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

//Qt includes
#include <qstring.h>

//KDE includes
#include <klocale.h>

CPrintItemList::CPrintItemList( ListCPrintItem* items,  QWidget *parent, const char *name ) : KListView(parent,name) {
	m_items = items;
	initView();
	
	insertItems(m_items);
}

CPrintItemList::~CPrintItemList(){
	for (m_items->first(); m_items->current(); m_items->next()) {
		if (m_items->current())
			m_items->current()->deleteListViewItem();
	}	
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
void CPrintItemList::insertItems( ListCPrintItem* itemList ) {
	const bool sameList = (itemList == m_items);
	for(itemList->last(); itemList->current(); itemList->prev() ) {
		if (!sameList)
			m_items->append(itemList->current());
		itemList->current()->listViewItem(this);	//insert the QListViewItem into the list
	}
}

/** Sets the tree to contain the items of the list. Other items will be removed. */
void CPrintItemList::setItems( ListCPrintItem* itemList ){
	for (m_items->first(); m_items->current(); m_items->next()) {
		if (m_items->current())
			m_items->current()->deleteListViewItem();
	}
	insertItems( itemList );
}

/** Deletes the current item. */
void CPrintItemList::deleteSelectedItems(){
	QPtrList<QListViewItem> items = selectedItems();	
	if (!items.count())
		return;
	const bool autoDelete = items.autoDelete();
	
	CPrintItem* p = 0;
	for (items.first(); items.current(); items.next()) {
		if ( CPrintItem::ListViewItem* item = dynamic_cast<CPrintItem::ListViewItem*>(items.current()) ) {
			if (!(p = item->printItem()))
				continue;
			if (int pos = m_items->findRef(p))
        m_items->take(pos);
			p->deleteListViewItem();
			if (!autoDelete)
				delete p;
		}
	}
}

/** Moves the item one item up. */
void CPrintItemList::moveUp(){
	QPtrList<QListViewItem> items = selectedItems();
	for (items.first(); items.current(); items.next()) {
		QListViewItem* current = items.current();
		if (current && current->itemAbove() &&  current->itemAbove()->itemAbove()) {
			current->moveItem( current->itemAbove()->itemAbove() );
		}
	}
}

/** Moves the selected entry one entry down. */
void CPrintItemList::moveDown(){
	QPtrList<QListViewItem> items = selectedItems();
	for (items.last(); items.current(); items.prev()) {
		QListViewItem* current = items.current();
		if (current && current->itemBelow()) {
			current->moveItem( current->itemBelow() );
		}
	}
}

/** Applies the style to the selected items. */
void CPrintItemList::applyStyleToSelected( CStyle* style){
	CPrintItem* printItem;
	CPrintItem::ListViewItem* item;
	QPtrList<QListViewItem> list = selectedItems();
	for ( QListViewItem* i = list.first(); i; i = list.next()) {
		if ((item = dynamic_cast<CPrintItem::ListViewItem*>(i))) {
			printItem = item->printItem();
			printItem->setStyle(style);
			printItem->updateListViewItem();
		}
	}
}

