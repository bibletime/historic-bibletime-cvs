/***************************************************************************
                          ccommentarypresenter.cpp  -  description
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

#include "ccommentarypresenter.h"
#include "cmodulechooserbar.h"
#include "cdisplaysettingsbutton.h"

#include "../ctoolclass.h"
#include "../chtmlwidget.h"
#include "../keychooser/ckeychooser.h"
#include "../../ressource.h"
#include "../../whatsthisdef.h"
#include "../../backend/cswordversekey.h"
#include "../../backend/chtmlchapterdisplay.h"
#include "../../backend/creferencemanager.h"
#include "../../backend/cswordbackend.h"


//Qt includes
#include <qclipboard.h>

//KDE includes
#include <ktoolbar.h>
#include <kaction.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kaccel.h>

CCommentaryPresenter::CCommentaryPresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent, const char *name )
	: CSwordPresenter(useModules, importantClasses, parent,name),
	m_key( new CSwordVerseKey(m_moduleList.first()) ), m_editToolBar(0)
{
	m_key->key("Genesis 1:1");
	
	initView();	
	show();		
	initConnections();		
}

CCommentaryPresenter::~CCommentaryPresenter(){
	checkChanges(); //save text if it was changed after last save
	delete m_key;
}

/** Initializes our view. */
void CCommentaryPresenter::initView(){
	m_mainToolBar = new KToolBar(this);
	m_keyChooser = CKeyChooser::createInstance(m_moduleList.first(), m_key, m_mainToolBar);
	m_mainToolBar->insertWidget(0,m_keyChooser->sizeHint().width(),m_keyChooser);	
	addToolBar(m_mainToolBar);			

//	m_displaySettingsButton = new CDisplaySettingsButton( &m_displayOptions, &m_moduleOptions, m_moduleList, m_mainToolBar);
//	m_mainToolBar->insertWidget(1,m_displaySettingsButton->sizeHint().width(),m_displaySettingsButton);
	
	m_moduleChooserBar = new CModuleChooserBar(m_important, m_moduleList, CSwordModuleInfo::Commentary, this );
	addToolBar(m_moduleChooserBar);
	
	presenterSync_action =  new KToggleAction(i18n("Synchronize..."), ICON_SYNC,
															IDK_PRESENTER_EDIT, this,	SLOT(syncToggled()), actionCollection(), "syncComment_action");
	presenterSync_action->setWhatsThis( WT_PRESENTER_SYNC );
	presenterSync_action->plug(m_mainToolBar);
	
	presenterEdit_action =  new KToggleAction(i18n("Edit entry..."), ICON_EDIT,
															IDK_PRESENTER_EDIT, this,	SLOT(editComment()), actionCollection(), "editComment_action");
	presenterEdit_action->setEnabled( m_moduleList.first()->module()->isWritable() );
	presenterEdit_action->setWhatsThis( WT_PRESENTER_EDIT );
	presenterEdit_action->plug(m_mainToolBar);
	
	m_htmlWidget = new CHTMLWidget(m_important, true, this);
	ASSERT(m_htmlWidget);
	
	//setup popup menu
	m_popup = new KPopupMenu(this);
	m_popup->insertTitle(i18n("Commentary window"));
	
	m_copyPopup = new KPopupMenu(m_popup);
	m_copyPopup->insertItem(i18n("Entry"), this, SLOT(copyEntry()),0,ID_PRESENTER_COPY_ONLY_KEY);
	m_copyPopup->insertItem(i18n("Text of entry"), this, SLOT(copyEntryText()),0,ID_PRESENTER_COPY_KEY_TEXT);	
	m_copyPopup->insertItem(i18n("Entry with text"), this, SLOT(copyEntryAndText()),0,ID_PRESENTER_COPY_KEY);
	m_copyPopup->insertSeparator();
	m_copyPopup->insertItem(i18n("Selected text"), m_htmlWidget, SLOT(copy()),0,ID_PRESENTER_COPY_SELECTED);
	
	m_printPopup = new KPopupMenu(m_popup);
	m_printPopup->insertItem(i18n("Entry with text"), this, SLOT(printHighlightedVerse()),0,ID_PRESENTER_PRINT_KEY);

	m_savePopup = new KPopupMenu(m_popup);	
	m_savePopup->insertItem(i18n("Entry as plain text"), m_htmlWidget, SLOT(slotSaveAsText()),0,ID_PRESENTER_SAVE_CHAPTER);
	m_savePopup->insertItem(i18n("Entry as HTML"), m_htmlWidget, SLOT(slotSaveAsHTML()),0,ID_PRESENTER_SAVE_CHAPTER_HTML);	

	m_popup->insertItem(i18n("Select all"), m_htmlWidget, SLOT(slotSelectAll()),0, ID_PRESENTER_SELECT_ALL);
  m_popup->insertItem(i18n("Lookup selected text in lexicon"), m_lexiconPopup, ID_PRESENTER_LOOKUP);	
	m_popup->insertSeparator();	
	m_popup->insertItem(SmallIcon(ICON_EDIT_COPY),i18n("Copy..."), 	m_copyPopup, ID_PRESENTER_COPY_POPUP);	
	m_popup->insertItem(SmallIcon(ICON_FILE_PRINT), i18n("Add to printing queue..."), m_printPopup, ID_PRESENTER_PRINT_POPUP);	
	m_popup->insertItem(SmallIcon(ICON_FILE_SAVE), i18n("Save..."), 	m_savePopup,ID_PRESENTER_SAVE_POPUP);		

	ASSERT(m_htmlWidget);
	m_htmlWidget->installPopup(m_popup);		
	m_htmlWidget->installAnchorMenu( m_popup );
	qWarning("before set icon");
	setCentralWidget(m_htmlWidget);	
	setIcon( COMMENTARY_ICON_SMALL );
}

/** Initializes the connections */
void CCommentaryPresenter::initConnections(){
	connect(m_htmlWidget, SIGNAL(referenceClicked(const QString&)),
		this, SLOT(lookup(const QString&))); 	 	
	connect(m_htmlWidget, SIGNAL(sigDeleteDocument()),
		this, SLOT(deleteText())); 	 	
	connect(m_htmlWidget, SIGNAL(sigSaveDocument(const QString)),
		this, SLOT(saveText(const QString))); 	 	 	
	connect(m_htmlWidget, SIGNAL(insertReference(const QString&)),
		this, SLOT(insertReference(const QString&))); 	 	 			
 	
 	connect( m_keyChooser, SIGNAL(beforeKeyChange(const QString&)),
 		this, SLOT(beforeKeyChange(const QString&)));
 	connect( m_keyChooser, SIGNAL(keyChanged(CSwordKey*)),
 		this, SLOT(lookup(CSwordKey*)));		
	
	connect(m_popup,SIGNAL(aboutToShow()),
		SLOT(popupAboutToShow()));
	
	connect(m_moduleChooserBar, SIGNAL( sigChanged() ),
		SLOT( modulesChanged() ));	
//	connect(m_displaySettingsButton, SIGNAL( sigChanged() ),	
//		SLOT(optionsChanged() ));
}

/** Is called when the selected modules changed. */
void CCommentaryPresenter::modulesChanged(){
  m_moduleList = m_moduleChooserBar->getModuleList();

  if (!m_moduleList.count())
  	close();
  else {
		presenterEdit_action->setEnabled( m_moduleList.first()->module()->isWritable() );
//		m_displaySettingsButton->reset(m_moduleList);
//	  refreshFeatures();	
	  m_key->module(m_moduleList.first());
	  m_keyChooser->setModule(m_moduleList.first());	
	
	  lookup(m_key);
	}
}
/**  */
void CCommentaryPresenter::optionsChanged(){
	lookup(m_key);
}

/** renders text and set it to the HTML widget */
void CCommentaryPresenter::lookup(CSwordKey* key){	
	setUpdatesEnabled(false);
	CSwordVerseKey* vKey = dynamic_cast<CSwordVerseKey*>(key);
	if (!vKey)
		return;
//	vKey->Persist(1);

	m_important->swordBackend->setAllModuleOptions( m_moduleOptions );
	m_important->swordBackend->setAllDisplayOptions( m_displayOptions );

  m_moduleList.first()->module()->SetKey(*vKey);

	if (m_moduleList.first()->getDisplay()) {	//do we have a display object?
		if (m_htmlWidget->isReadOnly())	 {
			if (m_moduleList.count()>1)
				m_moduleList.first()->getDisplay()->Display( &m_moduleList );
			else
				m_moduleList.first()->getDisplay()->Display( m_moduleList.first() );
			m_htmlWidget->setText( m_moduleList.first()->getDisplay()->getHTML() );
		}
		else
			m_htmlWidget->setText( QString::fromLocal8Bit( m_moduleList.first()->module()->getRawEntry() ) );
	}	
	if (m_key != vKey)
		m_key->key(vKey->key());
		
	m_htmlWidget->scrollToAnchor( QString::number(vKey->Verse()) );
	m_htmlWidget->setModified(false);
	setUpdatesEnabled(true);
	
	setCaption( windowCaption() );	
}

/** No descriptions */
void CCommentaryPresenter::popupAboutToShow(){
	m_popup->setItemEnabled(ID_PRESENTER_LOOKUP, !m_htmlWidget->selectedText().isEmpty());
	m_copyPopup->setItemEnabled(ID_PRESENTER_COPY_SELECTED, !m_htmlWidget->selectedText().isEmpty());	
}

/** Saves the given text in the module. */
void CCommentaryPresenter::saveText(const QString text){
	m_moduleList.first()->module()->SetKey(m_key);
	if (!text.isEmpty())
		*(m_moduleList.first()->module()) << (const char*)text.local8Bit();
	else
		m_moduleList.first()->module()->deleteEntry();		
}

/** Deletes the displayed and edited text. */
void CCommentaryPresenter::deleteText(){
	m_moduleList.first()->module()->deleteEntry();
	m_htmlWidget->clear();
}

void CCommentaryPresenter::editComment(){
	m_htmlWidget->setReadOnly( !m_htmlWidget->isReadOnly() );	
	if (!m_htmlWidget->isReadOnly() && !m_editToolBar) {
		m_editToolBar = new KToolBar(this);
		addToolBar(m_editToolBar);
		
		m_htmlWidget->createEditToolbar( m_editToolBar );
	}	
	if (!m_htmlWidget->isReadOnly() && !m_editToolBar)
		return;
		
	if (!m_htmlWidget->isReadOnly())
		m_editToolBar->show();
	else
		m_editToolBar->hide();
			
	lookup( m_key );	
	m_htmlWidget->setFocus();
}

/** Reimplementation. */
void CCommentaryPresenter::lookup(const QString& key){
	if (!key.isEmpty())
		m_key->key(key);
	m_keyChooser->setKey(m_key); //the key chooser send an update signal
}

/** No descriptions */
void CCommentaryPresenter::refresh( ){
	m_key->setLocale((const char*)m_important->swordBackend->getCurrentBooknameLanguage().local8Bit());
	m_keyChooser->refreshContent();
	lookup(m_key);
	m_htmlWidget->refresh();		
}

/** Printes the verse the user has chosen. */
void CCommentaryPresenter::printHighlightedVerse(){
	CSwordVerseKey* key = new CSwordVerseKey(m_moduleList.first());	//this key is deleted by the printem
	key->key(m_key->key());	
	printKey(key, key, m_moduleList.first());
}

/** The sync button was pressed for synchronization. */
void CCommentaryPresenter::syncToggled(){
}

/** Synchronizes to the given key if sync is enabled. */
void CCommentaryPresenter::synchronize( CSwordKey* syncKey ){
	if (!presenterSync_action->isChecked() || !syncKey)
		return;
	checkChanges();
	m_key->key(syncKey->key());
	m_keyChooser->setKey(m_key);
}

/** No descriptions */
void CCommentaryPresenter::insertReference(const QString& reference){
	if (m_htmlWidget->isReadOnly())
		return;
	CSwordVerseKey vk(m_moduleList.first());
	vk.key(reference);
	m_htmlWidget->insert(vk.strippedText());
}

/** Copies the highlighted text into clipboard. */
void CCommentaryPresenter::copyEntry(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::decodeHyperlink(currentAnchor, module, key);	
	CSwordModuleInfo* m = m_important->swordBackend->findModuleByName(module);		
	
	CSwordVerseKey vKey(m);
	vKey.key(key);
		
	QClipboard *cb = KApplication::clipboard();
	cb->setText(vKey.key());
}

/** Copies the highlighted text into clipboard. */
void CCommentaryPresenter::copyEntryText(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::decodeHyperlink(currentAnchor, module, key);	
	CSwordModuleInfo* m = m_important->swordBackend->findModuleByName(module);		
	
	CSwordVerseKey vKey(m);
	vKey.key(key);
	
	QClipboard *cb = KApplication::clipboard();
	cb->setText(vKey.strippedText());
}

/** Copies the highlighted text into clipboard. */
void CCommentaryPresenter::copyEntryAndText(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::decodeHyperlink(currentAnchor, module, key);	
	CSwordModuleInfo* m = m_important->swordBackend->findModuleByName(module);		
	
	CSwordVerseKey vKey(m);
	vKey.key(key);
	
	const QString text = QString("%1\n%2").arg(vKey.key()).arg(vKey.strippedText());
	QClipboard *cb = KApplication::clipboard();
	cb->setText(text);
}

//print functions
/** Copies the highlighted text into clipboard. */
void CCommentaryPresenter::printEntryAndText(){

	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_htmlWidget->getCurrentAnchor();
	CReferenceManager::decodeHyperlink(currentAnchor, module, key);	
	CSwordModuleInfo* m = m_important->swordBackend->findModuleByName(module);		
	
	CSwordVerseKey* vKey = new CSwordVerseKey(m);	//this key is deleted by the printem
	vKey->key(key);
		
	printKey(vKey, vKey, m);
}

/** Checks for changes and saves the text. */
void CCommentaryPresenter::checkChanges(){
//	qDebug("void CCommentaryPresenter::checkChanges()");
	if (!m_htmlWidget->isReadOnly() && m_htmlWidget->isModified()) {//save
//		qDebug("save!");
		saveText( m_htmlWidget->text() );
		m_htmlWidget->setModified( false );
	}
}

/** No descriptions */
void CCommentaryPresenter::beforeKeyChange(const QString& oldKey){
//	qWarning("check for %s", key.latin1());
	if (!m_key)
		return;
		
	const QString newKey = m_key->key();
	m_key->key(oldKey);
	checkChanges();		
	m_key->key(newKey);	
}

/** Inserts the actions used by this window class into the given KAccel object. */
void CCommentaryPresenter::insertKeyboardActions(KAccel* a){	
	ASSERT(a);
	a->setConfigGroup("Commentary window");
		
	a->insertItem(i18n("Next book"), "Next book", 0);
	a->insertItem(i18n("Previous book"), "Previous book", 0);	
	
	a->insertItem(i18n("Next chapter"), "Next chapter", 0);
	a->insertItem(i18n("Previous chapter"), "Previous chapter", 0);	

	a->insertItem(i18n("Next verse"), "Next verse", 0);
	a->insertItem(i18n("Previous verse"), "Previous verse", 0);	

	a->insertItem(i18n("Toggle synchronize button"), "Synchronize", 0);
}


/** Initializes the accelerator object. */
void CCommentaryPresenter::initAccels(){
	qWarning("CCommentaryPresenter::initAccels()");
	CSwordPresenter::initAccels();
	m_accel->setConfigGroup("Commentary window");
	
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

	m_accel->insertItem(i18n("Toggle synchronize button"), "Synchronize", 0);	
	m_accel->connectItem("Synchronize", this, SLOT(toggleSynchronize()));	
		
	m_accel->readSettings();
}

/** Jumps to the next entry */
void CCommentaryPresenter::nextBook(){
	m_key->NextBook();
	m_keyChooser->setKey(m_key);	
}

/** Jumps to the previous entry. */
void CCommentaryPresenter::previousBook(){
	m_key->PreviousBook();
	m_keyChooser->setKey(m_key);
}


/** Jumps to the next entry */
void CCommentaryPresenter::nextChapter(){
	m_key->NextChapter();
	m_keyChooser->setKey(m_key);	
}

/** Jumps to the previous entry. */
void CCommentaryPresenter::previousChapter(){
	m_key->PreviousChapter();
	m_keyChooser->setKey(m_key);
}

/** Jumps to the next entry */
void CCommentaryPresenter::nextVerse(){
	m_key->NextVerse();
	m_keyChooser->setKey(m_key);	
}

/** Jumps to the previous entry. */
void CCommentaryPresenter::previousVerse(){
	m_key->PreviousVerse();
	m_keyChooser->setKey(m_key);
}

/** Toggles the synchronize button. */
void CCommentaryPresenter::toggleSynchronize(){
	presenterSync_action->setChecked( !presenterSync_action->isChecked() );
}
