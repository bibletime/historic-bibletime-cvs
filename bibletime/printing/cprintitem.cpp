/***************************************************************************
                          cprintitem.cpp  -  description
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


#include "cprintitem.h"
#include "cprintitemlist.h"
#include "cstyle.h"
#include <../backend/cmoduleinfo.h>
#include <../backend/sword_backend/cswordmoduleinfo.h>
#include <../backend/ckey.h>
#include <../backend/sword_backend/cswordversekey.h>
#include <../backend/sword_backend/cswordldkey.h>
//#include <../backend/cdata.h>

//Qt includes
#include <qlistview.h>
#include <qregexp.h>

//Sword includes
#include <swmodule.h>
#include <swkey.h>
#include <versekey.h>

CPrintItem::CPrintItem() {
	m_listViewItem = 0;
	m_module = 0;
	m_style = 0;
	m_startKey = m_stopKey = 0;
	
	clearData();
}

CPrintItem::~CPrintItem(){
	if (m_startKey && m_startKey == m_stopKey)
		delete m_startKey;
	else {
		if (m_startKey) {
			delete m_startKey;
			m_startKey = 0;
		}
		if (m_stopKey) {
			delete m_stopKey;
			m_stopKey = 0;		
		}
	}
}

/** Returns the first key covered by this entry. */
CKey* CPrintItem::getStartKey() const{
	return m_startKey;
}

/** Sets the startkey. */
void CPrintItem::setStartKey(CKey* newKey) {
	m_startKey = newKey;

	if ( dynamic_cast<SWKey*>(m_startKey) ) {
		SWKey* swStartKey = dynamic_cast<SWKey*>(m_startKey);
		if (dynamic_cast<SWKey*>(m_stopKey)) {
			if (m_startKey == m_stopKey) {
				m_headerText = QString::fromLocal8Bit( (const char*)*swStartKey );
			}
			else {	//start and stop key do exist and are different
				SWKey* swStopKey = dynamic_cast<SWKey*>(m_stopKey);
				m_headerText = QString("%1 - %2").arg(QString::fromLocal8Bit((const char*)*swStartKey)).arg(QString::fromLocal8Bit((const char*)*swStopKey));
			}
		}
	}
	else
		m_headerText = QString::null;			
}

/** Sets the end key. */
void CPrintItem::setStopKey( CKey* newKey ){
	m_stopKey = newKey;
	
	if ( dynamic_cast<SWKey*>(m_startKey) ) {
		SWKey* swStartKey = dynamic_cast<SWKey*>(m_startKey);
		if (dynamic_cast<SWKey*>(m_stopKey)) {
			if (m_startKey == m_stopKey) {
				m_headerText = QString::fromLocal8Bit( (const char*)*swStartKey );
			}
			else {	//start and stop key do exist and are different
				SWKey* swStopKey = dynamic_cast<SWKey*>(m_stopKey);
				m_headerText = QString("%1 - %2").arg(QString::fromLocal8Bit((const char*)*swStartKey)).arg(QString::fromLocal8Bit((const char*)*swStopKey));
			}
		}
	}
	else
		m_headerText = QString::null;	
}

/** Returns the last covered key. */
CKey* CPrintItem::getStopKey() const {
	return m_stopKey;
}

/** Returns the used module. */
CModuleInfo* CPrintItem::getModule() const {
	return m_module;
}

/** Sets the used module. */
void CPrintItem::setModule( CModuleInfo* newModule){
	m_module = newModule;
}

/** Returns the description. Only valid for inserted bookmarks. */
const QString& CPrintItem::getDescription() const {
	return m_description;
}

/** Sets the decsription. */
void CPrintItem::setDescription( const QString& newDescription ){
	m_description = newDescription;
}

/** Returns the moduletext used by this item. */
const QString CPrintItem::getModuleText() const {
	/** If a special text is set use the text.
		* If the moduleText variable is empty use the CModuleInfo
		* object to retrieve the text,
		*/
	if (!m_moduleText.isEmpty())
		return m_moduleText;
	#warning Todo: This function is incomplete. Implement for range between startKey and stopKey
	CSwordVerseKey* vk = dynamic_cast<CSwordVerseKey*>(m_startKey);
	CSwordLDKey* lk = dynamic_cast<CSwordLDKey*>(m_startKey);		
	QString text = vk ? vk->getStrippedText() : (lk ? lk->getStrippedText() : QString());		
	text.replace(QRegExp("<BR>\n",false), "\n");	
	text.replace(QRegExp("<BR>",false), "\n");
	text.replace(QRegExp("</p>",false), "\n");
	text.replace(QRegExp("<[a-z\c\d#=\"/\s^<^>]>",false), "");
	return text;
}

/** Sets the module text. */
void CPrintItem::setModuleText( const QString& newText ){
	m_moduleText = newText;
}

/** Returns the text of the header. */
const QString& CPrintItem::getHeader() const {
	return m_headerText;
}

/**  */
void CPrintItem::setHeader( const QString& newText){
	m_headerText = newText;
}

/** Sets the style for this item. */
void CPrintItem::setStyle( CStyle* newStyle ) {
	m_style = newStyle;
}

/** Returns the style used by this item. */
CStyle* CPrintItem::getStyle() const {
	return m_style;
}
/** Returns the listview item for this printitem. */
QListViewItem* CPrintItem::getListViewItem( CPrintItemList* list ) {
	deleteListViewItem();
	m_listViewItem = new QListViewItem( list );
	updateListViewItem();
	return m_listViewItem;
}

/** sets the variables back. */
void CPrintItem::clearData(){
	if (m_listViewItem) {
		delete m_listViewItem;
		m_listViewItem = 0;			
	}	
	setHeader(QString::null);
	setDescription(QString::null);
	setModuleText(QString::null);	
	setStyle(0);	
	setStartKey(0);
	setStopKey(0);
	setModule(0);
}

/** Updates the item. */
void CPrintItem::updateListViewItem(){
	if (getModule() && (CSwordModuleInfo*)(getModule()) ) {
		CSwordModuleInfo* swModule = (CSwordModuleInfo*)(getModule());
		m_listViewItem->setText(0, swModule->module()->Name() );
	}

	if (getStartKey() && dynamic_cast<SWKey*>(getStartKey())) {
		SWKey* key = dynamic_cast<SWKey*>(getStartKey());
		m_listViewItem->setText(1, (const char*)*key);
	}
	if (getStopKey() && dynamic_cast<SWKey*>(getStopKey())) {
		SWKey* key = dynamic_cast<SWKey*>(getStopKey());
		m_listViewItem->setText(2, (const char*)*key);
	}
	if (getStyle()) {
		m_listViewItem->setText(3, getStyle()->getStyleName() );
	}
}

/**  */
QListViewItem* CPrintItem::getListViewItem() const {
		return m_listViewItem;
}

/** Deletes the list view item. */
void CPrintItem::deleteListViewItem(){
	if (m_listViewItem)
		delete m_listViewItem;
	m_listViewItem = 0;
}
