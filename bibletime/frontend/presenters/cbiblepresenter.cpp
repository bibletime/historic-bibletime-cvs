/***************************************************************************
                          cbiblepresenter.cpp  -  description
                             -------------------
    begin                : Sun Mar 18 2001
    copyright            : (C) 2001 by The BibleTime team
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

#include "cbiblepresenter.h"
#include "cmodulechooserbar.h"
#include "cdisplaysettingsbutton.h"

#include "../ctoolclass.h"
#include "../cexportmanager.h"
#include "../chtmlwidget.h"
#include "../keychooser/ckeychooser.h"
#include "../../resource.h"
#include "../../backend/cswordbiblemoduleinfo.h"
#include "../../backend/cswordversekey.h"
#include "../../backend/chtmlchapterdisplay.h"
#include "../../backend/cswordbackend.h"
#include "../../backend/creferencemanager.h"
#include "../cprofile.h"
#include "../cprofilewindow.h"

#include <math.h>

//Qt includes
#include <qclipboard.h>
#include <qlist.h>

//KDE includes
#include <kapp.h>
#include <ktoolbar.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kaccel.h>
#include <kpopupmenu.h>

CBiblePresenter::CBiblePresenter(ListCSwordModuleInfo useModules, QWidget *parent, const char *name )
	: CSwordPresenter(useModules,parent,name)
{		
	m_key = new CSwordVerseKey(m_moduleList.first());
	CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(m_moduleList.first());
	if (bible) {
		if (bible->hasTestament(CSwordBibleModuleInfo::OldTestament))
			m_key->key("Genesis 1:1");
		else
			m_key->key("Matthew 1:1");
	}
	
	initView();
	show();	
	initConnections();
	
	setInitialized();
}

CBiblePresenter::~CBiblePresenter(){
	delete m_key;
}

/** Initializes the view (central widget, toolbars etc) of this presenter */
void CBiblePresenter::initView(){
//	qWarning("CBiblePresenter::initView");	
	m_mainToolBar = new KToolBar(this);
	m_keyChooser = CKeyChooser::createInstance(m_moduleList.first(), m_key, m_mainToolBar);
	m_mainToolBar->insertWidget(0,m_keyChooser->sizeHint().width(),m_keyChooser);	
	m_mainToolBar->setItemAutoSized(0);

	m_displaySettingsButton = new CDisplaySettingsButton( &m_displayOptions, &m_moduleOptions, m_moduleList, m_mainToolBar);
	m_mainToolBar->insertWidget(1,m_displaySettingsButton->size().width(),m_displaySettingsButton);

	addToolBar(m_mainToolBar);			
		
	m_moduleChooserBar = new CModuleChooserBar(m_moduleList, CSwordModuleInfo::Bible, this );
	addToolBar(m_moduleChooserBar);
	
	m_htmlWidget = new CHTMLWidget(true, this);
		
	//setup popup menu
	m_popup = new KPopupMenu(this);
	m_popup->insertTitle(i18n("Bible window"));
	
	m_copyPopup = new KPopupMenu(m_popup);
	m_copyPopup->insertItem(i18n("Verse"), this, SLOT(copyVerse()),0,ID_PRESENTER_COPY_ONLY_KEY);
	m_copyPopup->insertItem(i18n("Text of verse"), this, SLOT(copyVerseText()),0,ID_PRESENTER_COPY_KEY_TEXT);	
	m_copyPopup->insertItem(i18n("Verse with text"), this, SLOT(copyVerseAndText()),0,ID_PRESENTER_COPY_KEY);
	m_copyPopup->insertItem(i18n("Chapter"), m_htmlWidget, SLOT(copyDocument()),0,ID_PRESENTER_COPY_CHAPTER);
	m_copyPopup->insertSeparator();
	m_copyPopup->insertItem(i18n("Selected text"), m_htmlWidget, SLOT(copy()),0,ID_PRESENTER_COPY_SELECTED);

	m_printPopup = new KPopupMenu(m_popup);
	m_printPopup->insertItem(i18n("Verse with text"), this, SLOT(printVerseAndText()),0,ID_PRESENTER_PRINT_KEY);
	m_printPopup->insertItem(i18n("Chapter"), this, SLOT(printChapter()),0,ID_PRESENTER_PRINT_CHAPTER);

	m_savePopup = new KPopupMenu(m_popup);	
	m_savePopup->insertItem(i18n("Verse with text"), this, SLOT(saveVerseAndText()),0,ID_PRESENTER_SAVE_KEY);
	m_savePopup->insertItem(i18n("Chapter as plain text"), m_htmlWidget, SLOT(slotSaveAsText()),0,ID_PRESENTER_SAVE_CHAPTER);
	m_savePopup->insertItem(i18n("Chapter as HTML"), m_htmlWidget, SLOT(slotSaveAsHTML()),0,ID_PRESENTER_SAVE_CHAPTER_HTML);	

	m_popup->insertItem(i18n("Select all"), m_htmlWidget, SLOT(slotSelectAll()),0, ID_PRESENTER_SELECT_ALL);
  m_popup->insertItem(i18n("Lookup selected text in lexicon"), m_lexiconPopup, ID_PRESENTER_LOOKUP);	
	m_popup->insertSeparator();	
	m_popup->insertItem(SmallIcon(ICON_EDIT_COPY),i18n("Copy..."), 	m_copyPopup, ID_PRESENTER_COPY_POPUP);	
	m_popup->insertItem(SmallIcon(ICON_FILE_PRINT), i18n("Add to printing queue..."), m_printPopup, ID_PRESENTER_PRINT_POPUP);	
	m_popup->insertItem(SmallIcon(ICON_FILE_SAVE), i18n("Save..."), 	m_savePopup,ID_PRESENTER_SAVE_POPUP);		

	m_htmlWidget->installPopup(m_popup);			
	m_htmlWidget->installAnchorMenu( m_popup );
	m_htmlWidget->setModules(m_moduleList);
		
	setCentralWidget(m_htmlWidget);	
	setIcon( BIBLE_ICON_SMALL );	
}

/** Displays the chapter using the aparameter. */
void CBiblePresenter::lookup(CSwordKey* key){
//	qWarning("CBiblePresenter::lookup(CSwordKey* key)");
	setUpdatesEnabled(false);	
	
	CSwordVerseKey* vKey = dynamic_cast<CSwordVerseKey*>(key);	
	if (!vKey)
		return;

	backend()->setFilterOptions( m_moduleOptions );
	backend()->setDisplayOptions( m_displayOptions );
		
	if (m_moduleList.first()->getDisplay()) {	//do we have a display object?
	  m_moduleList.first()->module()->SetKey(*vKey);
		if (m_moduleList.count()>1)
			m_moduleList.first()->getDisplay()->Display( &m_moduleList );
		else
			m_moduleList.first()->getDisplay()->Display( m_moduleList.first() );
 		m_htmlWidget->setText( m_moduleList.first()->getDisplay()->getHTML() );
	}
	if (m_key != vKey)
		m_key->key(vKey->key());
		
	m_htmlWidget->scrollToAnchor( QString::number(vKey->Verse()) );
	setUpdatesEnabled(true);		
	setCaption(windowCaption());	
}

/** This slot is called when the modules selected in the module chooserbar have changed. */
void CBiblePresenter::modulesChanged(){
  m_moduleList = m_moduleChooserBar->getModuleList();
  if (!m_moduleList.count()) {
  	close();
  }
  else {
		m_displaySettingsButton->reset(m_moduleList);
//    refreshFeatures()
	  m_key->module(m_moduleList.first());
	  m_keyChooser->setModule(m_moduleList.first());	
		m_htmlWidget->setModules(m_moduleList);
	  	
	  lookup(m_key);
	}
}
/**  */
void CBiblePresenter::optionsChanged(){
	lookup(m_key);
}


/** Initializes the Signal / Slot connections */
void CBiblePresenter::initConnections(){
	connect(m_htmlWidget, SIGNAL(referenceClicked(const QString&, const QString&)),
		this, SLOT(lookup(const QString&, const QString&))); 	
	connect(m_htmlWidget, SIGNAL(referenceDropped(const QString&)),
		this, SLOT(referenceDropped(const QString&)));

 	connect(m_keyChooser, SIGNAL(keyChanged(CSwordKey*)),
 		this, SLOT(lookup(CSwordKey*)));
	connect(m_popup, SIGNAL(aboutToShow()),
		SLOT(popupAboutToShow()));
	connect(m_moduleChooserBar, SIGNAL( sigChanged() ),
		SLOT(modulesChanged() ));
	connect(m_displaySettingsButton, SIGNAL( sigChanged() ),	
		SLOT(optionsChanged() ));
}

/** No descriptions */
void CBiblePresenter::popupAboutToShow() {
	m_popup->setItemEnabled(ID_PRESENTER_LOOKUP, !m_htmlWidget->selectedText().isEmpty());

	m_copyPopup->setItemEnabled(ID_PRESENTER_COPY_ONLY_KEY,!m_htmlWidget->getCurrentAnchor().isEmpty());	
	m_copyPopup->setItemEnabled(ID_PRESENTER_COPY_KEY_TEXT,!m_htmlWidget->getCurrentAnchor().isEmpty());	
	m_copyPopup->setItemEnabled(ID_PRESENTER_COPY_KEY,!m_htmlWidget->getCurrentAnchor().isEmpty());			
	m_copyPopup->setItemEnabled(ID_PRESENTER_COPY_SELECTED, !m_htmlWidget->selectedText().isEmpty());
	
	m_printPopup->setItemEnabled(ID_PRESENTER_PRINT_KEY,!m_htmlWidget->getCurrentAnchor().isEmpty());			

	m_savePopup->setItemEnabled(ID_PRESENTER_SAVE_KEY,!m_htmlWidget->getCurrentAnchor().isEmpty());
}

/** Reimplementation from CSwordPresenter. */
void CBiblePresenter::lookup(const QString& module, const QString& key){
	CSwordModuleInfo* m = backend()->findModuleByName(module);
	if (m && m_moduleList.containsRef(m)) {
		if (!key.isEmpty())
			m_key->key(key);
		m_keyChooser->setKey(m_key); //the key chooser does send an update signal	
	}
	else {
		emit lookupInModule(module, key);
	}
}

/** Reimplementation. Refreshes the things which are described by the event integer. */
void CBiblePresenter::refresh( ){

	CSwordPresenter::refresh();	//refreshes the display settings button

	m_key->setLocale((const char*)backend()->booknameLanguage().local8Bit());
	m_keyChooser->refreshContent();
	lookup(m_key);
	m_htmlWidget->refresh();		
}


/** Copies the highlighted text into clipboard. */
void CBiblePresenter::copyVerse(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::Type type;	
	CReferenceManager::decodeHyperlink(currentAnchor, module, key, type);	
	CSwordModuleInfo* m = backend()->findModuleByName(module);		
	
	CSwordVerseKey vKey(m);
	vKey.key(key);
		
	QClipboard *cb = KApplication::clipboard();
	cb->setText(vKey.key());
}

/** Copies the highlighted text into clipboard. */
void CBiblePresenter::copyVerseText(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::Type type;
	CReferenceManager::decodeHyperlink(currentAnchor, module, key, type);	
	CSwordModuleInfo* m = backend()->findModuleByName(module);		
	
	CSwordVerseKey vKey(m);
	vKey.key(key);	
	QClipboard *cb = KApplication::clipboard();
	cb->setText(vKey.strippedText());
}

/** Copies the highlighted text into clipboard. */
void CBiblePresenter::copyVerseAndText(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::Type type;	
	CReferenceManager::decodeHyperlink(currentAnchor, module, key, type);	
	CSwordModuleInfo* m = backend()->findModuleByName(module);		
	CSwordVerseKey vKey(m);
	vKey.key(key);
	
	const QString text = QString("%1\n%2").arg(vKey.key()).arg(vKey.strippedText());
	QClipboard *cb = KApplication::clipboard();
	cb->setText(text);
}

//print functions
/** Copies the highlighted text into clipboard. */
void CBiblePresenter::printVerseAndText(){
	QString key = QString::null;
	QString module = QString::null;
	const QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::Type type;	
	CReferenceManager::decodeHyperlink(currentAnchor, module, key, type);	
	CSwordModuleInfo* m = backend()->findModuleByName(module);		
	
	CSwordVerseKey vKey(m);	
	vKey.key(key);

	CExportManager::printKey(m, vKey.key());
}

/** Copies the highlighted text into clipboard. */
void CBiblePresenter::printChapter(){
	CSwordVerseKey startKey(*m_key);
	startKey.Verse(1);
	
	CSwordVerseKey stopKey(*m_key);	

	CSwordBibleModuleInfo* b = dynamic_cast<CSwordBibleModuleInfo*>(m_moduleList.first());
	if (b)
		stopKey.Verse( b->verseCount( b->bookNumber(startKey.book()), startKey.Chapter() ) );
	CExportManager::printKey(m_moduleList.first(), startKey.key(), stopKey.key());
}

//save functions

/** Copies the highlighted text into clipboard. */
void CBiblePresenter::saveVerseAndText(){
	QString key = QString::null;
	QString module = QString::null;
	CReferenceManager::Type type;		
	const QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	
	CReferenceManager::decodeHyperlink(currentAnchor, module, key, type);
	CSwordVerseKey vKey( backend()->findModuleByName(module) );
	vKey.key(key);
	
	CExportManager::saveKey(&vKey, true);
}

/** Inserts the actions used by this window class into the given KAccel object. */
void CBiblePresenter::insertKeyboardActions(KAccel* a){
	a->setConfigGroup("Bible window");
	a->insertItem(i18n("Next book"), "Next book", 0);
	a->insertItem(i18n("Previous book"), "Previous book", 0);	
	
	a->insertItem(i18n("Next chapter"), "Next chapter", 0);
	a->insertItem(i18n("Previous chapter"), "Previous chapter", 0);	

	a->insertItem(i18n("Next verse"), "Next verse", 0);
	a->insertItem(i18n("Previous verse"), "Previous verse", 0);	
}

/** Initializes the accelerator object. */
void CBiblePresenter::initAccels(){
//	ASSERT(m_accel);
	m_accel->setConfigGroup("Bible window");
	
	m_accel->insertItem(i18n("Next book"), "Next book", 0);
	m_accel->connectItem("Next book", this, SLOT(nextBook()));	
	m_accel->insertItem(i18n("Previous book"), "Previous book", 0);	
	m_accel->connectItem("Previous book", this, SLOT(previousBook()));
		
	m_accel->insertItem(i18n("Next chapter"), "Next chapter", 0);
	m_accel->connectItem("Next chapter", this, SLOT(nextChapter()));		
	m_accel->insertItem(i18n("Previous chapter"), "Previous chapter", 0);	
	m_accel->connectItem("Previous chapter", this, SLOT(previousChapter()));	

	m_accel->insertItem(i18n("Next verse"), "Next verse", 0);
	m_accel->connectItem("Next verse", this, SLOT(nextVerse()));		
	m_accel->insertItem(i18n("Previous verse"), "Previous verse", 0);	
	m_accel->connectItem("Previous verse", this, SLOT(previousVerse()));	
	
	m_accel->readSettings();
	CSwordPresenter::initAccels();	
}

/** Jumps to the next entry */
void CBiblePresenter::nextBook(){
	if (m_key->next(CSwordVerseKey::UseBook))
		m_keyChooser->setKey(m_key);	
}

/** Jumps to the previous entry. */
void CBiblePresenter::previousBook(){
	if (m_key->previous(CSwordVerseKey::UseBook))
		m_keyChooser->setKey(m_key);
}


/** Jumps to the next entry */
void CBiblePresenter::nextChapter(){
	if (m_key->next(CSwordVerseKey::UseChapter))
		m_keyChooser->setKey(m_key);	
}

/** Jumps to the previous entry. */
void CBiblePresenter::previousChapter(){
	if (m_key->previous(CSwordVerseKey::UseChapter))
		m_keyChooser->setKey(m_key);
}

/** Jumps to the next entry */
void CBiblePresenter::nextVerse(){
//	qWarning("CBiblePresenter::nextVerse()");
	if (	m_key->next(CSwordVerseKey::UseVerse) )	
		m_keyChooser->setKey(m_key);	
}

/** Jumps to the previous entry. */
void CBiblePresenter::previousVerse(){
	if (m_key->previous(CSwordVerseKey::UseVerse))
		m_keyChooser->setKey(m_key);
}

/** Stores some Bible window dpecific setttings in the profile. */
void CBiblePresenter::storeSettings( CProfileWindow* settings ){
	CSwordPresenter::storeSettings(settings);
	/**
	* store the state of the displaysetttings button in the profile
	* we use a binary digit to store the state. The most right digit is 1 if the
	* first button is set, 0 if it isn't. The second digit is for the second entry in the menu.
	* etc
	*/
	const int count = m_displaySettingsButton->menuItemCount();
	int result = 0;
	//now check	every item
	for (int i = 1; i<count; i++) { //first item is a title
		if (m_displaySettingsButton->itemStatus(i)) //item is checked
			result += (int)pow(2,i-1);//add 2^i (the i. digit in binary)
	}
	settings->setWindowSettings(result);
}

/** Applies some Bible window specific setttings. */
void CBiblePresenter::applySettings( CProfileWindow* settings ){
	CSwordPresenter::applySettings(settings);
  int result = settings->windowSettings();
	const int count = m_displaySettingsButton->menuItemCount();
	for (int i = count-1; i>=1; i--) {
		if (result-(int)pow(2,i-1)>= 0) { //2^i was added before, so item with index i is set
			result -= (int)pow(2,i-1);
			m_displaySettingsButton->setItemStatus(i,true);
		}
		else
			m_displaySettingsButton->setItemStatus(i,false);			
	}		
	m_displaySettingsButton->setChanged();
}
