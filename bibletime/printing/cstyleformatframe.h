/***************************************************************************
                          cstyleformatframe.h  -  description
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

#ifndef CSTYLEFORMATFRAME_H
#define CSTYLEFORMATFRAME_H

#include <qobject.h>
#include <qcolor.h>

/** A class which represents the frame around different CStyleFormats
  * @author The BibleTime team
  */

class CStyleFormatFrame  {
public: 
	CStyleFormatFrame();
	~CStyleFormatFrame();
  /**
  * Sets the color of this frame.
  */
  void setColor( const QColor& );
  /**
  * Returns the color of this frame.
  */
  const QColor& getColor() const;
  /**
  * Sets the style how to paint the lines.
  */
  void setLineStyle( const Qt::PenStyle );
	/**
  * Returns the style how to paint the lines of this frame.
  */
  const Qt::PenStyle& getLineStyle() const;
  /**
  * Sets te thickness of this frame.
  */
  void setThickness( const unsigned short int );
  /**
  * Returns the thickness of this frame.
  */
  const unsigned short int getThickness() const;

protected:
	QColor m_color;
	Qt::PenStyle m_lineStyle;
	unsigned short int m_thickness;	
};

#endif
