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
	if (info && ( ((CSwordModuleInfo*)info)->getType()==CSwordModuleInfo::Bible
	            ||((CSwordModuleInfo*)info)->getType()==CSwordModuleInfo::Commentary ) )
		m_info = (CSwordBibleModuleInfo*)info;
	else {
		qWarning("CBibleKeyChooser: module is not a Bible or commentary!");
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
	connect(w_book,SIGNAL(changed(int))       ,SLOT(bookChanged(int)));
	connect(w_book,SIGNAL(next_requested())   ,SLOT(bookNextRequested()));
	connect(w_book,SIGNAL(prev_requested())   ,SLOT(bookPrevRequested()));
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

CKey *CBibleKeyChooser::getKey(){
	return m_key;
}

void CBibleKeyChooser::setKey(CKey* key){
	if (dynamic_cast<CSwordVerseKey*>(key))
		m_key = dynamic_cast<CSwordVerseKey*>(key);
	
	const unsigned int bookIndex = m_info->getBookNumber( m_key->getBook() );
	const int chapter = m_key->Chapter();
	const int verse = m_key->Verse();

	//reset the keychooser parts only if we found a valid book
	if (bookIndex > 0) {	//we have a valid book
		w_book->ComboBox->setCurrentItem(bookIndex-1);
		w_chapter->reset(m_info->getChapterCount(bookIndex), chapter-1, false);
		w_chapter->adjustSize();	
		w_verse->reset(m_info->getVerseCount(bookIndex, chapter), verse-1, false);
		w_verse->adjustSize();
	}
	emit keyChanged(m_key);			
}

/**  */
void CBibleKeyChooser::bookNextRequested(void){
	setUpdatesEnabled(false);		
	m_key->NextBook();	
	setKey(m_key);
	setUpdatesEnabled(true);	
}

/**  */
void CBibleKeyChooser::bookPrevRequested(void){
	setUpdatesEnabled(false);		
	m_key->PreviousBook();	
	setKey(m_key);
	setUpdatesEnabled(true);	
}

/**  */
void CBibleKeyChooser::chapterNextRequested(void){
	setUpdatesEnabled(false);	
	m_key->NextChapter();	
	setKey(m_key);
	setUpdatesEnabled(true);	
}

/**  */
void CBibleKeyChooser::chapterPrevRequested(void){
	setUpdatesEnabled(false);		
	m_key->PreviousChapter();	
	setKey(m_key);
	setUpdatesEnabled(true);		
}

/**  */
void CBibleKeyChooser::verseNextRequested(void){
	setUpdatesEnabled(false);
	m_key->NextVerse();	
	setKey(m_key);
	setUpdatesEnabled(true);		
}

/**  */
void CBibleKeyChooser::versePrevRequested(void){
	setUpdatesEnabled(false);	
	m_key->PreviousVerse();	
	setKey(m_key);
	setUpdatesEnabled(true);	
}

void CBibleKeyChooser::bookChanged(int i){
	if (!isUpdatesEnabled())
		return;

	setUpdatesEnabled(false);			
	
	m_key->Verse( 1 );
	m_key->Chapter( 1 );	
	m_key->setBook( w_book->ComboBox->currentText() );
	setKey( m_key );	
	
	setUpdatesEnabled(true);		
}

void CBibleKeyChooser::chapterChanged(int i){
	if (!isUpdatesEnabled())
		return;
	
	setUpdatesEnabled(false);		
	
	m_key->Verse( 1 );		
	m_key->Chapter( w_chapter->ComboBox->currentText().toInt() );
	setKey( m_key );	
	
	setUpdatesEnabled(true);		
}

void CBibleKeyChooser::verseChanged(int i){
	if (!isUpdatesEnabled())
		return;
	
	setUpdatesEnabled(false);
			
	m_key->Verse( w_verse->ComboBox->currentText().toInt() );
	setKey( m_key );
	setUpdatesEnabled(true);		
}

/** Reimplementation */
QSize CBibleKeyChooser::sizeHint(){
  return QSize( w_book->sizeHint().width() + w_chapter->sizeHint().width() + w_verse->sizeHint().width(), w_book->sizeHint().height());
}

/** Reimplementation. */
void CBibleKeyChooser::refreshContent() {
	w_book->reset( m_info->getBooks(), w_book->ComboBox->currentItem(), true);
}

/** Sets te module and refreshes the combos */
void CBibleKeyChooser::setModule(CModuleInfo* module){
	if (module != m_info && (CSwordBibleModuleInfo*)module) {
		m_info = (CSwordBibleModuleInfo*)module;
		refreshContent();
	}
}
/** called when the book combo lost the focus
with reason == tab
@param the new book */
void CBibleKeyChooser::bookFocusOut(int index){
 	int chapter = m_key->Chapter();
	int verse = m_key->Verse();
	
	const int newchapters = m_info->getChapterCount(index+1);
	if (newchapters < chapter)
	  chapter = 1;

	const int newverses = m_info->getVerseCount(index+1,chapter);
	if (newverses < verse)
	  verse = 1;
	
	w_chapter->reset(newchapters, chapter-1,false);
	w_chapter->adjustSize();
	
	w_verse->reset(newverses, verse-1, false);
	w_verse->adjustSize();		
}
/** called when the chapter combo lost the focus
with reason == tab
@param the new chapter */
void CBibleKeyChooser::chapterFocusOut(int index){
  int book = m_info->getBookNumber( w_book->ComboBox->currentText() );

	int verse = w_verse->ComboBox->currentText().toInt();
	const int newverses = m_info->getVerseCount(book,index+1);
	if (newverses < verse)
	  verse = 1;
	
	w_verse->reset(newverses,verse-1,false);
	w_verse->adjustSize();
}
/** called when the verse combo lost the focus
with reason == tab
@param the new verse */
void CBibleKeyChooser::verseFocusOut(int index){
  const QString book = w_book->ComboBox->currentText();
 	const int chapter = w_chapter->ComboBox->currentText().toInt();

	m_key->setBook( book );	
	m_key->Chapter( chapter );	
	m_key->Verse( index + 1);
	setKey( m_key );	
}
