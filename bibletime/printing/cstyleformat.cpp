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

#include <qapplication.h>
#include <qcolor.h>

CStyleFormat::CStyleFormat() : m_frame(0) {
	clearData();
}

CStyleFormat::~CStyleFormat(){
	delete m_frame;
}

/** Returns the font of this style. */
const QFont& CStyleFormat::font() const {
	return m_font;
}

/** sets the font of this format. */
void CStyleFormat::setFont( QFont newFont ) {
	m_font = newFont;
}

/** Returns the background color of this format. */
const QColor& CStyleFormat::color( const Color type ) const {
	switch (type) {
		case Foreground:
			return m_FGColor;
		case Background:
			return m_BGColor;		
	};
	return Qt::white; //shouldn't be executed
}

/** Sets the foreground color of this format. */
void CStyleFormat::setColor( const Color type, const QColor& newColor ) {
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
CStyleFormat::Frame* const CStyleFormat::frame() {
	if (m_hasFrame)
		return !m_frame ? (m_frame = new CStyleFormat::Frame) : m_frame;
	return 0;
}

/** Sets the frame of this style. */
void CStyleFormat::setFrameEnabled( const bool hasFrame ) {
	m_hasFrame = hasFrame;
	if (!m_hasFrame) {
		delete m_frame;
		m_frame = 0;
	};
}

/** Returns the alignement of this style format. */
const CStyleFormat::Alignement& CStyleFormat::alignement() const {
	return m_alignement;
}

/** Sets the alignement flags of this style format. */
void CStyleFormat::setAlignement( const CStyleFormat::Alignement newAlignement) {
	m_alignement = newAlignement;
}

/** Resets the data variables. */
void CStyleFormat::clearData(){
	m_BGColor = Qt::white;
	m_FGColor = Qt::black;	
	delete m_frame;
	m_frame = 0;	
	m_hasFrame = false;
	m_font = QApplication::font();
	m_alignement = CStyleFormat::Left;
}


CStyleFormat::Frame::Frame() {
	m_thickness = 1;
	m_color = Qt::black;
	m_lineStyle = Qt::DashLine;
}

/** Returns the thickness of this frame. */
const unsigned short int& CStyleFormat::Frame::thickness() const {
	return m_thickness;
}

/** Sets te thickness of this frame. */
void CStyleFormat::Frame::setThickness( const unsigned short int newThickness ) {
	m_thickness = newThickness;
}

/** Returns the style how to paint the lines of this frame. */
const Qt::PenStyle& CStyleFormat::Frame::lineStyle() const {
	return m_lineStyle;
}

/** Sets the style how to paint the lines. */
void CStyleFormat::Frame::setLineStyle( const Qt::PenStyle newLineStyle ){
	m_lineStyle = newLineStyle;
}

/** Returns the color of this frame. */
const QColor& CStyleFormat::Frame::color() const {
	return m_color;
}

/** Sets the color of this frame. */
void CStyleFormat::Frame::setColor( const QColor& newColor ) {
	m_color = newColor;
}
