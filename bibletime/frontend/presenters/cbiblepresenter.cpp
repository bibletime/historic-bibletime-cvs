/***************************************************************************
                          cbiblepresenter.cpp  -  description
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

//own includes
#include "cbiblepresenter.h"
#include "clexikonpresenter.h"
#include "cmodulechooserbar.h"
#include "cmodulechooserbutton.h"
#include "../cmdiarea.h"
#include "../chtmlwidget.h"
#include "../actions.h"
#include "../keychooser/ckeychooser.h"
#include "../../printing/cprinter.h"
#include "../../printing/cprintitem.h"
#include "../ctoolclass.h"
#include "../../ressource.h"
#include "../../backend/ckey.h"
#include "../../whatsthisdef.h"
#include "../../backend/sword_backend/cswordbackend.h"
#include "../../backend/sword_backend/cswordversekey.h"
#include "../../backend/sword_backend/cswordbiblemoduleinfo.h"
#include "../../backend/sword_backend/cswordbackend.h"
#include "../../backend/sword_backend/chtmlentrydisplay.h"

//QT includes
#include <qwidget.h>
#include <qstrlist.h>
#include <qtoolbar.h>
#include <qpixmap.h>

//KDE includes
#include <kconfig.h>
#include <kstddirs.h>
#include <kiconloader.h>
#include <kglobal.h>
#include <ktoolbar.h>
#include <kcombobox.h>
#include <klocale.h>
#include <kaction.h>
#include <kstdaction.h>
#include <kmenubar.h>
#include <kcombobox.h>
#include <kcompletion.h>

//Sword includes
#include <swkey.h>
#include <versekey.h>
#include <swmodule.h>

CBiblePresenter::CBiblePresenter(CImportantClasses* importantClasses, QWidget *parent, const char* name, CModuleInfo* module )
	: CModulePresenter ( importantClasses, parent, name, module ) {	
	
	m_moduleList.setAutoDelete(false);	
	m_moduleList.append( m_swordModule );
	
	m_mainToolbar = 0;
	m_key = 0;
	m_keyChooser = 0;
	html_widget = 0;
	m_anchorPopup = 0;
	m_popup = 0;
	m_moduleChooserBar = 0;
		
	try {
		m_key = new CSwordVerseKey(m_swordModule);
	}
	catch (CKey::EBadModule) {
		qWarning("Assigned wrong module to key!");
	}	
	m_key->setKey("Genesis 1:1");
	
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

	
	initView();		
	initConnections();		//should be put short after initActions() and initView()
	setHelpMenuEnabled(false);	
	
	m_keyChooser->setKey(m_key);	
}

CBiblePresenter::~CBiblePresenter() {
	if (m_key)
		delete m_key;
}

/** Initialize the view */
void CBiblePresenter::initView() {		
	//init the toolbar(s)
	m_mainToolbar = new KToolBar(this);
	addToolBar(m_mainToolbar);		
	m_keyChooser = CKeyChooser::createInstance(m_swordModule, m_key, m_mainToolbar);			
	
	m_moduleChooserBar = new CModuleChooserBar(m_important, &m_moduleList, CSwordModuleInfo::Bible, this );
	addToolBar(m_moduleChooserBar);
		
//	ListCSwordModuleInfo* moduleList =  m_important->swordBackend->getModuleList();
//	ASSERT(moduleList);	
//	m_moduleChooserPopup = new KPopupMenu(this);	
//	m_moduleChooserPopup->insertTitle(i18n("Select additional modules"));
//	m_moduleChooserPopup->setCheckable(true);
//	connect(m_moduleChooserPopup, SIGNAL(activated(int)), this, SLOT(slotModuleChosen(int)));
//	for (CSwordModuleInfo* module = moduleList->first(); module; module = moduleList->next()) {
//		ASSERT(module);
//		if ( dynamic_cast<CSwordBibleModuleInfo*>(module) && module != m_swordModule) { //Bibles and commentaries
//			m_moduleChooserPopup->insertItem(QString::fromLocal8Bit(module->module()->Name()), moduleList->at());
//		}
//	}	
//	m_mainToolbar->insertButton(BIBLE_ICON_SMALL, ID_PRESENTER_CHOOSE_MODULES, m_moduleChooserPopup, true, i18n("Choose additional modules to display"));	
	
		
	html_widget = new CHTMLWidget(this);
//	html_widget->setWordTracking(false);
	html_widget->setReadOnly( true );
	
	m_popup = new KPopupMenu(this);		
	m_popup->insertTitle(i18n("Bible presenter"));
	m_popup->insertItem(i18n("Save chapter as HTML..."), html_widget, SLOT(slotSaveAsHTML()), 0,ID_PRESENTER_SAVE_AS_HTML);	
	m_popup->insertItem(i18n("Save chapter as plain text..."), html_widget, SLOT(slotSaveAsText()),0,ID_PRESENTER_SAVE_AS_TEXT);
	m_popup->insertSeparator();
	m_popup->insertItem(i18n("Select all"), html_widget, SLOT(slotSelectAll()),0, ID_PRESENTER_SELECT_ALL);
	m_popup->insertSeparator();	
	m_popup->insertItem(i18n("Copy selected text"), html_widget, SLOT(copy()),0,ID_PRESENTER_COPY_SELECTED);	
	m_popup->insertItem(i18n("Copy chapter into clipboard"), html_widget, SLOT(slotCopyAll()),0,ID_PRESENTER_COPY_ALL);
	
	html_widget->installPopup(m_popup);	
	
	m_anchorPopup = new KPopupMenu(html_widget);
	m_anchorPopup->insertTitle(i18n("Bible presenter"));	
	m_anchorPopup->insertItem( i18n("Add verse to print queue"), this, SLOT(slotPrintHighlightedVerse()));
	html_widget->installAnchorMenu( m_anchorPopup );	//use m_anchorPopup as RMB menu for anchors
	
	setIcon( BIBLE_ICON_SMALL );
	setCentralWidget( html_widget );
}

/**  */
void CBiblePresenter::lookup(CKey* key){
  qDebug("1: void CBiblePresenter::lookup(CKey* key)");
  qDebug("2: void CBiblePresenter::lookup(CKey* key)");

	setUpdatesEnabled(false);
	
	CSwordVerseKey* vKey = dynamic_cast<CSwordVerseKey*>(key);	
	if (!vKey) {
		qDebug("CBiblePresenter: Wrong type of Key!");
		return;
	}		
//	m_swordModule->module()->SetKey(*vKey);
  ASSERT(m_moduleList.first());
  m_moduleList.first()->module()->SetKey(*vKey);
  qDebug("3: void CBiblePresenter::lookup(CKey* key)");	
		
	if (m_moduleList.first()->getDisplay()) {	//do we have a display object?
		if (m_moduleChooserBar->getModuleList().count()>1)      //we want to display more than one module
			m_moduleList.first()->getDisplay()->Display( &m_moduleList );//render text using tables
		else
			m_moduleList.first()->getDisplay()->Display( m_moduleList.first() );	//render text
		html_widget->setText( m_moduleList.first()->getDisplay()->getHTML() );											
	}
	else
		qWarning("CBiblePresenter: We don't have a display object!");

	if (m_key != vKey) {
		m_key->setKey(QString::fromLocal8Bit((const char*)*vKey));
	}
	
	html_widget->scrollToAnchor( QString::number(vKey->Verse()) );
	setCaption( caption() );	
	
	setUpdatesEnabled(true);	
}

/**  */
void CBiblePresenter::refresh(const int useFeatures){
	qDebug("CBiblePresenter::refresh(const int useFeatures)");
	bool doLookup = false;
	bool refreshHTMLWidget = false;
	
	if (useFeatures & CModulePresenter::language) {
		m_key->setLocale((const char*)m_important->swordBackend->getCurrentBooknameLanguage().local8Bit());
		m_keyChooser->refreshContent();
		doLookup = true;
	}
	
	if ( (useFeatures & CModulePresenter::backgroundColor) || (useFeatures & CModulePresenter::textColor) || (useFeatures & CModulePresenter::verseNumberColor) ) {
		qDebug("textcolor, versenumber color or background changed");
		refreshHTMLWidget = true;
	}
	if ( (useFeatures & CModulePresenter::highlightedVerseColor) || (useFeatures & CModulePresenter::font) )	 {
		qDebug("HighlightedVerseColor changed");
		doLookup = true;
	}
	
	//check for footnotes
	if (useFeatures & CModulePresenter::footnotes) {
		qDebug("do we use footnotes?");
		for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
			if ( m_moduleList.current()->supportsFeature(CSwordBackend::footnotes) ) {
				doLookup = true;	
				qDebug("Yes!");
//				break;
			}
		}
	}
	
	//check for strongs support
	if (useFeatures & CModulePresenter::strongNumbers)
		for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next())
			if ( m_moduleList.current()->supportsFeature(CSwordBackend::strongNumbers) )
				doLookup = true;	
	if (doLookup)
		lookup(m_key);
	if (refreshHTMLWidget)
		html_widget->refresh();		
}

/**  */
void CBiblePresenter::readSettings(){
}

/** Initializes SLOTs and SIGNALs */
void CBiblePresenter::initConnections(){
	connect(html_widget, SIGNAL(referenceClicked(const QString&)),
		this, SLOT(slotReferenceClicked(const QString&))); 	
 	connect( m_keyChooser, SIGNAL(keyChanged(CKey*)),
 		this, SLOT(lookup(CKey*)));
 	connect(html_widget, SIGNAL(sigClickedOnWord(const QString&)),
 		SLOT(slotWordClicked(const QString&)));
	connect(m_popup, SIGNAL(aboutToShow()),
		SLOT(slotPopupAboutToShow()));
	connect(m_moduleChooserBar, SIGNAL( sigChanged() ),
		SLOT( slotModulesChanged() ));
}

/**  */
void CBiblePresenter::slotReferenceClicked( const QString& url){
	qDebug("CBiblePresenter::slotReferenceClicked( const QString& url)");
	if ( !url.isNull() ) {
		m_key->setKey(url);
		m_keyChooser->setKey( m_key );
	}
}

/** Reimplementation */
void CBiblePresenter::copy(){
	html_widget->copy();
}

/** Prints the highlighted verse. */
void CBiblePresenter::slotPrintHighlightedVerse(){
	debug("CBiblePresenter::printHighlightedVerse()");
	CPrintItem*	printItem = new CPrintItem();
	
	CSwordVerseKey *key = new CSwordVerseKey(m_moduleList.first());	//this key is deleted by the printem
	key->setKey(QString::fromLocal8Bit((const char*)*m_key));
	
	qDebug((const char*)html_widget->anchorAt(html_widget->m_pressedPos).local8Bit());
	/**
		* The anchors have a leading sword://,
		* so we should remove it before we set this to the key
		*/
	const QString currentAnchor = html_widget->anchorAt(html_widget->m_pressedPos);
	const QString newKey = currentAnchor.mid(8,currentAnchor.length()-9 );
	key->setKey( newKey );
	qDebug((const char*)*key);
	
	printItem->setModule(m_moduleList.first());	
	printItem->setStartKey(key);
	printItem->setStopKey(key);
	m_important->printer->addItemToQueue( printItem );
}

/** Returns the caption for this presenter window. */
QString CBiblePresenter::caption() {
	return QString("%1: %2").arg(m_moduleList.first()->getDescription()).arg(QString::fromLocal8Bit((const char*)*m_key));
}

/** This slot is called, when the HTML widget send a word, on which the user clicked. */
void CBiblePresenter::slotWordClicked( const QString& word){
	qDebug("CBiblePresenter::wordClicked( const QString& )");
	
	//get te list of the available presenter, sort out the lexicon presenters and
	//check if they have word-lookup enabled.
	//Look up the word

	CMDIArea* mdi;
	//I think we have to use use two times parentWidget() because the QWorkspace widgets adds a wrapper widget
	if (parentWidget()->parentWidget()->inherits("CMDIArea"))
		mdi = (CMDIArea*)parentWidget()->parentWidget();		
	else
		return;
		
	QWidgetList presenters = mdi->windowList();	
	CLexiconPresenter* lex = 0;
	for (QWidget* w = presenters.first(); w; w=presenters.next() ) {
		if ( (lex = dynamic_cast<CLexiconPresenter*>(w)) ) {
			if (lex->synchronizeEnabled())
				lex->lookupWord(word);
		}
	}	
}

/** Is called before the popup is shown. */
void CBiblePresenter::slotPopupAboutToShow(){
	m_popup->setItemEnabled(ID_PRESENTER_COPY_SELECTED, html_widget->hasSelectedText());
}

/** Is called after a module was added/deleted from the popup menu. */
void CBiblePresenter::slotModuleChosen(int id){
//	CSwordModuleInfo* module = m_important->swordBackend->getModuleList()->at(id);
//	
//	m_moduleChooserPopup->setItemChecked(id, !m_moduleChooserPopup->isItemChecked(id));
//	//refresh the m_modules list
//	m_moduleList->clear();
//	m_moduleList->append(m_swordModule);
//	
//	ListCSwordModuleInfo* moduleList =  m_important->swordBackend->getModuleList();
//	for (module = moduleList->first(); module; module = moduleList->next()) {
//		if (m_moduleChooserPopup->isItemChecked(moduleList->at())) { //modules is wanted
//			m_moduleList->append(module);
//		}
//	}		
//	
//	lookup(m_key);
}
/** called when the module chooser was modified */
void CBiblePresenter::slotModulesChanged(){
  m_moduleList = m_moduleChooserBar->getModuleList();
  if (!m_moduleList.count())
  	close();
  else
	  lookup(m_key);
}
