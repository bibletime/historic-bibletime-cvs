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
class KProcess;

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
  const unsigned int rightMarginMM() const;
  void setRightMarginMM( const unsigned int margin );
  const unsigned int leftMarginMM() const;
  void setLeftMarginMM( const unsigned int margin );
  const unsigned int upperMarginMM() const;
  void setUpperMarginMM( const unsigned int margin );
  const unsigned int lowerMarginMM() const;
  void setLowerMarginMM( const unsigned int margin );

  const unsigned int rightMargin() const;
  const unsigned int leftMargin() const;
  const unsigned int upperMargin() const;
  const unsigned int lowerMargin() const;
  /**
  * Sets the vertical position of the printer's painter.
  */
  void setVerticalPos( const int yPos );
  /**
  * returns the vertical position of the printer's painter.
  */
  const int getVerticalPos() const;
  /**
 	* Sets te preview application.
 	*/
  void setPreviewApplication( const QString& );
  /**
 	* Returns the path to the preview application.
 	*/
  const QString& getPreviewApplication() const;
  /**
 	* Prints the footer.
 	*/
  void printFooter( QPainter* painter );
  /**
 	* Paints the header.
 	*/
  void printHeader( QPainter* painter);
	/**
	* Creates a new page and positions the painter at the beginning of it.
	*/
  const bool newPage();
  /**
 	* Returns the margins of the pages.
 	*/
  CPrinter::CPageMargin getPageMargins();
  /**
  * draws a header on the page.
  */
  void drawHeader(QPainter *paint, int sx, int width, int y, CPrinter::CHeaderType type, const CPrinter::CPageHeader header, const CPrinter::CPagePosition position);
  /**
 	* Sets all the margins at one time.
 	*/
  void setAllMargins( const CPageMargin margins );
  /**
 	* Setups the printer using CPrinterDialog.
 	*/
  void setup( QWidget* parent );
  /**
 	* Returns true if we use the preview mechanism.
 	*/
  bool getPreview();
  /**
 	* Set to true if you want to open the printed things in the preview application.
 	*/
  void setPreview( const bool usePreview );
  /**
 	* Sets the status of the page header.
 	*/
  void setPageHeader( const bool enableHeader, const CPageHeader header);
  /**
 	* Sets the status of the page footer.
 	*/
  void setPageFooter( bool enableFooter, CPageHeader footer);
  /**
 	* Starts printing the items.
 	*/
  void printQueue();
  /**
 	* Sets the printing queue to queue.
 	*/
  void setPrintQueue(printItemList* queue);
  /**
 	* Returns the print queue object.
 	*/
  printItemList* getPrintQueue() const;
  /**
 	* Appends items to the printing queue.
 	*/
  void appendItemsToQueue( printItemList* items );
  /**
  * Appends the item o the queue.
  */
  void addItemToQueue( CPrintItem* newItem);
  /**
  * Sets the application wide style list to list.
  */
  void setStyleList( styleItemList* list);
  /**
  * Returns the list of styles.
  */
  styleItemList* getStyleList() const;
  /**
  * Reimplementation. cReates a new page.
  */
  const QRect getPageSize() const;
  /**
  * Returns the config used for this printer object.
  */
  KConfig* getConfig();

public slots: // Public slots
  /**
  * Clears the printing queue.
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
  void printedOneItem(const QString& key, const int index);
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

private slots: // Private slots
  /**
  * Is called after the preview application was closed.
  */
  void previewFinished(KProcess*);
};

#endif
