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
#include "cscrollbutton.h"

#include "backend/cswordversekey.h"
#include "backend/cswordbiblemoduleinfo.h"
#include "backend/cswordmoduleinfo.h"

#include "util/cresmgr.h"

//Qt includes
#include <qcombobox.h>
#include <qlistbox.h>
#include <qlayout.h>
#include <qpushbutton.h>

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
  layout->setDirection( QBoxLayout::LeftToRight );
		
	w_book = new CKeyChooserWidget(m_modules.first()->books(),false,this);	
	w_book->setToolTips(
    CResMgr::displaywindows::bibleWindow::bookList::tooltip,
    CResMgr::displaywindows::bibleWindow::nextBook::tooltip,
    CResMgr::displaywindows::general::scrollButton::tooltip,
    CResMgr::displaywindows::bibleWindow::previousBook::tooltip
  );
	w_book->setWhatsThis(
    CResMgr::displaywindows::bibleWindow::bookList::whatsthis,
    CResMgr::displaywindows::bibleWindow::nextBook::whatsthis,
    CResMgr::displaywindows::general::scrollButton::whatsthis,
    CResMgr::displaywindows::bibleWindow::previousBook::whatsthis
  );
	
  w_chapter = new CKeyChooserWidget( m_modules.first()->chapterCount(w_book->comboBox()->currentText()),true,this);		
	w_chapter->setToolTips(
    CResMgr::displaywindows::bibleWindow::chapterList::tooltip,
    CResMgr::displaywindows::bibleWindow::nextChapter::tooltip,
    CResMgr::displaywindows::general::scrollButton::tooltip,
    CResMgr::displaywindows::bibleWindow::previousChapter::tooltip
  );
	w_chapter->setWhatsThis(
    CResMgr::displaywindows::bibleWindow::chapterList::whatsthis,
    CResMgr::displaywindows::bibleWindow::nextChapter::whatsthis,
    CResMgr::displaywindows::general::scrollButton::whatsthis,
    CResMgr::displaywindows::bibleWindow::previousChapter::whatsthis
  );
	
  w_verse = new CKeyChooserWidget( m_modules.first()->verseCount(w_book->comboBox()->currentText(),1),true,this);
	w_verse->setToolTips(
    CResMgr::displaywindows::bibleWindow::verseList::tooltip,
    CResMgr::displaywindows::bibleWindow::nextVerse::tooltip,
    CResMgr::displaywindows::general::scrollButton::tooltip,
    CResMgr::displaywindows::bibleWindow::previousVerse::tooltip
  );
	w_verse->setWhatsThis(
    CResMgr::displaywindows::bibleWindow::verseList::whatsthis,
    CResMgr::displaywindows::bibleWindow::nextVerse::whatsthis,
    CResMgr::displaywindows::general::scrollButton::whatsthis,
    CResMgr::displaywindows::bibleWindow::previousVerse::whatsthis
  );

	QWidget::setTabOrder(w_book, w_chapter);
	QWidget::setTabOrder(w_chapter, w_verse);
	QWidget::setTabOrder(w_verse, 0);
		
	layout->addWidget(w_book,0);
	layout->addWidget(w_chapter,0);
	layout->addWidget(w_verse,0);

	/* Book connections */
	connect(w_book,SIGNAL(changed(int))       ,SLOT(bookChanged(int)));
	connect(w_book,SIGNAL(focusOut(int))      ,SLOT(bookFocusOut(int)));	
	
	/* Chapter Connections */
	connect(w_chapter,SIGNAL(changed(int))    ,SLOT(chapterChanged(int)));
	connect(w_chapter,SIGNAL(next_requested()),SLOT(chapterNextRequested()));
	connect(w_chapter,SIGNAL(prev_requested()),SLOT(chapterPrevRequested()));
	connect(w_chapter,SIGNAL(focusOut(int))   ,SLOT(chapterFocusOut(int)));


	/* Verse Connections */
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
	if ( !(m_key = dynamic_cast<CSwordVerseKey*>(key)) )
		return;

	emit (beforeKeyChange(m_key->key())); //required to make direct setKey calls work from the outside

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
		if (w_book->comboBox()->currentText() != m_key->book()) { //necessary?
			w_book->setItem( m_key->book() );
		}

		w_chapter->reset(m_modules.first()->chapterCount(m_key->book()), chapter-1, false);
		w_verse->reset(m_modules.first()->verseCount(m_key->book(), chapter), verse-1, false);

    emit keyChanged(m_key);
	}
	else {
  	w_chapter->comboBox()->setCurrentItem(0);
 		m_key->Chapter(1);

    w_verse->comboBox()->setCurrentItem(0);
		m_key->Verse(1);
	}
}

void CBibleKeyChooser::chapterNextRequested(void){
	if (!isUpdatesEnabled())
		return;

	setUpdatesEnabled(false);
	if (m_key) {
		emit beforeKeyChange(m_key->key());
	}
	if (m_key->next(CSwordVerseKey::UseChapter)) {
		setKey(m_key);
	}
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
//	qWarning("CBibleKeyChooser::bookChanged(int /*i*/)");
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
//	qWarning("CBibleKeyChooser::chapterChanged(int /*i*/)");
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
  if (!m_modules.count() || !m_modules.first())
    return;

  w_book->reset( m_modules.first()->books(), w_book->comboBox()->currentItem(), false);
	setKey(m_key);
}

/** Sets te module and refreshes the combos */
void CBibleKeyChooser::setModules(const ListCSwordModuleInfo& modules, const bool refresh){
  m_modules.clear();

//   for (modules.first(); modules.current(); modules.next()) {
	ListCSwordModuleInfo::const_iterator end_it = modules.end();
	for (ListCSwordModuleInfo::const_iterator it(modules.begin()); it != end_it; ++it) {
    if ((*it)->type() == CSwordModuleInfo::Bible || (*it)->type() == CSwordModuleInfo::Commentary) {
      if (CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(*it)) {
        m_modules.append(bible);
      }
    }
  }

  if (refresh) {
    refreshContent();
	}
}

/** called when the book combo lost the focus with reason == tab @param the new book */
void CBibleKeyChooser::bookFocusOut(int /*index*/){
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
void CBibleKeyChooser::updateKey(CSwordKey* /*key*/){
}

/** No descriptions */
void CBibleKeyChooser::adjustFont(){
//#warning implement a suitable solution. must be based on locales, not on module->isUnicode. Maybe just do nothing? =)
}
