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

CStyleList::CStyleList(styleItemList* items, QWidget *parent, const char *name ) : KListView(parent,name) {
	m_items = items;
	m_styleCombo = 0;
	initView();
	
	setItems(items);	
}

CStyleList::~CStyleList(){
	for (m_items->first(); m_items->current(); m_items->next()) {
		if (m_items->current())
			m_items->current()->deleteListViewItem();
	}
}

/** Initilizes the view. */
void CStyleList::initView(){
	addColumn(i18n("Name:"));
	header()->hide();
}

/** Sets the content to itemList */
void CStyleList::insertItems( styleItemList* itemList ){
	for(itemList->first(); itemList->current(); itemList->next() ) {
		if (itemList != m_items)	//check if we do not insert our own m_items list
			m_items->append( itemList->current() );
		QListViewItem* item = itemList->current()->getListViewItem(this);
		ASSERT(item);
	}
	updateStyleCombo();
}

/** Appends the items of itemList */
void CStyleList::setItems( styleItemList* itemList ){
//	clear();
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
	ASSERT(item->getListViewItem(this));
}

/** Opens the style editor dialog for he selected style. */
void CStyleList::editCurrentStyle(){
	QListViewItem*	item = currentItem();
	if (!item)
		return;
		
	//go through style items and find the right one
	CStyle*	currentStyle = 0;
	for (m_items->first(); m_items->current();m_items->next()) {
		if (m_items->current()->getListViewItem() == item) {
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
	if (!openStyleEditor( style ))
		delete style;
	else {
		insertItem( style );		
		style->updateListViewItem();
	}
}

/**  */
const bool CStyleList::openStyleEditor( CStyle* style ){
	CStyleEditorDialog* dlg = new CStyleEditorDialog(style, this);
	const int result = dlg->exec();
	delete dlg;	
		
	updateStyleCombo();
	
	return (result == CStyleEditorDialog::Ok);
}

/** Deletes the current style item. */
void CStyleList::deleteCurrentStyle(){
	QListViewItem* item = currentItem();
	if (!item)
		return;
	if (item->text(0) == i18n("Standard")) {
		qWarning("Deleting the standardstyle is not possible");
		return;
	}
	
	//go through style items and find the right one
	CStyle*	currentStyle = 0;
	for (m_items->first(); m_items->current();m_items->next()) {
		if (m_items->current()->getListViewItem() == item) {
			currentStyle = m_items->current();
			m_items->remove(currentStyle);
			currentStyle->deleteListViewItem();			
			if (!m_items->autoDelete() && currentStyle) {
				delete currentStyle;			
				currentStyle = 0;
			}	
			updateStyleCombo();					
			return;
		}
	}
}

/** Sets the combo box which contains the style names. */
void CStyleList::setStyleComboBox( QComboBox* combo ){
	m_styleCombo = combo;
	
	updateStyleCombo();
}

/** Updates the style combo box. */
void CStyleList::updateStyleCombo(){
	if (!m_styleCombo)
		return;
		
	m_styleCombo->clear();
	for (m_items->first(); m_items->current(); m_items->next()) {
		//insert name of style into combobox
		if (m_items->current())
			m_styleCombo->insertItem( m_items->current()->getStyleName() );	
	};
}

/** Returns a pointer to our list */
styleItemList* CStyleList::getStyleList(){
	return m_items;
}

