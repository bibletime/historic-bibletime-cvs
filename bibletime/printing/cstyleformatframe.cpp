/***************************************************************************
                          cstyleformatframe.cpp  -  description
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

#include "cstyleformatframe.h"

CStyleFormatFrame::CStyleFormatFrame() {
	qDebug("constructor of CStyleFormatFrame");
	m_thickness = 1;
	m_color = Qt::black;
}

CStyleFormatFrame::~CStyleFormatFrame(){
	qDebug("destructor of CStyleFormatFrame");
}

/** Returns the thickness of this frame. */
unsigned short int CStyleFormatFrame::getThickness() {
	qDebug("CStyleFormatFrame::getThickness()");
	return m_thickness;
}

/** Sets te thickness of this frame. */
void CStyleFormatFrame::setThickness( unsigned short int newThickness ) {
	qDebug("CStyleFormatFrame::setThickness( unsigned short int newThickness )");
	m_thickness = newThickness;
}

/** Returns the style how to paint the lines of this frame. */
Qt::PenStyle CStyleFormatFrame::getLineStyle() {
	qDebug("CStyleFormatFrame::getLineStyle()");
	return m_lineStyle;
}

/** Sets the style how to paint the lines. */
void CStyleFormatFrame::setLineStyle( Qt::PenStyle newLineStyle ){
	qDebug("CStyleFormatFrame::setLineStyle( Qt::PenStyle newLineStyle )");
	m_lineStyle = newLineStyle;
}

/** Returns the color of this frame. */
QColor& CStyleFormatFrame::getColor() {
	qDebug("CStyleFormatFrame::getColor()");
	return m_color;
}

/** Sets the color of this frame. */
void CStyleFormatFrame::setColor( const QColor& newColor ) {
	qDebug("CStyleFormatFrame::setColor( const QColor& newColor )");
	m_color = newColor;
}
