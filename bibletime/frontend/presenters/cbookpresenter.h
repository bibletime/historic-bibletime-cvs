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


class CSwordTreeKey;

/**Creates a presenter object for generic book support.
  *@author The BibleTime team
  */

class KAccel;

class CBookPresenter : public CSwordPresenter  {
   Q_OBJECT
public: 
	/**
	* Constructor of the presenter for generic books.
	*/
	CBookPresenter(ListCSwordModuleInfo useModules, QWidget *parent=0, const char *name=0 );
	virtual ~CBookPresenter();
  static void insertKeyboardActions(KAccel* a);

public slots: // Public slots
  /**
  * No descriptions
  */
  virtual void lookup( const QString&, const QString&);
    	
protected: // Protected methods
  /**
  * Initializes the interface of this presenter.
  */
  virtual void initView();
  /**
  * Init the Signal Slot connections between childs and this widget.
  */
  virtual void initConnections();
	virtual void initAccels();
	
protected slots:
	void modulesChanged();
  /**
  * renders text and set it to the HTML widget
  */
  void lookup(CSwordKey*);	

private:
	CSwordTreeKey* m_key;
};

#endif
