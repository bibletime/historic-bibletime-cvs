/***************************************************************************
                          cgroupmanager.h  -  description
                             -------------------
    begin                : Wed Jan 19 2000
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

#ifndef CGROUPMANAGER_H
#define CGROUPMANAGER_H

//QT includes
#include <qwidget.h>
#include <qlist.h>
#include <qguardedptr.h>
#include <qtooltip.h>

//KDE includes
#define private public
#include <qlistview.h>
#include <klistview.h>
#undef private


#include "../../backend/cmoduleinfo.h"
#include "../../backend/sword_backend/cswordmoduleinfo.h"
#include "../../structdef.h"
#include "cgroupmanageritem.h"

class KPopupMenu;
class KConfig;

class CSearchDialog;

/**
	* The groupmanager which handles all the modules and bookmarks
  * @author The BibleTime Team
  */
class CGroupManager : public KListView  {
   Q_OBJECT

public:
	CGroupManager(CImportantClasses* importantClasses, QWidget *parent=0, const char *name=0, ListCSwordModuleInfo *moduleInfo_list = 0);
	~CGroupManager();
  /**
 	* Initializes the tree of this CGroupmanager
 	*/
  void setupSwordTree();
 	/**
	*
	*/
  void setupSwordTree(ListCSwordModuleInfo*);
	/**
	*
	*/
  void setupStandardSwordTree();
	/**
	*
	*/
  void setupStandardSwordTree(ListCSwordModuleInfo*);
  /**
  * Read settings from config
  */
  void readSettings();
  /**
  * Saves settings to config file
  */
  void saveSettings();
	/**
	*
	*/
  void initView();
	/**
	*
	*/
  void searchBookmarkedModule(QString, CGroupManagerItem *);
	/**
	*
	*/
	CGroupManagerItem*	m_pressedItem;
	/**
	*
	*/
  QList<CGroupManagerItem>*	m_itemList;

public slots: // Public slots
  /**
  * Opens a dialog to enter the key to
  *	unlock an encrypted module.
 	*/
  void slotUnlockModule();
  /**
  * Resets the groupmanager. Recreates the original Bibles, Lexicons and Commentary groups.
  */
  void slotReset();

signals: // Signals
  /**
  * Is emitted when a module in the tree was selected.
  */
  void createSwordPresenter(CSwordModuleInfo*, const QString);
  /**
  * Is emitted when more than one module are selected
  */
  void createSwordPresenter(ListCSwordModuleInfo, const QString);

protected slots: // Protected slots
  /**
  * Deletes the item given as parameter
  */
  void slotDeleteSelectedItems();
  /**
  * Changes the current item
  */
  void slotChangeBookmark();
	/**
	* Is called before the menu is shown
	*/
  void slotPopupAboutToShow();
	/**
	* search the modules that were selected
	*/
  void slotSearchSelectedModules();
  /**
  * Shows an about dialog containing information about a module
  */
  void slotShowAbout();
	/**
	*
	*/
  void slotCreateNewPresenter();
  /**
  * Creates a new group
  */
  void slotCreateNewGroup();
  /**
  * Exports bookmarks of selected group
  */
  void slotExportBookmarks();
  /**
  * Impoorts bookmarks
  */
  void slotImportBookmarks();
  /**
  * Reimplementatuiion.
  */
  void viewportPaintEvent (QPaintEvent*)   ;
  /**
  * Reimplementation with different parameters
  */
  QRect drawDropVisualizer (QPainter *p, CGroupManagerItem *parent, CGroupManagerItem *after, const QString type);
  /**
  * Reimplementation.
  */
  void cleanDropVisualizer();
  /**
  * Reimplementation.
  */
  void contentsMouseDoubleClickEvent ( QMouseEvent * e);
  /**
  * Changes a group.
  */
  void slotChangeGroup();
  /**
  * Prints the selected bookmark.
  */
  void slotPrintBookmark();
  /**
  * Reimplementation.
  */
  void resizeEvent( QResizeEvent * )  ;
  /**
  * Deletes the searchdialog.
  */
  void slotDeleteSearchdialog();

private:
	/**
	* Our tooltip class. It displays a tooltip depending on the current item under the mouse cursor.
	*/
	class ToolTip : public QToolTip {
	public:
		ToolTip(QWidget* parent);
		/**
		* Displays a tooltip for position p
		*/		
		void maybeTip( const QPoint &pos);
	};
	/**
	*
	*/
	ListCSwordModuleInfo *m_swordList;
	/**
	* Initializes the connections of this class
	*/
  void initConnections();
  /**
  *
  */
  void contentsDragEnterEvent( QDragEnterEvent* e);
  /**
  *
  */
  void contentsDragMoveEvent( QDragMoveEvent* e);
  /**
  *
  */
  void contentsDragLeaveEvent( QDragLeaveEvent* e);
  /**
  *
  */
  void contentsDropEvent( QDropEvent* e);
  /**
  *
  */
  void contentsMousePressEvent(QMouseEvent*);
  /**
  *
  */
  void contentsMouseReleaseEvent(QMouseEvent*);
  /**
  * Reimplementation
  */
  void contentsMouseMoveEvent ( QMouseEvent * ) ;
	/**
	* Find ID of parent item.
	*/
	int parentId(CGroupManagerItem *item, CGroupManagerItem* parentItem = 0);
  /**
  * returns the parent of the item with the ID index
  */
  CGroupManagerItem* findParent( int ID, CGroupManagerItem* parentItem = 0 );
  /**
  *
  */
  void createNewBookmark(CGroupManagerItem* parent, CModuleInfo* module, const QString ref);
  /**
  * Returns true if the item "item" is a child of item "parent".
  */
  bool isChild(CGroupManagerItem* parent, CGroupManagerItem* child);
  /**
  * Save items of group to config. If grou is 0 we save all items.
  * The  path to the group-item itself is saved, too.
  */
  bool saveSwordBookmarks(KConfig* configFile, CGroupManagerItem* group);
  /**
  * Reads in bookmarks from config and creates them as subitems of group.
	*	If group is 0 we create them a toplevel items.
	*/
  bool readSwordBookmarks(KConfig* configFile, CGroupManagerItem* group);
  /**
  * Save items of group to config. If grou is 0 we save all items.
  * The  path to the group-item itself is saved, too.
  */
  bool saveSwordModules(KConfig* configFile, CGroupManagerItem* group);
  /**
  * Reads in bookmarks from config and creates them as subitems of group.
	*	If group is 0 we create them a toplevel items.
	*/
  bool readSwordModules(KConfig* configFile, CGroupManagerItem* group);
  /**
  * Save items of group to config. If grou is 0 we save all items.
  * The  path to the group-item itself is saved, too.
  */
  bool saveGroups(KConfig* configFile, CGroupManagerItem* group);
  /**
  * Reads in bookmarks from config and creates them as subitems of group.
	*	If group is 0 we create them a toplevel items.
	*/
  bool readGroups(KConfig* configFile, CGroupManagerItem* group);
  /**
  * The config for the groupmanager != global config of BibleTime
  */
  QGuardedPtr<KConfig> config;
  /**
  * The popup menu which conatins the options after RMB on an item
  */
  QGuardedPtr<KPopupMenu> popupMenu;
	/**
	*
	*/
	bool m_menu;
	/**
	*
	*/
  QPoint m_pressedPos;
	/**
	*
	*/
  QRect	oldDragRect;
	/**
	*
	*/
  CImportantClasses* m_important;
	/**
	*
	*/
  CGroupManagerItem* m_parentItemDrop;
	/**
	*
	*/
  CGroupManagerItem* m_afterItemDrop;
	/**
	*
	*/
  QString m_dragType;	
	/**
	*
	*/
	QGuardedPtr<CSearchDialog> searchDialog;
	ToolTip* m_toolTip;	
	/**
	*
	*/
	bool m_singleClick;	
};

#endif

