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


CBookReadWindow::CBookReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name) : CLexiconReadWindow(modules, parent, name){
}

CBookReadWindow::~CBookReadWindow(){
}

void CBookReadWindow::applyProfileSettings( CProfileWindow* profileWindow ) {
  CLexiconReadWindow::applyProfileSettings(profileWindow);
};

void CBookReadWindow::storeProfileSettings( CProfileWindow* profileWindow ) {
  CLexiconReadWindow::storeProfileSettings(profileWindow);
};

void CBookReadWindow::initKeyboardActions() {
  CReadWindow::initKeyboardActions();
};

void CBookReadWindow::insertKeyboardActions( KAccel* a ){
//  CReadWindow::insertKeyboardActions(a);
};

/** No descriptions */
void CBookReadWindow::initConnections(){
	qWarning("CBookReadWindow::initView()");
	CLexiconReadWindow::initConnections();
}

/** Init the view */
void CBookReadWindow::initView(){
	qWarning("CBookReadWindow::initView()");	
	CLexiconReadWindow::initView();
	moduleChooserBar()->setButtonLimit(1);
}

/** Reimplementation. */
void CBookReadWindow::setupPopupMenu(){
	CLexiconReadWindow::setupPopupMenu();
}
