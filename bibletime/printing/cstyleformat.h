/***************************************************************************
                          cstyleformat.h  -  description
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

#ifndef CSTYLEFORMAT_H
#define CSTYLEFORMAT_H

#include <qobject.h>
#include <qfont.h>
#include <qcolor.h>
#include <qguardedptr.h>

/**	
	* A class which contains methods to support a format for the CStyle class.
  *	@author The BibleTime team
  * @version $Id$
  */
class CStyleFormat {
public:
  class Frame  {
  public:
  	Frame();
    /**
    * Sets the color of this frame.
    */
    void setColor( const QColor& );
    /**
    * Returns the color of this frame.
    */
    const QColor& color() const;
    /**
    * Sets the style how to paint the lines.
    */
    void setLineStyle( const Qt::PenStyle );
  	/**
    * Returns the style how to paint the lines of this frame.
    */
    const Qt::PenStyle& lineStyle() const;
    /**
    * Sets te thickness of this frame.
    */
    void setThickness( const unsigned short int );
    /**
    * Returns the thickness of this frame.
    */
    const unsigned short int& thickness() const;

  protected:
  	QColor m_color;
  	Qt::PenStyle m_lineStyle;
  	unsigned short int m_thickness;	
  };

	enum Alignement { Left, Center, Right, Justification};
	enum Color { Background, Foreground };
	
	CStyleFormat();
	~CStyleFormat();

  /**
 	* Returns the foreground color of this format.
 	*/
	const QColor& color( const Color type ) const;
  /**
 	* Sets the background color of this format.
 	*/
  void setColor( const Color type,  const QColor& );
  /**
  * sets the font of this format.
  */
  void setFont( QFont );
  /**
  * Returns the font of this style.
  */
  const QFont& font() const;
  /**
 	* Sets the alignement flags of this style format.
 	*/
  void setAlignement( const CStyleFormat::Alignement );
  /**
 	* Returns the alignement of this style format.
 	*/
  const CStyleFormat::Alignement& alignement() const;
  /**
 	* Sets the frame of this style.
 	*/
  void setFrameEnabled( const bool hasFrame );
  /**
 	* Returns the frame if we have one. Otherwise we return 0.
 	*/
  Frame* const frame();

private:
  void clearData();
  	
	QFont m_font;
	QColor m_FGColor;
	QColor m_BGColor;
	bool m_hasFrame;
	Frame* m_frame;
	CStyleFormat::Alignement m_alignement;
};

#endif
