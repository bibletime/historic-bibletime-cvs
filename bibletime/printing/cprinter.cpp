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

//KDE includes
#include <kconfig.h>
#include <kprocess.h>
#include <kconfigbase.h>
#include <krandomsequence.h>
#include <klocale.h>
#include <kapp.h>

//Qt includes
#include <qfile.h>
#include <qstringlist.h>
#include <qpainter.h>
#include <qpaintdevice.h>
#include <qpaintdevicemetrics.h>

CPrinter::CPrinter( CImportantClasses* important, QObject* parent ) : QObject(parent) {
	config = new KConfig("bt-printing", false, true );

	m_important = important;
	m_backend = m_important->swordBackend;
		
	m_queue = new printItemList;	
	m_queue->setAutoDelete(true);	
	
	m_styleList = new styleItemList;
	m_styleList->setAutoDelete(true);		
			
	readSettings();
	setupStyles();
	setupStandardStyle();		
}

CPrinter::~CPrinter(){
	saveSettings();	
	config->sync();		
	delete config;	
	if (m_queue)
		delete m_queue;
	delete m_styleList;
	
	for ( QStringList::Iterator it = m_createdFiles.begin(); it != m_createdFiles.end(); ++it ) {
		if (QFile::exists(*it))
			QFile::remove(*it);
	}
}

/** Returns the path to the preview application. */
const QString& CPrinter::getPreviewApplication() const {
	return m_previewApplication;
}

/**  */
void CPrinter::setPreviewApplication( const QString& app){
	m_previewApplication = app;
}

/** Returns the right margin. */
const unsigned int CPrinter::rightMarginMM() const {
	QPaintDeviceMetrics m(this);
	const float r = (float)m.width() / m.widthMM();	
	return (int)((float)m_pageMargin.right / r);
}

const unsigned int CPrinter::rightMargin() const {
	return m_pageMargin.right;
}

/** Sets the right margin. */
void CPrinter::setRightMarginMM( const unsigned int margin ){
	QPaintDeviceMetrics m(this);
	const float r = (float)m.width() / m.widthMM();
	
	m_pageMargin.right = (int)((float)margin*r);
}

/** Returns the left margin. */
const unsigned int CPrinter::leftMarginMM() const {
	QPaintDeviceMetrics m(this);
	const float r = (float)m.width() / m.widthMM();
	
	return (int)((float)m_pageMargin.left / r);
}

const unsigned int CPrinter::leftMargin() const {
	return m_pageMargin.left;
}

/** Sets the left margin. */
void CPrinter::setLeftMarginMM( const unsigned int margin ){
	QPaintDeviceMetrics m(this);
	const float r = (float)m.width() / m.widthMM();
	
	m_pageMargin.left = (int)((float)margin*r);
}

/** Returns the right margin. */
const unsigned int CPrinter::upperMarginMM() const {
	QPaintDeviceMetrics m(this);
	const float r = (float)m.width() / m.widthMM();
	
	return (int)((float)m_pageMargin.top / r);
}

const unsigned int CPrinter::upperMargin() const {
	return m_pageMargin.top;
}

/** Sets the right margin. */
void CPrinter::setUpperMarginMM( const unsigned int margin ){
	QPaintDeviceMetrics m(this);
	const float r = (float)m.width() / m.widthMM();
	
	m_pageMargin.top = (int)((float)margin*r);
}

/** Returns the right margin. */
const unsigned int CPrinter::lowerMarginMM() const {
	QPaintDeviceMetrics m(this);
	const float r = (float)m.width() / m.widthMM();
	
	return (int)((float)m_pageMargin.bottom / r);
}

const unsigned int CPrinter::lowerMargin() const {
	return m_pageMargin.bottom;
}

/** Sets the right margin. */
void CPrinter::setLowerMarginMM( const unsigned int margin ){
	QPaintDeviceMetrics m(this);
	const float r = (float)m.width() / m.widthMM();
	
	m_pageMargin.bottom = (int)((float)margin*r);
}

/** Appends a new page where the next things will be painted. */
const bool CPrinter::newPage(){
	if (aborted()) {
		qWarning("CPrinter::newPage: Printing was aborted!");
		return false;
	}
	bool result = QPrinter::newPage();
	if (result) {
		m_pagePosition.curPage++;
	 	m_pagePosition.rect = getPageSize();			
	}
	return result;
}

/** Sets all the margins at one time. */
void CPrinter::setAllMargins( const CPageMargin margins ) {
	m_pageMargin = margins;
}

/** Returns the margins of the pages. */
CPrinter::CPageMargin CPrinter::getPageMargins(){
	return m_pageMargin;
}

/** Setups the printer using CPrinterDialog. */
void CPrinter::setup( QWidget* parent ){
	CPrinterDialog* dlg = new CPrinterDialog( this, parent, "printerdialog");
	dlg->exec();
	delete dlg;
}

/** Set to true if you want to open the printed things in the preview application. */
void CPrinter::setPreview( const bool usePreview ){
	m_usePreview = usePreview;
}

/** Returns true if we use the preview mechanism. */
bool CPrinter::getPreview(){
	return m_usePreview;
}

/** draws a header on the page. */
void CPrinter::drawHeader(QPainter */*paint*/, const int /*sx*/, const int /*width*/, const int /*y*/, const CPrinter::CHeaderType /*type*/, const CPrinter::CPageHeader /*header*/, const CPrinter::CPagePosition /*position*/){
}

/** Paints the header. */
void CPrinter::printHeader( QPainter* /*painter*/ ){

}

/** Prints the footer. */
void CPrinter::printFooter( QPainter* /*painter*/ ){
}

/** Sets the status of the page header. */
void CPrinter::setPageHeader( const bool /*enableHeader*/, CPageHeader /*header*/){

}

/** Sets the status of the page header. */
void CPrinter::setPageFooter( const bool /*enableFooter*/, CPageHeader /*footer*/){
}

/** Starts printing the items. */
void CPrinter::printQueue(){
	qDebug("CPrinter::printQueue()");
	/**
	* Go throgh the items of our print queue and print the items using the function printItem,
	* which takes care for margings, styles etc.
	*/
	if ( getPreview() ){//print a preview
		KRandomSequence r;
		const QString s = QString::fromLatin1("/tmp/") + KApplication::randomString(8) + QString::fromLatin1(".ps");
		qDebug("CPrinter: set filename for preview");
		setOutputFileName( s );
		m_createdFiles.append(s);
	}
	qDebug("emit printingStarted");
	emit printingStarted();
	QPainter p;
	if (!p.begin(this)) {
		qDebug("begin failed");
		p.end();
		return;
	}
	
	m_pagePosition.rect = getPageSize();
	for (int page = 1; page <= numCopies() && !aborted(); ++page) {	//make numCopies() copies of the pages
		qDebug("begin new page");
		for (m_queue->first(); m_queue->current() && !aborted(); m_queue->next()) {
			qDebug("inner loop");
			KApplication::kApplication()->processEvents(10); //do not lock the GUI!
			ASSERT(!aborted());
			if (!aborted()) {
				qDebug("call now m_queue->current()->draw(&p,this);");
				m_queue->current()->draw(&p,this);
			}
			QString keyName = QString::null;
			
			CKey* key = m_queue->current()->getStartKey();			
			CSwordVerseKey* vk = dynamic_cast<CSwordVerseKey*>(key);
			CSwordLDKey* lk = dynamic_cast<CSwordLDKey*>(key);			
			ASSERT(vk);
			ASSERT(lk);			
			if (vk)
				keyName = vk->getKey();
			else if (lk)
				keyName = lk->getKey();
			else
				keyName = QString::null;
			ASSERT(!aborted());
			if (!aborted()) {
				qDebug("emit printedOneItem");
				emit printedOneItem(keyName, m_queue->at()+1);
			}
		};
		if (!aborted() && (page < numCopies()) )
			newPage();	//new pages seperate copies
	}
	qDebug("emit finished");
	emit printingFinished();	
	if (!getPreview())
		clearQueue();
		
	if ( !aborted() && getPreview() ) {
		if (p.isActive())
			p.end();
		qDebug("start preview now");
		KProcess process;
		process << getPreviewApplication();
		process << outputFileName();
		process.start(KProcess::DontCare);
	}	
	qDebug("finished printQueue");
}

/** Appends items to the printing queue. */
void CPrinter::appendItemsToQueue( printItemList* items ){
	for(items->first(); items->current(); items->next()) {
		ASSERT( items->current() );
		m_queue->append(items->current());
	}
	if (m_queue->count() == 1)
		emit addedFirstQueueItem();	
}

/**  */
void CPrinter::clearQueue(){
	m_queue->clear();
	emit queueCleared();	
}

/** Returns the print queue object. */
printItemList* CPrinter::getPrintQueue() const {
	return m_queue;
}

/** Sets the printing queue to queue. */
void CPrinter::setPrintQueue( printItemList* queue ){
	if (queue != m_queue) { //delete old queue
		clearQueue();
		delete m_queue;
	}
	m_queue = queue;
}

/** Appends the item o the queue. */
void CPrinter::addItemToQueue(CPrintItem* newItem){
	qDebug("CPrinter::addItemToQueue(CPrintItem* newItem)");
	ASSERT(m_styleList);
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
		if (*it == "Standard")
			dummyStyle->setStyleName(i18n("Standard"));		
		else
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
		}		
		//set settings for Header
		m_styleList->append(dummyStyle);
	}
}

/** Saves the styles to config file. */
void CPrinter::saveStyles(){
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
	}

	
	//save settings for each style
	QString names[3];
	names[0] = "HEADER";
	names[1] = "DESCRIPTION";
	names[2] = "MODULETEXT";
	
	for (m_styleList->first(); m_styleList->current(); m_styleList->next()) {
		ASSERT(m_styleList->current());
		config->setGroup(m_styleList->current()->getStyleName());
		CStyle*	current = m_styleList->current();
				
		for (short int index = 0; index < 3; index++) {
			config->setGroup(QString("%1__%2").arg(current->getStyleName()).arg(names[index]));

			CStyleFormat* format[3];
			format[0] = current->getFormatForType( CStyle::Header );
			format[1] = current->getFormatForType( CStyle::Description );
			format[2] = current->getFormatForType( CStyle::ModuleText );
												
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
			}
		}
	}	
}

/**  */
void CPrinter::readSettings(){
	KConfigGroupSaver gs(config, "Settings");	
	setFullPage(true);
	
	setPrinterName(config->readEntry("Printer", "lp"));		
	
	m_pagePosition.curPage = 1;
	m_pagePosition.rect = getPageSize();

	QPaintDeviceMetrics m(this);
	const float r = (float)m.width() / m.widthMM();		
	m_pageMargin.left 	= config->readNumEntry("left margin", 15*r);
	m_pageMargin.right 	= config->readNumEntry("right margin",15*r);
	m_pageMargin.top 		= config->readNumEntry("upper margin",15*r);
	m_pageMargin.bottom	= config->readNumEntry("lower margin",15*r);
	
	m_printIntoFile = config->readBoolEntry("Print to file", false);
	m_filename = config->readEntry("Filename", QString::null);
	
	setNumCopies(1);
	
	setPageSize( (QPrinter::PageSize)config->readNumEntry("Paper size", 5) );	//default is A4
	setPreviewApplication( config->readEntry("preview application", "kghostview") );
}

/**  */
void CPrinter::saveSettings(){
	config->setGroup("Settings");
	config->writeEntry("Filename", m_filename);	
	config->writeEntry("Paper size", (int)pageSize());
	config->writeEntry("Printer", printerName());
	config->writeEntry("upper margin", m_pageMargin.top);
	config->writeEntry("lower margin", m_pageMargin.bottom);	
	config->writeEntry("left margin", m_pageMargin.left);
	config->writeEntry("right margin", m_pageMargin.right);	
	config->writeEntry("preview application", getPreviewApplication());

	saveStyles();
}

/** Returns the list of styles. */
styleItemList* CPrinter::getStyleList() const {
	return m_styleList;
}

/** Sets the application wide style list to list. */
void CPrinter::setStyleList( styleItemList* list){
	m_styleList = list;
}

/** Returns the page size without headers. */
const QRect CPrinter::getPageSize() const {
  QPaintDeviceMetrics metric( this );
  QRect r;

  r.setLeft( m_pageMargin.left );
  r.setTop( m_pageMargin.top );
  r.setRight( metric.width() -  m_pageMargin.right );
  r.setBottom( metric.height() - m_pageMargin.top - m_pageMargin.bottom );

  return r;
}

/** Returns the config used for this printer object. */
KConfig* CPrinter::getConfig() {
	return config;	
}

/** Creates the standard style. */
void CPrinter::setupStandardStyle(){
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
/** returns the vertical position of the printer's painter. */
const int CPrinter::getVerticalPos() const {
	return m_pagePosition.rect.y();
}

/** Sets the vertical position of the printer's painter. */
void CPrinter::setVerticalPos( const int yPos ){
	m_pagePosition.rect.setY(yPos);
}
