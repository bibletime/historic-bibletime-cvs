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

#include "ccommentaryreadwindow.h"

CCommentaryReadWindow::CCommentaryReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name) : CBibleReadWindow(modules, parent,name) {
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
