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
#include "../chtmlwidget.h"
#include "../keychooser/ckeychooser.h"
#include "../../ressource.h"
#include "../../backend/sword_backend/cswordversekey.h"
#include "../../backend/sword_backend/chtmlchapterdisplay.h"

//Qt includes
#include <qclipboard.h>

//KDE includes
#include <kapp.h>
#include <ktoolbar.h>
#include <klocale.h>


CBiblePresenter::CBiblePresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent, const char *name )
	: CSwordPresenter(useModules, importantClasses, parent,name) {
	m_key = 0;
	if (m_moduleList.first())
		m_key = new CSwordVerseKey(m_moduleList.first());	
	m_key->setKey("Genesis 1:1");
	
	initView();
	show();	
	initConnections();	
}

CBiblePresenter::~CBiblePresenter(){
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
	
	m_htmlWidget = new CHTMLWidget(this);
		
	//setup popup menu
	m_popup = new KPopupMenu(this);
	m_popup->insertTitle(i18n("Bible window"));
	m_popup->insertItem(i18n("Save chapter as HTML..."), m_htmlWidget, SLOT(slotSaveAsHTML()), 0,ID_PRESENTER_SAVE_AS_HTML);	
	m_popup->insertItem(i18n("Save chapter as plain text..."), m_htmlWidget, SLOT(slotSaveAsText()),0,ID_PRESENTER_SAVE_AS_TEXT);
	m_popup->insertItem(i18n("Copy chapter into clipboard"), m_htmlWidget, SLOT(copyDocument()),0,ID_PRESENTER_COPY_ALL);		
	m_popup->insertSeparator();
	m_popup->insertItem(i18n("Add verse to print queue"), this, SLOT(printHighlightedVerse()),0, ID_PRESENTER_PRINT_VERSE);			
	m_popup->insertItem(i18n("Copy verse into clipboard"), this, SLOT(copyHighlightedVerse()),0, ID_PRESENTER_COPY_VERSE);
	m_popup->insertSeparator();
	m_popup->insertItem(i18n("Select all"), m_htmlWidget, SLOT(slotSelectAll()),0, ID_PRESENTER_SELECT_ALL);
	m_popup->insertItem(i18n("Copy selected text"), m_htmlWidget, SLOT(copy()),0,ID_PRESENTER_COPY_SELECTED);	
  m_popup->insertItem(i18n("Lookup selected text in lexicon"), m_lexiconPopup, ID_PRESENTER_LOOKUP );	
	
	m_htmlWidget->installPopup(m_popup);			
	m_htmlWidget->installAnchorMenu( m_popup );
		
	setCentralWidget(m_htmlWidget);	
	setIcon( BIBLE_ICON_SMALL );	
}

/** Displays the chapter using the aparameter. */
void CBiblePresenter::lookup(CKey* key){
	setUpdatesEnabled(false);	
	
	CSwordVerseKey* vKey = dynamic_cast<CSwordVerseKey*>(key);	
	if (!vKey)
		return;
  m_moduleList.first()->module()->SetKey(*vKey);
		
	if (m_moduleList.first()->getDisplay()) {	//do we have a display object?
		if (m_moduleChooserBar->getModuleList().count()>1)  //we want to display more than one module
			m_moduleList.first()->getDisplay()->Display( &m_moduleList );
		else
			m_moduleList.first()->getDisplay()->Display( m_moduleList.first() );
 		m_htmlWidget->setText( m_moduleList.first()->getDisplay()->getHTML() );
	}
	if (m_key != vKey)
		m_key->setKey(*vKey);
		
	m_htmlWidget->scrollToAnchor( QString::number(vKey->Verse()) );
	setUpdatesEnabled(true);		
	
	setPlainCaption( caption() );	
}

/** This slot is called when the modules selected in the module chooserbar have changed. */
void CBiblePresenter::modulesChanged(){
  m_moduleList = m_moduleChooserBar->getModuleList();
  if (!m_moduleList.count())
  	close();
  else {
    refreshFeatures();
	  m_key->setModule(m_moduleList.first());
	  m_keyChooser->setModule(m_moduleList.first());	
	
	  lookup(m_key);
	}
}

/** Initializes the Signal / Slot connections */
void CBiblePresenter::initConnections(){
	connect(m_htmlWidget, SIGNAL(referenceClicked(const QString&)),
		this, SLOT(lookup(const QString&))); 	
 	connect(m_keyChooser, SIGNAL(keyChanged(CKey*)),
 		this, SLOT(lookup(CKey*)));
	connect(m_popup, SIGNAL(aboutToShow()),
		SLOT(popupAboutToShow()));
	connect(m_moduleChooserBar, SIGNAL( sigChanged() ),
		SLOT(modulesChanged() ));
}


/** No descriptions */
void CBiblePresenter::popupAboutToShow(){
	m_popup->setItemEnabled(ID_PRESENTER_COPY_SELECTED, m_htmlWidget->hasSelectedText());
	m_popup->setItemEnabled(ID_PRESENTER_LOOKUP, m_htmlWidget->hasSelectedText());
	m_popup->setItemEnabled(ID_PRESENTER_PRINT_VERSE, !m_htmlWidget->getCurrentAnchor().isEmpty());
	m_popup->setItemEnabled(ID_PRESENTER_COPY_VERSE, !m_htmlWidget->getCurrentAnchor().isEmpty());	
}

/** Reimplementation from CSwordPresenter. */
void CBiblePresenter::lookup(const QString& key){
	if (!key.isEmpty())
		m_key->setKey(key);
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
	if ( (events & highlightedVerseColorChanged) || (events & fontChanged) )
		doLookup = true;
	
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
			
	if (doLookup)
		lookup(m_key);
	if (refreshHTMLWidget)
		m_htmlWidget->refresh();		
}

/** Printes the verse the user has chosen. */
void CBiblePresenter::printHighlightedVerse(){
	CSwordVerseKey *key = new CSwordVerseKey(m_moduleList.first());	//this key is deleted by the printem
	key->setKey(m_key->getKey());
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	if (currentAnchor.left(8) == "sword://")
		currentAnchor = currentAnchor.mid(8, currentAnchor.length()- (currentAnchor.right(1) == "/" ? 9 : 8));
	key->setKey(currentAnchor);
	
	printKey(key, key, m_moduleList.first());
}

/** Printes the verse the user has chosen. */
void CBiblePresenter::copyHighlightedVerse(){
	CSwordVerseKey key(m_moduleList.first());	//this key is deleted by the printem
	key.setKey(m_key->getKey());
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	if (currentAnchor.left(8) == "sword://")
		currentAnchor = currentAnchor.mid(8, currentAnchor.length()-(currentAnchor.right(1) == "/" ? 9 : 8));
	key.setKey(currentAnchor);
	
	const QString text = QString("%1\n%2").arg(key.getKey()).arg(key.getStrippedText());
	QClipboard *cb = KApplication::clipboard();
	cb->setText(text);
}
/** Reimplementation. */
const QString CBiblePresenter::caption() const{
	return m_key->getKey();
}
