/***************************************************************************
                          ctextdisplay.h  -  description
                             -------------------
    begin                : Fre Mai 3 2002
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

#ifndef CTEXTDISPLAY_H
#define CTEXTDISPLAY_H

//BibleTime includes
#include "cpointers.h"

//Qt include files
#include <qtooltip.h>
#include <qwidget.h>

//KDE include files
#include <dom/dom_node.h>
#include <khtml_part.h>
#include <khtmlview.h>
#include <kparts/browserextension.h>

//forward class declarations
class QPopupMenu;

class CDisplayWidgetView;

/** The implementation which displays the text of the modules (chapters, entries, books ...)
  * @author The BibleTime team
  */
class CDisplayWidget : public KHTMLPart, public CPointers  {
   Q_OBJECT
public:
  enum TextType {
    AllText,
    SelectedText
  };
  enum TextFormat {
    HTMLText,
    PlainText
  };

	CDisplayWidget(QWidget *parent=0, const char *name=0);
	~CDisplayWidget();
  /**
  * Set the content of this display widget to the parameter "text".
  */
  void setText( const QString& text );
  /**
  * Returns the content of this display widget.
  */
  const QString text( const TextFormat format = CDisplayWidget::PlainText ) const;
  /**
  * Returns the view object.
  */
  virtual CDisplayWidgetView* view() const;
  /**
  * Copies the given type into the clipboard.
  */
  void copy( const TextType type );
  /**
  * Returns the node which was activated by a RMB.
  */
  const QString activeURLNode();
  /**
  * Returns true if the active URL node is valid and false if it's not valid.
  */
  const bool hasActiveURLNode() const;

public slots: // Public slots
  /**
  * Copies the selection into the clipboard.
  */
  void copySelection();
  /**
  * Copies the whole text into the clipboard.
  */
  void copyAll();
  /**
  * Selects all text.
  */
  void selectAll();
  /**
  * Saves the document as plain text.
  */
  void saveAsPlain();
  /**
  * Saves the document as HTML file.
  */
  void saveAsHTML();

protected: // Protected methods
  friend class CDisplayWidgetView;

  /**
  * Saves the document in the given format.
  */
  void save( const TextFormat format );
  /**
  * Reimplemented for current anchor methods.
  */
  virtual void khtmlMousePressEvent( khtml::MousePressEvent* event );
  /**
  * Reimplementation for our drag&drop system.
  */
  virtual void khtmlMouseMoveEvent( khtml::MouseMoveEvent* e );
  /**
  * Reimplemented for current anchor methods.
  */
  virtual void khtmlMouseReleaseEvent( khtml::MouseReleaseEvent* event );
  /**
  * Emits the correct signal when a ref was dropped on the widget.
  */
  void emitReferenceDropped( const QString& module, const QString& key );
  /**
  * Reimplementation.
  */
  virtual void urlSelected( const QString& url, int button, int state, const QString& _target, KParts::URLArgs args = KParts::URLArgs());

private:
  CDisplayWidgetView* m_view;
  DOM::DOMString m_activatedURLNode;

  struct DNDData {
    DOM::Node node;
    DOM::DOMString anchor;
    bool mousePressed;
    bool isDragging;
    QPoint startPos;
    enum { Link, Text } dragType;
  } m_dndData;

signals:
  void referenceDropped(const QString& module, const QString& key);
  void referenceClicked(const QString& module, const QString& key);
};

class CDisplayWidgetView : public KHTMLView, public CPointers {
  Q_OBJECT
public:
  class ToolTip : public QToolTip {
  public:
    ToolTip(CDisplayWidgetView* parent);
  protected: // Protected methods
    /**
    * Decides whether a tooltip should be shown.
    */
    virtual void maybeTip( const QPoint& );
  private:
    CDisplayWidgetView* m_view;
  };


  enum PopupType {
    Normal,
    Anchor
  };

  CDisplayWidgetView(CDisplayWidget* displayWidget, QWidget* parent);
  /**
  * Installs the popup menu which should occur when the right mouse button was pressed.
  * Set the parametzer to 0 if yu want to disable the popup.
  */
  void installPopup( const PopupType type, QPopupMenu* popup );

protected: // Protected methods
  /**
  * Reimplementation from QScrollView. Sets the right slots
  */
  virtual void polish();
  /**
  * Reimplementatiob from QScrollView.
  */
  virtual void contentsDropEvent( QDropEvent* );
  /**
  * Reimplementatiob from QScrollView.
  */
  virtual void contentsDragEnterEvent( QDragEnterEvent* );

protected slots: // Protected slots
  /**
  * Opens the popupmenu at the given position.
  */
  void popupMenu( const QString&, const QPoint& );

private:
  struct {
    QPopupMenu* anchor;
    QPopupMenu* normal;
  } m_popups;
  CDisplayWidget* m_displayWidget;
};


#endif
