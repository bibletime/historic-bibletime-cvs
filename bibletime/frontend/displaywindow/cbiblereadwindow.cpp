/***************************************************************************
                          cbiblereadwindow.cpp  -  description
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

#include "cbiblereadwindow.h"
#include "ccommentaryreadwindow.h"
#include "cbuttons.h"

#include "backend/cswordversekey.h"
#include "backend/cswordbiblemoduleinfo.h"

#include "frontend/cprofilewindow.h"
#include "frontend/cexportmanager.h"
#include "frontend/cbtconfig.h"
#include "frontend/cmdiarea.h"
#include "util/cresmgr.h"

#include "frontend/display/creaddisplay.h"
#include "frontend/keychooser/ckeychooser.h"

#include "util/ctoolclass.h"

#include <math.h>

//Qt includes
#include <qwidgetlist.h>

//KDE includes
#include <kaccel.h>
#include <klocale.h>
#include <kpopupmenu.h>

CBibleReadWindow::CBibleReadWindow(ListCSwordModuleInfo moduleList, CMDIArea* parent, const char *name ) : CLexiconReadWindow(moduleList, parent,name) {
  m_transliterationButton = 0;
}

CBibleReadWindow::~CBibleReadWindow(){
}

void CBibleReadWindow::applyProfileSettings( CProfileWindow* const settings ) {
	CLexiconReadWindow::applyProfileSettings(settings);

	const int count = displaySettingsButton()->menuItemCount();
	int result = settings->windowSettings();
	for (int i = count-1; i>=1; i--) {
		if (result-(int)pow((double)2,i-1)>= 0) { //2^i was added before, so item with index i is set
			result -= (int)pow((double)2,i-1);
			displaySettingsButton()->setItemStatus(i,true);
		}
		else {
			displaySettingsButton()->setItemStatus(i,false);
  	}
	}
	displaySettingsButton()->setChanged();
};

void CBibleReadWindow::storeProfileSettings( CProfileWindow* const settings ) {
	CLexiconReadWindow::storeProfileSettings(settings);

	const int count = displaySettingsButton()->menuItemCount();
	int result = 0;
	//now check	every item
	for (int i = 1; i < count; i++) { //first item is a title
		if (displaySettingsButton()->itemStatus(i)) //item is checked
			result += (int)pow((double)2,i-1);//add 2^i (the i. digit in binary)
	}
	settings->setWindowSettings(result);
};


/** Reimplementation. */
void CBibleReadWindow::insertKeyboardActions( KAccel* const a ){
  a->insert("Next book",        i18n("Next book"),        "", CResMgr::displaywindows::bibleWindow::nextBook::accel,        0, "");
	a->insert("Previous book",    i18n("Previous book"),    "", CResMgr::displaywindows::bibleWindow::previousBook::accel,    0, "");
	a->insert("Next chapter",     i18n("Next chapter"),     "", CResMgr::displaywindows::bibleWindow::nextChapter::accel,     0, "");
	a->insert("Previous chapter", i18n("Previous chapter"), "", CResMgr::displaywindows::bibleWindow::previousChapter::accel, 0, "");
	a->insert("Next verse",       i18n("Next verse"),       "", CResMgr::displaywindows::bibleWindow::nextVerse::accel,       0, "");
	a->insert("Previous verse",   i18n("Previous verse"),   "", CResMgr::displaywindows::bibleWindow::previousVerse::accel,   0, "");
}

void CBibleReadWindow::initKeyboardActions() {
  CReadWindow::initKeyboardActions();

  CBTConfig::setupAccel( CBTConfig::bibleWindow, accel() );
  insertKeyboardActions( accel() );
  CReadWindow::insertKeyboardActions(accel());

  accel()->readSettings(); 

	accel()->setSlot("Next book", this, SLOT(nextBook()));
  accel()->setSlot("Previous book", this, SLOT(previousBook()));
  accel()->setSlot("Next chapter", this, SLOT(nextChapter()));
  accel()->setSlot("Previous chapter", this, SLOT(previousChapter()));
  accel()->setSlot("Next verse", this, SLOT(nextVerse()));
  accel()->setSlot("Previous verse", this, SLOT(previousVerse()));
}

void CBibleReadWindow::initConnections(){
  CLexiconReadWindow::initConnections();
  connect(keyChooser(), SIGNAL(keyChanged(CSwordKey*)), SLOT(keyChanged(CSwordKey*)));

  if (m_transliterationButton) { // Transliteration is not always available
    connect(m_transliterationButton, SIGNAL(sigChanged()), SLOT(lookup()));
  }
}

void CBibleReadWindow::initView(){
 	CLexiconReadWindow::initView();

  parentWidget()->installEventFilter( this );  

//  qWarning("initView: %i modules", modules().count());  
  setDisplaySettingsButton( new CDisplaySettingsButton( &displayOptions(), &filterOptions(), modules(), mainToolBar()) );
	mainToolBar()->insertWidget(2,displaySettingsButton()->size().width(),displaySettingsButton());

	if (backend()->useICU()){
	  m_transliterationButton = new CTransliterationButton(&filterOptions(), mainToolBar());
		mainToolBar()->insertWidget( 3, m_transliterationButton->size().width(), m_transliterationButton );
	}
}

/** Reimplementation. */
void CBibleReadWindow::setupPopupMenu(){
//	qWarning("CBibleReadWindow::setupPopupMenu()");
	popup()->insertTitle(CToolClass::getIconForModule(modules().first()), i18n("Bible window"));

 	m_actions.selectAll = new KAction(i18n("Select all"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(selectAll()), actionCollection());
  m_actions.selectAll->plug(popup());

  (new KActionSeparator())->plug( popup() );

 	m_actions.copyMenu = new KActionMenu(i18n("Copy..."), CResMgr::displaywindows::bibleWindow::copyMenu::icon);
 	m_actions.copy.referenceOnly = new KAction(i18n("Reference only"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAnchorOnly()), actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.referenceOnly);
	m_actions.copy.referenceTextOnly = new KAction(i18n("Text of reference"), KShortcut(0),displayWidget()->connectionsProxy(), SLOT(copyAnchorTextOnly()), actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.referenceTextOnly);
  m_actions.copy.referenceAndText = new KAction(i18n("Reference with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAnchorWithText()), actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.referenceAndText);
  m_actions.copy.chapter = new KAction(i18n("Chapter"), KShortcut(0), this, SLOT(copyDisplayedText()), actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.chapter);
  m_actions.copyMenu->insert(new KActionSeparator());
  m_actions.copy.selectedText = new KAction(i18n("Selected text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copySelection()),actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.selectedText);
  m_actions.copyMenu->plug(popup());

 	m_actions.saveMenu = new KActionMenu(i18n("Save..."),CResMgr::displaywindows::bibleWindow::saveMenu::icon);
 	m_actions.save.referenceAndText = new KAction(i18n("Reference with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(saveAnchorWithText()), actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.referenceAndText);
	m_actions.save.chapterAsPlain = new KAction(i18n("Chapter as plain text"), KShortcut(0), this, SLOT(saveChapterPlain()),actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.chapterAsPlain);
 	m_actions.save.chapterAsHTML = new KAction(i18n("Chapter as HTML"), KShortcut(0), this, SLOT(saveChapterHTML()),actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.chapterAsHTML);
 	m_actions.saveMenu->plug(popup());

 	m_actions.printMenu = new KActionMenu(i18n("Print..."),CResMgr::displaywindows::bibleWindow::printMenu::icon);	
 	m_actions.print.reference = new KAction(i18n("Reference with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(printAnchorWithText()), actionCollection());
 	m_actions.printMenu->insert(m_actions.print.reference); 	
  m_actions.print.chapter = new KAction(i18n("Chapter"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(printAll()),actionCollection());
 	m_actions.printMenu->insert(m_actions.print.chapter);
 	m_actions.printMenu->plug(popup());
}

/** Reimplemented. */
void CBibleReadWindow::updatePopupMenu(){
//	qWarning("CBibleReadWindow::updatePopupMenu()");

  m_actions.copy.referenceOnly->setEnabled( displayWidget()->hasActiveAnchor() );
 	m_actions.copy.referenceTextOnly->setEnabled( displayWidget()->hasActiveAnchor() );
 	m_actions.copy.referenceAndText->setEnabled( displayWidget()->hasActiveAnchor() );
	m_actions.copy.selectedText->setEnabled( displayWidget()->hasSelection() );

 	m_actions.save.referenceAndText->setEnabled( displayWidget()->hasActiveAnchor() );

  m_actions.print.reference->setEnabled( displayWidget()->hasActiveAnchor() );
}

/** Moves to the next book. */
void CBibleReadWindow::nextBook(){
	if (verseKey()->next(CSwordVerseKey::UseBook))
		keyChooser()->setKey(key());
}

/** Moves one book behind. */
void CBibleReadWindow::previousBook(){
	if (verseKey()->previous(CSwordVerseKey::UseBook))
		keyChooser()->setKey(key());
}

/** Moves to the next book. */
void CBibleReadWindow::nextChapter(){
	if (verseKey()->next(CSwordVerseKey::UseChapter))
		keyChooser()->setKey(key());
}

/** Moves one book behind. */
void CBibleReadWindow::previousChapter(){
	if (verseKey()->previous(CSwordVerseKey::UseChapter))
		keyChooser()->setKey(key());
}

/** Moves to the next book. */
void CBibleReadWindow::nextVerse(){
	if (verseKey()->next(CSwordVerseKey::UseVerse))
		keyChooser()->setKey(key());
}

/** Moves one book behind. */
void CBibleReadWindow::previousVerse(){
	if (verseKey()->previous(CSwordVerseKey::UseVerse))
		keyChooser()->setKey(key());
}

/** rapper around key() to return the right type of key. */
CSwordVerseKey* CBibleReadWindow::verseKey(){
//  qWarning("CBibleReadWindow::key()");
	CSwordVerseKey* k = dynamic_cast<CSwordVerseKey*>(CDisplayWindow::key());
 	Q_ASSERT(k);
	return k;
}

/** Is called when the key of the keychooser changed. */
void CBibleReadWindow::keyChanged(CSwordKey* key){
	QWidgetList windows = mdi()->windowList();
	if (!windows.count())
		return;

	for (windows.first(); windows.current(); windows.next()) {
		CDisplayWindow* w = dynamic_cast<CDisplayWindow*>(windows.current());
		if (w && w->syncAllowed()) {
			w->lookup( key->key() );
		}
	}
}

/** Copies the current chapter into the clipboard. */
void CBibleReadWindow::copyDisplayedText(){
//normal function
  CSwordVerseKey vk(*verseKey());
  CSwordVerseKey dummy(*verseKey());

  dummy.Verse(1);
//  qWarning("copyChapter: lower bound is %s", (const char*)dummy);
  vk.LowerBound(dummy);

  CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(modules().first());
  dummy.Verse(bible->verseCount(dummy.book(), dummy.Chapter()));
//  qWarning("copyChapter: upper bound is %s", (const char*)dummy);
  vk.UpperBound(dummy);

  CExportManager mgr(i18n("Copy chapter to clipboard ..."), false, i18n("Copying"), filterOptions(), displayOptions());
  mgr.copyKey(&vk, CExportManager::Text, true);
}

/** Saves the chapter as valid HTML page. */
void CBibleReadWindow::saveChapterHTML(){
  //saves the complete chapter to disk

	CSwordVerseKey dummy(*verseKey());
  dummy.Verse(1);
	qWarning("start saving from %s", dummy.key().latin1());

  CSwordVerseKey vk(*verseKey());
	vk.LowerBound(dummy);
	qWarning("vk's start is %s", dummy.key().latin1());

  CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(modules().first());
	qWarning("verseCount for %s, chapter %i is %i", (const char*)dummy.book().local8Bit(), dummy.Chapter(), bible->verseCount(dummy.book(), dummy.Chapter()));
	dummy.Verse(bible->verseCount(dummy.book(), dummy.Chapter()));

	vk.UpperBound(dummy);

  CExportManager mgr(i18n("Saving chapter ..."), true, i18n("Saving"), filterOptions(), displayOptions());
  mgr.saveKey(&vk, CExportManager::HTML, true);
}

/** Saves the chapter as valid HTML page. */
void CBibleReadWindow::saveChapterPlain(){
  //saves the complete chapter to disk

  CSwordVerseKey vk(*verseKey());
  CSwordVerseKey dummy(*verseKey());

  dummy.Verse(1);
  vk.LowerBound(dummy);

  CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(modules().first());
  dummy.Verse(bible->verseCount(dummy.book(), dummy.Chapter()));
  vk.UpperBound(dummy);

  CExportManager mgr(i18n("Saving chapter ..."), true, i18n("Saving"), filterOptions(),displayOptions());
  mgr.saveKey(&vk, CExportManager::Text, true);
}

void CBibleReadWindow::reload(){
  CLexiconReadWindow::reload();

	//refresh the book lists
  verseKey()->setLocale( backend()->booknameLanguage().latin1() );
  keyChooser()->refreshContent();
}

/** No descriptions */
bool CBibleReadWindow::eventFilter( QObject* o, QEvent* e) {
  if (e && (e->type() == QEvent::FocusIn)) { //sync other windows to this active window now
  	QWidgetList windows = mdi()->windowList();
   	for (windows.first(); windows.current(); windows.next()) {
			CDisplayWindow* w = dynamic_cast<CDisplayWindow*>(windows.current());
   		if (w && w->syncAllowed()) {
   			w->lookup( key()->key() );
			}
   	}
  }
  return CLexiconReadWindow::eventFilter(o,e);
}
