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
#include "backend/creferencemanager.h"
#include "backend/cswordldkey.h"
#include "backend/cswordversekey.h"
#include "backend/cswordbackend.h"
#include "backend/cswordmoduleinfo.h"
#include "presenters/cswordpresenter.h"
#include "thirdparty/qt3stuff/qt3stuff.h"
#include "thirdparty/qt3stuff/qrichtext_p.h"
#include "resource.h"
#include "tooltipdef.h"
#include "whatsthisdef.h"
#include "cbtconfig.h"

#include "util/scoped_resource.h"

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
#include <qapplication.h>

//KDE includes
#include <kapp.h>
#include <kaction.h>
#include <kstdaction.h>
#include <klocale.h>
#include <kurl.h>
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

CHTMLWidget::ToolTip::ToolTip(QWidget* parent)
	: QToolTip(parent) {

}

void CHTMLWidget::ToolTip::maybeTip(const QPoint& p) {
//	qWarning("void CHTMLWidget::ToolTip::maybeTip(const QPoint& p) ");
	if (!parentWidget()->inherits("CHTMLWidget"))
		return;

	CHTMLWidget* htmlWidget = dynamic_cast<CHTMLWidget*>(parentWidget());	
	ASSERT(htmlWidget);
	QPoint p1 = htmlWidget->viewportToContents(p);
	QString link = QString::null;
	QString text = QString::null;

  link = htmlWidget->anchorAt(p1);
	if ( link.isEmpty() )
		return;
	else {
//		qWarning("link is valid");
	  Qt3::QTextCursor c( htmlWidget->getDocument() );
	  htmlWidget->placeCursor( p1, &c );
		QRect rect = c.parag()->rect();
		
		//map rect coordinates to widget's ones
    rect.setX( htmlWidget->contentsToViewport(rect.topLeft()).x() );
    rect.setY( htmlWidget->contentsToViewport(rect.topLeft()).y() );
    rect.setWidth( htmlWidget->contentsToViewport(rect.bottomRight()).x() - rect.x() );
    rect.setHeight( htmlWidget->contentsToViewport(rect.bottomLeft()).y() - rect.y() );
        				
		QString module = QString::null;
		QString ref = QString::null;
		CReferenceManager::Type type;		
		const bool ok = CReferenceManager::decodeHyperlink(link, module, ref, type);
		if (!ok || ref.isEmpty())
			return;

		const QFont oldFont = font();				
		
		CSwordModuleInfo* m	= 0;
		if (module.isEmpty() || module.isNull()) {
			module = CReferenceManager::preferredModule( type );
		}		
		m = backend()->findModuleByName(module);
		if (m){
			if (m->type() == CSwordModuleInfo::Bible || m->type() == CSwordModuleInfo::Commentary) {
				CSwordModuleInfo* module = htmlWidget->modules().first();
				if (module) {
					ref = CReferenceManager::parseVerseReference(ref/*, module->module()->Lang(), backend()->booknameLanguage()*/ );
				}
			}
			util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance( m ) );
			if (key) {
				backend()->setFilterOptions( CBTConfig::getFilterOptionDefaults() );				
				
				key->key(ref);
				text = key->renderedText();
			}
			if (m->isUnicode()) {
				setFont( CBTConfig::get(CBTConfig::unicode) );
			}
		}
		if (!text.isEmpty()) {
			text = QString::fromLatin1("<B>%1</B><HR>%2").arg(ref).arg(text);
			tip(rect, text);
		}
		setFont(oldFont);
	}
}

CHTMLWidget::CHTMLWidget(const bool useColorsAndFonts,QWidget *parent, const char *name )
	: Qt3::QTextEdit(parent, name),m_moduleList( new ListCSwordModuleInfo ) {
	
	m_popup = 0;
	m_anchor = QString::null;
	m_anchorMenu = 0;
	m_selectedWord = false;
	mousePressed = inDoubleClick = false;		
	setTextFormat( Qt::RichText );
	setReadOnly(true);

	QFont unicodeFont = CBTConfig::get(CBTConfig::unicode);
 	if (!document()->charsetMap->contains(unicodeFont.family()))
 		document()->charsetMap->insert(unicodeFont.family(), QFont::Unicode);
		
	initView();	
	initConnections();
	if (useColorsAndFonts) {
		initColors();
		initFonts();	
	}
}

CHTMLWidget::~CHTMLWidget(){
	delete m_moduleList;
	m_moduleList = 0;
}

/**  */
void CHTMLWidget::initColors(){
//	qDebug("CHTMLWidget::initColors()");
//	setLinkColor( m_config->readColorEntry("Versenumber/URL", &Qt::darkBlue) );		
//	QColor textColor = m_config->readColorEntry("Normal Text", &Qt::red);	
	const QColor bgColor = CBTConfig::get(CBTConfig::backgroundColor);
	setPaper(QBrush(bgColor));
	
//using a standard text color doesn't work at the moment!

//	QPalette p = palette();
//	p.setBrush( QPalette::Active, QColorGroup::Base, QBrush(bgColor) );	
//	QColorGroup cg = p.active();
//	p.setColor(QPalette::Active, QColorGroup::Text, textColor);
//	cg.setColor(QColorGroup::Base, bgColor);
//	p.setActive(cg);	
//	setPalette(p);		
//	refresh();
}

/** Initializes the fonts of the HTML-widget */
void CHTMLWidget::initFonts(){
// ACHTUNG: CBTCONFIG benutzen
//	ConfigGroupSaver groupSaver(m_config, "Fonts");		
//	if (document()->charsetMap->contains(font().family())) { //remove old standard font
//		document()->charsetMap->remove(font().family());
//	}
//	
//	QFont f = m_config->readFontEntry(i18n("Display window"));	
//	document()->setDefaultFont( f );
//	setFont(f);
//	
//	document()->charsetMap->replace(f.family(), f.charSet());
}

/**  */
void CHTMLWidget::initView(){
	m_toolTip = new ToolTip(this);
	disconnect(dragStartTimer, SIGNAL(timeout()),
		this, SLOT(startDrag()));
	
//	QStringList paths;	
	
	setAcceptDrops(true);
	viewport()->setAcceptDrops(true);
}


/** Initializes the connections to SIGNALs */
void CHTMLWidget::initConnections(){
	connect( this, SIGNAL( linkClicked( const QString & ) ),
		this, SLOT( setSource( const QString & ) ) );	
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
//	qDebug("CHTMLWidget::refresh()");
	initColors();
	initFonts();
}

/** Loads a HTML file in the widget. */
void CHTMLWidget::setHTMLSource(const QString& url){
	qWarning(url.local8Bit());
	if (url.left(1) != "/") {	//a filename without path
		QString myFile = CToolClass::locatehtml( url );
		if (QFile::exists(myFile)) {
			KURL kurl(myFile);
  		mimeSourceFactory()->addFilePath( kurl.directory() );
			
			//read in the HTML file and use setText()
			QFile file(myFile);
			if ( file.open(IO_ReadOnly) ) {    // file opened successfully
				QTextStream t( &file );        // use a text stream
				const QString text = t.read();
				file.close();
				setText( text/*, kurl.directory()*/ );
			}
		}
		else
			setText( i18n("Unable to find the page %1!").arg(url) );
	}
	else {
		if (QFile::exists(url)) {
			//read in the HTML file and use setText()
			KURL kurl(url);
  		mimeSourceFactory()->addFilePath( kurl.directory() );			
			QFile file(url);
			if ( file.open(IO_ReadOnly) ) {    // file opened successfully
				QTextStream t( &file );        // use a text stream
				const QString text = t.read();
				file.close();
				setText( text/*, kurl.directory()*/ );
			}
		}
	}
}

/** Saves the HTML page */
void CHTMLWidget::slotSaveAsHTML(){
	QString file = KFileDialog::getSaveFileName (QString::null, i18n("*.html *.htm *.shtml *.shtm | HTML files\n *.* | All files (*.*)"), 0, i18n("Save text as HTML page ..."));	
	if (!file.isNull())
		CToolClass::savePlainFile( file, text().utf8());
}

/** Saves the HTML page */
void CHTMLWidget::slotSaveAsText(){
	const QString file = KFileDialog::getSaveFileName (QString::null, i18n("*.txt | Text file (*.txt)\n*.* | All files (*.*)"), 0, i18n("Save text as plain text ..."));

	if (!file.isNull()) {
		QString html = document()->plainText();
		CToolClass::savePlainFile( file, html);
	}
}

//**  */
void CHTMLWidget::contentsDragEnterEvent(QDragEnterEvent* e){
	Qt3::QTextEdit::contentsDragEnterEvent(e);
  e->accept(QTextDrag::canDecode(e));
}

/**  */
void CHTMLWidget::contentsDragMoveEvent(QDragMoveEvent* e){
	Qt3::QTextEdit::contentsDragMoveEvent(e);
  e->accept(QTextDrag::canDecode(e));
}

/**  */
void CHTMLWidget::contentsDropEvent(QDropEvent* e){
 	QString str;
 	QCString submime;

	if (isReadOnly()) {
	 	if ( ( QTextDrag::decode(e,str,submime=BOOKMARK) || QTextDrag::decode(e,str,submime=REFERENCE) ) && !str.isEmpty() ){
			QString ref = QString::null;
			QString mod = QString::null;		
	 		CReferenceManager::decodeReference(str,mod,ref);
	 		emit referenceDropped(/*mod,*/ ref);
		}
		else
			e->ignore();
	}
	else {
		QString text = QString::null;
	 	if ( ( QTextDrag::decode(e,str,submime=BOOKMARK) || QTextDrag::decode(e,str,submime=REFERENCE) ) && !str.isEmpty() ){
			QString ref = QString::null;
			QString mod = QString::null;
	 		CReferenceManager::decodeReference(str,mod,ref);
			CSwordModuleInfo* module = backend()->findModuleByName(mod);
	 		if (module) {		 			
	 			CSwordKey* key = CSwordKey::createInstance(module);; 					
				key->key(ref);
				text = key->strippedText();
				delete key;
		 	}
		}
		else if (QTextDrag::decode(e,str) && !str.isEmpty())
			text = str;

		if (!text.isEmpty())
			insert(text);
		else {
			e->ignore();
		}
	}
}


/** Reimplementation. */
void CHTMLWidget::contentsMousePressEvent(QMouseEvent* e) {	
  m_pressedPos = e->pos();
  m_anchor = anchorAt(e->pos());
	viewport()->setCursor(anchorAt(e->pos()).isEmpty() ? arrowCursor : KCursor::handCursor() );
	Qt3::QTextEdit::contentsMousePressEvent(e);
		
 	if (!onLink.isEmpty() && (e->button() == RightButton) && m_anchorMenu) {	//popup installed menu 	
		m_anchorMenu->exec( e->globalPos() );
  }
  else if (m_popup && e->button() == RightButton){ //popup normal menu
    m_selectedWord = false;
    QString selectedWord = QString::null;

   	drawCursor(false);
   	placeCursor(e->pos());    	
   	ensureCursorVisible();
		emit cursorPositionChanged(cursor);
		
    if (selectedText().isEmpty()) {
    	Qt3::QTextCursor c1 = *cursor;
	    Qt3::QTextCursor c2 = *cursor;
	    c1.gotoWordLeft();
	    c2.gotoWordRight();
	    doc->setSelectionStart( Qt3::QTextDocument::Standard, &c1 );
	    doc->setSelectionEnd( Qt3::QTextDocument::Standard, &c2 );
	    *cursor = c2;
	    selectedWord = selectedText();
			emit cursorPositionChanged(cursor);		    	
	   	ensureCursorVisible();			
	    repaintChanged();
	    m_selectedWord = true;
	  }
		m_popup->exec( e->globalPos() );		
		if (m_selectedWord && (selectedWord == selectedText()) ) {
			m_selectedWord = false;
			selectAll(false);
		}		
  }	
}

/** Reimplementation.*/
void CHTMLWidget::contentsMouseMoveEvent(QMouseEvent* e) {
  if ( mousePressed ) {
		if ( mightStartDrag ) { //we might start a drag
	    dragStartTimer->stop();
	    if ( ( e->pos() - dragStartPos ).manhattanLength() > KApplication::startDragDistance() )
				startDrag();
	    if ( !isReadOnly() )
				viewport()->setCursor( ibeamCursor );
	    return;
		}
		else if (!m_anchor.isEmpty()/*!anchorAt(e->pos()).isEmpty() && !hasSelectedText()*/) {
			QString module = QString::null;
			QString key = QString::null;
			CReferenceManager::Type type;			
			const bool ok = CReferenceManager::decodeHyperlink(m_anchor, module, key, type);
			if (!ok)
				return;
			
			mousePressed = false;
			inDoubleClick = false;				 				
			mightStartDrag = false;
					
			QTextDrag *d = new QTextDrag(CReferenceManager::encodeReference(module,key),viewport());
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
		if ( c.parag() && c.parag()->at( c.index() ) &&
	     !anchorAt(e->pos()).isEmpty() ) {
			viewport()->setCursor( pointingHandCursor );
	    onLink = c.parag()->at( c.index() )->format()->anchorHref();
	    QUrl u( doc->context(), onLink, true );
			m_hoverPos = e->pos();
	    emitHighlighted( u.toString( false, false ) );			
		} else {
	    viewport()->setCursor( isReadOnly() ? arrowCursor : ibeamCursor );
	    onLink = QString::null;
	    emitHighlighted( QString::null );
		}
	}
}

/** Installes a menu which will popup if the right mouse button was pressed on an anchor. */
void CHTMLWidget::installAnchorMenu( QPopupMenu* anchorMenu ){
//	qDebug("CHTMLWidget::installAnchorMenu( QPopupMenu* anchorMenu )");
	ASSERT( anchorMenu );
	m_anchorMenu = anchorMenu;
}

/** Returns the current anchor. */
QString CHTMLWidget::getCurrentAnchor(){
	return anchorAt(m_pressedPos);
}

/** No descriptions */
void CHTMLWidget::installPopup( QPopupMenu* popup ){
	ASSERT(popup);
	m_popup = popup;
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

	m_docSaveAction = new KAction(i18n("Save"), ICON_FILE_SAVE, 0, this, SLOT(slotSaveDocument()),bar, "saveDoc_action");	
	m_docSaveAction->setToolTip( TT_PRESENTER_EDIT_SAVE );
	m_docSaveAction->setWhatsThis( WT_PRESENTER_EDIT_SAVE );	
	m_docSaveAction->plug(bar);
	
	m_docDeleteAction = new KAction(i18n("Delete"), ICON_FILE_DELETE, 0, this, SIGNAL(sigDeleteDocument()),bar, "deleteDoc_action");	
	m_docDeleteAction->setToolTip( TT_PRESENTER_EDIT_DELETE );
	m_docDeleteAction->setWhatsThis( WT_PRESENTER_EDIT_DELETE );	
	m_docDeleteAction->plug(bar);
			
	m_colorChooser = new KColorButton(black, bar);
	connect(m_colorChooser, SIGNAL(changed(const QColor&)),SLOT(slotSetCurrentColor(const QColor&)));		
	m_fontAction = new KFontAction(i18n("Choose a font:"), 0, bar);
	connect(m_fontAction, SIGNAL(activated(const QString&)), SLOT(slotSetCurrentFont(const QString&)));
	m_fontAction->plug(bar);
	
	m_fontSizeAction = new KFontSizeAction(i18n("Choose font size"), 0, bar);		
	connect(m_fontSizeAction, SIGNAL(fontSizeChanged(int)), SLOT(setPointSize(int)));
	m_fontSizeAction->plug(bar);
	
	m_boldAction = new KToggleAction(i18n("Bold"), ICON_EDIT_BOLD,0,this, SLOT(slotToggleBold()), bar);
	m_boldAction->plug(bar);
	m_italicAction = new KToggleAction(i18n("Italic"), ICON_EDIT_ITALIC,0, this, SLOT(slotToggleItalic()),bar);	
	m_italicAction->plug(bar);	
	m_underlineAction = new KToggleAction(i18n("Underlined"), ICON_EDIT_UNDER,0, this, SLOT(slotToggleUnderline()),bar);	
	m_underlineAction->plug(bar);
			
	m_alignLeftAction = new KToggleAction(i18n("Left"), ICON_EDIT_LEFT,0, this, SLOT(slotAlignLeft()),bar);
	m_alignLeftAction->plug(bar);
	m_alignCenterAction = new KToggleAction(i18n("Center"), ICON_EDIT_CENTER,0, this, SLOT(slotAlignCenter()),bar);
	m_alignCenterAction->plug(bar);
	m_alignRightAction = new KToggleAction(i18n("Right"), ICON_EDIT_RIGHT,0, this, SLOT(slotAlignRight()),bar);
	m_alignRightAction->plug(bar);
	m_alignJustifyAction = new KToggleAction(i18n("Justify"), ICON_EDIT_JUST,0, this, SLOT(slotAlignJustify()),bar);
	m_alignJustifyAction->plug(bar);
}

/** emit a signal to save the text */
void CHTMLWidget::slotSaveDocument(){
	emit sigSaveDocument(text());
}

/** No descriptions */
void CHTMLWidget::slotToggleBold(){
	Qt3::QTextEdit::setBold( !bold() );
	m_boldAction->setChecked(bold());	
}

/** No descriptions */
void CHTMLWidget::slotToggleItalic(){
	Qt3::QTextEdit::setItalic(!italic() );
	m_italicAction->setChecked(italic());	
}

/** No descriptions */
void CHTMLWidget::slotToggleUnderline(){
	Qt3::QTextEdit::setUnderline( !underline() );
	m_underlineAction->setChecked(underline());		
}

/** No descriptions */
void CHTMLWidget::slotSetCurrentFontSize( int size ){
	if (size > 0)
		setPointSize(size);
}

void CHTMLWidget::slotSetCurrentFont(const QString& font) {
	QFont newFont(font, pointSize());
	newFont.setBold(bold());
	newFont.setItalic(italic());
	newFont.setUnderline(underline());	
	
	setFont( newFont );
}

/** Is calledwhen a new color was selected. */
void CHTMLWidget::slotSetCurrentColor( const QColor& color){
	setColor(color);
}

/** No descriptions */
void CHTMLWidget::slotCurrentFontChanged( const QFont& f){
	m_fontAction->setFont( f.family() );	
	m_fontSizeAction->setFontSize( f.pointSize() );	

	m_boldAction->setChecked( f.bold() );
	m_italicAction->setChecked( f.italic() );
	m_underlineAction->setChecked( f.underline() );	
}

/** No descriptions */
void CHTMLWidget::slotCurrentColorChanged( const QColor& c){
	m_colorChooser->setColor(c);
}

/** No descriptions */
void CHTMLWidget::slotCurrentAlignementChanged(int a){
	m_alignLeftAction->setChecked( (a == AlignLeft) || (a == Qt3::AlignAuto) );
	m_alignCenterAction->setChecked( a == AlignHCenter );
	m_alignRightAction->setChecked( a == AlignRight );
	m_alignJustifyAction->setChecked( a == Qt3::AlignJustify );	
}

/** No descriptions */
void CHTMLWidget::slotAlignLeft(){
	m_alignLeftAction->setChecked(true);	
	m_alignCenterAction->setChecked(false);	
	m_alignRightAction->setChecked(false);	
	m_alignJustifyAction->setChecked(false);		
		
	setAlignment(AlignLeft);
}

/** No descriptions */
void CHTMLWidget::slotAlignCenter(){
	m_alignLeftAction->setChecked(false);	
	m_alignCenterAction->setChecked(true);	
	m_alignRightAction->setChecked(false);	
	m_alignJustifyAction->setChecked(false);		
	
	setAlignment(AlignHCenter);
}

/** No descriptions */
void CHTMLWidget::slotAlignRight(){
	m_alignLeftAction->setChecked(false);	
	m_alignCenterAction->setChecked(false);	
	m_alignRightAction->setChecked(true);	
	m_alignJustifyAction->setChecked(false);		
		
	setAlignment(AlignRight);
}

/** No descriptions */
void CHTMLWidget::slotAlignJustify(){
	m_alignLeftAction->setChecked(false);	
	m_alignCenterAction->setChecked(false);	
	m_alignRightAction->setChecked(false);	
	m_alignJustifyAction->setChecked(true);		
		
	setAlignment(Qt3::AlignJustify);
}

/** No descriptions */
void CHTMLWidget::slotSelectAll(){
	selectAll(false);//workaround
	selectAll(true);
}

/** Returns true if the links are enabled. */
bool CHTMLWidget::linksEnabled() const {
	return true;
}

/** Reimplementation from QTextView. */
void CHTMLWidget::emitLinkClicked( const QString& link){
	if (link.left(7) == QString::fromLatin1("mailto:")) {
		qDebug("open mailer for %s", link.mid(7).latin1());
		KApplication::kApplication()->invokeMailer(link.mid(7), QString::null);
		return;
	}
	
	if (CReferenceManager::isHyperlink(link)) {
		QString ref;
		QString module;
		CReferenceManager::Type type;			
		CReferenceManager::decodeHyperlink(link, module, ref, type);
		CSwordModuleInfo* m = backend()->findModuleByName(module);
		CSwordKey* key = CSwordKey::createInstance(m);
		if (key) {
			emit referenceClicked(module, key->key(ref));
			delete key;			
		}
	}
	else {
		QString url = link;
		if (link.left(1) == "/")
			url = link.mid(1);
		emit linkClicked(url);
	}
}

/** Copies the displayed document into the clipboard. */
void CHTMLWidget::copyDocument(){
	if (!document()->text().isEmpty()) {
		QClipboard* cb = KApplication::clipboard();			
		cb->setText(document()->plainText());
	}	
}

/** Sets the source of this widget. */
void CHTMLWidget::setSource(const QString& name){
	qWarning(name.latin1());	
	if ( isVisible() )
		qApp->setOverrideCursor( waitCursor );
	QString source = name;
	QString mark;
	int hash = name.find('#');
	if ( hash != -1) {
		source  = name.left( hash );
		mark = name.mid( hash+1 );
	}
	if ( source.left(5) == "file:" )
		source = source.mid(6);

	QString url = mimeSourceFactory()->makeAbsolute( source, context() );
	qWarning(url.latin1());
	QString txt;
	bool dosettext = false;

	if ( !source.isEmpty()/* && url != d->curmain */) {
		const QMimeSource* m = mimeSourceFactory()->data( source, context() );
		if ( !m )
			qWarning("CHTMLWidget: no mimesource for %s", source.latin1() );
		else if ( !QTextDrag::decode( m, txt ) )
			qWarning("CHTMLWidget: cannot decode %s", source.latin1() );
		dosettext = true;
	}	
	if ( !mark.isEmpty() ) {
		url += "#";
		url += mark;
	}	
	if ( dosettext )
		setText( txt, url );

	if ( isVisible() && !mark.isEmpty() )
		scrollToAnchor( mark );
	else
		setContentsPos( 0, 0 );	
	if ( isVisible() )
		qApp->restoreOverrideCursor();
}

/** Is called if a link was highlighted. Normally a signal should be emitted. */
void CHTMLWidget::emitHighlighted( const QString& s ){
}

/** Returns the document used by this widget */
Qt3::QTextDocument* CHTMLWidget::getDocument() const{
	return Qt3::QTextEdit::document();
}

/** Places the cursor at position pos */
void CHTMLWidget::placeCursor( const QPoint &pos, Qt3::QTextCursor *c ){
	Qt3::QTextEdit::placeCursor(pos, c);
}

/** Returns a list of modules which are used by the display window which uses this HTML widget. */
ListCSwordModuleInfo& CHTMLWidget::modules() const {
	if (m_moduleList)
		return *m_moduleList;
}

/** Sets the list of modules used by the display window which uses this widget. */
void CHTMLWidget::setModules( ListCSwordModuleInfo& modules ) {
	*m_moduleList = modules; //copy entries
	//add data path of all modules tom our config
	for (m_moduleList->first(); m_moduleList->current(); m_moduleList->next()) {
		const QString path = m_moduleList->current()->config(CSwordModuleInfo::AbsoluteDataPath);
		mimeSourceFactory()->addFilePath( path );		
	};
}
