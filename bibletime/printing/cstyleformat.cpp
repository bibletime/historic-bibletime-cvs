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

CStyleFormat::CStyleFormat() : m_frame(0) {
	clearData();
}

CStyleFormat::~CStyleFormat(){
	delete m_frame;
}

/** Returns the font of this style. */
const QFont& CStyleFormat::getFont() const {
	return m_font;
}

/** sets the font of this format. */
void CStyleFormat::setFont( QFont newFont) {
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
CStyleFormatFrame* const CStyleFormat::frame() {
	if (m_hasFrame && !m_frame)
			return (m_frame = new CStyleFormatFrame());	
	return 0;
}

/** Sets the frame of this style. */
void CStyleFormat::setFrame( const bool haveFrame, CStyleFormatFrame* frame ) {
	m_hasFrame = haveFrame;
	delete m_frame;
	m_frame = frame;
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

