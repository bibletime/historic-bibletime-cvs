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
#include "../ctoolclass.h"
#include "../chtmlwidget.h"
#include "../keychooser/ckeychooser.h"
#include "../../ressource.h"
#include "../../backend/cswordbiblemoduleinfo.h"
#include "../../backend/cswordversekey.h"
#include "../../backend/chtmlchapterdisplay.h"
#include "../cprofile.h"
#include "../cprofilewindow.h"

//Qt includes
#include <qclipboard.h>
#include <qlist.h>

//KDE includes
#include <kapp.h>
#include <ktoolbar.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kaccel.h>

CBiblePresenter::CBiblePresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent, const char *name )
	: CSwordPresenter(useModules, importantClasses, parent,name),
	m_key( new CSwordVerseKey(m_moduleList.first()) )
{		
	m_key->key("Genesis 1:1");
	
	initView();
	show();	
	initConnections();	
}

CBiblePresenter::~CBiblePresenter(){
//	qDebug("destructor of CBiblePresenter");
	delete m_key;
}

/** Initializes the view (central widget, toolbars etc) of this presenter */
void CBiblePresenter::initView(){
	m_mainToolBar = new KToolBar(this);
	m_keyChooser = CKeyChooser::createInstance(m_moduleList.first(), m_key, m_mainToolBar);
	m_mainToolBar->insertWidget(0,m_keyChooser->sizeHint().width(),m_keyChooser);	
	addToolBar(m_mainToolBar);			
	
	m_moduleChooserBar = new CModuleChooserBar(m_important, m_moduleList, CSwordModuleInfo::Bible, this );
	addToolBar(m_moduleChooserBar);
	
	m_htmlWidget = new CHTMLWidget(m_important, true, this);
		
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
		
	setCentralWidget(m_htmlWidget);	
	setIcon( BIBLE_ICON_SMALL );	
}

/** Displays the chapter using the aparameter. */
void CBiblePresenter::lookup(CSwordKey* key){
	setUpdatesEnabled(false);	
	
	CSwordVerseKey* vKey = dynamic_cast<CSwordVerseKey*>(key);	
	if (!vKey)
		return;
  m_moduleList.first()->module()->SetKey(*vKey);
		
	if (m_moduleList.first()->getDisplay()) {	//do we have a display object?
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
	
	QWidget::setCaption( windowCaption() );	
}

/** This slot is called when the modules selected in the module chooserbar have changed. */
void CBiblePresenter::modulesChanged(){
  m_moduleList = m_moduleChooserBar->getModuleList();
  if (!m_moduleList.count()) {
  	close();
  }
  else {
    refreshFeatures();
	  m_key->module(m_moduleList.first());
	  m_keyChooser->setModule(m_moduleList.first());	
	
	  lookup(m_key);
	}
}

/** Initializes the Signal / Slot connections */
void CBiblePresenter::initConnections(){
	connect(m_htmlWidget, SIGNAL(referenceClicked(const QString&)),
		this, SLOT(lookup(const QString&))); 	
 	connect(m_keyChooser, SIGNAL(keyChanged(CSwordKey*)),
 		this, SLOT(lookup(CSwordKey*)));
	connect(m_popup, SIGNAL(aboutToShow()),
		SLOT(popupAboutToShow()));
	connect(m_moduleChooserBar, SIGNAL( sigChanged() ),
		SLOT(modulesChanged() ));
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
		
//	m_popup->setItemEnabled(ID_PRESENTER_PRINT_VERSE, !m_htmlWidget->getCurrentAnchor().isEmpty());
//	m_popup->setItemEnabled(ID_PRESENTER_COPY_VERSE, !m_htmlWidget->getCurrentAnchor().isEmpty());	
}

/** Reimplementation from CSwordPresenter. */
void CBiblePresenter::lookup(const QString& key){
	if (!key.isEmpty())
		m_key->key(key);
	m_keyChooser->setKey(m_key); //the key chooser send an update signal
}

/** Reimplementation. Refreshes the things which are described by the event integer. */
void CBiblePresenter::refresh( const int events ){	
	bool doLookup = false;
	bool refreshHTMLWidget = false;
	
	if (events & languageChanged) {
		m_key->setLocale((const char*)m_important->swordBackend->getCurrentBooknameLanguage().local8Bit());
		m_keyChooser->refreshContent();
		doLookup = true;
	}
	
	if ( (events & backgroundChanged) || (events & textColorChanged) || (events & verseNumberColorChanged) )
		refreshHTMLWidget = true;
	if ( events & highlightedVerseColorChanged )
		doLookup = true;
		
	if ( events & fontChanged ) {
		doLookup = true;
		refreshHTMLWidget = true;
	}
	
	//check for footnotes
	if (events & footnotesChanged) {
		for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
			if ( m_moduleList.current()->supportsFeature(CSwordBackend::footnotes) ) {
				doLookup = true;	
				break;
			}
		}
	}
	
	//check for strongs support
	if (events & strongNumbersChanged) {
		for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
			if ( m_moduleList.current()->supportsFeature(CSwordBackend::strongNumbers) ) {
				doLookup = true;	
				break;
			}
		}
	}

	//check for headings support
	if (events & headingsChanged) {
		for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
			if ( m_moduleList.current()->supportsFeature(CSwordBackend::headings) ) {
				doLookup = true;	
				break;
			}
		}
	}
				
	//check for morphological tags support
	if (events & morphTagsChanged) {
		for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
			if ( m_moduleList.current()->supportsFeature(CSwordBackend::morphTags) ) {
				doLookup = true;	
				break;
			}
		}
	}
	
	if (doLookup)
		lookup(m_key);
	if (refreshHTMLWidget)
		m_htmlWidget->refresh();		
}


/** Copies the highlighted text into clipboard. */
void CBiblePresenter::copyVerse(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::decodeHyperlink(currentAnchor, module, key);	
	CSwordModuleInfo* m = m_importantClasses->swordBackend->findModuleByName(module);		
	CSwordVerseKey key(m);
	key.key(key);
		
	QClipboard *cb = KApplication::clipboard();
	cb->setText(key.key());
}

/** Copies the highlighted text into clipboard. */
void CBiblePresenter::copyVerseText(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::decodeHyperlink(currentAnchor, module, key);	
	CSwordModuleInfo* m = m_importantClasses->swordBackend->findModuleByName(module);		
	CSwordVerseKey key(m);
	key.key(key);
	
	QClipboard *cb = KApplication::clipboard();
	cb->setText(key.strippedText());
}

/** Copies the highlighted text into clipboard. */
void CBiblePresenter::copyVerseAndText(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::decodeHyperlink(currentAnchor, module, key);	
	CSwordModuleInfo* m = m_importantClasses->swordBackend->findModuleByName(module);		
	CSwordVerseKey key(m);
	key.key(key);
	
	const QString text = QString("%1\n%2").arg(key.key()).arg(key.strippedText());
	QClipboard *cb = KApplication::clipboard();
	cb->setText(text);
}

//print functions
/** Copies the highlighted text into clipboard. */
void CBiblePresenter::printVerseAndText(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::decodeHyperlink(currentAnchor, module, key);	
	CSwordModuleInfo* m = m_importantClasses->swordBackend->findModuleByName(module);		
	CSwordVerseKey key(m);
	key.key(key);

	printKey(key, key, m_moduleList.first());
}

/** Copies the highlighted text into clipboard. */
void CBiblePresenter::printChapter(){
	CSwordVerseKey *startKey = new CSwordVerseKey(m_moduleList.first());	//this key is deleted by the printem
	startKey->key(m_key->key());
		
	CSwordVerseKey *stopKey = new CSwordVerseKey(m_moduleList.first());	//this key is deleted by the printem	
	stopKey->key(m_key->key());
	
	CSwordBibleModuleInfo* b = dynamic_cast<CSwordBibleModuleInfo*>(m_moduleList.first());
	if (b)
		stopKey->Verse( b->getVerseCount(b->getBookNumber(startKey->book()),startKey->Chapter()) );
	printKey(startKey, stopKey, m_moduleList.first());	
}

//save functions

/** Copies the highlighted text into clipboard. */
void CBiblePresenter::saveVerseAndText(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::decodeHyperlink(currentAnchor, module, key);	
	CSwordModuleInfo* m = m_importantClasses->swordBackend->findModuleByName(module);		
	CSwordVerseKey key(m);
	key.key(key);
	
	const QString text = QString::fromLatin1("%1\n%2").arg(key.key()).arg(key.strippedText());	
	const QString file = CToolClass::getSaveFileName(QString::null, i18n("*.txt | Text file (*.txt)\n*.* | All files (*.*)"), 0, i18n("Save verse with text as ..."));
	if (!file.isNull())
		CToolClass::savePlainFile( file, text);
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
	qWarning("CBiblePresenter::initAccels()");
	CSwordPresenter::initAccels();
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
}

/** Jumps to the next entry */
void CBiblePresenter::nextBook(){
	m_key->NextBook();
	m_keyChooser->setKey(m_key);	
}

/** Jumps to the previous entry. */
void CBiblePresenter::previousBook(){
	m_key->PreviousBook();
	m_keyChooser->setKey(m_key);
}


/** Jumps to the next entry */
void CBiblePresenter::nextChapter(){
	m_key->NextChapter();
	m_keyChooser->setKey(m_key);	
}

/** Jumps to the previous entry. */
void CBiblePresenter::previousChapter(){
	m_key->PreviousChapter();
	m_keyChooser->setKey(m_key);
}

/** Jumps to the next entry */
void CBiblePresenter::nextVerse(){
	m_key->NextVerse();
	m_keyChooser->setKey(m_key);	
}

/** Jumps to the previous entry. */
void CBiblePresenter::previousVerse(){
	m_key->PreviousVerse();
	m_keyChooser->setKey(m_key);
}

