/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2006 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/


//BibleTime includes
#include "ckeyreferencewidget.h"
#include "cscrollerwidgetset.h"

//BibleTime frontend includes
#include "frontend/cbtconfig.h"
#include "backend/cswordversekey.h"
#include "util/cresmgr.h"

//Qt includes
#include <klineedit.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qevent.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <kcompletion.h>
#include <kglobalsettings.h>
#include <kcompletionbox.h>
#include <klocale.h>
#include <kpushbutton.h>
#include <kguiitem.h>

/* Override the completion box for our references */
CKeyReferenceCompletion::CKeyReferenceCompletion(CSwordBibleModuleInfo *mod) : KCompletion()
{
	m_key = new CSwordVerseKey(mod);
	m_module = mod;
}

QString CKeyReferenceCompletion::makeCompletion(const QString &text) {
	if(!text.isEmpty() && m_key->key(text)) {
		// XXX: key() does not check bounds properly if we only have eg the NT.
		return m_key->key();
	}

	return QString::null;
}

//**********************************************************************************/
/* To get popup working we have to rework KLineEdit too */
CKeyReferenceLineEdit::CKeyReferenceLineEdit(QWidget *parent, const char *name) : KLineEdit(parent,name) {
}

void CKeyReferenceLineEdit::makeCompletion(const QString &text) {
	KCompletion *comp = compObj();
	KGlobalSettings::Completion mode = completionMode();

	if ( !comp || mode == KGlobalSettings::CompletionNone )
		return;  // No completion object...

	QString match = comp->makeCompletion( text );
	if ( mode == KGlobalSettings::CompletionPopup ||
		mode == KGlobalSettings::CompletionPopupAuto )
	{
		if ( match.isNull() )
		{
			KCompletionBox *compbox = completionBox();
			compbox->hide();
			compbox->clear();
		} else {
			QStringList t;
			t.append(match);
			setCompletedItems(t);
		}
	} else {
		KLineEdit::makeCompletion(text);
	}
}

//**********************************************************************************/

CKeyReferenceWidget::CKeyReferenceWidget( CSwordBibleModuleInfo *mod, CSwordVerseKey *key, QWidget *parent, const char *name) : QWidget(parent,name) {

	updatelock = false;
	m_module = mod;

	setFocusPolicy(QWidget::WheelFocus);

	// Erase button
	KGuiItem erase_picture;
	erase_picture.setIconName("locationbar_erase");
	KPushButton *clearRef = new KPushButton(this);
	clearRef->setGuiItem(erase_picture);
	connect(clearRef, SIGNAL(clicked( ) ), SLOT(slotClearRef( )));

	m_bookScroller = new CScrollerWidgetSet(this);

	m_textbox = new CKeyReferenceLineEdit( this );
	setKey(key);	// The order of these two functions is important.
	setModule();

	m_chapterScroller = new CScrollerWidgetSet(this);
	m_verseScroller = new CScrollerWidgetSet(this);

	m_mainLayout = new QHBoxLayout( this );
	m_mainLayout->addWidget(clearRef);
	m_mainLayout->addWidget(m_bookScroller);
	m_mainLayout->addWidget(m_textbox);
	m_mainLayout->addWidget(m_chapterScroller);
	m_mainLayout->addWidget(m_verseScroller);

	setTabOrder(m_textbox, 0);

        m_bookScroller->setToolTips(
                CResMgr::displaywindows::bibleWindow::nextBook::tooltip,
                CResMgr::displaywindows::general::scrollButton::tooltip,
                CResMgr::displaywindows::bibleWindow::previousBook::tooltip
        );
        m_chapterScroller->setToolTips(
                CResMgr::displaywindows::bibleWindow::nextChapter::tooltip,
                CResMgr::displaywindows::general::scrollButton::tooltip,
                CResMgr::displaywindows::bibleWindow::previousChapter::tooltip
        );
        m_verseScroller->setToolTips(
                CResMgr::displaywindows::bibleWindow::nextVerse::tooltip,
                CResMgr::displaywindows::general::scrollButton::tooltip,
                CResMgr::displaywindows::bibleWindow::previousVerse::tooltip
        );


	// signals and slots connections

	connect(m_bookScroller, SIGNAL(change(int)), SLOT(slotBookChange(int)));
	connect(m_bookScroller, SIGNAL(scroller_pressed()), SLOT(slotUpdateLock()));
	connect(m_bookScroller, SIGNAL(scroller_released()), SLOT(slotUpdateUnlock()));
	connect(m_textbox, SIGNAL(returnPressed()), SLOT(slotReturnPressed()));
	connect(m_chapterScroller, SIGNAL(change(int)), SLOT(slotChapterChange(int)));
	connect(m_chapterScroller, SIGNAL(scroller_pressed()), SLOT(slotUpdateLock()));
	connect(m_chapterScroller, SIGNAL(scroller_released()), SLOT(slotUpdateUnlock()));
	connect(m_verseScroller, SIGNAL(change(int)), SLOT(slotVerseChange(int)));
	connect(m_verseScroller, SIGNAL(scroller_pressed()), SLOT(slotUpdateLock()));
	connect(m_verseScroller, SIGNAL(scroller_released()), SLOT(slotUpdateUnlock()));
}

void CKeyReferenceWidget::setModule(CSwordBibleModuleInfo *m) {
	if(m)
		m_module = m;

	delete m_textbox->completionObject();
	CKeyReferenceCompletion *comp = new CKeyReferenceCompletion(m_module);
	m_textbox->setCompletionObject(comp);
	m_textbox->setCompletionMode(KGlobalSettings::CompletionPopup);
}

void CKeyReferenceWidget::slotClearRef( ) {
	m_textbox->setText("");
	m_textbox->setFocus();
}

void CKeyReferenceWidget::updateText() {
	m_textbox->setText(m_key->key());
}

bool CKeyReferenceWidget::setKey(CSwordVerseKey *key) {
	m_key = key;
	updateText();
	
	return true;
}

KLineEdit* CKeyReferenceWidget::textbox() {
	return m_textbox;
}

void CKeyReferenceWidget::slotReturnPressed()
{
	QString text = m_textbox->text();
	if(m_key->key(text)) {
		// XXX: a basic error fix but see validator above there must be a better way...
		m_key->module(m_module);

		updateText();
		emit changed(m_key);
	} else {
		// Something was invalid. Pop up an error box?
	}
}


/* Handlers for the various scroller widgetsets. Do we really want a verse scroller? */
void CKeyReferenceWidget::slotUpdateLock() {
	updatelock = true;
	oldKey = m_key->key();
}

void CKeyReferenceWidget::slotUpdateUnlock() {
	updatelock = false;
	if(oldKey != m_key->key())
		emit changed(m_key);
}

void CKeyReferenceWidget::slotBookChange(int n) {
	QStringList *books = m_module->books();
	const int old_book = (int)books->findIndex(m_key->book());
	int new_book = old_book + n;
	if(new_book < 0) new_book = 0;
	if(new_book >= (int)books->count()) new_book = books->count() - 1;

	if(old_book != new_book) {
		m_key->book(*(books->at(new_book)));		// we have to set by text rather than reference
		updateText();
		if(!updatelock)
			emit changed(m_key);
	}
}

void CKeyReferenceWidget::slotChapterChange(int n) {
	const int chs = (int)m_module->chapterCount(m_key->book());
	const int old_chapter = m_key->Chapter();
	int new_chapter = old_chapter + n;

	/* XXX: Do we actually want these limits? Could scroll across books if we didnt have them */
	if(new_chapter < 1) new_chapter = 1;
	if(new_chapter > chs) new_chapter = chs;

	if(old_chapter != new_chapter) {
		m_key->Chapter(new_chapter);
		updateText();
		if(!updatelock)
			emit changed(m_key);	
	}
}

void CKeyReferenceWidget::slotVerseChange(int n) {
	const int vrs = (int)m_module->verseCount(m_key->book(), m_key->Chapter());
	const int old_verse =  m_key->Verse();
	int new_verse = old_verse + n;

	/* XXX: Do we actually want these limits? Could scroll across chapters if we didnt have them */
	if(new_verse < 1) new_verse = 1;
	if(new_verse > vrs) new_verse = vrs;

	if(old_verse != new_verse) {
		m_key->Verse(new_verse);
		updateText();
		if(!updatelock)
			emit changed(m_key);
	}
}
