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
#include <klistview.h>

#include "cprintitem.h"

/**A KListView derived class which handles CPrintItem classes
  *@author The BibleTime team
  */


class CPrintItemList : public KListView  {
   Q_OBJECT
public: 
	CPrintItemList(printItemList* items, QWidget *parent=0, const char *name=0);
	~CPrintItemList();
  /**
  	* Sets the tree to contain the items of the list. Other items will be removed.
  	*/
  virtual void setItems( printItemList* itemList);
  /**
  	* Inserts the items of the list into the tree.
  	*/
  virtual void insertItems( printItemList* itemList );
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
  void deleteCurrentItem();
  /**
  	* Inserts a page divider
  	*/
  void newPage();

protected:
	printItemList*	m_items;		
  /**
  	* Reimplementation.
  	*/
  virtual void clear();
};
#endif
