/***************************************************************************
                          cbooktreechooser.h  -  description
                             -------------------
    begin                : Sat Jan 26 2002
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

#ifndef CBOOKTREECHOOSER_H
#define CBOOKTREECHOOSER_H

//BibleTime includes
#include "ckeychooser.h"

//Sword includes

//Qt includes
#include <qwidget.h>
#include <qsize.h>
#include <qmap.h>
#include <qlist.h>
#include <qstringlist.h>

class CSwordKey;
class CSwordBookModuleInfo;
class CSwordTreeKey;

class TreeKeyIdx;

/** The keychooser implementation for books.
  * @author The BibleTime team
  */
class CBookTreeChooser : public CKeyChooser  {
   Q_OBJECT
public: 
	CBookTreeChooser(CSwordModuleInfo *module=0, CSwordKey *key=0, QWidget *parent=0, const char *name=0);
	~CBookTreeChooser();

public slots:
	/**
	* see @ref CKeyChooser::getKey
	*/
	CSwordKey* key();
	/**
	* see @ref CKeyChooser::setKey
	*/
	void setKey(CSwordKey *key);
  /**
 	* Reimplementation
 	*/
  virtual QSize sizeHint();
  /**
  * Sets te module and refreshes the combos
  */
  virtual void setModule(CSwordModuleInfo* module);
  /**
 	* Reimplementation.
 	*/
  void refreshContent();

private:
//	QList<CKeyChooserWidget> m_chooserWidgets;	
	CSwordBookModuleInfo	*m_module;
	CSwordTreeKey *m_key;
	QStringList m_topElements;

protected: // Protected methods
  /**
  * Sets up the entries of the given key chooser.
  */
  void setupKeyChooser(const int number, TreeKeyIdx* tree);

protected slots: // Protected slots
  void keyChooserChanged(int);
};

#endif
