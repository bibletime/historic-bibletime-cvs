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

#include "resource.h"

#include "backend/cswordversekey.h"
#include "backend/cswordbiblemoduleinfo.h"

#include "frontend/cprofilewindow.h"
#include "frontend/ctoolclass.h"
#include "frontend/cexportmanager.h"
#include "frontend/cbtconfig.h"
#include "frontend/cmdiarea.h"
#include "frontend/display/creaddisplay.h"
#include "frontend/keychooser/ckeychooser.h"

#include <math.h>

//Qt includes
#include <qwidgetlist.h>

//KDE includes
#include <kaccel.h>
#include <klocale.h>
#include <kpopupmenu.h>

CBibleReadWindow::CBibleReadWindow(ListCSwordModuleInfo moduleList, CMDIArea* parent, const char *name ) : CLexiconReadWindow(moduleList, parent,name) {
  m_transliterationButton = 0;	
//  if (CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(moduleList.first())) {
//#warning Fix this hack!		
////    key()->key(bible->LowerBound().key());	
//  }
}

CBibleReadWindow::~CBibleReadWindow(){
}

void CBibleReadWindow::applyProfileSettings( CProfileWindow* const settings ) {
	CLexiconReadWindow::applyProfileSettings(settings);

	const int count = displaySettingsButton()->menuItemCount();
	int result = settings->windowSettings();
// 	qWarning("count == %i", count);
	for (int i = count-1; i>=1; i--) {
		if (result-(int)std::pow((double)2,i-1)>= 0) { //2^i was added before, so item with index i is set
			result -= (int)std::pow((double)2,i-1);
//   		qWarning("set item %i to true", i);
			displaySettingsButton()->setItemStatus(i,true);
		}
		else {
//   		qWarning("set item %i to false", i);
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
			result += (int)std::pow((double)2,i-1);//add 2^i (the i. digit in binary)
	}
	settings->setWindowSettings(result);
};


/** Reimplementation. */
void CBibleReadWindow::insertKeyboardActions( KAccel* const a ){
  a->insert("Next book",        i18n("Next book"),        "", IDK_PRESENTER_NEXT_BOOK,        0, "");
	a->insert("Previous book",    i18n("Previous book"),    "", IDK_PRESENTER_PREVIOUS_BOOK,    0, "");
	a->insert("Next chapter",     i18n("Next chapter"),     "", IDK_PRESENTER_NEXT_CHAPTER,     0, "");
	a->insert("Previous chapter", i18n("Previous chapter"), "", IDK_PRESENTER_PREVIOUS_CHAPTER, 0, "");	
	a->insert("Next verse",       i18n("Next verse"),       "", IDK_PRESENTER_NEXT_VERSE,       0, "");
	a->insert("Previous verse",   i18n("Previous verse"),   "", IDK_PRESENTER_PREVIOUS_VERSE,   0, "");	
}


void CBibleReadWindow::initKeyboardActions() {
  CReadWindow::initKeyboardActions();

  CBTConfig::setupAccel( CBTConfig::bibleWindow, accel() );	
  insertKeyboardActions( accel() );

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
  connect(keyChooser(), SIGNAL(keyChanged(CSwordKey*)),
    this, SLOT(keyChanged(CSwordKey*)));
  connect(m_transliterationButton, SIGNAL(sigChanged()),
    this, SLOT(refresh()));
}

void CBibleReadWindow::initView(){
//	qWarning("CBibleReadWindow::initView");
 	CLexiconReadWindow::initView();
	
//  qWarning("create the display settings button");
  setDisplaySettingsButton( new CDisplaySettingsButton( &displayOptions(), &filterOptions(), modules(), mainToolBar()) );
	mainToolBar()->insertWidget(2,displaySettingsButton()->size().width(),displaySettingsButton());

	if (backend()->useICU()){
//    qWarning("add the ICU button!");
	  m_transliterationButton = new CTransliterationButton(&filterOptions(), mainToolBar());
		mainToolBar()->insertWidget(3,m_transliterationButton->size().width(),m_transliterationButton);
	}
}

/** Reimplementation. */
void CBibleReadWindow::setupPopupMenu(){
//	qWarning("CBibleReadWindow::setupPopupMenu()");
	popup()->insertTitle(CToolClass::getIconForModule(modules().first()), i18n("Bible window"));

 	m_actions.selectAll = new KAction(i18n("Select all"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(selectAll()), actionCollection());
  m_actions.selectAll->plug(popup());

  (new KActionSeparator())->plug( popup() );

 	m_actions.copyMenu = new KActionMenu(i18n("Copy..."), ICON_EDIT_COPY);	
 	m_actions.copy.referenceOnly = new KAction(i18n("Reference"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAnchorOnly()), actionCollection());
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

 	m_actions.saveMenu = new KActionMenu(i18n("Save..."),ICON_FILE_SAVE);	
 	m_actions.save.referenceAndText = new KAction(i18n("Reference with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(saveAnchorWithText()), actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.referenceAndText);
	m_actions.save.chapterAsPlain = new KAction(i18n("Chapter as plain text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(saveAsPlain()),actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.chapterAsPlain);
 	m_actions.save.chapterAsHTML = new KAction(i18n("Chapter as HTML"), KShortcut(0), this, SLOT(saveChapter()),actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.chapterAsHTML);
 	m_actions.saveMenu->plug(popup());

 	m_actions.printMenu = new KActionMenu(i18n("Print..."),ICON_FILE_PRINT);	
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
		if (CCommentaryReadWindow* p = dynamic_cast<CCommentaryReadWindow*>(windows.current()))
			p->syncToKey(key);
	}	
}

/** Copies the current chapter into the clipboard. */
void CBibleReadWindow::copyDisplayedText(){
  CSwordVerseKey vk(*verseKey());
  CSwordVerseKey dummy(*verseKey());

  dummy.Verse(1);
//  qWarning("copyChapter: lower bound is %s", (const char*)dummy);
  vk.LowerBound(dummy);

  CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(modules().first());
  dummy.Verse(bible->verseCount(dummy.book(), dummy.Chapter()));
//  qWarning("copyChapter: upper bound is %s", (const char*)dummy);
  vk.UpperBound(dummy);

  CExportManager mgr(i18n("Copy chapter into clipboard ..."), false, i18n("Copying"), filterOptions(),displayOptions());
  mgr.copyKey(&vk, CExportManager::Text, true);
}

/** Saves the chapter as valid HTML page. */
void CBibleReadWindow::saveChapter(){
  //saves the complete chapter to disk

  CSwordVerseKey vk(*verseKey());
  CSwordVerseKey dummy(*verseKey());

  dummy.Verse(1);
//  qWarning("copyChapter: lower bound is %s", (const char*)dummy);
  vk.LowerBound(dummy);

  CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(modules().first());
  dummy.Verse(bible->verseCount(dummy.book(), dummy.Chapter()));
//  qWarning("copyChapter: upper bound is %s", (const char*)dummy);
  vk.UpperBound(dummy);

  CExportManager mgr(i18n("Saving chapter ..."), true, i18n("Saving"), filterOptions(),displayOptions());
  mgr.saveKey(&vk, CExportManager::HTML, true);
}

void CBibleReadWindow::refresh(){
  //refresh the book lists
  verseKey()->setLocale( backend()->booknameLanguage().latin1() );
  keyChooser()->refreshContent();
  
  lookup(key());
}
