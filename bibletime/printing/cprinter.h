/***************************************************************************
                          cprinter.h  -  description
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

#ifndef CPRINTER_H
#define CPRINTER_H

//BibleTime includes
#include <time.h>
#include "../structdef.h"
#include "cprintitemlist.h"
#include "cprintitem.h"
#include "cstylelist.h"
#include "cstyle.h"

//Qt includes
#include <qprinter.h>
#include <qobject.h>
#include <qpainter.h>


/**
	* This is the class which prints the items (low-level).
	* It uses a QPainter to do it.
  * @author The BibleTime team
  */

class CPrinterDialog;
class CPrintItem;
class CSwordBackend;

class KConfig;

class CPrinter : public QObject, public QPrinter  {
	Q_OBJECT
public:	
	struct CPageSize {
	  unsigned int width;
	  unsigned int height;
	};
	struct CPagePosition {
	  unsigned int curPage;
	  QRect rect;
	  time_t now;
	};	
	struct CPageMargin {
	  unsigned int top;
	  unsigned int right;
	  unsigned int bottom;
	 	unsigned int left;
	};
	struct CPageHeader
	{
	  enum CHeaderString
	  {
	    NoString = 0,
	    DateTime,
	    PageNumber,
	    FreeString
	  };
	  enum CHeaderLine
	  {
	    NoLine = 0,
	    SingleLine,
	    Rectangle
	  };
	  bool enable;
	  CHeaderString pos[2]; // left, right
	  CHeaderLine line;
	};	
	enum CHeaderType {
		header = 0,
		footer
	};	

	CPrinter( CImportantClasses* important, QObject* parent = 0 );
	~CPrinter();
  /**
  	* Returns the right margin.
  	*/
  virtual unsigned int rightMargin();
  /**
  	* Sets the right margin.
  	*/
  virtual void setRightMargin( unsigned int margin );
  /**
  	* Returns the right margin.
  	*/
  virtual unsigned int leftMargin();
  /**
  	* Sets the right margin.
  	*/
  virtual void setLeftMargin( unsigned int margin );
  /**
  	* Returns the right margin.
  	*/
  virtual unsigned int upperMargin();
  /**
  	* Sets the right margin.
  	*/
  virtual void setUpperMargin( unsigned int margin );
  /**
  	* Returns the right margin.
  	*/
  virtual unsigned int lowerMargin();
  /**
  	* Sets the right margin.
  	*/
  virtual void setLowerMargin( unsigned int margin );
  /**
  	* Sets te preview application.
  	*/
  virtual void setPreviewApplication( const QString& );
  /**
  	* Returns the path to the preview application.
  	*/
  virtual QString getPreviewApplication();
  /**
  	* Prints the footer.
  	*/
  virtual void printFooter( QPainter* painter );
  /**
  	* Paints the header.
  	*/
  virtual void printHeader( QPainter* painter);
  virtual bool newPage();
  /**
  	* Returns the margins of the pages.
  	*/
  virtual CPrinter::CPageMargin getPageMargins();
  /**
  	* draws a header on the page.
  	*/
  virtual void drawHeader(QPainter *paint, int sx, int width, int y, CPrinter::CHeaderType type, const CPrinter::CPageHeader header, const CPrinter::CPagePosition position);
  /**
  	* Sets all the margins at one time.
  	*/
  virtual void setAllMargins( CPageMargin margins );
  /**
  	* Setups the printer using CPrinterDialog.
  	*/
  virtual void setup( QWidget* parent );
  /**
  	* Print item at bottom of the current page.
  	* If there's no more space left on the current page create a new one.
  	*/
  virtual void printItem( QPainter* p, CPrintItem* item);
  /**
  	* Returns true if we use the preview mechanism.
  	*/
  virtual bool getPreview();
  /**
  	* Set to true if you want to open the printed things in the preview application.
  	*/
  virtual void setPreview( bool usePreview );
  /**
  	* Sets the status of the page header.
  	*/
  virtual void setPageHeader( bool enableHeader, CPageHeader header);
  /**
  	* Sets the status of the page footer.
  	*/
  virtual void setPageFooter( bool enableFooter, CPageHeader footer);
  /**
  	* Starts printing the items.
  	*/
  virtual void printQueue();
  /**
  	* Sets the printing queue to queue.
  	*/
  virtual void setPrintQueue( printItemList* queue);
  /**
  	* Returns the print queue object.
  	*/
  virtual printItemList* getPrintQueue();
  /**
  	* Appends items to the printing queue.
  	*/
  virtual void appendItemsToQueue( printItemList* items );
  /**
  	* Appends the item o the queue.
  	*/
  virtual void addItemToQueue( CPrintItem* newItem);
  /**
  	* Sets the application wide style list to list.
  	*/
  virtual void setStyleList( styleItemList* list);
  /**
  	* Returns the list of styles.
  	*/
  virtual styleItemList* getStyleList();
  /**
  	* Reimplementation. cReates a new page.
  	*/
  virtual QRect getPageSize();
  /**
  	* Returns the config used for this printer object.
  	*/
  KConfig* getConfig();

public slots: // Public slots
  /**
  	*
  	*/
  void clearQueue();

protected:
  /**
  	 * Saves the styles to config file.
  	 */
  void saveStyles();
  /**
  	* Reads the style from config.
  	*/
  void setupStyles();
  /**
  	*
  	*/
  void saveSettings();
  /**
  	*
  	*/
  void readSettings();
  /**
  	* Creates the standard style.
  	*/
  void setupStandardStyle();

private:
	CPageMargin	m_pageMargin;
	CPagePosition	m_pagePosition;
	CPageHeader m_pageHeader;
	CPageHeader m_pageFooter;
	QString m_previewApplication;
	QString m_filename;
	bool m_printIntoFile;
	bool m_usePreview;
	CSwordBackend*	m_backend;
	CImportantClasses* m_important;
	printItemList* m_queue;
	styleItemList*	m_styleList;
	KConfig* config;

signals: // Signals
  /**
  	* Is emitted everytime after an item was printed.
  	*/
  void printedOneItem();
  /**
  	* Is emitted after all items were printed.
  	*/
  void printingFinished();
  /**
  	* Is emitted when printing was interrupted by the user.
  	*/
  void printingInterrupted();
  /**
  	* Is emmitted when the printing has started.
  	*/
  void printingStarted();	
  /**
  	* Is emmitted when the first item was added to the queue
  	* This signal is useful to enable the "Clear printer queue"
  	* icon if the queue contains items
  	*/
  void addedFirstQueueItem();	
  /**
  	* Is emmitted when the printing queue was cleared.
  	*/
  void queueCleared();
};

#endif
