/***************************************************************************
                          ccommentarypresenter.cpp  -  description
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

#include "ccommentarypresenter.h"
#include "../keychooser/ckeychooser.h"
#include "../../ressource.h"
#include "../actions.h"
#include "../cmdiarea.h"
#include "../chtmlwidget.h"
#include "../ctoolclass.h"
#include "../../ressource.h"
#include "../../backend/ckey.h"
#include "../../backend/sword_backend/cswordversekey.h"
#include "../../backend/sword_backend/chtmlentrydisplay.h"
#include "../../backend/sword_backend/chtmlchapterdisplay.h"
#include "../../whatsthisdef.h"
#include "../../printing/cprinter.h"
#include "../../printing/cprintitem.h"

//QT includes
#include <qwidget.h>
#include <qstrlist.h>
#include <qtoolbar.h>
#include <qmultilineedit.h>
#include <qpixmap.h>
#include <qcolor.h>

//KDE includes
#include <kstddirs.h>
#include <kstdaction.h>
#include <kiconloader.h>
#include <ktoolbar.h>
#include <kcombobox.h>
#include <klineedit.h>
#include <ktoolbarbutton.h>
#include <klocale.h>
#include <kaction.h>
#include <kmessagebox.h>
#include <kmenubar.h>

//Sword include
#include <swmodule.h>

CCommentaryPresenter::CCommentaryPresenter( CImportantClasses*	importantClasses, QWidget* parent, const char* name, CModuleInfo* module  )
	: CModulePresenter( importantClasses,parent, name, module ) {	
	qDebug("Constructor of CCommentaryPresenter");
	
	isEditMode = false;	
	m_editToolbar = 0;
	m_mainToolbar = 0;
	m_key = 0;
	m_keyChooser = 0;
	html_widget = 0;
	
	ASSERT(m_swordModule);
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
		
	initActions();		
	initView();	
	initConnections();
	m_keyChooser->setKey(m_key);
}

CCommentaryPresenter::~CCommentaryPresenter(){
	qDebug("CCommentaryPresenter::~CCommentaryPresenter()");	
	if (m_key)
		delete m_key;
}

/** Initializes KAction objects */
void CCommentaryPresenter::initActions(){
	qDebug("CCommentaryPresenter::initActions()");
	presenterEdit_action =  new KToggleAction(i18n("Edit"), "pencil",
															IDK_PRESENTER_EDIT, this,	SLOT(editComment()), actionCollection(), "editComment_action");
	presenterEdit_action->setEnabled( m_swordModule->module()->isWritable() );
	presenterEdit_action->setWhatsThis( WT_PRESENTER_EDIT );
}

/** Initialize the view */
void CCommentaryPresenter::initView(){	
	qDebug("CCommentaryPresenter::initView()");
	//plug actions manually	
	html_widget = new CHTMLWidget(this);
	html_widget->setReadOnly( true );	

	m_popup = new KPopupMenu(this);		
	m_popup->insertTitle(i18n("Commentary presenter"));
	m_popup->insertItem(i18n("Save verse as HTML..."), html_widget, SLOT(slotSaveAsHTML()), 0,ID_PRESENTER_SAVE_AS_HTML);	
	m_popup->insertItem(i18n("Save verse as plain text..."), html_widget, SLOT(slotSaveAsText()),0, ID_PRESENTER_SAVE_AS_TEXT);
	m_popup->insertSeparator();
	m_popup->insertItem(i18n("Select all"), html_widget, SLOT(slotSelectAll()), 0,ID_PRESENTER_SELECT_ALL);
	m_popup->insertSeparator();	
	m_popup->insertItem(i18n("Copy selected text"), html_widget, SLOT(copy()), 0,ID_PRESENTER_COPY_SELECTED);	
	m_popup->insertItem(i18n("Copy verse into clipboard"), html_widget, SLOT(slotCopyAll()), 0,ID_PRESENTER_COPY_ALL);
	m_popup->insertSeparator();	
	m_popup->insertItem(i18n("Add verse to print queue"), this, SLOT(slotPrintCurrentVerse()), 0, ID_PRESENTER_PRINT_VERSE);	
	
	html_widget->installPopup(m_popup);	
		
	
	setIcon( COMMENTARY_ICON_SMALL );

	m_mainToolbar = new KToolBar(this);
	addToolBar( m_mainToolbar );
	m_mainToolbar->show();	
	
	m_keyChooser = CKeyChooser::createInstance( m_swordModule, m_key, m_mainToolbar );
	presenterEdit_action->plug( m_mainToolbar );

	setCentralWidget( html_widget);		
}

/**  */
void CCommentaryPresenter::initConnections() {
  qDebug("CCommentaryPresenter::initConnections()");
  ASSERT(m_swordModule);
  ASSERT(html_widget);	
	connect(html_widget, SIGNAL(referenceClicked(const QString&)),
		this, SLOT(slotReferenceClicked(const QString&))); 	 	
	connect(html_widget, SIGNAL(sigDeleteDocument()),
		this, SLOT(deleteText())); 	 	
	connect(html_widget, SIGNAL(sigSaveDocument(const QString)),
		this, SLOT(saveText(const QString))); 	 	
 	
 	connect( m_keyChooser, SIGNAL(keyChanged(CKey*)),
 		this, SLOT(lookup(CKey*)));		
	connect(m_popup, SIGNAL(aboutToShow()),
		SLOT(slotPopupAboutToShow()));
}

void CCommentaryPresenter::editComment(){
	qDebug("CCommentaryPresenter::editComment()");
	isEditMode = presenterEdit_action->isChecked();	
	html_widget->setReadOnly( !isEditMode );
	if (isEditMode && !m_editToolbar) {
		m_editToolbar = new KToolBar(this);
		addToolBar(m_editToolbar, QMainWindow::Top, true);
		
		html_widget->createEditToolbar( m_editToolbar );
	}	
	ASSERT(m_editToolbar);
	if (isEditMode && !m_editToolbar)
		return;
		
	if (isEditMode)
		m_editToolbar->show();
	else
		m_editToolbar->hide();
	
	lookup( m_key );	
	html_widget->setFocus();
}

/**  */
void CCommentaryPresenter::lookup(CKey* key){
	qDebug("CCommentaryPresenter::lookup");	
	ASSERT(m_swordModule);	
	ASSERT(key);
	QString text;

	CSwordVerseKey* vKey = 0;
	if ((CSwordVerseKey*)key)
		vKey = (CSwordVerseKey*)key;
	else {
		qDebug("CCommentaryPresenter: Wrong type of Key!");
		return;
	}
	ASSERT(vKey);
	m_swordModule->module()->SetKey(*vKey);
	
	if (!isEditMode && m_swordModule->getDisplay()) {	//do we have a display object?
		m_swordModule->getDisplay()->setIncludeHeader(true);		
		m_swordModule->getDisplay()->Display( m_swordModule );	//render text
		text = m_swordModule->getDisplay()->getHTML();
		if (text.isEmpty())
			text = "<HTML></HTML>";
	}
	else if (isEditMode && m_swordModule->getDisplay()) {
		m_swordModule->getDisplay()->setIncludeHeader(false);
		m_swordModule->module()->SetKey(*m_key);
		text = m_swordModule->module()->getRawEntry();
	}
	else
		qWarning("CBiblePresenter: We don't have a valid display object!");
	
	html_widget->setText( text );	
	setCaption( caption() );	
}

/**  */
void CCommentaryPresenter::refresh(const int useFeatures){
	bool doLookup = false;
	if (useFeatures & CModulePresenter::language) {
		m_key->setLocale((const char*)m_important->swordBackend->getCurrentBooknameLanguage().local8Bit());
		m_keyChooser->refreshContent();		
		doLookup = true;
	}
	if ( (useFeatures & CModulePresenter::textColor)
			|| (useFeatures & CModulePresenter::verseNumberColor)
			|| (useFeatures & CModulePresenter::highlightedVerseColor)
			|| (useFeatures & CModulePresenter::font)) {
			
		html_widget->refresh();
		doLookup = true;
	}
	
	if (doLookup)
		m_keyChooser->setKey(m_key);
}

/** Reimplementation */
void CCommentaryPresenter::cut(){
//	edit->cut();		
}

/** Reimplementation */
void CCommentaryPresenter::paste(){
//	edit->paste();
}

/** Reimplementation */
void CCommentaryPresenter::copy(){
// 	if (isEditMode)
//// 		edit->copy();
// 	else
 		html_widget->copy();
}

/**  */
void CCommentaryPresenter::slotReferenceClicked( const QString& url){
	//if the link is verse reference set the verse to the current one
	qDebug("referenceClicked");
	
	if (!url.isEmpty()) {
		m_key->setKey(url);
		m_keyChooser->setKey( m_key );
	}
}

/** Returns the caption for this presenter window. */
QString CCommentaryPresenter::caption() const {
	return QString("%1: %2").arg(m_swordModule->getDescription()).arg(QString::fromLocal8Bit((const char*)*m_key));
}

/** Saves the document in the module. */
void CCommentaryPresenter::saveText(const QString text){
	qDebug("CCommentaryPresenter::saveText()");
	m_swordModule->module()->SetKey(*m_key);
	(*(m_swordModule->module())) << (const char*)text.local8Bit();
	
	lookup(m_key);	//update current key so the saved text will be displayed
}

/** Deletes the current entry in the module. */
void CCommentaryPresenter::deleteText(){
	qDebug("CCommentaryPresenter::deleteText()");
	m_swordModule->module()->deleteEntry();
	
	lookup(m_key);
}

/** This function appens the displayed verse to the printer queue. */
void CCommentaryPresenter::slotPrintCurrentVerse(){
	debug("CCommentaryPresenter::printHighlightedVerse()");
	CPrintItem*	printItem = new CPrintItem();
	
	CSwordVerseKey *key = new CSwordVerseKey(m_swordModule);	//this key is deleted by the printem
	key->setKey(QString::fromLocal8Bit((const char*)*m_key));	
	
	printItem->setModule(m_swordModule);
	printItem->setStartKey(key);
	printItem->setStopKey(key);
	
	m_important->printer->addItemToQueue( printItem );
}

/** Is caklled before the popup menu is shown */
void CCommentaryPresenter::slotPopupAboutToShow(){
	m_popup->setItemEnabled(ID_PRESENTER_COPY_SELECTED, html_widget->hasSelectedText());		
}
