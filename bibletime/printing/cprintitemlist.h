/***************************************************************************
                          cprintitemlist.h  -  description
                             -------------------
    begin                : Sun Aug 20 2000
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

#ifndef CPRINTITEMLIST_H
#define CPRINTITEMLIST_H

#include <qwidget.h>
#include <qlist.h>
#include <klistview.h>

#include "cprintitem.h"

typedef QList<CPrintItem> printItemList;


/**A KListView derived class which handles CPrintItem classes
  *@author The BibleTime team
  */
class CPrintItemList : public KListView  {
   Q_OBJECT
public: 
	CPrintItemList(PrintItemList* items, QWidget *parent=0, const char *name=0);
	~CPrintItemList();
  /**
 	* Sets the tree to contain the items of the list. Other items will be removed.
 	*/
  virtual void setItems( PrintItemList* itemList);
  /**
 	* Inserts the items of the list into the tree.
 	*/
  virtual void insertItems( PrintItemList* itemList );
  /**
 	* Initializes the view.
 	*/
  virtual void initView();
  /**
 	* Applies the style to the selected items.
 	*/
  void applyStyleToSelected( CStyle* );

public slots: // Public slots
  /**
 	* Moves the selected entry one entry down.
 	*/
  void moveDown();
  /**
 	* Moves the item one item up.
 	*/
  void moveUp();
  /**
 	* Deletes the current item.
 	*/
  void deleteSelectedItems();
  /**
 	* Inserts a page divider
 	*/
  void newPage();

private:
	PrintItemList* m_items;		
};
#endif
