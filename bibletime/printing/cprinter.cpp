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
#include "cprintdialogpages.h"

#include "backend/cswordbackend.h"
#include "backend/cswordversekey.h"
#include "backend/cswordldkey.h"

//KDE includes
#include <kconfig.h>
#include <kprocess.h>
#include <kconfigbase.h>
#include <kstandarddirs.h>
#include <klocale.h>
#include <kapplication.h>

//Qt includes
#include <qfile.h>
#include <qdir.h>
#include <qstringlist.h>
#include <qpainter.h>
#include <qpaintdevice.h>
#include <qpaintdevicemetrics.h>


CPrinter::CPrinter( QObject* parent ) : QObject(parent) {
  m_styleData.standardStyle = QString::fromLatin1(\
"<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE DOC ><BibleTimePrintingStyle syntaxVersion=\"1\" name=\"%1\">\
 <HEADER alignment=\"0\" enabled=\"1\" >\
  <COLORS bgcolor=\"#ffffff\" fgcolor=\"#000000\" />\
  <FONT family=\"\" weight=\"75\" pointsize=\"12\" italic=\"0\" />\
  <FRAME enabled=\"0\" />\
 </HEADER>\
 <DESCRIPTION alignment=\"0\" enabled=\"1\" >\
  <COLORS bgcolor=\"#ffffff\" fgcolor=\"#000000\" />\
  <FONT family=\"\" weight=\"50\" pointsize=\"10\" italic=\"0\" />\
  <FRAME enabled=\"0\" />\
 </DESCRIPTION>\
 <MODULETEXT alignment=\"0\" enabled=\"1\" >\
  <COLORS bgcolor=\"#ffffff\" fgcolor=\"#000000\" /> \
  <FONT family=\"\" weight=\"50\" pointsize=\"10\" italic=\"0\" />\
  <FRAME enabled=\"0\" />\
 </MODULETEXT>\
</BibleTimePrintingStyle>").arg(i18n("Standard"));

  m_styleData.bwStyle = QString::fromLatin1(\
"<?xml version=\"1.0\" encoding=\"UTF-8\"?><!DOCTYPE DOC ><BibleTimePrintingStyle syntaxVersion=\"1\" name=\"%1\" >\
 <HEADER alignment=\"1\" enabled=\"1\" >\
  <COLORS bgcolor=\"#ffffff\" fgcolor=\"#000000\" />\
  <FONT family=\"\" weight=\"75\" pointsize=\"12\" italic=\"0\" />\
  <FRAME thickness=\"1\" style=\"1\" color=\"#000000\" enabled=\"1\" />\
 </HEADER>\
 <DESCRIPTION alignment=\"1\" enabled=\"1\" >\
  <COLORS bgcolor=\"#ffffff\" fgcolor=\"#000000\" />\
  <FONT family=\"\" weight=\"50\" pointsize=\"10\" italic=\"1\" />\
  <FRAME enabled=\"0\" />\
 </DESCRIPTION>\
 <MODULETEXT alignment=\"0\" enabled=\"1\" >\
  <COLORS bgcolor=\"#ffffff\" fgcolor=\"#000000\" />\
  <FONT family=\"\" weight=\"50\" pointsize=\"10\" italic=\"0\" />\
  <FRAME thickness=\"1\" style=\"1\" color=\"#000000\" enabled=\"1\" />\
 </MODULETEXT>\
</BibleTimePrintingStyle>").arg(i18n("Black&amp;White for larger text portions"));


	m_config = new KConfig("bt-printing", false, true );

	m_queue.setAutoDelete(true);			
	m_styleList.setAutoDelete(true);		
	m_cachedPage.initialized = false;
	m_cachedPage.refresh = false;	
	m_addedItem = false;
	
	{
		KConfigGroupSaver gs(m_config, "Options");	
		QMap<QString, QString> map = m_config->entryMap("Options");
		setOptions(map);
	}
	
	m_styleDir = "printing/";
	KStandardDirs stdDirs;
	m_styleSaveLocation = stdDirs.saveLocation("data", "bibletime/"+m_styleDir);		
	
	readSettings();
	setupStyles();
	setupStandardStyle();		
}

CPrinter::~CPrinter(){
	saveSettings();
	saveStyles();	
	m_config->sync();
			
	delete m_config;
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
	 	m_pagePosition.rect = contentSize();			
	 	setVerticalPos(pageMargins().top);
	}
	return result;
}

/** Sets all the margins at one time. */
void CPrinter::setAllMargins( const CPageMargin margins ) {
	m_cachedPage.refresh = true;	
	m_pageMargin = margins;
}

/** Returns the margins of the pages. */
const CPrinter::CPageMargin& CPrinter::pageMargins() const {
	return m_pageMargin;
}

/** Setups the printer using CPrinterDialog. */
void CPrinter::setup( QWidget* parent ){
	KPrinter::addDialogPage( new CPrintItemListPage(this) );
	KPrinter::addDialogPage( new CStyleListPage(this) );

	if ( KPrinter::setup(parent) ) {
		saveSettings();
		readSettings();
		print();
	}
}

/** Starts printing the items. */
void CPrinter::print(){
	emit printingStarted();
	QPainter p;
	if (!p.begin(this)) {
		p.end();
		return;
	}	
	int lastPercent = 0;
	int pos = 1;
	const int count = m_queue.count();
	const int copies = numCopies();
	float copyFrac;
	emit percentCompleted(0);
	
	for (int copy = 0; copy < copies && !aborted(); copy++) {	//make numCopies() copies of the pages
		copyFrac = (float(copies)) / (float)(copy+1);
		for (m_queue.first(), pos = 1; m_queue.current(); m_queue.next(), ++pos) {
			KApplication::kApplication()->processEvents(5); //do not lock the GUI!
			if (!aborted()) {
				m_queue.current()->draw(&p,this);				
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
}

/** Appends items to the printing queue. */
void CPrinter::appendItems( ListCPrintItem& items ){
	for(items.first(); items.current(); items.next()) {
		items.current()->setStyle(m_standardStyle);
		m_queue.append(items.current());
	}
	if (items.count() && !m_addedItem) {
		m_addedItem = true;
		emit addedFirstQueueItem();	
	}
}

/**  */
void CPrinter::clearQueue(){
	m_queue.clear();
	m_addedItem = false;//queue is empty	
	emit queueCleared();	
}

/** Returns the print queue object. */
ListCPrintItem& CPrinter::printQueue() {
	return m_queue;
}

/** Sets the printing queue to queue. */
void CPrinter::setPrintQueue( ListCPrintItem& queue ){
	clearQueue();
	m_queue = queue; //copy items
}

/** Appends the item o the queue. */
void CPrinter::appendItem(CPrintItem* newItem){
	if (!newItem)
		return;
	newItem->setStyle(m_standardStyle);
	m_queue.append(newItem);

	if (!m_addedItem) {
		m_addedItem = true;
		emit addedFirstQueueItem();
	}
}

/** Reads the style from config. */
void CPrinter::setupStyles(){	
//load local styles	
	QDir d( m_styleSaveLocation );
	QStringList files = d.entryList("*.xml");
  CStyle* style = 0;
	for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
		m_styleList.append( style = new CStyle() ); //automatically load from file		
    style->loadFromFile(m_styleSaveLocation + *it);
	}
	
//load styles included in this source file!
  if (CStyle* newStyle = new CStyle(m_styleData.standardStyle)) {
 		bool found = false;				
 		for(m_styleList.first(); m_styleList.current() && !found; m_styleList.next()) {
 			if (newStyle->styleName() == m_styleList.current()->styleName()) {
 				found = true;
 			}
 		}		
				
 		if (!found) {
 			m_styleList.append( newStyle );
 		}
     else {
       delete newStyle;
     };
  };

  if (CStyle* newStyle = new CStyle(m_styleData.bwStyle)) {
 		bool found = false;				
 		for(m_styleList.first(); m_styleList.current() && !found; m_styleList.next()) {
 			if (newStyle->styleName() == m_styleList.current()->styleName()) {
 				found = true;
 			}
 		}		
				
 		if (!found) {
 			m_styleList.append( newStyle );
 		}
     else {
       delete newStyle;
     };
  };


//load systemwide styles, probably standard styles installed by BibleTime
	KStandardDirs stdDirs;
	QStringList globalPaths = stdDirs.findDirs("data", "bibletime/"+m_styleDir);
	if (globalPaths.count()) { //try to find some new global styles	
		for (QStringList::Iterator path = globalPaths.begin(); path!=globalPaths.end(); ++path) {
			d = QDir( *path );
			QStringList files = d.entryList("*.xml");
			for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
				CStyle* newStyle = new CStyle();
        newStyle->loadFromFile(*path + *it);
				
				bool found = false;				
				for(m_styleList.first(); m_styleList.current() && !found; m_styleList.next()) {
					if (newStyle->styleName() == m_styleList.current()->styleName()) {
						found = true;
					}
				}		
				
				if (!found) {
					m_styleList.append( newStyle );
				}
        else {
          delete newStyle;
        };
			}		
		}
	}
}

/** Saves the styles to config file. */
void CPrinter::saveStyles(){
	QDir d(m_styleSaveLocation);	
	QStringList files = d.entryList("*.xml");
	for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
		d.remove(*it);
	}
	
	for (m_styleList.first(); m_styleList.current(); m_styleList.next()) {
		m_styleList.current()->saveToFile(  m_styleSaveLocation + QString::fromLatin1("printing-style-%1").arg(m_styleList.at()) + ".xml" );
	}
}

/**  */
void CPrinter::readSettings(){
	KConfigGroupSaver gs(m_config, "Settings");
	const QString leading = "kde-bibletime-";
		
	setFullPage(true);
	m_pagePosition.curPage = 1;
	m_pagePosition.rect = contentSize();

	QPaintDeviceMetrics m(this);
	const float r = static_cast<float>(m.width()) / m.widthMM();		
	m_pageMargin.left 	= (int)(r * m_config->readNumEntry("Left margin", 15));
	setOption(leading+"left_margin", QString::number(m_config->readNumEntry("Left margin", 15)));
	
	m_pageMargin.right 	= (int)(r * m_config->readNumEntry("Right margin", 15));
	setOption(leading+"right_margin", QString::number(m_config->readNumEntry("Right margin", 15)));
		
	m_pageMargin.top 		= (int)(r * m_config->readNumEntry("Top margin", 15));
 	setOption(leading+"upper_margin", QString::number(m_config->readNumEntry("Top margin", 15)));
		
	m_pageMargin.bottom = (int)(r * m_config->readNumEntry("Bottom margin", 15));
	setOption(leading+"lower_margin", QString::number(m_config->readNumEntry("Bottom margin", 15)));		
	
	m_cachedPage.refresh = true;	
}

/**  */
void CPrinter::saveSettings(){	
	const QString leading = "kde-bibletime-";
	KConfigGroupSaver gs(m_config, "Settings");
	m_config->writeEntry("Left margin", option(leading+"left_margin").toInt());
	m_config->writeEntry("Right margin", option(leading+"right_margin").toInt());
	m_config->writeEntry("Top margin", option(leading+"upper_margin").toInt());
	m_config->writeEntry("Bottom margin", option(leading+"lower_margin").toInt());
}

/** Returns the list of styles. */
StyleItemList& CPrinter::styleList() {
	return m_styleList;
}

/** Sets the application wide style list to list. */
void CPrinter::setStyleList( StyleItemList& list){
	m_styleList = list; //copy items
}

/** Returns the page size without headers. */
const QRect CPrinter::contentSize() {
  if 	(	m_cachedPage.refresh || !m_cachedPage.initialized || (m_cachedPage.initialized && (m_cachedPage.cachedPaper != pageSize())) )
  { //refresh page size info
		m_cachedPage.initialized = true;
		m_cachedPage.refresh = false;
		m_cachedPage.cachedPaper = pageSize();		
		
	  QPaintDeviceMetrics metric( this ); //note that metric's width and height span the whole page				
	  m_cachedPage.size.setLeft( m_pageMargin.left );
	  m_cachedPage.size.setTop( m_pageMargin.top );
	  m_cachedPage.size.setRight( metric.width() -  m_pageMargin.right );
	  m_cachedPage.size.setBottom( metric.height() /*- m_pageMargin.top*/ - m_pageMargin.bottom );
	}
  return m_cachedPage.size;
}

/** Returns the config used for this printer object. */
KConfig* const CPrinter::config() const {
	return m_config;	
}

/** Creates the standard style. */
void CPrinter::setupStandardStyle(){
	//see if m_items contains standard style
	bool found = false;
	for (m_styleList.first(); m_styleList.current(); m_styleList.next()) {
		if (m_styleList.current()->styleName() == i18n("Standard")) {	//found the style
			found = true;
			m_standardStyle = m_styleList.current();
			break;
		}
	}
		
	if (!found) {
		m_standardStyle = new CStyle();		
		m_standardStyle->setStyleName(i18n("Standard"));		
		m_styleList.append( m_standardStyle );
	}
}

/** Sets the vertical position of the printer's painter. */
void CPrinter::setVerticalPos( const int yPos ){
	m_pagePosition.rect.setY(yPos);
}

/** Emits the signal that the styles changed. */
CStyle* const CPrinter::standardStyle() const {
	return m_standardStyle;
}
