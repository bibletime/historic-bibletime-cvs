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


//Qt includes
#include <qlistview.h>

//KDE includes
#include <klocale.h>

CStyle::CStyle() {
	m_headerFormat = new CStyleFormat();
	m_moduleTextFormat = new CStyleFormat();
	m_descriptionFormat = new CStyleFormat();
	m_listViewItem = 0;
	m_isHeaderFormatEnabled = m_isDescriptionFormatEnabled = m_isModuleTextFormatEnabled = true;
	
	setStyleName( i18n("unknown name") );
}

CStyle::~CStyle(){
	if (m_headerFormat)
		delete m_headerFormat;
	if (m_moduleTextFormat)
		delete m_moduleTextFormat;
	if (m_descriptionFormat)		
		delete m_descriptionFormat;
}

/** Returns the proper CStyleFormat for the given type. */
CStyleFormat* CStyle::formatForType( const CStyle::styleType type) const {
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
		default:
			break;
	}
}

/** Set the printing of the header (true enables it). */
const bool CStyle::hasFormatTypeEnabled( const CStyle::styleType type) const {	
//	qWarning("CStyle::hasFormatTypeEnabled( const CStyle::styleType type)");
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
void CStyle::setFormatTypeEnabled( const CStyle::styleType type, const bool setEnabled) {
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
QListViewItem* CStyle::listViewItem( CStyleList* list ){
	if (!list) {
		if (!m_listViewItem)
			return 0;
		else
			return m_listViewItem;
	}
	deleteListViewItem();
	m_listViewItem = new QListViewItem( list );
	updateListViewItem();	
	return m_listViewItem;
}

/** Returns the style name */
const QString& CStyle::styleName() const{
	return m_styleName;
}

/** Sets the name of the style. */
void CStyle::setStyleName( const QString name ){
//	qDebug("CStyle::setStyleName( const QString name)");
	m_styleName = name;
}

/** Clears all variables and sets them back */
void CStyle::clearData(){
//	qDebug("CStyle::clearData()");
	deleteListViewItem();
	m_styleName = QString::null;
	m_isDescriptionFormatEnabled = m_isHeaderFormatEnabled = m_isModuleTextFormatEnabled = true;		
}

/** Updates the Listview items */
void CStyle::updateListViewItem(){
	m_listViewItem->setText(0, styleName() );
}

/** Deletes the list view item. */
void CStyle::deleteListViewItem(){
//	if (m_listViewItem) {
		delete m_listViewItem;
//	}
	m_listViewItem = 0;
}
