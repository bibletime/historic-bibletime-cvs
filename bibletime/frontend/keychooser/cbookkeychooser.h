/***************************************************************************
                          cbookkeychooser.h  -  description
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

#ifndef CBOOKKEYCHOOSER_H
#define CBOOKKEYCHOOSER_H

//BibleTime includes
#include "ckeychooser.h"
#include "ckeychooserwidget.h"

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

class QHBoxLayout;

/** The keychooser implementation for books.
  * @author The BibleTime team
  */
class CBookKeyChooser : public CKeyChooser  {
   Q_OBJECT
public: 
	CBookKeyChooser(CSwordModuleInfo *module=0, CSwordKey *key=0, QWidget *parent=0, const char *name=0);
	~CBookKeyChooser();
  /**
  * Refreshes the content.
  */
  virtual void refreshContent();
  /**
  * Sets another module to this keychooser
  */
  virtual void setModule(CSwordModuleInfo*);
  /**
  * Returns the key of this kechooser.
  */
  virtual CSwordKey* key();
  /**
  * Sets a new key to this keychooser
  */
  virtual void setKey(CSwordKey*);
  /**
  * Sets a new key to this keychooser
  */
  void setKey(CSwordKey*, const bool emitSignal);

private:
	QList<CKeyChooserWidget> m_chooserWidgets;	
	CSwordBookModuleInfo	*m_module;
	CSwordTreeKey *m_key;
	QHBoxLayout* m_layout;

protected: // Protected methods
  /**
  * Fills the combo given by depth with the items from the key having depth "depth".
	* The parent sibling is given by key.
	*/
  void setupCombo(const QString key, const int depth, const int currentItem);

protected slots:
  /**
  * A keychooser changed. Update and emit a signal if necessary.
  */
  void keyChooserChanged(int);

public slots: // Public slots
  /**
  * Updates the keychoosers for the given key but emit no signal.
  */
  void updateKey(CSwordKey*);
};

#endif
