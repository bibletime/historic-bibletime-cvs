/***************************************************************************
                          cstyle.cpp  -  description
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

#include "cstyle.h"

#include "cstyleformat.h"
#include "cstylelist.h"
#include "../backend/ckey.h"
#include "../backend/cmoduleinfo.h"

#include <klocale.h>

CStyle::CStyle() {
	qDebug("constructor of CStyle");
	
	m_descriptionFormat = m_headerFormat = m_moduleTextFormat = 0;
	m_listViewItem = 0;
	
	clearData();
	m_headerFormat = new CStyleFormat();
	m_moduleTextFormat = new CStyleFormat();
	m_descriptionFormat = new CStyleFormat();
	
	m_isHeaderFormatEnabled = m_isDescriptionFormatEnabled = m_isModuleTextFormatEnabled = true;
	
	setStyleName( i18n("unknown name") );
}

CStyle::~CStyle(){
	qDebug("CStyle::~CStyle()");
	delete m_headerFormat;
	delete m_moduleTextFormat;
	delete m_descriptionFormat;
}

/** Returns the proper CStyleFormat for the given type. */
CStyleFormat* CStyle::getFormatForType( const CStyle::styleType type) {
	qDebug("CStyle::getFormatForType( const CStyle::styleType type)");
	switch (type) {
		case Header:
			return m_headerFormat;
		case ModuleText:
			return m_moduleTextFormat;
		case Description:
			return m_descriptionFormat;
		default:
			return 0;
	}
}

/** Sets the format for the given type. */
void CStyle::setFormatForType( const CStyle::styleType type, CStyleFormat* format){
	qDebug("CStyle::setFormatForType( const CStyle::styleType type, CStyleFormat* format)");
	switch (type) {
		case Header:
			m_headerFormat = format;
			break;
		case ModuleText:
			m_moduleTextFormat = format;
			break;
		case Description:
			m_descriptionFormat = format;
			break;
	}
}

/** Set the printing of the header (true enables it). */
bool CStyle::hasFormatTypeEnabled( CStyle::styleType type) {
	qDebug("CStyle::hasFormatTypeEnabled( CStyle::styleType type)");
	switch (type) {
		case Header:
			return m_isHeaderFormatEnabled;
		case ModuleText:
			return m_isModuleTextFormatEnabled;
		case Description:
			return m_isModuleTextFormatEnabled;		
		default:
			return false;
	}
}

/** Set the printing of the header (true enables it). */
void CStyle::setFormatTypeEnabled( CStyle::styleType type, bool setEnabled) {
	qDebug("CStyle::setFormatTypeEnabled( CStyle::styleType type, bool setEnabled)");
	switch (type) {
		case Header:
			m_isHeaderFormatEnabled = setEnabled;
			break;
		case ModuleText:
			m_isModuleTextFormatEnabled = setEnabled;		
			break;
		case Description:
			m_isDescriptionFormatEnabled = setEnabled;		
			break;
		default:
			break;
	}
}

/** Returns a QListViewItem for inserted in list. */
QListViewItem* CStyle::getListViewItem( CStyleList* list ){
	qDebug("CStyle::getListViewItem( CStyleList* list )");
	ASSERT(m_listViewItem);
	ASSERT(list);
	deleteListViewItem();
//	if (m_listViewItem) {
//		qDebug("delete item");
//		delete m_listViewItem;
//		qDebug("deleted item");		
//	}
	
	m_listViewItem = new QListViewItem( list );
//	qDebug("created item");
//qDebug( getStyleName() );
	ASSERT( m_listViewItem );
	updateListViewItem();
	
	return m_listViewItem;
}
/** Returns the style name */
QString CStyle::getStyleName() const{
	qDebug("CStyle::getStyleName()");
	return m_styleName;
}

/** Sets the name of the style. */
void CStyle::setStyleName( const QString& name){
	qDebug("CStyle::setStyleName( const QString& name)");
	m_styleName = name;
}

/** Clears all variables and sets them back */
void CStyle::clearData(){
	qDebug("CStyle::clearData()");	
	if (m_listViewItem) {
		delete m_listViewItem;
		m_listViewItem = 0;
	}
//	if (m_headerFormat) {
//		delete m_headerFormat;
//		m_headerFormat = 0;
//	}
//	if (m_moduleTextFormat) {
//		delete m_moduleTextFormat ;
//		m_moduleTextFormat = 0;
//	}
//	if (m_descriptionFormat) {
//		delete m_descriptionFormat;
//		m_descriptionFormat = 0;
//	}
	setStyleName(QString::null);
	m_isDescriptionFormatEnabled = m_isHeaderFormatEnabled = m_isModuleTextFormatEnabled = true;		
}

/** Returns  listviewitem of this style, if it's not created already return 0. */
QListViewItem* CStyle::getListViewItem(){
	qDebug("CStyle::getListViewItem()");
	if (!m_listViewItem)
		return 0;
	else
		return m_listViewItem;
}

/** Updates the Listview items */
void CStyle::updateListViewItem(){
	qDebug("CStyle::updateListViewItem()");
	m_listViewItem->setText(0, getStyleName() );
}

/** Deletes the list view item. */
void CStyle::deleteListViewItem(){
	qDebug("CStyle::deleteListViewItem()");
	if (m_listViewItem) {
		delete m_listViewItem;
	}
	m_listViewItem = 0;
}
