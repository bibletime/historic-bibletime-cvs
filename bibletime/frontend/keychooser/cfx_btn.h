/***************************************************************************
                          cfx_btn.h  -  description
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

#ifndef CFX_BTN_H
#define CFX_BTN_H

#include <qwidget.h>
#include <qtoolbutton.h>

class QMouseEvent;

/** This Class implements the direct chooser button used in the KeyChooser Widget
  * @author The BibleTime team
  */
class cfx_btn : public QToolButton  {
   Q_OBJECT
public:
	/**
		* The constructor
		*/
	cfx_btn(QWidget *parent=0, const char *name=0);
	/**
		* indicates whether the button is in locked state or not
		*/
  bool isLocked;

signals:
	/**
		* is emitted when the button enters locked state
		*/
	void lock(void);
	/**
		* is emitted when the button leaves locked state
		*/	
	void unlock(void);
	/**
		* indicates a change the user made by moving the mouse
		* @param count the number of items to be changed in the KeyChooser ComboBox
		*/
	void change_requested(int count);

protected slots:
	/*
		* used to process the button press events
		*/
	void was_pressed(void);
	/**
		* used to process the button release events
		*/
	void was_released(void);

protected:
	/**
		* Reimplementation from @ref QWidget#mouseMoveEvent - processes
		* the mouse move events
		*/
	virtual void mouseMoveEvent( QMouseEvent* e );
	/**
		* used to find the lock point - the middle of the button
		* @return the lock point
		*/
	QPoint get_lock_Point(void);
	/**
		* stores the lock point
		*/	
	QPoint lock_Point;
};

#endif
