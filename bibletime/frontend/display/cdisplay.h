/***************************************************************************
                          cdisplay.h  -  description
                             -------------------
    begin                : Don Mai 9 2002
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

#ifndef CDISPLAY_H
#define CDISPLAY_H

//BibleTime includes
#include "frontend/cpointers.h"

//Qt includes
#include <qobject.h>
#include <qstring.h>


class CDisplayConnections;
class CReadWindow;
class CWriteWindow;

class CDisplayWindow;
class CReadDisplay;
class CWriteDisplay;


/** The base class for all display widgets.
  * @author The BibleTime team
  */
class CDisplay : public CPointers {
public:
	static CReadDisplay* createReadInstance(CReadWindow* readWindow, QWidget* parent = 0);

	static CWriteDisplay* createWriteInstance(CWriteWindow* writeWindow);

  enum TextType {
    HTMLText, /* Used for HTML markup */
    PlainText /* Plain text without links etc. */
  };
  enum TextPart {
    Document, /* All text */
    SelectedText, /* Only the selected text */
    AnchorOnly,
    AnchorTextOnly,
    AnchorWithText
  };

  /**
  * Copies the given text with the specified format into the applications clipboard.
  */
  virtual const bool copy( const CDisplay::TextType format, const CDisplay::TextPart part );
  /**
  * Saves the given text with the specified format into the applications clipboard.
  */
  virtual const bool save( const CDisplay::TextType format, const CDisplay::TextPart part );

  //the pure virtual methods of this base class

  /** Returns the text in the given format.
  *
  */
  virtual const QString text( const CDisplay::TextType format = CDisplay::HTMLText, const CDisplay::TextPart part = CDisplay::Document ) = 0;
  /**
  * Sets the new text for this display widget.
  */
  virtual void setText( const QString& newText ) = 0;
  /**
  * Returns true if the display widget has a selection. Otherwise false.
  */
  virtual const bool hasSelection() = 0;
  /**
  * Returns the view of this display widget.
  */
  virtual QWidget* view() = 0;
  /**
	*	 Selects the document text.
	*/
  virtual void selectAll() = 0;
  /**
  * Returns the connections obect used for signas and slots.
  */
  CDisplayConnections* const connectionsProxy() const;
  /**
  * Returns the parent window used for this display widget.
  */
  CDisplayWindow* const parentWindow() const;
  virtual void print( const CDisplay::TextPart ) = 0;

protected:
  /**
  * Used when a reference was dropped onto the widget.
  */
  void emitReferenceDropped( const QString& reference );
  /**
  * Emits the signal which used when a reference was clicked.
  */
  void emitReferenceClicked( const QString& reference );

protected:
	CDisplay(CDisplayWindow* parent);
	virtual ~CDisplay();

private:
 	CDisplayWindow* m_parentWindow;
	CDisplayConnections* m_connections;  
};

class CDisplayConnections : public QObject {
	Q_OBJECT
public:	
	CDisplayConnections( CDisplay* parent );

public slots:
  /**
  * Reimplementation from CDisplay.
  */
  virtual void selectAll();
	void emitReferenceClicked( const QString& module, const QString& key);
	void emitReferenceDropped( const QString& key );

  void saveAsPlain();
  void saveAsHTML();		
  void saveAnchorWithText();

  void printAll();
  void printAnchorWithText();

  void copySelection();
  void copyAll();
  void copyAnchorWithText();
  void copyAnchorTextOnly();
  void copyAnchorOnly();

  void clear();

signals:
  void referenceClicked(const QString& module, const QString& key);
  void referenceDropped(const QString& key);

private:
	CDisplay* m_display;
};

#endif
