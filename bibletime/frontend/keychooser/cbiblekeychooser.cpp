/***************************************************************************
                          cbiblekeychooser.cpp  -  description
                             -------------------
    begin                : Wed Sep 20 2000
    copyright            : (C) 2000 by The BibleTime team
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
#include "cbiblekeychooser.h"
#include "ckeychooserwidget.h"
#include "cfx_btn.h"
#include "../../whatsthisdef.h"
#include "../../tooltipdef.h"
#include "../../backend/cswordversekey.h"
#include "../../backend/cswordbiblemoduleinfo.h"
#include "../../backend/cswordmoduleinfo.h"

//Qt includes
#include <qcombobox.h>
#include <qlistbox.h>
#include <qlayout.h>

//KDE includes
#include <klocale.h>

CBibleKeyChooser::CBibleKeyChooser(CSwordModuleInfo *module, CSwordKey *key, QWidget *parent, const char *name )
	: CKeyChooser(module, key, parent, name), m_info(0), m_key(0), w_book(0), w_chapter(0), w_verse(0)
{

	if (module && (module->type() == CSwordModuleInfo::Bible || (module->type() == CSwordModuleInfo::Commentary )) )
		m_info = dynamic_cast<CSwordBibleModuleInfo*>(module);
	else {
		qWarning("CBibleKeyChooser: module is not a Bible or commentary!");
		return;
	}
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setResizeMode(QLayout::Fixed);
		
	w_book = new CKeyChooserWidget(m_info->books(),false,this);	
	w_book->setToolTips(TT_PRESENTER_BOOK_COMBO, TT_PRESENTER_NEXT_BOOK, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_BOOK);
	w_book->setWhatsThis(WT_PRESENTER_BOOK_COMBO, WT_PRESENTER_NEXT_BOOK, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_BOOK);
	
	w_chapter = new CKeyChooserWidget( m_info->chapterCount(1),true,this);		
	w_chapter->setToolTips(TT_PRESENTER_CHAPTER_COMBO, TT_PRESENTER_NEXT_CHAPTER, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_CHAPTER);	
	w_chapter->setWhatsThis(WT_PRESENTER_CHAPTER_COMBO, WT_PRESENTER_NEXT_CHAPTER, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_CHAPTER);		
	
	w_verse = new CKeyChooserWidget( m_info->verseCount(1,1),true,this);
	w_verse->setToolTips(TT_PRESENTER_VERSE_COMBO, TT_PRESENTER_NEXT_VERSE, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_VERSE);
	w_verse->setWhatsThis(WT_PRESENTER_VERSE_COMBO, WT_PRESENTER_NEXT_VERSE, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_VERSE);
						
	QWidget::setTabOrder(w_book, w_chapter);
	QWidget::setTabOrder(w_chapter, w_verse);
	QWidget::setTabOrder(w_verse, 0);	
		
	layout->addWidget(w_book);
	layout->addWidget(w_chapter);
	layout->addWidget(w_verse);	

	/*Book connections*/
	connect(w_book,SIGNAL(changed(int))       ,SLOT(bookChanged(int)));
	connect(w_book,SIGNAL(focusOut(int))      ,SLOT(bookFocusOut(int)));	
	
	/*Chapter Connections*/
	connect(w_chapter,SIGNAL(changed(int))    ,SLOT(chapterChanged(int)));
	connect(w_chapter,SIGNAL(next_requested()),SLOT(chapterNextRequested()));
	connect(w_chapter,SIGNAL(prev_requested()),SLOT(chapterPrevRequested()));
	connect(w_chapter,SIGNAL(focusOut(int))   ,SLOT(chapterFocusOut(int)));
	
	
	/*Verse Connections*/
	connect(w_verse,SIGNAL(changed(int))      ,SLOT(verseChanged(int)));
	connect(w_verse,SIGNAL(next_requested())  ,SLOT(verseNextRequested()));
	connect(w_verse,SIGNAL(prev_requested())  ,SLOT(versePrevRequested()));
	connect(w_verse,SIGNAL(focusOut(int))     ,SLOT(verseFocusOut(int)));	
	
	if (key)
		setKey(key);
}

CSwordKey *CBibleKeyChooser::key(){
	m_key->book(w_book->comboBox()->currentText());	
	m_key->Chapter(w_chapter->comboBox()->currentText().toInt());			
	m_key->Verse(w_verse->comboBox()->currentText().toInt());			
	
	return m_key;
}

void CBibleKeyChooser::setKey(CSwordKey* key){
//	qDebug("CBibleKeyChooser::setKey(CKey* key)");
	if ( !(m_key = dynamic_cast<CSwordVerseKey*>(key)) )
		return;
		
	const unsigned int bookIndex = m_info->bookNumber( m_key->book() );
	const int chapter = m_key->Chapter();
	const int verse = m_key->Verse();

	//reset the keychooser parts only if we found a valid book
	const int count = w_book->comboBox()->count();
	const QString desiredBook = m_key->book();
	bool bookIsValid = false;
	for (int i = 0; i < count; ++i) {
		if (w_book->comboBox()->text(i) == desiredBook) {
			bookIsValid = true;
			break;
		}
	}
	
	if (bookIsValid) {	//we have a valid book
		if (w_book->comboBox()->currentText() != m_key->book()) //necessary?
			w_book->setItem( m_key->book() );
		
		w_chapter->reset(m_info->chapterCount(bookIndex), chapter-1, false);
		w_chapter->adjustSize();
		
		w_verse->reset(m_info->verseCount(bookIndex, chapter), verse-1, false);
		w_verse->adjustSize();
	}
	else {
		qWarning("book %s is invalid!", m_key->book().latin1());
		w_verse->comboBox()->setCurrentItem(0);				
		m_key->Verse(w_verse->comboBox()->currentText().toInt());		
	}
	emit keyChanged(m_key);			
}

/**  */
void CBibleKeyChooser::chapterNextRequested(void){
	if (!isUpdatesEnabled())
		return;
	
	setUpdatesEnabled(false);	
	if (m_key)
		emit beforeKeyChange(m_key->key());	
	if (m_key->NextChapter())	
		setKey(m_key);
	setUpdatesEnabled(true);	
}

/**  */
void CBibleKeyChooser::chapterPrevRequested(void){
	if (!isUpdatesEnabled())
		return;
		
	setUpdatesEnabled(false);		
	if (m_key)
		emit beforeKeyChange(m_key->key());
	
	if (m_key->PreviousChapter())
		setKey(m_key);
	setUpdatesEnabled(true);
}

/**  */
void CBibleKeyChooser::verseNextRequested(void){
	if (!isUpdatesEnabled())
		return;
	
	setUpdatesEnabled(false);
	if (m_key)
		emit beforeKeyChange(m_key->key());

	if (m_key->NextVerse())	
		setKey(m_key);
	setUpdatesEnabled(true);		
}

/**  */
void CBibleKeyChooser::versePrevRequested(void){
	if (!isUpdatesEnabled())
		return;
	
	setUpdatesEnabled(false);	
	if (m_key)
		emit beforeKeyChange(m_key->key());
	
	if (m_key->PreviousVerse())
		setKey(m_key);
	setUpdatesEnabled(true);	
}

void CBibleKeyChooser::bookChanged(int /*i*/){
//	qDebug("CBibleKeyChooser::bookChanged(int /*i*/)");
	if (!isUpdatesEnabled())
		return;
	setUpdatesEnabled(false);	
	if (m_key)
		emit beforeKeyChange(m_key->key());
	
	if (m_key->book() != w_book->comboBox()->currentText()) {
//		m_key->Verse( 1 );
//		m_key->Chapter( 1 );		
		m_key->book( w_book->comboBox()->currentText() );/*chapter and verse are set to 1 by Sword*/
		setKey( m_key );
	}	
	setUpdatesEnabled(true);		
}

void CBibleKeyChooser::chapterChanged(int /*i*/){
//	qDebug("CBibleKeyChooser::chapterChanged(int /*i*/)");
	if (!isUpdatesEnabled())
		return;
	
	setUpdatesEnabled(false);		
	if (m_key)
		emit beforeKeyChange(m_key->key());
	if (m_key)
		emit beforeKeyChange(m_key->key());
	
	if (m_key->Chapter() != w_chapter->comboBox()->currentText().toInt()) {	
//		m_key->Verse( 1 );		
		m_key->Chapter( w_chapter->comboBox()->currentText().toInt() );
		setKey( m_key );	
	}
	
	setUpdatesEnabled(true);		
}

void CBibleKeyChooser::verseChanged(int /*i*/){
//	qDebug("CBibleKeyChooser::verseChanged(int /*i*/)");
	if (!isUpdatesEnabled())
		return;	
	
	setUpdatesEnabled(false);			

	if (m_key->Verse() != w_verse->comboBox()->currentText().toInt()) {		
		m_key->Verse( w_verse->comboBox()->currentText().toInt() );	
		setKey( m_key );
	}
	
	setUpdatesEnabled(true);	
}

/** Reimplementation */
QSize CBibleKeyChooser::sizeHint(){
  return QSize( w_book->sizeHint().width() + w_chapter->sizeHint().width() + w_verse->sizeHint().width(), w_book->sizeHint().height());
}

/** Reimplementation. */
void CBibleKeyChooser::refreshContent() {
	w_book->reset( m_info->books(), w_book->comboBox()->currentItem(), false);
	setKey(m_key);
}

/** Sets te module and refreshes the combos */
void CBibleKeyChooser::setModule(CSwordModuleInfo* module){
	if (module != m_info && module) {
		m_info = dynamic_cast<CSwordBibleModuleInfo*>(module);
		refreshContent();
	}
}

/** called when the book combo lost the focus with reason == tab @param the new book */
void CBibleKeyChooser::bookFocusOut(int index){
	if (!isUpdatesEnabled())
		return;
		
	setUpdatesEnabled(false);
	
	m_key->book( w_book->comboBox()->currentText() );
	const int chapterCount = m_info->chapterCount( m_info->bookNumber(m_key->book()));
	w_chapter->reset( chapterCount, m_key->Chapter()-1, false);
			
	const int verseCount = m_info->verseCount(m_info->bookNumber(m_key->book()),m_key->Chapter());
	w_verse->reset(verseCount,m_key->Verse()-1,false);

	setUpdatesEnabled(true);
}

/** called when the chapter combo lost the focus with reason == tab @param the new chapter */
void CBibleKeyChooser::chapterFocusOut(int /*index*/){
	const int chapter = w_chapter->comboBox()->currentText().toInt();
	m_key->Chapter( chapter );
	w_verse->reset(m_info->verseCount(m_info->bookNumber(m_key->book()),chapter), 0, false);
}

/** called when the verse combo lost the focus with reason == tab @param the new verse */
void CBibleKeyChooser::verseFocusOut(int /*index*/){
	m_key->Verse( w_verse->comboBox()->currentText().toInt() );
	setKey( m_key );	
}
