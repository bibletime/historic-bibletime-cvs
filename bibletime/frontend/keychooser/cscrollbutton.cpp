/***************************************************************************
                          cscrollbutton.cpp  -  description
                             -------------------
    begin                : Tue Sep 19 2000
    copyright            : (C) 2000 by The BibleTime team
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

#include "cscrollbutton.h"

#include "frontend/cbtconfig.h"

#include <stdlib.h>
#include <math.h>

//Qt includes
#include <qevent.h>
#include <qapplication.h>
#include <qcursor.h>

CScrollButton::CScrollButton(QWidget *parent, const char *name ) : QToolButton(parent,name) {
	setFocusPolicy(QWidget::WheelFocus);
	setCursor( splitVCursor );
		
	m_isLocked = false;
	connect(this, SIGNAL(pressed() ), SLOT(was_pressed() ));
	connect(this, SIGNAL(released()), SLOT(was_released()));
}

const bool CScrollButton::isLocked( ) const {
	return m_isLocked;
}

void CScrollButton::was_pressed( ){
	QApplication::setOverrideCursor( BlankCursor );
	m_isLocked = true;
	lock_Point = get_lock_Point();
	
	emit lock();
}

void CScrollButton::was_released( ){
	QApplication::restoreOverrideCursor();
	m_isLocked = false;
	
	emit unlock();
}

const QPoint CScrollButton::get_lock_Point() const {
	return mapToGlobal( QPoint( width()/2, height()/2 ) );
}

void CScrollButton::mouseMoveEvent( QMouseEvent* e ){
 	const short signed int scrollDirection = CBTConfig::get(CBTConfig::scroll) ? -1 : 1;
	
	if (m_isLocked) {
		int vchange = (QCursor::pos().y() - lock_Point.y()) * scrollDirection;
		
		if (abs(vchange) < 10) {
      vchange = (int)((vchange>0 ? -1 : 1) * pow(abs(vchange), 0.3));
		}
		else if (abs(vchange) < 30) {
      vchange = (int)((vchange>0 ? -1 : 1) * pow(abs(vchange), 0.6));
		}
		else if (abs(vchange) < 40) {
      vchange = (int)((vchange>0 ? -1 : 1) * pow(abs(vchange), 1.2));
		}
		else {
      vchange = (int)((vchange>0 ? -1 : 1) * pow(abs(vchange), 2.0));
		}

		if (vchange) { //not emit 0
			emit change_requested( vchange );
		}
		
		QCursor::setPos( lock_Point );
	}
	else {
		QToolButton::mouseMoveEvent(e);
	}
}



/** If the wheel of the mouse is used while the mouse stays over our scrollbutton the content is  scrolled like the mouse was pressed and moved. */
void CScrollButton::wheelEvent( QWheelEvent* e ){
	/**
	* The problem is, that wheel events do everytime have the delta value 120
	*/
 	const short signed int scrollDirection = CBTConfig::get(CBTConfig::scroll) ? -1 : 1;	 	
	const int vchange = scrollDirection * ((e->delta() > 0) ? (1) : (-1));
	
	if (vchange!=0) {//do not emit a change with value 0	
		emit change_requested( vchange );
		e->accept();			
	}
	else {
		e->ignore();
	}
}