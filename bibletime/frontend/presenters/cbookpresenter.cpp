/***************************************************************************
                          cbookpresenter.cpp  -  description
                             -------------------
    begin                : Thu Jan 24 2002
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


//BibleTime includes
#include "cbookpresenter.h"
#include "cmodulechooserbar.h"

#include "frontend/keychooser/ckeychooser.h"
#include "frontend/keychooser/cbooktreechooser.h"
#include "frontend/ctoolclass.h"
#include "frontend/cdisplaywidget.h"
#include "frontend/cexportmanager.h"
#include "frontend/cbtconfig.h"

#include "backend/cswordtreekey.h"
#include "backend/chtmlentrydisplay.h"
#include "backend/cswordbackend.h"

#include "cdisplaysettingsbutton.h"
#include "resource.h"
#include "tooltipdef.h"
#include "whatsthisdef.h"

//Qt includes
#include <qsplitter.h>

//KDE includes
#include <kaccel.h>
#include <klocale.h>
#include <kaction.h>
#include <kpopupmenu.h>

CBookPresenter::CBookPresenter(ListCSwordModuleInfo useModules, QWidget *parent, const char *name )
	: CSwordPresenter(useModules,parent,name)
{
	m_key = dynamic_cast<CSwordTreeKey*>( CSwordKey::createInstance(useModules.first()) );
	m_key->root();
	
	initView();
	show();
	initConnections();
	
	setInitialized();
}

CBookPresenter::~CBookPresenter(){
	delete m_key;
	m_key = 0;
}

/** Initializes the interface of this presenter. */
void CBookPresenter::initView(){
	m_mainToolBar = new KToolBar(this);
	m_keyChooser = CKeyChooser::createInstance(m_moduleList.first(), m_key, m_mainToolBar);	
	m_mainToolBar->insertWidget(0,m_keyChooser->sizeHint().width(),m_keyChooser);	
	
	m_treeAction = new KToggleAction(i18n("Toggle tree..."), ICON_VIEW_BOOKTREE,
    IDK_PRESENTER_TOGGLE_TREE, this, SLOT(treeToggled()), actionCollection(), "treeToggle_action");
//	m_treeAction->setWhatsThis( WT_PRESENTER_SYNC );
	m_treeAction->plug(m_mainToolBar);

	addToolBar(m_mainToolBar);			
	
	m_moduleChooserBar = new CModuleChooserBar(m_moduleList, CSwordModuleInfo::GenericBook, this );
	m_moduleChooserBar->setButtonLimit(1);
	addToolBar(m_moduleChooserBar);
	

	QSplitter* splitter = new QSplitter(this);	
	m_treeChooser = new CBookTreeChooser(m_moduleList.first(), m_key, splitter);
	m_treeAction->setChecked(false);
	m_treeChooser->hide();
	
	splitter->setResizeMode(m_treeChooser, QSplitter::FollowSizeHint);
	
	m_displayWidget = new CDisplayWidget(splitter);
		
	m_popup = new KPopupMenu(this);
	m_popup->insertTitle(i18n("Book window"));

	m_copyPopup = new KPopupMenu(m_popup);
	m_copyPopup->insertItem(i18n("Verse"), this, SLOT(copyVerse()),0,ID_PRESENTER_COPY_ONLY_KEY);
	m_copyPopup->insertItem(i18n("Text of verse"), this, SLOT(copyVerseText()),0,ID_PRESENTER_COPY_KEY_TEXT);	
	m_copyPopup->insertItem(i18n("Verse with text"), this, SLOT(copyVerseAndText()),0,ID_PRESENTER_COPY_KEY);
	m_copyPopup->insertItem(i18n("Chapter"), m_displayWidget, SLOT(copyAll()),0,ID_PRESENTER_COPY_CHAPTER);
	m_copyPopup->insertSeparator();

	m_copyPopup->insertItem(i18n("Selected text"), m_displayWidget, SLOT(copySelection()),0,ID_PRESENTER_COPY_SELECTED);

	m_printPopup = new KPopupMenu(m_popup);
	m_printPopup->insertItem(i18n("Entry"), this, SLOT(printEntry()),0,ID_PRESENTER_PRINT_KEY);

	m_savePopup = new KPopupMenu(m_popup);	
	m_savePopup->insertItem(i18n("Verse with text"), this, SLOT(saveVerseAndText()),0,ID_PRESENTER_SAVE_KEY);
	m_savePopup->insertItem(i18n("Chapter as plain text"), m_displayWidget, SLOT(saveAsPlain()),0,ID_PRESENTER_SAVE_CHAPTER);
	m_savePopup->insertItem(i18n("Entry as HTML"), m_displayWidget, SLOT(saveAsHTML()),0,ID_PRESENTER_SAVE_CHAPTER_HTML);	

	m_popup->insertItem(i18n("Select all"), m_displayWidget, SLOT(selectAll()),0, ID_PRESENTER_SELECT_ALL);
  m_popup->insertItem(i18n("Lookup selected text in lexicon"), m_lexiconPopup, ID_PRESENTER_LOOKUP);	
	m_popup->insertSeparator();	
	m_popup->insertItem(SmallIcon(ICON_EDIT_COPY),i18n("Copy..."), m_copyPopup, ID_PRESENTER_COPY_POPUP);	
	m_popup->insertItem(SmallIcon(ICON_FILE_PRINT), i18n("Add to printing queue..."), m_printPopup, ID_PRESENTER_PRINT_POPUP);	
	m_popup->insertItem(SmallIcon(ICON_FILE_SAVE), i18n("Save..."), m_savePopup,ID_PRESENTER_SAVE_POPUP);		

	m_displayWidget->view()->installPopup(CDisplayWidgetView::Normal, m_popup);
	m_displayWidget->view()->installPopup(CDisplayWidgetView::Anchor, m_popup );
//	m_htmlWidget->setModules(m_moduleList);
		
	setCentralWidget(splitter);
}

/** Initializes the Signal / Slot connections */
void CBookPresenter::initConnections(){
	connect(m_displayWidget, SIGNAL(referenceClicked(const QString&, const QString&)),
		this, SLOT(lookup(const QString&, const QString&))); 	
	connect(m_displayWidget, SIGNAL(referenceDropped(const QString&, const QString&)),
		this, SLOT(referenceDropped(const QString&, const QString&)));

 	connect(m_keyChooser, SIGNAL(keyChanged(CSwordKey*)),
 		this, SLOT(lookup(CSwordKey*)));
 	connect(m_keyChooser, SIGNAL(keyChanged(CSwordKey*)),
 		m_treeChooser, SLOT(updateKey(CSwordKey*)));
 	
 	connect(m_treeChooser, SIGNAL(keyChanged(CSwordKey*)),
 		this, SLOT(lookup(CSwordKey*)));
 	connect(m_treeChooser, SIGNAL(keyChanged(CSwordKey*)),
 		m_keyChooser, SLOT(updateKey(CSwordKey*)));
		
//	connect(m_popup, SIGNAL(aboutToShow()),
//		SLOT(popupAboutToShow()));
	connect(m_moduleChooserBar, SIGNAL( sigChanged() ),
		SLOT(modulesChanged() ));
}

void CBookPresenter::modulesChanged(){
  m_moduleList = m_moduleChooserBar->getModuleList();
  if (!m_moduleList.count())
  	close();
  else {
	  m_key->module(m_moduleList.first());
	  m_keyChooser->setModule(m_moduleList.first());	
	  m_treeChooser->setModule(m_moduleList.first());		
//		m_htmlWidget->setModules(m_moduleList);
	  	
	  lookup(m_key);
	}
}

void CBookPresenter::lookup(CSwordKey* key) {
	setUpdatesEnabled(false);	
	CSwordTreeKey* treeKey = dynamic_cast<CSwordTreeKey*>(key);	
	CSwordModuleInfo* m = m_moduleList.first();
	m->module()->SetKey(treeKey);
	
		
	if (m->getDisplay()) {
		m->getDisplay()->Display( m );
		m_displayWidget->setText(m->getDisplay()->getHTML());
	}	
	if (m_key != treeKey) {
		m_key->key(treeKey->key());
	}
	setUpdatesEnabled(true);
	setCaption( windowCaption() );
	
	m_displayWidget->gotoAnchor( QString::fromLocal8Bit(treeKey->getLocalName()) );	
}

/** No descriptions */
void CBookPresenter::lookup( const QString& module, const QString& key){
	CSwordModuleInfo* m = backend()->findModuleByName(module);
	if (m && m_moduleList.containsRef(m)) {
		if (!key.isEmpty())
			m_key->key(key);
		else
			m_key->root();
		m_keyChooser->setKey(m_key); //the key chooser does send an update signal	
	}
	else
		emit lookupInModule(module, key);	
}

/** Initializes keyboard accelerators. */
void CBookPresenter::initAccels(){
	CBTConfig::setupAccel( CBTConfig::bookWindow, m_accel );		
	insertKeyboardActions(m_accel);
   m_accel->readSettings();
	CSwordPresenter::initAccels();
	
	m_treeAction->setShortcut(m_accel->shortcut("Toggle tree"));	
	m_accel->setSlot("Toggle tree", this, SLOT(treeToggled()));
}

void CBookPresenter::insertKeyboardActions(KAccel* a){
	a->insert( "Toggle tree", i18n("Toggle tree"), i18n("Toggle the tree on the left side of the book window on or off."), IDK_PRESENTER_TOGGLE_TREE, 0 , "");
}

/** No descriptions */
void CBookPresenter::printEntry(){
	CExportManager::printKey( m_moduleList.first(), m_key->key() );
}

/** Is called when the tree should be toggled. */
void CBookPresenter::treeToggled(){
	if (m_treeAction->isChecked())
		m_treeChooser->show();
	else
		m_treeChooser->hide();
}

void CBookPresenter::applySettings( CProfileWindow* settings ){
	CSwordPresenter::applySettings(settings);
	
	if (settings->windowSettings()) {
		m_treeAction->setChecked(true);		
		m_treeChooser->show();		
	}
	else {
		m_treeAction->setChecked(false);
		m_treeChooser->hide();
	}
}

void CBookPresenter::storeSettings( CProfileWindow* settings ){
	CSwordPresenter::storeSettings(settings);	
	settings->setWindowSettings( m_treeAction->isChecked() );
}
