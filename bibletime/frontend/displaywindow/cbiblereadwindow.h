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

  struct {
    KAction* selectAll;
    KActionMenu* copyMenu;
  	struct {
			KAction* verseOnly;
   		KAction* textOnly;
     	KAction* verseAndText;
      KAction* chapter;
      KAction* selectedText;
   	} copy;

    KActionMenu* saveMenu;
    struct {
			KAction* verseAndText;
   		KAction* chapterAsPlain;
     	KAction* chapterAsHTML;
    } save;

    KActionMenu* printMenu;
    struct {
			KAction* verseAndText;
			KAction* chapter;
		} print;
  } m_actions;

protected slots: // Protected slots
  void nextBook();
  void previousBook();
  void nextChapter();
  void previousChapter();
  void nextVerse();
  void previousVerse();

private:
  friend class CCommentaryReadWindow;
  /**
  * Wrapper around key() to return the right type of key.
  */
  CSwordVerseKey* verseKey();
};

#endif
