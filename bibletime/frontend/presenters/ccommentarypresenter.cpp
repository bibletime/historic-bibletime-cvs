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

#include "frontend/ctoolclass.h"
#include "frontend/cexportmanager.h"
#include "frontend/cdisplaywidget.h"
#include "frontend/cbtconfig.h"

#include "frontend/keychooser/ckeychooser.h"
#include "resource.h"
#include "whatsthisdef.h"

#include "backend/cswordversekey.h"
#include "backend/chtmlchapterdisplay.h"
#include "backend/creferencemanager.h"
#include "backend/cswordbackend.h"
#include "backend/cswordcommentarymoduleinfo.h"


//Qt includes
#include <qclipboard.h>

//KDE includes
#include <kmessagebox.h>
#include <ktoolbar.h>
#include <kaction.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kaccel.h>
#include <kapplication.h>
#include <kpopupmenu.h>

CCommentaryPresenter::CCommentaryPresenter(ListCSwordModuleInfo useModules, QWidget *parent, const char *name )
	: CSwordPresenter(useModules, parent,name),
	m_key( new CSwordVerseKey(m_moduleList.first()) ), m_editToolBar(0)
{
	CSwordCommentaryModuleInfo* commentary = dynamic_cast<CSwordCommentaryModuleInfo*>(m_moduleList.first());
	if (commentary) {
		if (commentary->hasTestament(CSwordBibleModuleInfo::OldTestament))
			m_key->key("Genesis 1:1");
		else
			m_key->key("Matthew 1:1");
	}
	
	initView();	
	show();		
	initConnections();		
	
	setInitialized();
}

CCommentaryPresenter::~CCommentaryPresenter(){
	delete m_key;
}

/** Initializes our view. */
void CCommentaryPresenter::initView(){
	m_mainToolBar = new KToolBar(this);
	m_keyChooser = CKeyChooser::createInstance(m_moduleList.first(), m_key, m_mainToolBar);
	m_mainToolBar->insertWidget(0,m_keyChooser->sizeHint().width(),m_keyChooser);	
	addToolBar(m_mainToolBar);			

	m_moduleChooserBar = new CModuleChooserBar(m_moduleList, CSwordModuleInfo::Commentary, this );
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
	

  m_displayWidget = new CDisplayWidget(this);
//	m_htmlWidget = new CHTMLWidget(true, this);
//	ASSERT(m_htmlWidget);
	
	//setup popup menu
	m_popup = new KPopupMenu(this);
	m_popup->insertTitle(i18n("Commentary window"));
	
	m_copyPopup = new KPopupMenu(m_popup);
	m_copyPopup->insertItem(i18n("Entry"), this, SLOT(copyEntry()),0,ID_PRESENTER_COPY_ONLY_KEY);
	m_copyPopup->insertItem(i18n("Text of entry"), this, SLOT(copyEntryText()),0,ID_PRESENTER_COPY_KEY_TEXT);	
	m_copyPopup->insertItem(i18n("Entry with text"), this, SLOT(copyEntryAndText()),0,ID_PRESENTER_COPY_KEY);
	m_copyPopup->insertSeparator();
	m_copyPopup->insertItem(i18n("Selected text"), m_displayWidget, SLOT(copySelection()),0,ID_PRESENTER_COPY_SELECTED);
	
	m_printPopup = new KPopupMenu(m_popup);
	m_printPopup->insertItem(i18n("Entry with text"), this, SLOT(printHighlightedVerse()),0,ID_PRESENTER_PRINT_KEY);

	m_savePopup = new KPopupMenu(m_popup);	
	m_savePopup->insertItem(i18n("Entry as plain text"), m_displayWidget, SLOT(saveAsPlain()),0,ID_PRESENTER_SAVE_CHAPTER);
	m_savePopup->insertItem(i18n("Entry as HTML"), m_displayWidget, SLOT(saveAsHTML()),0,ID_PRESENTER_SAVE_CHAPTER_HTML);	

	m_popup->insertItem(i18n("Select all"), m_displayWidget, SLOT(selectAll()),0, ID_PRESENTER_SELECT_ALL);
  m_popup->insertItem(i18n("Lookup selected text in lexicon"), m_lexiconPopup, ID_PRESENTER_LOOKUP);	
	m_popup->insertSeparator();	
	m_popup->insertItem(SmallIcon(ICON_EDIT_COPY),i18n("Copy..."), 	m_copyPopup, ID_PRESENTER_COPY_POPUP);	
	m_popup->insertItem(SmallIcon(ICON_FILE_PRINT), i18n("Add to printing queue..."), m_printPopup, ID_PRESENTER_PRINT_POPUP);	
	m_popup->insertItem(SmallIcon(ICON_FILE_SAVE), i18n("Save..."), 	m_savePopup,ID_PRESENTER_SAVE_POPUP);		

  m_displayWidget->view()->installPopup(CDisplayWidgetView::Normal, m_popup);
	m_displayWidget->view()->installPopup(CDisplayWidgetView::Anchor, m_popup );
	
  setCentralWidget(m_displayWidget->view());
	setIcon( COMMENTARY_ICON_SMALL );
}

/** Initializes the connections */
void CCommentaryPresenter::initConnections(){
	connect(m_displayWidget, SIGNAL(referenceClicked(const QString&, const QString&)),
		this, SLOT(lookup(const QString&, const QString&))); 	 	
	connect(m_displayWidget, SIGNAL(referenceDropped(const QString&, const QString&)),
		this, SLOT(referenceDropped(const QString&, const QString&)));

//	connect(m_htmlWidget, SIGNAL(sigDeleteDocument()),
//		this, SLOT(deleteText())); 	 	
//	connect(m_htmlWidget, SIGNAL(sigSaveDocument(const QString)),
//		this, SLOT(saveText(const QString))); 	 	 	
//	connect(m_htmlWidget, SIGNAL(insertReference(const QString&)),
//		this, SLOT(insertReference(const QString&))); 	 	 			
 	
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
//		m_htmlWidget->setModules(m_moduleList);	
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

	backend()->setFilterOptions( m_moduleOptions );
	backend()->setDisplayOptions( m_displayOptions );

  m_moduleList.first()->module()->SetKey(*vKey);

	if (m_moduleList.first()->getDisplay()) {	//do we have a display object?
//		if (m_htmlWidget->isReadOnly())	 {
			if (m_moduleList.count()>1)
				m_moduleList.first()->getDisplay()->Display( &m_moduleList );
			else
				m_moduleList.first()->getDisplay()->Display( m_moduleList.first() );
			m_displayWidget->setText( m_moduleList.first()->getDisplay()->getHTML() );
//		}
//		else
//			m_displayWidget->setText( QString::fromUtf8( m_moduleList.first()->module()->getRawEntry() ) );
	}	
	if (m_key != vKey)
		m_key->key(vKey->key());
		
	m_displayWidget->gotoAnchor( QString::number(vKey->Verse()) );
//	m_htmlWidget->setModified(false);
	setUpdatesEnabled(true);
	
	setCaption( windowCaption() );	
}

/** No descriptions */
void CCommentaryPresenter::popupAboutToShow(){
	const bool hasSelection = m_displayWidget->hasSelection();
  m_popup->setItemEnabled(ID_PRESENTER_LOOKUP, hasSelection);
	m_copyPopup->setItemEnabled(ID_PRESENTER_COPY_SELECTED, hasSelection);	
}

/** Saves the given text in the module. */
void CCommentaryPresenter::saveText(const QString text){
	m_moduleList.first()->module()->SetKey(m_key);
	if (!text.isEmpty())
		*(m_moduleList.first()->module()) << (const char*)text.utf8();
	else
		m_moduleList.first()->module()->deleteEntry();		
//	m_htmlWidget->setModified( false );		
}

/** Deletes the displayed and edited text. */
void CCommentaryPresenter::deleteText(){
	m_moduleList.first()->module()->deleteEntry();
//	m_htmlWidget->clear();
}

void CCommentaryPresenter::editComment(){
//	if (!m_htmlWidget->isReadOnly()) {
//		switch (KMessageBox::warningYesNo( this, i18n("The text was not saved to the module. Save the changes now?")) ) {
//			case KMessageBox::Yes:
//			{
//	   		saveText( m_htmlWidget->text() );
//	   		m_htmlWidget->setModified( false );
//				break;
//			}
//	   	default: //no
//	   		break;
//	  }
//	}
//	
//	m_htmlWidget->setReadOnly( !m_htmlWidget->isReadOnly() );	
//	if (!m_htmlWidget->isReadOnly() && !m_editToolBar) {
//		m_editToolBar = new KToolBar(this);
//		addToolBar(m_editToolBar);		
//		m_htmlWidget->createEditToolbar( m_editToolBar );
//	}	
//	if (!m_htmlWidget->isReadOnly() && !m_editToolBar)
//		return;
//		
//	if (!m_htmlWidget->isReadOnly())
//		m_editToolBar->show();
//	else
//		m_editToolBar->hide();
//			
//	lookup( m_key );	
//	m_htmlWidget->setFocus();
}

/** Reimplementation. */
void CCommentaryPresenter::lookup(const QString& module, const QString& key){
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

/** No descriptions */
void CCommentaryPresenter::refresh( ){

//	CSwordPresenter::refresh();	//refreshes the display settings button

	m_key->setLocale((const char*)backend()->booknameLanguage().local8Bit());
	m_keyChooser->refreshContent();
	lookup(m_key);
//	m_htmlWidget->refresh();		
}

/** Printes the verse the user has chosen. */
void CCommentaryPresenter::printHighlightedVerse(){
//	CSwordVerseKey key(m_moduleList.first());	//this key is deleted by the printem
//	key->key(m_key->key());	
//	CSwordVerseKey key(*m_key);
	CExportManager::printKey(m_moduleList.first(), m_key->key());
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
//	if (m_htmlWidget->isReadOnly())
//		return;
//	CSwordVerseKey vk(m_moduleList.first());
//	vk.key(reference);
//	m_htmlWidget->insert(vk.strippedText());
}

/** Copies the highlighted text into clipboard. */
void CCommentaryPresenter::copyEntry(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_displayWidget->activeURLNode();
	CReferenceManager::Type type;
	CReferenceManager::decodeHyperlink(currentAnchor, module, key, type);	
	CSwordModuleInfo* m = backend()->findModuleByName(module);		
	
	CSwordVerseKey vKey(m);
	vKey.key(key);
		
	QClipboard *cb = KApplication::clipboard();
	cb->setText(vKey.key());
}

/** Copies the highlighted text into clipboard. */
void CCommentaryPresenter::copyEntryText(){
	QString key = QString::null;
	QString module = QString::null;	
	QString currentAnchor = m_displayWidget->activeURLNode();
	CReferenceManager::Type type;	
	CReferenceManager::decodeHyperlink(currentAnchor, module, key, type);	
	CSwordModuleInfo* m = backend()->findModuleByName(module);		
	
	CSwordVerseKey vKey(m);
	vKey.key(key);
	
	QClipboard *cb = KApplication::clipboard();
	cb->setText(vKey.strippedText());
}

/** Copies the highlighted text into clipboard. */
void CCommentaryPresenter::copyEntryAndText(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_displayWidget->activeURLNode();
	CReferenceManager::Type type;	
	CReferenceManager::decodeHyperlink(currentAnchor, module, key, type);	
	CSwordModuleInfo* m = backend()->findModuleByName(module);		
	
	CSwordVerseKey vKey(m);
	vKey.key(key);
	
	const QString text = QString::fromLatin1("%1\n%2").arg(vKey.key()).arg(vKey.strippedText());
	QClipboard *cb = KApplication::clipboard();
	cb->setText(text);
}

//print functions
void CCommentaryPresenter::printEntry(){
	QString key = QString::null;
	QString module = QString::null;
	QString currentAnchor = m_displayWidget->activeURLNode();
	CReferenceManager::Type type;	
	CReferenceManager::decodeHyperlink(currentAnchor, module, key, type);	
	
	CSwordModuleInfo* m = backend()->findModuleByName(module);			
	CSwordVerseKey vKey(m);
	CExportManager::printKey(m, vKey.key(key));
}

/** Checks for changes and saves the text. */
void CCommentaryPresenter::checkChanges(){
//	qDebug("void CCommentaryPresenter::checkChanges()");
//	if (!m_htmlWidget->isReadOnly() && m_htmlWidget->isModified()) {//save
//		saveText( m_htmlWidget->text() );
//		m_htmlWidget->setModified( false );
//	}
}

/** No descriptions */
void CCommentaryPresenter::beforeKeyChange(const QString& oldKey){
//	if (!m_htmlWidget->isReadOnly()) {
//		switch (KMessageBox::warningYesNo( this, i18n("The text was not saved to the module. Save the changes now?")) ) {
//			case KMessageBox::Yes:
//			{
//				const QString newKey = m_key->key();
//				m_key->key(oldKey);
//	   		saveText( m_htmlWidget->text() );
//				m_key->key(newKey);		   		
//				break;
//			}
//	   	default: //no
//	   		break;
//	  }
//	}
//	m_htmlWidget->setModified( false );	
}

/** Inserts the actions used by this window class into the given KAccel object. */
void CCommentaryPresenter::insertKeyboardActions(KAccel* a){
	a->insert("Next book",        i18n("Next book"),         "",   IDK_PRESENTER_NEXT_BOOK,0,"");
	a->insert("Previous book",    i18n("Previous book"),     "",   IDK_PRESENTER_PREVIOUS_BOOK, 0, "");
	a->insert("Next chapter",     i18n("Next chapter"),      "",   IDK_PRESENTER_NEXT_CHAPTER, 0, "");
	a->insert("Previous chapter", i18n("Previous chapter"),  "",   IDK_PRESENTER_PREVIOUS_CHAPTER, 0, "");	
	a->insert("Next verse",       i18n("Next verse"),        "",   IDK_PRESENTER_NEXT_VERSE, 0, "");
	a->insert("Previous verse",   i18n("Previous verse"),    "",   IDK_PRESENTER_PREVIOUS_VERSE, 0, "");	
	a->insert("Toggle synchronize button", i18n("Synchronize"), "", 0, 0, "");
}


/** Initializes the accelerator object. */
void CCommentaryPresenter::initAccels(){
	CBTConfig::setupAccel( CBTConfig::commentaryWindow, m_accel );
	insertKeyboardActions(m_accel);
   CSwordPresenter::initAccels();
	m_accel->readSettings();
	
	m_accel->setSlot("Next book",       this, SLOT(nextBook()));	
	m_accel->setSlot("Previous book",   this, SLOT(previousBook()));
	m_accel->setSlot("Next chapter",    this, SLOT(nextChapter()));		
	m_accel->setSlot("Previous chapter",this, SLOT(previousChapter()));	
	m_accel->setSlot("Next verse",      this, SLOT(nextVerse()));		
	m_accel->setSlot("Previous verse",  this, SLOT(previousVerse()));	
	m_accel->setSlot("Synchronize",     this, SLOT(toggleSynchronize()));	
	
}

/** Jumps to the next entry */
void CCommentaryPresenter::nextBook(){
	if (m_key->next(CSwordVerseKey::UseBook))
		m_keyChooser->setKey(m_key);	
}

/** Jumps to the previous entry. */
void CCommentaryPresenter::previousBook(){
	if (m_key->previous(CSwordVerseKey::UseBook))
		m_keyChooser->setKey(m_key);
}


/** Jumps to the next entry */
void CCommentaryPresenter::nextChapter(){
	if ( m_key->next(CSwordVerseKey::UseChapter) )
		m_keyChooser->setKey(m_key);	
}

/** Jumps to the previous entry. */
void CCommentaryPresenter::previousChapter(){
	if (m_key->previous(CSwordVerseKey::UseChapter))
		m_keyChooser->setKey(m_key);
}

/** Jumps to the next entry */
void CCommentaryPresenter::nextVerse(){
	if ( m_key->next(CSwordVerseKey::UseVerse) )
		m_keyChooser->setKey(m_key);	
}

/** Jumps to the previous entry. */
void CCommentaryPresenter::previousVerse(){
	if (m_key->previous(CSwordVerseKey::UseVerse))
		m_keyChooser->setKey(m_key);
}

/** Toggles the synchronize button. */
void CCommentaryPresenter::toggleSynchronize(){
	presenterSync_action->setChecked( !presenterSync_action->isChecked() );
}

/** Stores some Bible window dpecific setttings in the profile. */
void CCommentaryPresenter::storeSettings( CProfileWindow* settings ){
	CSwordPresenter::storeSettings(settings);
	settings->setWindowSettings( presenterSync_action->isChecked() );	
}

/** Applies some Bible window specific setttings. */
void CCommentaryPresenter::applySettings( CProfileWindow* settings ){
	CSwordPresenter::applySettings(settings);
	presenterSync_action->setChecked(settings->windowSettings() == 1);
}

/** Saves settings */
bool CCommentaryPresenter::queryClose(){
	//save the text
//	if (!m_htmlWidget->isReadOnly() && m_htmlWidget->isModified()) {	
//		switch (KMessageBox::warningYesNoCancel( this, i18n("Save changes to module?")) ) {
//			case KMessageBox::Yes:
//			{
//	    	//save
//	   		saveText( m_htmlWidget->text() );
//	   		m_htmlWidget->setModified( false );
//	     	return true;
//			}
//	   	case KMessageBox::No :
//	     	return true;
//	   	default: // cancel
//	     	return false;	
//		}
//	}
//	return true;
}
