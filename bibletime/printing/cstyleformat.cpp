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

#include <qcolor.h>

CStyleFormat::CStyleFormat(/*QObject* parent*/)/* :  QObject(parent)*/{
	qDebug("constructor of CStyleFormat");
	
	m_frame = 0;
	
	clearData();
}

CStyleFormat::~CStyleFormat(){
	qDebug("destructor of CStyleFormat");
	if (m_frame)
		delete m_frame;
}

/** Returns the font of this style. */
QFont& CStyleFormat::getFont() {
	qDebug("CStyleFormat::getFont()");
	return m_font;
}

/** sets the font of this format. */
void CStyleFormat::setFont( QFont newFont) {
	qDebug("CStyleFormat::setFont( QFont& newFont)");
	m_font = newFont;
}

/** Returns the foreground color of this format. */
QColor& CStyleFormat::getFGColor() {
	qDebug("CStyleFormat::getFGColor()");
	return m_FGColor;
}

/** Sets the background color of this format. */
void CStyleFormat::setFGColor( const QColor& newColor) {
	qDebug("CStyleFormat::setFGColor( const QColor& newColor)");
	m_FGColor = newColor;
}

/** Returns the background color of this format. */
QColor& CStyleFormat::getBGColor() {
	qDebug("CStyleFormat::getBGColor()");
	return m_BGColor;
}

/** Sets the foreground color of this format. */
void CStyleFormat::setBGColor( const QColor& newColor ) {
	qDebug("CStyleFormat::setBGColor( const QColor& newColor )");
	m_BGColor = newColor;
}

/** Returns true if this style has a frame for the given type. */
bool CStyleFormat::hasFrame() {
	return m_hasFrame;
}

/** Returns the frame if we have one. Otherwise we return 0. */
CStyleFormatFrame* CStyleFormat::getFrame(){
	qDebug("CStyleFormat::getFrame()");
	ASSERT(m_frame);
	if (!m_frame)
		m_frame = new CStyleFormatFrame();
	return m_frame;
}

/** Sets the frame of this style. */
void CStyleFormat::setFrame( bool haveFrame, CStyleFormatFrame* frame ) {
	qDebug("CStyleFormat::setFrame( bool haveFrame, CStyleFormatFrame* frame = 0)");
	ASSERT(frame);
	m_hasFrame = haveFrame;
	if (m_frame != frame)
		m_frame = frame;
}

/** Returns the alignement of this style format. */
CStyleFormat::alignement CStyleFormat::getAlignement(){
	qDebug("CStyleFormat::getAlignement()");
	return m_alignement;
}

/** Sets the alignement flags of this style format. */
void CStyleFormat::setAlignement( CStyleFormat::alignement newAlignement) {
	qDebug("CStyleFormat::setAlignement( CStyleFormat::alignement newAlignement)");
	m_alignement = newAlignement;
}

/** Returns the ident of this format */
int CStyleFormat::getIdentation(){
	qDebug("CStyleFormat::getIdentation()");
	return m_ident;
}

/** sets the identation of this format. */
void CStyleFormat::setIdentation( int newIdent) {
	qDebug("CStyleFormat::setIdentation( int newIdent)");
	m_ident = newIdent;
}

/** Resets the data variables. */
void CStyleFormat::clearData(){
	qDebug("CStyleFormat::clearData()");
	m_BGColor = Qt::white;
	m_FGColor = Qt::black;
	m_hasFrame = false;
	m_ident = 0;
}
