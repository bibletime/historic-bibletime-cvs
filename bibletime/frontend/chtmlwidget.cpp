/***************************************************************************
                          chtmlwidget.cpp  -  description
                             -------------------
    begin                : Sun Feb 27 2000
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

#include "chtmlwidget.h"
#include "ctoolclass.h"
#include "presenters/cswordpresenter.h"
#include "thirdparty/qt3stuff/qrichtext_p.h"
#include "thirdparty/qt3stuff/qt3stuff.h"
#include "../ressource.h"

#include <stdio.h>

//QT includes
#include <qurl.h>
#include <qfile.h>
#include <qtextstream.h>
#include <qclipboard.h>
#include <qevent.h>
#include <qdragobject.h>
#include <qpixmapcache.h>
#include <qstylesheet.h>
#include <qwidget.h>
#include <qtoolbutton.h>

//KDE includes
#include <kapp.h>
#include <kaction.h>
#include <kstdaction.h>
#include <klocale.h>
#include <kurl.h>
#include <kconfig.h>
#include <kfiledialog.h>
#include <kcmenumngr.h>
#include <kglobal.h>
#include <kcursor.h>
#include <kiconloader.h>
#include <kglobalsettings.h>
#include <kpopupmenu.h>
#include <ktoolbar.h>
#include <kcolorbutton.h>

//Sword includes
#include <swmodule.h>

CHTMLWidget::CHTMLWidget(QWidget *parent, const char *name ) : QTextEdit(parent, name) {
	qDebug("constructor of CHTMLWidget");
	m_config = KGlobal::config();
	m_popup = 0;
	m_anchor = QString::null;
	m_anchorMenu = 0;
	mousePressed = inDoubleClick = false;
		
	setTextFormat( Qt::RichText );
	setReadOnly(true);
	viewport()->setMouseTracking(true);
	
	initView();	
	initConnections();
	initColors();
	initFonts();	
}

CHTMLWidget::~CHTMLWidget(){
	qDebug("CHTMLWidget::~CHTMLWidget()");
}

/**  */
void CHTMLWidget::initColors(){
	qDebug("CHTMLWidget::initColors()");
#warning ToDo: Implementation!
	
	KConfigGroupSaver groupSaver(m_config, "Colors");
//	setLinkColor( m_config->readColorEntry("Versenumber/URL", &Qt::darkBlue) );	
	
	QColor textColor = m_config->readColorEntry("Normal Text", &Qt::black);
	QColor bgColor = m_config->readColorEntry("Background", &Qt::lightGray);
	
	QPalette p = palette();
	p.setBrush( QPalette::Active, QColorGroup::Base, QBrush(bgColor) );
	
//	QColorGroup cg = p.active();
	p.setColor(QPalette::Active, QColorGroup::Text, textColor);
//	cg.setColor(QColorGroup::Base, bgColor);
//	p.setActive(cg);	
//	setPalette(p);		
//	refresh();
}

/** Initializes the fonts of the HTML-widget */
void CHTMLWidget::initFonts(){
//	KConfigGroupSaver groupSaver(m_config,"Fonts");
//	QFont dummy = m_config->readFontEntry("Presenter", QFont());
//	setFont(dummy);	
}

/**  */
void CHTMLWidget::initView(){
	disconnect(dragStartTimer, SIGNAL(timeout()),
		this, SLOT(startDrag()));

	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
}


/** Initializes the connections to SIGNALs */
void CHTMLWidget::initConnections(){
	if (!isReadOnly()) {
	 	connect(this, SIGNAL(currentFontChanged(const QFont&)),
			SLOT(slotCurrentFontChanged(const QFont&)));
		connect(this, SIGNAL(currentColorChanged(const QColor&)),
			SLOT(slotCurrentColorChanged(const QColor&)));
		connect(this, SIGNAL(currentAlignmentChanged(int)),
			SLOT(slotCurrentAlignementChanged(int)));	
	}
	else {
	 	disconnect(this, SIGNAL(currentFontChanged(const QFont&)),
			this, SLOT(slotCurrentFontChanged(const QFont&)));
		disconnect(this, SIGNAL(currentColorChanged(const QColor&)),
			this, SLOT(slotCurrentColorChanged(const QColor&)));
		disconnect(this, SIGNAL(currentAlignmentChanged(int)),
			this, SLOT(slotCurrentAlignementChanged(int)));		
	}
}

/** Reinitialize the colors, fonts etc. */
void CHTMLWidget::refresh(){
	initColors();
	initFonts();
}

/** A url was called or setSource was called. If the url is a refrence emit rerenceClicked. */
void CHTMLWidget::slotURLClicked(const QString& url){
	qDebug("CHTMLWidget::slotURLClicked");
	if (url.left(8) == "sword://") {
		// it's reference, emit reference selected
		emit referenceClicked(url);
	}
}

/** Loads a HTML file in the widget. */
void CHTMLWidget::setHTMLSource(const QString& url){
	if (url.left(1) != "/") {	//a filename without path
		qDebug("CHTMLWidget::setHTMLSource filename without path!");
		QString myFile = CToolClass::locatehtml( url );
		if (QFile::exists(myFile)) {
			qDebug("file exists!");
			KURL kurl(myFile);
  		mimeSourceFactory()->addFilePath( kurl.directory() );
			
			//read in the HTML file and use setText()
			QString text = QString::null;
			QFile file(myFile);
			if ( file.open(IO_ReadOnly) ) {    // file opened successfully
				QTextStream t( &file );        // use a text stream
				text = t.read();
				file.close();
		
				setText( text );				
			}
		}
		else setText( i18n("<B>HTML page %1 not found!<BR>Please check the follwing points:	permissions of $KDEDIR/share/apps/bibletime and subdirecries, installation of files.").arg(url) );
	}
	else {
		if (QFile::exists(url)) {
			qDebug("file exists!");
			//read in the HTML file and use setText()
			KURL kurl(url);
  		mimeSourceFactory()->addFilePath( kurl.directory() );
			
			QString text = QString::null;			
			QFile file(url);
			if ( file.open(IO_ReadOnly) ) {    // file opened successfully
				QTextStream t( &file );        // use a text stream
				text = t.read();
				file.close();
				setText( text );
			}
		}
	}
}

/** Saves the HTML page */
void CHTMLWidget::slotSaveAsHTML(){
	qDebug("CHTMLWidget::slotSaveAsHTML");	
	QString file = KFileDialog::getSaveFileName (QString::null, i18n("*.html *.htm *.shtml *.shtm | HTML files\n *.* | All files (*.*)"), 0, i18n("Save text as HTML page ..."));	
	if (!file.isNull()) {
		CToolClass::savePlainFile( file, text());
	}
}

/** Saves the HTML page */
void CHTMLWidget::slotSaveAsText(){
	qDebug("CHTMLWidget::slotSaveAsText");
	
	QString file = KFileDialog::getSaveFileName (QString::null, i18n("*.txt | Text file (*.txt)\n*.* | All files (*.*)"), 0, i18n("Save text as plain text ..."));

	if (!file.isNull()) {
		QString html = document()->plainText();
		CToolClass::savePlainFile( file, html);
	}
}

//**  */
void CHTMLWidget::contentsDragEnterEvent(QDragEnterEvent* e){
  qDebug("CHTMLWidget::contentsDragEnterEvent");
  e->accept(QTextDrag::canDecode(e));
}

/**  */
void CHTMLWidget::contentsDragMoveEvent(QDragMoveEvent* e){
  e->accept(QTextDrag::canDecode(e));
}

/**  */
void CHTMLWidget::contentsDropEvent(QDropEvent* e){
	qDebug("CHTMLWidget::contentsDropEvent");
 	QString str;
 	QCString submime;

 	if ( ( QTextDrag::decode(e,str,submime=BOOKMARK) || QTextDrag::decode(e,str,submime=REFERENCE) ) && !str.isEmpty() ){
		QString ref = QString::null;
		QString mod = QString::null;		
		qDebug((const char*)str.local8Bit());
  		
 		CToolClass::decodeReference(str,mod,ref);   		   		
 		qDebug((const char*)ref.local8Bit());
 		emit referenceClicked(ref);
	}
}


/** Reimplementation. */
void CHTMLWidget::contentsMousePressEvent(QMouseEvent* e) {	
  m_pressedPos = e->pos();
  m_anchor = anchorAt(e->pos());
//  if (!m_anchor.isEmpty())
//  	mightStartDrag = true;  	
	viewport()->setCursor( anchorAt(e->pos()).isEmpty() ? arrowCursor : KCursor::handCursor() );
	
 	if (!onLink.isEmpty() && e->button() == RightButton && m_anchorMenu) {	//popup installed menu 	
		m_anchorMenu->popup( e->globalPos() );
  }
  else if (m_popup && e->button() == RightButton){ //popup normal menu
    bool selectedWord = false;
    if (!hasSelectedText()) {
    	placeCursor(e->pos());
    	
	    Qt3::QTextCursor c1 = *cursor;
	    Qt3::QTextCursor c2 = *cursor;
	    c1.gotoWordLeft();
	    c2.gotoWordRight();
	    doc->setSelectionStart( Qt3::QTextDocument::Standard, &c1 );
	    doc->setSelectionEnd( Qt3::QTextDocument::Standard, &c2 );
	    *cursor = c2;
	    repaintChanged();
	    selectedWord = true;
		}		
		m_popup->exec( e->globalPos() );
		
		if (hasSelectedText() && selectedWord) {
			document()->removeSelection( Qt3::QTextDocument::Standard);
		}
	  repaintChanged();		
  }	
	QTextEdit::contentsMousePressEvent(e);
}

/** Reimplementation of QTextEdit */
void CHTMLWidget::contentsMouseReleaseEvent( QMouseEvent* e){
	QTextEdit::contentsMouseReleaseEvent( e );
}

/** Reimplementation.*/
void CHTMLWidget::contentsMouseMoveEvent(QMouseEvent* e) {
  if ( mousePressed ) {
		if ( mightStartDrag ) { //e might star a drag
	    dragStartTimer->stop();
	    if ( ( e->pos() - dragStartPos ).manhattanLength() > QApplication::startDragDistance() )
				startDrag();
	    if ( !isReadOnly() )
				viewport()->setCursor( ibeamCursor );
	    return;
	}
	else if (!anchorAt(e->pos()).isEmpty()) {
		QString ref = m_anchor;
		if (ref.left(8) == "sword://") //remove sword://, if it is there
			ref = ref.mid( 8, ref.length() - 8 );
		if (ref.right(1) == "/") {
			ref = ref.mid( 0, ref.length() - 1 );
		}

#warning HACK!		
		QString Module = QString::null;
		if ( parent() && parent()->inherits("CSwordPresenter") ) {
			if (static_cast<CSwordModuleInfo*>(((CSwordPresenter*)parent())->getModuleList().first())) {
				Module = static_cast<CSwordModuleInfo*>(((CSwordPresenter*)parent())->getModuleList().first())->module()->Name();
			}
			else {
				Module = "unknown";
			}
		}
		else
			return;
		mousePressed = false;
		inDoubleClick = false;				 				
		mightStartDrag = false;
				
		mousePressed = false;
		inDoubleClick = false;

		QTextDrag *d = new QTextDrag(CToolClass::encodeReference(Module,ref),viewport());
    d->setSubtype(REFERENCE);
    d->setPixmap(REFERENCE_ICON_SMALL);
    d->drag();
		return;
	}
	
	mousePos = e->pos();
	doAutoScroll();
	oldMousePos = mousePos;
    }

    if ( !isReadOnly() && !mousePressed ) {
	if ( doc->hasSelection( Qt3::QTextDocument::Standard ) && doc->inSelection( Qt3::QTextDocument::Standard, e->pos() ) )
	    viewport()->setCursor( arrowCursor );
	else
	    viewport()->setCursor( ibeamCursor );
    }

    if ( isReadOnly() && linksEnabled() ) {
	Qt3::QTextCursor c = *cursor;
	placeCursor( e->pos(), &c );
#ifndef QT_NO_NETWORKPROTOCOL
	if ( c.parag() && c.parag()->at( c.index() ) &&
	     !anchorAt(e->pos()).isEmpty() ) {
#ifndef QT_NO_CURSOR
	    viewport()->setCursor( pointingHandCursor );
#endif
	    onLink = c.parag()->at( c.index() )->format()->anchorHref();
	    QUrl u( doc->context(), onLink, TRUE );
	    emitHighlighted( u.toString( FALSE, FALSE ) );
	} else {
#ifndef QT_NO_CURSOR
	    viewport()->setCursor( isReadOnly() ? arrowCursor : ibeamCursor );
#endif
	    onLink = QString::null;
	    emitHighlighted( QString::null );
	}
#endif
    }
//	if (mousePressed) {	//time to start a drag (ref or text) ? 	
// 	  if (abs(e->pos().x() - m_pressedPos.x()) > KGlobalSettings::dndEventDelay()
// 	  	|| abs(e->pos().y() - m_pressedPos.y()) > KGlobalSettings::dndEventDelay())
//		{
//			dragStartTimer->stop();					
//			if (mightStartDrag && mousePressed &&
//				document()->hasSelection(Qt3::QTextDocument::Standard) )
//			{
//				mousePressed = false;
//				inDoubleClick = false;								
//				mightStartDrag = false;				
//				
//				QDragObject *drag = new QTextDrag( document()->selectedText( Qt3::QTextDocument::Standard ), viewport() );
//				if ( isReadOnly() )
//					drag->dragCopy();
//		    else  if ( drag->drag() && QDragObject::target() != this
//		    	&& QDragObject::target() != viewport() )
//		    {
//			    doc->removeSelectedText( Qt3::QTextDocument::Standard, cursor );
//			    repaintChanged();
//				}
//				return;
//			}			
//			if ( !m_anchor.isEmpty()
//				&& linksEnabled()
//				&& !mightStartDrag
//				&& mousePressed /* mightStartDrag is for plain text drags */
//			)
//			{ //drag a reference
//				//qDebug((const char*)m_anchor.local8Bit());
//				QString ref = m_anchor;
//				if (ref.left(8) == "sword://") //remove sword://, if it is there
//					ref = onLink.mid( 8, ref.length() - 9 );
//				QString Module = QString::null;
//				if ( parent() && parent()->inherits("CModulePresenter") ) {
//					if (static_cast<CSwordModuleInfo*>(((CModulePresenter *)this->parent())->getModuleInfo())) {
//						Module = static_cast<CSwordModuleInfo*>(((CModulePresenter *)this->parent())->getModuleInfo())->module()->Name();
//					}
//					else {
//						Module = "unknown";
//					}
//				}				
//				mousePressed = false;
//				inDoubleClick = false;				 				
//				mightStartDrag = false;
//				
// 				QTextDrag *d = new QTextDrag(CToolClass::encodeReference(Module,ref),viewport());
//        d->setSubtype(REFERENCE);
//        d->setPixmap(REFERENCE_ICON_SMALL);
//        d->drag();
//
//				return;
//			}
//		}							
//		mousePos = e->pos();
//		doAutoScroll();
//		oldMousePos = mousePos;				
//	}
//	
//  if ( !isReadOnly() && !mousePressed ) {
//		if ( document()->hasSelection( Qt3::QTextDocument::Standard )
//			&& doc->inSelection( Qt3::QTextDocument::Standard, e->pos() )
//		)			
//			viewport()->setCursor( arrowCursor );
//		else
//			viewport()->setCursor( ibeamCursor );
//	}
//
//	if ( isReadOnly() && linksEnabled() ) {
//		Qt3::QTextCursor c = *cursor;
//		placeCursor( e->pos(), &c );
//		if ( c.parag() && c.parag()->at( c.index() )
//			&& /*c.parag()->at( c.index() )->format()->isAnchor()*/!anchorAt(e->pos()).isEmpty() )
//		{
//			viewport()->setCursor( KCursor::handCursor() );
//			onLink = c.parag()->at( c.index() )->format()->anchorHref();
//			QUrl u( document()->context(), onLink, true );
//			emitHighlighted( u.toString( false, false ) );
//		}
//		else {
//			viewport()->setCursor( isReadOnly() ? arrowCursor : ibeamCursor );
//			onLink = QString::null;
//			emitHighlighted( QString::null );
//		}
//	}	
}

/** Installes a menu which will popup if the right mouse button was pressed on an anchor. */
void CHTMLWidget::installAnchorMenu( QPopupMenu* anchorMenu ){
	debug("CHTMLWidget::installAnchorMenu( QPopupMenu* anchorMenu )");
	ASSERT( anchorMenu );
	m_anchorMenu = anchorMenu;
}

/** Returns the current anchor. */
QString CHTMLWidget::getCurrentAnchor(){
	qDebug("CHTMLWidget::getCurrentAnchor()");
	return pressedLink;
}

/** No descriptions */
void CHTMLWidget::installPopup( QPopupMenu* popup ){
	ASSERT(popup);
	m_popup = popup;
}

/** No descriptions */
void CHTMLWidget::slotCopyAll(){
	QClipboard *cb = KApplication::clipboard();
	cb->setText( document()->text() );
}

/** Sets the HTML widget editable or not */
void CHTMLWidget::setReadOnly( const bool readOnly ){
	m_readOnly = readOnly;
	initConnections();
		
	viewport()->setCursor( m_readOnly ? arrowCursor : ibeamCursor );
	viewport()->update();	
}

/** Returns true if this HTML widget is read only, false if it's editable */
bool CHTMLWidget::isReadOnly() const {
	return m_readOnly;
}

/** This function returns the edit toolbar used for the HTML widget. The items of the toolbar are already connected to the correct slots. */
void CHTMLWidget::createEditToolbar( KToolBar* bar ){
	ASSERT(bar);
	if (!bar)
		return;

	m_docSaveButton = new QToolButton(SmallIcon("filesave"),i18n("Save"), "",
		this, SLOT(slotSaveDocument()), bar);
	m_docDeleteButton = new QToolButton(SmallIcon("trashcan_empty"),i18n("Delete"), "",
		this, SIGNAL(sigDeleteDocument()), bar);
				
	m_colorChooser = new KColorButton(black, bar);
	connect(m_colorChooser, SIGNAL(changed(const QColor&)),
		this, SLOT(slotSetCurrentColor(const QColor&)));
		
	m_fontAction = new KFontAction(i18n("Choose a font:"), 0, bar);
	connect(m_fontAction, SIGNAL(activated(const QString&)),
		this, SLOT(slotSetCurrentFont(const QString&)));
	m_fontAction->plug(bar);
		
	m_fontSizeCombo = new QComboBox(bar, "toolbar");
	m_fontSizeCombo->setEditable(true);
	connect(m_fontSizeCombo, SIGNAL(activated(const QString&)),
		this, SLOT(slotSetCurrentFontSize(const QString&)));
	
	//insert items into the combo box, we use the sized introduced in StarOffice	
	int index = 6;
	int stepSize = 1;
	for (index = 6; index <= 100; index+=stepSize) {
		m_fontSizeCombo->insertItem( QString::number(index) );
		if (index >=12 )
			stepSize = 2;
		if (index >= 28)
			stepSize = 4;
		if (index >= 48)
			stepSize = 6;
		if (index >= 72)
			stepSize = 8;
	}
	
	//format buttons		
	m_boldButton = new QToolButton(SmallIcon("text_bold"),i18n("Set bold"), "",
		this, SLOT(slotToggleBold()), bar);
	m_boldButton->setToggleButton(true);
	m_italicButton = new QToolButton(SmallIcon("text_italic"),i18n("Set italic"), "",
		this, SLOT(slotToggleItalic()), bar);
	m_italicButton->setToggleButton(true);		
	m_underlineButton = new QToolButton(SmallIcon("text_under"),i18n("Set underlined"), "",
		this, SLOT(slotToggleUnderline()), bar);		
	m_underlineButton->setToggleButton(true);		
	
	//alignement buttons
	m_alignLeftButton = new QToolButton(SmallIcon("text_left"),i18n("Align left"), "",
		this, SLOT(slotAlignLeft()), bar);
	m_alignLeftButton->setToggleButton(true);		
	m_alignCenterButton = new QToolButton(SmallIcon("text_center"),i18n("Center text"), "",
		this, SLOT(slotAlignCenter()), bar);
	m_alignCenterButton->setToggleButton(true);		
	m_alignRightButton = new QToolButton(SmallIcon("text_right"),i18n("Align right"), "",
		this, SLOT(slotAlignRight()), bar);
	m_alignRightButton->setToggleButton(true);		
	m_alignJustifyButton = new QToolButton(SmallIcon("text_block"),i18n("Justify"), "",
		this, SLOT(slotAlignJustify()), bar);
	m_alignJustifyButton->setToggleButton(true);
}

/** emit a signal to save the text */
void CHTMLWidget::slotSaveDocument(){
	emit sigSaveDocument(text());
}

/** No descriptions */
void CHTMLWidget::slotToggleBold(){
	QTextEdit::setBold( !bold() );
	m_boldButton->setOn(bold());	
}

/** No descriptions */
void CHTMLWidget::slotToggleItalic(){
	QTextEdit::setItalic(!italic() );
	m_italicButton->setOn(italic());	
}

/** No descriptions */
void CHTMLWidget::slotToggleUnderline(){
	QTextEdit::setUnderline( !underline() );
	m_underlineButton->setOn(underline());		
}

/** No descriptions */
void CHTMLWidget::slotSetCurrentFontSize( const QString& sizeText ){
	const int size = sizeText.toInt();
	if (size > 0)
		setPointSize(size);
}

void CHTMLWidget::slotSetCurrentFont(const QString& font) {
	qDebug("CHTMLWidget::fontChanged(const QString& font)");
	
	QFont newFont(font, pointSize());
	newFont.setBold(bold());
	newFont.setItalic(italic());
	newFont.setUnderline(underline());	
	
	setFont( newFont );
}

/** Is calledwhen a new color was selected. */
void CHTMLWidget::slotSetCurrentColor( const QColor& color){
	qDebug("CHTMLWidget::colorChanged( const QColor& )");
	setColor(color);
}

/** No descriptions */
void CHTMLWidget::slotCurrentFontChanged( const QFont& f){
	m_fontAction->setFont( f.family() );	
	for (int index = 0; index < m_fontSizeCombo->count(); index++ ) {		
		if (m_fontSizeCombo->text(index).toInt() == f.pointSize()) {
			m_fontSizeCombo->setCurrentItem(index);	
			break;
		}
	}
	m_fontSizeCombo->setEditText( QString::number(f.pointSize()) );
	m_boldButton->setOn( f.bold() );
	m_italicButton->setOn( f.italic() );
	m_underlineButton->setOn( f.underline() );	
}

/** No descriptions */
void CHTMLWidget::slotCurrentColorChanged( const QColor& c){
	m_colorChooser->setColor(c);
}

/** No descriptions */
void CHTMLWidget::slotCurrentAlignementChanged(int a){
	m_alignLeftButton->setOn( (a == AlignLeft) || (a == Qt3::AlignAuto) );
	m_alignCenterButton->setOn( a == AlignHCenter );
	m_alignRightButton->setOn( a == AlignRight );
	m_alignJustifyButton->setOn( a == Qt3::AlignJustify );	
}

/** No descriptions */
void CHTMLWidget::slotAlignLeft(){
	m_alignLeftButton->setOn(true);	
	m_alignCenterButton->setOn(false);	
	m_alignRightButton->setOn(false);	
	m_alignJustifyButton->setOn(false);		
		
	setAlignment(AlignLeft);
}

/** No descriptions */
void CHTMLWidget::slotAlignCenter(){
	m_alignLeftButton->setOn(false);	
	m_alignCenterButton->setOn(true);	
	m_alignRightButton->setOn(false);	
	m_alignJustifyButton->setOn(false);		
	
	setAlignment(AlignHCenter);
}

/** No descriptions */
void CHTMLWidget::slotAlignRight(){
	m_alignLeftButton->setOn(false);	
	m_alignCenterButton->setOn(false);	
	m_alignRightButton->setOn(true);	
	m_alignJustifyButton->setOn(false);		
		
	setAlignment(AlignRight);
}

/** No descriptions */
void CHTMLWidget::slotAlignJustify(){
	m_alignLeftButton->setOn(false);	
	m_alignCenterButton->setOn(false);	
	m_alignRightButton->setOn(false);	
	m_alignJustifyButton->setOn(true);		
		
	setAlignment(Qt3::AlignJustify);
}

/** No descriptions */
void CHTMLWidget::slotSelectAll(){
	selectAll(true);
}

/** Returns true if the links are enabled. */
bool CHTMLWidget::linksEnabled() const {
//	qDebug("CHTMLWidget::linksEnabled()");	
	return true;
}

/** Reimplementation from QTextView. */
void CHTMLWidget::emitLinkClicked( const QString& s){
	if (s.left(8) == "sword://") {
		emit referenceClicked(s.mid(8,s.length()-9)); //the URL has a trailing slash at the end
	}
	else {
		emit referenceClicked(s);
	}
}

/** Copies the displayed document into the clipboard. */
void CHTMLWidget::copyDocument(){
	if (!document()->text().isEmpty()) {
		QClipboard* cb = KApplication::clipboard();			
		cb->setText(document()->text());
	}
	
}
