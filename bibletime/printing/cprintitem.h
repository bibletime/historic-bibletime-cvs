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

#include <qobject.h>
#include <qguardedptr.h>
#include <qlist.h>
#include <qlistview.h>

class CSwordKey;
class CSwordModuleInfo;

class CStyle;
class CPrintItemList;
class CPrinter;

class CPrintItem;
typedef QList<CPrintItem> PrintItemList;

/**
	* The class which implements the printable items.
  * @author The BibleTime team
  */
class CPrintItem {
public:
  class ListViewItem : public QListViewItem {
		public:
			ListViewItem( QListView* parent, CPrintItem* printItem );
			CPrintItem* printItem() const;		
			CStyle* style() const;			
		private:
			CPrintItem* m_printItem;
  };
	

	CPrintItem(CSwordModuleInfo* module, const QString& startKey, const QString& stopKey, const QString& description = QString::null);
  /**
	* Sets the style for this item.
 	*/
	void setStyle( CStyle* );
  /**
	* Sets the style for this item.
 	*/
	CStyle* style() const;
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
  QListViewItem* listViewItem() const;
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
  QString m_description;
  QString m_headerText;
  QString m_moduleText;
	QString m_startKey;
	QString m_stopKey;
	bool m_startEmpty;
	bool m_stopEmpty;
  CSwordModuleInfo* m_module;
  QListViewItem* m_listViewItem;
};

#endif
