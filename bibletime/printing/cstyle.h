/***************************************************************************
                          cstyle.h  -  description
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

//#include <qwidget.h>
//#include <qobject.h>
#include <qlistview.h>
#include <qlist.h>

#include "cprinter.h"


#ifndef CSTYLE_H
#define CSTYLE_H

//forward class decalarations
class CStyleFormat;
class CStyleList;

class CStyle;
typedef QList<CStyle> styleItemList;

/**The class containing the properties requiredfor prinitng styles.
  *@author The BibleTime team
  */
class CStyle {
public:
	enum styleType { Header, ModuleText, Description, Unknown };
	
	CStyle();
	~CStyle();
  /**
  	* Returns the proper CStyleFormat for the given type.
  	*/
  CStyleFormat* getFormatForType( const CStyle::styleType );
  /**
  	* Sets the format for the given type.
  	*/
  void setFormatForType( const CStyle::styleType type, CStyleFormat* format);
  /**
  	* Set the printing of the header (true enables it).
  	*/
  void setFormatTypeEnabled( CStyle::styleType, bool );
  /**
  	* Returns true if we have the given type enabled.
  	*/
  bool hasFormatTypeEnabled( CStyle::styleType );
  /**
  	* Returns a QListViewItem for inserted in list.
  	*/
  QListViewItem* getListViewItem( CStyleList* list );
  /**
  	* Sets the name of the style.
  	*/
  void setStyleName( const QString& name);
  /**
  	* Returns the style name
  	*/
  QString getStyleName() const;
  /**
  	* Retusnt eh listvuiewitem of this style, if it's no created already return 0.
  	*/
  QListViewItem* getListViewItem();
  /**
  	* Updates the Listview items
  	*/
  void updateListViewItem();
  /**
  	* Deletes the list view item.
  	*/
  void deleteListViewItem();

private:
  /**
  	* Clears all variables and sets them back
  	*/
  void clearData();
	
	QString	m_styleName;
	CStyleFormat*	m_headerFormat;
	CStyleFormat*	m_moduleTextFormat;
	CStyleFormat*	m_descriptionFormat;	
	
	bool m_isHeaderFormatEnabled;
	bool m_isModuleTextFormatEnabled;
	bool m_isDescriptionFormatEnabled;
	
	QListViewItem*	m_listViewItem;
};

#endif
