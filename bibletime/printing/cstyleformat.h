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
	enum alignement { Left, Center, Right, Justification};
	
	CStyleFormat();
	~CStyleFormat();
  /**
 	* Sets the foreground color of this format.
 	*/
  void setFGColor( const QColor& );
  /**
 	* Returns the foreground color of this format.
 	*/
	const QColor& getFGColor() const;
  /**
 	* Sets the background color of this format.
 	*/
  void setBGColor( const QColor& );
  /**
 	* Returns the background color of this format.
 	*/
	const QColor& getBGColor() const;
  /**
  * sets the font of this format.
  */
  void setFont( QFont );
  /**
  * Returns the font of this style.
  */
  const QFont& getFont() const;
  /**
 	* sets the identation of this format.
 	*/
  void setIdentation( int ) ;
  /**
 	* Returns the ident of this format
 	*/
  const int getIdentation() const;
  /**
 	* Sets the alignement flags of this style format.
 	*/
  void setAlignement( CStyleFormat::alignement );
  /**
 	* Returns the alignement of this style format.
 	*/
  const CStyleFormat::alignement& getAlignement() const;
  /**
 	* Sets the frame of this style.
 	*/
  void setFrame( bool hasFrame, CStyleFormatFrame* frame = 0);
  /**
 	* Returns the frame if we have one. Otherwise we return 0.
 	*/
  CStyleFormatFrame* getFrame();
  /**
 	* Returns true if this style has a frame for the given type.
 	*/
  const bool& hasFrame() const;

private:
	QFont m_font;
	QColor m_FGColor;
	QColor m_BGColor;
	int m_ident;
	bool m_hasFrame;
	CStyleFormatFrame* m_frame;
	CStyleFormat::alignement m_alignement;
  void clearData();
};

#endif
