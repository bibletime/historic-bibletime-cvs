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

//#include <qnamespace.h>
#include <qwidget.h>
#include "ckeychooser.h"

class CKeyChooserWidget;
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
	CBibleKeyChooser(CSwordModuleInfo *module=0, CSwordKey *key=0, QWidget *parent=0, const char *name=0);

public slots:
	/**
	* see @ref CKeyChooser::getKey
	*/
	CSwordKey*const key();
	/**
	* see @ref CKeyChooser::setKey
	*/
	virtual void setKey(CSwordKey *key);
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
  /** No descriptions */
  void updateKey(CSwordKey* key);

private:
	CKeyChooserWidget* w_book;
	CKeyChooserWidget* w_chapter;
	CKeyChooserWidget* w_verse;
	CSwordBibleModuleInfo	*m_info;
	CSwordVerseKey *m_key;

private slots: // Private slots
  /**
  *	called when the book combo lost the focus with reason == tab
  * @param the new book
  */
  void bookFocusOut(int);
  /**
  * called when the chapter combo lost the focus with reason == tab
  * @param the new chapter
  */
  void chapterFocusOut(int);
  /** called when the verse combo lost the focus
  with reason == tab
  @param the new verse
  */
  void verseFocusOut(int);
};

#endif
