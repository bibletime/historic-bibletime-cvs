/***************************************************************************
                          cscrollbutton.h  -  description
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

#ifndef CSRCOLLBUTTON_H
#define CSRCOLLBUTTON_H

#include <qwidget.h>
#include <qtoolbutton.h>

class QMouseEvent;

/** This Class implements the direct chooser button used in the KeyChooser Widget
  * @author The BibleTime team
  */
class CScrollButton : public QToolButton  {
   Q_OBJECT
public:
	/**
	* The constructor
	*/
	CScrollButton(QWidget *parent=0, const char *name=0);
  const bool isLocked() const;
	
signals:
	/**
	* is emitted when the button enters locked state
	*/
	void lock();
	/**
	* is emitted when the button leaves locked state
	*/	
	void unlock();
	/**
	* indicates a change the user made by moving the mouse
	* @param count the number of items to be changed in the KeyChooser ComboBox
	*/
	void change_requested(int count);

protected slots:
	/*
	* used to process the button press events
	*/
	void was_pressed();
	/**
	* used to process the button release events
	*/
	void was_released();

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
	const QPoint get_lock_Point() const;
  /**
  * If the wheel of the mouse is used while the mouse stays over our scrollbutton the content is
	* scrolled like the mouse was pressed and moved.
	*/
  virtual void wheelEvent( QWheelEvent* e );
	
private:
	/**
	* Indicates whether the button is in locked state or not
	*/
  bool m_isLocked;
	/**
	* stores the lock point
	*/	
	QPoint lock_Point;	
};

#endif