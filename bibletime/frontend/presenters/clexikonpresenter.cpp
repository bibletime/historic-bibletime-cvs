/***************************************************************************
                          CLexiconPresenter.cpp  -  description
                             -------------------
    begin                : Thu Jan 20 2000
    copyright            : (C) 2000 by The BibleTime Team
    email                : Info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "clexikonpresenter.h"

//own includes
#include "../keychooser/ckeychooser.h"
#include "../../ressource.h"
#include "../chtmlwidget.h"
#include "../actions.h"
#include "../../whatsthisdef.h"
#include "../../backend/ckey.h"
#include "../../backend/sword_backend/cswordldkey.h"
#include "../../backend/sword_backend/chtmlentrydisplay.h"
#include "../../printing/cprinter.h"
#include "../../printing/cprintitem.h"

//QT includes

//KDE includes
#include <klineedit.h>
#include <kcompletion.h>
#include <kcombobox.h>
#include <kmenubar.h>
#include <kapp.h>
#include <kiconloader.h>
#include <klocale.h>
#include <kaction.h>

CLexiconPresenter::CLexiconPresenter(CImportantClasses* importantClasses, QWidget* parent, const char* name, CModuleInfo* module )
	: CModulePresenter( importantClasses,parent, name, module) {
	qDebug("constructor of CLexiconPresenter");
	
	try {
		m_key = new CSwordLDKey(m_swordModule);
	}
	catch (CKey::EBadModule) {
		qWarning("Assigned wrong module to key!");
	}
	m_key->setKey(" ");
	
	availableFeatures = 0;
	if (m_swordModule->supportsFeature( CSwordBackend::strongNumbers )){
		if (availableFeatures != 0)
			availableFeatures |= strongNumbers;
		else
			availableFeatures = strongNumbers;
	}
	if (m_swordModule->supportsFeature( CSwordBackend::footnotes )){
		if (availableFeatures != 0)
			availableFeatures |= footnotes;
		else
			availableFeatures = footnotes;
	}
	
	initActions();
	initView();
	initConnections();
	
	m_keyChooser->setKey(m_key);
}

CLexiconPresenter::~CLexiconPresenter(){
	qDebug("CLexiconPresenter::~CLexiconPresenter()");
	if (m_key)
		delete m_key;
}

/**  */
void CLexiconPresenter::refresh(const int useFeatures){
	bool doLookup = false;
	if ( (useFeatures & CModulePresenter::textColor)
			|| (useFeatures & CModulePresenter::verseNumberColor)
			|| (useFeatures & CModulePresenter::highlightedVerseColor)
			|| (useFeatures & CModulePresenter::font) ) {
			
		html_widget->refresh();
		doLookup = true;
	}
	
	if (doLookup)
		m_keyChooser->setKey(m_key);
}

/**  */
void CLexiconPresenter::lookup(CKey* key){
	qDebug("CLexiconPresenter::lookup(CKey* key)");
	if (!key)
		return;
	ASSERT(m_swordModule);
	ASSERT(key);
	ASSERT(html_widget);
	
	CSwordLDKey* ldKey = dynamic_cast<CSwordLDKey*>(key);
	if (!ldKey || (const char*)*ldKey == "")
		return;

	m_swordModule->module()->SetKey(*ldKey);
	if (m_swordModule->getDisplay()) {	//we have a valid display object
		m_swordModule->getDisplay()->Display( m_swordModule );
		html_widget->setText(m_swordModule->getDisplay()->getHTML());
	}
	setCaption( caption() );
}

/** Reimplementation */
void CLexiconPresenter::copy(){
		html_widget->copy();
}

/** Initializes connections between SIGNALs and SLOTs */
void CLexiconPresenter::initConnections(){
 	qDebug("CLexiconPresenter::initConnections()");
 	ASSERT(m_keyChooser);
 	connect( m_keyChooser, SIGNAL(keyChanged(CKey*)),
 		this, SLOT(lookup(CKey*)));
}

/** Initializes the view */
void CLexiconPresenter::initView(){
	m_mainToolbar = new KToolBar(this);
	addToolBar(m_mainToolbar);		
	
	m_keyChooser = CKeyChooser::createInstance(m_swordModule, m_key, m_mainToolbar);	
	const int ID = 0;	
	m_mainToolbar->insertWidget(ID, m_keyChooser->sizeHint().width(), m_keyChooser);

	synchronize_action->plug(m_mainToolbar);
		
	html_widget = new CHTMLWidget(this, "html_widget");	
	html_widget->setReadOnly( true );
		
	m_popup = new KPopupMenu(this);		
	m_popup->insertTitle(i18n("Lexicon presenter"));
	m_popup->insertItem(i18n("Save entry as HTML..."), html_widget, SLOT(slotSaveAsHTML()), 0,ID_PRESENTER_SAVE_AS_HTML);	
	m_popup->insertItem(i18n("Save entry as plain text..."), html_widget, SLOT(slotSaveAsText()),0, ID_PRESENTER_SAVE_AS_TEXT);
	m_popup->insertSeparator();
	m_popup->insertItem(i18n("Select all"), html_widget, SLOT(slotSelectAll()), 0,ID_PRESENTER_SELECT_ALL);
	m_popup->insertSeparator();	
	m_popup->insertItem(i18n("Copy selected text"), html_widget, SLOT(copy()),0, ID_PRESENTER_COPY_SELECTED);
	m_popup->insertItem(i18n("Copy entry into clipboard"), html_widget, SLOT(slotCopyAll()), 0,ID_PRESENTER_COPY_ALL);
	m_popup->insertSeparator();
	m_popup->insertItem(i18n("Add entry to print queue"), this, SLOT(slotPrintCurrentEntry()), 0,ID_PRESENTER_PRINT_ENTRY);
	
	html_widget->installPopup(m_popup);	
	
	setIcon( LEXICON_ICON_SMALL );	
	setCentralWidget(html_widget);
}

/** Returns the caption for this presenter window. */
QString CLexiconPresenter::caption() const {
	return QString("%1: %2")
		.arg(m_swordModule->getDescription())
		.arg(QString::fromLocal8Bit(m_swordModule->module() ? m_swordModule->module()->KeyText() : (const char*)*m_key));
}

/** Looks up the word given by the first parameter. */
void CLexiconPresenter::lookupWord( const QString& word){
	qDebug("CLexiconPresenter::lookupWord( const QString& word)");
	m_key->setKey(word);
	
	
	m_keyChooser->setKey( m_key );
	
	
}

/** No descriptions */
void CLexiconPresenter::initActions(){
	synchronize_action =
		new KToggleAction(i18n("Synchronize"), "synchronize",
			0, this, SLOT(synchronizeToggled()),
			actionCollection(), "synchronize_action");
#warning Add keybinding for synchronize			
}

/** No descriptions */
void CLexiconPresenter::synchronizeToggled(){
	qDebug("CLexiconPresenter::synchronizeToggled()");
	m_doSynchronize = synchronize_action->isChecked();
}

/** This function appends the current entry of the lexicon to the print queue. */
void CLexiconPresenter::slotPrintCurrentEntry(){
	debug("CCommentaryPresenter::printHighlightedVerse()");
	CPrintItem*	printItem = new CPrintItem();
	
	CSwordLDKey *key = new CSwordLDKey(m_swordModule);	//this key is deleted by the printem
	key->setKey(QString::fromLocal8Bit((const char*)*m_key));	
	printItem->setModule(m_swordModule);
	printItem->setStartKey(key);
	printItem->setStopKey(key);
	
	m_important->printer->addItemToQueue( printItem );	
}

/** This slot is called before the popup is shown. */
void CLexiconPresenter::slotPopupAboutToShow(){
	m_popup->setItemEnabled(ID_PRESENTER_COPY_SELECTED, html_widget->hasSelectedText());	
}
