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

#include "cprintitemlist.h"
#include <../backend/cmoduleinfo.h>
#include <../backend/ckey.h>
//#include <../ressource.h>

#include <klocale.h>
#include <qstring.h>

CPrintItemList::CPrintItemList( printItemList* items,  QWidget *parent, const char *name ) : KListView(parent,name) {
	qDebug("Constructor of CPrinItemList");
	ASSERT(items);
	m_items = items;
	initView();
	
	insertItems(m_items);
}

CPrintItemList::~CPrintItemList(){
	qDebug("CPrintItemList::~CPrintItemList()");
	clear();
}

/** Initializes the view. */
void CPrintItemList::initView(){
	qDebug("CPrintItemList::initView()");
	setAllColumnsShowFocus(true);
	addColumn(i18n("Module"),-1);
	addColumn(i18n("From"), -1);
	addColumn(i18n("To"), -1);	
	addColumn(i18n("Style"), -1);	
	
	setSelectionMode( QListView::Extended );
}

/** Inserts the items of the list into the tree. */
void CPrintItemList::insertItems( printItemList* itemList ){
	qDebug("CPrintItemList::insertItems( printItemList* itemList )");
	for(itemList->last(); itemList->current(); itemList->prev() ) {
		ASSERT( itemList->current() );
		if (itemList != m_items)
			m_items->append(itemList->current());
		ASSERT(itemList->current()->getListViewItem(this));	//insert the QListViewItem into the list
	}
}

/** Sets the tree to contain the items of the list. Other items will be removed. */
void CPrintItemList::setItems( printItemList* itemList ){
	qDebug("CPrintItemList::setItems( printItemList* itemList )");
	clear();
	insertItems( itemList );
}

/** Inserts a page divider */
void CPrintItemList::newPage(){
	qDebug("CPrintItemList::newPage()");
}

/** Deletes the current item. */
void CPrintItemList::deleteCurrentItem(){
	qDebug("CPrintItemList::deleteCurrentItem()");	
	if (!currentItem())
		return;
	
	QListViewItem* item = currentItem();
	//search item in list
	for (m_items->first(); m_items->current(); m_items->next() ) {
		if (m_items->current()->getListViewItem() == item) {
			CPrintItem*	dummyItem = m_items->current();			
			dummyItem->deleteListViewItem();
			m_items->remove(dummyItem);
			if (!m_items->autoDelete()) {
				delete dummyItem;			
				dummyItem = 0;
			}
			return;
		}
	}
}

/** Moves the item one item up. */
void CPrintItemList::moveUp(){
	qDebug("CPrintItemList::moveUp()");
	if (currentItem() && currentItem()->itemAbove() &&  currentItem()->itemAbove()->itemAbove()) {
		currentItem()->moveItem( currentItem()->itemAbove()->itemAbove() );
	}
//	else {
//		currentItem()->moveItem( 0 );
//	}
}

/** Moves the selected entry one entry down. */
void CPrintItemList::moveDown(){
	qDebug("CPrintItemList::moveDown()");
	if (currentItem() && currentItem()->itemBelow()) {
		currentItem()->moveItem( currentItem()->itemBelow() );
	}
}

/** Applies the style to the selected items. */
void CPrintItemList::applyStyleToSelected( CStyle* style){
	qDebug("CPrintItemList::applyStyleToSelected( CStyle* )");
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
	//deletes all listVieItems
	for (m_items->first(); m_items->current(); m_items->next()) {
		if (m_items->current())
			m_items->current()->deleteListViewItem();
	}
	KListView::clear();
}
