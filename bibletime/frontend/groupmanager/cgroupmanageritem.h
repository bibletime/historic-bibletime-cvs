/***************************************************************************
                          cgroupmanageritem.h  -  The items in a CGroupManager
                             -------------------
    begin                : Tue Jan 25 2000
    copyright            : (C) 2000 by The BibleTime Team
    email                : Info@bibletime.de
 ***************************************************************************/
/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CGROUPMANAGERITEM_H
#define CGROUPMANAGERITEM_H

//Own includes
#include "../../structdef.h"

//Qt includes
#include <qstring.h>

//KDE includes
#include <qpixmap.h>
#include <klistview.h>


class CGroupManager;
class CKey;
class CSwordModuleInfo;
class CModuleInfo;
class QListViewItem;

/**
	* An item which implements functions to get the module of this item.
  *
  * An item which implements functions to get the module of this item. The constructor gets the required arguments to operate
  * @author Joachim Ansorg <jansorg@gmx.de>
  */
class CGroupManagerItem : public QListViewItem  {
	
	friend class CGroupManager;

public:
	enum itemType	{ Bookmark, Module, Group };
	
	/**
 	* The constructor. This constructor takes a CGroupManager as parent, the created item will be on the top of the tree.
 	* @param parent The CGRoupManager which will contain the new item
 	* @param caption	The caption of the item. This parameter will eb passes to @see #init()
 	* @param modulename The name of our module. This is used in addition to the CModuleInfo object because sometimes the moduleinfo is missing and we don't want to loose bookmarks.
 	* @param module_info The CModuleInfo object we use for the item. May be zero because a group doesn't have a CModuleInfo.
 	* @param Type The type of the created item. May ba Module, Bookmark or Group. If you leave out the parameter it will be Module by default.
 	*/
	CGroupManagerItem( CGroupManager *parent, const QString& caption,const QString& modulename, CModuleInfo *module_info, CKey* bookmarkKey = 0, CGroupManagerItem::itemType Type = CGroupManagerItem::Module, CImportantClasses* importantClasses = 0 );
	/**
 	* The constructor. This constructor takes a CGroupManagerItem as parent. The created item will be on a child of the parent item
 	* @param parent The CGroupManagerItem which will be parent item of the new item
 	* @param caption	The caption of the item. This parameter will eb passes to @see #init()
 	* @param modulename The name of our module. This is used in addition to the CModuleInfo object because sometimes the moduleinfo is missing and we don't want to loose bookmarks.
 	* @param module_info The CModuleInfo object we use for the item. May be zero because a group doesn't have a CModuleInfo.
 	* @param Type The type of the created item. May ba Module, Bookmark or Group. If you leave out the parameter it will be Module by default.
 	*/	
	CGroupManagerItem( CGroupManagerItem *parent, const QString& caption, const QString& modulename, CModuleInfo *module_info, CKey* bookmarkKey = 0, CGroupManagerItem::itemType Type = CGroupManagerItem::Module, CImportantClasses* importantClasses = 0 );
	/**
 	* The destructor. Cleans up memory.
 	*/
	~CGroupManagerItem();
  /**
 	* Returns the description of the item
 	* @return The description of the item. Useful for bookmarks.
 	*/
  virtual QString description() const;
  /**
 	* Use this to get the parent item of the item-
 	* @return The parent item. Maybe NULL if the item has no parent.
 	* Reimplementation.
 	*/
  virtual CGroupManagerItem* parent();
  /**
 	* Sets the module of ths item.
 	*/
  virtual void setModuleInfo( CModuleInfo* moduleInfo );
  /**
 	* Sets the type of the item.
 	*/
  virtual void setType( CGroupManagerItem::itemType type);
  /**
 	* Updates this item (icons, captions, etc.)
 	*/
  virtual void update();
  virtual CGroupManager* listView();
  /**
 	* Sets the key, which is only used if this item is a Bookmark
 	*/
  virtual void setBookmarkKey( CKey* key );
  /**
 	* Returns the key if it's a Bookmark, othwerwise return 0
 	*/
  CKey* getBookmarkKey();
  /**
 	* Returns a QString version of the key.
 	*/
  QString getKeyText();
  /**
  * Returns the CModuleInfo obkject for this tree-item.
  * @return Gives back a pointer to the moduleinfo. May ba NULL because groups do not have moduleinfos.
  */
  virtual CSwordModuleInfo* moduleInfo();
  /**
 	* Use this function to get the type of this item.
 	* @return Returns the type of the item. May be Group, Bookmark or Module.
 	*/
  virtual CGroupManagerItem::itemType type();
	/**
 	* Show sub item if true, otherwise do not show them.
 	* Reimplementation from QListViewItem
 	*/
  virtual void setOpen( bool );
  /**
   * Sets / changes the description
   */
  void setDescription(const QString&);
  /**
  * Returns the tooltip for this ite, QString::null is returned if this item has no tooltip.
  */
  const QString getToolTip();

private:
  /**
 	* Initializes the item Used by both constructors to share the double used source code
 	* @param caption The caption of the item
 	* @param modulename The modulename as a QSTring
 	* @param module_info	The CModuleInfo object for this item. May be NULL because groups have no moduleinfo.
 	* @param Type The type of the item (Group, Bookmark or Module)
 	*/
  virtual void init( const QString& caption, const QString& modulename, CModuleInfo *module_info,CKey* bookmarkKey, CGroupManagerItem::itemType Type,CImportantClasses* importantClasses);
  /**
 	* The type of this entry
 	*/
  CGroupManagerItem::itemType m_type;
  /**
 	* The pointer to the internal module_info
	* At the moment we assume that the module of a CGroupmanagerItem is a CSwordModuleInfo object.
 	* If we add new backend this should be changed.
 	*/
	CSwordModuleInfo* m_moduleInfo;	//at the moment we use sword
	/**
 	* The key which is only valid for bookmarks
 	*/
	CKey* m_bookmarkKey;
	/**
	* This is true if this item uses a key which was created by this item.
	*/
	bool m_createdOwnKey;
  /**
 	* The description of the item.
 	* At the moment only useable for bookmarks.
 	*/
  QString m_description;	
  /**
 	* The caption of the item.
	* Used for bookmarks and groups.
 	*/
  QString m_caption;	
  /**
	* The namec of the module.
 	*/
  QString m_moduleName;	
	/**
 	* The pixmap which belongs to folders
 	*/
	QPixmap m_folderIcon;
	/**
 	* The pixmap which belongs to opened folders
 	*/
	QPixmap m_openFolderIcon;
	
	CImportantClasses* m_important;
};

#endif

