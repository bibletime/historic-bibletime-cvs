/***************************************************************************
                          clexiconreadwindow.cpp  -  description
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
#include "clexiconreadwindow.h"
#include "cmodulechooserbar.h"

#include "backend/cswordkey.h"
#include "backend/cswordldkey.h"

#include "frontend/cbtconfig.h"
#include "frontend/cexportmanager.h"
#include "frontend/display/cdisplay.h"
#include "frontend/display/creaddisplay.h"
#include "frontend/keychooser/ckeychooser.h"

#include "util/ctoolclass.h"

#include "resource.h"

//Qt includes

//KDE includes
#include <kaccel.h>
#include <ktoolbar.h>
#include <klocale.h>
#include <kpopupmenu.h>

CLexiconReadWindow::CLexiconReadWindow(ListCSwordModuleInfo moduleList, CMDIArea* parent, const char *name) : CReadWindow(moduleList, parent,name) {
	setKey( CSwordKey::createInstance(moduleList.first()) );
// 	key()->key("");
}

CLexiconReadWindow::~CLexiconReadWindow(){
}

void CLexiconReadWindow::applyProfileSettings( CProfileWindow* profileWindow ) {
	CReadWindow::applyProfileSettings(profileWindow);
};

void CLexiconReadWindow::storeProfileSettings( CProfileWindow* profileWindow ) {
	CReadWindow::storeProfileSettings(profileWindow);
};

/** Reimplementation. */
void CLexiconReadWindow::insertKeyboardActions( KAccel* a ){
  a->insert("Next entry",     i18n("Next entry"),     "", IDK_PRESENTER_NEXT_ENTRY, 0, "", true, true);
	a->insert("Previous entry", i18n("Previous entry"), "", IDK_PRESENTER_PREVIOUS_ENTRY, 0, "", true, true);	
}

void CLexiconReadWindow::initKeyboardActions() {
  CReadWindow::initKeyboardActions();

  CBTConfig::setupAccel(CBTConfig::lexiconWindow, accel());
  insertKeyboardActions(accel());
  CReadWindow::insertKeyboardActions(accel());

  accel()->readSettings();

  accel()->setSlot("Next entry", this, SLOT(nextEntry()));
  accel()->setSlot("Previous entry",  this, SLOT(previousEntry()));
};

/** No descriptions */
void CLexiconReadWindow::initConnections(){
 	connect(keyChooser(), SIGNAL(keyChanged(CSwordKey*)),
		this, SLOT(lookup(CSwordKey*)));
}

void CLexiconReadWindow::initView(){
	setDisplayWidget( CDisplay::createReadInstance(this) );
 	setCentralWidget( displayWidget()->view() );


  setMainToolBar( new KToolBar(this) );
  mainToolBar()->setFullSize(true);
	addDockWindow(mainToolBar());

	setKeyChooser( CKeyChooser::createInstance(modules(), key(), mainToolBar()) );
	mainToolBar()->insertWidget(0,keyChooser()->sizeHint().width(),keyChooser());
 	mainToolBar()->setFullSize(false);

	setModuleChooserBar( new CModuleChooserBar(modules(), modules().first()->type(), mainToolBar()) );
	mainToolBar()->insertWidget(1,moduleChooserBar()->sizeHint().width(),moduleChooserBar());	  
  
	setIcon(CToolClass::getIconForModule(modules().first()));
}

void CLexiconReadWindow::setupPopupMenu(){
	popup()->insertTitle(CToolClass::getIconForModule(modules().first()), i18n("Lexicon window"));

 	m_actions.selectAll = new KAction(i18n("Select all"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(selectAll()), actionCollection());
  m_actions.selectAll->plug(popup());

  (new KActionSeparator())->plug( popup() );

 	m_actions.copyMenu = new KActionMenu(i18n("Copy..."), ICON_EDIT_COPY);	
 	
  m_actions.copy.reference = new KAction(i18n("Reference"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAnchorOnly()), actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.reference);

  m_actions.copy.entry = new KAction(i18n("Entry with text"), KShortcut(0), this, SLOT(copyDisplayedText()), actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.entry);
  m_actions.copyMenu->insert(new KActionSeparator());	
	m_actions.copy.selectedText = new KAction(i18n("Selected text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copySelection()),actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.selectedText);
 	m_actions.copyMenu->plug(popup());

 	m_actions.saveMenu = new KActionMenu(i18n("Save..."),ICON_FILE_SAVE);	
	m_actions.save.entryAsPlain = new KAction(i18n("Entry as plain text"), KShortcut(0), this, SLOT(saveAsPlain()),actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.entryAsPlain);
 	m_actions.save.entryAsHTML = new KAction(i18n("Entry as HTML"), KShortcut(0), this, SLOT(saveAsHTML()),actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.entryAsHTML);
 	m_actions.saveMenu->plug(popup());

 	m_actions.printMenu = new KActionMenu(i18n("Print..."),ICON_FILE_PRINT);	
 	m_actions.print.reference = new KAction(i18n("Reference"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(printAnchorWithText()), actionCollection());
 	m_actions.printMenu->insert(m_actions.print.reference); 	 	
  m_actions.print.entry = new KAction(i18n("Entry with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(printAll()), actionCollection());
 	m_actions.printMenu->insert(m_actions.print.entry); 	
 	m_actions.printMenu->plug(popup());
}

/** Reimplemented. */
void CLexiconReadWindow::updatePopupMenu(){
 	m_actions.copy.reference->setEnabled( displayWidget()->hasActiveAnchor() );
	m_actions.copy.selectedText->setEnabled( displayWidget()->hasSelection() );

 	m_actions.print.reference->setEnabled( displayWidget()->hasActiveAnchor() );
}

/** No descriptions */
void CLexiconReadWindow::nextEntry(){
  keyChooser()->setKey(ldKey()->NextEntry());
}

/** No descriptions */
void CLexiconReadWindow::previousEntry(){
  keyChooser()->setKey(ldKey()->PreviousEntry());
}

/** Reimplementation to return the right key. */
CSwordLDKey* CLexiconReadWindow::ldKey(){
  return dynamic_cast<CSwordLDKey*>(CDisplayWindow::key());
}

/** This function saves the entry as html using the CExportMgr class. */
void CLexiconReadWindow::saveAsHTML(){
  CExportManager mgr(i18n("Saving entry ..."), true, i18n("Saving"), filterOptions(), displayOptions());
  mgr.saveKey(key(), CExportManager::HTML, true);  
}

/** This function saves the entry as html using the CExportMgr class. */
void CLexiconReadWindow::saveAsPlain(){
  CExportManager mgr(i18n("Saving entry ..."), true, i18n("Saving"), filterOptions(), displayOptions());
  mgr.saveKey(key(), CExportManager::Text, true);
}