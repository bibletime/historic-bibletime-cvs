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

static QRect screenSize; // = QRect();

CToolTip::CToolTip(QWidget *parent, const char *name ) : QFrame( 0, name, WStyle_Customize | WStyle_NoBorder | WStyle_Tool | WStyle_StaysOnTop | WX11BypassWM ),
  m_filter(false), m_parentWidget( parent ) {

  connect(m_parentWidget, SIGNAL(destroyed()), SLOT(destroyObject()));

  QHBoxLayout* layout = new QHBoxLayout(this,0,0);
  layout->setResizeMode(QLayout::FreeResize);

  hide();  
  m_display = new KHTMLPart(this);
  m_display->view()->setMarginWidth(4);
  m_display->view()->setMarginHeight(4);
  layout->addWidget(m_display->view());

  //set the size for the tooltip now only one time, and not everytime in tip()
  screenSize = KApplication::desktop()->geometry();
  setMaximumHeight( int(screenSize.height()*0.6) );
  setFixedWidth( int(screenSize.width()*0.25) );  
  m_display->view()->setHScrollBarMode(QScrollView::AlwaysOff); //never show a horizontal bar, only the vertcal one

  setPalette( QToolTip::palette() );
  setMargin( 1 );
  setFrameStyle( QFrame::Plain | QFrame::Box );
  m_parentWidget->installEventFilter(this);
  setFilter(false);
}

/** This function shows a tip with the given text. The tip disappears if the mouse moves out of the rectangle rect. */
void CToolTip::tip( const QPoint& p, const QRect& rect, const QString& text ){
  //m_tipRect must have global coordinates!
  QPoint globalPos = parentWidget()->mapToGlobal(rect.topLeft());
  m_tipRect = QRect(globalPos.x(), globalPos.y(), rect.width(), rect.height());

  m_display->begin();
  m_display->write(text);
  m_display->end();


  //original code
//  show();
//  m_display->view()->layout(); //refresh painted text etc.
//  resize(m_display->view()->sizeHint());

  // the maximum size was set in our constructor, so we won't grow too large
  // resize((int)((float)screenSize.width()*0.6), 0);
//  resize(int(double(screenSize.width())*0.50),1);

  show();
  m_display->view()->layout();
    
  // resize to the size hint,
  // we can't grow too large because maximumSize was set in the constructor
  resize( sizeHint().width(),  m_display->view()->height()+4 );
    
  // if the scrollbar is not visible position the tooltip
  // that the tip will be hidden as soon as the mouse will be moved
  // if the bar s visible position the tip under the mouse so moving the bar is still possible
  const QPoint mp = (m_display->view()->verticalScrollBar()->isVisible()) ? QPoint(p.x()-10, p.y()-10) : QPoint(p.x()+10, p.y()+10);
  QPoint pos = parentWidget()->mapToGlobal( mp );
  QRect widgetRect = QRect(pos.x(), pos.y(), width(), height());
  if (!KApplication::desktop()->geometry().contains(widgetRect, true)) {
    //the tooltip would be outside of the screen, move inside the screen now
    QRect intersect = KApplication::desktop()->geometry().intersect(widgetRect);
    widgetRect.moveBy(-(widgetRect.width()-intersect.width()),-(widgetRect.height()-intersect.height()));
    pos = widgetRect.topLeft();
  }
  move(pos);

  m_display->view()->setContentsPos(0,0); //show from the bgeinning, reset any scrollbar position
}

/** Reimplementation. */
void CToolTip::timerEvent( QTimerEvent*  ) {
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
  if (o == parentWidget()) {
    if (e->type() == QEvent::Show) {
      setFilter(true);
      return false;
    }
  }

  QMouseEvent* me = dynamic_cast<QMouseEvent*>(e);  
  switch ( e->type() ) {
    case QEvent::MouseButtonPress: {
      //we have not yet shown the tip, but while the timer is running for this a drag was started
      if (me && !isVisible() && ((me->state() != NoButton) || (me->stateAfter() != Qt::NoButton))) {
        killTimers();
        hide();
        break;
      }

      if ( !m_display->view()->verticalScrollBar()->isVisible()
		||  (m_display->view()->verticalScrollBar()->isVisible() && !widgetContainsPoint(m_display->view()->verticalScrollBar(),
            me->globalPos() )
      ))
    {
        killTimers();
        hide();
        break;
      }
      break;
    }
      
    case QEvent::MouseButtonRelease: {
      //allow clicking on the scrollbar for reading the text
      if (      m_display->view()->verticalScrollBar()->isVisible()
          && (m_display->view()->verticalScrollBar()->draggingSlider() || widgetContainsPoint(m_display->view()->verticalScrollBar(), me->globalPos()))
      ) {
        break;
      }
      else {
        hide();
        break;
      }
      break;
    }
    
    case QEvent::KeyPress: //fall through
    case QEvent::KeyRelease: //fall though
    case QEvent::FocusIn: //fall though
    case QEvent::FocusOut:
      killTimers();
      hide();
      break;

    case QEvent::MouseMove:
    {
      const bool validMousePos = //true when the mouse is at a valid position, false if not. In this case hide the tooltip.
				 widgetContainsPoint(this, me->globalPos())
              || m_tipRect.contains( me->globalPos() )
              || m_display->view()->verticalScrollBar()->draggingSlider(); //if the user's scrolling and moved the mouse out of the area

      if (isVisible() && !validMousePos) { //mouse moved outside the visible tooltip area!
        hide();
        break;
      }
      if (isVisible() && validMousePos) { //moving withing the tooltip area or dragging the scroll bar
        break;
      }

      if (me) {
        if (!m_display->view()->verticalScrollBar()->draggingSlider() && //user doesn't scoll at the moment
            ((me->state() != Qt::NoButton) || (me->stateAfter() != Qt::NoButton))) //probaby dragging at the moment - show no tip
        {   
          killTimers();
          hide();
          break;
        }

        if (QWidget* w = KApplication::widgetAt( me->globalPos(), true )) { //check whether parentWidget is visible on top
          while ( w && (w != parentWidget()) ) {
            if (w->isPopup()) {
				break;
			}
			
            w = w->parentWidget();
          }

          const bool validPopup = KApplication::activePopupWidget() ? (KApplication::activePopupWidget() == parentWidget()) : true;
          if (w && (w == parentWidget()) && validPopup) { //inside the parent widget and not a popup which appears outside of parentwidget
		  	// if we processed the event of one of parentWidget()'s childs
		   // but we have to make sure the toplevel widget isn't a child popup of the parent widget which hides the parent
		   // In this case the tooltip would be wrong
		   
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
  if ( enable == m_filter ) return;  //nothing changed

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
  if (p.x() >= origin.x() && p.x() <= w->mapToGlobal(QPoint(w->width(),w->height())).x() ) { //X is valid
    if (p.y() >= origin.y() && p.y() <= w->mapToGlobal(QPoint(w->width(),w->height())).y() ) { //Y is valid
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
