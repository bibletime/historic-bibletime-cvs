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
	CSwordModuleInfo* module = dynamic_cast<CSwordModuleInfo*>(info);
	if (module &&
		(module->getType() == CSwordModuleInfo::Bible ||(module->getType() == CSwordModuleInfo::Commentary )) )
		m_info = (CSwordBibleModuleInfo*)(info);
	else {
		qWarning("CBibleKeyChooser: module is not a Bible or commentary!");
		return;
	}
	m_key = 0;	
	QHBoxLayout *layout = new QHBoxLayout(this);
	layout->setResizeMode(QLayout::Fixed);
		
	w_book = new CKeyChooserWidget(m_info->getBooks(),false,this);	
	w_book->setToolTips(TT_PRESENTER_BOOK_COMBO, TT_PRESENTER_NEXT_BOOK, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_BOOK);
	w_book->setWhatsThis(WT_PRESENTER_BOOK_COMBO, WT_PRESENTER_NEXT_BOOK, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_BOOK);
	
	w_chapter = new CKeyChooserWidget( m_info->getChapterCount(1),true,this);		
	w_chapter->setToolTips(TT_PRESENTER_CHAPTER_COMBO, TT_PRESENTER_NEXT_CHAPTER, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_CHAPTER);	
	w_chapter->setWhatsThis(WT_PRESENTER_CHAPTER_COMBO, WT_PRESENTER_NEXT_CHAPTER, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_CHAPTER);		
	
	w_verse = new CKeyChooserWidget( m_info->getVerseCount(1,1),true,this);
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

CKey *CBibleKeyChooser::getKey(){
	m_key->book(w_book->ComboBox->currentText());	
	m_key->Chapter(w_chapter->ComboBox->currentText().toInt());			
	m_key->Verse(w_verse->ComboBox->currentText().toInt());			
	return m_key;
}

void CBibleKeyChooser::setKey(CKey* key){
	qDebug("CBibleKeyChooser::setKey(CKey* key)");
	if ( !(m_key = dynamic_cast<CSwordVerseKey*>(key)) )
		return;
		
	const unsigned int bookIndex = m_info->getBookNumber( m_key->book() );
	const int chapter = m_key->Chapter();
	const int verse = m_key->Verse();

	//reset the keychooser parts only if we found a valid book
	const int count = w_book->ComboBox->count();
	const QString desiredBook = m_key->book();
	bool bookIsValid = false;
	for (int i = 0; i < count; ++i) {
		if (w_book->ComboBox->text(i) == desiredBook) {
			bookIsValid = true;
			break;
		}
	}
	
	if (bookIsValid) {	//we have a valid book
		if (w_book->ComboBox->currentText() != m_key->book()) //necessary?
			w_book->setItem( m_key->book() );
		
		w_chapter->reset(m_info->getChapterCount(bookIndex), chapter-1, false);
		w_chapter->adjustSize();
		
		w_verse->reset(m_info->getVerseCount(bookIndex, chapter), verse-1, false);
		w_verse->adjustSize();
	}
	else {
		qWarning("book %s is invalid!", m_key->book().latin1());
		w_verse->ComboBox->setCurrentItem(0);				
		m_key->Verse(w_verse->ComboBox->currentText().toInt());		

/*chapter and verse will be set to 1 if the bok is changed*/				
/*		w_chapter->ComboBox->setCurrentItem(0);
		m_key->Chapter(w_chapter->ComboBox->currentText().toInt());		
		
		w_book->ComboBox->setCurrentItem(0);
		m_key->book(w_book->ComboBox->currentText());
*/		
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
	
	if (m_key->book() != w_book->ComboBox->currentText()) {
//		m_key->Verse( 1 );
//		m_key->Chapter( 1 );		
		m_key->book( w_book->ComboBox->currentText() );/*chapter and verse are set to 1 by Sword*/
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
	
	if (m_key->Chapter() != w_chapter->ComboBox->currentText().toInt()) {	
//		m_key->Verse( 1 );		
		m_key->Chapter( w_chapter->ComboBox->currentText().toInt() );
		setKey( m_key );	
	}
	
	setUpdatesEnabled(true);		
}

void CBibleKeyChooser::verseChanged(int /*i*/){
//	qDebug("CBibleKeyChooser::verseChanged(int /*i*/)");
	if (!isUpdatesEnabled())
		return;	
	
	setUpdatesEnabled(false);			

	if (m_key->Verse() != w_verse->ComboBox->currentText().toInt()) {		
		m_key->Verse( w_verse->ComboBox->currentText().toInt() );	
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
	w_book->reset( m_info->getBooks(), w_book->ComboBox->currentItem(), false);
	setKey(m_key);
}

/** Sets te module and refreshes the combos */
void CBibleKeyChooser::setModule(CModuleInfo* module){
	if (module != m_info && dynamic_cast<CSwordBibleModuleInfo*>(module)) {
		m_info = dynamic_cast<CSwordBibleModuleInfo*>(module);
		refreshContent();
	}
}
/** called when the book combo lost the focus with reason == tab @param the new book */
void CBibleKeyChooser::bookFocusOut(int index){
//	qWarning("book focus out");
	if (!isUpdatesEnabled())
		return;
		
	setUpdatesEnabled(false);
	const char oldNormalize = m_key->AutoNormalize();
//	m_key->AutoNormalize(false);
	
	m_key->book( w_book->ComboBox->currentText() );
	const int chapterCount = m_info->getChapterCount( m_info->getBookNumber(m_key->book()));
//	if (m_key->Chapter() > chapterCount) //chapter is not available in the new book
//		m_key->Chapter( 1 );
	w_chapter->reset( chapterCount, m_key->Chapter()-1, false);
			
	const int verseCount = m_info->getVerseCount(m_info->getBookNumber(m_key->book()),m_key->Chapter());
//	if (m_key->Verse() > verseCount) //verse is not available in the new book and chapter
//		m_key->Verse( 1 );
	w_verse->reset(verseCount,m_key->Verse()-1,false);

//	m_key->AutoNormalize(oldNormalize);
	setUpdatesEnabled(true);
}

/** called when the chapter combo lost the focus with reason == tab @param the new chapter */
void CBibleKeyChooser::chapterFocusOut(int index){
//	qWarning("chapter focus out");
	const int chapter = w_chapter->ComboBox->currentText().toInt();
//	if (chapter != m_key->Chapter())
//		m_key->Verse(1);
	m_key->Chapter( chapter ); //Verse is set to 1 by Sword
	w_verse->reset(m_info->getVerseCount(chapter,1), 0, false);
}

/** called when the verse combo lost the focus with reason == tab @param the new verse */
void CBibleKeyChooser::verseFocusOut(int /*index*/){
//	qWarning("verse focus out");
	m_key->Verse( w_verse->ComboBox->currentText().toInt() );
	setKey( m_key );	
}
