/***************************************************************************
                          ccommentaryreadwindow.cpp  -  description
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

//BibleTime includes
#include "ccommentaryreadwindow.h"

#include "resource.h"

#include "frontend/cprofilewindow.h"
#include "frontend/cbtconfig.h"
#include "frontend/keychooser/ckeychooser.h"
#include "frontend/display/cdisplay.h"
#include "frontend/display/creaddisplay.h"

#include "util/ctoolclass.h"

//Qt includes

//KDE includes
#include <kaction.h>
#include <kaccel.h>
#include <klocale.h>
#include <kpopupmenu.h>

CCommentaryReadWindow::CCommentaryReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name) : CLexiconReadWindow(modules, parent,name) {
}

void CCommentaryReadWindow::applyProfileSettings( CProfileWindow* profileWindow ) {
  CLexiconReadWindow::applyProfileSettings(profileWindow);
  if (profileWindow->windowSettings())
    m_syncButton->setChecked(true);
};

void CCommentaryReadWindow::storeProfileSettings( CProfileWindow* profileWindow ) {
  CLexiconReadWindow::storeProfileSettings(profileWindow);
  profileWindow->setWindowSettings( m_syncButton->isChecked() );
};

void CCommentaryReadWindow::initView(){
	CLexiconReadWindow::initView();
	m_syncButton = new KToggleAction(i18n("Sync with active bible"), ICON_SYNC, KShortcut(0)/*, this, SLOT(syncToggled(bool))*/, actionCollection());	
  m_syncButton->plug(mainToolBar());	
}

/** Jumps to the given key if syncronize is enabled for this display window. */
void CCommentaryReadWindow::syncToKey( CSwordKey* const newKey ){
  if (m_syncButton->isChecked()) {
    CDisplayWindow::lookup(newKey->key());
  }
}

/** Reimplementation to handle the keychooser refresh. */
void CCommentaryReadWindow::refresh(){
  //refresh the book lists
  verseKey()->setLocale( backend()->booknameLanguage().latin1() );
  keyChooser()->refreshContent();

  lookup(key());
}

/** rapper around key() to return the right type of key. */
CSwordVerseKey* CCommentaryReadWindow::verseKey(){
//  qWarning("CCommentaryReadWindow::key()");
	CSwordVerseKey* k = dynamic_cast<CSwordVerseKey*>(CDisplayWindow::key());
// 	Q_ASSERT(k);
	return k;
}


/** Reimplementation. */
void CCommentaryReadWindow::insertKeyboardActions( KAccel* const a ){
  a->insert("Next book",        i18n("Next book"),        "", IDK_PRESENTER_NEXT_BOOK,        0, "");
	a->insert("Previous book",    i18n("Previous book"),    "", IDK_PRESENTER_PREVIOUS_BOOK,    0, "");
	a->insert("Next chapter",     i18n("Next chapter"),     "", IDK_PRESENTER_NEXT_CHAPTER,     0, "");
	a->insert("Previous chapter", i18n("Previous chapter"), "", IDK_PRESENTER_PREVIOUS_CHAPTER, 0, "");
	a->insert("Next verse",       i18n("Next verse"),       "", IDK_PRESENTER_NEXT_VERSE,       0, "");
	a->insert("Previous verse",   i18n("Previous verse"),   "", IDK_PRESENTER_PREVIOUS_VERSE,   0, "");
}

void CCommentaryReadWindow::initKeyboardActions() {
  CReadWindow::initKeyboardActions();

  CBTConfig::setupAccel( CBTConfig::bibleWindow, accel() );
  CReadWindow::insertKeyboardActions(accel());  
  insertKeyboardActions( accel() );

  accel()->readSettings();

	accel()->setSlot("Next book", this, SLOT(nextBook()));
  accel()->setSlot("Previous book", this, SLOT(previousBook()));
  accel()->setSlot("Next chapter", this, SLOT(nextChapter()));
  accel()->setSlot("Previous chapter", this, SLOT(previousChapter()));
  accel()->setSlot("Next verse", this, SLOT(nextVerse()));
  accel()->setSlot("Previous verse", this, SLOT(previousVerse()));
}

/** Moves to the next book. */
void CCommentaryReadWindow::nextBook(){
	if (verseKey()->next(CSwordVerseKey::UseBook))
		keyChooser()->setKey(key());
}

/** Moves one book behind. */
void CCommentaryReadWindow::previousBook(){
	if (verseKey()->previous(CSwordVerseKey::UseBook))
		keyChooser()->setKey(key());
}

/** Moves to the next book. */
void CCommentaryReadWindow::nextChapter(){
	if (verseKey()->next(CSwordVerseKey::UseChapter))
		keyChooser()->setKey(key());
}

/** Moves one book behind. */
void CCommentaryReadWindow::previousChapter(){
	if (verseKey()->previous(CSwordVerseKey::UseChapter))
		keyChooser()->setKey(key());
}

/** Moves to the next book. */
void CCommentaryReadWindow::nextVerse(){
	if (verseKey()->next(CSwordVerseKey::UseVerse))
		keyChooser()->setKey(key());
}

/** Moves one book behind. */
void CCommentaryReadWindow::previousVerse(){
	if (verseKey()->previous(CSwordVerseKey::UseVerse))
		keyChooser()->setKey(key());
}
