/***************************************************************************
                          cbiblekeychooser.h  -  description
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

#ifndef CBIBLEKEYCHOOSER_H
#define CBIBLEKEYCHOOSER_H

#include <qwidget.h>
#include "ckeychooser.h"

class CKeyChooserWidget;
class CModuleInfo;
//class CKeyChain;
class CSwordVerseKey;
class CSwordBibleModuleInfo;


/** This class implements the KeyChooser for bibles and commentaries
	*
	* it inhertits @ref CKeyChooser
	*
	*	it uses 3 @ref CKeyChooserWidget 's to represent the bible keys
	*
  * @author The BibleTime team
  */

class CBibleKeyChooser : public CKeyChooser  {
   Q_OBJECT

public:
	/**
		* the constructor
		*	you should not need to use this, use @ref CKeyChooser::createInstance instead
		*/
	CBibleKeyChooser(CModuleInfo *info=0, CKey *key=0, QWidget *parent=0, const char *name=0);
	/**
		* the destructor
		*/
	~CBibleKeyChooser();

public slots:
	/**
		* see @ref CKeyChooser::getKey
		*/
	CKey*	getKey();
	/**
		* see @ref CKeyChooser::setKey
		*/
	void setKey(CKey* key);
  /**
  	* Reimplementation
  	*/
  virtual QSize sizeHint();
  /**
  	* Reimplementation.
  	*/
  void refreshContent();
	/**
		* used to react to changes in the 3 @ref CKeyChooserWidget 's
		* @param index not used
		*/
	void bookChanged(int index);
	/**
		* used to react to changes in the 3 @ref CKeyChooserWidget 's
		* @param index not used
		*/
	void chapterChanged(int index);
	/**
		* used to react to changes in the 3 @ref CKeyChooserWidget 's
		* @param index not used
		*/
	void verseChanged(int index);
	
  /**
  	* used to react to a request from the book @ref CKeyChooserWidget
  	*/
  void bookPrevRequested(void);
  /**
   	* see @ref bookPrevRequested
   	*/
  void bookNextRequested(void);
  /**
  	* see @ref bookPrevRequested
  	*/
  void chapterPrevRequested(void);
  /**
  	* see @ref bookPrevRequested
  	*/
  void chapterNextRequested(void);
  /**
  	* see @ref bookPrevRequested
  	*/
  void versePrevRequested(void);
  /**
  	* see @ref bookPrevRequested
  	*/
  void verseNextRequested(void);

protected:
	/**
		* to represent the book part of the bible key
		*/
	CKeyChooserWidget* w_book;
	/**
		* to represent the chapter part of the bible key
		*/
	CKeyChooserWidget* w_chapter;
	/**
		* to represent the verse part of the bible key
		*/
	CKeyChooserWidget* w_verse;

	CSwordBibleModuleInfo	*m_info;
	CSwordVerseKey	*m_key;
};

#endif
