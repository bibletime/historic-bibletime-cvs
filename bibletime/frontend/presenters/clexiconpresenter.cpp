/***************************************************************************
                          clexiconpresenter.cpp  -  description
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

#include "clexiconpresenter.h"
#include "cmodulechooserbar.h"
#include "../chtmlwidget.h"
#include "../keychooser/ckeychooser.h"
#include "../../ressource.h"
#include "../../backend/sword_backend/cswordldkey.h"
#include "../../backend/sword_backend/chtmlentrydisplay.h"

//KDE icnludes
#include <klocale.h>
#include <ktoolbar.h>
#include <kpopupmenu.h>

CLexiconPresenter::CLexiconPresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent, const char *name )
	: CSwordPresenter(useModules, importantClasses, parent,name) {

	m_key = new CSwordLDKey(m_moduleList.first());
	m_key->setKey("");
	
	initView();
	show();		
	initConnections();		
}

CLexiconPresenter::~CLexiconPresenter(){
	delete m_key;
}

/** Initializes the view. */
void CLexiconPresenter::initView(){
	ASSERT(m_moduleList.first());
	
	m_mainToolBar = new KToolBar(this);
	m_keyChooser = CKeyChooser::createInstance(m_moduleList.first(), m_key, m_mainToolBar);
	m_mainToolBar->insertWidget(0,m_keyChooser->sizeHint().width(),m_keyChooser);	
	addToolBar(m_mainToolBar);
	
	m_moduleChooserBar = new CModuleChooserBar(m_important, m_moduleList, CSwordModuleInfo::Lexicon, this );
	m_moduleChooserBar->setButtonLimit(1);	
	addToolBar(m_moduleChooserBar);
	
	m_htmlWidget = new CHTMLWidget(this);
		
	//setup popup menu
	m_popup = new KPopupMenu(this);
	m_popup->insertTitle(i18n("Lexicon presenter"));
	m_popup->insertItem(i18n("Save entry as HTML..."), m_htmlWidget, SLOT(slotSaveAsHTML()), 0,ID_PRESENTER_SAVE_AS_HTML);	
	m_popup->insertItem(i18n("Save entry as plain text..."), m_htmlWidget, SLOT(slotSaveAsText()),0,ID_PRESENTER_SAVE_AS_TEXT);
	m_popup->insertSeparator();
	m_popup->insertItem(i18n("Select all"), m_htmlWidget, SLOT(slotSelectAll()),0, ID_PRESENTER_SELECT_ALL);
	m_popup->insertItem(i18n("Copy selected text"), m_htmlWidget, SLOT(copy()),0,ID_PRESENTER_COPY_SELECTED);	
	m_popup->insertItem(i18n("Copy entry into clipboard"), m_htmlWidget, SLOT(copyDocument()),0,ID_PRESENTER_COPY_ALL);
	m_popup->insertSeparator();		
  m_popup->insertItem(i18n("Lookup word in lexicon"), m_lexiconPopup, ID_PRESENTER_LOOKUP );	
	m_popup->insertSeparator();			
	m_popup->insertItem(i18n("Add entry to print queue"), this, SLOT(printEntry()),0, ID_PRESENTER_PRINT_ENTRY);	
	
	m_htmlWidget->installPopup(m_popup);	
	m_htmlWidget->installAnchorMenu(m_popup);

	setCentralWidget(m_htmlWidget);
	setIcon(LEXICON_ICON_SMALL);
}

/** No descriptions */
void CLexiconPresenter::initConnections(){
	connect(m_htmlWidget, SIGNAL(referenceClicked(const QString&)),
		this, SLOT(referenceClicked(const QString&))); 	
 	connect( m_keyChooser, SIGNAL(keyChanged(CKey*)),
 		this, SLOT(lookup(CKey*)));
	connect(m_popup, SIGNAL(aboutToShow()),
		SLOT(popupAboutToShow()));
	connect(m_moduleChooserBar, SIGNAL( sigChanged() ),
		SLOT( moduleChanged() ));
}

/** No descriptions */
void CLexiconPresenter::lookup(CKey* key){
	if (!key)
		return;
	CSwordLDKey* ldKey = dynamic_cast<CSwordLDKey*>(key);
	if (!ldKey || ldKey->getKey().isEmpty())
		return;
	m_moduleList.first()->module()->SetKey(*ldKey);
	if (m_moduleList.first()->getDisplay()) {	//we have a valid display object
		m_moduleList.first()->getDisplay()->Display( m_moduleList.first() );
		m_htmlWidget->setText(m_moduleList.first()->getDisplay()->getHTML());
	}	
	setPlainCaption( m_key->getKey() );
}

/** No descriptions */
void CLexiconPresenter::moduleChanged(){
  m_moduleList = m_moduleChooserBar->getModuleList();
  refreshFeatures();
  if (!m_moduleList.count())
  	close();
  else {
	  m_key->setModule(m_moduleList.first());
	  m_keyChooser->setModule(m_moduleList.first());
	  lookup(m_key);
	}
}

/** No descriptions */
void CLexiconPresenter::popupAboutToShow(){
	m_popup->setItemEnabled(ID_PRESENTER_COPY_SELECTED, m_htmlWidget->hasSelectedText());
	m_popup->setItemEnabled(ID_PRESENTER_LOOKUP, m_htmlWidget->hasSelectedText());
}

/** No descriptions */
void CLexiconPresenter::referenceClicked( const QString& ref){
	if ( ref.isEmpty() )
		return;		
	m_key->setKey(ref);
	m_keyChooser->setKey(m_key);
}

/** No descriptions */
void CLexiconPresenter::lookup(const QString& key){
	if (!key.isEmpty())
		m_key->setKey(key);		
	m_keyChooser->setKey(m_key); //the key chooser does send an update signal	
}

/** Refreshes all parts decsribed by the parameter. */
void CLexiconPresenter::refresh( const int events){
	bool doLookup = false;
	bool refreshHTMLWidget = false;
	
	if ( (events & backgroundChanged) || (events & textColorChanged) )
		refreshHTMLWidget = true;
	if ( events & fontChanged )
		doLookup = true;
	
	//check for footnotes			
	if (doLookup)
		lookup(m_key);
	if (refreshHTMLWidget)
		m_htmlWidget->refresh();
}

/** Printes the displayed entry of the used module. */
void CLexiconPresenter::printEntry(){
	
}
