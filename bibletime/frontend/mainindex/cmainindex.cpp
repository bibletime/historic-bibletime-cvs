/***************************************************************************
                          cmainindex.cpp  -  description
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

//BibleTime includes
#include "cmainindex.h"
#include "cindexitem.h"

#include "backend/creferencemanager.h"
#include "frontend/searchdialog/csearchdialog.h"

#include "resource.h"
#include "tooltipdef.h"
#include "whatsthisdef.h"

//Qt includes
#include <qheader.h>
#include <qwhatsthis.h>
#include <qlistview.h>
#include <qdragobject.h>
#include <qinputdialog.h>

//KDE includes
#include <klocale.h>
#include <kstandarddirs.h>
#include <kpopupmenu.h>
#include <kmessagebox.h>
#include <kglobalsettings.h>

CMainIndex::ToolTip::ToolTip(QWidget* parent) : CToolTip(parent) {
}

void CMainIndex::ToolTip::maybeTip(const QPoint& p) {
	if (!parentWidget()->inherits("CMainIndex"))
		return;
	
	CMainIndex* m = 0;
	if ( !(m = dynamic_cast<CMainIndex*>(parentWidget())) )
		return;	

  CItemBase* i = 0;
	if ( !( i = dynamic_cast<CItemBase*>(m->itemAt(p))) )
		return;
	
	QRect r = m->itemRect(i);
	if (!r.isValid())
		return;
	
	//get type of item and display correct text
	const QString text = i->toolTip();
	if (!text.isEmpty()) {
    QPoint globalPoint = m->viewport()->mapTo(m, p);
		tip(globalPoint, r, text);
	}
}

/*new class : CMainIndex*/
CMainIndex::CMainIndex(QWidget *parent) : KListView(parent),
  m_searchDialog(0), m_toolTip(0), m_itemsMovable(false), m_autoOpenFolder(0), m_autoOpenTimer(this)
{
  qWarning("constructor of CMainIndex!");
  initView();
  initConnections();
}

CMainIndex::~CMainIndex(){
  qWarning("destructor of CMainIndex");

  //find the bookmark folder
  CItemBase* i = 0;
  QListViewItemIterator it( this );
  while ( it.current() != 0 ) {
    i = dynamic_cast<CItemBase*>( it.current() );
    if (i && i->type() == CItemBase::BookmarkFolder) { //found the bookmark folder
      KStandardDirs stdDirs;
    	const QString path = stdDirs.saveLocation("data", "bibletime/");	
      if (!path.isEmpty()) {
        //save the bookmarks to the right file
        if (CBookmarkFolder* f = dynamic_cast<CBookmarkFolder*>(i))
          f->saveBookmarks( path + "bookmarks.xml" );
      }
      break;
    }
    ++it;
  }
}

/** Reimplementation. Adds the given group to the tree. */
void CMainIndex::addGroup(const CItemBase::Type type, const QString language){
  CTreeFolder *i = 0;
  if (type == CItemBase::BookmarkFolder)
    i = new CBookmarkFolder(this);
  else
    i = new CTreeFolder(this, type, language);
  i->init();

  if (!i->childCount() && type != CItemBase::BookmarkFolder)
    delete i;
}


/** Initializes the view. */
void CMainIndex::initView(){
  setRootIsDecorated(true);
 	addColumn(i18n("Caption"));
 	header()->hide();

 	m_toolTip = new ToolTip(this);
  setTooltipColumn(-1);
  setShowToolTips(false);//to disable Qt's tooltips   	
 	QWhatsThis::add(this, WT_GM_WIDGET );
 	 		
	setBackgroundMode(PaletteBase);
	setSorting(-1);
  setFullWidth(true);

  setAcceptDrops( true );
  setDragEnabled( true );
  setDropVisualizer( true );
  setDropHighlighter( true );
  setAutoOpen(true);
 	viewport()->setAcceptDrops(true);
 	setRootIsDecorated(false);
 	setAllColumnsShowFocus(true);
  setItemsMovable(false);
  setSelectionModeExt(Extended);

  //setup the popup menu
  m_popup = new KPopupMenu(this);
  m_popup->insertTitle(i18n("Main index"));

  m_actions.newFolder = new KAction(i18n("Create a new folder"),GROUP_NEW_ICON_SMALL, 0, this, SLOT(createNewFolder()), this);
  m_actions.changeFolder = new KAction(i18n("Change this folder"),GROUP_CHANGE_ICON_SMALL, 0, this, SLOT(changeFolder()), this);

  m_actions.changeBookmark = new KAction(i18n("Change this bookmark"),BOOKMARK_CHANGE_ICON_SMALL, 0, this, SLOT(changeBookmark()), this);
  m_actions.importBookmarks = new KAction(i18n("Import bookmarks"),BOOKMARK_IMPORT_ICON_SMALL, 0, this, SLOT(importBookmarks()), this);
  m_actions.exportBookmarks = new KAction(i18n("Export bookmarks"),BOOKMARK_EXPORT_ICON_SMALL, 0, this, SLOT(exportBookmarks()), this);
  m_actions.printBookmarks = new KAction(i18n("Print bookmarks"),BOOKMARK_PRINT_ICON_SMALL, 0, this, SLOT(printBookmarks()), this);

  m_actions.deleteEntries = new KAction(i18n("Remove selected item(s)"),ITEMS_DELETE_ICON_SMALL, 0, this, SLOT(deleteEntries()), this);

  m_actions.searchInModules = new KAction(i18n("Search in selected module(s)"),MODULE_SEARCH_ICON_SMALL, 0, this, SLOT(searchInModules()), this);
  m_actions.unlockModule = new KAction(i18n("Unlock this module"),MODULE_UNLOCK_ICON_SMALL, 0, this, SLOT(unlockModule()), this);
  m_actions.aboutModule = new KAction(i18n("About this module"),MODULE_ABOUT_ICON_SMALL, 0, this, SLOT(aboutModule()), this);


  m_actions.newFolder->plug(m_popup);
  m_actions.changeFolder->plug(m_popup);
  (new KActionSeparator(this))->plug(m_popup);
  m_actions.changeBookmark->plug(m_popup);
  m_actions.importBookmarks->plug(m_popup);
  m_actions.exportBookmarks->plug(m_popup);
  m_actions.printBookmarks->plug(m_popup);
  (new KActionSeparator(this))->plug(m_popup);
  m_actions.deleteEntries->plug(m_popup);
  (new KActionSeparator(this))->plug(m_popup);
  m_actions.searchInModules->plug(m_popup);
  m_actions.unlockModule->plug(m_popup);
  m_actions.aboutModule->plug(m_popup);
}

/** Initialize the SIGNAL<->SLOT connections */
void CMainIndex::initConnections(){
  connect(this, SIGNAL(executed(QListViewItem*)),
    SLOT(slotExecuted(QListViewItem*)));
  connect(this, SIGNAL(dropped(QDropEvent*, QListViewItem*)),
    SLOT(dropped(QDropEvent*, QListViewItem*)));
//  connect(this, SIGNAL(moved( QPtrList<QListViewItem>& items, QPtrList<QListViewItem>& afterFirst, QPtrList<QListViewItem>& afterNow)),
//    SLOT(moved( QPtrList<QListViewItem>& items, QPtrList<QListViewItem>& afterFirst, QPtrList<QListViewItem>& afterNow)));
  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
    SLOT(contextMenu(KListView*, QListViewItem*, const QPoint&)));
  connect(&m_autoOpenTimer, SIGNAL(timeout()),
    this, SLOT(autoOpenTimeout()));
}

/** Is called when an item was clicked/double clicked. */
void CMainIndex::slotExecuted( QListViewItem* i ){
  CItemBase* ci = dynamic_cast<CItemBase*>(i);
  if (!ci)
    return;

  if (ci->isFolder()) {
    i->setOpen(!i->isOpen());
  }
  else if (CModuleItem* m = dynamic_cast<CModuleItem*>(i))  { //clicked on a module
    CSwordModuleInfo* mod = m->module();
    ListCSwordModuleInfo modules;
    modules.append(mod);
    emit modulesChosen(modules, QString::null);
  }
  else if (CBookmarkItem* b = dynamic_cast<CBookmarkItem*>(i) ) { //clicked on a bookmark
    CSwordModuleInfo* mod = b->module();
    ListCSwordModuleInfo modules;
    modules.append(mod);

    emit modulesChosen(modules, b->key());
  }
}

/** Reimplementation. Returns the drag object for the current selection. */
QDragObject* CMainIndex::dragObject() {
  if (CBookmarkItem* bookmark = dynamic_cast<CBookmarkItem*>(currentItem())) {
    const QString ref = bookmark->key();
    const QString mod = bookmark->module()->name();

	  QTextDrag* d = new QTextDrag(CReferenceManager::encodeReference(mod,ref), viewport());
    d->setSubtype(BOOKMARK);
    return d;
  }
  return 0;
}

/** Reimplementation from KListView. Returns true if the drag is acceptable for the listview. */
bool CMainIndex::acceptDrag( QDropEvent* event ) const {
  qWarning("CMainIndex::acceptDrag( QDropEvent* event )");
  if (m_itemsMovable) {
    qWarning("return true");
    return true;
  }

  QPoint pos = contentsToViewport(event->pos());
  if (QListViewItem* i = itemAt(pos)) {
    return i->acceptDrop(event);
  }
  return false;
}

/** No descriptions */
void CMainIndex::initTree(){
  addGroup(CItemBase::BookmarkFolder, QString::fromLatin1("*"));
  addGroup(CItemBase::BibleModuleFolder, QString::fromLatin1("*"));
  addGroup(CItemBase::BookModuleFolder, QString::fromLatin1("*"));
  addGroup(CItemBase::CommentaryModuleFolder, QString::fromLatin1("*"));
  addGroup(CItemBase::DevotionalModuleFolder, QString::fromLatin1("*"));
  addGroup(CItemBase::GlossaryModuleFolder, QString::fromLatin1("*"));
  addGroup(CItemBase::LexiconModuleFolder, QString::fromLatin1("*"));
}

/** No descriptions */
void CMainIndex::dropped( QDropEvent* e, QListViewItem* after){
  qWarning("CMainIndex::dropped( QDropEvent* e, QListViewItem* after)");
//  if (m_itemsMovable)
//    KListView::dropped(e, after);
  if (CItemBase* i = dynamic_cast<CItemBase*>(after))
    i->dropped(e);
}

/** Opens the searchdialog using the given modules using the given search text. */
void CMainIndex::openSearchDialog( ListCSwordModuleInfo modules, const QString searchText){
	if (!m_searchDialog)
		m_searchDialog = new CSearchDialog(modules,0);
	else
		m_searchDialog->setModuleList(modules);
  m_searchDialog->setSearchText(searchText);
  m_searchDialog->show();
  m_searchDialog->raise();
 	if (!searchText.isEmpty())
    m_searchDialog->startSearch();	

}

/** No descriptions */
void CMainIndex::emitModulesChosen( ListCSwordModuleInfo modules, QString key ){
  emit modulesChosen(modules, key);
}

/** Returns the correct KAction object for the given type of action. */
KAction* CMainIndex::action( const CItemBase::MenuAction type ){
  switch (type) {
    case CItemBase::NewFolder:
      return m_actions.newFolder;
    case CItemBase::ChangeFolder:
      return m_actions.changeFolder;

    case CItemBase::ChangeBookmark:
      return m_actions.changeBookmark;
    case CItemBase::ImportBookmarks:
      return m_actions.importBookmarks;
    case CItemBase::ExportBookmarks:
      return m_actions.exportBookmarks;
    case CItemBase::PrintBookmarks:
      return m_actions.printBookmarks;

    case CItemBase::DeleteEntries:
      return m_actions.deleteEntries;
    case CItemBase::SearchInModules:
      return m_actions.searchInModules;
    case CItemBase::UnlockModule:
      return m_actions.unlockModule;
    case CItemBase::AboutModule:
      return m_actions.aboutModule;
    default:
      return 0;
  };
}

/** Shows the context menu at the given position. */
void CMainIndex::contextMenu(KListView* list, QListViewItem* i, const QPoint& p){
  //setup menu entries depending on current selection
  QPtrList<QListViewItem> items = selectedItems();

  if (items.count() == 0) { //special handling for no selection

  }
  else if (items.count() == 1) { //special handling for one selected item
    CItemBase* item = dynamic_cast<CItemBase*>(i);
    CItemBase::MenuAction actionType;
    for (int index = CItemBase::ActionBegin; index <= CItemBase::ActionEnd; ++index) {
      actionType = static_cast<CItemBase::MenuAction>(index);
      if (KAction* a = action(actionType))
        a->setEnabled(item->enableAction(actionType));
    }
  }
  else {
    //first disable all actions
    CItemBase::MenuAction actionType;
    for (int index = CItemBase::ActionBegin; index <= CItemBase::ActionEnd; ++index) {
      actionType = static_cast<CItemBase::MenuAction>(index);
      if (KAction* a = action(actionType))
        a->setEnabled(false);
    }

    for (int index = CItemBase::ActionBegin; index <= CItemBase::ActionEnd; ++index) {
      actionType = static_cast<CItemBase::MenuAction>(index);
      bool enableAction = isMultiAction(actionType);
      for (items.first(); items.current(); items.next()) {
        CItemBase* i = dynamic_cast<CItemBase*>(items.current());
        enableAction = enableAction && i->enableAction(actionType);
      }
      if (enableAction) {
        KAction* a = action(actionType) ;
        if (i && a)
          a->setEnabled(enableAction);
      }
    }
  }
  m_popup->exec(p);
}

/** Adds a new subfolder to the current item. */
void CMainIndex::createNewFolder(){
  if (CFolderBase* i = dynamic_cast<CFolderBase*>(currentItem()) ) {
    i->newSubFolder();
  }
}

/** Opens a dialog to change the current folder. */
void CMainIndex::changeFolder(){
  if (CFolderBase* i = dynamic_cast<CFolderBase*>(currentItem()) ) {
    i->rename();
  }
}

/** Changes the current bookmark. */
void CMainIndex::changeBookmark(){
  if (CBookmarkItem* i = dynamic_cast<CBookmarkItem*>(currentItem()) ) {
    i->rename();
  }
}

/** Exports the bookmarks being in the selected folder. */
void CMainIndex::exportBookmarks(){
  if (CBookmarkFolder* i = dynamic_cast<CBookmarkFolder*>(currentItem()) ) {
    i->exportBookmarks();
  }
}

/** Import bookmarks from a file and add them to the selected folder. */
void CMainIndex::importBookmarks(){
  if (CBookmarkFolder* i = dynamic_cast<CBookmarkFolder*>(currentItem()) ) {
    i->importBookmarks();
  }
}

/** Prints the selected bookmarks. */
void CMainIndex::printBookmarks(){
  QPtrList<QListViewItem> items = selectedItems();
  for (items.first(); items.current(); items.next()) {
    if (CBookmarkItem* i = dynamic_cast<CBookmarkItem*>(items.current())) {
      i->print();
    }
  }
}

/** Deletes the selected entries. */
void CMainIndex::deleteEntries(){
  QPtrList<QListViewItem> items = selectedItems();
  QPtrList<QListViewItem> deleteItems;

  for (items.first(); items.current(); items.next()) {
    if (CItemBase* i = dynamic_cast<CItemBase*>(items.current())) {
      if (i->enableAction(CItemBase::DeleteEntries)) {
        deleteItems.append(i);
      }
    }
  }

  deleteItems.setAutoDelete(true);
  deleteItems.clear();
}

/** Opens the searchdialog for the selected modules. */
void CMainIndex::searchInModules(){
  QPtrList<QListViewItem> items = selectedItems();
  ListCSwordModuleInfo modules;
  for (items.first(); items.current(); items.next()) {
    if (CModuleItem* i = dynamic_cast<CModuleItem*>(items.current())) {
      modules.append(i->module());
    }
  }
  if (modules.count())
    openSearchDialog(modules, QString::null);
}

/** Unlocks the current module. */
void CMainIndex::unlockModule(){
  if (CModuleItem* i = dynamic_cast<CModuleItem*>(currentItem())) {
  	bool ok;
  	QString unlockKey = QInputDialog::getText(i18n("BibleTime - Unlock module"),i18n("Enter the key to unlock the module!"),QLineEdit::Normal, i->module()->config(CSwordModuleInfo::CipherKey), &ok);
  	if (ok) {
  		CSwordModuleInfo::UnlockErrorCode ret = i->module()->unlock( unlockKey );
  		if ( ret != CSwordModuleInfo::noError) {
  			//an error occured
  			switch (ret) {
  				case CSwordModuleInfo::noPermission:				
  					break;
  				case CSwordModuleInfo::wrongUnlockKey:				
  					break;
  				case CSwordModuleInfo::notLocked:
  					break;
  				default:
  					break;
  			}
  		}
  	}
  }
}

/** Shows information about the current module. */
void CMainIndex::aboutModule(){
  if (CModuleItem* i = dynamic_cast<CModuleItem*>(currentItem())) {
  	KMessageBox::about(this, i->aboutInfo(), i->module()->config(CSwordModuleInfo::Description), false);
  }
}

/** Reimplementation. Takes care of movable items. */
void CMainIndex::startDrag(){
  QPtrList<QListViewItem> items = selectedItems();
  m_itemsMovable = true;
  for (items.first(); items.current() && m_itemsMovable; items.next()) {
    if (CItemBase* i = dynamic_cast<CItemBase*>(items.current())) {
      m_itemsMovable = m_itemsMovable && i->isMovable();
    }
    else {
      m_itemsMovable = false;
    }
  }
  qWarning("movable? %i", m_itemsMovable);
  KListView::startDrag();
}

/** Reimplementation to support the items dragEnter and dragLeave functions. */
void CMainIndex::contentsDragMoveEvent( QDragMoveEvent* event ){
  if ( CItemBase* i = dynamic_cast<CItemBase*>( itemAt( contentsToViewport(event->pos())) )) {
  	if (i->acceptDrop(event) && i->isFolder() && !i->isOpen() && autoOpen()) {
      if (m_autoOpenFolder != i)
        m_autoOpenTimer.stop();
      m_autoOpenFolder = i;
      m_autoOpenTimer.start( 400, true );
    }
    else
      m_autoOpenFolder = 0;
  }
  else
    m_autoOpenFolder = 0;

  KListView::contentsDragMoveEvent(event);
}

void CMainIndex::autoOpenTimeout(){
  m_autoOpenTimer.stop();
  if (m_autoOpenFolder && !m_autoOpenFolder->isOpen() && m_autoOpenFolder->childCount()) {
    m_autoOpenFolder->setOpen(true);
  }
}

/** No descriptions */
void CMainIndex::contentsDragLeaveEvent( QDragLeaveEvent* e ){
  m_autoOpenTimer.stop();

  KListView::contentsDragLeaveEvent(e);
}

/** Returns true if more than one netry is supported by this action type. Returns false for actions which support only one entry, e.g. about module etc. */
const bool CMainIndex::isMultiAction( const CItemBase::MenuAction type ) const {
  switch (type) {
    case CItemBase::NewFolder:
      return false;
    case CItemBase::ChangeFolder:
      return false;

    case CItemBase::ChangeBookmark:
      return false;
    case CItemBase::ImportBookmarks:
      return false;
    case CItemBase::ExportBookmarks:
      return false;
    case CItemBase::PrintBookmarks:
      return true;

    case CItemBase::DeleteEntries:
      return true;

    case CItemBase::SearchInModules:
      return true;
    case CItemBase::UnlockModule:
      return false;
    case CItemBase::AboutModule:
      return false;
  }
  return false;
}

/** Is called when items should be moved. */
void CMainIndex::moved( QPtrList<QListViewItem>& items, QPtrList<QListViewItem>& afterFirst, QPtrList<QListViewItem>& afterNow){
  qWarning("moved( QPtrList<QListViewItem>& items, QPtrList<QListViewItem>& afterFirst, QPtrList<QListViewItem>& afterNow)");
}
