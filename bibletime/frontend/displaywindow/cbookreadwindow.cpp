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

#include "backend/cswordtreekey.h"

CBookReadWindow::CBookReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name) : CLexiconReadWindow(modules, parent, name){
}

CBookReadWindow::~CBookReadWindow(){
}

void CBookReadWindow::applyProfileSettings( CProfileWindow* profileWindow ) {

};

void CBookReadWindow::storeProfileSettings( CProfileWindow* profileWindow ) {

};

void CBookReadWindow::initKeyboardActions() {

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
}

/** Reimplementation. */
void CBookReadWindow::setupPopupMenu(){
}
