/***************************************************************************
                          chtmlwritedisplay.h  -  description
                             -------------------
    begin                : Fre Nov 29 2002
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

#ifndef CHTMLWRITEDISPLAY_H
#define CHTMLWRITEDISPLAY_H

//BibleTime includes
#include "cplainwritedisplay.h"

//Qt includes
#include <qwidget.h>
#include <qtextedit.h>

class CWriteWindow;

class KToggleAction;
class KFontAction;
class KFontSizeAction;

class KColorButton;

/** The WYSIWYG implementation of the write display interface.
  * @author The BibleTime team
  */
class CHTMLWriteDisplay : public CPlainWriteDisplay  {
   Q_OBJECT
public: 
  /**
  * Reimplementation.
  */
//  virtual void selectAll();
  /**
  * Sets the new text for this display widget.
  */
//  virtual void setText( const QString& newText );
  /**
  * Returns true if the display widget has a selection. Otherwise false.
  */
//  virtual const bool hasSelection();
  /**
  * Returns the view of this display widget.
  */
//  virtual QWidget* view();
//  virtual const QString text( const CDisplay::TextType format = CDisplay::HTMLText, const CDisplay::TextPart part = CDisplay::Document );
//  virtual void print( const CDisplay::TextPart );
  /**
  * Reimplementation.
  */
//  virtual const bool isModified() const;
  /**
  * Sets the current status of the edit widget.
  */
//  virtual void setModified( const bool modified );
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
	CHTMLWriteDisplay(CWriteWindow* parentWindow, QWidget* parent);
	~CHTMLWriteDisplay();  
//  /**
//  * Reimplementation from QTextEdit. Provides an popup menu for the given position.
//  */
//  virtual QPopupMenu* createPopupMenu( const QPoint& pos );
//  /**
//  * Reimplementation from QTextEdit. Provides an popup menu.
//  */
//  virtual QPopupMenu* createPopupMenu();

protected slots:
  void toggleBold();
  void toggleItalic();
  void toggleUnderline();

  void alignLeft();  
  void alignCenter();
  void alignRight();
//  void alignJustify();
        
  void slotFontChanged( const QFont& );  
  /**
  * The text's alignment changed. Enable the right buttons.
  */
  void slotAlignmentChanged( int );
  /**
  * Is called when a new color was selected.
  */
  void slotColorSelected( const QColor& );
  /**
  * Is called when a text with another color was selected.
  */
  void slotColorChanged( const QColor& );
        
private:
  struct {
    KAction* save;

    KToggleAction* bold;
    KToggleAction* italic;
    KToggleAction* underline;

    KToggleAction* alignLeft;
	  KToggleAction* alignCenter;
  	KToggleAction* alignRight;
   
    KFontAction* fontChooser;
    KFontSizeAction* fontSizeChooser;  
  } m_actions;
  KColorButton* m_colorButton;
};

#endif
