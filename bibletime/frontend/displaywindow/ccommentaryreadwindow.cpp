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
#include "frontend/display/cdisplay.h"
#include "frontend/display/creaddisplay.h"

//Qt includes

//KDE includes
#include <kaction.h>
#include <klocale.h>
#include <kpopupmenu.h>

CCommentaryReadWindow::CCommentaryReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name) : CLexiconReadWindow(modules, parent,name) {
}

CCommentaryReadWindow::~CCommentaryReadWindow(){
}

void CCommentaryReadWindow::applyProfileSettings( CProfileWindow* profileWindow ) {

};

void CCommentaryReadWindow::storeProfileSettings( CProfileWindow* profileWindow ) {

};

void CCommentaryReadWindow::initKeyboardActions() {
	CLexiconReadWindow::initKeyboardActions();
};

void CCommentaryReadWindow::initConnections(){
	CLexiconReadWindow::initConnections();
}

void CCommentaryReadWindow::initView(){
	CLexiconReadWindow::initView();
}

///** Reimplementation. */
//void CCommentaryReadWindow::setupPopupMenu(){
//	popup()->insertTitle(CToolClass::getIconForModule(modules().first()), i18n("Bible window"));
//
// 	m_actions.selectAll = new KAction(i18n("Select all"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(selectAll()), actionCollection());
//  m_actions.selectAll->plug(popup());
//
//  (new KActionSeparator())->plug( popup() );
//
// 	m_actions.copyMenu = new KActionMenu(i18n("Copy..."), ICON_EDIT_COPY);	
// 	m_actions.copy.verseOnly = new KAction(i18n("Verse"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAnchorOnly()), actionCollection());
// 	m_actions.copyMenu->insert(m_actions.copy.verseOnly);
//	m_actions.copy.textOnly = new KAction(i18n("Text of verse"), KShortcut(0),displayWidget()->connectionsProxy(), SLOT(copyAnchorTextOnly()), actionCollection());	
// 	m_actions.copyMenu->insert(m_actions.copy.textOnly);
// 	m_actions.copy.verseAndText = new KAction(i18n("Verse with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAnchorWithText()), actionCollection());
// 	m_actions.copyMenu->insert(m_actions.copy.verseAndText); 	
// 	m_actions.copy.entry = new KAction(i18n("Entry"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAll()), actionCollection());
// 	m_actions.copyMenu->insert(m_actions.copy.entry); 	
//  m_actions.copyMenu->insert(new KActionSeparator());	
//	m_actions.copy.selectedText = new KAction(i18n("Selected text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copySelection()),actionCollection());
// 	m_actions.copyMenu->insert(m_actions.copy.selectedText);
// 	m_actions.copyMenu->plug(popup());
//
// 	m_actions.saveMenu = new KActionMenu(i18n("Save..."),ICON_FILE_SAVE);	
// 	m_actions.save.verseAndText = new KAction(i18n("Verse with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(saveAnchorWithText()), actionCollection());
// 	m_actions.saveMenu->insert(m_actions.save.verseAndText);
//	m_actions.save.entryAsPlain = new KAction(i18n("Entry as plain text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(saveAsPlain()),actionCollection());
// 	m_actions.saveMenu->insert(m_actions.save.entryAsPlain);
// 	m_actions.save.entryAsHTML = new KAction(i18n("Entry as HTML"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(saveAsHTML()),actionCollection());
// 	m_actions.saveMenu->insert(m_actions.save.entryAsHTML);
// 	m_actions.saveMenu->plug(popup());
//
// 	m_actions.printMenu = new KActionMenu(i18n("Print..."),ICON_FILE_PRINT);	
// 	m_actions.print.verseAndText = new KAction(i18n("Verse with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(printAnchorWithText()), actionCollection());
// 	m_actions.printMenu->insert(m_actions.print.verseAndText); 	
//  m_actions.print.entry = new KAction(i18n("Entry"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(printAll()),actionCollection());
// 	m_actions.printMenu->insert(m_actions.print.entry);
// 	m_actions.printMenu->plug(popup());
//}
//
///** Reimplementation to update the actions of the popup menu. */
//void CCommentaryReadWindow::updatePopupMenu(){	
//  m_actions.copy.verseOnly->setEnabled( displayWidget()->hasActiveAnchor() );	
// 	m_actions.copy.textOnly->setEnabled( displayWidget()->hasActiveAnchor() );	
// 	m_actions.copy.verseAndText->setEnabled( displayWidget()->hasActiveAnchor() );
//	m_actions.copy.selectedText->setEnabled( displayWidget()->hasSelection() );
//
// 	m_actions.save.verseAndText->setEnabled( displayWidget()->hasActiveAnchor() );
// 	
//  m_actions.print.verseAndText->setEnabled( displayWidget()->hasActiveAnchor() );
//}
