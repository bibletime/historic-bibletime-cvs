/***************************************************************************
                          cbookreadwindow.cpp  -  description
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

#include "cbookreadwindow.h"
#include "cmodulechooserbar.h"

#include "backend/cswordtreekey.h"

#include "frontend/display/cdisplay.h"
#include "frontend/keychooser/cbooktreechooser.h"
#include "frontend/cprofilewindow.h"
#include "frontend/cresmgr.h"

#include "util/ctoolclass.h"

//Qt includes
#include <qsplitter.h>

//KDE includes
#include <kaction.h>
#include <klocale.h>

CBookReadWindow::CBookReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name) : CLexiconReadWindow(modules, parent, name){
}

CBookReadWindow::~CBookReadWindow(){
}

void CBookReadWindow::applyProfileSettings( CProfileWindow* profileWindow ) {
  CLexiconReadWindow::applyProfileSettings(profileWindow);
  const bool enable = static_cast<bool>( profileWindow->windowSettings() );
//	m_treeAction->setChecked( enable );
  if (enable) {
    m_treeAction->activate();
  }
};

void CBookReadWindow::storeProfileSettings( CProfileWindow* profileWindow ) {
  CLexiconReadWindow::storeProfileSettings(profileWindow);

  //store information about our show tree structure button
	profileWindow->setWindowSettings( static_cast<int>( m_treeAction->isChecked() ) );
};

void CBookReadWindow::initKeyboardActions() {
  CReadWindow::initKeyboardActions();
};

void CBookReadWindow::insertKeyboardActions( KAccel* /*a*/ ){
//  CReadWindow::insertKeyboardActions(a);
};

/** No descriptions */
void CBookReadWindow::initConnections(){
//	qWarning("CBookReadWindow::initView()");
	CLexiconReadWindow::initConnections();
 	connect(m_treeChooser, SIGNAL(keyChanged(CSwordKey*)),
 		this, SLOT(lookup(CSwordKey*)));
 	connect(m_treeChooser, SIGNAL(keyChanged(CSwordKey*)),
 		keyChooser(), SLOT(updateKey(CSwordKey*)));
 	connect(keyChooser(), SIGNAL(keyChanged(CSwordKey*)),
 		m_treeChooser, SLOT(updateKey(CSwordKey*)));
}

/** Init the view */
void CBookReadWindow::initView(){
  QSplitter* splitter = new QSplitter(this);
  setMainToolBar( new KToolBar(this) );
	addDockWindow(mainToolBar());

  setKeyChooser( CKeyChooser::createInstance(modules(), key(), mainToolBar()) );
	mainToolBar()->insertWidget(0,keyChooser()->sizeHint().width(),keyChooser());
 	mainToolBar()->setFullSize(false);
  
  setModuleChooserBar( new CModuleChooserBar(modules(), modules().first()->type(), mainToolBar()) );
	moduleChooserBar()->setButtonLimit(1);
	mainToolBar()->insertWidget(1,moduleChooserBar()->sizeHint().width(),moduleChooserBar());
  
  m_treeAction = new KToggleAction(i18n("Toggle tree..."), CResMgr::displaywindows::bookWindow::toggleTree::icon, CResMgr::displaywindows::bookWindow::toggleTree::accel, this, SLOT(treeToggled()), actionCollection(), "treeToggle_action");
	m_treeAction->plug(mainToolBar());

	m_treeChooser = new CBookTreeChooser(modules(), key(), splitter);
  setDisplayWidget( CDisplay::createReadInstance(this, splitter) ); 	 	

  m_treeAction->setChecked(false);
	m_treeChooser->hide();
  	
  splitter->setResizeMode(m_treeChooser, QSplitter::FollowSizeHint);
  setCentralWidget( splitter );
	setIcon(CToolClass::getIconForModule(modules().first()));
}

/** Is called when the action was executed to toggle the tree view. */
void CBookReadWindow::treeToggled(){
	if (m_treeAction->isChecked())
		m_treeChooser->show();
	else
		m_treeChooser->hide();
}

/** Reimplementation to take care of the tree chooser. */
void CBookReadWindow::modulesChanged(){
  CLexiconReadWindow::modulesChanged();
  m_treeChooser->setModules(modules());
}


//void CBookReadWindow::setupPopupMenu(){
//	popup()->insertTitle(CToolClass::getIconForModule(modules().first()), i18n("Lexicon window"));
//
// 	m_actions.selectAll = new KAction(i18n("Select all"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(selectAll()), actionCollection());
//  m_actions.selectAll->plug(popup());
//
//  (new KActionSeparator())->plug( popup() );
//
// 	m_actions.copyMenu = new KActionMenu(i18n("Copy..."), ICON_EDIT_COPY);
//
//  m_actions.copy.reference = new KAction(i18n("Reference"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAnchorOnly()), actionCollection());
// 	m_actions.copyMenu->insert(m_actions.copy.reference);
//
//  m_actions.copy.entry = new KAction(i18n("Entry with text"), KShortcut(0), this, SLOT(copyDisplayedText()), actionCollection());
// 	m_actions.copyMenu->insert(m_actions.copy.entry);
//  m_actions.copyMenu->insert(new KActionSeparator());
//	m_actions.copy.selectedText = new KAction(i18n("Selected text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copySelection()),actionCollection());
// 	m_actions.copyMenu->insert(m_actions.copy.selectedText);
// 	m_actions.copyMenu->plug(popup());
//
// 	m_actions.saveMenu = new KActionMenu(i18n("Save..."),ICON_FILE_SAVE);
//	m_actions.save.entryAsPlain = new KAction(i18n("Entry as plain text"), KShortcut(0), this, SLOT(saveAsPlain()),actionCollection());
// 	m_actions.saveMenu->insert(m_actions.save.entryAsPlain);
// 	m_actions.save.entryAsHTML = new KAction(i18n("Entry as HTML"), KShortcut(0), this, SLOT(saveAsHTML()),actionCollection());
// 	m_actions.saveMenu->insert(m_actions.save.entryAsHTML);
// 	m_actions.saveMenu->plug(popup());
//
// 	m_actions.printMenu = new KActionMenu(i18n("Print..."),ICON_FILE_PRINT);
// 	m_actions.print.reference = new KAction(i18n("Reference"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(printAnchorWithText()), actionCollection());
// 	m_actions.printMenu->insert(m_actions.print.reference);
//  m_actions.print.entry = new KAction(i18n("Entry with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(printAll()), actionCollection());
// 	m_actions.printMenu->insert(m_actions.print.entry);
// 	m_actions.printMenu->plug(popup());
//}
//
///** Saves the displayed page as plain text. */
//void CBookReadWindow::saveAsPlain(){
//
//}
//
///** Saves the current text as as HTML page. */
//void CBookReadWindow::saveAsHTML(){
//
//}
