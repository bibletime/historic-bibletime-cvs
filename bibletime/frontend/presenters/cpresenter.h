/***************************************************************************
                          cpresenter.h  -  description
                             -------------------
    begin                : Sun Mar 18 2001
    copyright            : (C) 2001 by The BibleTime team
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

#ifndef CPRESENTER_H
#define CPRESENTER_H

#include <qwidget.h>
#include <kmainwindow.h>

class KConfig;

/**The base class for all presenters.
  *@author The BibleTime team
  */

class CPresenter : public KMainWindow  {
   Q_OBJECT
public: 
	/**
	* Default xonstructor
	*/
	CPresenter(QWidget *parent, const char *name=0, WFlags f=0/*WDestructiveClose*/);

protected:
	KConfig* m_config;
protected: // Protected methods
  /**
  * Is called when the presenter should be closed. To delete the presenter it emits "close(CPresenter*)".
  */
  virtual void closeEvent(QCloseEvent*e);

signals: // Signals
  /**
  * Is emitted when this presenter should be closed.
  */
  void closePresenter(CPresenter*);
};

#endif