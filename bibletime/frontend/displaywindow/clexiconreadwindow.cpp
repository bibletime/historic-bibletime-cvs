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
#include "backend/chtmlentrydisplay.h"

#include "frontend/ctoolclass.h"
#include "frontend/cbtconfig.h"
#include "frontend/display/cdisplay.h"
#include "frontend/display/creaddisplay.h"
#include "frontend/keychooser/ckeychooser.h"

#include "resource.h"

//Qt includes

//KDE includes
#include <kaccel.h>
#include <ktoolbar.h>
#include <klocale.h>
#include <kpopupmenu.h>

CLexiconReadWindow::CLexiconReadWindow(ListCSwordModuleInfo moduleList, CMDIArea* parent, const char *name) : CReadWindow(moduleList, parent,name) {
	setKey( CSwordKey::createInstance(moduleList.first()) );
 	key()->key("");
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
	a->insert("Previous entry", i18n("Previous entry"), "", IDK_PRESENTER_NEXT_ENTRY, 0, "", true, true);	
}

void CLexiconReadWindow::initKeyboardActions() {
  CReadWindow::initKeyboardActions();


  CBTConfig::setupAccel(CBTConfig::lexiconWindow, accel());
  insertKeyboardActions(accel());
  accel()->readSettings();

  accel()->setSlot("Next entry", this, SLOT(nextEntry()));
  accel()->setSlot("Previous entry",  this, SLOT(previousEntry()));
};

/** No descriptions */
void CLexiconReadWindow::initConnections(){
//	connect(displayWidget(), SIGNAL(referenceClicked(const QString&, const QString&)),
//		this, SLOT(lookup(const QString&, const QString&)));
//	connect(displayWidget(), SIGNAL(referenceDropped(const QString&, const QString&)),
//		this, SLOT(referenceDropped(const QString&, const QString&)));

 	connect(keyChooser(), SIGNAL(keyChanged(CSwordKey*)),
		this, SLOT(lookup(CSwordKey*)));
}

void CLexiconReadWindow::initView(){
	qWarning("CLexiconReadWindow::initView()");
 	setMainToolBar( new KToolBar(this) );
 	Q_ASSERT( mainToolBar() );
	addDockWindow(mainToolBar());

	setKeyChooser( CKeyChooser::createInstance(modules().first(), key(), mainToolBar()) );
	mainToolBar()->insertWidget(0,keyChooser()->sizeHint().width(),keyChooser());	
	mainToolBar()->setItemAutoSized(0);

	setModuleChooserBar( new CModuleChooserBar(modules(), modules().first()->type(), this) );
 	addDockWindow( moduleChooserBar() );
	setDisplayWidget( CDisplay::createReadInstance(this) ); 	
 	setCentralWidget( displayWidget()->view() );

	setIcon(CToolClass::getIconForModule(modules().first()));
}

void CLexiconReadWindow::setupPopupMenu(){
	popup()->insertTitle(CToolClass::getIconForModule(modules().first()), i18n("Bible window"));

 	m_actions.selectAll = new KAction(i18n("Select all"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(selectAll()), actionCollection());
  m_actions.selectAll->plug(popup());

  (new KActionSeparator())->plug( popup() );

 	m_actions.copyMenu = new KActionMenu(i18n("Copy..."), ICON_EDIT_COPY);	
 	m_actions.copy.entryOnly = new KAction(i18n("Entry"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAnchorOnly()), actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.entryOnly);
	m_actions.copy.textOnly = new KAction(i18n("Text of entry"), KShortcut(0),displayWidget()->connectionsProxy(), SLOT(copyAnchorTextOnly()), actionCollection());	
 	m_actions.copyMenu->insert(m_actions.copy.textOnly);
 	m_actions.copy.entryAndText = new KAction(i18n("Entry with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAnchorWithText()), actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.entryAndText); 	
  m_actions.copyMenu->insert(new KActionSeparator());	
	m_actions.copy.selectedText = new KAction(i18n("Selected text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copySelection()),actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.selectedText);
 	m_actions.copyMenu->plug(popup());

 	m_actions.saveMenu = new KActionMenu(i18n("Save..."),ICON_FILE_SAVE);	
	m_actions.save.entryAsPlain = new KAction(i18n("Entry as plain text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(saveAsPlain()),actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.entryAsPlain);
 	m_actions.save.entryAsHTML = new KAction(i18n("Entry as HTML"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(saveAsHTML()),actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.entryAsHTML);
 	m_actions.saveMenu->plug(popup());

 	m_actions.printMenu = new KActionMenu(i18n("Print..."),ICON_FILE_PRINT);	
 	m_actions.print.entryAndText = new KAction(i18n("Entry with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(printAnchorWithText()), actionCollection());
 	m_actions.printMenu->insert(m_actions.print.entryAndText); 	
 	m_actions.printMenu->plug(popup());
}

/** Reimplemented. */
void CLexiconReadWindow::updatePopupMenu(){
 	
  m_actions.copy.entryOnly->setEnabled( displayWidget()->hasActiveAnchor() );	
 	m_actions.copy.textOnly->setEnabled( displayWidget()->hasActiveAnchor() );	
 	m_actions.copy.entryAndText->setEnabled( displayWidget()->hasActiveAnchor() );
	m_actions.copy.selectedText->setEnabled( displayWidget()->hasSelection() );
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
  qWarning("CLexiconReadWindow::key()");
	CSwordLDKey* k = dynamic_cast<CSwordLDKey*>(CDisplayWindow::key());
 	Q_ASSERT(k);
	return k;
}