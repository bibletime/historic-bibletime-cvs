/***************************************************************************
                          clexiconkeychooser.h  -  description
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

#ifndef CLEXICONKEYCHOOSER_H
#define CLEXICONKEYCHOOSER_H

#include <qwidget.h>
#include "ckeychooser.h"
#include "backend/cswordldkey.h"
#include "backend/cswordmoduleinfo.h"
#include "backend/cswordlexiconmoduleinfo.h"

class CKeyChooserWidget;
class QHBoxLayout;

/**
	* This class implements the KeyChooser for lexicons
	*
	* it inhertits @ref CKeyChooser
	*	it uses 1 @ref CKeyChooserWidget to represent the lexicon keys
	*
  * @author The BibleTime team
  */
class CLexiconKeyChooser : public CKeyChooser  {
   Q_OBJECT
public:
	/**
	* The constructor
	*
	*	you should not need to use this, use @ref CKeyChooser::createInstance instead
	*/
	CLexiconKeyChooser(ListCSwordModuleInfo modules, CSwordKey *key=0, QWidget *parent=0, const char *name=0);

public slots:
	/**
	* see @ref CKeyChooser::getKey
	* @return Return the key object we use.
	*/
	virtual CSwordKey* const key();
	/**
	* see @ref CKeyChooser::setKey
	*/
	virtual void setKey(CSwordKey* key);
	/**
	* used to react to changes in the @ref CKeyChooserWidget
	*
	* @param index not used
	**/
	virtual void activated(int index);	
  /**
  * Reimplementatuion.
  */
  virtual void refreshContent();
  /**
  * Sets the module and refreshes the combo boxes of this keychooser.
  */
  virtual void setModules( ListCSwordModuleInfo modules, const bool refresh = true );

protected:	
	class CLexiconPresenter;	
	friend class CLexiconPresenter;	
	CKeyChooserWidget *m_widget;
	CSwordLDKey* m_key;
	QPtrList<CSwordLexiconModuleInfo> m_modules;
	QHBoxLayout *m_layout;

public slots: // Public slots
  virtual void updateKey(CSwordKey* key);
};

#endif
