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
#include "../../backend/sword_backend/cswordbackend.h"
#include "csearchdialog.h"
#include "../ctoolclass.h"
#include "../../structdef.h"

//QT includes
#include <qdatetime.h>
#include <qlist.h>
#include <qpicture.h>
#include <qpainter.h>
#include <qtooltip.h>
#include <qrect.h>
#include <qpoint.h>

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

#define ITEM_TEXT_SIZE 8
#define LABEL_TEXT_SIZE 8

//used for the shift between the bars
#define BAR_DELTAX 4
#define BAR_DELTAY 2
#define BAR_WIDTH 2+2*BAR_DELTAX  //should be equal or bigger than the label font size
//used for the text below the bars
#define BAR_LOWER_BORDER 100

#define LEGEND_INNER_BORDER 10
#define LEGEND_DELTAY 5
#define LEGEND_WIDTH 70

CSearchDialogAnalysis::CSearchDialogAnalysis(QObject *parent, const char *name )
	: QCanvas(parent,name) {

  m_scaleFactor = 0.0;
  m_legend = 0;
	setBackgroundColor(Qt::white);
	
	m_canvasItemList.resize(67);
	m_canvasItemList.setAutoDelete(true);
	
	connect(this, SIGNAL(resized()), SLOT(slotResized()));	
	resize(300,300);
}

CSearchDialogAnalysis::~CSearchDialogAnalysis(){
  reset(); // deletes the legend and the items
}


/** Starts the analysis of the search result. This should be called only once because QCanvas handles the updates automatically. */
void CSearchDialogAnalysis::analyse(){
  /**
	* Steps of analysing our search result;
	*	-Create the items for all available books ("Genesis" - "Revelation")
	* -Iterate through all modules we analyse
	*		-Go through all books of this module
	*			-Find out how many times we found the book
	*			-Set the count to the items which belongs to the book
	*/
	KApplication::setOverrideCursor(Qt::waitCursor);

	const int numberOfModules = m_moduleList.count();
	if (!numberOfModules)
		return;
		
	int moduleIndex = 0;
	
	QString currentBook = QString::null;
	QString oldBook     = QString::null;
	
	CSwordModuleInfo* currentModule = 0;
	ListKey currentSearchResult;	
  long int itemsIndex = 0;
  int itemsCount = 0;
  int xPos = 0;

	CSearchDialogAnalysisItem* analysisItem = 0;	
	QString dummyString = QString::null;
		
	m_maxCount = 0;
	
	xPos = LEFT_BORDER;
	m_legend = new CSearchDialogAnalysisLegendItem(this, &m_moduleList);	
	m_legend->setX(xPos);
	m_legend->setY(UPPER_BORDER);
	m_legend->setSize(LEGEND_WIDTH,
	                LEGEND_INNER_BORDER*2 + ITEM_TEXT_SIZE*numberOfModules + LEGEND_DELTAY*(numberOfModules-1));
  m_legend->show();
  xPos += m_legend->width() + SPACE_BETWEEN_PARTS;
	
		
	//now create a CanvasItem for each book of the Bible
	CSwordVerseKey testKey(m_moduleList.first());	
	testKey.setKey("Genesis 1:1");
	currentBook = testKey.getBook();
	do {
  	analysisItem = new CSearchDialogAnalysisItem(this, numberOfModules, currentBook, &m_scaleFactor);
		m_canvasItemList.insert( currentBook, analysisItem);
		
		analysisItem->setX(xPos);
		analysisItem->setY(UPPER_BORDER);
		analysisItem->show();//the sizing stuff will be done in slotResized()

		xPos += (int)analysisItem->width() + SPACE_BETWEEN_PARTS;		
		
		testKey.NextBook();		
		oldBook = currentBook;
		currentBook = testKey.getBook();
	} while (currentBook != oldBook);
 	
 	currentBook = oldBook = QString::null;	
	//now do the real analysis
	for (moduleIndex=0,currentModule = m_moduleList.first();currentModule;currentModule=m_moduleList.next(),++moduleIndex) {		
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
			currentKey.setKey(QString::fromLocal8Bit((const char*)*currentSearchResult.GetElement(itemsIndex)));
			oldBook = currentBook;
			currentBook = currentKey.getBook();
			
			if (currentBook != oldBook && !oldBook.isEmpty()) {	//we have found a new book
				analysisItem = m_canvasItemList.find(oldBook);
				if (analysisItem) {
					analysisItem->setCountForModule(moduleIndex, itemsCount);
					m_maxCount = (itemsCount > m_maxCount) ? itemsCount+2 : m_maxCount;
					itemsCount = 0;
				}
			}
			else
				itemsCount++;
			itemsIndex++;			
		} //while
		
		//set count for last book
		if (!oldBook.isEmpty()) {
			analysisItem = m_canvasItemList.find(oldBook);
			if (analysisItem) {
				analysisItem->setCountForModule(moduleIndex, itemsCount);		
				m_maxCount = (itemsCount > m_maxCount) ? itemsCount+2 : m_maxCount;				
			} //if
		}	// if		
	} // for
	m_scaleFactor = (double)( (double)(height()-UPPER_BORDER-LOWER_BORDER-BAR_LOWER_BORDER-(numberOfModules-1)*BAR_DELTAY)
	                    /(double)m_maxCount);		
	resize(xPos+BAR_WIDTH+(m_moduleList.count()-1)*BAR_DELTAX+RIGHT_BORDER, height() );
	update();
	
	slotResized();
	KApplication::restoreOverrideCursor();	
}

/** Sets te module list used for the analysis. */
void CSearchDialogAnalysis::setModuleList(ListCSwordModuleInfo* modules){
	m_moduleList.clear();
	for (modules->first(); modules->current(); modules->next())
		if ( modules->current()->getType() == CSwordModuleInfo::Bible || modules->current()->getType() == CSwordModuleInfo::Commentary )//a Bible or an commentary
			m_moduleList.append(modules->current());
}

/** Sets back the items and deletes things to cleanup */
void CSearchDialogAnalysis::reset(){
	m_scaleFactor = 0.0;
	m_canvasItemList.clear();
	delete m_legend;
	m_legend = 0;	
	
	update();
}


/** No descriptions */
void CSearchDialogAnalysis::slotResized(){
	m_scaleFactor = (double)( (double)(height()-UPPER_BORDER-LOWER_BORDER-BAR_LOWER_BORDER-(m_moduleList.count()-1)*BAR_DELTAY)
	                                    /(double)m_maxCount);
	QDictIterator<CSearchDialogAnalysisItem> it( m_canvasItemList );
	while ( it.current() ) {
		it.current()->hide();		
		it.current()->setSize(BAR_WIDTH + (m_moduleList.count()-1)*BAR_DELTAX, height()-UPPER_BORDER-LOWER_BORDER);
		it.current()->setY(UPPER_BORDER);
		it.current()->show();
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

CSearchDialogAnalysisItem::CSearchDialogAnalysisItem(QCanvas *parent, const int moduleCount, const QString &bookname, double *scaleFactor)
	: QCanvasRectangle(parent) {
	
	m_scaleFactor = scaleFactor;
	m_moduleCount = moduleCount;
	m_bookName = bookname;
	
 	m_resultCountArray.resize(m_moduleCount);
 	int index = 0;
 	for (index = 0; index < m_moduleCount; index++)
 		m_resultCountArray[index] = 0;
}

/** Sets the resultcount of this item for the given module */
void CSearchDialogAnalysisItem::setCountForModule( const int moduleIndex, const int count) {
	qWarning(QString::number(count).local8Bit());
	m_resultCountArray[moduleIndex] = count;
}

/** Reimplementation. Draws the content of this item. */
void CSearchDialogAnalysisItem::draw(QPainter& painter) {
	QFont f = painter.font();
	f.setPointSize(ITEM_TEXT_SIZE);
	painter.setFont(f);
	
	setPen(QPen(black,1));
	setBrush(Qt::red);
  /**	
	* We have to paint so many bars as we have modules available (we use m_moduleCount)
	* We paint inside the area which is given by height and widt of this rectangle item
	*/	
	int index = 0;	
	int drawn = 0;
	int Value = 0;
	
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
    		          p1.y() - (!m_resultCountArray[index] ? 0 : ((m_resultCountArray[index]+2)*(*m_scaleFactor))) );
    		QRect r(p1, p2);
    		painter.fillRect(r, QBrush(CSearchDialogAnalysis::getColor(index)) );
    		painter.drawRect(r);
    		drawn++;
      }
    }
    //finds the next smaller value
 	  int newValue = 0;
 		for (index=0;index < m_moduleCount; index++)
   	  if (m_resultCountArray[index] < Value && m_resultCountArray[index] >= newValue)
   	    newValue = m_resultCountArray[index];
   	Value = newValue;
	}		

#warning ToDo: Add correct font drawing!!
//	painter.save();	
//	painter.translate(x(),height()+y()-BAR_LOWER_BORDER);
//	painter.rotate(90);
//	QPoint p(5,-(double)(width()-painter.fontMetrics().height()-painter.fontMetrics().ascent()-painter.fontMetrics().descent())/(double)2);
//	QRect clip = painter.clipRegion().boundingRect();
//	clip.setWidth(2*clip.width());	
//	clip.setX( clip.x() + clip.width() );
//	clip.setY( p.y() );	
//	painter.drawText(clip, Qt::AlignLeft, m_bookName);
//	painter.restore();	
}

/** Returns the width of this item. */
int CSearchDialogAnalysisItem::width(){
	return m_moduleCount*(m_moduleCount >1 ? BAR_WIDTH - BAR_DELTAX : BAR_WIDTH);	
}

/** Returns the tooltip for this item. */
const QString CSearchDialogAnalysisItem::getToolTip(){
	QString ret = i18n("Entries found for <B>%1</B><HR>").arg(m_bookName);
	for (int i = 0; i < m_moduleCount; ++i) {
		ret.append( QString("%1%2").arg(m_resultCountArray[i]).arg(i>1 ? "<BR>": ""));
	}
	return ret;
}

//------------------------------------------------------------------
//------------------------------------------------------------------

CSearchDialogAnalysisView::CSearchDialogAnalysisView(QCanvas* canvas, QWidget* parent)
	: QCanvasView(canvas, parent) {
	setFocusPolicy(QWidget::WheelFocus);
	ToolTip* t = new ToolTip(this);
	
	resize(sizeHint());
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

CSearchDialogAnalysisView::ToolTip::ToolTip(QWidget* parent) : QToolTip(parent) {
	qWarning("CSearchDialogAnalysisView::ToolTip::ToolTip(QWidget* parent) : QToolTip(parent)");
}

void CSearchDialogAnalysisView::ToolTip::maybeTip(const QPoint& p) {
	qWarning("CSearchDialogAnalysisView::ToolTip::maybeTip(const QPoint& p)");	
	CSearchDialogAnalysisView* view = dynamic_cast<CSearchDialogAnalysisView*>(parentWidget());
	if (!view)
		return;
	QPoint point(p);
	point = view->viewport()->mapFrom(view, point);
//	point.setX( point.x() );
//	point.setY( point.y() );	
	CSearchDialogAnalysisItem* i = view->itemAt( view->viewportToContents(point) );
	ASSERT(i);
	if (!i)
		return;
				
	//get type of item and display correct text
	QString text = i->getToolTip();
	ASSERT(text.isEmpty());
	if (text.isEmpty())
		return;

	qWarning("computing points");
	QPoint p1 = view->contentsToViewport(i->rect().topLeft());
	QPoint p2 = view->contentsToViewport(i->rect().bottomRight());	
	QRect r = QRect( p1, p2 );
	tip(r, text);
}


/** Returns the item at position p. If there no item at that point return 0. */
CSearchDialogAnalysisItem* CSearchDialogAnalysisView::itemAt( const QPoint& p ){
	qWarning("CSearchDialogAnalysisView::itemAt( const QPoint& p )");
	QCanvasItemList l = canvas()->collisions(p);
	if (!l.count())
		return 0;
	return dynamic_cast<CSearchDialogAnalysisItem*>(l.first());	
}

//------------------------------------------------------------------
//------------------------------------------------------------------

CSearchDialogAnalysisLegendItem::CSearchDialogAnalysisLegendItem(QCanvas *parent, ListCSwordModuleInfo *list )
	: QCanvasRectangle(parent) {
	m_moduleList = list;
}

/** Reimplementation. Draws the content of this item. */
void CSearchDialogAnalysisLegendItem::draw (QPainter& painter) {	
 	painter.save();
 		
	setPen( QPen(black,2) );
	setBrush( Qt::white );
  //the outer rectangle
  QPoint p1(x(),y());
  QPoint p2(x()+width(), y()+height());
  QRect r(p1, p2);
  r.normalize();
 	painter.drawRect(r);
 	
  QFont f = painter.font();
  f.setPointSize(ITEM_TEXT_SIZE);
  painter.setFont(f);
 	
 	for (unsigned int index=0; index < m_moduleList->count(); index++){
 	  // the module color indicators
 	  QPoint p1(x()+LEGEND_INNER_BORDER, y() + LEGEND_INNER_BORDER + index*(LEGEND_DELTAY + ITEM_TEXT_SIZE));
 	  QPoint p2( p1.x() + ITEM_TEXT_SIZE, p1.y() + ITEM_TEXT_SIZE);
 	  QRect r(p1,p2);
 		painter.fillRect(r, QBrush(CSearchDialogAnalysis::getColor(index)) );
 		r.normalize();
 		painter.drawRect(r);
 		
 		QPoint p3( p2.x() + LEGEND_INNER_BORDER, p2.y() );
  	painter.drawText(p3, QString(m_moduleList->at(index)->module()->Name()) );
 	}
  painter.restore();
}
