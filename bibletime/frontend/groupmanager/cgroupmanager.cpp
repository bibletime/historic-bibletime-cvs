/***************************************************************************
                          cgroupmanager.cpp  -  description
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

#define BOOKMARK_FORMAT_VERSION	1


//own includes
#include "cgroupmanager.h"
#include "cgroupmanageritem.h"

#include "../chtmldialog.h"
#include "../../structdef.h"
#include "../../whatsthisdef.h"
#include "../../ressource.h"
#include "../ctoolclass.h"
#include "../searchdialog/csearchdialog.h"
#include "../../printing/cprintitem.h"
#include "../../printing/cprinter.h"

#include "../../backend/ckey.h"
#include "../../backend/sword_backend/cswordversekey.h"
#include "../../backend/sword_backend/cswordldkey.h"
#include "../../backend/sword_backend/cswordbiblemoduleinfo.h"
#include "../../backend/sword_backend/cswordcommentarymoduleinfo.h"
#include "../../backend/sword_backend/cswordlexiconmoduleinfo.h"

//QT includes
#include <qheader.h>
#include <qevent.h>
#include <qmessagebox.h>
#include <qdragobject.h>
#include <qstringlist.h>
#include <qvaluelist.h>
#include <qwhatsthis.h>
#include <qinputdialog.h>

//KDE includes
#include <klocale.h>
#include <kstddirs.h>
#include <kiconloader.h>
#include <kconfig.h>
#include <kapp.h>
#include <kpopupmenu.h>
#include <kglobalsettings.h>
#include <kdestyle.h>
#include <kpassdlg.h>
#include <klineeditdlg.h>
#include <kfiledialog.h>
#include <kmessagebox.h>

//Sword includes
#include <swmodule.h>


CGroupManager::ToolTip::ToolTip(QWidget* parent) : QToolTip(parent) {
}

void CGroupManager::ToolTip::maybeTip(const QPoint& p) {
	qDebug("CGroupManager::ToolTip::maybeTip(const QPoint& p)");
	
	if (!parentWidget()->inherits("CGroupManager"))
		return;
	
	CGroupManager* m = dynamic_cast<CGroupManager*>(parentWidget());
	ASSERT(m);
	
	CGroupManagerItem* i = dynamic_cast<CGroupManagerItem*>(m->itemAt(p));
	ASSERT(i);
	if (!i)
		return;
	
	QRect r = m->itemRect(i);
	if (!r.isValid())
		return;
	
	//get tyope of item anddisplay correct text
	QString text = i->getToolTip();
	if (!text.isEmpty()) {
		tip(r, text);		
	}
}


CGroupManager::CGroupManager(CImportantClasses* importantClasses, QWidget *parent, const char *name, ListCSwordModuleInfo *swordList )
	: KListView(parent, name) {
	
	qDebug("CGroupManager::constructor");
	config = new KConfig("bt-groupmanager", false, false );

	m_important = importantClasses;
  m_menu = false;
	searchDialog = 0;
	m_pressedItem = 0;
		
	m_swordList = swordList;
	ASSERT(m_swordList);
	initView();	
	initConnections();
	readSettings();
	
	qDebug("finished CGroupManager::CGroupManager");	
}

CGroupManager::~CGroupManager(){	
	saveSettings();
	if (config)
		delete config;
}

/** Initializes the tree of this CGroupmanager */
void CGroupManager::setupSwordTree() {
	//read in all module entries
	qDebug("CGroupManager::setupSwordTree()");
	readGroups( config, 0);
	readSwordModules( config, 0);
	readSwordBookmarks( config, 0 );	
	
	setupStandardSwordTree();
}

void CGroupManager::setupSwordTree(ListCSwordModuleInfo* m_swordList){
	ASSERT(m_swordList);

	if (m_swordList) {
		this->m_swordList = m_swordList;
		setupSwordTree();	
	};
}

/** Initializes the tree of this CGroupmanager */
void CGroupManager::setupStandardSwordTree() {
	qDebug("CGroupManager::setupStandardSwordTree()");
	ASSERT(m_swordList);	
	if (!m_swordList) {
		qDebug("m_swordList is ZERO!");
		return;
	}

	bool initialized = false;
	if (config->readBoolEntry("initialized", false)) {
		initialized = true;
	}
	
	CSwordModuleInfo* moduleInfo = 0;
		
	CGroupManagerItem* item = 0;
	QListViewItemIterator it( this );
	
	#define CGROUPMANAGER_GROUP(name, groupItem) \
	{ \
		it = QListViewItemIterator(this); \
		item = 0; \
		for ( ; it.current(); ++it ) { \
			if ( it.current() ) { \
				item = (CGroupManagerItem*)it.current(); \
				ASSERT(item); \
				if (item && (item->text(0) == name) ) { \
					groupItem = item; \
					break; \
				} \
				else \
					groupItem = 0; \
			} \
		}	\
		if (!groupItem) \
			groupItem = new CGroupManagerItem(this, name, QString::null, 0, 0, CGroupManagerItem::Group); \
	}
	CGroupManagerItem* bibleGroup = 0;
	CGROUPMANAGER_GROUP(i18n("Bibles"), bibleGroup);
		
	CGroupManagerItem* commentaryGroup = 0;
	CGROUPMANAGER_GROUP(i18n("Commentaries"), commentaryGroup);
		
	CGroupManagerItem* lexiconGroup = 0;
	CGROUPMANAGER_GROUP(i18n("Lexicons"), lexiconGroup);
		
	#undef CGROUPMANAGER_GROUP

	qDebug("create new modules");	
	ASSERT(m_swordList);
	for(moduleInfo = m_swordList->first(); moduleInfo !=0; moduleInfo = m_swordList->next()) {
		bool alreadyCreated = false;  	
  	QListViewItemIterator it( this );
		for ( ; it.current(); ++it ) {
			CGroupManagerItem* item = dynamic_cast<CGroupManagerItem*>(it.current());
			if (item) {
				if (item->moduleInfo() && item->moduleInfo() == moduleInfo) { //we have it already
					alreadyCreated = true;
				}
			}
		}
		ASSERT(moduleInfo);
		if ( moduleInfo && !alreadyCreated) {
			if ( dynamic_cast<CSwordCommentaryModuleInfo*>(moduleInfo) ) {	//a Dictionary
				(void) new CGroupManagerItem(commentaryGroup, "",QString::null, moduleInfo,0, CGroupManagerItem::Module);
			}
			else if ( dynamic_cast<CSwordBibleModuleInfo*>(moduleInfo) ) {	//a Bible
				(void)new CGroupManagerItem(bibleGroup, "",QString::null, moduleInfo, 0,CGroupManagerItem::Module);
			}
			else if ( dynamic_cast<CSwordLexiconModuleInfo*>(moduleInfo) ) {	//a Dictionary
				(void) new CGroupManagerItem(lexiconGroup, "",QString::null, moduleInfo,0, CGroupManagerItem::Module);
			}
		}
	}

	// Now delete the groupes which have no child items
	qDebug("delete empty groups");
	if (bibleGroup->childCount() == 0)
		delete bibleGroup;
	if (lexiconGroup->childCount() == 0)
		delete lexiconGroup;
	if (commentaryGroup->childCount() == 0)
		delete commentaryGroup;
}

void CGroupManager::setupStandardSwordTree(ListCSwordModuleInfo* m_swordList){
	ASSERT(m_swordList);

	if (m_swordList) {
		this->m_swordList = m_swordList;
		setupStandardSwordTree();	
	};
}

/** Initializes the connections of this class */
void CGroupManager::initConnections(){
	connect(popupMenu, SIGNAL(aboutToShow()),
		this, SLOT(slotPopupAboutToShow()));
}

/**  */
void CGroupManager::saveSettings(){	
	qDebug("CGroupManager::saveSettings");
	KConfigGroupSaver groupSaver(config, "Groupmanager");
		
	//save column width	
	config->writeEntry("initialized", true);
	config->writeEntry("First column", columnWidth(0));
//	config->writeEntry("Second column", columnWidth(1));
	
	//save the bookmarks and the groups
	saveGroups(config,0);
	saveSwordModules(config,0);
	saveSwordBookmarks(config, 0);

	config->sync();
}

/**  */
void CGroupManager::readSettings(){
	qDebug("CGroupManager::readSettings()");
	KConfigGroupSaver groupSaver(config, "Groupmanager");
	if (config->readBoolEntry("initialized")) {
		setupSwordTree();
	}
	else {
		setupStandardSwordTree();
	};

	setColumnWidth(0, config->readNumEntry("First column", width()) );
}

/** Initializes this widget */
void CGroupManager::initView(){
 	m_toolTip = new ToolTip(this);
 	
 	QWhatsThis::add(this, WT_GM_WIDGET );
 	m_singleClick = KGlobalSettings::singleClick();
 	 		
	setBackgroundMode(PaletteBase);
//  setTreeStepSize(15);
	setSorting(-1);
	 	
 	viewport()->setAcceptDrops(true);
	setSelectionMode(QListView::Extended);
 	setRootIsDecorated(false);
 	setAllColumnsShowFocus ( true );
 			 	
 	addColumn(i18n("Caption"));
 	header()->hide();
	
	popupMenu = new KPopupMenu(this);
	popupMenu->insertTitle(i18n("Groupmanager"));
//	popupMenu->insertItem(PRESENTER_NEW_ICON_SMALL,i18n("New presenter"),
//		this, SLOT(slotCreateNewPresenter()),0,ID_GM_PRESENTER_CREATE);
//	popupMenu->setWhatsThis(ID_GM_PRESENTER_CREATE, WT_GM_NEW_PRESENTER);
//	popupMenu->insertSeparator();
	popupMenu->insertItem(GROUP_NEW_ICON_SMALL, i18n("Create new folder"),
		this, SLOT(slotCreateNewGroup()),0,ID_GM_GROUP_CREATE);
	popupMenu->setWhatsThis(ID_GM_GROUP_CREATE, WT_GM_NEW_GROUP);	
	popupMenu->insertItem(GROUP_CHANGE_ICON_SMALL, i18n("Change this folder"),
		this, SLOT(slotChangeGroup()),0,ID_GM_GROUP_CHANGE);
	popupMenu->setWhatsThis(ID_GM_GROUP_CHANGE, WT_GM_CHANGE_GROUP);		
	popupMenu->insertSeparator();	
	popupMenu->insertItem(BOOKMARK_CHANGE_ICON_SMALL,i18n("Change this bookmark"),
		this,SLOT(slotChangeBookmark()),0,ID_GM_BOOKMARK_CHANGE);
	popupMenu->setWhatsThis(ID_GM_BOOKMARK_CHANGE, WT_GM_CHANGE_BOOKMARK);
	popupMenu->insertItem(BOOKMARK_IMPORT_ICON_SMALL,i18n("Import bookmarks"),
		this,SLOT(slotImportBookmarks()),0,ID_GM_BOOKMARKS_IMPORT);	
	popupMenu->setWhatsThis(ID_GM_BOOKMARKS_IMPORT, WT_GM_IMPORT_BOOKMARKS);
	popupMenu->insertItem(BOOKMARK_EXPORT_ICON_SMALL,i18n("Export bookmarks"),
		this,SLOT(slotExportBookmarks()),0,ID_GM_BOOKMARKS_EXPORT);		
	popupMenu->setWhatsThis(ID_GM_BOOKMARKS_EXPORT, WT_GM_EXPORT_BOOKMARKS);	
	popupMenu->insertItem(BOOKMARK_PRINT_ICON_SMALL,i18n("Print bookmark"),
		this,SLOT(slotPrintBookmark()),0,ID_GM_BOOKMARK_PRINT);
	popupMenu->setWhatsThis(ID_GM_BOOKMARK_PRINT, WT_GM_PRINT_BOOKMARK);
	popupMenu->insertItem(ITEMS_DELETE_ICON_SMALL, i18n("Remove selected item(s)"),
		this, SLOT(slotDeleteSelectedItems()),0,ID_GM_ITEMS_DELETE);
	popupMenu->insertSeparator();
	popupMenu->insertItem(MODULE_SEARCH_ICON_SMALL,i18n("Search in module(s)"),
		this, SLOT(slotSearchSelectedModules()),0,ID_GM_MODULES_SEARCH);
	popupMenu->insertSeparator();
	popupMenu->insertItem(MODULE_UNLOCK_ICON_SMALL,i18n("Set unlock key"),
		this, SLOT(slotUnlockModule()),0,ID_GM_MODULE_UNLOCK);	
	popupMenu->setWhatsThis(ID_GM_MODULE_UNLOCK, WT_GM_UNLOCK_MODULE);	
	popupMenu->insertItem(MODULE_ABOUT_ICON_SMALL, i18n("About module"),
		this, SLOT(slotShowAbout()),0,ID_GM_MODULE_ABOUT);
	popupMenu->setWhatsThis(ID_GM_MODULE_ABOUT, WT_GM_ABOUT_MODULE);
}

/** Deletes the item given as parameter */
void CGroupManager::slotDeleteSelectedItems(){
	QListViewItemIterator it( this );
	for ( ; it.current(); ++it ) {
		if ( it.current()->isSelected() )
			delete it.current();
	}
}	

/** call the search dialog */
void CGroupManager::slotSearchSelectedModules() {
	qDebug("CGroupManager::searchSelectedModules");
	ListCSwordModuleInfo searchList;
	
	CGroupManagerItem *item = 0;
	
	QListViewItemIterator it( this );
	for ( ; it.current(); ++it ) {
		if ( it.current()->isSelected()) {
			item = (CGroupManagerItem*)it.current();
			ASSERT(item);
			if (item->type() == CGroupManagerItem::Module)
				searchList.append(item->moduleInfo());
			else
				qDebug("Can't append a group to the search list");
		}
	}
	if (!searchDialog)
		searchDialog = new CSearchDialog(m_important,0,0);
	connect(searchDialog, SIGNAL(finished()),
		this, SLOT(slotDeleteSearchdialog()));
	searchDialog->setModuleList(&searchList);
	searchDialog->show();
}	

void CGroupManager::searchBookmarkedModule(QString text, CGroupManagerItem* item) {	
	qDebug("CGroupManager::searchBookmarkedModule");
  if (!item->moduleInfo())
  	return;
	ListCSwordModuleInfo searchList;
	searchList.append(item->moduleInfo());
	
	if (!searchDialog)
		searchDialog = new CSearchDialog(m_important, 0,0);
	searchDialog->setModuleList(&searchList);
  searchDialog->setSearchText(text);
	searchDialog->show();
}	

/**  */
void CGroupManager::createNewBookmark(CGroupManagerItem* parent, CModuleInfo* module, const QString ref){
	qDebug("CGroupManager::createNewBookmark(CGroupManagerItem* parent, CModuleInfo* module)");
	if (!module) {
		qDebug("module is invalid. Return.");
		return;
	}

//	bool isOk = false;
	CGroupManagerItem* myItem = 0;	
	QString description = QInputDialog::getText(i18n("BibleTime - Bookmark description"), i18n("Please enter here the description:"), QString::null,0);
	//= KLineEditDlg::getText(i18n("Please enter the description of the new bookmark"), QString::null, &isOk, 0);
	setFocus();
	
		
//	if (isOk) {
    myItem = 0;
    if ( parent && (parent->type() == CGroupManagerItem::Group) ) {
    	myItem = new CGroupManagerItem(parent,QString::null,QString::null,module, 0, CGroupManagerItem::Bookmark);
    	parent->setOpen(true);
    }
    else
			myItem = new CGroupManagerItem(this,QString::null,QString::null,module, 0, CGroupManagerItem::Bookmark);
		
		if (myItem && (CSwordModuleInfo*)module) {	//it's a Sword module
			CSwordModuleInfo* swordModule = (CSwordModuleInfo*)module;
			if (dynamic_cast<CSwordBibleModuleInfo*>(swordModule) ) {	//a bible or commentary
				qDebug("create new versekey for bookmark");
				CSwordVerseKey* key = new CSwordVerseKey(swordModule);
				key->setKey(ref);				
				myItem->setBookmarkKey(key);	//the key is deleted by the groupmmanager item
			}
			else if (dynamic_cast<CSwordLexiconModuleInfo*>(swordModule) ) {	//a lexicon module
				qDebug("create new ldkey for bookmark");			
				CSwordLDKey* key = new CSwordLDKey(swordModule);
				key->setKey(ref);
				myItem->setBookmarkKey(key);	//the key is deleted by the groupmmanager item
			}			
		}		
    if (myItem)	//set the description
			myItem->setDescription(description);
//  }
}


/** Changes the current item */
void CGroupManager::slotChangeBookmark(){
	if (!m_pressedItem)
		return;
		
	bool isOk;
	QString description = QInputDialog::getText(i18n("BibleTime - Change bookmark description"),i18n("Please change the description of the item!"), m_pressedItem->description(), &isOk, 0);	
	if (isOk)
		m_pressedItem->setDescription( description );
}

/** Changes a group. */
void CGroupManager::slotChangeGroup(){
	if (!m_pressedItem)
		return;		
	
	bool isOk;
	QString description = QInputDialog::getText(i18n("BibleTime - Change folder"),i18n("Please change the name of the group!"), m_pressedItem->text(0), &isOk, 0);	
	if (isOk)
		m_pressedItem->setText( 0, description );
}

/** Is called before the menu is shown */
void CGroupManager::slotPopupAboutToShow(){
	m_menu = true;
	
	if (m_pressedItem) {
		if (m_pressedItem->type() == CGroupManagerItem::Module) {
			bool moduleIsEncrypted = m_pressedItem->moduleInfo()->isEncrypted();
			
			popupMenu->setItemEnabled(ID_GM_PRESENTER_CREATE, true);			
			
			popupMenu->setItemEnabled(ID_GM_GROUP_CREATE, false);
			popupMenu->setItemEnabled(ID_GM_GROUP_CHANGE, false);
			
			popupMenu->setItemEnabled(ID_GM_BOOKMARK_CHANGE, false);
			popupMenu->setItemEnabled(ID_GM_BOOKMARKS_IMPORT, false);
			popupMenu->setItemEnabled(ID_GM_BOOKMARKS_EXPORT, false);			
			popupMenu->setItemEnabled(ID_GM_BOOKMARK_PRINT, false);			

			popupMenu->setItemEnabled(ID_GM_ITEMS_DELETE, true);
						
			popupMenu->setItemEnabled(ID_GM_MODULES_SEARCH, true);	
			popupMenu->setItemEnabled(ID_GM_MODULE_UNLOCK, moduleIsEncrypted);			
			popupMenu->setItemEnabled(ID_GM_MODULE_ABOUT, true);
		}
		else if (m_pressedItem->type() == CGroupManagerItem::Bookmark) {
			popupMenu->setItemEnabled(ID_GM_PRESENTER_CREATE, true);
						
			popupMenu->setItemEnabled(ID_GM_GROUP_CREATE, false);			
			popupMenu->setItemEnabled(ID_GM_GROUP_CHANGE, false);
						
			popupMenu->setItemEnabled(ID_GM_BOOKMARK_CHANGE, true);
			popupMenu->setItemEnabled(ID_GM_BOOKMARKS_IMPORT, false);
			popupMenu->setItemEnabled(ID_GM_BOOKMARKS_EXPORT, false);			
			popupMenu->setItemEnabled(ID_GM_BOOKMARK_PRINT, true);
			
			popupMenu->setItemEnabled(ID_GM_ITEMS_DELETE, true);						
			
			popupMenu->setItemEnabled(ID_GM_MODULES_SEARCH, false);
			popupMenu->setItemEnabled(ID_GM_MODULE_UNLOCK, false);			
			popupMenu->setItemEnabled(ID_GM_MODULE_ABOUT, false);

		}
		else if (m_pressedItem->type() == CGroupManagerItem::Group) {
			popupMenu->setItemEnabled(ID_GM_PRESENTER_CREATE, false);
						
			popupMenu->setItemEnabled(ID_GM_GROUP_CREATE, true);
			popupMenu->setItemEnabled(ID_GM_GROUP_CHANGE, true);
												
			popupMenu->setItemEnabled(ID_GM_BOOKMARK_CHANGE, false);							
			popupMenu->setItemEnabled(ID_GM_BOOKMARKS_IMPORT, true);
			popupMenu->setItemEnabled(ID_GM_BOOKMARKS_EXPORT, true);
			popupMenu->setItemEnabled(ID_GM_BOOKMARK_PRINT, false);
				
			popupMenu->setItemEnabled(ID_GM_ITEMS_DELETE, true);

			popupMenu->setItemEnabled(ID_GM_MODULES_SEARCH, false);
			popupMenu->setItemEnabled(ID_GM_MODULE_UNLOCK, false);
			popupMenu->setItemEnabled(ID_GM_MODULE_ABOUT, false);
		}		
	}
	else {
			popupMenu->setItemEnabled(ID_GM_PRESENTER_CREATE, false);
						
			popupMenu->setItemEnabled(ID_GM_GROUP_CREATE, true);
			popupMenu->setItemEnabled(ID_GM_GROUP_CHANGE, false);
			
			popupMenu->setItemEnabled(ID_GM_BOOKMARK_CHANGE, false);				
			popupMenu->setItemEnabled(ID_GM_BOOKMARKS_IMPORT, true);
			popupMenu->setItemEnabled(ID_GM_BOOKMARKS_EXPORT, true);
			
			popupMenu->setItemEnabled(ID_GM_ITEMS_DELETE, false);
						
			popupMenu->setItemEnabled(ID_GM_MODULES_SEARCH, false);
			popupMenu->setItemEnabled(ID_GM_MODULE_UNLOCK, false);			
			popupMenu->setItemEnabled(ID_GM_MODULE_ABOUT, false);			
	}
}

/**  */
void CGroupManager::slotShowAbout(){
	ASSERT(m_pressedItem);
	qDebug("create about dialog");
		
	if (!m_pressedItem || !m_pressedItem->moduleInfo())
		return;
	
	CHTMLDialog* dlg = new CHTMLDialog(this, "about dialog");
	ASSERT(dlg);
	
	QString text;
	
	CSwordModuleInfo* module = 0;
	if (dynamic_cast<CSwordModuleInfo*>(m_pressedItem->moduleInfo()))
		module = m_pressedItem->moduleInfo();		
	else {
		qWarning("Invalid module");
		return;
	}		
	ASSERT(module);
		
	QString isWritable;
	if (module->module()->isWritable())
		isWritable = i18n("Yes");
	else
		isWritable = i18n("No");			

	QString hasFootnotes;
//#warning: ToDo: Implementation
	if (module->supportsFeature(CSwordBackend::footnotes))
		hasFootnotes = i18n("Yes");
	else
		hasFootnotes = i18n("No");			
		
	QString hasStrongNumbers;
//#warning: ToDo: Implementation	
	if (module->supportsFeature(CSwordBackend::strongNumbers))
		hasStrongNumbers = i18n("Yes");
	else
		hasStrongNumbers = i18n("No");			
		
	QString unlockKey;
	if ( !module->isEncrypted() )
		unlockKey = i18n("<I>Not necessary!</I>");
	else
		unlockKey = module->getCipherKey();	
				
	text = QString("<HTML><BODY><TABLE border=\"0\" cellspacing=\"0\" cellpadding=\"2\">\
<TR><TD BGCOLOR=\"#0F86D0\" ALIGN=\"CENTER\" COLSPAN=\"2\"><H2>%1</H2></TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%2:</B></TD><TD BGCOLOR=\"#FFE9C8\">%3</TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%4:</B></TD><TD BGCOLOR=\"#FFE9C8\">%5</TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%6:</B></TD><TD BGCOLOR=\"#FFE9C8\">%7</TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%8:</B></TD><TD BGCOLOR=\"#FFE9C8\">%9</TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%10:</B></TD><TD BGCOLOR=\"#FFE9C8\">%11</TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%12:</B></TD><TD BGCOLOR=\"#FFE9C8\">%13</TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%15:</B></TD><TD BGCOLOR=\"#FFE9C8\">%16/TD></TR>\
<TR><TD VALIGN=\"TOP\" BGCOLOR=\"#0F86D0\"><B>%15:</B></TD><TD BGCOLOR=\"#FFE9C8\">%17</TD></TR>\
</TABLE></BODY></HTML>")
	.arg( module->module()->Name() )		
	.arg( i18n("Datapath") )
	.arg( module->getPath() )	
	.arg( i18n("Version") )	
	.arg( module->getVersion() )
	.arg(i18n("Unlock key"))	
	.arg( unlockKey )
	.arg(i18n("Writable"))	
	.arg( isWritable )
	.arg(i18n("Footnotes"))	
	.arg( hasFootnotes )
	.arg(i18n("Strong's numbers"))	
	.arg( hasStrongNumbers )
	.arg(i18n("Description"))	
	.arg( module->getDescription() )
	.arg(i18n("About"))	
	.arg( module->getAboutInformation() );
	
	dlg->setText(text);
	
	dlg->exec();
	delete dlg;	
}

/**  */
void CGroupManager::slotCreateNewPresenter(){
	ASSERT(m_pressedItem);
	
	if (m_pressedItem && m_pressedItem->moduleInfo()) {
		if (m_pressedItem->type() == CGroupManagerItem::Module || m_pressedItem->type() == CGroupManagerItem::Bookmark)
			emit createSwordPresenter( m_pressedItem->moduleInfo(), QString::null );
	}
	else
		qDebug("Probably module of item isn't valid");
}

/**  */
void CGroupManager::contentsDragEnterEvent( QDragEnterEvent* e){
  qDebug("CGroupMAnager::contentsDragEnterEvent");
  //CGroupmanager accapts every QTextDrag
  if (QTextDrag::canDecode(e)) {
    e->accept(true);
    QString str;
	  QCString submime;
  	if (QTextDrag::decode(e,str,submime=BOOKMARK))
  		m_dragType = BOOKMARK;
  	else if (QTextDrag::decode(e,str,submime=REFERENCE))
  		m_dragType = REFERENCE;  	
  	else if (QTextDrag::decode(e,str,submime=MODULE))
  		m_dragType = MODULE;  	
  	else if (QTextDrag::decode(e,str,submime=GROUP))
  		m_dragType = GROUP;  	  		
  	else if (QTextDrag::decode(e,str,submime=TEXT))
  		m_dragType = TEXT;
  	else
  		m_dragType = "";
  	qDebug((const char*)m_dragType.local8Bit());
  }
  else {
  	e->ignore();
  	m_dragType = "";
  }


}

/**  */
void CGroupManager::contentsDragMoveEvent( QDragMoveEvent* e){
  //open folders
  CGroupManagerItem* item = (CGroupManagerItem*)itemAt( contentsToViewport(e->pos()) );			
  e->accept(QTextDrag::canDecode(e));		
	if (item && item->type() == CGroupManagerItem::Group)
		item->setOpen(true);

	m_afterItemDrop = item;
	if (item)
		m_parentItemDrop = (CGroupManagerItem*)item->parent();
	else
		m_parentItemDrop = 0;
	
	QRect tmpRect = drawDropVisualizer(0, m_parentItemDrop, m_afterItemDrop, m_dragType );
	if (tmpRect != oldDragRect ) {
		cleanDropVisualizer();
		oldDragRect = tmpRect;
		if (tmpRect.isValid())
			viewport()->repaint(tmpRect);
	}
}

/**  */
void CGroupManager::contentsDragLeaveEvent( QDragLeaveEvent* e){	
  qDebug("CGroupMAnager::contentsDragLeaveEvent");	
	KListView::contentsDragLeaveEvent(e);	
	cleanDropVisualizer();
	
	oldDragRect = QRect();	
	m_dragType = "";
}

/** This define is used to use one block of code for the different sections (bookmarks,
	* references, modules, groups etc.). We have only to maintain this code.
	*/
#define MOVE_ITEMS  \
			for ( item = m_itemList->last(); item != 0; item = m_itemList->prev()){ \
				if ( item && (item != target) ){ \
					parentItem = (CGroupManagerItem*)item->parent(); \
					if ( isChild(item, target) ) \
						continue; \
					if (parentItem && parentItem != item) \
						parentItem->takeItem(item); \
					else \
						this->takeItem(item); \
					if ( target && target!=item ) \
						if (target->type()==CGroupManagerItem::Group) \
							target->insertItem(item); \
						else{ \
							if (target->parent()) \
								target->parent()->insertItem(item); \
							else \
								this->insertItem(item); \
						}  \
					else \
						this->insertItem(item); \
				} \
			}

/**  */
void CGroupManager::contentsDropEvent( QDropEvent* e){
  qDebug("CGroupMAnager::contentsDropEvent");
  CGroupManagerItem* target = (CGroupManagerItem *)itemAt(contentsToViewport(e->pos()));

  QString str;
  QCString submime;
  if (QTextDrag::decode(e,str,submime=BOOKMARK)){
    //a bookmark was dragged
    qDebug("bookmark decoded");
    if ( e->source() != this->viewport() ){
      qDebug("erroneous drag");
      return;
    }
    if ( !(m_itemList) ){
      qDebug("no item(s) to be dragged");
      return;
    }
    CGroupManagerItem* item = 0;
    CGroupManagerItem* parentItem = 0;
    // Bookmark dragged on module - open a presenter
    if ( (m_pressedItem && m_pressedItem->type() == CGroupManagerItem::Bookmark)
    		&& target && (target->type() == CGroupManagerItem::Module) ){
      emit createSwordPresenter(target->moduleInfo(),m_pressedItem->getKeyText() );
    }
    //move around bookmarks
    else{    	
    	MOVE_ITEMS    	
    }//else
  }//Bookmark
  if (QTextDrag::decode(e,str,submime=GROUP)){
    //a group was dragged
    if ( e->source() != this->viewport() ){
      qDebug("erroneous drag");
      return;
    }
    if ( !(m_itemList) ){
      qDebug("no item(s) to be dragged");
      return;
    }
    CGroupManagerItem* item = 0;
    CGroupManagerItem* parentItem = 0;
    //move around groups    	
    MOVE_ITEMS
  }//group
  if (QTextDrag::decode(e,str,submime=MODULE)){
    //a group was dragged
    if ( e->source() != this->viewport() ){
      qDebug("erroneous drag");
      return;
    }
    if ( !(m_itemList) ){
      qDebug("no item(s) to be dragged");
      return;
    }
    CGroupManagerItem* item = 0;
    CGroupManagerItem* parentItem = 0;
    MOVE_ITEMS
  }//module
  else if (QTextDrag::decode(e,str,submime=REFERENCE)){
    //a reference was dragged
    qDebug("reference decoded");
		QString ref;
    QString mod;
    CToolClass::decodeReference(str,mod,ref);

    CSwordModuleInfo* info = 0;
    for (info=m_swordList->first();(info!=0);info=m_swordList->next())
      if (info->module()->Name() == mod)
      	break;
    if ( (info) && (info->module()->Name() == mod) ){

			if (!target){ //Reference was dragged on no bookmark
				if (info)
					createNewBookmark( 0, info, ref); //CREATE A NEW BOOKMARK
			}
			else{
				switch (target->type()){
					case (CGroupManagerItem::Group):
						if (info)  //Create a bookmark
							createNewBookmark(target,info, ref);
						break;
					case (CGroupManagerItem::Bookmark):
						break;
					case (CGroupManagerItem::Module):
						if (target->moduleInfo()){
							// In bibles or commentaries, the reference is opened
							if (dynamic_cast<CSwordModuleInfo*>(target->moduleInfo()) ) {
								if (dynamic_cast<CSwordBibleModuleInfo*>(target->moduleInfo()) )
									emit createSwordPresenter( target->moduleInfo(),ref );
								else if  (dynamic_cast<CSwordLexiconModuleInfo*>(target->moduleInfo())) // in lexicons, the reference is searched
									searchBookmarkedModule(ref,target);							
							}
						}
						break;
				}//switch;
			}//(target)
    }//(info)
  }//reference
  else if (QTextDrag::decode(e,str,submime=TEXT)){
    //plain text was dragged -> open searchdialog
    if ( (target) && (target->type()==CGroupManagerItem::Module) && (target->moduleInfo()) )
      searchBookmarkedModule(str,target);
  }
	m_dragType = "";
	cleanDropVisualizer();
}
#undef MOVE_ITEMS

/**  */
void CGroupManager::contentsMousePressEvent ( QMouseEvent* e ) {
	qDebug("CGroupManager::cotentsMousePressEvent");	
	m_pressedPos = e->pos();
  m_pressedItem = (CGroupManagerItem*)itemAt(contentsToViewport(m_pressedPos));
  bool open = false;	
  if (m_pressedItem)
  	open = m_pressedItem->isOpen();		
	KListView::contentsMousePressEvent(e);
	if ((e->state() & ControlButton) || (e->state() & ShiftButton))
			return;
			
	if (e->button() == LeftButton) {
		if (m_pressedItem && m_pressedItem->type() == CGroupManagerItem::Group
				&& m_singleClick && (m_pressedItem->isOpen() == open) ) {
			//open group with first click if it's a global setting of KDE
			m_pressedItem->setOpen( !m_pressedItem->isOpen() );
		}
	}
	else if (e->button() == RightButton) {
			m_menu = true;
			popupMenu->exec( viewport()->mapToGlobal( contentsToViewport(m_pressedPos) ));
	}
	m_menu = false;	
}

/** Reimplementation. */
void CGroupManager::contentsMouseDoubleClickEvent ( QMouseEvent * e){
  bool open = false;	
  if (m_pressedItem)
  	open = m_pressedItem->isOpen();
		
	KListView::contentsMouseDoubleClickEvent(e);

	if ((e->state() & ControlButton) || (e->state() & ShiftButton))
			return;
	if (e->button() == LeftButton) {
		if (m_pressedItem && m_pressedItem->type() == CGroupManagerItem::Group
				&& !m_singleClick && (m_pressedItem->isOpen() == open) ) {
			//open group with first click if it's a global setting of KDE
			m_pressedItem->setOpen( !m_pressedItem->isOpen() );
		}
	}	
}

void CGroupManager::contentsMouseReleaseEvent ( QMouseEvent* e ) {
	qDebug("CGroupManager::contentsMouseReleaseEvent");
	KListView::contentsMouseReleaseEvent(e);		
 	
	if ((e->state() & ControlButton) || (e->state() & ShiftButton))
		return;

  if ( !(m_pressedItem=(CGroupManagerItem*)itemAt(contentsToViewport(e->pos()))) )
    return;

	if (m_pressedItem && (e->button() == LeftButton)) {
		if (m_pressedItem->type() == CGroupManagerItem::Module && m_pressedItem->moduleInfo()) {
	  	//check if module is encrypted and show dialog if it wasn't opened before	  	
	  	if (m_pressedItem->moduleInfo()->isEncrypted()) {
  			KConfigGroupSaver groupSaver(config, "Groupmanager");
	  		if (!config->readBoolEntry(QString("shown %1 encrypted").arg(m_pressedItem->moduleInfo()->module()->Name()), false))
	  			HTML_DIALOG(HELPDIALOG_MODULE_LOCKED);
	  		config->writeEntry(QString("shown %1 encrypted").arg(m_pressedItem->moduleInfo()->module()->Name()), true);
	  	}
			emit createSwordPresenter( m_pressedItem->moduleInfo(), QString::null );
		}
		else if  (m_pressedItem->type() == CGroupManagerItem::Bookmark) {
			ASSERT(m_pressedItem->getBookmarkKey());
			if (m_pressedItem->moduleInfo() && m_pressedItem->getBookmarkKey() ) {
				emit createSwordPresenter( m_pressedItem->moduleInfo(), m_pressedItem->getKeyText() );
			}			
		}
	}	
 	m_menu = false;	
}

/** Reimplementation */
void CGroupManager::contentsMouseMoveEvent ( QMouseEvent * e) {
	KListView::contentsMouseMoveEvent( e );	
	CGroupManagerItem* dragItem=(CGroupManagerItem *)itemAt( contentsToViewport(e->pos()) );

	//mouse is pressed, an item is selected and the popup menu isn't opened
	if ( (e->state()&LeftButton) && (e->stateAfter()&LeftButton) && dragItem && !m_menu)
   	{
		//Is it time to start a drag?
      	if (abs(e->pos().x() - m_pressedPos.x()) > KGlobalSettings::dndEventDelay() ||
          abs(e->pos().y() - m_pressedPos.y()) > KGlobalSettings::dndEventDelay() )
      	{
			// Collect all selected items
        	ASSERT(m_itemList);
        	m_itemList = new QList<CGroupManagerItem>;
        	QListViewItemIterator it( this );
        	for( ; it.current(); it++ )
           	if ( it.current()->isSelected() )
					m_itemList->append( (CGroupManagerItem*)it.current() );
  				  			
        	QTextDrag *d = 0;
        	if (!dragItem)
        		return;
			switch (dragItem->type()){
				case (CGroupManagerItem::Bookmark):
	         if (dragItem->moduleInfo()) {
	           	QString ref = dragItem->getKeyText();
	           	QString mod = dragItem->moduleInfo()->module()->Name();
	            	
	          	d = new QTextDrag(CToolClass::encodeReference(mod,ref), viewport());
	          	d->setSubtype(BOOKMARK);
	        		m_dragType = BOOKMARK;
					}
					break;					
				case (CGroupManagerItem::Module):
					d = new QTextDrag( "" , viewport());
					d->setSubtype(MODULE);
					m_dragType = MODULE;
					break;					
				case (CGroupManagerItem::Group):
					d = new QTextDrag( "" , viewport());
					d->setSubtype(GROUP);
					m_dragType = GROUP;
					break;
			}
			
			if (d) {
				d->setPixmap( *(dragItem->pixmap(0)) );
				m_menu = false;
				d->drag();
			}			
		}
	}
}

/** Creates a new group */
void CGroupManager::slotCreateNewGroup(){
	bool isOk;
	QString groupname = QInputDialog::getText(i18n("BibleTime - Enter name of folder"),i18n("Please enter the name of the folder!"),"", &isOk, 0);
	
	if (isOk) {
		if (m_pressedItem && m_pressedItem->type() == CGroupManagerItem::Group) {
			(void)new CGroupManagerItem(m_pressedItem, groupname, QString::null, 0,0, CGroupManagerItem::Group);
			m_pressedItem->setOpen(true);
		}
		else if (m_pressedItem && m_pressedItem->parent() && m_pressedItem->parent()->type() == CGroupManagerItem::Group){
			(void)new CGroupManagerItem(m_pressedItem->parent(), groupname, QString::null, 0,0, CGroupManagerItem::Group);
			m_pressedItem->parent()->setOpen(true);
		}
		else {
			(void)new CGroupManagerItem(this, groupname, QString::null, 0, 0,CGroupManagerItem::Group);
		}
	}
}

int CGroupManager::parentId(CGroupManagerItem *item, CGroupManagerItem* parentItem)
{
	// search parent of item and return the id of the parent
	int ret = -1; 	// the view and the parent item have id -1
	int index = 0;
	CGroupManagerItem* myItem = 0;
	
	if (item) {
		if (item->parent() && item != parentItem) {
		  QListViewItemIterator it;
		  if (parentItem) {
		  	it = QListViewItemIterator( parentItem );		
		  }
		  else
		  	it = QListViewItemIterator( this );
		  	
			if (item->parent() == parentItem)
				return -1;
			while (it.current() && (it.current() != item->parent()) ) {
				myItem = (CGroupManagerItem*)it.current();
				if (parentItem && !isChild(parentItem, myItem) ) {
					it++;
					continue;
				}
				if (myItem && myItem->type() == CGroupManagerItem::Group) {
					index++;
				}
				it++;
			}
			if (it.current())
				ret = index;
		}
	}
	return ret;
}

/** returns the parent of the item with the id ID */
CGroupManagerItem* CGroupManager::findParent( int ID, CGroupManagerItem* parentItem){
	CGroupManagerItem* myItem = 0;
	int index = 0;
	
	QListViewItemIterator it;
  if (parentItem)
  	it = QListViewItemIterator(parentItem);
  else
	  it = QListViewItemIterator( this );

  //traverse the tree and try to find the group with the id ID index using comparision
  for( ; it.current(); it++ ) {
 		myItem = (CGroupManagerItem*)it.current();
 		if (parentItem && !isChild(parentItem, (CGroupManagerItem*)it.current())) {
 			continue;
 		}		
		if (myItem && myItem->type() == CGroupManagerItem::Group) {
			if (index == ID) {
				return myItem;
			}
			else
				index++;
		}
	}		
	return 0;	// return 0 if parent wasn't found
}

/** Opens a dialog to enter the key to unlock an encrypted module. */
void CGroupManager::slotUnlockModule(){	
	bool ok;
	QString unlockKey = QInputDialog::getText(i18n("BibleTime - Unlock module"),i18n("Enter the key to unlock the module!"), m_pressedItem->moduleInfo()->getCipherKey(), &ok, 0 );
	if (ok) {
		CSwordModuleInfo::unlockErrorCode ret = m_pressedItem->moduleInfo()->unlock( unlockKey );
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

/** Reads in bookmarks from config and creates them as subitems of group. If group is 0 we create them a toplevel items. */
bool CGroupManager::readSwordBookmarks(KConfig* configFile, CGroupManagerItem* group){
	qDebug("CGroupManager::readSwordBookmarks(KConfig* configFile, CGroupManagerItem* group)");
	//read and create group entries
	CGroupManagerItem* 	parentItem = 0;	
	QStringList				groupList 	= configFile->readListEntry("Groups");
	QValueList<int>		parentList 	= configFile->readIntListEntry("Group parents");
	
	QValueList<int>::Iterator it_parents = parentList.begin();	

	//read in all bookmarks
	QStringList	bookmarkList 							= configFile->readListEntry("Bookmarks");
	QStringList	bookmarkModulesList 			= configFile->readListEntry("Bookmark modules");
	QStringList	bookmarkDescriptionsList 	= configFile->readListEntry("Bookmark descriptions");
	parentList 														= configFile->readIntListEntry("Bookmark parents");
	
	QStringList::Iterator it_bookmarks 	= bookmarkList.begin();
	QStringList::Iterator it_modules 		= bookmarkModulesList.begin();
	QStringList::Iterator it_descriptions	= bookmarkDescriptionsList.begin();
	it_parents 													= parentList.begin();
	
	CSwordModuleInfo* moduleInfo 	= 0;
	CSwordModuleInfo* myModuleInfo = 0;
	CGroupManagerItem *myItem = 0;	
	CGroupManagerItem *oldItem = 0;

	while ( it_bookmarks != bookmarkList.end() && it_parents != parentList.end()
			 		&& it_modules != bookmarkModulesList.end() ) {		
		//search for module
		for(moduleInfo = m_swordList->first(); moduleInfo !=0; moduleInfo = m_swordList->next()) {
			if ( moduleInfo->module()->Name() == (*it_modules) ) {
				myModuleInfo = moduleInfo;
				break;
			}
		}
		if ( (*it_parents) == -1) {
			if (group)
				myItem = new CGroupManagerItem(group, (*it_bookmarks), (*it_modules), myModuleInfo, 0, CGroupManagerItem::Bookmark);
			else
				myItem = new CGroupManagerItem(this, (*it_bookmarks), (*it_modules), myModuleInfo, 0, CGroupManagerItem::Bookmark);
		}
		else {
			parentItem = findParent( (*it_parents), group ? group : 0 );
			if (parentItem)
				myItem = new CGroupManagerItem(parentItem, (*it_bookmarks), (*it_modules), myModuleInfo,0, CGroupManagerItem::Bookmark);
			else if (group)
				myItem = new CGroupManagerItem(group, (*it_bookmarks), (*it_modules), myModuleInfo,0, CGroupManagerItem::Bookmark);
			else
				myItem = new CGroupManagerItem(this, (*it_bookmarks), (*it_modules), myModuleInfo,0, CGroupManagerItem::Bookmark);
		}
		if (myItem && it_descriptions != bookmarkDescriptionsList.end())
			myItem->setDescription( *it_descriptions );
		if (myItem && oldItem) {	//if both items exist we can move myItem behind oldItem
			myItem->moveToJustAfter(oldItem);
		}
		oldItem = myItem;
		
		++it_parents;
		++it_modules;
		++it_descriptions;
		++it_bookmarks;
	}	
	return true;
}

/** Save items of group to config. If grou is 0 we save all items. The  path to the group-item itself is saved, too. */
bool CGroupManager::saveSwordBookmarks(KConfig* configFile, CGroupManagerItem* group){
	qDebug("CGroupManager::saveBookmarks");
		
	int parentID = 0;
	CGroupManagerItem* myItem = 0;
	QStringList groupList;
	QValueList<int> parentList;
	QStringList bookmarkList;	
	QStringList bookmarkModuleList;	
	QStringList bookmarkDescriptionList;		
	QListViewItemIterator it;
	
	//save bookmarks
	parentList.clear();
	bookmarkList.clear();
	bookmarkModuleList.clear();
	bookmarkDescriptionList.clear();

	if (group)
		it = QListViewItemIterator ( group );
	else if (group && group->childCount() == 0) {
		return false;
	}
	else
 		it = QListViewItemIterator (this);

  for( ; it.current(); it++ ) {
		myItem = (CGroupManagerItem*)it.current();
  	if (group)
  		if (!isChild(m_pressedItem, myItem)) {
				continue;					
			}
		if (myItem && myItem->type() == CGroupManagerItem::Bookmark)  {
			if (group) {
				parentID = parentId( (CGroupManagerItem*)it.current(),m_pressedItem );	
			}
			else {
				parentID = parentId( (CGroupManagerItem*)it.current() );	
			}
			parentList.append( parentID );
						
			if (myItem->getBookmarkKey())	{ //has a
				if (dynamic_cast<SWKey*>(myItem->getBookmarkKey())) {
					SWKey* key = dynamic_cast<SWKey*>(myItem->getBookmarkKey());
					bookmarkList.append( QString::fromLocal8Bit((const char*)*key) );
				}				
			}
			else
				bookmarkList.append( "" );	//invalid key
				
			if (myItem && !myItem->description().isEmpty())	//save description
				bookmarkDescriptionList.append( myItem->description() );
			else
				bookmarkDescriptionList.append( QString::null );
				
			if (myItem->moduleInfo()) {	//save the module
				bookmarkModuleList.append( myItem->moduleInfo()->module()->Name() );
			}
			else {
				bookmarkModuleList.append( myItem->m_moduleName );
			}
		}
	}	
	
	configFile->writeEntry("Bookmarks", bookmarkList);
	configFile->writeEntry("Bookmark descriptions", bookmarkDescriptionList);
	configFile->writeEntry("Bookmark modules", bookmarkModuleList);
	configFile->writeEntry("Bookmark parents", parentList);
	
	return true;
}

/** Impoorts bookmarks */
void CGroupManager::slotImportBookmarks(){
	qDebug("CGroupManager::slotImportBookmarks");	
	if (!m_pressedItem || ( m_pressedItem && m_pressedItem->type() == CGroupManagerItem::Group) ) {
		QString file = KFileDialog::getOpenFileName(QString::null, "*.btb | *.btb", 0, i18n("Import bookmarks ..."));	
		if (!file.isNull()) {
			KSimpleConfig simpleConfig(file, false);
			simpleConfig.setGroup("Bookmarks");
			short int formatVersion = simpleConfig.readNumEntry("Bookmark format version", -1);
			
			//bookmark format of imported file is newer than our version			
			if (formatVersion > BOOKMARK_FORMAT_VERSION) {
				int retValue = KMessageBox::warningContinueCancel(this, i18n("<qt>A problem occurred while importing bookmarks!<BR>\
The bookmarks format of the imprted file is newer<BR>\
than the bookmarks format version of this version of BibleTime!<BR>\
<B>Importing the bookmarks may not work correctly!</B><BR>Do you want to continue?</qt>"), i18n("Import of bookmarks"), i18n("Continue") );
				if ( retValue != KMessageBox::Continue )
					return;
			}
			
			readGroups(&simpleConfig, m_pressedItem);			
			readSwordBookmarks(&simpleConfig, m_pressedItem);
		}
	}
}

/** Exports bookmarks of selected group */
void CGroupManager::slotExportBookmarks(){
	qDebug("CGroupManager::slotExportBookmarks");
	if (!m_pressedItem || ( m_pressedItem && m_pressedItem->type() == CGroupManagerItem::Group) ) {
		QString file = KFileDialog::getSaveFileName (QString::null, "*.btb | *.btb", 0, i18n("Export bookmarks ..."));	
		if (!file.isNull()) {
			KSimpleConfig simpleConfig(file, false);
			simpleConfig.setGroup("Bookmarks");			
			simpleConfig.writeEntry("Bookmark format version", BOOKMARK_FORMAT_VERSION);
			saveGroups(&simpleConfig, m_pressedItem);			
			saveSwordBookmarks(&simpleConfig, m_pressedItem);
			simpleConfig.sync();
		}
	}
}


bool CGroupManager::readSwordModules(KConfig* configFile, CGroupManagerItem* group) {
	qDebug("CGroupManager::readSwordModules(KConfig* configFile, CGroupManagerItem* group)");

	ASSERT(m_swordList);
	if (!m_swordList) {
		qWarning("no sword modules, return false.");
		return false;
	}
	ASSERT(configFile);
	ASSERT(group);
	
	//read and create group entries
	CGroupManagerItem* 	parentItem = 0;	
	
	QStringList				groupList 	= configFile->readListEntry("Groups");
	QValueList<int>		parentList 	= configFile->readIntListEntry("Group parents");
	QValueList<int>::Iterator it_parents = parentList.begin();
	
	//read in all bookmarks
	QStringList	moduleList 	= configFile->readListEntry("Modules");
	parentList 							= configFile->readIntListEntry("Module parents");
	
	QStringList::Iterator it_modules 		= moduleList.begin();
	it_parents 													= parentList.begin();
	
	CSwordModuleInfo* myModuleInfo = 0;
	CGroupManagerItem *myItem = 0;
	CGroupManagerItem *oldItem = 0;
	
	while ( it_modules != moduleList.end() && it_parents != parentList.end() ) {		
		myModuleInfo = 0;
		
		if ((*it_modules).isEmpty()) {
			++it_parents;
			++it_modules;
			continue;
		}
		
		for(m_swordList->first(); m_swordList->current(); m_swordList->next()) {
			if ( m_swordList->current()->module()->Name() == (*it_modules) ) {
				myModuleInfo = m_swordList->current();
				break;
			}
		}
		if (!myModuleInfo) {	//if the module was removed do not show it
			++it_parents;
			++it_modules;			
			continue;
		}
			
		if ( (*it_parents) == -1) {
		 	if (group)
				myItem = new CGroupManagerItem(group, QString::null, QString::null, myModuleInfo,0, CGroupManagerItem::Module);
			else
				myItem = new CGroupManagerItem(this, QString::null, QString::null, myModuleInfo,0, CGroupManagerItem::Module);
		}
		else {
			parentItem = findParent( (*it_parents), group ? group : 0  );
			if (parentItem)
				myItem = new CGroupManagerItem(parentItem, QString::null, QString::null, myModuleInfo, 0,CGroupManagerItem::Module);	
			else if (group)
				myItem = new CGroupManagerItem(group, QString::null, QString::null, myModuleInfo, 0, CGroupManagerItem::Module);
			else				
				myItem = new CGroupManagerItem(this, QString::null, QString::null, myModuleInfo, 0, CGroupManagerItem::Module);
		}
		if (myItem && oldItem)
			myItem->moveToJustAfter(oldItem);
		oldItem = myItem;
			
		++it_parents;
		++it_modules;
	}	
	return true;
};

bool CGroupManager::saveSwordModules(KConfig* configFile, CGroupManagerItem* group) {
	int parentID = 0;
	CGroupManagerItem* myItem = 0;
	QValueList<int> parentList;
	QStringList moduleList;	
	
	QListViewItemIterator it;
	
	parentList.clear();
	moduleList.clear();

	if (group)
		it = QListViewItemIterator ( group );
	else
 		it = QListViewItemIterator (this);

  for( ; it.current(); it++ ) {
			myItem = (CGroupManagerItem*) it.current();
	  	if (group)
	  		if (!isChild(m_pressedItem, myItem))
					continue;					
			
			if (myItem && myItem->type() == CGroupManagerItem::Module)  {
				if (group)
					parentID = parentId( (CGroupManagerItem*)it.current(),m_pressedItem );	
				else
					parentID = parentId( (CGroupManagerItem*)it.current() );	
				parentList.append( parentID );
				
				if (myItem->moduleInfo())
					moduleList.append( myItem->moduleInfo()->module()->Name() );				
				else {
					moduleList.append( myItem->text(0) );	//first column is the modulename
//					qDebug("CGroupManager::SaveSettings: myItem has no moduleInfo");
				}
			}
	}	
	configFile->writeEntry("Modules", moduleList);
	configFile->writeEntry("Module parents", parentList);

	return true;
}

bool CGroupManager::readGroups(KConfig* configFile, CGroupManagerItem* group) {
	//read and create group entries
	CGroupManagerItem* 	parentItem = 0;
	CGroupManagerItem*	oldItem = 0;
	CGroupManagerItem* newItem = 0;
	
	QStringList				groupList 	= configFile->readListEntry("Groups");
	QValueList<int>		parentList 	= configFile->readIntListEntry("Group parents");
	
	QStringList::Iterator it_groups = groupList.begin();
	QValueList<int>::Iterator it_parents = parentList.begin();
	
	while ( (it_groups != groupList.end()) && (it_parents != parentList.end()) ) {
		if ( (*it_parents) == -1) {
			if (group)
				newItem = new CGroupManagerItem(group, (*it_groups), QString::null, 0,0, CGroupManagerItem::Group);
			else
				newItem = new CGroupManagerItem(this, (*it_groups), QString::null, 0,0, CGroupManagerItem::Group);
		}
		else {
			parentItem = findParent( (*it_parents),group ? group : 0  );			
			if (parentItem)
				newItem = new CGroupManagerItem(parentItem, (*it_groups),QString::null,0,0, CGroupManagerItem::Group);
			else if (group)
				newItem = new CGroupManagerItem(group, (*it_groups),QString::null,0, 0,CGroupManagerItem::Group);
			else
				newItem = new CGroupManagerItem(this, (*it_groups),QString::null,0, 0,CGroupManagerItem::Group);
		}
		ASSERT(newItem);
		ASSERT(oldItem);
		if ( newItem && oldItem ) {
			if ( isChild(oldItem, newItem ) || (!newItem->parent() && !oldItem->parent()) || (newItem->parent() == oldItem->parent() )) {
				newItem->moveToJustAfter( oldItem );
			}
		}
		++it_parents;
		++it_groups;
		
		if (newItem) {
			while (parentId(newItem) > *it_parents) {
				if ( (group && isChild(group, newItem)) || (newItem && newItem->parent()) )
					newItem = newItem->parent();
				else
					break;
			}
			oldItem = newItem;
		}		
	}	
	return true;	
};

bool CGroupManager::saveGroups(KConfig* configFile, CGroupManagerItem* group) {
	int parentID = 0;
	CGroupManagerItem* myItem = 0;
	QStringList groupList;
	QValueList<int> parentList;
	
	QListViewItemIterator it;
	
	//save groups	
	if (group && group->childCount() > 0)
	 	it = QListViewItemIterator ( group->firstChild() );
	else if (group && group->childCount() == 0) {
		return false;
	}
	else
	  it = QListViewItemIterator ( this );

  for( ; it.current(); it++ ) {
		myItem = (CGroupManagerItem*) it.current();
  	if (group && !isChild(m_pressedItem, myItem))
			continue;		
		if (myItem && myItem->type() == CGroupManagerItem::Group)  {
		 	if (group)
				parentID = parentId( (CGroupManagerItem*)it.current(), m_pressedItem );
			else
				parentID = parentId( (CGroupManagerItem*)it.current() );
			groupList.append( myItem->text(0) );
			parentList.append( parentID );
		}
	}
	configFile->writeEntry("Groups", groupList);
	configFile->writeEntry("Group parents", parentList);
	return true;
}

/** Returns true if the item "item" is a child of item "parent". */
bool CGroupManager::isChild(CGroupManagerItem* parent, CGroupManagerItem* child){
	CGroupManagerItem *myParent = child;
	while (myParent && myParent != parent ) {
		myParent = myParent->parent();	
	}
	
	if (myParent == parent && parent!=child)
		return true;
	else
		return false;
}

/** Reimplementatuiion. */
void CGroupManager::viewportPaintEvent (QPaintEvent* e) {
  KListView::viewportPaintEvent(e);

  if (oldDragRect.isValid())
  {
    static bool invalidated=false;
    if (!invalidated)
    {
      invalidated=true;
      viewport()->repaint(oldDragRect);
    }
    QPainter painter(viewport());
    drawDropVisualizer(&painter, m_parentItemDrop, m_afterItemDrop, "");
    invalidated=false;
  }
}

/** Reimplementation with different parameters */
QRect CGroupManager::drawDropVisualizer (QPainter *p, CGroupManagerItem *parent, CGroupManagerItem *after, const QString type){
  QRect insertmarker;
  bool useParent = false;
		
	if ( m_dragType == BOOKMARK ) { 	//we are moving a module
		if ( after && after->type() == CGroupManagerItem::Group)
			insertmarker = itemRect(after);
		else if ( after && after && after->type() == CGroupManagerItem::Module )			
			insertmarker = QRect(itemRect(after));
		else if ( after && after && after->type() == CGroupManagerItem::Bookmark ) {
			if (after->parent()) {
				useParent = true;
				insertmarker = itemRect(after->parent());	//paint nothing
			}
			else
				insertmarker = QRect();
		}
		else if ( after && after->parent() && after->parent()->type() == CGroupManagerItem::Group ) {
			useParent = true;
			insertmarker = itemRect(after->parent());				
		}
		else if (!after)
			insertmarker = QRect(); //this->visibleRect();		
		else
			insertmarker = QRect();
	}
	else if ( m_dragType == REFERENCE ) { 	//we are moving a reference
		if ( after && after->type() == CGroupManagerItem::Group)
			insertmarker = itemRect(after);
		else if ( after && after->type() == CGroupManagerItem::Module )			
			insertmarker = QRect(itemRect(after));
		else if ( after && after->type() == CGroupManagerItem::Bookmark ) {
			if (after->parent()) {
				useParent = true;
				insertmarker = itemRect(after->parent());
			}
			else
				insertmarker = QRect(); 	//paint nothing			
		}
		else if ( after && after->parent() && after->parent()->type() == CGroupManagerItem::Group ) {
			useParent = true;
			insertmarker = itemRect(after->parent());				
		}
		else if (!after)
			insertmarker = QRect(); //this->visibleRect();
		else
			insertmarker = QRect();
	}
	else if ( m_dragType == MODULE ) { 	//we are moving a reference
		if ( after && after->type() == CGroupManagerItem::Group)
			insertmarker = itemRect(after);
		else if ( after && (after->type() == CGroupManagerItem::Module || after->type() == CGroupManagerItem::Bookmark) ) {
			if (after->parent()) {
				useParent = true;
				insertmarker = itemRect(after->parent());
			}
			else
				insertmarker = QRect(); 	//paint nothing			
		}		
		else if ( after && after->parent() && after->parent()->type() == CGroupManagerItem::Group ) {
			useParent = true;
			insertmarker = itemRect(after->parent());
		}
		else if (!after)
			insertmarker = QRect();//this->visibleRect();
		else
			insertmarker = QRect();
	}	
	else if (m_dragType == TEXT) {
		if ( after && after && after->type() == CGroupManagerItem::Module )			
			insertmarker = QRect(itemRect(after));
		else
			insertmarker = QRect();
	}
	else if (m_pressedItem) {
  	if ( m_pressedItem->type() == CGroupManagerItem::Group  ) {	//we are moving a group
			if (after && after->type() == CGroupManagerItem::Group)
				insertmarker = itemRect(after);
			else if (after && after->parent() && after->parent()->type() == CGroupManagerItem::Group) {
				useParent = true;
				insertmarker = itemRect(after->parent());
			}
			else if (!after)
				insertmarker = QRect();//this->visibleRect();
		else
			insertmarker = QRect();
  	}
  }

  if ( p && insertmarker.isValid() )  {
  	if (useParent)
  		insertmarker.setLeft( treeStepSize()*(after->parent()->depth()+(rootIsDecorated() ? 1 :0 ))+itemMargin()- contentsX());
  	else
  		insertmarker.setLeft( treeStepSize()*(after->depth()+(rootIsDecorated() ? 1 :0 ))+itemMargin() - contentsX());
  	style().drawFocusRect( p, insertmarker, colorGroup(), after->isSelected() ? &colorGroup().highlight() : &colorGroup().base(), after->isSelected() && !useParent );
  }
  else if (!insertmarker.isValid()) {
  	cleanDropVisualizer();
  }

  return insertmarker;
}

/** Reimplementation. */
void CGroupManager::cleanDropVisualizer(){
  if ( oldDragRect.isValid() ) {
    QRect rect = oldDragRect;
    oldDragRect = QRect();
    viewport()->repaint(rect, true);
  }
}

/** Resets the groupmanager. Recreates the original Bibles, Lexicons and Commentary groups. */
void CGroupManager::slotReset(){
	int result = KMessageBox:: warningContinueCancel( this, i18n("<qt>This function will reset the groupmanager!<BR>This will recreate the original groups and delete all other items!<BR>Be sure not to delete important bookmarks etc!<BR>Do you want to continue?</qt>"), i18n("BibleTime - Reset groupmanager"), i18n("Continue"));
	
	if (result == KMessageBox::Continue ) {
		this->clear();
		this->setupStandardSwordTree();
	}
}

/** Prints the selected bookmark. */
void CGroupManager::slotPrintBookmark(){
	qDebug("CGroupManager::slotPrintBookmark()");
	if (!m_pressedItem || ( m_pressedItem && m_pressedItem->type() != CGroupManagerItem::Bookmark) )
		return;
	CPrinter*	printer = m_important->printer;	
	ASSERT(printer);
	
	CPrintItem*	printItem = new CPrintItem();
	printItem->setDescription( m_pressedItem->description() );
	printItem->setStartKey( m_pressedItem->getBookmarkKey());
	printItem->setStopKey( m_pressedItem->getBookmarkKey());
	printItem->setModule(m_pressedItem->moduleInfo());
	
	printer->addItemToQueue( printItem );
}

/** Deletes the searchdialog. */
void CGroupManager::slotDeleteSearchdialog(){
	if (searchDialog)
		searchDialog->delayedDestruct();	//delete the search dialog
}

/** Reimplementation. */
void CGroupManager::resizeEvent ( QResizeEvent* e )  {
	KListView::resizeEvent(e);
		
	setColumnWidth(0, visibleWidth() );
}
