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

#include <qwidget.h>

#include <klistview.h>
#include <kpopupmenu.h>

#include "cstyle.h"

/** A KListView derived class which handles CStyle items.
  * @author The BibleTime team
  */
class KAction;
class QComboBox;

class CStyleList : public KListView  {
   Q_OBJECT
public:
	CStyleList(styleItemList* items, QWidget *parent=0, const char *name=0);
	~CStyleList();
  /**
 	* Initilizes the view.
 	*/
  void initView();
  /**
 	* Appends the items of itemList
 	*/
  void insertItems( styleItemList* itemList );
  /**
 	* Sets the content to itemList
 	*/
  void setItems( styleItemList* itemList );
  /**
 	* Returns true if the editor was closed with Ok and false if it was closed with another button.
 	*/
  const bool openStyleEditor( CStyle* style );
  /**
 	* Appends the item to the list and m_items.
 	*/
  void insertItem( CStyle* item);
  /**
 	* Sets the combo box which contains the style names.
 	*/
  void setStyleComboBox( QComboBox* );
  /**
 	*
 	*/
  styleItemList* getStyleList();

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
	styleItemList* m_items;
	QGuardedPtr<KAction> editStyle_action;
	QGuardedPtr<KAction> deleteStyle_action;
	QGuardedPtr<QComboBox>	m_styleCombo;	
  /**
 	* Updates the combo box listing the styles being available.
 	*/
  void updateStyleCombo();
};

#endif
