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
#include "cdisplaysettingsbutton.h"

#include "resource.h"

#include "backend/cswordversekey.h"
#include "backend/cswordbiblemoduleinfo.h"

#include "frontend/cprofilewindow.h"
#include "frontend/ctoolclass.h"
#include "frontend/cbtconfig.h"
#include "frontend/display/creaddisplay.h"

#include <math.h>

//Qt includes

//KDE includes
#include <kaccel.h>
#include <klocale.h>
#include <kpopupmenu.h>

CBibleReadWindow::CBibleReadWindow(ListCSwordModuleInfo moduleList, CMDIArea* parent, const char *name ) : CLexiconReadWindow(moduleList, parent,name) {
	CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(moduleList.first());
	if (bible) {
		if (bible->hasTestament(CSwordBibleModuleInfo::OldTestament))
			key()->key("Genesis 1:1");
		else
			key()->key("Matthew 1:1");
	}
}

CBibleReadWindow::~CBibleReadWindow(){
}

void CBibleReadWindow::applyProfileSettings( CProfileWindow* const settings ) {
	CLexiconReadWindow::applyProfileSettings(settings);

	const int count = displaySettingsButton()->menuItemCount();
	int result = 0;
	//now check	every item
	for (int i = 1; i < count; i++) { //first item is a title
		if (displaySettingsButton()->itemStatus(i)) //item is checked
			result += (int)pow(2,i-1);//add 2^i (the i. digit in binary)
	}
	settings->setWindowSettings(result);
};

void CBibleReadWindow::storeProfileSettings( CProfileWindow* const settings ) {
	CLexiconReadWindow::storeProfileSettings(settings);

  int result = settings->windowSettings();
	const int count = displaySettingsButton()->menuItemCount();
	for (int i = count-1; i>=1; i--) {
		if (result-(int)pow(2,i-1)>= 0) { //2^i was added before, so item with index i is set
			result -= (int)pow(2,i-1);
			displaySettingsButton()->setItemStatus(i,true);
		}
		else
			displaySettingsButton()->setItemStatus(i,false);			
	}		
	displaySettingsButton()->setChanged();
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
}

void CBibleReadWindow::initView(){
	qWarning("CBibleWindow::initView");
 	CLexiconReadWindow::initView();
	
  setDisplaySettingsButton( new CDisplaySettingsButton( &displayOptions(), &filterOptions(), modules(), mainToolBar()) );
	mainToolBar()->insertWidget(1,displaySettingsButton()->size().width(),displaySettingsButton());
}

/** Reimplementation to handle the anchors. */
void CBibleReadWindow::lookup( CSwordKey* newKey ){
	qWarning("CBibleReadWindow::lookup( CSwordKey* newKey )");
 	CLexiconReadWindow::lookup(newKey);

  if (CSwordVerseKey* vk = dynamic_cast<CSwordVerseKey*>(newKey))
 		displayWidget()->moveToAnchor( QString::number(vk->Verse()) );
}

/** Reimplementation. */
void CBibleReadWindow::setupPopupMenu(){
	popup()->insertTitle(CToolClass::getIconForModule(modules().first()), i18n("Bible window"));

 	m_actions.selectAll = new KAction(i18n("Select all"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(selectAll()), actionCollection());
  m_actions.selectAll->plug(popup());

  (new KActionSeparator())->plug( popup() );

 	m_actions.copyMenu = new KActionMenu(i18n("Copy..."), ICON_EDIT_COPY);	
 	m_actions.copy.verseOnly = new KAction(i18n("Verse"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAnchorOnly()), actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.verseOnly);
	m_actions.copy.textOnly = new KAction(i18n("Text of verse"), KShortcut(0),displayWidget()->connectionsProxy(), SLOT(copyAnchorTextOnly()), actionCollection());	
 	m_actions.copyMenu->insert(m_actions.copy.textOnly);
 	m_actions.copy.verseAndText = new KAction(i18n("Verse with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAnchorWithText()), actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.verseAndText); 	
 	m_actions.copy.chapter = new KAction(i18n("Chapter"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copyAll()), actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.chapter); 	
  m_actions.copyMenu->insert(new KActionSeparator());	
	m_actions.copy.selectedText = new KAction(i18n("Selected text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(copySelection()),actionCollection());
 	m_actions.copyMenu->insert(m_actions.copy.selectedText);
 	m_actions.copyMenu->plug(popup());

 	m_actions.saveMenu = new KActionMenu(i18n("Save..."),ICON_FILE_SAVE);	
 	m_actions.save.verseAndText = new KAction(i18n("Verse with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(saveAnchorWithText()), actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.verseAndText);
	m_actions.save.chapterAsPlain = new KAction(i18n("Chapter as plain text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(saveAsPlain()),actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.chapterAsPlain);
 	m_actions.save.chapterAsHTML = new KAction(i18n("Chapter as HTML"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(saveAsHTML()),actionCollection());
 	m_actions.saveMenu->insert(m_actions.save.chapterAsHTML);
 	m_actions.saveMenu->plug(popup());

 	m_actions.printMenu = new KActionMenu(i18n("Print..."),ICON_FILE_PRINT);	
 	m_actions.print.verseAndText = new KAction(i18n("Verse with text"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(printAnchorWithText()), actionCollection());
 	m_actions.printMenu->insert(m_actions.print.verseAndText); 	
  m_actions.print.chapter = new KAction(i18n("Chapter"), KShortcut(0), displayWidget()->connectionsProxy(), SLOT(printAll()),actionCollection());
 	m_actions.printMenu->insert(m_actions.print.chapter);
 	m_actions.printMenu->plug(popup());
}

/** Reimplemented. */
void CBibleReadWindow::updatePopupMenu(){
 	
  m_actions.copy.verseOnly->setEnabled( displayWidget()->hasActiveAnchor() );	
 	m_actions.copy.textOnly->setEnabled( displayWidget()->hasActiveAnchor() );	
 	m_actions.copy.verseAndText->setEnabled( displayWidget()->hasActiveAnchor() );
	m_actions.copy.selectedText->setEnabled( displayWidget()->hasSelection() );

 	m_actions.save.verseAndText->setEnabled( displayWidget()->hasActiveAnchor() );
 	
  m_actions.print.verseAndText->setEnabled( displayWidget()->hasActiveAnchor() );
}
