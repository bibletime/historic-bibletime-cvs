/***************************************************************************
                          cbookpresenter.h  -  description
                             -------------------
    begin                : Thu Jan 24 2002
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

#ifndef CBOOKPRESENTER_H
#define CBOOKPRESENTER_H

#include <qwidget.h>
#include "cswordpresenter.h"

/**Creates a presenter object for generic book support.
  *@author The BibleTime team
  */

class CBookPresenter : public CSwordPresenter  {
   Q_OBJECT
public: 
	/**
	* Constructor of the presenter for generic books.
	*/
	CBookPresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent=0, const char *name=0 );
	virtual ~CBookPresenter();

protected: // Protected methods
  /**
  * Initializes the interface of this presenter.
  */
  virtual void initView();
  /**
  * Init the Signal Slot connections between childs and this widget.
  */
  virtual void initConnections();

protected slots:
	void modulesChanged();
};

#endif
