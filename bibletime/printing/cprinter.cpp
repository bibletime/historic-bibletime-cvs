/***************************************************************************
                          cprinter.cpp  -  description
                             -------------------
    begin                : Sat Aug 5 2000
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

#include "cprinter.h"
#include "cprinterdialog.h"
#include "cprintitem.h"
#include "cstyleformat.h"
#include "cstyleformatframe.h"

#include "../backend/sword_backend/cswordbackend.h"
#include "../backend/sword_backend/cswordversekey.h"
#include "../backend/sword_backend/cswordldkey.h"

#include <kconfig.h>
#include <kprocess.h>
#include <kconfigbase.h>
#include <krandomsequence.h>
#include <klocale.h>
#include <kapp.h>

#include <qfile.h>
#include <qstringlist.h>
#include <qpainter.h>
#include <qpaintdevice.h>
#include <qpaintdevicemetrics.h>


#define PARAGRAPH_SPACE 10 // Space between different paragraphs
#define STYLE_PART_SPACE 1	//Space between the differnt parts (header, moduletext ...)
#define BORDER_SPACE 5	//border between text and rectangle

CPrinter::CPrinter( CImportantClasses* important, QObject* parent ) : QObject(parent) {
	config = new KConfig("bt-printing", false, true );

	ASSERT(important);
	m_important = important;
	m_backend = m_important->swordBackend;
	m_queue = new printItemList;	
	m_queue->setAutoDelete(true);
	m_styleList = new styleItemList;
	m_styleList->setAutoDelete(true);	
	
		
	readSettings();
	setupStandardStyle();		
}

CPrinter::~CPrinter(){
	qDebug("destructor of CPrinter");
	saveSettings();	
	config->sync();		
	delete config;	
	delete m_queue;
	delete m_styleList;
}

/** Returns the path to the preview application. */
QString CPrinter::getPreviewApplication(){
	qDebug("CPrinter::getPreviewApplication()");
	return m_previewApplication;
}

/**  */
void CPrinter::setPreviewApplication( const QString& app){
	qDebug("CPrinter::setPreviewApplication( const QString& app)");
	m_previewApplication = app;
}

/** Returns the right margin. */
unsigned int CPrinter::rightMargin() {
	return m_pageMargin.right;
}

/** Sets the right margin. */
void CPrinter::setRightMargin( unsigned int margin ){
	m_pageMargin.right = margin;
}

/** Returns the left margin. */
unsigned int CPrinter::leftMargin(){
	return m_pageMargin.left;
}

/** Sets the left margin. */
void CPrinter::setLeftMargin( unsigned int margin ){
	m_pageMargin.left = margin;
}

/** Returns the right margin. */
unsigned int CPrinter::upperMargin(){
	return m_pageMargin.top;
}

/** Sets the right margin. */
void CPrinter::setUpperMargin( unsigned int margin ){
	m_pageMargin.top = margin;
}

/** Returns the right margin. */
unsigned int CPrinter::lowerMargin(){
	return m_pageMargin.bottom;
}

/** Sets the right margin. */
void CPrinter::setLowerMargin( unsigned int margin ){
	m_pageMargin.bottom = margin;
}

/** Appends a new page where the next things will be painted. */
bool CPrinter::newPage(){
	bool result = QPrinter::newPage();
	if (result)
		m_pagePosition.curPage++;

	m_pagePosition.rect = getPageSize();			
	return result;
}

/** Sets all the margins at one time. */
void CPrinter::setAllMargins( CPageMargin margins ){
	qDebug("CPrinter::setAllMargins( CPageMargin margins )");
	m_pageMargin = margins;
}

/** Returns the margins of the pages. */
CPrinter::CPageMargin CPrinter::getPageMargins(){
	qDebug("CPrinter::getPageMargins()");
	return m_pageMargin;
}

/** Print item at bottom of the current page. If there's no more space left on the current page create a new one. */
void CPrinter::printItem( QPainter* p, CPrintItem* item){
	qDebug("CPrinter::printItem( CPrintItem* item)");
	/** Printing one item:
		* Printing one item is done in the following way:
		*		- Get style of the item
		*		- See which style parts should be printer
		*		- 3 different part of the function, in the parts we do:
		*			1. Print the header if printing of headers is enabled in the style
		*			2. Print the description if printing of descriptionss is enabled in the style
		*			3. Print the moduletext if printing of the module text is enabled in the style
		*		- In each of these parts we do the same
		*			- Create correct QPen and QBrush objects (for frame of part, foregroundcolor, backgroundcolor ...)
		*			- Calculate space and create new page if it doesn't fit on the current page.
		*			- If the item is larger than one page split the part into more pages.
		*			- print the part
		*	While printing we have to manage the headers of the page.
		*/

//style variables
	CStyle*	style = item->getStyle();
	CStyle::styleType styleType = CStyle::Unknown;	
	QString currentText = QString::null;	
	ASSERT(style);
	if (!style) {
		qDebug("Style is 0. Won't print this item.");
		return;
	}
	
	QPen framePen;
	QPen oldPen = p->pen();
	QPen pen = oldPen;
		
//CStyleFormat relevant	
	CStyleFormat* format = 0;
	QFont	font;	
	QColor bgColor, fgColor;
	CStyleFormat::alignement alignement = CStyleFormat::Left;
	unsigned int identation = 0;
	int arguments = 0;	

//CStyleFormatFrame relevant		
	CStyleFormatFrame*	frame = 0;
	QColor frameColor;
	unsigned int frameThickness = 1;
	
	bool isEnabled = false;
	bool createNewPage = false;
	bool printed = false;
	
	for (unsigned short int index=1; index <= 3; index++ ) {
		if (styleType == CStyle::Unknown)
			styleType = CStyle::Header;
		else if (styleType == CStyle::Header)
			styleType = CStyle::Description;
		else	if (styleType == CStyle::Description)
			styleType = CStyle::ModuleText;
		
		//print the part if it's enabled
		isEnabled = style->hasFormatTypeEnabled( styleType );
		if (isEnabled) {
			format = style->getFormatForType( styleType );
			ASSERT(format);
			if (styleType == CStyle::Header)
				currentText = item->getHeader();
			else if (styleType == CStyle::Description)
				currentText = item->getDescription();
			else	if (styleType == CStyle::ModuleText)
				currentText = item->getModuleText();
			else
				currentText = QString::null;
			
		 	if (currentText.isEmpty())	//we don't want to print empty text
				continue;
				
			font = format->getFont();
			font.setCharSet(QFont::Unicode);
			fgColor = format->getFGColor();
			bgColor = format->getBGColor();
			identation = format->getIdentation();
			alignement = format->getAlignement();
			pen = p->pen();
		
			if (format->hasFrame()) {
				frame = format->getFrame();
				ASSERT(frame);
				frameColor =	frame->getColor();
				frameThickness = frame->getThickness();
			}		
			
			arguments = WordBreak;		
			if (alignement == CStyleFormat::Left)
				arguments |= AlignLeft;
			else if (alignement == CStyleFormat::Center)
				arguments |= AlignHCenter;
			else if (alignement == CStyleFormat::Right)
				arguments |= AlignRight;
			#warning TODO: Make use of justification type
			
			QRect r = getPageSize();
			
			pen.setColor( fgColor );
			p->setPen( pen );		
			p->setFont( font );		
			
			QRect boundingRect = p->boundingRect( m_pagePosition.rect.x()+BORDER_SPACE, m_pagePosition.rect.y(),
				r.width()-BORDER_SPACE, r.height(), arguments, currentText );
			
			//check if the new text fits into page
			if ( (m_pagePosition.rect.y() + boundingRect.height() + (format->hasFrame() ? 2*frameThickness : 0) + STYLE_PART_SPACE ) > r.height() ) {
				//this part doesn't fit on the current page
				newPage();
				boundingRect = p->boundingRect( m_pagePosition.rect.x()+BORDER_SPACE,
					m_pagePosition.rect.y(), r.width()-BORDER_SPACE,
					r.height(), arguments, currentText );
			}
			
			QRect brushRect = boundingRect;
			brushRect.setX( m_pagePosition.rect.x() );
			brushRect.setWidth(r.width());
			p->fillRect( brushRect, bgColor );
			
			if (format->hasFrame()) {
				framePen = p->pen();
				framePen.setWidth( frameThickness );
				framePen.setColor( frameColor );
				p->setPen( framePen );
				
				p->drawRect( brushRect );
			}
			
			p->setPen(pen);
			p->drawText( boundingRect, arguments, currentText );
			m_pagePosition.rect.setY( m_pagePosition.rect.y() + boundingRect.height() + (format->hasFrame() ? 2*frameThickness : 0) + STYLE_PART_SPACE );
			
			printed = true;			
		}			
	}	
	if (printed)	//something was printed
		m_pagePosition.rect.setY( m_pagePosition.rect.y() + PARAGRAPH_SPACE );
}

/** Setups the printer using CPrinterDialog. */
void CPrinter::setup( QWidget* parent ){
	qDebug("CPrinterDialog::setup()");
	
	CPrinterDialog* dlg = new CPrinterDialog( this, parent, "printerdialog");
	dlg->exec();
	delete dlg;
}

/** Set to true if you want to open the printed things in the preview application. */
void CPrinter::setPreview( bool usePreview ){
	m_usePreview = usePreview;
}

/** Returns true if we use the preview mechanism. */
bool CPrinter::getPreview(){
	return m_usePreview;
}

/** draws a header on the page. */
void CPrinter::drawHeader(QPainter *paint, int sx, int width, int y, CPrinter::CHeaderType type, const CPrinter::CPageHeader header, const CPrinter::CPagePosition position){
}

/** Paints the header. */
void CPrinter::printHeader( QPainter* painter){

}

/** Prints the footer. */
void CPrinter::printFooter( QPainter* painter ){
}

/** Sets the status of the page header. */
void CPrinter::setPageHeader( bool enableHeader, CPageHeader header){

}

/** Sets the status of the page header. */
void CPrinter::setPageFooter( bool enableFooter, CPageHeader footer){
}

/** Starts printing the items. */
void CPrinter::printQueue(){
	qDebug("CPrinter::printQueue()");
	/** Go throgh the items of our print queue and print the items using the function printItem,
		* which takes care for margings, styles etc.
		*/
	if ( getPreview() ){//print a preview
		KRandomSequence r;
		setOutputFileName( QString("/tmp/") + KApplication::randomString(8)+".ps" );
	}

	emit printingStarted();
	QPainter *p = new QPainter();	
	if (!p->begin(this)) {
		clearQueue();
		return;	
	}
	
	m_pagePosition.rect = getPageSize();
	for (int page = 1; page <= numCopies(); page++) {	//make numCopies() copies of the pages
		for (m_queue->first(); m_queue->current(); m_queue->next()) {
			KApplication::kApplication()->processEvents(10); //do not lock the GUI!			
			printItem( p, m_queue->current());
			CKey* key = m_queue->current()->getStartKey();
			
			QString keyName = QString::null;			
			CSwordVerseKey* vk = dynamic_cast<CSwordVerseKey*>(key);
			if (vk) {
				keyName = vk->getKey();
			}
			else {			
				CSwordLDKey* lk = dynamic_cast<CSwordLDKey*>(key);
				keyName = lk->getKey();
			}
			emit printedOneItem(keyName, m_queue->at()+1);			
		};
		if (page < numCopies())
			newPage();	//new pages seperate copies
	}
	p->end();	//send away print job

	if ( getPreview() ) {
		KProcess process;
		process << getPreviewApplication();
		process << outputFileName();
		process.start(KProcess::DontCare);
//		connect(&process, SIGNAL(processExited(KProcess*)), SLOT(previewFinished(KProcess*)));		
	}	
	if (!getPreview())
		clearQueue();
	emit printingFinished();
}

/** Appends items to the printing queue. */
void CPrinter::appendItemsToQueue( printItemList* items ){
	qDebug("CPrinter::appendItemsToQueue( printItemList* items )");
	for(items->first(); items->current(); items->next()) {
		ASSERT( items->current() );
		m_queue->append(items->current());
	}
	if (m_queue->count() == 1)
		emit addedFirstQueueItem();	
}

/**  */
void CPrinter::clearQueue(){
	qDebug("CPrinter::clearQueue()");
	m_queue->clear();
}

/** Returns the print queue object. */
printItemList* CPrinter::getPrintQueue(){
	qDebug("CPrinter::getPrintQueue()");
	ASSERT(m_queue);
	return m_queue;
}

/** Sets the printing queue to queue. */
void CPrinter::setPrintQueue( printItemList* queue){
	qDebug("CPrinter::setPrintQueue( printItemList* queue)");
	if (queue != m_queue) { //delete old queue
		clearQueue();
		delete m_queue;
	}
	m_queue = queue;
}

/** Appends the item o the queue. */
void CPrinter::addItemToQueue( CPrintItem* newItem){
	qDebug("CPrinter::addItemToQueue( CPrintItem* newItem)");
	ASSERT(newItem);
	ASSERT(m_queue);	
	if (!newItem)
		return;
		
	for (m_styleList->first(); m_styleList->current(); m_styleList->next()) {
		if (m_styleList->current()->getStyleName() == i18n("Standard")) {
			newItem->setStyle(m_styleList->current());
		}
	}

	m_queue->append( newItem );	
	if (m_queue->count() == 1)
		emit addedFirstQueueItem();
}


/** Reads the style from config. */
void CPrinter::setupStyles(){
	qDebug("CPrinter::setupStyles()");
	// See function saveStyles for format of config file
	
	KConfigGroupSaver gs(config, "Styles");
	QStringList list = config->readListEntry("styles");
	CStyle* dummyStyle = 0;
	
	QString names[3];
	names[0] = "HEADER";
	names[1] = "DESCRIPTION";
	names[2] = "MODULETEXT";
	
	CStyle::styleType formatTypes[3];
	formatTypes[0] = CStyle::Header;
	formatTypes[1] = CStyle::Description;
	formatTypes[2] = CStyle::ModuleText;
							
	
	for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )	{
		dummyStyle = new CStyle();
		dummyStyle->setStyleName(*it);

		CStyleFormat* format[3];
		format[0] = dummyStyle->getFormatForType( CStyle::Header );
		format[1] = dummyStyle->getFormatForType( CStyle::Description );
		format[2] = dummyStyle->getFormatForType( CStyle::ModuleText );
			
		for (int index = 0; index < 3; index++) {
			config->setGroup(QString("%1__%2").arg(*it).arg(names[index]));
			format[index]->setFGColor( config->readColorEntry("FGColor", &Qt::black) );
			format[index]->setBGColor( config->readColorEntry("BGColor", &Qt::white) );			
 			format[index]->setFont( config->readFontEntry("Font") );
			format[index]->setIdentation( config->readNumEntry("Identation",0) );
			format[index]->setAlignement( (CStyleFormat::alignement)config->readNumEntry("Alignement",0));
			dummyStyle->setFormatTypeEnabled( formatTypes[index], config->readBoolEntry("isEnabled", true) );			
			const bool hasFrame = config->readBoolEntry( "has frame", false );
			
			CStyleFormatFrame* frame = format[index]->getFrame();			
			config->setGroup(QString("%1__%2__FRAME").arg(*it).arg(names[index]));
			frame->setColor( config->readColorEntry("Color", &Qt::black) );
			frame->setThickness( config->readNumEntry("Thickness", 1) );
			format[index]->setFrame( hasFrame, frame);
#warning implement reading of line style
		}
		
		//set settings for Header
		m_styleList->append(dummyStyle);
	}
}

/** Saves the styles to config file. */
void CPrinter::saveStyles(){
	qDebug("CPrinter::saveStyle()");
	/**
		* Format of styles in config file:
		*		[Styles]
		*		styles=name1,name2
		*	
		*		[name 1__Description]
		*		hasFrame=true
		*
		*		[name 1__HEADER]
		*		hasFrame=true
		*
		*		...
		*
		*/
	ASSERT(config);
	
	//save list of styles
	qDebug("save list of styles");
	{
		KConfigGroupSaver gs( config, "Styles");	
		QStringList strList;
		ASSERT(m_styleList);
		for (m_styleList->first(); m_styleList->current(); m_styleList->next()) {
			ASSERT(m_styleList);
			ASSERT(m_styleList->current());
			if (m_styleList->current())
				strList.append(m_styleList->current()->getStyleName());
		}	
		config->writeEntry( "styles", strList);			
		qDebug("wrote list of styles");
	}

	
	//save settings for each style
	qDebug("save style specific settings");
	QString names[3];
	names[0] = "HEADER";
	names[1] = "DESCRIPTION";
	names[2] = "MODULETEXT";
	
	for (m_styleList->first(); m_styleList->current(); m_styleList->next()) {
		qDebug("loop1");
		ASSERT(m_styleList->current());
		config->setGroup(m_styleList->current()->getStyleName());
		CStyle*	current = m_styleList->current();
				
		for (short int index = 0; index < 3; index++) {
			qDebug("loop2");
			config->setGroup(QString("%1__%2").arg(current->getStyleName()).arg(names[index]));

			CStyleFormat* format[3];
			format[0] = current->getFormatForType( CStyle::Header );
			format[1] = current->getFormatForType( CStyle::Description );
			format[2] = current->getFormatForType( CStyle::ModuleText );

			ASSERT(format[index]);
												
			config->writeEntry( "FGColor", format[index]->getFGColor() );
			config->writeEntry( "BGColor", format[index]->getBGColor() );
			config->writeEntry( "Font", format[index]->getFont() );
			config->writeEntry( "Identation", format[index]->getIdentation() );			
			config->writeEntry( "isEnabled", current->hasFormatTypeEnabled( (index == 0) ? CStyle::Header : ( (index == 1) ? CStyle::Description : CStyle::ModuleText)) );
			config->writeEntry( "Alignement", (int)(format[index]->getAlignement()) );
			//save frame settings
			config->writeEntry( "has frame", format[index]->hasFrame() );
			config->setGroup(QString("%1__%2__FRAME").arg(m_styleList->current()->getStyleName()).arg(names[index]) );
			if (format[index]->hasFrame()) {	//save only if we have a frame
				CStyleFormatFrame* frame = format[index]->getFrame();
				config->writeEntry("Color", frame->getColor() );
				config->writeEntry("Thickness", frame->getThickness());
				#warning Implement saving of line style
			}
		}
	}	
}

/**  */
void CPrinter::readSettings(){
	qDebug("CPrinter::readSettings()");
	KConfigGroupSaver gs(config, "Settings");	
	setFullPage(true);
	
	m_pagePosition.curPage = 1;
	m_pagePosition.rect = getPageSize();
	
	m_printIntoFile = config->readBoolEntry("Print to file", false);
	m_filename = config->readEntry("Filename", QString::null);
	setNumCopies(1);
	setPageSize( (QPrinter::PageSize)config->readNumEntry("Paper size", 4) );	//default is A4
			
	setupStyles();
}

/**  */
void CPrinter::saveSettings(){
	qDebug("CPrinter::saveSettings()");
	
	config->setGroup("Settings");
	config->writeEntry("Paper size", (int)pageSize());
	
	saveStyles();
}

/** Returns the list of styles. */
styleItemList* CPrinter::getStyleList(){
	qDebug("CPrinter::getStyleList()");
	ASSERT(m_styleList);
	return m_styleList;
}

/** Sets the application wide style list to list. */
void CPrinter::setStyleList( styleItemList* list){
	qDebug("CPrinter::setStyleList( styleItemList* list)");
	ASSERT(list);
	m_styleList = list;
}

/** Returns the page size without headers. */
QRect CPrinter::getPageSize(){
	qDebug("CPrinter::getPageSize()");
  QPaintDeviceMetrics metric( this );
  QRect r;

  CPageSize size;
	size.width  = metric.width();
  size.height = metric.height();

  r.setLeft( m_pageMargin.left );
  r.setTop( m_pageMargin.top );
  r.setRight( metric.width() -  m_pageMargin.right );
  r.setBottom( metric.height() - m_pageMargin.top - m_pageMargin.bottom );

  return r;
}

/** Returns the config used for this printer object. */
KConfig* CPrinter::getConfig() {
	ASSERT(config);
	return config;	
}

/** Creates the standard style. */
void CPrinter::setupStandardStyle(){
	qDebug("CPrinter::setupStandardStyle()");
	
	//see if m_items contains standard style
	bool found = false;
	for (m_styleList->first(); m_styleList->current(); m_styleList->next()) {
		if (m_styleList->current()->getStyleName() == i18n("Standard")) {	//found the style
			found = true;
			break;
		}
	}
	
	if (!found) {
		CStyle* standardStyle = new CStyle();		
		standardStyle->setStyleName(i18n("Standard"));		
		m_styleList->append( standardStyle );
	}
}

/** Is called after the preview application was closed. */
void CPrinter::previewFinished(KProcess*){
	if (QFile::exists( outputFileName() ))
		QFile::remove( outputFileName() );
}
