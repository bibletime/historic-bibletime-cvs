/***************************************************************************
                          cplainwritewindow.cpp  -  description
                             -------------------
    begin                : Fre Okt 11 2002
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

#include "cplainwritewindow.h"
#include "frontend/keychooser/ckeychooser.h"

CPlainWriteWindow::CPlainWriteWindow(ListCSwordModuleInfo moduleList, CMDIArea* parent, const char *name ) : CWriteWindow(moduleList, parent, name) {
  qWarning("CPlainWriteWindow::CPlainWriteWindow");
	setKey( CSwordKey::createInstance(moduleList.first()) );  
}


CPlainWriteWindow::~CPlainWriteWindow(){
}

/** Initialize the state of this widget. */
void CPlainWriteWindow::initView(){
  qWarning("CPlainWriteWindow::initView()");
 	setDisplayWidget( CDisplay::createWriteInstance(this) );
  Q_ASSERT(displayWidget());
 	setCentralWidget( displayWidget()->view() );

  setMainToolBar( new KToolBar(this) );
  mainToolBar()->setFullSize(true);
	addDockWindow(mainToolBar());

	setKeyChooser( CKeyChooser::createInstance(modules(), key(), mainToolBar()) );
	mainToolBar()->insertWidget(0,keyChooser()->sizeHint().width(),keyChooser());
 	mainToolBar()->setFullSize(false);  
}

void CPlainWriteWindow::initConnections(){
 	connect(keyChooser(), SIGNAL(keyChanged(CSwordKey*)),
		this, SLOT(lookup(CSwordKey*)));
}
