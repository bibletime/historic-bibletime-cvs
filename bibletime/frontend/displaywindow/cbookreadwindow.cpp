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

#include "util/ctoolclass.h"

#include "resource.h"

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

void CBookReadWindow::insertKeyboardActions( KAccel* a ){
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
  
  m_treeAction = new KToggleAction(i18n("Toggle tree..."), ICON_VIEW_BOOKTREE, IDK_PRESENTER_TOGGLE_TREE, this, SLOT(treeToggled()), actionCollection(), "treeToggle_action");
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