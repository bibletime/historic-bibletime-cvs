/***************************************************************************
                          cfx_btn.cpp  -  description
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

#include "cfx_btn.h"

#include <stdlib.h>
#include <math.h>

#include <qevent.h>
#include <qapplication.h>
#include <qcursor.h>

cfx_btn::cfx_btn(QWidget *parent, const char *name ) : QToolButton(parent,name) {
	setCursor( splitVCursor );	
		
	isLocked = false;
	connect(this, SIGNAL(pressed() ), SLOT(was_pressed() ));
	connect(this, SIGNAL(released()), SLOT(was_released()));
}

void cfx_btn::was_pressed( void ){
	QApplication::setOverrideCursor( BlankCursor );
	isLocked = true;
	lock_Point = get_lock_Point();
	
	emit lock();
}

void cfx_btn::was_released( void ){
	QApplication::restoreOverrideCursor();
	isLocked = false;
	
	emit unlock();
}

QPoint cfx_btn::get_lock_Point(void){
	return mapToGlobal( QPoint( width()/2, height()/2 ) );
}

void cfx_btn::mouseMoveEvent( QMouseEvent* e ){
	if (isLocked) {
		int vchange = QCursor::pos().y() - lock_Point.y();
		/**
		* This works for me. Changing the exponent (0.3) changes the speed of the scrolling
		*/
		//vchange = (vchange>0 ? -1 : 1) * pow(abs(vchange), 0.3);
		//problem: too slow to scroll trough large sets
		//soluton: increase the exponent
		//change the border values or the exponents to make it slower / faster
		if      (abs(vchange) < 10)
      vchange = (int)((vchange>0 ? -1 : 1) * pow(abs(vchange), 0.3));
		else if (abs(vchange) < 20)
      vchange = (int)((vchange>0 ? -1 : 1) * pow(abs(vchange), 0.6));
		else if (abs(vchange) < 30)
      vchange = (int)((vchange>0 ? -1 : 1) * pow(abs(vchange), 1.2));
		else
      vchange = (int)((vchange>0 ? -1 : 1) * pow(abs(vchange), 2.0));

		if (vchange) //not emit 0
			emit change_requested( vchange );
		QCursor::setPos( lock_Point );
	}
	else
		QToolButton::mouseMoveEvent(e);
}



