/***************************************************************************
                          cstylelist.h  -  description
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

#ifndef CSTYLELIST_H
#define CSTYLELIST_H

//BibleTime includes
#include "../frontend/cpointers.h"
#include "cstyle.h"


//Qt includes
#include <qwidget.h>
#include <qlist.h>

//KDE includes
#include <klistview.h>
#include <kpopupmenu.h>

class KAction;
class QComboBox;

typedef QList<CStyle> styleList;

  /** A KListView derived class which handles CStyle items.
  * @author The BibleTime team
  */
class CStyleList : public KListView, public CPointers   {
   Q_OBJECT
public:
	CStyleList(CPrinter* printer, StyleItemList* items, QWidget *parent=0, const char *name=0);
	~CStyleList();
  /**
 	* Initilizes the view.
 	*/
  void initView();
  /**
 	* Appends the items of itemList
 	*/
  void insertItems( StyleItemList* itemList );
  /**
 	* Sets the content to itemList
 	*/
  void setItems( StyleItemList* itemList );
  /**
 	*
 	*/
  void openStyleEditor( CStyle* const style );
  /**
 	* Appends the item to the list and m_items.
 	*/
  void insertItem( CStyle* item);
  /**
 	*
 	*/
  StyleItemList* styleList();

public slots: // Public slots
  /**
 	* Deletes the current style item.
 	*/
  void deleteCurrentStyle();
  /**
 	* Creates a new style item.
 	*/
  void createNewStyle();
  /**
 	* Opens the style editor dialog for he selected style.
 	*/
  void editCurrentStyle();

private:
	StyleItemList* m_items;
	QGuardedPtr<KAction> editStyle_action;
	QGuardedPtr<KAction> deleteStyle_action;
	CPrinter* m_printer;
  void updateStyleCombo();
};

#endif
