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
#include "../frontend/cpointers.h"
#include "cprintitemlist.h"
#include "cprintitem.h"
#include "cstylelist.h"
#include "cstyle.h"

//KDE includes
#include <kprinter.h>

//Qt includes
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

class CPrinter : public QObject, public KPrinter, public CPointers  {
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
	
	CPrinter( QObject* parent = 0 );
	virtual ~CPrinter();
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
	* Creates a new page and positions the painter at the beginning of it.
	*/
  const bool newPage();
  /**
 	* Returns the margins of the pages.
 	*/
  CPrinter::CPageMargin getPageMargins();
  /**
 	* Sets all the margins at one time.
 	*/
  void setAllMargins( const CPageMargin margins );
  /**
 	* Setups the printer using CPrinterDialog.
 	*/
  void setup( QWidget* parent );
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
  const QRect getPageSize();
  /**
  * Returns the config used for this printer object.
  */
  KConfig* getConfig();
  /**
  * Returns the standard stlye of the printer
  */
  CStyle* standardStyle() const;

public slots: // Public slots
  /**
  * Clears the printing queue.
  */
  void clearQueue();
  /** Emits the signal that the styles changed. */
  void emitStylesChanged();

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
	CSwordBackend*	m_backend;
	printItemList* m_queue;
	styleItemList*	m_styleList;
	CStyle* m_standardStyle;
	KConfig* config;

	struct PaperSizeCache {
		bool initialized;
		bool refresh;		
		KPrinter::PageSize cachedPaper;
		QRect size;
	} m_cachedPage;
	
	bool m_addedItem;
	
signals: // Signals
  /**
 	* Is emitted everytime after an item was printed.
 	*/
//  void printedOneItem(/*const QString& key,*/ const int index);
	void percentCompleted(const int percent);
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
  /**
  * The styles changed.
  */
  void sigStylesChanged();
};

#endif
