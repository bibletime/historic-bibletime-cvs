/***************************************************************************
                   csearchdialogresultview.h  -  description
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

#ifndef CSEARCHDIALOGRESULTVIEW_H
#define CSEARCHDIALOGRESULTVIEW_H

//BibleTime includes
#include "../../backend/cswordmoduleinfo.h"
#include "../cpointers.h"

//QT includes
#include <qwidget.h>
#include <qlistview.h>
#include <qlistbox.h>
#include <qlistview.h>

class KPopupMenu;
class CSearchDialogResultItem;
class CSearchDialog;
class KAction;

/**
	*	Handles the search result
  *	@author The BibleTime Team
  */
class CSearchDialogResultModuleView : public QListView, public CPointers {
	Q_OBJECT
public:
	CSearchDialogResultModuleView(QWidget *parent=0, const char *name=0);
	~CSearchDialogResultModuleView();
	/**
	*	Sets te modulelist displayed in this module tree.
	*/
	void setModuleList(ListCSwordModuleInfo *moduleList) { this->moduleList = moduleList; };
	/**
	* Initializes the tree of this CGroupmanager
	*/
	void setupTree();
	/**
	* Initiales the view of this widget.
	*/
	void initView();
  /**
 	* Returns the module belonging to the selected item.
 	*/
  CSwordModuleInfo* getCurrentModule();
  /**
 	* Reimplementation. Sets the cortrect QHeader width.
 	*/
  void resizeEvent( QResizeEvent* e);

protected:
  void viewportMousePressEvent(QMouseEvent *e);

protected slots: // Protected slots
  /**
 	* Adds all items  of the search result of this module
 	* to the printing queue of BibleTime.
  */
  void printSearchResult();
  /**
 	* This slot saves the search result to disk.
 	*/
  void slotSaveSearchResult();
  /**
 	* This function copies the search result into the clipboard
 	*/
  void slotCopySearchResult();
  /**
 	* This slot saves the search result to disk.
 	*/
  void slotSaveSearchResultWithKeytext();
  /**
 	* This function copies the search result into the clipboard
 	*/
  void slotCopySearchResultWithKeytext();

signals: // Signals
  /**
 	* Is emitted when a module in the tree was selected.
 	*/
  void moduleSelected(CSwordModuleInfo*);

private:
  ListCSwordModuleInfo*	moduleList;
  QListViewItem*	m_currentItem;
  CSwordModuleInfo*	m_currentModule;

  KPopupMenu* m_popup;	
	KPopupMenu* m_copyPopup;	
	KPopupMenu* m_savePopup;	
	KPopupMenu* m_printPopup;		
};


class CSearchDialogResultView : public QListBox, public CPointers  {
   Q_OBJECT
public:
	CSearchDialogResultView(QWidget *parent=0, const char *name=0);
	virtual ~CSearchDialogResultView();
	/**
	* Initializes the tree of this CGroupmanager
	*/
	void setupTree();
  /**
 	* Initialzes the view of this widget.
 	*/
	void initView();

protected:
	/**
	* Initializes the connections of this class
	*/
  void initConnections();
  void viewportMouseMoveEvent(QMouseEvent *e);
  /**
  * Returns the selected items of this listbox.
  */
  QList<QListBoxItem> selectedItems();
  /**
   Reimplementation to make Drag&Drop work.
  */
  virtual void viewportMousePressEvent( QMouseEvent* e );

public slots:
  /**
 	* Appends the selected item to prinint queue
 	*/
  void printItem();
  /**
 	*
 	*/
	void setModule(CSwordModuleInfo *module) { if (module) m_module = module; setupTree();};
	
protected slots: // Protected slots
	/**
	* Is called before the menu is shown
	*/	
	void popupAboutToShow();
  /**
 	* This slot is called when the current item changed.
 	*/
  void itemChanged(QListBoxItem*);
  /**
 	* Reimplementation from QListBox
 	*/
  void mousePressed(QListBoxItem*);
  /**
 	* Opens the popup menu.
 	*/
  void rightButtonPressed( QListBoxItem*, const QPoint&);
  /**
 	* This slot copies the current active item into the clipboard.
 	*/
  void slotCopyCurrent();
  /**
 	* This slot copies the current active item into the clipboard.
 	*/
  void slotCopyCurrentWithKeytext();
  /**
 	* This slot copies the current active item into the clipboard.
 	*/  	
  void slotSaveCurrent();
  /**
 	* This slot copies the current active item into the clipboard.
 	*/
  void slotSaveCurrentWithKeytext();

private:
  KPopupMenu	*m_popup;
	KPopupMenu* m_copyPopup;	
	KPopupMenu* m_savePopup;	
	KPopupMenu* m_printPopup;		
	
  CSwordModuleInfo*	m_module;
  QListBoxItem	*m_currentItem;
  QPoint m_pressedPos;
  	
signals: // Signals	
	/**
	* Is emitted when a key in the tree was selected.
	*/
	void keySelected(const QString);	
};

#endif
