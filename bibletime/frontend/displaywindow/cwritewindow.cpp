/***************************************************************************
                          cwritewindow.cpp  -  description
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

#include "cwritewindow.h"

#include "backend/cswordmoduleinfo.h"

#include "frontend/keychooser/ckeychooser.h"
#include "frontend/cprofilewindow.h"


CWriteWindow::CWriteWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name )
  : CDisplayWindow(modules, parent,name), m_writeDisplay(0) {
}

CWriteWindow::~CWriteWindow(){
}


void CWriteWindow::insertKeyboardActions( KAccel* const a ) {

};

const bool CWriteWindow::init( const QString& keyName ) {
  qWarning("CWriteWindow::init( const QString& keyName )");
  CDisplayWindow::init(keyName);
// 	setupPopupMenu();
  keyChooser()->setKey(key());
	setReady(true);
  return true;
};


void CWriteWindow::initConnections() {

};

void CWriteWindow::initKeyboardActions() {

};

void CWriteWindow::storeProfileSettings(CProfileWindow * const settings) {

  settings->setIsWriteWindow(true);
  
	QRect rect;
	rect.setX(parentWidget()->x());
	rect.setY(parentWidget()->y());
	rect.setWidth(width());
	rect.setHeight(height());
	settings->setGeometry(rect);

//	settings->setScrollbarPositions( m_htmlWidget->view()->horizontalScrollBar()->value(), m_htmlWidget->view()->verticalScrollBar()->value() );
	settings->setType(modules().first()->type());
	settings->setMaximized(isMaximized() || parentWidget()->isMaximized());

	if (key()) {
		sword::VerseKey* vk = dynamic_cast<sword::VerseKey*>(key());
		QString oldLang;
		if (vk) {
			 oldLang = QString::fromLatin1(vk->getLocale());
			vk->setLocale("en"); //save english locale names as default!
		}
		settings->setKey( key()->key() );
		if (vk) {
			vk->setLocale(oldLang.latin1());
		}
	}

	QStringList mods;
	for (CSwordModuleInfo* m = modules().first(); m; m = modules().next()) {
		mods.append(m->name());
	}
	settings->setModules(mods);
};

void CWriteWindow::applyProfileSettings(CProfileWindow * const settings) {
	setUpdatesEnabled(false);

	if (settings->maximized()) {
		showMaximized();
	}
	else {
		const QRect rect = settings->geometry();
		resize(rect.width(), rect.height());
		parentWidget()->move(rect.x(), rect.y());
		//setGeometry( settings->geometry() );
	}
//	displayWidget()->view()->horizontalScrollBar()->setValue( settings->scrollbarPositions().horizontal );
//	m_htmlWidget->view()->verticalScrollBar()->setValue( settings->scrollbarPositions().vertical );

	setUpdatesEnabled(true);
};

/** Sets the write display-widget for this write display window. */
void CWriteWindow::setDisplayWidget( CWriteDisplay* display ){
  CDisplayWindow::setDisplayWidget(display);
  m_writeDisplay = display;
}

/** Look up the given key and display the text. In our case we offer to edit the text. */
void CWriteWindow::lookup( CSwordKey* newKey ){
  //set the raw text to the display widget
	if (!newKey)
		return;

//	if (CEntryDisplay* display = modules().first()->getDisplay()) {	//do we have a display object?
// 		displayWidget()->setText( display->text( modules(), newKey->key(), displayOptions(), filterOptions() ) );
//	}
  if (CSwordModuleInfo* module = modules().first()) {
    displayWidget()->setText( newKey->rawText() );
  }
  
	if (key() != newKey)
		key()->key(newKey->key());

	setCaption( windowCaption() );
  
}

/** Returns the write display widget used by this window. */
CWriteDisplay* const CWriteWindow::displayWidget(){
  return m_writeDisplay;
}
