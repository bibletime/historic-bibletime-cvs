/***************************************************************************
                          cmainindex.h  -  description
                             -------------------
    begin                : Sam Jun 22 2002
    copyright            : (C) 2002 by The BibleTime team
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

#ifndef CMAININDEX_H
#define CMAININDEX_H

//BibleTime includes
#include "cindexitem.h"

#include "backend/cswordmoduleinfo.h"
#include "frontend/cpointers.h"
#include "frontend/ctooltip.h"



//Qt includes
#include <qwidget.h>
#include <qtimer.h>

//KDE includes
#include <kaction.h>
#include <klistview.h>

/** The class which manages all bookmarks and modules. The modules are put into own, fixed subfolders sorted by language.
  * @author The BibleTime team
  */

class CSearchDialog;

class CMainIndex : public KListView {
  Q_OBJECT
	
  class ToolTip : public CToolTip {
	public:
		ToolTip(QWidget* parent);
		/**
		* Displays a tooltip for position p using the getToolTip() function of CGroupManagerItem
		*/		
		void maybeTip( const QPoint &pos);
	};

public:
	CMainIndex(QWidget *parent);
	virtual ~CMainIndex();
  void initTree();
  /**
  * Opens the searchdialog using the given modules using the given search text.
  */
  void openSearchDialog( ListCSwordModuleInfo modules, const QString searchText);
  void emitModulesChosen( ListCSwordModuleInfo modules, QString key );

protected: // Protected methods
  /**
  * Reimplementation. Adds the given group to the tree.
  */
  virtual void addGroup( const CItemBase::Type type, const QString language);
  /**
  * Initialize the SIGNAL<->SLOT connections
  */
  void initConnections();
  /**
  * Reimplementation. Returns the drag object for the current selection.
  */
  virtual QDragObject* dragObject();
  /**
  * Reimplementation from KListView. Returns true if the drag is acceptable for the listview.
  */
  virtual bool acceptDrag( QDropEvent* event ) const;
  /**
  * Returns the correct KAction object for the given type of action.
  */
  KAction* action( const CItemBase::MenuAction type );
  /**
  * Reimplementation. Takes care of movable items.
  */
  virtual void startDrag();
  /**
  * Reimplementation to support the items dragEnter and dragLeave functions.
  */
  virtual void contentsDragMoveEvent( QDragMoveEvent* event );
  virtual void contentsDragLeaveEvent( QDragLeaveEvent* e );

protected slots: // Protected slots
  /**
  * Is called when an item was clicked/double clicked.
  */
  void slotExecuted( QListViewItem* );
  void dropped( QDropEvent*, QListViewItem*);
  /**
  * Shows the context menu at the given position.
  */
  void contextMenu(KListView*, QListViewItem*, const QPoint&);
  /**
  * Adds a new subfolder to the current item.
  */
  void createNewFolder();
  /**
  * Opens a dialog to change the current folder.
  */
  void changeFolder();
  /**
  * Exports the bookmarks being in the selected folder.
  */
  void exportBookmarks();
  /**
  * Changes the current bookmark.
  */
  void changeBookmark();
  /**
  * Import bookmarks from a file and add them to the selected folder.
  */
  void importBookmarks();
  /**
  * Opens the searchdialog for the selected modules.
  */
  void searchInModules();
  /**
  * Deletes the selected entries.
  */
  void deleteEntries();
  /**
  * Prints the selected bookmarks.
  */
  void printBookmarks();
  /**
  * Shows information about the current module.
  */
  void aboutModule();
  /**
  * Unlocks the current module.
  */
  void unlockModule();
  void autoOpenTimeout();

private: // Private methods
  CSearchDialog* m_searchDialog;
  ToolTip* m_toolTip;
  bool m_itemsMovable;
  QListViewItem* m_autoOpenFolder;
  QTimer m_autoOpenTimer;

  /**
  * Initializes the view.
  */
  void initView();

  struct Actions {
    KAction* newFolder;
    KAction* changeFolder;

    KAction* changeBookmark;
    KAction* importBookmarks;
    KAction* exportBookmarks;
    KAction* printBookmarks;

    KAction* deleteEntries;

    KAction* searchInModules;
    KAction* unlockModule;
    KAction* aboutModule;
  } m_actions;
  KPopupMenu* m_popup;

signals: // Signals
  /**
  * Is emitted when a module should be opened,
  */
  void modulesChosen( ListCSwordModuleInfo, const QString& );
};

#endif
