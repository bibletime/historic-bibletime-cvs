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
#include "../../backend/cmoduleinfo.h"
#include "../../backend/sword_backend/cswordversekey.h"
#include "../../backend/sword_backend/cswordbiblemoduleinfo.h"
#include "../../backend/sword_backend/cswordmoduleinfo.h"
#include "../../backend/cmoduleinfo.h"

//Qt includes
#include <qcombobox.h>
#include <qlistbox.h>
#include <qlayout.h>


//KDE includes
#include <klocale.h>

CBibleKeyChooser::CBibleKeyChooser(CModuleInfo *info, CKey *key, QWidget *parent, const char *name )
	: CKeyChooser(info, key, parent, name){
	qDebug("CBibleKeyChooser::CBibleKeyChooser(CModuleInfo *info, CKey *key, QWidget *parent, const char *name )");
	ASSERT(info);
	if ((CSwordBibleModuleInfo*)info)
		m_info = (CSwordBibleModuleInfo*)info;
	else {
		qWarning("CBibleKeyChooser: module is not a Bible! HELP!");
		return;
	}
	
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setResizeMode(QLayout::Fixed);
		
	w_book = new CKeyChooserWidget(m_info->getBooks(),this);	
	w_book->setToolTips(TT_PRESENTER_BOOK_COMBO, TT_PRESENTER_NEXT_BOOK, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_BOOK);
	w_book->setWhatsThis(WT_PRESENTER_BOOK_COMBO, WT_PRESENTER_NEXT_BOOK, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_BOOK);
	
	w_chapter = new CKeyChooserWidget( m_info->getChapterCount(1), this);		
	w_chapter->setToolTips(TT_PRESENTER_CHAPTER_COMBO, TT_PRESENTER_NEXT_CHAPTER, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_CHAPTER);	
	w_chapter->setWhatsThis(WT_PRESENTER_CHAPTER_COMBO, WT_PRESENTER_NEXT_CHAPTER, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_CHAPTER);		
	
	w_verse = new CKeyChooserWidget( m_info->getVerseCount(1,1),this);
	w_verse->setToolTips(TT_PRESENTER_VERSE_COMBO, TT_PRESENTER_NEXT_VERSE, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_VERSE);
	w_verse->setWhatsThis(WT_PRESENTER_VERSE_COMBO, WT_PRESENTER_NEXT_VERSE, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_VERSE);
						
	layout->addWidget(w_book);
	layout->addWidget(w_chapter);
	layout->addWidget(w_verse);	

	/*Book connections*/
	connect(w_book,SIGNAL(changed(int)),this,SLOT(bookChanged(int)));
	connect(w_book,SIGNAL(next_requested()),this,SLOT(bookNextRequested()));
	connect(w_book,SIGNAL(prev_requested()),this,SLOT(bookPrevRequested()));
	
	/*Chapter Connections*/
	connect(w_chapter,SIGNAL(changed(int)),this,SLOT(chapterChanged(int)));
	connect(w_chapter,SIGNAL(next_requested()),this,SLOT(chapterNextRequested()));
	connect(w_chapter,SIGNAL(prev_requested()),this,SLOT(chapterPrevRequested()));
	
	/*Verse Connections*/
	connect(w_verse,SIGNAL(changed(int)),this,SLOT(verseChanged(int)));
	connect(w_verse,SIGNAL(focusOut(int)),this,SLOT(verseChanged(int)));
	connect(w_verse,SIGNAL(next_requested()),this,SLOT(verseNextRequested()));
	connect(w_verse,SIGNAL(prev_requested()),this,SLOT(versePrevRequested()));
	
	if (key)
		setKey(key);
}

CBibleKeyChooser::~CBibleKeyChooser(){
	qDebug("destructor of CBibleKeyChooser");
}

CKey *CBibleKeyChooser::getKey(){
	ASSERT(m_key);
	return m_key;
}

void CBibleKeyChooser::setKey(CKey* key){
	qDebug("CBibleKeyChooser::setKey(CKey* key)");
	ASSERT(key);	
	if (!key) {
		qWarning("Key is 0! Return now.");
		return;
	}	

	if (m_key != key) {
		if (dynamic_cast<CSwordVerseKey*>(key))
			m_key = dynamic_cast<CSwordVerseKey*>(key);
	}
	
	const QString book = m_key->getBook();
	const int chapter = m_key->Chapter();
	const int verse = m_key->Verse();

/**
	* Unnecessary!
	* This returns everytime the same list, which does cost speed and time
	* BUT: m_info caches the booklist
	*/	
#warning Possible point to optimize!
	QStringList* bookList = m_info->getBooks();
	int bookIndex = 1;
	for (QStringList::Iterator it = bookList->begin(); it != bookList->end(); it++, bookIndex++) {
		if ((*it) == book)
			break;
	}
		
	//reset the keychooser parts only if we found a valid book
	if (!book.isEmpty()) {	//we have a valid book
		w_book->ComboBox->setCurrentItem(bookIndex-1);

		w_chapter->reset(
			m_info->getChapterCount(bookIndex)
			,chapter-1
			,false);
		w_chapter->adjustSize();
	
		w_verse->reset(
			m_info->getVerseCount(bookIndex, chapter)
			,verse-1
			,false);
		w_verse->adjustSize();		
	}	
	qDebug("emit the signal keyChanged");
	emit keyChanged(m_key);
}

/**  */
void CBibleKeyChooser::bookNextRequested(void){
	ASSERT(m_key);
	if (!m_key)
		return;
	
	setUpdatesEnabled(false);		
	m_key->NextBook();	
	setKey(m_key);
	setUpdatesEnabled(true);	
}

/**  */
void CBibleKeyChooser::bookPrevRequested(void){
	ASSERT(m_key);
	if (!m_key)
		return;
	
	setUpdatesEnabled(false);		
	m_key->PreviousBook();	
	setKey(m_key);
	setUpdatesEnabled(true);	
}

/**  */
void CBibleKeyChooser::chapterNextRequested(void){
	ASSERT(m_key);
	if (!m_key)
		return;
		
	setUpdatesEnabled(false);	
	m_key->NextChapter();	
	setKey(m_key);
	setUpdatesEnabled(true);	
}

/**  */
void CBibleKeyChooser::chapterPrevRequested(void){
	ASSERT(m_key);
	if (!m_key)
		return;	
	
	setUpdatesEnabled(false);		
	m_key->PreviousChapter();	
	setKey(m_key);
	setUpdatesEnabled(true);		
}

/**  */
void CBibleKeyChooser::verseNextRequested(void){
	ASSERT(m_key);
	if (!m_key)
		return;	
	
	setUpdatesEnabled(false);
	m_key->NextVerse();	
	setKey(m_key);
	setUpdatesEnabled(true);		
}

/**  */
void CBibleKeyChooser::versePrevRequested(void){
	ASSERT(m_key);
	if (!m_key)
		return;			
	
	setUpdatesEnabled(false);	
	m_key->PreviousVerse();	
	setKey(m_key);
	setUpdatesEnabled(true);	
}

void CBibleKeyChooser::bookChanged(int i){
	qDebug("CBibleKeyChooser::bookChanged(int i)");
	if (!isUpdatesEnabled()) {
		qDebug("already updating");
		return;
	}
	ASSERT(m_key);
	if (!m_key)
		return;
	
	setUpdatesEnabled(false);			
	
	m_key->Verse( 1 );
	m_key->Chapter( 1 );	
	m_key->setBook( w_book->ComboBox->currentText() );
	setKey( m_key );	
	
	setUpdatesEnabled(true);		
}

void CBibleKeyChooser::chapterChanged(int i){
	qDebug("CBibleKeyChooser::chapterChanged(int i)");
	if (!isUpdatesEnabled()) {
		qDebug("already updating");
		return;
	}
	ASSERT(m_key);
	if (!m_key)
		return;
	
	setUpdatesEnabled(false);		
	
	m_key->Verse( 1 );		
	m_key->Chapter( w_chapter->ComboBox->currentText().toInt() );
	setKey( m_key );	
	
	setUpdatesEnabled(true);		
}

void CBibleKeyChooser::verseChanged(int i){
	qDebug("CBibleKeyChooser::verseChanged(int i)");
	if (!isUpdatesEnabled()) {
		qDebug("already updating");
		return;
	}
	ASSERT(m_key);
	if (!m_key)
		return;
	
	setUpdatesEnabled(false);		
	m_key->Verse( w_verse->ComboBox->currentText().toInt() );
	setKey( m_key );
	setUpdatesEnabled(true);		
}

/** Reimplementation */
QSize CBibleKeyChooser::sizeHint(){
	const QSize s( w_book->sizeHint().width() + w_chapter->sizeHint().width() + w_verse->sizeHint().width(), w_book->sizeHint().height());
	return s;
}

/** Reimplementation. */
void CBibleKeyChooser::refreshContent() {
	const int currentBookIndex = w_book->ComboBox->currentItem();
	w_book->reset( m_info->getBooks(), currentBookIndex, true);
}

/** Sets te module and refreshes the combos */
void CBibleKeyChooser::setModule(CModuleInfo* module){
	if (module != m_info && (CSwordBibleModuleInfo*)module) {
		m_info = (CSwordBibleModuleInfo*)module;
		refreshContent();
	}
}
