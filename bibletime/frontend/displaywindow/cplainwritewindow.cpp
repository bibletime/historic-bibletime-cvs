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
#include "frontend/cprofilewindow.h"
#include "frontend/cbtconfig.h"

#include "util/cresmgr.h"

//Qt includes
#include <qregexp.h>

//KDE includes
#include <kaction.h>
#include <kaccel.h>
#include <klocale.h>

CPlainWriteWindow::CPlainWriteWindow(ListCSwordModuleInfo moduleList, CMDIArea* parent, const char *name ) : CWriteWindow(moduleList, parent, name) {
	setKey( CSwordKey::createInstance(moduleList.first()) );
}


CPlainWriteWindow::~CPlainWriteWindow(){
}

/** Initialize the state of this widget. */
void CPlainWriteWindow::initView(){
//  qWarning("CPlainWriteWindow::initView()");
 	setDisplayWidget( CDisplay::createWriteInstance(this) );
//  Q_ASSERT(displayWidget());
 	setCentralWidget( displayWidget()->view() );

  setMainToolBar( new KToolBar(this) );
  mainToolBar()->setFullSize(true);
	addDockWindow(mainToolBar());

	setKeyChooser( CKeyChooser::createInstance(modules(), key(), mainToolBar()) );
	mainToolBar()->insertWidget(0,keyChooser()->sizeHint().width(),keyChooser());
 	mainToolBar()->setFullSize(false);

	m_actions.syncWindow = new KToggleAction(i18n("Sync with active bible"),
		CResMgr::displaywindows::commentaryWindow::syncWindow::icon,
		CResMgr::displaywindows::commentaryWindow::syncWindow::accel,
		actionCollection(),
		CResMgr::displaywindows::commentaryWindow::syncWindow::actionName
	);
	m_actions.syncWindow->setToolTip(CResMgr::displaywindows::commentaryWindow::syncWindow::tooltip); m_actions.syncWindow->setWhatsThis(CResMgr::displaywindows::commentaryWindow::syncWindow::whatsthis);
  m_actions.syncWindow->plug(mainToolBar());


	m_actions.saveText = new KAction(i18n("Save text"),
    CResMgr::displaywindows::writeWindow::saveText::icon,
    CResMgr::displaywindows::writeWindow::saveText::accel,
    this, SLOT(saveCurrentText()),
    actionCollection(),
		CResMgr::displaywindows::writeWindow::saveText::actionName
  );
  m_actions.saveText->setToolTip( CResMgr::displaywindows::writeWindow::saveText::tooltip );
  m_actions.saveText->setWhatsThis( CResMgr::displaywindows::writeWindow::saveText::whatsthis );
  m_actions.saveText->plug(mainToolBar());


 	m_actions.deleteEntry = new KAction(i18n("Delete current entry"),
    CResMgr::displaywindows::writeWindow::deleteEntry::icon,
    CResMgr::displaywindows::writeWindow::deleteEntry::accel,
    this, SLOT(deleteEntry()),
    actionCollection(),
		CResMgr::displaywindows::writeWindow::deleteEntry::actionName
  );
  m_actions.deleteEntry->setToolTip( CResMgr::displaywindows::writeWindow::deleteEntry::tooltip );
  m_actions.deleteEntry->setWhatsThis( CResMgr::displaywindows::writeWindow::deleteEntry::whatsthis );
  m_actions.deleteEntry->plug(mainToolBar());


 	m_actions.restoreText = new KAction(i18n("Restore original text"),
    CResMgr::displaywindows::writeWindow::restoreText::icon,
    CResMgr::displaywindows::writeWindow::restoreText::accel,
    this, SLOT(restoreText()),
    actionCollection(),
		CResMgr::displaywindows::writeWindow::restoreText::actionName
  );
  m_actions.restoreText->setToolTip( CResMgr::displaywindows::writeWindow::restoreText::tooltip );
  m_actions.restoreText->setWhatsThis( CResMgr::displaywindows::writeWindow::restoreText::whatsthis );
  m_actions.restoreText->plug(mainToolBar());
}

void CPlainWriteWindow::initConnections(){
	CWriteWindow::initConnections();
	connect(keyChooser(), SIGNAL(keyChanged(CSwordKey*)),
		this, SLOT(lookup(CSwordKey*)));

  connect(displayWidget()->connectionsProxy(), SIGNAL(textChanged()),
    this, SLOT(textChanged()) );
}

void CPlainWriteWindow::storeProfileSettings( CProfileWindow* profileWindow ) {
  CWriteWindow::storeProfileSettings(profileWindow);
  profileWindow->setWindowSettings( m_actions.syncWindow->isChecked() );
};

void CPlainWriteWindow::applyProfileSettings( CProfileWindow* profileWindow ) {
  CWriteWindow::applyProfileSettings(profileWindow);
  if (profileWindow->windowSettings()) {
    m_actions.syncWindow->setChecked(true);
	}
};

/** Saves the text for the current key. Directly writes the changed text into the module. */
void CPlainWriteWindow::saveCurrentText( const QString& /*key*/ ) {
  QString t = displayWidget()->plainText();
  //since t is a complete HTML page at the moment, strip away headers and footers of a HTML page
  QRegExp re("(?:<html.*>.+<body.*>)", false); //remove headers, case insensitive
  re.setMinimal(true);
  t.replace(re, "");
  t.replace(QRegExp("</body></html>", false), "");//remove footer

	const QString& oldKey = this->key()->key();
  modules().first()->write(this->key(), t );
	this->key()->key( oldKey );

  displayWidget()->setModified(false);
  textChanged();
}

/** Loads the original text from the module. */
void CPlainWriteWindow::restoreText(){
  lookup(key());
  displayWidget()->setModified(false);
  textChanged();
}

/** Is called when the current text was changed. */
void CPlainWriteWindow::textChanged() {
	m_actions.saveText->setEnabled( displayWidget()->isModified() );
  m_actions.restoreText->setEnabled( displayWidget()->isModified() );
}

/** Deletes the module entry and clears the edit widget, */
void CPlainWriteWindow::deleteEntry(){
  modules().first()->deleteEntry( key() );
  lookup( key() );
  displayWidget()->setModified(false);
}

/** Setups the popup menu of this display widget. */
void CPlainWriteWindow::setupPopupMenu(){
}

const bool CPlainWriteWindow::syncAllowed() const {
	return m_actions.syncWindow->isChecked();
}

void CPlainWriteWindow::initKeyboardActions() {
  CWriteWindow::initKeyboardActions();

	CBTConfig::setupAccel( CBTConfig::writeWindow, accel() );
  insertKeyboardActions( accel() );

  accel()->readSettings();
	//accel()->setSlot("Copy", displayWidget()->connectionsProxy(), SLOT(copySelection()));

/*
	accel()->setSlot("Next book", this, SLOT(nextBook()));
  accel()->setSlot("Previous book", this, SLOT(previousBook()));
  accel()->setSlot("Next chapter", this, SLOT(nextChapter()));
  accel()->setSlot("Previous chapter", this, SLOT(previousChapter()));
  accel()->setSlot("Next verse", this, SLOT(nextVerse()));
  accel()->setSlot("Previous verse", this, SLOT(previousVerse()));
*/
}

void CPlainWriteWindow::insertKeyboardActions( KAccel* const a ) {
  a->insert("Next book",        i18n("Next book"),        "", CResMgr::displaywindows::bibleWindow::nextBook::accel,        0, "");
	a->insert("Previous book",    i18n("Previous book"),    "", CResMgr::displaywindows::bibleWindow::previousBook::accel,    0, "");
	a->insert("Next chapter",     i18n("Next chapter"),     "", CResMgr::displaywindows::bibleWindow::nextChapter::accel,     0, "");
	a->insert("Previous chapter", i18n("Previous chapter"), "", CResMgr::displaywindows::bibleWindow::previousChapter::accel, 0, "");
	a->insert("Next verse",       i18n("Next verse"),       "", CResMgr::displaywindows::bibleWindow::nextVerse::accel,       0, "");
	a->insert("Previous verse",   i18n("Previous verse"),   "", CResMgr::displaywindows::bibleWindow::previousVerse::accel,   0, "");

	a->insert("Save text",   i18n("Save text"),   "", CResMgr::displaywindows::writeWindow::saveText::accel,   0, "");
}

//void CPlainWriteWindow::setupPopupMenu() {
//	qWarning("Plain wite window: setup popup menu");
//}
