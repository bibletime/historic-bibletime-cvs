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
#include <qbitmap.h>

//KDE includes
#include <kapplication.h>
#include <dom/html_element.h>

CToolTip::CToolTip(QWidget *parent, const char *name ) : QFrame( 0, 0, WStyle_Customize | WStyle_NoBorder | WStyle_Tool | WStyle_StaysOnTop | WX11BypassWM ),
  m_filter(false), m_parentWidget( parent ) {

  connect(m_parentWidget, SIGNAL(destroyed()), SLOT(destroyObject()));

  QHBoxLayout* layout = new QHBoxLayout(this,0,0);
  layout->setResizeMode(QLayout::FreeResize);

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

/** Returns the widget this tooltip applies to. This tooltip widget is destroyed when he parent is deleted. */
QWidget* const CToolTip::parentWidget() const{
  return m_parentWidget;
}

/** This function shows a tip with the given text. The tip disappears if the mouse moves out of the rectangle rect. */
void CToolTip::tip( const QPoint& p, const QRect& rect, const QString& text ){
  //m_tipRect must have global coordinates!
  QPoint globalPos = parentWidget()->mapToGlobal(rect.topLeft());
  m_tipRect = QRect(globalPos.x(), globalPos.y(), rect.width(), rect.height());

  m_display->begin();
  m_display->write(text);
  m_display->end();
  m_display->view()->setHScrollBarMode(QScrollView::AlwaysOff);

//make usre the tooltip won't grow to large
  const QRect screenSize = KApplication::desktop()->geometry();
  setMaximumSize(QSize(screenSize.width()*0.6, screenSize.height()*0.6));
  resize((int)((float)screenSize.width()*0.6), 0);
  show();
//now resize to the correct size!
  m_display->view()->layout();
  resize(m_display->view()->sizeHint());    
  
  const QPoint mp = (!m_display->view()->verticalScrollBar()->isHidden()) ? QPoint(p.x()-5, p.y()-5) : QPoint(p.x()+10, p.y()+10);
  QPoint pos = parentWidget()->mapToGlobal( mp );
  QRect widgetRect = QRect(pos.x(), pos.y(), width(), height());
  if (!KApplication::desktop()->geometry().contains(widgetRect, true)) {
    //the tooltip would be outside of the screen, move inside the screen now
    QRect intersect = KApplication::desktop()->geometry().intersect(widgetRect);
    widgetRect.moveBy(-(widgetRect.width()-intersect.width()),-(widgetRect.height()-intersect.height()));
    pos = widgetRect.topLeft();
  }
  move(pos);

  m_display->view()->setContentsPos(0,0);
}

/** Reimplementation. */
void CToolTip::timerEvent( QTimerEvent* e ) {
  killTimers();
  if ( !isVisible() ) {
    maybeTip( parentWidget()->mapFromGlobal(QCursor::pos()) );
  }
  else {
    hide();
  }
}

/** Reimplementation. */
bool CToolTip::eventFilter( QObject *o, QEvent *e ){
  QMouseEvent* me = dynamic_cast<QMouseEvent*>(e);
  if (o == parentWidget()) {
    if (e->type() == QEvent::Show) {
      setFilter(true);
      return false;
    }
  }

  switch ( e->type() ) {
    case QEvent::DragMove:
    case QEvent::DragEnter:
    case QEvent::DragLeave:
    case QEvent::DragResponse:
      break;

    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
      if (QMouseEvent* me = dynamic_cast<QMouseEvent*>(e)) {
        if (me->state() != Qt::NoButton || me->stateAfter() != Qt::NoButton) { //probaby dragging - show no tip
          killTimers();
          hide();
          break;
        }
      }
      if (isVisible() && widgetContainsPoint(m_display->view()->verticalScrollBar(), me->globalPos()))
        break;
      else {
        hide();
        break;
      }
    case QEvent::KeyPress:
    case QEvent::KeyRelease:
    case QEvent::FocusIn:
    case QEvent::FocusOut:
      killTimers();
      hide();
      break;

    case QEvent::MouseMove:
    {
      bool validMousePos = widgetContainsPoint(this, me->globalPos());
      validMousePos = validMousePos || m_tipRect.contains(me->globalPos());
      if (isVisible() && !validMousePos)
        hide();
      if (isVisible() && validMousePos)
        break;

      if (QMouseEvent* me = dynamic_cast<QMouseEvent*>(e)) {
        if (me->state() != Qt::NoButton || me->stateAfter() != Qt::NoButton) { //probaby dragging - show no tip
          killTimers();
          hide();
          break;
        }

        if (QWidget * w = KApplication::widgetAt( me->globalPos(), true )) {
          while ( w && w != parentWidget()) {
            w = w->parentWidget();
          }
          if (w == parentWidget()) {
            startTimer(1500);
          }
          else {
            killTimers();
            break;
          };
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
      return true;
    }
  }
  return false;
}

/** No descriptions */
void CToolTip::destroyObject(){
  setFilter(false);
  deleteLater();
}
