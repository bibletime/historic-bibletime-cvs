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
#include "../../whatsthisdef.h"

//QT includes
#include <qpixmap.h>
#include <qdatetime.h>
#include <qlist.h>
#include <qpicture.h>
#include <qpainter.h>
#include <qtooltip.h>
#include <qrect.h>
#include <qpoint.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

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
#define LABEL_TEXT_SIZE 6

//used for the shift between the bars
#define BAR_DELTAX 4
#define BAR_DELTAY 2
#define BAR_WIDTH 2+2*BAR_DELTAX  //should be equal or bigger than the label font size
//used for the text below the bars
#define BAR_LOWER_BORDER 100

#define LEGEND_INNER_BORDER 5
#define LEGEND_DELTAY 4
#define LEGEND_WIDTH 85

CSearchDialogAnalysis::CSearchDialogAnalysis(QObject *parent, const char *name )
	: QCanvas(parent,name) {

  m_scaleFactor = 0.0;
  m_legend = 0;
	setBackgroundColor(Qt::white);	
	m_canvasItemList.resize(67);
	m_canvasItemList.setAutoDelete(true);
	resize(1,1);
	connect(this, SIGNAL(resized()), SLOT(slotResized()));	
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
//	KApplication::setOverrideCursor(Qt::waitCursor);
//	setCursor(Qt::waitCursor);
	m_lastPosList.clear();		
	const int numberOfModules = m_moduleList.count();
	if (!numberOfModules)
		return;	
	m_legend = new CSearchDialogAnalysisLegendItem(this, &m_moduleList);	
	m_legend->setX(LEFT_BORDER);
	m_legend->setY(UPPER_BORDER);
	m_legend->setSize(LEGEND_WIDTH,
	           LEGEND_INNER_BORDER*2 + ITEM_TEXT_SIZE*numberOfModules + LEGEND_DELTAY*(numberOfModules-1));
  m_legend->show();	

  int xPos = LEFT_BORDER + m_legend->width() + SPACE_BETWEEN_PARTS;			
	int moduleIndex = 0;	
	m_maxCount = 0;	
	int count = 0;
	CSwordVerseKey key(m_moduleList.first());	
	key.setKey("Genesis 1:1");	
	CSearchDialogAnalysisItem* analysisItem = m_canvasItemList[key.getBook()];
	bool ok = true;
	while (ok) {
		for (moduleIndex = 0,m_moduleList.first(); m_moduleList.current(); m_moduleList.next(),++moduleIndex) {
			KApplication::kApplication()->processEvents(10);
			if (!m_lastPosList.contains(m_moduleList.current()))
				m_lastPosList.insert(m_moduleList.current(),0);
			analysisItem->setCountForModule(moduleIndex, (count = getCount(key.getBook(),m_moduleList.current())));
			m_maxCount = (count > m_maxCount) ? count : m_maxCount;
		}
		analysisItem->setX(xPos);
		analysisItem->setY(UPPER_BORDER);
		analysisItem->show();
		
		xPos += (int)analysisItem->width() + SPACE_BETWEEN_PARTS;		
		ok = key.NextBook();		
   	analysisItem = m_canvasItemList[key.getBook()];
	}	
	resize(xPos+BAR_WIDTH+(m_moduleList.count()-1)*BAR_DELTAX+RIGHT_BORDER, height() );	
	slotResized();

//	setCursor(Qt::arowCursor);
}

/** Sets te module list used for the analysis. */
void CSearchDialogAnalysis::setModuleList(ListCSwordModuleInfo* modules){
	m_moduleList.clear();
	for (modules->first(); modules->current(); modules->next())
		if ( modules->current()->getType() == CSwordModuleInfo::Bible || modules->current()->getType() == CSwordModuleInfo::Commentary )//a Bible or an commentary
			m_moduleList.append(modules->current());

	m_canvasItemList.clear();
	CSwordVerseKey key(m_moduleList.first());	
	key.setKey("Genesis 1:1");
	CSearchDialogAnalysisItem* analysisItem = 0;
	bool ok = true;
	while (ok) {
   	analysisItem = new CSearchDialogAnalysisItem(this, m_moduleList.count(), key.getBook(), &m_scaleFactor, &m_moduleList);
   	analysisItem->hide();
		m_canvasItemList.insert(key.getBook(), analysisItem);
		ok = key.NextBook();
	}	
	update();
}

/** Sets back the items and deletes things to cleanup */
void CSearchDialogAnalysis::reset(){
	m_scaleFactor = 0.0;
  QDictIterator<CSearchDialogAnalysisItem> it( m_canvasItemList ); // iterator for dict
	while ( it.current() ) {
		it.current()->hide();
		++it;
	}	
	m_lastPosList.clear();	
	
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
		it.current()->setSize(BAR_WIDTH + (m_moduleList.count()-1)*BAR_DELTAX, height()-UPPER_BORDER-LOWER_BORDER);
		it.current()->setY(UPPER_BORDER);
  	++it;
	}
	update();
}

/** This function returns a color for each module */
QColor CSearchDialogAnalysis::getColor(int index){
  switch (index){
    case  0: return Qt::red;
    case 	1: return Qt::darkGreen;
    case  2: return Qt::blue;
    case  3: return Qt::cyan;
    case  4: return Qt::magenta;
    case  5: return Qt::white;
    case  6: return Qt::darkRed;
    case  7: return Qt::darkGray;
    case  8: return Qt::black;
    case  9: return Qt::darkCyan;
    case 10: return Qt::darkMagenta;
    default: return Qt::red;
  }
}


//------------------------------------------------------------------
//------------------------------------------------------------------

CSearchDialogAnalysisItem::CSearchDialogAnalysisItem(QCanvas *parent, const int moduleCount, const QString &bookname, double *scaleFactor, ListCSwordModuleInfo* modules)
	: QCanvasRectangle(parent) {

	m_moduleList = modules;	
	m_scaleFactor = scaleFactor;
	m_moduleCount = moduleCount;
	m_bookName = bookname;	
	m_bufferPixmap = 0;
	
 	m_resultCountArray.resize(m_moduleCount);
 	int index = 0;
 	for (index = 0; index < m_moduleCount; index++)
 		m_resultCountArray[index] = 0;
}

CSearchDialogAnalysisItem::~CSearchDialogAnalysisItem() {
	if (m_bufferPixmap)
		delete m_bufferPixmap;
}

/** Sets the resultcount of this item for the given module */
void CSearchDialogAnalysisItem::setCountForModule( const int moduleIndex, const int count) {
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
    		          p1.y() - (!m_resultCountArray[index] ? 0 : ((m_resultCountArray[index])*(*m_scaleFactor))) );
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
	if (!m_bufferPixmap) {
		m_bufferPixmap = new QPixmap();
		m_bufferPixmap->resize(width(),BAR_LOWER_BORDER);
		m_bufferPixmap->fill();		
		QPainter p(m_bufferPixmap);				
		f = p.font();
		f.setPointSize(ITEM_TEXT_SIZE);
		p.setFont(f);		
		p.rotate(90);
		p.drawText(QPoint(5,0), m_bookName);
	}
	painter.drawPixmap(QPoint(x(),height()+y()-BAR_LOWER_BORDER), *m_bufferPixmap);
}

/** Returns the width of this item. */
int CSearchDialogAnalysisItem::width(){
	return m_moduleCount*(m_moduleCount >1 ? BAR_WIDTH - BAR_DELTAX : BAR_WIDTH);	
}

/** Returns the tooltip for this item. */
const QString CSearchDialogAnalysisItem::getToolTip(){
	QString ret = i18n("<CENTER><FONT SIZE=\"+2\">%1</FONT></CENTER><HR>").arg(m_bookName);
	for (int i = 0; i < m_moduleCount; ++i) {
		CSwordModuleInfo* info = m_moduleList->at(i);
		const QColor c = CSearchDialogAnalysis::getColor(i);
		ret.append(
			QString("<B><FONT COLOR=\"#%1\">%2</FONT></B>: %3%4")			
				.arg(QString().sprintf("%02X%02X%02X",c.red(),c.green(),c.blue()))
				.arg(info ? QString::fromLocal8Bit(info->module()->Name()) : QString(""))
				.arg(m_resultCountArray[i])
				.arg(m_moduleCount>1 && i<(m_moduleCount-1) ? "<BR>" : "")
		);
	}
	return ret;
}

//------------------------------------------------------------------
//------------------------------------------------------------------

CSearchDialogAnalysisView::CSearchDialogAnalysisView(QCanvas* canvas, QWidget* parent)
	: QCanvasView(canvas, parent) {
	QWhatsThis::add(this, WT_SD_ANALYSIS_VIEW);
	setFocusPolicy(QWidget::WheelFocus);
	m_toolTip = new ToolTip(this);	
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
}

void CSearchDialogAnalysisView::ToolTip::maybeTip(const QPoint& p) {
	CSearchDialogAnalysisView* view = dynamic_cast<CSearchDialogAnalysisView*>(parentWidget());
	if (!view)
		return;
	QPoint point(p);
	point = view->viewport()->mapFrom(view, point);
	CSearchDialogAnalysisItem* i = view->itemAt( view->viewportToContents(point) );
	if (!i)
		return;
				
	//get type of item and display correct text
	QString text = i->getToolTip();
	if (text.isEmpty())
		return;
	
	QPoint p1 = view->viewport()->mapTo(view, view->contentsToViewport(i->rect().topLeft()));
	p1.setY(0);	
	QPoint p2 = view->viewport()->mapTo(view, view->contentsToViewport(i->rect().bottomRight()));
	p2.setY(view->height());	
	QRect r = QRect( p1, p2 );	
	if (r.contains(p))
		tip(r, text);
}


/** Returns the item at position p. If there no item at that point return 0. */
CSearchDialogAnalysisItem* CSearchDialogAnalysisView::itemAt( const QPoint& p ){
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
