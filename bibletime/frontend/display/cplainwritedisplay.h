/***************************************************************************
                          cplainwritedisplay.h  -  description
                             -------------------
    begin                : Fre Okt 11 2002
    copyright            : (C) 2002 by The BibleTime team
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

#ifndef CPLAINWRITEDISPLAY_H
#define CPLAINWRITEDISPLAY_H

//Bibletime include files
#include "cwritedisplay.h"

//Qt includes
#include <qwidget.h>
#include <qtextedit.h>

/**The write display implementation for plain source code editing.
  *@author The BibleTime team
  */

class CPlainWriteDisplay : public QTextEdit, public CWriteDisplay  {
public: 
  /**
  * Reimplementation.
  */
  virtual void selectAll();
  /**
  * Sets the new text for this display widget.
  */
  virtual void setText( const QString& newText );
  /**
  * Returns true if the display widget has a selection. Otherwise false.
  */
  virtual const bool hasSelection();
  /**
  * Returns the view of this display widget.
  */
  virtual QWidget* view();
  virtual const QString text( const CDisplay::TextType format = CDisplay::HTMLText, const CDisplay::TextPart part = CDisplay::Document );
  virtual void print( const CDisplay::TextPart );

protected:
	friend class CDisplay;
	CPlainWriteDisplay(CWriteWindow* parentWindow, QWidget* parent);
	virtual ~CPlainWriteDisplay();
};

#endif
