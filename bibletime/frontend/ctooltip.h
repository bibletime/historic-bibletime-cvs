/***************************************************************************
                          ctooltip.h  -  description
                             -------------------
    begin                : Don Mai 30 2002
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

#ifndef CTOOLTIP_H
#define CTOOLTIP_H

#include <qwidget.h>
#include <qframe.h>

//KDE includes
#include <khtml_part.h>
#include <khtmlview.h>

/**Our own ToolTip widget which uses KHTMLView to display the tooltip.
  *@author The BibleTime team
  */

class CToolTip : public QFrame  {
   Q_OBJECT
public: 
	CToolTip(QWidget *parent=0, const char *name=0);
//	virtual ~CToolTip();
  /**
  * Returns the widget this tooltip applies to. This tooltip widget is destroyed when he parent is deleted.
  */
  QWidget* const parentWidget() const;

protected: // Protected methods
  /**
  * This function shows a tip with the given text.
  * The tooltip will open at the position p on the screen. When the mouse moves out of
  * the rectangle rect the tooltip will disappear.
  * The point p must be in the rectangle rect, otherwise the behaviour of CToolTip will be incorrect!
  */
  void tip( const QPoint& p, const QRect& rect, const QString& text );
  /**
  * This function is called when a tooltip may be displayed at position p.
  * Reimplement this function to manage the appearance of tooltips.
  */
  virtual void maybeTip( const QPoint& p ) = 0;

private:
  /**
  * Reimplementation.
  */
  virtual void timerEvent( QTimerEvent* e );
  /**
  * Sets the tooltip on or off.
  */
  void setFilter( const bool enable );
  /**
  * Reimplementation.
  */
  virtual bool eventFilter( QObject *, QEvent *e );
  /**
  * A helper function which returns true if the given widget contains the global pos p.
  */
  const bool widgetContainsPoint( QWidget* const w, const QPoint& p );

  bool m_filter;
  QWidget* m_parentWidget;
  KHTMLPart* m_display;
  QRect m_tipRect;

private slots: // Private slots
  void destroyObject();
};

/** Returns the widget this tooltip applies to. This tooltip widget is destroyed when he parent is deleted. */
inline QWidget* const CToolTip::parentWidget() const{
  return m_parentWidget;
}

#endif
