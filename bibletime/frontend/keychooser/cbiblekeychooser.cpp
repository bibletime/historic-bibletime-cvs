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
#include "whatsthisdef.h"
#include "tooltipdef.h"
#include "backend/cswordversekey.h"
#include "backend/cswordbiblemoduleinfo.h"
#include "backend/cswordmoduleinfo.h"

//Qt includes
#include <qcombobox.h>
#include <qlistbox.h>
#include <qlayout.h>

//KDE includes
#include <klocale.h>

CBibleKeyChooser::CBibleKeyChooser(ListCSwordModuleInfo modules, CSwordKey *key, QWidget *parent, const char *name )
	: CKeyChooser(modules, key, parent, name), m_key(dynamic_cast<CSwordVerseKey*>(key))
{
	setModules(modules, false);
	if (!m_modules.count()) {
		qWarning("CBibleKeyChooser: module is not a Bible or commentary!");
    m_key = 0;
		return;
	}

	QHBoxLayout* layout = new QHBoxLayout(this);
	layout->setResizeMode(QLayout::Minimum);
		
	w_book = new CKeyChooserWidget(m_modules.first()->books(),false,this);	
	w_book->setToolTips(TT_PRESENTER_BOOK_COMBO, TT_PRESENTER_NEXT_BOOK, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_BOOK);
	w_book->setWhatsThis(WT_PRESENTER_BOOK_COMBO, WT_PRESENTER_NEXT_BOOK, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_BOOK);
	
//	qWarning("Biblekey chooser: has %i chapters",m_info->chapterCount(w_book->comboBox()->currentText()));
  w_chapter = new CKeyChooserWidget( m_modules.first()->chapterCount(w_book->comboBox()->currentText()),true,this);		
	w_chapter->setToolTips(TT_PRESENTER_CHAPTER_COMBO, TT_PRESENTER_NEXT_CHAPTER, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_CHAPTER);	
	w_chapter->setWhatsThis(WT_PRESENTER_CHAPTER_COMBO, WT_PRESENTER_NEXT_CHAPTER, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_CHAPTER);		
	
//	qWarning("Biblekey chooser: has %i verses",m_info->verseCount(w_book->comboBox()->currentText(),1));
  w_verse = new CKeyChooserWidget( m_modules.first()->verseCount(w_book->comboBox()->currentText(),1),true,this);
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
}

CSwordKey* const CBibleKeyChooser::key(){
	if (m_key) {
    const int chapter =  w_chapter->comboBox()->currentText().toInt();
    const int verse = w_verse->comboBox()->currentText().toInt();
		m_key->book(w_book->comboBox()->currentText());	
		m_key->Chapter(chapter < 0 ? 0 : chapter);			
		m_key->Verse(verse < 0 ? 0 : verse);			
	}
	return m_key;
}

void CBibleKeyChooser::setKey(CSwordKey* key){
	qWarning("CBibleKeyChooser::setKey(CSwordKey* key)");
 	if ( !(m_key = dynamic_cast<CSwordVerseKey*>(key)) )
		return;
		
	const unsigned int bookIndex = m_modules.first()->bookNumber( m_key->book() );
	const int chapter = m_key->Chapter();
	const int verse = m_key->Verse();

//  qWarning("setkey: %s: %i %i:%i", m_key->book().latin1(), bookIndex, chapter, verse);

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

		w_chapter->reset(m_modules.first()->chapterCount(m_key->book()), chapter-1, false);
		w_verse->reset(m_modules.first()->verseCount(m_key->book(), chapter), verse-1, false);
		
//    qWarning("key changed: %s", m_key->key().latin1());
    emit keyChanged(m_key);					
	}
	else {
  	w_chapter->comboBox()->setCurrentItem(0);				
 		m_key->Chapter(1);		

    w_verse->comboBox()->setCurrentItem(0);				
		m_key->Verse(1);		
	}
}

/**  */
void CBibleKeyChooser::chapterNextRequested(void){
	if (!isUpdatesEnabled())
		return;
	
	setUpdatesEnabled(false);	
	if (m_key)
		emit beforeKeyChange(m_key->key());	
	if (m_key->next(CSwordVerseKey::UseChapter))	
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
	
	if (m_key->previous(CSwordVerseKey::UseChapter))
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

	if (m_key->next(CSwordVerseKey::UseVerse))	
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
	
	if (m_key->previous(CSwordVerseKey::UseVerse))
		setKey(m_key);
	setUpdatesEnabled(true);	
}

void CBibleKeyChooser::bookChanged(int /*i*/){
	qWarning("CBibleKeyChooser::bookChanged(int /*i*/)");
 	Q_ASSERT(m_key);

	if (!isUpdatesEnabled())
		return;

	setUpdatesEnabled(false);	
	if (m_key)
		emit beforeKeyChange(m_key->key());
	
	if (m_key->book() != w_book->comboBox()->currentText()) {
		m_key->book( w_book->comboBox()->currentText() );
		setKey( m_key );
	}	
	setUpdatesEnabled(true);		
}

void CBibleKeyChooser::chapterChanged(int /*i*/){
	qWarning("CBibleKeyChooser::chapterChanged(int /*i*/)");
	if (!isUpdatesEnabled())
		return;
	
	setUpdatesEnabled(false);		
	if (m_key)
		emit beforeKeyChange(m_key->key());
//	if (m_key)
//		emit beforeKeyChange(m_key->key());
	
	if (m_key->Chapter() != w_chapter->comboBox()->currentText().toInt()) {	
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


	w_book->reset( m_modules.first()->books(), w_book->comboBox()->currentItem(), false);
	setKey(m_key);
}

/** Sets te module and refreshes the combos */
void CBibleKeyChooser::setModules(ListCSwordModuleInfo modules, const bool refresh){
  m_modules.clear();
  for (modules.first(); modules.current(); modules.next()) {
    if (modules.current()->type() == CSwordModuleInfo::Bible || modules.current()->type() == CSwordModuleInfo::Commentary) {
      if (CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(modules.current())) {
        m_modules.append(bible);
      }
    }
  }

  if (refresh)
    refreshContent();
}

/** called when the book combo lost the focus with reason == tab @param the new book */
void CBibleKeyChooser::bookFocusOut(int index){
	if (!isUpdatesEnabled())
		return;
		
	setUpdatesEnabled(false);
	
	m_key->book( w_book->comboBox()->currentText() );
	const int chapterCount = m_modules.first()->chapterCount( m_modules.first()->bookNumber(m_key->book()));
	w_chapter->reset( chapterCount, m_key->Chapter()-1, false);
			
	const int verseCount = m_modules.first()->verseCount(m_modules.first()->bookNumber(m_key->book()),m_key->Chapter());
	w_verse->reset(verseCount,m_key->Verse()-1,false);

	setUpdatesEnabled(true);
}

/** called when the chapter combo lost the focus with reason == tab @param the new chapter */
void CBibleKeyChooser::chapterFocusOut(int /*index*/){
	const int chapter = w_chapter->comboBox()->currentText().toInt();
	m_key->Chapter( chapter );
	w_verse->reset(m_modules.first()->verseCount(m_modules.first()->bookNumber(m_key->book()),chapter), 0, false);
}

/** called when the verse combo lost the focus with reason == tab @param the new verse */
void CBibleKeyChooser::verseFocusOut(int /*index*/){
	m_key->Verse( w_verse->comboBox()->currentText().toInt() );
	setKey( m_key );	
}

/** No descriptions */
void CBibleKeyChooser::updateKey(CSwordKey* key){
}
