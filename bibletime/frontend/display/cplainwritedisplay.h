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

class CHTMLWriteDisplay;
class KAction;

/** The write display implementation for plain source code editing.
  * @author The BibleTime team
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
  /**
  * Reimplementation.
  */
  virtual const bool isModified() const;
  /**
  * Sets the current status of the edit widget.
  */
  virtual void setModified( const bool modified );
  /**
  * Returns the text of this edit widget.
  */
  virtual const QString plainText();
  /**
  * Creates the necessary action objects and puts them on the toolbar.
  */
  virtual void setupToolbar(KToolBar * bar, KActionCollection * actionCollection);
  
protected:
	friend class CDisplay;
 	friend class CHTMLWriteDisplay;
  
	CPlainWriteDisplay(CWriteWindow* parentWindow, QWidget* parent);
	virtual ~CPlainWriteDisplay();
  /**
  * Reimplementation from QTextEdit. Provides an popup menu for the given position.
  */
  virtual QPopupMenu* createPopupMenu( const QPoint& pos );
  /**
  * Reimplementation from QTextEdit. Provides an popup menu.
  */
  virtual QPopupMenu* createPopupMenu();

};

#endif
