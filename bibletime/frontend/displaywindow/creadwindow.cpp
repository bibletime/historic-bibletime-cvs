/***************************************************************************
                          creadwindow.cpp  -  description
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
#include "creadwindow.h"
#include "resource.h"

#include "backend/centrydisplay.h"
#include "backend/cswordkey.h"

#include "frontend/cbtconfig.h"
#include "frontend/cexportmanager.h"
#include "frontend/cprofilewindow.h"
#include "frontend/display/creaddisplay.h"
#include "frontend/displaywindow/cmodulechooserbar.h"
#include "frontend/keychooser/ckeychooser.h"


//KDE includes
#include <kpopupmenu.h>
#include <kaccel.h>
#include <kstdaccel.h>
#include <klocale.h>

CReadWindow::CReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name ) : CDisplayWindow(modules,parent,name) {
//	qWarning("constructor of CReadWindow");
 	m_popupMenu = 0;
  m_displayWidget = 0;
}

CReadWindow::~CReadWindow(){
//	qWarning("destructor of CReadWindow");
}

/** Returns the display widget of this window. */
CReadDisplay* const CReadWindow::displayWidget(){
//  qWarning("CReadWindow::displayWidget()");
//	Q_ASSERT(m_displayWidget);
	return m_displayWidget;
}

/** Sets the display widget of this display window. */
void CReadWindow::setDisplayWidget( CReadDisplay* newDisplay ){
//	qWarning("CReadWindow::setDisplayWidget()"); 	
 	if (m_displayWidget) {
  	disconnect(m_displayWidget->connectionsProxy(), SIGNAL(referenceClicked(const QString&, const QString&)), this, SLOT(lookup(const QString&, const QString&)));
  	disconnect(m_displayWidget->connectionsProxy(), SIGNAL(referenceDropped(const QString&)), this, SLOT(lookup(const QString&)));
  }

	m_displayWidget = newDisplay;
  connect(m_displayWidget->connectionsProxy(), SIGNAL(referenceClicked(const QString&, const QString&)),this, SLOT(lookup(const QString&, const QString&)));
  connect(m_displayWidget->connectionsProxy(), SIGNAL(referenceDropped(const QString&)), this, SLOT(lookup(const QString&)));
}

/** Lookup the given entry. */
void CReadWindow::lookup( CSwordKey* newKey ){
//	qWarning("CReadWindow::lookup");	
	if (!newKey)
		return;

	if (CEntryDisplay* display = modules().first()->getDisplay()) {	//do we have a display object?
 		displayWidget()->setText( display->text( modules(), newKey->key(), displayOptions(), filterOptions() ) );
	}	
	if (key() != newKey)
		key()->key(newKey->key());
	
  displayWidget()->moveToAnchor( key()->key() );
	setCaption( windowCaption() );
}

/** Returns the installed popup menu. */
KPopupMenu* const CReadWindow::popup(){
//	qWarning("CReadWindow::popup()");
	if (!m_popupMenu) {
 		m_popupMenu = new KPopupMenu(this);
		connect(m_popupMenu, SIGNAL(aboutToShow()), this, SLOT(updatePopupMenu()));
	  if (displayWidget())
	  	displayWidget()->installPopup(m_popupMenu);
//	  else
//	  	qWarning("CAN't INSTALL POPUP");
  }
 	return m_popupMenu;
}

/** Update the status of the popup menu entries. */
void CReadWindow::updatePopupMenu(){

}

/** Reimplementation to use the popup menu. */
const bool CReadWindow::init( const QString& keyName ){
  qWarning("CReadWindow::init( const QString& keyName )");
  CDisplayWindow::init(keyName);
 	setupPopupMenu();
  keyChooser()->setKey(key());
	setReady(true);
}

/** Store the settings of this window in the given CProfileWindow object. */
void CReadWindow::storeProfileSettings(CProfileWindow * const settings){
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
		VerseKey* vk = dynamic_cast<VerseKey*>(key());
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
}

void CReadWindow::applyProfileSettings(CProfileWindow * const settings){
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
}

void CReadWindow::insertKeyboardActions( KAccel* const a ){
  a->insert("Copy", i18n("Copy selected text"),"", KStdAccel::copy(), 0, "");
}


void CReadWindow::initKeyboardActions() {
  CBTConfig::setupAccel( CBTConfig::readWindow, accel() );	
  insertKeyboardActions( accel() );

  accel()->readSettings();
	accel()->setSlot("Copy", displayWidget()->connectionsProxy(), SLOT(copySelection()));
}

/** No descriptions */
void CReadWindow::copyDisplayedText(){
  CExportManager mgr(QString::null);
  mgr.copyKey(key(),CExportManager::Text,true);
}
