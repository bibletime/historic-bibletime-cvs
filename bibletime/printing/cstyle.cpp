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
#include "cstylelist.h"


//Qt includes
#include <qlistview.h>
#include <qapp.h>

//KDE includes
#include <klocale.h>

CStyle::Format::Format() : m_frame(0) {
	clearData();
}

CStyle::Format::~Format(){
	delete m_frame;
}

/** Returns the font of this style. */
const QFont& CStyle::Format::font() const {
	return m_font;
}

/** sets the font of this format. */
void CStyle::Format::setFont( QFont newFont ) {
	m_font = newFont;
}

/** Returns the background color of this format. */
const QColor& CStyle::Format::color( const Color type ) const {
	switch (type) {
		case Foreground:
			return m_FGColor;
		case Background:
			return m_BGColor;		
	};
	return Qt::white; //shouldn't be executed
}

/** Sets the foreground color of this format. */
void CStyle::Format::setColor( const Color type, const QColor& newColor ) {
	switch (type) {
		case Foreground:
			m_FGColor = newColor;
			break;
		case Background:
			m_BGColor = newColor;
			break;
	};
}

/** Returns the frame if we have one. Otherwise we return 0. */
CStyle::Format::Frame* const CStyle::Format::frame() {
	if (m_hasFrame)
		return !m_frame ? (m_frame = new CStyle::Format::Frame) : m_frame;
	return 0;
}

/** Sets the frame of this style. */
void CStyle::Format::setFrameEnabled( const bool hasFrame ) {
	m_hasFrame = hasFrame;
	if (!m_hasFrame) {
		delete m_frame;
		m_frame = 0;
	};
}

/** Returns the alignement of this style format. */
const CStyle::Format::Alignement& CStyle::Format::alignement() const {
	return m_alignement;
}

/** Sets the alignement flags of this style format. */
void CStyle::Format::setAlignement( const CStyle::Format::Alignement newAlignement) {
	m_alignement = newAlignement;
}

/** Resets the data variables. */
void CStyle::Format::clearData(){
	m_BGColor = Qt::white;
	m_FGColor = Qt::black;	
	delete m_frame;
	m_frame = 0;	
	m_hasFrame = false;
	m_font = QApplication::font();
	m_alignement = CStyle::Format::Left;
}


CStyle::Format::Frame::Frame() {
	m_thickness = 1;
	m_color = Qt::black;
	m_lineStyle = Qt::DashLine;
}

/** Returns the thickness of this frame. */
const unsigned short int& CStyle::Format::Frame::thickness() const {
	return m_thickness;
}

/** Sets te thickness of this frame. */
void CStyle::Format::Frame::setThickness( const unsigned short int newThickness ) {
	m_thickness = newThickness;
}

/** Returns the style how to paint the lines of this frame. */
const Qt::PenStyle& CStyle::Format::Frame::lineStyle() const {
	return m_lineStyle;
}

/** Sets the style how to paint the lines. */
void CStyle::Format::Frame::setLineStyle( const Qt::PenStyle newLineStyle ){
	m_lineStyle = newLineStyle;
}

/** Returns the color of this frame. */
const QColor& CStyle::Format::Frame::color() const {
	return m_color;
}

/** Sets the color of this frame. */
void CStyle::Format::Frame::setColor( const QColor& newColor ) {
	m_color = newColor;
}

CStyle::CStyle() {
	m_headerFormat = new Format();
	m_moduleTextFormat = new Format();
	m_descriptionFormat = new Format();
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
CStyle::Format* CStyle::formatForType( const CStyle::styleType type) const {
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
void CStyle::setFormatForType( const CStyle::styleType type, CStyle::Format* format){
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
