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

/** Inserts a page divider */
void CPrintItemList::newPage(){
}

/** Deletes the current item. */
void CPrintItemList::deleteSelectedItems(){
	QList<QListViewItem> items = selectedItems();	
	if (!items.count())
		return;
	const bool autoDelete = items.autoDelete();
	CPrintItem* p = 0;
	CPrintItem::ListViewItem* item = 0;
	for (items.first(); items.current(); items.next()) {
		item = dynamic_cast<CPrintItem::ListViewItem*>(items.current());
		if (item) {
			if (!(p = item->printItem()))
				continue;
			p->deleteListViewItem();
			m_items->removeRef(p);			
			if (!autoDelete)
				delete p;
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
	if (currentItem() && currentItem()->itemBelow())
		currentItem()->moveItem( currentItem()->itemBelow() );
}

/** Applies the style to the selected items. */
void CPrintItemList::applyStyleToSelected( CStyle* style){
	CPrintItem* printItem;
	CPrintItem::ListViewItem* item;
	QList<QListViewItem> list = selectedItems();
	for ( QListViewItem* i = list.first(); i; i = list.next()) {
		if ((item = dynamic_cast<CPrintItem::ListViewItem*>(i))) {
			printItem = item->printItem();
			printItem->setStyle(style);
			printItem->updateListViewItem();
		}
	}
}

