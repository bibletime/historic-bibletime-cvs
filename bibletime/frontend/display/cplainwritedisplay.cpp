/********* Read the file LICENSE for license details. *********/

#include "cplainwritedisplay.h"

#include "frontend/cdragdropmgr.h"
#include "frontend/displaywindow/cdisplaywindow.h"
#include "frontend/displaywindow/cwritewindow.h"

#include "util/scoped_resource.h"

//Qt includes

//KDE includes
#include <kaction.h>
#include <klocale.h>

CPlainWriteDisplay::CPlainWriteDisplay(CWriteWindow* parentWindow, QWidget* parent) : QTextEdit(parentWindow ? parentWindow : parent), CWriteDisplay(parentWindow) {
	//  qWarning("constructor of CPlainWriteDisplay");
	setTextFormat(Qt::PlainText);
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);

	connect(this, SIGNAL(textChanged()),
			connectionsProxy(), SLOT(emitTextChanged()));
}

CPlainWriteDisplay::~CPlainWriteDisplay() {}

/** Reimplementation. */
void CPlainWriteDisplay::selectAll() {
	QTextEdit::selectAll(true);
}

void CPlainWriteDisplay::setText( const QString& newText ) {
	QTextEdit::setText(newText);
};

const bool CPlainWriteDisplay::hasSelection() {
	return hasSelectedText();
};

QWidget* CPlainWriteDisplay::view() {
	qWarning("CPlainWriteDisplay::view()");
	return this;
};

const QString CPlainWriteDisplay::text( const CDisplay::TextType /*format*/, const CDisplay::TextPart /*part*/) {
	return QString::null;
};

void CPlainWriteDisplay::print( const CDisplay::TextPart ) {}
;

/** Sets the current status of the edit widget. */
void CPlainWriteDisplay::setModified( const bool modified ) {
	QTextEdit::setModified(modified);
}

/** Reimplementation. */
const bool CPlainWriteDisplay::isModified() const {
	return QTextEdit::isModified();
}


/** Returns the text of this edit widget. */
const QString CPlainWriteDisplay::plainText() {
	return QTextEdit::text();
}

/** Reimplementation from QTextEdit. Provides an popup menu for the given position. */
QPopupMenu* CPlainWriteDisplay::createPopupMenu( const QPoint& /*pos*/ ) {
	return installedPopup();
}

/** Reimplementation from QTextEdit. Provides an popup menu for the given position. */
QPopupMenu* CPlainWriteDisplay::createPopupMenu( ) {
	return installedPopup();
}

/** Creates the necessary action objects and puts them on the toolbar. */
void CPlainWriteDisplay::setupToolbar(KToolBar* /*bar*/, KActionCollection* /*actionCollection*/) {}

/** Reimplementation to insert the text of a dragged reference into the edit view. */
void CPlainWriteDisplay::contentsDragEnterEvent( QDragEnterEvent* e ) {
	if (CDragDropMgr::canDecode(e)) {
		e->accept(true);
	} else {
		e->accept(false);
		e->ignore();
	}
}

/** Reimplementation to insert the text of a dragged reference into the edit view. */
void CPlainWriteDisplay::contentsDragMoveEvent( QDragMoveEvent* e ) {
	if (CDragDropMgr::canDecode(e)) {
		placeCursor(e->pos());
		ensureCursorVisible();
		e->accept(true);
	} else {
		e->accept(false);
		e->ignore();
	}
}

/** Reimplementation to manage drops of our drag and drop objects. */
void CPlainWriteDisplay::contentsDropEvent( QDropEvent* e ) {
	if ( CDragDropMgr::canDecode(e) ) {
		e->acceptAction();

		CDragDropMgr::ItemList items = CDragDropMgr::decode(e);
		CDragDropMgr::ItemList::iterator it;
		for (it = items.begin(); it != items.end(); ++it) {
			switch ((*it).type()) {
			case CDragDropMgr::Item::Bookmark: {
						CSwordModuleInfo* module = backend()->findModuleByName((*it).bookmarkModule());
						util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
						key->key( (*it).bookmarkKey() );

						QString moduleText = key->strippedText();

						const QString text = QString::fromLatin1("%1\n(%2, %3)\n").arg(moduleText).arg((*it).bookmarkKey()).arg((*it).bookmarkModule());

						placeCursor( e->pos() );
						insert( text );
						break;
					}
				case CDragDropMgr::Item::Text: {
					placeCursor( e->pos() );
					insert( (*it).text() );
					break;
				}
			default:
				break;
			}

		};
	}
}
