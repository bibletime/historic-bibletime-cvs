/***************************************************************************
                          cbiblereadwindow.h  -  description
                             -------------------
    begin                : Don Mai 9 2002
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

#ifndef CBIBLEREADWINDOW_H
#define CBIBLEREADWINDOW_H

//BibleTime includes
#include "clexiconreadwindow.h"

#include "backend/cswordkey.h"
#include "backend/cswordversekey.h"

//Qt includes
#include <qwidget.h>

//KDE includes
#include <kaction.h>


class CTransliterationButton;

/**The read display window for Bibles.
  *@author The BibleTime team
  */

class CBibleReadWindow : public CLexiconReadWindow  {
   Q_OBJECT
public: 
	CBibleReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name=0);
	virtual ~CBibleReadWindow();
  /**
  * Store the settings of this window in the given CProfileWindow object.
  */
  virtual void storeProfileSettings( CProfileWindow* const settings );
  /**
  * Store the settings of this window in the given profile window.
  */
  virtual void applyProfileSettings( CProfileWindow* const settings );
  /**
  * Reimplementation.
  */
  static void insertKeyboardActions( KAccel* const a );

protected:
	virtual void initKeyboardActions();
  virtual void initConnections();
  virtual void initView();
  /**
  * Reimplementation.
  */
  virtual void setupPopupMenu();
  /**
	* Reimplemented.
	*/
  virtual void updatePopupMenu();
  /** Event filter.
  * Reimplementation of the event filter to filter out events like focus in.
  */
  virtual bool eventFilter( QObject* o, QEvent* e);

  struct {
    KAction* selectAll;
    KActionMenu* copyMenu;
  	struct {
			KAction* referenceOnly;
   		KAction* referenceTextOnly;
     	KAction* referenceAndText;
      KAction* chapter;
      KAction* selectedText;
   	} copy;

    KActionMenu* saveMenu;
    struct {
			KAction* referenceAndText;
   		KAction* chapterAsPlain;
     	KAction* chapterAsHTML;
    } save;

    KActionMenu* printMenu;
    struct {
			KAction* reference;
			KAction* chapter;
		} print;
  } m_actions;


public slots:
  void nextBook();
  void previousBook();
  void nextChapter();
  void previousChapter();
  void nextVerse();
  void previousVerse();
  /**
  * Refreshes the content of this display window and the content of the keychooser.
  */
  virtual void reload();
     
protected slots: // Protected slots 
  void keyChanged(CSwordKey* key);
  /**
  * Copies the current chapter into the clipboard.
  */
  void copyDisplayedText();
  /**
  * Saves the chapter as valid HTML page.
  */
  void saveChapterHTML();
  /**
  * Saves the chapter as valid HTML page.
  */
  void saveChapterPlain();
	virtual void lookup( CSwordKey* newKey );
	void syncWindows();

private:
  friend class CCommentaryReadWindow;
  /**
  * Wrapper around key() to return the right type of key.
  */
  CSwordVerseKey* verseKey();

//  CTransliterationButton* m_transliterationButton;
};

#endif
