/***************************************************************************
                          cstyleformat.cpp  -  description
                             -------------------
    begin                : Mon Aug 21 2000
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

#include "cstyleformat.h"
#include "cstyleformatframe.h"

#include <qapplication.h>
#include <qcolor.h>

CStyleFormat::CStyleFormat() {
//	qDebug("constructor of CStyleFormat!");
	m_frame = 0;
	clearData();
}

CStyleFormat::~CStyleFormat(){
	clearData();
}

/** Returns the font of this style. */
const QFont& CStyleFormat::getFont() const {
	return m_font;
}

/** sets the font of this format. */
void CStyleFormat::setFont( QFont newFont) {
	m_font = newFont;
}

/** Returns the foreground color of this format. */
const QColor& CStyleFormat::getFGColor() const {
	return m_FGColor;
}

/** Sets the background color of this format. */
void CStyleFormat::setFGColor( const QColor& newColor) {
	m_FGColor = newColor;
}

/** Returns the background color of this format. */
const QColor& CStyleFormat::getBGColor() const {
	return m_BGColor;
}

/** Sets the foreground color of this format. */
void CStyleFormat::setBGColor( const QColor& newColor ) {
	m_BGColor = newColor;
}

/** Returns true if this style has a frame for the given type. */
const bool& CStyleFormat::hasFrame() const {
	return m_hasFrame;
}

/** Returns the frame if we have one. Otherwise we return 0. */
CStyleFormatFrame* CStyleFormat::getFrame() {
	if (!m_frame)
		m_frame = new CStyleFormatFrame();
	return m_frame;
}

/** Sets the frame of this style. */
void CStyleFormat::setFrame( bool haveFrame, CStyleFormatFrame* frame ) {
	m_hasFrame = haveFrame;
	if (m_frame != frame)
		m_frame = frame;
}

/** Returns the alignement of this style format. */
const CStyleFormat::alignement& CStyleFormat::getAlignement() const {
	return m_alignement;
}

/** Sets the alignement flags of this style format. */
void CStyleFormat::setAlignement( CStyleFormat::alignement newAlignement) {
	m_alignement = newAlignement;
}

/** Returns the ident of this format */
const int CStyleFormat::getIdentation() const {
	return m_ident;
}

/** sets the identation of this format. */
void CStyleFormat::setIdentation( int newIdent) {
	m_ident = newIdent;
}

/** Resets the data variables. */
void CStyleFormat::clearData(){
	m_BGColor = Qt::white;
	m_FGColor = Qt::black;	
	if (m_frame)
		delete m_frame;
	m_frame = 0;	
	m_hasFrame = false;
	m_font = QApplication::font();
	m_ident = 0;
	m_alignement = CStyleFormat::Left;
}

