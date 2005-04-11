/********* Read the file LICENSE for license details. *********/

#ifndef CBIBLEKEYCHOOSER_H
#define CBIBLEKEYCHOOSER_H

#include <qwidget.h>
#include "ckeychooser.h"

#include "backend/cswordbiblemoduleinfo.h"

class CKeyChooserWidget;
class CSwordVerseKey;

class CSwordBibleModuleInfo;

class QuickBrowseDialog;


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
	CBibleKeyChooser(ListCSwordModuleInfo modules, CSwordKey *key=0, QWidget *parent=0, const char *name=0);

public slots:
	/**
	* see @ref CKeyChooser::getKey
	*/
	CSwordKey* const key();
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
  virtual void setModules(const ListCSwordModuleInfo& modules, const bool refresh = true);
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
  void updateKey(CSwordKey* key);


protected:
  virtual void adjustFont();

private:
	CKeyChooserWidget* w_book;
	CKeyChooserWidget* w_chapter;
	CKeyChooserWidget* w_verse;
	QPtrList<CSwordBibleModuleInfo> m_modules;
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
