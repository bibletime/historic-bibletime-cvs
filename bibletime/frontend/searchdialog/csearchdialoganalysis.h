/***************************************************************************
                          csearchdialoganalysis.h  -  description
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

#ifndef CSEARCHDIALOGANALYSIS_H
#define CSEARCHDIALOGANALYSIS_H

//BibleTime includes
#include "../../backend/cswordmoduleinfo.h"
#include "../../backend/cswordbackend.h"

//Qt includes
#include <qwidget.h>
#include <qstring.h>
#include <qcanvas.h>
#include <qarray.h>
#include <qdict.h>
#include <qtooltip.h>

class QPixmap;

class CSearchDialog;
class CSearchDialogAnalysisItem;
class CSearchDialogAnalysisLegendItem;

/**
	* CSearchDialogAnaylsis shows the graphical analysis of the search result.
  * @author The BibleTime Team
  */
class CSearchDialogAnalysis : public QCanvas {
   Q_OBJECT
public:	
	CSearchDialogAnalysis(QObject *parent=0, const char *name=0);
  ~CSearchDialogAnalysis();
  /**
  * Sets the module list used for the analysis.
  */
  void setModuleList(ListCSwordModuleInfo*);
  /**
  * Starts the analysis of the search result.
	* This should be called only once because
	*	QCanvas handles the updates automatically.
	*/
  void analyse();
  /**
  * Sets back the items and deletes things to cleanup
  */
  void reset();
  /**
  * This function returns a color for each module
  * @return The color at position index in the list
  */
  static QColor getColor(int index);

 	/* This function returns a pointer to the list of AnalysisItems */
 	QDict<CSearchDialogAnalysisItem>* getSearchAnalysisItemList();


protected slots: // Protected slots
  /**
  * No descriptions
  */
  void slotResized();

private:
  /**
  * Returns the count of the book in the module
  */
  const unsigned int getCount( const QString book, CSwordModuleInfo* module );
 // const unsigned int getCount( const QString book, CSwordModuleInfo* module );
	
	ListCSwordModuleInfo m_moduleList;
 	QDict<CSearchDialogAnalysisItem> m_canvasItemList;
 	QMap<CSwordModuleInfo*,unsigned int> m_lastPosList; 	
  int m_maxCount;
  double m_scaleFactor;
  CSearchDialogAnalysisLegendItem* m_legend;

};


//----------------------------------------------------------
//----------------------------------------------------------

/** Paints the bars for one book for one or more modules
	*
	*/
class CSearchDialogAnalysisItem : public QCanvasRectangle  {
public:	
	/**
	* Standard constructor.
	*/
	CSearchDialogAnalysisItem(QCanvas* parent, const int moduleCount, const QString& bookname, double *scaleFactor, ListCSwordModuleInfo* modules);
	~CSearchDialogAnalysisItem();	
  /**
  * Sets the resultcount of this item
  */
  void setCountForModule( const int moduleIndex, const int count);

  /**
  * Returns the resultcount of this item
  */
	int getCountForModule( const int moduleIndex);
  /**
  * Returns the width of this item.
  */
  virtual int width();
  /**
  * Returns the tooltip for this item.
  */
  const QString getToolTip();
  	
private:
  virtual void draw (QPainter & painter);	
	
	ListCSwordModuleInfo* m_moduleList;	
	double *m_scaleFactor;
	QString m_bookName;
 	int m_moduleCount;
 	QArray<int> m_resultCountArray;
 	QPixmap* m_bufferPixmap;
};

class CSearchDialogAnalysisLegendItem : public QCanvasRectangle  {

public:	
	CSearchDialogAnalysisLegendItem(QCanvas* parent, ListCSwordModuleInfo* list );

private:
  virtual void draw (QPainter & painter);
  ListCSwordModuleInfo* m_moduleList;
};

//----------------------------------------------------------
//----------------------------------------------------------
class CSearchDialogAnalysisView : public QCanvasView  {
   Q_OBJECT
public:
	/**
	* Standard constructor
	*/
	CSearchDialogAnalysisView(QCanvas* canvas, QWidget* parent);
  /**
  * Returns the sizeHint for this view
  * We give back the size of the parent widgetas default.
  * This is a reimplementation from QCanvasView::sizeHint().
  */
  virtual QSize sizeHint();	
  /**
  * Returns the item at position p.
	* If there no item at that point return 0.
	*/
  CSearchDialogAnalysisItem* itemAt( const QPoint& p );

protected:
  /**
  * Reimplementation.
  */
  virtual void resizeEvent(QResizeEvent* e);

private:
	class ToolTip : public QToolTip {
	public:
		ToolTip(QWidget* parent);
		/**
		* Displays a tooltip for position p
		*/		
		virtual void maybeTip(const QPoint &pos);
	};
	ToolTip* m_toolTip;
	
};


#endif
