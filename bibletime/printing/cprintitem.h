/***************************************************************************
                          cprintitem.h  -  description
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

#ifndef CPRINTITEM_H
#define CPRINTITEM_H

//BibleTime includes
#include "frontend/cbtconfig.h"

//Qt includes
#include <qobject.h>
#include <qguardedptr.h>
#include <qptrlist.h>
#include <qlistview.h>

class CSwordKey;
class CSwordModuleInfo;
class CStyle;
class CPrinter;
class CPrintItem;
class CPrintItemList;

typedef QPtrList<CPrintItem> ListCPrintItem;

/**
	* The class which implements the printable items.
  * @author The BibleTime team
  */
class CPrintItem {
public:
  class ListViewItem : public QListViewItem {
		public:
			ListViewItem( QListView* parent, CPrintItem* printItem );
			CPrintItem* const printItem() const;		
			CStyle* const style() const;			
		private:
			CPrintItem* m_printItem;
  };
	
	CPrintItem(CSwordModuleInfo* module, const QString& startKey, const QString& stopKey, const QString& description = QString::null, const CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults(),const CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults() );
  /**
	* Sets the style for this item.
 	*/
	void setStyle( CStyle* );
  /**
	* Sets the style for this item.
 	*/
	inline CStyle* const style() const;
  /**
 	* Returns the listview item for this printitem.
 	*/
  QListViewItem* listViewItem( CPrintItemList* );
  /**
 	* Updates the item.
 	*/
  void updateListViewItem();
  /**
	* Returns the used ListView item.
	*/
  inline QListViewItem* const listViewItem() const;
  /**
 	* Deletes the list view item.
 	*/
  void deleteListViewItem();
  /**
  * Draws the content of this item on the page.
  */
  void draw(QPainter* p, CPrinter* printer);

private: // Protected attributes
  const QString& headerText();
  const QString& moduleText();

  CStyle* m_style;
  QString m_headerText;
  QString m_description;
  QString m_moduleText;
	QString m_startKey;
	QString m_stopKey;
	bool m_startEmpty;
	bool m_stopEmpty;
  CSwordModuleInfo* m_module;
  QListViewItem* m_listViewItem;

  CSwordBackend::DisplayOptions m_displayOptions;
  CSwordBackend::FilterOptions m_filterOptions;
};

inline CStyle* const CPrintItem::style() const {
	return m_style;
}

inline QListViewItem* const CPrintItem::listViewItem() const {
	return m_listViewItem;
}

#endif
