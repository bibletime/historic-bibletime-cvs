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
#include "../../backend/sword_backend/cswordmoduleinfo.h"

//Qt includes
#include <qwidget.h>
#include <qcanvas.h>
#include <qarray.h>
#include <qdict.h>

class CSearchDialog;
class CSearchDialogAnalysisItem;
class CSearchDialogAnalysisLegendItem;

/**
	* The widget which provides the graphical search analysis of the search results
  * @author The BibleTime Team
  */
class CSearchDialogAnalysis : public QCanvas {
   Q_OBJECT

public:	
	CSearchDialogAnalysis(QObject *parent=0, const char *name=0);
  ~CSearchDialogAnalysis();
  /**
  	* Sets te module list used for the analysis.
  	*/
  void setModuleList(ListCSwordModuleInfo* modules);
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
  /** This function returns a color for each module */
  static QColor getColor(int index);
private:
	ListCSwordModuleInfo m_moduleList;
 	QDict<CSearchDialogAnalysisItem> m_canvasItemList;
//  int m_maxTextWidth;
  int m_maxCount;
  double m_scaleFactor;
  CSearchDialogAnalysisLegendItem* m_legend;
 	
protected slots: // Protected slots
  /** No descriptions */
  void slotResized();
};



//----------------------------------------------------------
//----------------------------------------------------------

class CSearchDialogAnalysisItem : public QCanvasRectangle  {
public:	
	/**
		*
		*/
	CSearchDialogAnalysisItem(QCanvas* parent, const unsigned int moduleCount, const QString& bookname, double *scaleFactor);
  /**
  	* Sets the resultcount of this item
  	*/
  void setCountForModule( const unsigned int moduleIndex, const int count);
  /**
  	* Sets the scale factor we use to scale the height of
  	* this item according to the set height.
  	*/
protected:
  /**
  	* Reimplementation.
  	*	Draws the content of this item.
  	*/
  virtual void draw (QPainter & painter);
	
private:
	double *m_scaleFactor;
	QString m_bookName;
 	unsigned int m_moduleCount;
 	QArray<unsigned int> m_resultCountArray;
};

class CSearchDialogAnalysisLegendItem : public QCanvasRectangle  {
public:	
	CSearchDialogAnalysisLegendItem(QCanvas* parent, ListCSwordModuleInfo* list );

protected:
  /**
 	* Reimplementation.
 	*	Draws the content of this item.
 	*/
  virtual void draw (QPainter & painter);

private:
  ListCSwordModuleInfo* m_moduleList;
};

//----------------------------------------------------------
//----------------------------------------------------------
class CSearchDialogAnalysisView : public QCanvasView  {
   Q_OBJECT
public:
	/**
		*
		*/
	CSearchDialogAnalysisView(QCanvas* canvas, QWidget* parent);
  /**
  	* Returns the sizeHint for this view
  	* We give back the size of the parent widgetas default.
  	* This is a reimplementation from QCanvasView::sizeHint().
  	*/
  virtual QSize sizeHint();	
  /** No descriptions */
  virtual void resizeEvent( QResizeEvent* e);
};


#endif
