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

CToolTip::CToolTip(QWidget *parent, const char *name ) : QFrame( 0, 0, WStyle_Customize | WStyle_NoBorder | WStyle_Tool | WStyle_StaysOnTop | WX11BypassWM ),
  m_parentWidget( parent ), m_filter(false) {

  connect(m_parentWidget, SIGNAL(destroyed()), SLOT(destroyObject()));

  QHBoxLayout* layout = new QHBoxLayout(this);
  m_display = new KHTMLPart(this);
  layout->addWidget(m_display->view());

  setPalette( QToolTip::palette() );
  setMargin( 1 );
  setFrameStyle( QFrame::Plain | QFrame::Box );
  hide();

  setFilter(true);
//  KApplication::setGlobalMouseTracking(true);
//  KApplication::kApplication()->installEventFilter(this);
}

CToolTip::~CToolTip(){
  qWarning("destructopr of CToolTip");
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
  m_display->view()->layout();

  m_display->view()->setHScrollBarMode(QScrollView::AlwaysOff);
//  m_display->view()->setVScrollBarMode(QScrollView::AlwaysOff);

  QSize hint = m_display->view()->sizeHint();
  Q_ASSERT(hint.isValid());
  if (hint.isValid()) {
    setMinimumWidth(hint.width());
  }
//  m_display->view()->adjustSize();
//  setMinimumHeight(m_display->view()->viewport()->height());

//  setWidth( m_display->view()->viewport()->sizeHint().width() );
//  setHeight( m_display->view()->viewport()->sizeHint().height() );
//  m_display->view()->adjustSize();

//  adjustSize();

  QPoint pos = parentWidget()->mapToGlobal( QPoint(rect.x()-2, rect.y()-2) );
//  if (pos.x() + m_display->width() > KApplication::desktopWidget()->geometry().bottomRight().x()
//    || pos.y() + m_display->height() > KApplication::desktopWidget()->geometry().bottomRight().y()) {
//
//
//  }
  move(pos);
  show();
}

/** Reimplementation. */
void CToolTip::timerEvent( QTimerEvent* e ) {
  qWarning("CToolTip::timerEvent( QTimerEvent* e )");

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

  switch ( e->type() ) {
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
          if (QWidget * w = KApplication::widgetAt( me->globalPos(), true )) {
//            if (!parentWidget()->children()->contains(w))
//              break;
            while ( w && w != parentWidget()) {
              w = w->parentWidget();
            }
            if (w == parentWidget()) {
              startTimer(1000);
            }
          }
        }
        break;
      }
      default:
        break;
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
