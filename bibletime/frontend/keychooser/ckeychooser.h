/***************************************************************************
                          ckeychooser.h  -  description
                             -------------------
    begin                : Wed Sep 20 2000
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

#ifndef CKEYCHOOSER_H
#define CKEYCHOOSER_H

#include <qwidget.h>

class CModuleInfo;
class CKey;

/**
	* The base class for the KeyChooser.
	* Do not use directly, create a KeyChooser with
	* @ref #createInstance , this will create the proper one
	* of the classes that inherit from @ref CKeyChooser
	*
  * @author The BibleTime team
  */

class CKeyChooser : public QWidget {
   Q_OBJECT

public:
  /**
  * Creates a proper Instance, either
	*
	 @ref CLexiconKeyChooser or
	* @ref CBibleKeyChooser
	* @param info the @ref CModuleInfo to be represented by the KeyChooser
	* @param key if not NULL, the @ref CKey the KeyChooser should be set to
	* @param parent the parent of the widget to create
	*/
  static CKeyChooser* createInstance(CModuleInfo *info, CKey *key, QWidget *parent);

signals:
	/**
	* is emitted if the @ref CKey was changed by the user
	*/
	void keyChanged(CKey* key);
	/**
	* Is emitted before the key is changed!
	*/
	void beforeKeyChange(const QString& key);
	
public slots:
	/**
	* sets the @ref CKey
	* @param key the key which the widget should be set to
	*/
	virtual void	setKey(CKey* key) = 0;
	/**
	* gets the current @ref CKey
	*
	* @return the current @ref CKey
	*/
  virtual CKey*	getKey() = 0;	
  /**
  * Sets the module of this keychooser and refreshes the comboboxes
  */
  virtual void setModule( CModuleInfo* ) = 0;
  /**
  * Freshes the content of the different key chooser parts.
  */
  virtual void refreshContent() = 0;

protected:
	/**
	* the constructor - DO NOT USE! -- use @ref #createInstance
	*/
	CKeyChooser(CModuleInfo *info=0, CKey *key=0, QWidget *parent=0, const char *name=0);
};

#endif
