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
#include "cprintitem.h"
#include "cstyleformat.h"
#include "cstyleformatframe.h"
#include "cprintdialogpages.h"

#include "../backend/cswordbackend.h"
#include "../backend/cswordversekey.h"
#include "../backend/cswordldkey.h"

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

CPrinter::CPrinter( QObject* parent ) : QObject(parent) {
	config = new KConfig("bt-printing", false, true );

	m_queue = new printItemList;	
	m_queue->setAutoDelete(true);	
		
	m_styleList = new styleItemList;
	m_styleList->setAutoDelete(true);		

	m_cachedPage.initialized = false;
	m_cachedPage.refresh = false;
	
	m_addedItem = false;
	
	{
		KConfigGroupSaver gs(config, "Options");	
		QMap<QString, QString> map = config->entryMap("Options");
		setOptions(map);
	}
	readSettings();
	setupStyles();
	setupStandardStyle();		
}

CPrinter::~CPrinter(){
	saveSettings();
	saveStyles();	
	config->sync();		
	delete config;		
	if (m_queue)
		delete m_queue;
	delete m_styleList;
}

const unsigned int CPrinter::rightMargin() const {
	return m_pageMargin.right;
}

const unsigned int CPrinter::leftMargin() const {
	return m_pageMargin.left;
}

const unsigned int CPrinter::upperMargin() const {
	return m_pageMargin.top;
}

const unsigned int CPrinter::lowerMargin() const {
	return m_pageMargin.bottom;
}

/** Appends a new page where the next things will be painted. */
const bool CPrinter::newPage(){
	if (aborted()) {
		qWarning("CPrinter::newPage: Printing was aborted!");
		return false;
	}
	const bool result = KPrinter::newPage();
	if (result) {
		m_pagePosition.curPage++;
	 	m_pagePosition.rect = getPageSize();			
	}
	return result;
}

/** Sets all the margins at one time. */
void CPrinter::setAllMargins( const CPageMargin margins ) {
	m_cachedPage.refresh = true;	
	m_pageMargin = margins;
}

/** Returns the margins of the pages. */
CPrinter::CPageMargin CPrinter::getPageMargins(){
	return m_pageMargin;
}

/** Setups the printer using CPrinterDialog. */
void CPrinter::setup( QWidget* parent ){
	CPrintItemListPage* printItemPage = new CPrintItemListPage(this);
	KPrinter::addDialogPage(printItemPage);

	CStyleListPage* stylePage = new CStyleListPage(this);
	KPrinter::addDialogPage(stylePage);

	if ( KPrinter::setup(parent) ) {
		saveSettings();
		readSettings();
		printQueue();
	}
}

/** Starts printing the items. */
void CPrinter::printQueue(){
	emit printingStarted();
	QPainter p;
	if (!p.begin(this)) {
		p.end();
		return;
	}	
	int lastPercent = 0;
	int percent = 0;
	int pos = 1;
	const int count = m_queue->count();
	const int copies = numCopies();
	float copyFrac;
	
	for (int copy = 0; copy < copies && !aborted(); copy++) {	//make numCopies() copies of the pages
		copyFrac = (float(copies))/ (float)(copy+1);
		for (m_queue->first(), pos = 1; m_queue->current(); m_queue->next(), ++pos) {
			KApplication::kApplication()->processEvents(5); //do not lock the GUI!
			if (!aborted()) {
				m_queue->current()->draw(&p,this);				
				if ((int)((float)pos / (float)count *(float)100 * copyFrac) > lastPercent) {
					emit percentCompleted(++lastPercent);
				}
			}
		};
		if (!aborted() && (copy+1 < copies) ) {
			newPage();	//new pages seperate copies
		}
	}
	emit printingFinished();
	
	clearQueue();//delete all items
	m_addedItem = false;//queue is empty
}

/** Appends items to the printing queue. */
void CPrinter::appendItemsToQueue( printItemList* items ){
	for(items->first(); items->current(); items->next()) {
		m_queue->append(items->current());
	}
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
	if (!newItem)
		return;
	newItem->setStyle(m_standardStyle);
	m_queue->append(newItem);
	if (!m_addedItem) {
		m_addedItem = true;
		emit addedFirstQueueItem();
	}
}

/** Reads the style from config. */
void CPrinter::setupStyles(){
	// See function saveStyles for format of config file	
	KConfigGroupSaver gs(config, "Styles");
	QStringList list = config->readListEntry("styles");
	CStyle* dummyStyle = 0;
	
	const	QString names[3] = { "HEADER", "DESCRIPTION", "MODULETEXT" };
	const CStyle::styleType formatTypes[3] = {
		CStyle::Header,
		CStyle::Description,
		CStyle::ModuleText
	};
							
	
	for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it )	{
		dummyStyle = new CStyle();
		if (*it == "Standard")
			dummyStyle->setStyleName(i18n("Standard"));		
		else
			dummyStyle->setStyleName(*it);
		

		CStyleFormat* format[3] = {
			dummyStyle->getFormatForType( CStyle::Header ),
		 	dummyStyle->getFormatForType( CStyle::Description ),
		 	dummyStyle->getFormatForType( CStyle::ModuleText )
		};
			
		for (int index = 0; index < 3; index++) {
			config->setGroup(QString("%1__%2").arg(*it).arg(names[index]));
			format[index]->setFGColor( config->readColorEntry("FGColor", &Qt::black) );
			format[index]->setBGColor( config->readColorEntry("BGColor", &Qt::white) );
 			format[index]->setFont( config->readFontEntry("Font") );
			format[index]->setIdentation( config->readNumEntry("Identation",0) );
			format[index]->setAlignement( (CStyleFormat::alignement)config->readNumEntry("Alignement",CStyleFormat::Left));
			dummyStyle->setFormatTypeEnabled( formatTypes[index], config->readBoolEntry("isEnabled", true) );
			const bool hasFrame = config->readBoolEntry( "has frame", false );
			
			CStyleFormatFrame* frame = format[index]->getFrame();
			if (frame) {
				config->setGroup(QString("%1__%2__FRAME").arg(*it).arg(names[index]));
				frame->setColor( config->readColorEntry("Color", &Qt::black) );
				frame->setThickness( config->readNumEntry("Thickness", 1) );
				frame->setLineStyle( (Qt::PenStyle)(config->readNumEntry("Line style", (int)Qt::SolidLine)) );				
			}
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
	//save list of styles
	{
		KConfigGroupSaver gs( config, "Styles");	
		QStringList strList;
		ASSERT(m_styleList);
		for (m_styleList->first(); m_styleList->current(); m_styleList->next()) {
			strList.append(m_styleList->current()->getStyleName());
		}	
		config->writeEntry( "styles", strList);			
	}

	
	//save settings for each style
	const QString names[3] = {"HEADER", "DESCRIPTION", "MODULETEXT"};
	
	for (m_styleList->first(); m_styleList->current(); m_styleList->next()) {
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
				config->writeEntry("Line style", (int)(frame->getLineStyle()));
			}
		}
	}	
}

/**  */
void CPrinter::readSettings(){
	KConfigGroupSaver gs(config, "Settings");
	const QString leading = "kde-bibletime-";
		
	setFullPage(true);
	m_pagePosition.curPage = 1;
	m_pagePosition.rect = getPageSize();

	QPaintDeviceMetrics m(this);
	const float r = (float)m.width() / m.widthMM();		
	m_pageMargin.left 	= (int)(r * config->readNumEntry("Left margin", 15));
	setOption(leading+"left_margin", QString::number(config->readNumEntry("Left margin", 15)));
	
	m_pageMargin.right 	= (int)(r * config->readNumEntry("Right margin", 15));
	setOption(leading+"right_margin", QString::number(config->readNumEntry("Right margin", 15)));
		
	m_pageMargin.top 		= (int)(r * config->readNumEntry("Top margin", 15));
 	setOption(leading+"upper_margin", QString::number(config->readNumEntry("Top margin", 15)));
		
	m_pageMargin.bottom = (int)(r * config->readNumEntry("Bottom margin", 15));
	setOption(leading+"lower_margin", QString::number(config->readNumEntry("Bottom margin", 15)));		
	
	m_cachedPage.refresh = true;	
}

/**  */
void CPrinter::saveSettings(){	
	const QString leading = "kde-bibletime-";
	KConfigGroupSaver gs(config, "Settings");
	config->writeEntry("Left margin", option(leading+"left_margin").toInt()/*m_pagePosition.left*/);
	config->writeEntry("Right margin", option(leading+"right_margin").toInt() /*m_pagePosition.right*/);
	config->writeEntry("Top margin", option(leading+"upper_margin").toInt()/*m_pagePosition.top*/);
	config->writeEntry("Bottom margin", option(leading+"lower_margin").toInt()/*m_pagePosition.bottom*/);
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
const QRect CPrinter::getPageSize() {
  if 	(	m_cachedPage.refresh || !m_cachedPage.initialized || (m_cachedPage.initialized && (m_cachedPage.cachedPaper != pageSize())) )
  { //refresh page size info
		m_cachedPage.initialized = true;
		m_cachedPage.refresh = false;
		m_cachedPage.cachedPaper = pageSize();		
		
	  QPaintDeviceMetrics metric( this ); //note that metric's width and height span the whole page				
	  m_cachedPage.size.setLeft( m_pageMargin.left );
	  m_cachedPage.size.setTop( m_pageMargin.top );
	  m_cachedPage.size.setRight( metric.width() -  m_pageMargin.right );
	  m_cachedPage.size.setBottom( metric.height() - m_pageMargin.top - m_pageMargin.bottom );
	}
  return m_cachedPage.size;
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
			m_standardStyle = m_styleList->current();
			break;
		}
	}
		
	if (!found) {
		m_standardStyle = new CStyle();		
		m_standardStyle->setStyleName(i18n("Standard"));		
		m_styleList->append( m_standardStyle );
	}
}

/** returns the vertical position of the printer's painter. */
const int CPrinter::getVerticalPos() const {
	return m_pagePosition.rect.y();
}

/** Sets the vertical position of the printer's painter. */
void CPrinter::setVerticalPos( const int yPos ){
	m_pagePosition.rect.setY(yPos);
}

/** Emits the signal that the styles changed. */
void CPrinter::emitStylesChanged(){
	emit sigStylesChanged();
}

/** Emits the signal that the styles changed. */
CStyle* CPrinter::standardStyle() const {
	return m_standardStyle;
}
