/***************************************************************************
                          ctooltip.cpp  -  description
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

#include "ctooltip.h"

//Qt includes
#include <qobject.h>
#include <qobjectlist.h>
#include <qtooltip.h>
#include <qlayout.h>
#include <qcursor.h>

//KDE includes
#include <kapplication.h>
#include <dom/html_element.h>

CToolTip::CToolTip(QWidget *parent, const char *name ) : QFrame( 0, 0, WStyle_Customize | WStyle_NoBorder | WStyle_Tool | WStyle_StaysOnTop | WX11BypassWM ),
  m_parentWidget( parent ), m_filter(false) {

  connect(m_parentWidget, SIGNAL(destroyed()), SLOT(destroyObject()));

  QHBoxLayout* layout = new QHBoxLayout(this,0,0);
  m_display = new KHTMLPart(this);
  m_display->view()->setMarginWidth(4);
  m_display->view()->setMarginHeight(4);
  layout->addWidget(m_display->view());

  setPalette( QToolTip::palette() );
  setMargin( 1 );
  setFrameStyle( QFrame::Plain | QFrame::Box );
  hide();

  m_parentWidget->installEventFilter(this);
  setFilter(false);
}

CToolTip::~CToolTip(){
}

/** Returns the widget this tooltip applies to. This tooltip widget is destroyed when he parent is deleted. */
QWidget* const CToolTip::parentWidget() const{
  return m_parentWidget;
}

/** This function shows a tip with the given text. The tip disappears if the mouse moves out of the rectangle rect. */
void CToolTip::tip( const QRect& rect, const QString& text ){
  qWarning("CToolTip::tip");

  m_display->begin();
  m_display->write(text);
  m_display->end();

  m_display->view()->setHScrollBarMode(QScrollView::AlwaysOff);
  qWarning("view size is %i x %i", m_display->view()->width(), m_display->view()->height());

  QPoint pos = parentWidget()->mapToGlobal( QPoint(rect.x()-2, rect.y()-2) );
  QRect tipRect = QRect(pos.x(), pos.y(), width(), height());
  if (!KApplication::desktop()->geometry().contains(tipRect, true)) {
    qWarning("outside of the screen!");
    QRect intersect = KApplication::desktop()->geometry().intersect(tipRect);
    tipRect.moveBy(-(tipRect.width()-intersect.width()),-(tipRect.height()-intersect.height()));
    pos = tipRect.topLeft();
  }
  move(pos);
  show();
}

/** Reimplementation. */
void CToolTip::timerEvent( QTimerEvent* e ) {
//  qWarning("CToolTip::timerEvent( QTimerEvent* e )");

  killTimers();
  if ( !isVisible() ) {
//    startTimer( 15000 );
    maybeTip( parentWidget()->mapFromGlobal(QCursor::pos()) );
  }
  else {
//    setFilter( false );
    hide();
  }
}

/** Reimplementation. */
bool CToolTip::eventFilter( QObject *o, QEvent *e ){
  QMouseEvent* me = dynamic_cast<QMouseEvent*>(e);


  if (o == m_parentWidget) {
    if (e->type() == QEvent::Show)
      setFilter(true);
  }
  else {
    switch ( e->type() ) {
      case QEvent::DragMove:
      case QEvent::DragEnter:
      case QEvent::DragLeave:
      case QEvent::DragResponse:
        break;

      case QEvent::MouseButtonPress:
      case QEvent::MouseButtonRelease:
        if (isVisible() && widgetContainsPoint(m_display->view()->verticalScrollBar(), me->globalPos()))
          break;
      case QEvent::KeyPress:
      case QEvent::KeyRelease:
      case QEvent::FocusIn:
      case QEvent::FocusOut:
        killTimers();
//        setFilter( false );
        hide();
        break;

      case QEvent::MouseMove:
      {
        const bool validMousePos = widgetContainsPoint(this, me->globalPos());
        if (isVisible() && !validMousePos)
          hide();
        if (isVisible() && validMousePos)
          break;

        if (QMouseEvent* me = dynamic_cast<QMouseEvent*>(e)) {
          if (me->stateAfter() != Qt::NoButton) //probaby dragging - show no tip
            break;
          if (QWidget * w = KApplication::widgetAt( me->globalPos(), true )) {
            while ( w && w != parentWidget()) {
              w = w->parentWidget();
            }
            if (w == parentWidget()) {
              startTimer(1500);
            }
          }
        }
        break;
      }
      default:
        break;
    }
  }
  return false;
}

/** Sets the tooltip on or off. */
void CToolTip::setFilter( const bool enable ){
  if ( enable == m_filter ) return;

  if ( enable ) {
     KApplication::kApplication()->installEventFilter( this );
     KApplication::setGlobalMouseTracking( true );
  }
  else {
     KApplication::setGlobalMouseTracking( false );
     KApplication::kApplication()->removeEventFilter( this );
  }
  m_filter = enable;
}

/** A helper function which returns true if the given widget contains the global pos p. */
const bool CToolTip::widgetContainsPoint( QWidget* const w, const QPoint& p ){
  const QPoint origin = w->mapToGlobal(QPoint(0,0));
  if (p.x() >= origin.x() && p.x() <= w->mapToGlobal(QPoint(w->width(),w->height())).x() ) { //x is valid
    if (p.y() >= origin.y() && p.y() <= w->mapToGlobal(QPoint(w->width(),w->height())).y() ) { //x is valid
//      qWarning("widget contains point!");
      return true;
    }
  }
//  qWarning("widget doesn contain pos!");
  return false;
}

/** No descriptions */
void CToolTip::destroyObject(){
  setFilter(false);
  deleteLater();
}
