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

#include "frontend/ctoolclass.h"
#include "frontend/cprofilewindow.h"
#include "frontend/keychooser/ckeychooser.h"
#include "frontend/display/cdisplay.h"
#include "frontend/display/creaddisplay.h"

//Qt includes

//KDE includes
#include <kaction.h>
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
//  qWarning("CBibleReadWindow::key()");
	CSwordVerseKey* k = dynamic_cast<CSwordVerseKey*>(CDisplayWindow::key());
// 	Q_ASSERT(k);
	return k;
}
