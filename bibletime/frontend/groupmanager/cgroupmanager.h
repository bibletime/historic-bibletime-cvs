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
#include <qtooltip.h>

//KDE includes
#define private public
#include <qlistview.h>
#include <klistview.h>
#undef private

#include "../../backend/cswordmoduleinfo.h"
#include "../../structdef.h"
#include "cgroupmanageritem.h"
#include "../cpointers.h"

class KPopupMenu;
class KConfig;

class CSearchDialog;

/**
	* The groupmanager which handles all the modules and bookmarks
  * @author The BibleTime Team
  */
class CGroupManager : public KListView, public CPointers  {
   Q_OBJECT

public:
	CGroupManager(QWidget *parent=0, const char *name=0, ListCSwordModuleInfo *moduleInfo_list = 0, const bool useBookmarks = true, const bool saveSettings = true, const bool useDnD=true, const bool useExtendedMode=true, const bool useRMBMenu = true, const bool showHelpDialogs = true);
	~CGroupManager();
  /**
 	* Initializes the tree of this CGroupmanager
 	*/
  void setupSwordTree();
	/**
	*
	*/
  void setupStandardSwordTree();
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
  void searchBookmarkedModule(const QString&, CGroupManagerItem *);
  /**
  * Returns true if the item "item" is a child of item "parent".
  */
  const bool isChild(QListViewItem* parent, QListViewItem* child);

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
  /**
  * Resizes the content size of this list view.
  */
  virtual void resizeContents(int w, int h);


signals: // Signals
  /**
  * Is emitted when a module in the tree was selected.
  */
  void createSwordPresenter(CSwordModuleInfo*, const QString&);
  /**
  * Is emitted when more than one module are selected
  */
  void createSwordPresenter(ListCSwordModuleInfo, const QString&);

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
  const QRect drawDropVisualizer (QPainter *p, CGroupManagerItem *parent, CGroupManagerItem *after, const QString& type);
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
		* Displays a tooltip for position p using the getToolTip() function of CGroupManagerItem
		*/		
		void maybeTip( const QPoint &pos);
	};
	/**
	*
	*/
	ListCSwordModuleInfo *m_swordList;
  void initConnections();
  void contentsDragEnterEvent( QDragEnterEvent* e);
  void contentsDragMoveEvent( QDragMoveEvent* e);
  void contentsDragLeaveEvent( QDragLeaveEvent* e);
  void contentsDropEvent( QDropEvent* e);
  void contentsMousePressEvent(QMouseEvent*);
  void contentsMouseReleaseEvent(QMouseEvent*);
  void contentsMouseMoveEvent ( QMouseEvent * ) ;
	/**
	* Find ID of parent item.
	*/
	const int parentId(CGroupManagerItem *item, CGroupManagerItem* parentItem = 0);
  /**
  * returns the parent of the item with the ID index
  */
  CGroupManagerItem* findParent( const int ID, CGroupManagerItem* parentItem = 0 );
  void createNewBookmark(CGroupManagerItem* parent, CSwordModuleInfo* module, const QString& ref);
  /**
  * Save items of group to config. If grou is 0 we save all items.
  * The  path to the group-item itself is saved, too.
  */
  const bool saveSwordBookmarks(KConfig* configFile, CGroupManagerItem* group);
  /**
  * Reads in bookmarks from config and creates them as subitems of group.
	*	If group is 0 we create them a toplevel items.
	*/
  const bool readSwordBookmarks(KConfig* configFile, CGroupManagerItem* group);
  /**
  * Save items of group to config. If grou is 0 we save all items.
  * The  path to the group-item itself is saved, too.
  */
  const bool saveSwordModules(KConfig* configFile, CGroupManagerItem* group);
  /**
  * Reads in bookmarks from config and creates them as subitems of group.
	*	If group is 0 we create them a toplevel items.
	*/
  const bool readSwordModules(KConfig* configFile, CGroupManagerItem* group);
  /**
  * Save items of group to config. If grou is 0 we save all items.
  * The  path to the group-item itself is saved, too.
  */
  const bool saveGroups(KConfig* configFile, CGroupManagerItem* group);
  /**
  * Reads in bookmarks from config and creates them as subitems of group.
	*	If group is 0 we create them a toplevel items.
	*/	
  const bool readGroups(KConfig* configFile, CGroupManagerItem* group);

 	KConfig* m_config;
  KPopupMenu* m_popupMenu;
	bool m_menu;
  QPoint m_pressedPos;
  QRect	m_oldDragRect;
  CGroupManagerItem* m_parentItemDrop;
  CGroupManagerItem* m_afterItemDrop;
  QString m_dragType;	
	CSearchDialog* m_searchDialog;
	ToolTip* m_toolTip;	
	CGroupManagerItem*	m_pressedItem;
  QList<CGroupManagerItem>*	m_itemList;	
	bool m_singleClick;	
	bool m_useBookmarks;
	bool m_saveSettings;
	bool m_useDragDrop;
	bool m_useExtendedMode;
	bool m_useRMBMenu;		
	bool m_showHelpDialogs;
	
private slots: // Private slots
  /**
  * Is called when the return key was pressed on a listview item.
  */
  void slotReturnPressed( QListViewItem* );	
};

#endif

