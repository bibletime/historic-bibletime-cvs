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

#include "backend/centrydisplay.h"
#include "backend/cdisplayrendering.h"
#include "backend/cswordkey.h"

#include "frontend/cbtconfig.h"
#include "frontend/cexportmanager.h"
#include "frontend/cmdiarea.h"
#include "frontend/cprofilewindow.h"
#include "frontend/displaywindow/cmodulechooserbar.h"
#include "frontend/keychooser/ckeychooser.h"


//KDE includes
#include <kpopupmenu.h>
#include <kaccel.h>
#include <kstdaccel.h>
#include <klocale.h>

CReadWindow::CReadWindow(ListCSwordModuleInfo modules, CMDIArea* parent, const char *name )
		: CDisplayWindow(modules,parent,name),
m_displayWidget(0) {
}

CReadWindow::~CReadWindow() {
	//	qWarning("destructor of CReadWindow");
}

/** Returns the display widget of this window. */
CReadDisplay* const CReadWindow::displayWidget() {
	return m_displayWidget;
}

/** Sets the display widget of this display window. */
void CReadWindow::setDisplayWidget( CReadDisplay* newDisplay ) {
	CDisplayWindow::setDisplayWidget(newDisplay);
	if (m_displayWidget) {
		disconnect(m_displayWidget->connectionsProxy(), SIGNAL(referenceClicked(const QString&, const QString&)), this, SLOT(lookup(const QString&, const QString&)));
		disconnect(m_displayWidget->connectionsProxy(), SIGNAL(referenceDropped(const QString&)), this, SLOT(lookup(const QString&)));
	}

	m_displayWidget = newDisplay;
	connect(m_displayWidget->connectionsProxy(), SIGNAL(referenceClicked(const QString&, const QString&)),this, SLOT(lookup(const QString&, const QString&)));
	connect(m_displayWidget->connectionsProxy(), SIGNAL(referenceDropped(const QString&)), this, SLOT(lookup(const QString&)));
}

/** Lookup the given entry. */
void CReadWindow::lookup( CSwordKey* newKey ) {
	using namespace Rendering;
// 	qWarning("lookup: %s", newKey->key().latin1());

	if (!newKey || !modules().first()) {
		return;
	}

	if (CEntryDisplay* display = modules().first()->getDisplay()) {	//do we have a display object?
		displayWidget()->setText( display->text( modules(), newKey->key(), displayOptions(), filterOptions() ) );
	}
	if (key() != newKey) {
		key()->key(newKey->key());
	}

	setCaption( windowCaption() );
	displayWidget()->moveToAnchor( CDisplayRendering::keyToHTMLAnchor(key()->key()) );
	
// 	qWarning("lookup: key is now %s", newKey->key().latin1());
}

/** Reimplementation to use the popup menu. */
const bool CReadWindow::init( const QString& keyName ) {
	CDisplayWindow::init(keyName);
	keyChooser()->setKey(key());
	setReady(true);

	return true;
}

/** Store the settings of this window in the given CProfileWindow object. */
void CReadWindow::storeProfileSettings(CProfileWindow * const settings) {
	QRect rect;
	rect.setX(parentWidget()->x());
	rect.setY(parentWidget()->y());
	rect.setWidth(width());
	rect.setHeight(height());
	settings->setGeometry(rect);

	//	settings->setScrollbarPositions( m_htmlWidget->view()->horizontalScrollBar()->value(), m_htmlWidget->view()->verticalScrollBar()->value() );
	settings->setType(modules().first()->type());
	settings->setMaximized(isMaximized() || parentWidget()->isMaximized());
	settings->setFocus( (this == mdi()->activeWindow()) ); //set property to true if this window is the active one.

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
	ListCSwordModuleInfo allMods = modules();
	for (CSwordModuleInfo* m = allMods.first(); m; m = allMods.next()) {
		mods.append(m->name());
	}
	settings->setModules(mods);
}

void CReadWindow::applyProfileSettings(CProfileWindow * const settings) {
	//  parentWidget()->setUpdatesEnabled(false);
	setUpdatesEnabled(false);

	if (settings->maximized()) { //maximize this window
		// Use parentWidget() to call showMaximized. Otherwise we'd get lot's of X11 errors
		parentWidget()->showMaximized();
	} else {
		const QRect rect = settings->geometry();
		resize(rect.width(), rect.height());
		parentWidget()->move(rect.x(), rect.y());
	}

	setUpdatesEnabled(true);
	//  parentWidget()->setUpdatesEnabled(true);
}

void CReadWindow::insertKeyboardActions( KAccel* const a ) {
	a->remove("Copy");
	a->insert("Copy", i18n("Copy selected text"),"", KStdAccel::copy(), 0, "");
}

void CReadWindow::initKeyboardActions() {
	CBTConfig::setupAccel( CBTConfig::readWindow, accel() );
	CReadWindow::insertKeyboardActions( accel() );

	accel()->readSettings();
	accel()->setSlot("Copy", displayWidget()->connectionsProxy(), SLOT(copySelection()));
}

/** No descriptions */
void CReadWindow::copyDisplayedText() {
	CExportManager mgr(QString::null);
	mgr.copyKey(key(), CExportManager::Text, true);
}
