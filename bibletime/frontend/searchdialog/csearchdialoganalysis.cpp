/***************************************************************************
                          csearchdialoganalysis.cpp  -  description
                             -------------------
    begin                : Sat Mar 4 2000
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

//BibleTime includes
#include "csearchdialoganalysis.h"
#include "../../backend/sword_backend/cswordmoduleinfo.h"
#include "../../backend/sword_backend/cswordbiblemoduleinfo.h"
#include "../../backend/sword_backend/cswordcommentarymoduleinfo.h"
#include "../../backend/sword_backend/cswordlexiconmoduleinfo.h"
#include "../../backend/sword_backend/cswordversekey.h"
#include "csearchdialog.h"
#include "../ctoolclass.h"

//QT includes
#include <qdatetime.h>
#include <qlist.h>
#include <qpicture.h>
#include <qpainter.h>
#include <qtooltip.h>

//KDE includes
#include <kfiledialog.h>
#include <klocale.h>

//Sword includes
#include <swkey.h>
#include <swmodule.h>
#include <versekey.h>

//our defines
#define SPACE_BETWEEN_PARTS 5
#define RIGHT_BORDER 15
#define LEFT_BORDER 15
#define LOWER_BORDER 10
#define UPPER_BORDER 10

#define BAR_WIDTH 9  //should be equal or bigger than the label font size
//used for the shift between the bars
#define BAR_DELTAX 4
#define BAR_DELTAY 2
//used for the text below the bars
#define BAR_LOWER_BORDER 100

#define LEGEND_INNER_BORDER 10
#define LEGEND_DELTAY 5
#define LEGEND_WIDTH 70

#define ITEM_TEXT_SIZE 8
#define LABEL_TEXT_SIZE 8

CSearchDialogAnalysis::CSearchDialogAnalysis(QObject *parent, const char *name )
	: QCanvas(parent,name) {

  m_moduleList = 0;
  m_canvasItemList = 0;
	setBackgroundColor(Qt::white);
	
	connect(this, SIGNAL(resized()), SLOT(slotResized()));	
	resize(300,300);
	
	update();
}

CSearchDialogAnalysis::~CSearchDialogAnalysis(){
  if (m_canvasItemList)
  	delete m_canvasItemList;
}

/** Starts the analysis of the search result. This should be called only once because QCanvas handles the updates automatically. */
void CSearchDialogAnalysis::analyse(){
	qDebug("void CSearchDialogAnalysis::analyse()");
/**
	* Steps of analysing our search result;
	*	-Create the items for all available books ("Genesis" - "Revelation")
	*		If the items do already exist, do not create thhem but reset them.
	* -Iterate through all modules we analyse
	*		-Go through all books of this module
	*			-Find out how many times we found the book
	*			-Set the count to the items which belongs to the book
	*/
	QApplication::setOverrideCursor(Qt::waitCursor);

//temporary stuff
  unsigned long int loops = 0;
  QTime time = QTime::currentTime();
	
	
	const unsigned int numberOfModules = m_moduleList->count();
	unsigned int moduleIndex = 0;
	
	QString currentBook = QString::null;
	QString oldBook     = QString::null;
	
	CSwordModuleInfo* currentModule = 0;
	ListKey currentSearchResult;	
  unsigned int itemsIndex = 0;
  unsigned int itemsCount = 0;
  unsigned int xPos = 0;
  double scaleFactor = 1.0;

	CSearchDialogAnalysisItem* analysisItem = 0;	
	QString dummyString = QString::null;
		
	m_maxCount = 0;
	
	//create all items (one item for each book of the bible)
	if (!m_canvasItemList) {
		m_canvasItemList = new QDict<CSearchDialogAnalysisItem>(67);
		m_canvasItemList->setAutoDelete(true);
	}
	else {
	//reset all items	
		QDictIterator<CSearchDialogAnalysisItem> it( *m_canvasItemList );
		while ( it.current() ) {
			it.current()->reset(numberOfModules);
			++it;
		}	
	}
	xPos = LEFT_BORDER;
	CSearchDialogAnalysisLegendItem *legend = new CSearchDialogAnalysisLegendItem(this, m_moduleList);
	legend->setX(xPos);
	legend->setY(UPPER_BORDER);
	legend->setSize(LEGEND_WIDTH,
									LEGEND_INNER_BORDER*2 + ITEM_TEXT_SIZE*numberOfModules + LEGEND_DELTAY*(numberOfModules-1));
  legend->show();
  xPos += legend->width() + SPACE_BETWEEN_PARTS;
	
		
	//now create a CanvasItem for each book of the Bible
	CSwordVerseKey testKey(m_moduleList->first());	
	testKey.setKey("Genesis 1:1");
	currentBook = testKey.getBook();
	do {
		if (!(analysisItem = m_canvasItemList->find(currentBook))) {
			analysisItem = new CSearchDialogAnalysisItem(this, numberOfModules);
			m_canvasItemList->insert( currentBook, analysisItem);
		}
		analysisItem->setBookname(currentBook);		
		
		testKey.NextBook();		
		oldBook = currentBook;
		currentBook = testKey.getBook();
	} while (currentBook != oldBook);
 	
 	currentBook = oldBook = QString::null;	
	qWarning(QString("The initialization took from %1 until %2").arg(time.toString()).arg(QTime::currentTime().toString()).local8Bit());	
	//now do the real analysis
	for (moduleIndex=0,currentModule = m_moduleList->first();currentModule;currentModule=m_moduleList->next(),++moduleIndex) {		
		currentSearchResult = currentModule->getSearchResult();
		if (!currentSearchResult.Count())
			continue;
		
		//now go through all different books and get the search result
		oldBook = currentBook = QString::null;
		itemsIndex = 0;
		itemsCount = 0;		
		CSwordVerseKey currentKey(currentModule);						
		
		while (itemsIndex < currentSearchResult.Count()) {
			KApplication::kApplication()->processEvents(10);
			++loops;
			
			currentKey.setKey(QString::fromLocal8Bit((const char*)*currentSearchResult.GetElement(itemsIndex)));
			oldBook = currentBook;
			currentBook = currentKey.getBook();
			
			if (currentBook != oldBook && !oldBook.isEmpty()) {	//we have found a new book
				analysisItem = m_canvasItemList->find(oldBook);
				if (analysisItem) {
					analysisItem->setCountForModule(moduleIndex, itemsCount);
					m_maxCount = (itemsCount > m_maxCount) ? itemsCount : m_maxCount;
					itemsCount = 0;
				}
			}
			else
				itemsCount++;
			itemsIndex++;			
		} //while
		
		//set count for last book
		if (!oldBook.isEmpty()) {
			analysisItem = m_canvasItemList->find(oldBook);
			if (analysisItem) {
				analysisItem->setCountForModule(moduleIndex, itemsCount);		
				m_maxCount = (itemsCount > m_maxCount) ? itemsCount : m_maxCount;				
			} //if
		}	// if		
		qWarning(QString("%1 loops").arg(loops).local8Bit());
		QTime t = QTime::currentTime();
		qWarning(time.toString().local8Bit());
		qWarning(t.toString().local8Bit());
	} // for

	scaleFactor = (double)( (double)(height()-UPPER_BORDER-LOWER_BORDER-BAR_LOWER_BORDER-(numberOfModules-1)*BAR_DELTAY)
	                    /(double)m_maxCount);
	
	CSwordVerseKey vk( m_moduleList->first() );
	vk.setKey("Genesis 1:1");
	currentBook = vk.getBook();
	oldBook = QString::null;
	while ( (analysisItem = m_canvasItemList->find(currentBook)) && (currentBook != oldBook) ) {
		analysisItem->setX(xPos);
		analysisItem->setY(UPPER_BORDER);
		
		analysisItem->setSize(BAR_WIDTH + (numberOfModules-1)*BAR_DELTAX,height()-UPPER_BORDER-LOWER_BORDER);
		analysisItem->setScaleFactor(scaleFactor);
		analysisItem->show();		
		
		xPos += (int)analysisItem->width() + SPACE_BETWEEN_PARTS;		
		oldBook = currentBook;
		
		vk.NextBook();
		currentBook = vk.getBook();
	}

	vk.setKey("Revelation 1:1");
	resize( xPos+m_canvasItemList->find(vk.getBook())->width()+RIGHT_BORDER, height() );
	update();
	QApplication::restoreOverrideCursor();	
	
	qWarning(QString("The whole analysis was done from %1 until %2").arg(time.toString()).arg(QTime::currentTime().toString()).local8Bit());
}

/** Sets te module list used for the analysis. */
void CSearchDialogAnalysis::setModuleList(ListCSwordModuleInfo* modules){
	if (!m_moduleList)
		m_moduleList = new ListCSwordModuleInfo();
	m_moduleList->clear();
	
	for (modules->first(); modules->current(); modules->next())
		if ( dynamic_cast<CSwordBibleModuleInfo*>(modules->current()) )//a Bible or an commentary
			m_moduleList->append(modules->current());
}

/** Sets back the items and deletes things to cleanup */
void CSearchDialogAnalysis::reset(){
	if (!m_canvasItemList)
		return;
		
	QDictIterator<CSearchDialogAnalysisItem> it( *m_canvasItemList );
	while ( it.current() ) {
		it.current()->hide();
		++it;
	}
	update();
}


/** No descriptions */
void CSearchDialogAnalysis::slotResized(){
	qDebug("CSearchDialogAnalysis::slotResized()");
  if (!m_canvasItemList)
  	return;
		
	const double scaleFactor = (double)( (double)(height()-UPPER_BORDER-LOWER_BORDER-BAR_LOWER_BORDER-(m_moduleList->count()-1)*BAR_DELTAY)
	                                    /(double)m_maxCount);
	QDictIterator<CSearchDialogAnalysisItem> it( *m_canvasItemList );
	while ( it.current() ) {
		it.current()->setScaleFactor(scaleFactor);
		it.current()->setSize(BAR_WIDTH + (m_moduleList->count()-1)*BAR_DELTAX, height()-UPPER_BORDER-LOWER_BORDER);
		it.current()->setY(UPPER_BORDER);
  	++it;
	}
	update();
}
/** This function returns a color for each module */
QColor CSearchDialogAnalysis::getColor(int index){
  switch (index){
    case  0: return Qt::red;
    case  1: return Qt::green;
    case  2: return Qt::blue;
    case  3: return Qt::cyan;
    case  4: return Qt::magenta;
    case  5: return Qt::yellow;
    case  6: return Qt::darkRed;
    case  7: return Qt::darkGreen;
    case  8: return Qt::darkBlue;
    case  9: return Qt::darkCyan;
    case 10: return Qt::darkMagenta;
    case 11: return Qt::darkYellow;
    default: return Qt::red;
  }
}


//------------------------------------------------------------------
//------------------------------------------------------------------

CSearchDialogAnalysisItem::CSearchDialogAnalysisItem(QCanvas *parent, const unsigned int moduleCount)
	: QCanvasRectangle(parent) {
	
	m_scaleFactor = 1;
	m_moduleCount = moduleCount;
	m_bookName = QString::null;
	
 	m_resultCountArray = QArray<unsigned int>(m_moduleCount);
 	unsigned int index = 0;
 	for (index = 0; index < m_moduleCount; index++) {
 		m_resultCountArray[index] = 0;
 	} 	
}

CSearchDialogAnalysisItem::~CSearchDialogAnalysisItem(){
}

/** Sets the scale factor we use to scale the height of this item according to the set height.*/
void CSearchDialogAnalysisItem::setScaleFactor(const double factor){
	m_scaleFactor = factor;
}

/** Sets the resultcount of this item for the given module */
void CSearchDialogAnalysisItem::setCountForModule( const unsigned int moduleIndex, const int count) {
	m_resultCountArray[moduleIndex] = count;
}

void CSearchDialogAnalysisItem::setBookname( const QString bookName){
	m_bookName = bookName;
}


/** Reimplementation. Draws the content of this item. */
void CSearchDialogAnalysisItem::draw (QPainter& painter) {
	QFont f = painter.font();
	f.setPointSize(ITEM_TEXT_SIZE);
	painter.setFont(f);
	
	setPen( QPen(black,2) );
	setBrush(Qt::red);
  /**	
	* We have to paint so many bars as we have modules available (we use m_moduleCount)
	* We paint inside the area which is given by height and widt of this rectangle item
	*/	
	unsigned int index = 0;	
	unsigned int drawn = 0;
	unsigned int Value = 0;
	
	//find out the biggest value
	for (index=0;index < m_moduleCount; index++)
	  if (m_resultCountArray[index] > Value)
	    Value = m_resultCountArray[index];
	
	while (drawn < m_moduleCount){
    for (index=0; index < m_moduleCount; index++){
      if (m_resultCountArray[index] == Value){
    		QPoint p1(x()+(m_moduleCount-drawn-1)*BAR_DELTAX,
    		          height()+y()-BAR_LOWER_BORDER-(m_moduleCount-drawn)*BAR_DELTAY);
    		QPoint p2(p1.x() + BAR_WIDTH,
    		          p1.y() - m_resultCountArray[index]*m_scaleFactor);		
    		QRect r(p1, p2);
    		r.normalize();
    		painter.fillRect(r, QBrush(CSearchDialogAnalysis::getColor(index)) );
    		painter.drawRect(r);
    		drawn++;
    		break;
      }
    }
    //finds the next smaller value
 	  int newValue = 0;
 		for (index=0;index < m_moduleCount; index++)
   	  if (m_resultCountArray[index] < Value && m_resultCountArray[index] >= newValue)
   	    newValue = m_resultCountArray[index];
   	Value = newValue;
	}		
	painter.save();
	
	painter.translate(x(),height()+y()-BAR_LOWER_BORDER);
	painter.rotate(90);
	QPoint p(5,-(double)(width()-painter.fontMetrics().height())/(double)2 );
	painter.drawText(p, m_bookName);
		
	painter.restore();
	
}

/** No descriptions */
void CSearchDialogAnalysisItem::reset( const int moduleCount ){
	m_moduleCount = moduleCount;
	
	m_scaleFactor = 1;
	m_moduleCount = moduleCount;
	m_bookName = QString::null;
	
	setPen( QPen(black) );
	setBrush( QBrush(red) );
	
 	m_resultCountArray = QArray<unsigned int>(m_moduleCount);
 	unsigned int index = 0;
 	for (index = 0; index < m_moduleCount; index++) {
 		m_resultCountArray[index] = 0;
 	} 	

}

//------------------------------------------------------------------
//------------------------------------------------------------------

CSearchDialogAnalysisView::CSearchDialogAnalysisView(QCanvas* canvas, QWidget* parent)
	: QCanvasView(canvas, parent) {
	
	resize(sizeHint());
}

CSearchDialogAnalysisView::~CSearchDialogAnalysisView(){

}

/** Returns the sizeHint for this view */
QSize CSearchDialogAnalysisView::sizeHint(){
	if ( parentWidget() )
		return parentWidget()->sizeHint();
	return QCanvasView::sizeHint();
}

/** No descriptions */
void CSearchDialogAnalysisView::resizeEvent( QResizeEvent* e){
	QCanvasView::resizeEvent(e);
	canvas()->resize( canvas()->width(), viewport()->height() );
}

//------------------------------------------------------------------
//------------------------------------------------------------------

CSearchDialogAnalysisLegendItem::CSearchDialogAnalysisLegendItem(QCanvas *parent, ListCSwordModuleInfo *list )
	: QCanvasRectangle(parent) {
	m_moduleList = list;
}

CSearchDialogAnalysisLegendItem::~CSearchDialogAnalysisLegendItem(){
}
/** Reimplementation. Draws the content of this item. */
void CSearchDialogAnalysisLegendItem::draw (QPainter& painter) {
	
	setPen( QPen(black,2) );
	setBrush( Qt::white );
  //the outer rectangle
  QPoint p1(x(),y());
  QPoint p2(x()+width(), y()+height());
  QRect r(p1, p2);
  r.normalize();
 	painter.drawRect(r);
 	
 	painter.save();
  QFont f = painter.font();
  f.setPointSize(ITEM_TEXT_SIZE);
  painter.setFont(f);
 	
 	for (int index=0; index < m_moduleList->count(); index++){
 	  // the module color indicators
 	  QPoint p1(x()+LEGEND_INNER_BORDER, y() + LEGEND_INNER_BORDER + index*(LEGEND_DELTAY + ITEM_TEXT_SIZE));
 	  QPoint p2( p1.x() + ITEM_TEXT_SIZE, p1.y() + ITEM_TEXT_SIZE);
 	  QRect r(p1,p2);
// 		painter.setBrush(  );
 		painter.fillRect(r, QBrush(CSearchDialogAnalysis::getColor(index)) );
 		r.normalize();
 		painter.drawRect(r);
 		
 		QPoint p3( p2.x() + LEGEND_INNER_BORDER, p2.y() );
  	painter.drawText(p3, (QString) m_moduleList->at(index)->module()->Name() );
 	}
  painter.restore();


}



