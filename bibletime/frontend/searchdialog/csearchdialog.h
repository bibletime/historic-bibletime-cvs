/***************************************************************************
                          csearchdialog.h  -  description
                             -------------------
    begin                : Die Jul 30 2002
    copyright            : (C) 2002 by The BibleTime team
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

#ifndef CSEARCHDIALOG_H
#define CSEARCHDIALOG_H

//BibleTime includes
#include "csearchdialogpages.h"

#include "backend/cswordmoduleinfo.h"
#include "backend/cswordbackend.h"
#include "backend/cswordmodulesearch.h"

#include "frontend/cpointers.h"

//Sword includes


//Qt includes
#include <qwidget.h>
#include <qstring.h>
#include <qcanvas.h>
#include <qdict.h>
#include <qtooltip.h>

//KDE includes
#include <kdialog.h>
#include <kdialogbase.h>

//forward declarations
class CSearchAnalysisItem;
class CSearchAnalysisLegendItem;
class CSearchAnalysis;
class CSearchAnalysisView;


/**
  *@author The BibleTime team
  */

class CSearchDialog : public KDialogBase  {
  Q_OBJECT
public: 
	CSearchDialog(QWidget *parent);
	virtual ~CSearchDialog();
  /**
  * Starts the search with the given module list and given search text.
  * Doesn't wait for the start button press, starts immediatly
  */
  void startSearch( const ListCSwordModuleInfo modules, const QString& searchText);
  /**
  * Sets the list of modules for the search.
  */
  void setModules( const ListCSwordModuleInfo modules );
  /**
  * Returns the list of used modules.
  */
  const ListCSwordModuleInfo modules();
  /**
  * Returns the search text which is used for the search.
  */
  void setSearchText( const QString searchText );
  /**
  * Returns the search text which is set currently.
  */
  const QString searchText();
  /** Resets the parts to the default. */
  void reset();

public slots:
  /**
  * Starts the search with the set modules and the set search text.
  */
  /** Updates the percentage bars. */
  void percentUpdate();
  void startSearch();
  void searchFinished();

private:
  CSearchResultPage* m_searchResultPage;
  CSearchOptionsPage* m_searchOptionsPage;
	CSwordModuleSearch m_searcher;
  struct {
    int optionsPage;
    int resultPage;
  } m_index;
  bool m_interruptedSearch;

protected: // Protected methods
  /**
  * Initializes this object.
  */
  void initView();

protected slots: // Protected slots
  /**
  * Initializes the signal slot connections
  */
  void initConnections();
  /**
  * Updates the progress.
  */
  void updateProgress();
  /**
  * Interrupts the search.
  */
  void interruptSearch();
};

class CModuleChooser : public KListView, public CPointers {
  Q_OBJECT
public:
  class ModuleCheckBoxItem : virtual public QCheckListItem {
  public:
    ModuleCheckBoxItem(QListViewItem* item, CSwordModuleInfo* module);
    ~ModuleCheckBoxItem();
    /**
    * Returns the used module.
    */
    CSwordModuleInfo* const module() const;
  private:
    CSwordModuleInfo* m_module;
  };

  CModuleChooser(QWidget* parent);
  ~CModuleChooser();
  /**
  * Sets the list of modules and updates the state of the checkbox items.
  */
  void setModules( ListCSwordModuleInfo modules );
  /**
  * Returns a list of selected modules.
  */
  ListCSwordModuleInfo modules();

protected: // Protected methods
  /**
  * Initializes this widget and the childs of it.
  */
  void initView();
  /**
  * Initializes the tree of this widget.
  */
  void initTree();
};

class CModuleChooserDialog : public KDialogBase  {
  Q_OBJECT
public:
  CModuleChooserDialog(QWidget* parentDialog, ListCSwordModuleInfo modules);
  ~CModuleChooserDialog();

signals:
  void modulesChanged(ListCSwordModuleInfo modules);

protected: // Protected methods
  /**
  * Initializes the view of this dialog
  */
  void initView();
  /**
  * Initializes the connections of this dialog.
  */
  void initConnections();

protected slots: // Protected slots
  /**
  * Reimplementation to handle the modules.
  */
  virtual void slotOk();

private:
  CModuleChooser* m_moduleChooser;
};

class CSearchAnalysisDialog : public KDialogBase  {
  Q_OBJECT
public:
  CSearchAnalysisDialog(ListCSwordModuleInfo modules, QWidget* parentDialog);
  ~CSearchAnalysisDialog();

protected: // Protected methods
  /**
  * Initializes the widgets SIGNAL and SLOT connections,.
  */
  void initConnections();
  /**
  * Initializes this dialog.
  */
  void initView();

private:
  CSearchAnalysis* m_analysis;
  CSearchAnalysisView* m_analysisView;
};

class CRangeChooserDialog : public KDialog  {
  Q_OBJECT
public:
  CRangeChooserDialog(QWidget* parentDialog);
  ~CRangeChooserDialog();
};

/**
	* CSearchDialogAnaylsis shows the graphical analysis of the search result.
  * @author The BibleTime Team
  */
class CSearchAnalysis : public QCanvas {
   Q_OBJECT
public:	
	CSearchAnalysis(QObject *parent=0, const char *name=0);
  virtual ~CSearchAnalysis();
  /**
  * Starts the analysis of the search result.
	* This should be called only once because
	*	QCanvas handles the updates automatically.
	*/
  void analyse(ListCSwordModuleInfo modules);
  /**
  * This function returns a color for each module
  * @return The color at position index in the list
  */
  static QColor getColor(int index);
 	/*
 	* This function returns a pointer to the list of AnalysisItems
 	*/
 	QDict<CSearchAnalysisItem>* getSearchAnalysisItemList();
  void reset();

protected slots: // Protected slots
  /**
  * No descriptions
  */
  void slotResized();

protected:
  void setModules(ListCSwordModuleInfo modules);

private:
  /**
  * Returns the count of the book in the module
  */
  const unsigned int getCount( const QString book, CSwordModuleInfo* module );
	
	ListCSwordModuleInfo m_moduleList;
 	QDict<CSearchAnalysisItem> m_canvasItemList;
 	QMap<CSwordModuleInfo*,unsigned int> m_lastPosList; 	
  int m_maxCount;
  double m_scaleFactor;
  CSearchAnalysisLegendItem* m_legend;
};


//----------------------------------------------------------
//----------------------------------------------------------

/** Paints the bars for one book for one or more modules
	*
	*/
class CSearchAnalysisItem : public QCanvasRectangle  {
public:	
	/**
	* Standard constructor.
	*/
	CSearchAnalysisItem(QCanvas* parent, const int moduleCount, const QString& bookname, double *scaleFactor, ListCSwordModuleInfo* modules);
	virtual ~CSearchAnalysisItem();	
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
 	QMemArray<int> m_resultCountArray;
 	QPixmap* m_bufferPixmap;
};

class CSearchAnalysisLegendItem : public QCanvasRectangle  {

public:	
	CSearchAnalysisLegendItem(QCanvas* parent, ListCSwordModuleInfo* list );

private:
  virtual void draw (QPainter & painter);
  ListCSwordModuleInfo* m_moduleList;
};

//----------------------------------------------------------
//----------------------------------------------------------
class CSearchAnalysisView : public QCanvasView  {
   Q_OBJECT
public:
	/**
	* Standard constructor
	*/
	CSearchAnalysisView(QCanvas* canvas, QWidget* parent);
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
  CSearchAnalysisItem* itemAt( const QPoint& p );

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
