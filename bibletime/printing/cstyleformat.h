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

class CStyleFormatFrame;

/**	
	* A class which contains methods to support a format for the CStyle class.
  *	@author The BibleTime team
  * @version $Id$
  */

class CStyleFormat {
public: 
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
  const QFont& getFont() const;
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
  void setFrame( const bool hasFrame, CStyleFormatFrame* frame = 0);
  /**
 	* Returns the frame if we have one. Otherwise we return 0.
 	*/
  CStyleFormatFrame* const frame();

private:
  void clearData();
  	
	QFont m_font;
	QColor m_FGColor;
	QColor m_BGColor;
	bool m_hasFrame;
	CStyleFormatFrame* m_frame;
	CStyleFormat::Alignement m_alignement;
};

#endif
