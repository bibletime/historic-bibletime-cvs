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
#include "cdisplaysettingsbutton.h"

#include "../chtmlwidget.h"
#include "../cexportmanager.h"
#include "../keychooser/ckeychooser.h"
#include "../../ressource.h"
#include "../../backend/cswordldkey.h"
#include "../../backend/chtmlentrydisplay.h"
#include "../../backend/cswordbackend.h"


//Qt includes
#include <qclipboard.h>

//KDE icnludes
#include <klocale.h>
#include <ktoolbar.h>
#include <kpopupmenu.h>
#include <kfiledialog.h>
#include <kapp.h>
#include <kaccel.h>

CLexiconPresenter::CLexiconPresenter(ListCSwordModuleInfo useModules, QWidget *parent, const char *name )
	: CSwordPresenter(useModules, parent,name),
	m_key( new CSwordLDKey(m_moduleList.first()) )
{
	m_key->key("");
//	m_key->SWKey::operator=( TOP );
//	*m_key = TOP;

	initView();
	show();
	initConnections();			
	
	setInitialized();
}

CLexiconPresenter::~CLexiconPresenter(){
	delete m_key;
}

/** Initializes the view. */
void CLexiconPresenter::initView(){
	qWarning("CLexiconPresenter::initView()");
	
	m_mainToolBar = new KToolBar(this);
	m_keyChooser = CKeyChooser::createInstance(m_moduleList.first(), m_key, m_mainToolBar);
	m_mainToolBar->insertWidget(0,m_keyChooser->sizeHint().width(),m_keyChooser);	
	m_mainToolBar->setItemAutoSized(0);
	addToolBar(m_mainToolBar);
	
//	m_displaySettingsButton = new CDisplaySettingsButton( &m_displayOptions, &m_moduleOptions, m_moduleList, m_mainToolBar);
//	m_mainToolBar->insertWidget(1,m_displaySettingsButton->sizeHint().width(),m_displaySettingsButton);

	m_moduleChooserBar = new CModuleChooserBar(m_moduleList, CSwordModuleInfo::Lexicon, this );
	addToolBar(m_moduleChooserBar);
	
	m_htmlWidget = new CHTMLWidget(true, this);
		
	//setup popup menu
	m_popup = new KPopupMenu(this);
	m_popup->insertTitle(i18n("Lexicon window"));
	
	m_copyPopup = new KPopupMenu(m_popup);	
	m_copyPopup->insertItem(i18n("Entry"), this, SLOT(copyEntry()),0,ID_PRESENTER_COPY_ONLY_KEY);
	m_copyPopup->insertItem(i18n("Text of entry"), this, SLOT(copyEntryText()),0,ID_PRESENTER_COPY_KEY_TEXT);	
	m_copyPopup->insertItem(i18n("Entry with text"), this, SLOT(copyEntryAndText()),0,ID_PRESENTER_COPY_KEY);
	m_copyPopup->insertSeparator();
	m_copyPopup->insertItem(i18n("Selected text"), m_htmlWidget, SLOT(copy()),0,ID_PRESENTER_COPY_SELECTED);
	
	m_printPopup = new KPopupMenu(m_popup);
	m_printPopup->insertItem(i18n("Entry with text"), this, SLOT(printEntry()),0,ID_PRESENTER_PRINT_KEY);

	m_savePopup = new KPopupMenu(m_popup);	
	m_savePopup->insertItem(i18n("Entry as plain text"), m_htmlWidget, SLOT(slotSaveAsText()),0,ID_PRESENTER_SAVE_CHAPTER);
	m_savePopup->insertItem(i18n("Entry as HTML"), m_htmlWidget, SLOT(slotSaveAsHTML()),0,ID_PRESENTER_SAVE_CHAPTER_HTML);	

	m_popup->insertItem(i18n("Select all"), m_htmlWidget, SLOT(slotSelectAll()),0, ID_PRESENTER_SELECT_ALL);
  m_popup->insertItem(i18n("Lookup selected text in lexicon"), m_lexiconPopup, ID_PRESENTER_LOOKUP);	
	m_popup->insertSeparator();	
	m_popup->insertItem(SmallIcon(ICON_EDIT_COPY),i18n("Copy..."), 	m_copyPopup, ID_PRESENTER_COPY_POPUP);	
	m_popup->insertItem(SmallIcon(ICON_FILE_PRINT), i18n("Add to printing queue..."), m_printPopup, ID_PRESENTER_PRINT_POPUP);	
	m_popup->insertItem(SmallIcon(ICON_FILE_SAVE), i18n("Save..."), 	m_savePopup,ID_PRESENTER_SAVE_POPUP);		


	m_htmlWidget->installPopup(m_popup);	
	m_htmlWidget->installAnchorMenu(m_popup);
	m_htmlWidget->setModules(m_moduleList);	

	setCentralWidget(m_htmlWidget);
	setIcon(LEXICON_ICON_SMALL);
}

/** No descriptions */
void CLexiconPresenter::initConnections(){
	qWarning("CLexiconPresenter::initConnections()");
	connect(m_htmlWidget, SIGNAL(referenceClicked(const QString&, const QString&)),
		this, SLOT(lookup(const QString&, const QString&)));
	connect(m_htmlWidget, SIGNAL(referenceDropped(const QString&)),
		this, SLOT(referenceDropped(const QString&)));

 	connect(m_keyChooser, SIGNAL(keyChanged(CSwordKey*)),
		this, SLOT(lookup(CSwordKey*)));
	connect(m_popup, SIGNAL(aboutToShow()),this,
		SLOT(popupAboutToShow()));
	connect(m_moduleChooserBar, SIGNAL(sigChanged()), this,
		SLOT(modulesChanged()));
}

/** No descriptions */
void CLexiconPresenter::lookup(CSwordKey* key){
	qWarning("CLexiconPresenter::lookup(CSwordKey* key)");
	setUpdatesEnabled(false);	
	
	CSwordLDKey* ldKey = dynamic_cast<CSwordLDKey*>(key);
	if (!ldKey)
		return;

	backend()->setFilterOptions( m_moduleOptions );
	backend()->setDisplayOptions( m_displayOptions );

	m_moduleList.first()->module()->SetKey(*ldKey);
	
	if (m_moduleList.first()->getDisplay()) {	//do we have a display object?
		if (m_moduleChooserBar->getModuleList().count() > 1)  //we want to display more than one module
			m_moduleList.first()->getDisplay()->Display( &m_moduleList );
		else
			m_moduleList.first()->getDisplay()->Display( m_moduleList.first() );
		m_htmlWidget->setText(m_moduleList.first()->getDisplay()->getHTML());
	}	
	if (m_key != ldKey)
		m_key->key(ldKey->key());
		
	setUpdatesEnabled(true);
	setCaption( windowCaption() );
	qWarning("finished CLexiconPresenter::lookup(CSwordKey* key)");
}

/** No descriptions */
void CLexiconPresenter::popupAboutToShow(){
	m_popup->setItemEnabled(ID_PRESENTER_LOOKUP, !m_htmlWidget->selectedText().isEmpty());

	m_copyPopup->setItemEnabled(ID_PRESENTER_COPY_SELECTED, !m_htmlWidget->selectedText().isEmpty());	
}

/** No descriptions */
void CLexiconPresenter::lookup(const QString& module, const QString& key){
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

/** Refreshes all parts decsribed by the parameter. */
void CLexiconPresenter::refresh( ){

//	CSwordPresenter::refresh();	//refreshes the display settings button

	lookup(m_key);
	m_htmlWidget->refresh();
}

/** Is called when the modules shown by this display window were changed. */
void CLexiconPresenter::modulesChanged(){
  m_moduleList = m_moduleChooserBar->getModuleList();
  if (!m_moduleList.count())
  	close();
  else {
//		m_displaySettingsButton->reset(m_moduleList);
//    refreshFeatures();
	  m_key->module(m_moduleList.first());
	  m_keyChooser->setModule(m_moduleList.first());	
		m_htmlWidget->setModules(m_moduleList);		
	  lookup(m_key);
	}
}
/**  */
void CLexiconPresenter::optionsChanged(){
	lookup(m_key);
}

/** Copies the highlighted text into clipboard. */
void CLexiconPresenter::copyEntry(){
	CSwordLDKey key(m_moduleList.first());	//this key is deleted by the printem
	key.key(m_key->key());
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	if (currentAnchor.left(8) == "sword://")
		currentAnchor = currentAnchor.mid(8, currentAnchor.length()-(currentAnchor.right(1) == "/" ? 9 : 8));
	key.key(currentAnchor);
		
	QClipboard *cb = KApplication::clipboard();
	cb->setText(key.key());
}

/** Copies the highlighted text into clipboard. */
void CLexiconPresenter::copyEntryText(){
	CSwordLDKey key(m_moduleList.first());	//this key is deleted by the printem
	key.key(m_key->key());
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	if (currentAnchor.left(8) == "sword://")
		currentAnchor = currentAnchor.mid(8, currentAnchor.length()-(currentAnchor.right(1) == "/" ? 9 : 8));
	key.key(currentAnchor);
	
	QClipboard *cb = KApplication::clipboard();
	cb->setText(key.strippedText());
}

/** Copies the highlighted text into clipboard. */
void CLexiconPresenter::copyEntryAndText(){
	CSwordLDKey key(m_moduleList.first());	//this key is deleted by the printem
	key.key(m_key->key());
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	if (currentAnchor.left(8) == "sword://")
		currentAnchor = currentAnchor.mid(8, currentAnchor.length()-(currentAnchor.right(1) == "/" ? 9 : 8));
	key.key(currentAnchor);
	
	const QString text = QString("%1\n%2").arg(key.key()).arg(key.strippedText());
	QClipboard *cb = KApplication::clipboard();
	cb->setText(text);
}

//print functions
void CLexiconPresenter::printEntry(){
	CExportManager::printKey(m_moduleList.first(), m_key->key());
}

/** Inserts the used keyboard actions into the given KAccel object. */
void CLexiconPresenter::insertKeyboardActions(KAccel* a){
	a->setConfigGroup("Lexicon window");	
	a->insertItem(i18n("Next entry"), "Next entry", 0);
	a->insertItem(i18n("Previous entry"), "Previous entry", 0);	
}

/** Initializes keyboard accelerators. */
void CLexiconPresenter::initAccels(){
	CSwordPresenter::initAccels();
	m_accel->setConfigGroup("Lexicon window");	
	m_accel->insertItem(i18n("Next entry"), "Next entry", 0);
	m_accel->connectItem("Next entry", this, SLOT(nextEntry()));
		
	m_accel->insertItem(i18n("Previous entry"), "Previous entry", 0);		
	m_accel->connectItem("Previous entry", this, SLOT(previousEntry()));
		
	m_accel->readSettings();
}

/** Jumps to the previous entry. */
void CLexiconPresenter::previousEntry(){
	m_key->PreviousEntry();
	m_keyChooser->setKey(m_key);
}

/** Jumps to the next entry */
void CLexiconPresenter::nextEntry(){
	m_key->NextEntry();
	m_keyChooser->setKey(m_key);	
}
