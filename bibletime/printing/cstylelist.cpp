/***************************************************************************
                          cstylelist.cpp  -  description
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

#include "cstylelist.h"
#include "cstyleeditordialog.h"

#include <qheader.h>
#include <qlistview.h>
#include <qcombobox.h>

#include <klocale.h>
#include <kaction.h>
#include <kiconloader.h>

CStyleList::CStyleList(StyleItemList* items, QWidget *parent, const char *name )
	: KListView(parent,name), m_items(items)
{
	initView();	
	setItems(m_items);	
}

CStyleList::~CStyleList(){
	for (m_items->first(); m_items->current(); m_items->next()) {
		m_items->current()->deleteListViewItem();
	}
}

/** Initilizes the view. */
void CStyleList::initView(){
	addColumn(i18n("Name:"));
	header()->hide();
}

/** Sets the content to itemList */
void CStyleList::insertItems( StyleItemList* itemList ){
	for(itemList->first(); itemList->current(); itemList->next() ) {
		if (itemList != m_items)	//check if we do not insert our own m_items list
			m_items->append( itemList->current() );
		QListViewItem* item = itemList->current()->listViewItem(this);
	}
	printer()->emitStylesChanged();
}

/** Appends the items of itemList */
void CStyleList::setItems( StyleItemList* itemList ){
	for (m_items->first(); m_items->current(); m_items->next()) {
		if (m_items->current())
			m_items->current()->deleteListViewItem();
	}

	if (m_items != itemList)
		m_items->clear();
	insertItems(m_items);	

	QListViewItemIterator it( this );
	if (it.current())
		setCurrentItem(it.current());
	emit currentChanged(it.current());
}

/** Appends the item to the list and m_items. */
void CStyleList::insertItem( CStyle* item ){
	m_items->append( item );
	item->listViewItem(this);
}

/** Opens the style editor dialog for he selected style. */
void CStyleList::editCurrentStyle(){
	QListViewItem*	item = currentItem();
	if (!item)
		return;
		
	//go through style items and find the right one
	CStyle*	currentStyle = 0;
	for (m_items->first(); m_items->current();m_items->next()) {
		if (m_items->current()->listViewItem() == item) {
			currentStyle = m_items->current();
			break;
		}
	}
	if (currentStyle) {
		openStyleEditor( currentStyle );		
		currentStyle->updateListViewItem();
	}
}

/** Creates a new style item. */
void CStyleList::createNewStyle(){
	CStyle*	style = new CStyle();	
	insertItem( style );
	
	openStyleEditor( style );
	
	style->updateListViewItem();
}

/**  */
void CStyleList::openStyleEditor( CStyle* const style ){
	CStyleEditorDialog dlg(style, this);
	dlg.exec();
	printer()->emitStylesChanged();
}

/** Deletes the current style item. */
void CStyleList::deleteCurrentStyle(){
	QListViewItem* item = currentItem();
	if (!item)
		return;
	if (item == printer()->standardStyle()->listViewItem()) {
		qWarning("Deleting the standardstyle is not possible");
		return;
	}
	
	//go through style items and find the right one
	CStyle*	currentStyle = 0;
	for (m_items->first(); m_items->current();m_items->next()) {
		if (m_items->current()->listViewItem() == item) {
			currentStyle = m_items->current();
			m_items->remove(currentStyle);
			currentStyle->deleteListViewItem();			
			if (!m_items->autoDelete() && currentStyle) {
				delete currentStyle;			
				currentStyle = 0;
			}	
			if (printer())
				printer()->emitStylesChanged();
			return;
		}
	}
}

///** Updates the style combo box. */
//void CStyleList::updateStyleCombo(){
//	if (m_printer)
//		m_printer->emitStylesChanged();
//}

/** Returns a pointer to our list */
StyleItemList* const CStyleList::styleList() const {
	return m_items;
}

