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

#include "../../backend/cswordbiblemoduleinfo.h"
#include "../../backend/cswordcommentarymoduleinfo.h"
#include "../../backend/cswordlexiconmoduleinfo.h"
#include "../../backend/cswordversekey.h"
#include "../../backend/cswordldkey.h"
#include "../../backend/creferencemanager.h"

#include "../../printing/cprintitem.h"
#include "../../printing/cprinter.h"

#include "../chtmldialog.h"
#include "../cinputdialog.h"
//#include "../ctoolclass.h"
#include "../searchdialog/csearchdialog.h"
#include "../cbtconfig.h"

#include "../../structdef.h"
#include "../../whatsthisdef.h"
#include "../../ressource.h"

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
#include <kfiledialog.h>
#include <kmessagebox.h>


CGroupManager::ToolTip::ToolTip(QWidget* parent) : QToolTip(parent) {
}

void CGroupManager::ToolTip::maybeTip(const QPoint& p) {
	if (!parentWidget()->inherits("CGroupManager"))
		return;
	
	CGroupManager* m = 0;
	if ( !(m = dynamic_cast<CGroupManager*>(parentWidget())) )
		return;	
	CGroupManagerItem* i = 0;
	if ( !( i = dynamic_cast<CGroupManagerItem*>(m->itemAt(p))) )
		return;
	
	QRect r = m->itemRect(i);
	if (!r.isValid())
		return;
	
	//get type of item and display correct text
	const QString text = i->getToolTip();
	if (!text.isEmpty()) {
		const QFont oldFont = font();				
		CSwordModuleInfo* m = dynamic_cast<CSwordModuleInfo*>(i->moduleInfo());

		//Module is Unicode-based. Only set font if a bookmark should be displayed
		if (i->getBookmarkKey() && m && m->isUnicode() )
			setFont(CBTConfig::get( CBTConfig::unicode ));

		tip(r, text);
		setFont(oldFont);
	}
}


CGroupManager::CGroupManager(CImportantClasses* importantClasses, QWidget *parent, const char *name, ListCSwordModuleInfo *swordList, const bool useBookmarks, const bool saveSettings, const bool useDnD, const bool useExtendedMode, const bool useRMBMenu, const bool showHelpDialogs)
	: KListView(parent, name),
		m_useBookmarks(useBookmarks), m_saveSettings(saveSettings),
		m_useDragDrop(useDnD),
		m_useExtendedMode(useExtendedMode), m_useRMBMenu(useRMBMenu),
		m_showHelpDialogs(showHelpDialogs),
		m_config( new KConfig("bt-groupmanager", false, false ) ),
		m_important(importantClasses),
	  m_menu(false),
		m_searchDialog(0),
		m_pressedItem(0),
		m_swordList(swordList)
{
	initView();
	initConnections();
	readSettings();
}

CGroupManager::~CGroupManager(){	
	saveSettings();
	m_config->sync();
	delete m_config;
}

/** Initializes the tree of this CGroupmanager */
void CGroupManager::setupSwordTree() {
	readGroups(m_config, 0);
	readSwordModules(m_config, 0);	
	if (m_useBookmarks)
		readSwordBookmarks(m_config, 0);	

	setupStandardSwordTree();
}

/** Initializes the tree of this CGroupmanager */
void CGroupManager::setupStandardSwordTree() {
	if (!m_swordList || (m_swordList && !m_swordList->count())) {
		qWarning("CGroupManager::setupStandardSwordTree: m_swordList is empty or qual to NULL. return now.");
		return;
	}
	
	const bool initialized = m_config->readBoolEntry("initialized", false);
	CSwordModuleInfo* moduleInfo = 0;
	CGroupManagerItem* item = 0;
	QListViewItemIterator it( this );
	
	#define CGROUPMANAGER_GROUP(name, groupItem) \
	{ \
		it = QListViewItemIterator(this); \
		item = 0; \
		for ( ; it.current(); ++it ) { \
			if ( it.current() ) { \
				item = dynamic_cast<CGroupManagerItem*>(it.current()); \
				if (item && (item->text(0) == name) ) { \
					groupItem = item; \
					break; \
				} \
				else \
					groupItem = 0; \
			} \
		}	\
		if (!groupItem) \
			groupItem = new CGroupManagerItem(this, name, QString::null, 0, 0, CGroupManagerItem::Group, m_important); \
	}
	CGroupManagerItem* bibleGroup = 0;
	CGROUPMANAGER_GROUP(i18n("Bibles"), bibleGroup);
		
	CGroupManagerItem* commentaryGroup = 0;
	CGROUPMANAGER_GROUP(i18n("Commentaries"), commentaryGroup);
		
	CGroupManagerItem* lexiconGroup = 0;
	CGROUPMANAGER_GROUP(i18n("Lexicons"), lexiconGroup);

	CGroupManagerItem* bookGroup = 0;
	CGROUPMANAGER_GROUP(i18n("Books"), bookGroup);
			
	#undef CGROUPMANAGER_GROUP
	
	for(moduleInfo = m_swordList->first(); moduleInfo; moduleInfo = m_swordList->next()) {
		bool alreadyCreated = false;
  	QListViewItemIterator it( this );
		for ( ; it.current(); ++it ) {
			CGroupManagerItem* item = dynamic_cast<CGroupManagerItem*>(it.current());
			if (item && item->moduleInfo() == moduleInfo) { //already there
				alreadyCreated = true;
				break;
			}
		}
		if ( moduleInfo && !alreadyCreated) {
			CGroupManagerItem* itemParent = 0;
			switch (moduleInfo->getType()) {
				case CSwordModuleInfo::Bible:
					itemParent = bibleGroup;
					break;
				case CSwordModuleInfo::Commentary:
					itemParent = commentaryGroup;
					break;
				case CSwordModuleInfo::Lexicon:
					itemParent = lexiconGroup;
					break;
				case CSwordModuleInfo::GenericBook:
					itemParent = bookGroup;
					break;					
				default:
					break;
			}
			if (itemParent)
				(void)new CGroupManagerItem(itemParent, "",QString::null, moduleInfo,0, CGroupManagerItem::Module, m_important);
		}
	}

	// Now delete the groupes which have no child items
	if (!bibleGroup->childCount()) {		
		delete bibleGroup;
		bibleGroup = 0;
	}
	if (!lexiconGroup->childCount()) {
		delete lexiconGroup;
		lexiconGroup = 0;
	}
	if (!commentaryGroup->childCount()) {
		delete commentaryGroup;
		commentaryGroup = 0;
	}
	if (!bookGroup->childCount()) {
		delete bookGroup;
		bookGroup = 0;
	}

	if (!initialized) {
		if (bibleGroup)
			bibleGroup->sortChildItems(0,true);
		if (lexiconGroup)
			lexiconGroup->sortChildItems(0,true);
		if (commentaryGroup)
			commentaryGroup->sortChildItems(0,true);	
		if (bookGroup)
			bookGroup->sortChildItems(0,true);				
	}
}

/** Initializes the connections of this class */
void CGroupManager::initConnections(){
	connect(this, SIGNAL(returnPressed(QListViewItem*)), SLOT(slotReturnPressed(QListViewItem*)));
	if (m_useRMBMenu)
		connect(m_popupMenu, SIGNAL(aboutToShow()),
			this, SLOT(slotPopupAboutToShow()));
}

/**  */
void CGroupManager::saveSettings(){	
	KConfigGroupSaver groupSaver(m_config, "Groupmanager");
	
	if (!m_saveSettings) {
		return;
	}
			
	//save column width	
	m_config->writeEntry("initialized", true);
	m_config->writeEntry("First column", columnWidth(0));
	
	//save the bookmarks and the groups
	
	saveGroups(m_config,0);
	saveSwordModules(m_config,0);
	saveSwordBookmarks(m_config, 0);

	m_config->sync();
}

/**  */
void CGroupManager::readSettings(){
	KConfigGroupSaver groupSaver(m_config, "Groupmanager");
	if (m_config->readBoolEntry("initialized"))
		setupSwordTree();
	else
		setupStandardSwordTree();
	setColumnWidth(0, m_config->readNumEntry("First column", visibleWidth()) );
}

/** Initializes this widget */
void CGroupManager::initView(){
 	m_toolTip = new ToolTip(this);
 	
 	QWhatsThis::add(this, WT_GM_WIDGET );
 	m_singleClick = KGlobalSettings::singleClick();
 	 		
	setBackgroundMode(PaletteBase);
	setSorting(-1);
	 	
 	if (m_useDragDrop)
	 	viewport()->setAcceptDrops(true);
	if (m_useExtendedMode)
		setSelectionModeExt(KListView::Extended);
 	setRootIsDecorated(false);
 	setAllColumnsShowFocus(true);
 			 	
 	addColumn(i18n("Caption"));
 	header()->hide();
	
	if (!m_useRMBMenu) {
		m_popupMenu = 0;
	}
	else {		
		m_popupMenu = new KPopupMenu(this);
		m_popupMenu->insertTitle(i18n("Main index"));
		m_popupMenu->insertItem(GROUP_NEW_ICON_SMALL, i18n("Create a new folder"),
			this, SLOT(slotCreateNewGroup()),0,ID_GM_GROUP_CREATE);
		m_popupMenu->setWhatsThis(ID_GM_GROUP_CREATE, WT_GM_NEW_GROUP);	
		m_popupMenu->insertItem(GROUP_CHANGE_ICON_SMALL, i18n("Change this folder"),
			this, SLOT(slotChangeGroup()),0,ID_GM_GROUP_CHANGE);
		m_popupMenu->setWhatsThis(ID_GM_GROUP_CHANGE, WT_GM_CHANGE_GROUP);		
		m_popupMenu->insertSeparator();	
		m_popupMenu->insertItem(BOOKMARK_CHANGE_ICON_SMALL,i18n("Change this bookmark"),
			this,SLOT(slotChangeBookmark()),0,ID_GM_BOOKMARK_CHANGE);
		m_popupMenu->setWhatsThis(ID_GM_BOOKMARK_CHANGE, WT_GM_CHANGE_BOOKMARK);
		m_popupMenu->insertItem(BOOKMARK_IMPORT_ICON_SMALL,i18n("Import bookmarks"),
			this,SLOT(slotImportBookmarks()),0,ID_GM_BOOKMARKS_IMPORT);	
		m_popupMenu->setWhatsThis(ID_GM_BOOKMARKS_IMPORT, WT_GM_IMPORT_BOOKMARKS);
		m_popupMenu->insertItem(BOOKMARK_EXPORT_ICON_SMALL,i18n("Export bookmarks"),
			this,SLOT(slotExportBookmarks()),0,ID_GM_BOOKMARKS_EXPORT);		
		m_popupMenu->setWhatsThis(ID_GM_BOOKMARKS_EXPORT, WT_GM_EXPORT_BOOKMARKS);	
		m_popupMenu->insertItem(BOOKMARK_PRINT_ICON_SMALL,i18n("Print bookmark"),
			this,SLOT(slotPrintBookmark()),0,ID_GM_BOOKMARK_PRINT);
		m_popupMenu->setWhatsThis(ID_GM_BOOKMARK_PRINT, WT_GM_PRINT_BOOKMARK);
		m_popupMenu->insertItem(ITEMS_DELETE_ICON_SMALL, i18n("Remove selected item(s)"),
			this, SLOT(slotDeleteSelectedItems()),0,ID_GM_ITEMS_DELETE);
		m_popupMenu->insertSeparator();
		m_popupMenu->insertItem(MODULE_SEARCH_ICON_SMALL,i18n("Search in selected module(s)"),
			this, SLOT(slotSearchSelectedModules()),0,ID_GM_MODULES_SEARCH);
		m_popupMenu->insertSeparator();
		m_popupMenu->insertItem(MODULE_UNLOCK_ICON_SMALL,i18n("Unlock this module"),
			this, SLOT(slotUnlockModule()),0,ID_GM_MODULE_UNLOCK);	
		m_popupMenu->setWhatsThis(ID_GM_MODULE_UNLOCK, WT_GM_UNLOCK_MODULE);	
		m_popupMenu->insertItem(MODULE_ABOUT_ICON_SMALL, i18n("About this module"),
			this, SLOT(slotShowAbout()),0,ID_GM_MODULE_ABOUT);
		m_popupMenu->setWhatsThis(ID_GM_MODULE_ABOUT, WT_GM_ABOUT_MODULE);
	}
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
	ListCSwordModuleInfo searchList;	
	CGroupManagerItem *item = 0;	
	QListViewItemIterator it( this );
	for (; it.current(); ++it ) {
		if ( it.current()->isSelected()) {
			item = dynamic_cast<CGroupManagerItem*>(it.current());
			if (item && item->type() == CGroupManagerItem::Module)
				searchList.append(item->moduleInfo());
		}
	}	
	saveSettings();
	m_config->sync();

	if (m_searchDialog) {
		delete m_searchDialog;
	}
	m_searchDialog = new CSearchDialog(m_important,&searchList,0,0);		
	connect(m_searchDialog, SIGNAL(finished()),
		this, SLOT(slotDeleteSearchdialog()));

	m_searchDialog->show();
	m_searchDialog->raise();
}	

void CGroupManager::searchBookmarkedModule(const QString& text, CGroupManagerItem* item) {	
  if (!item->moduleInfo())
  	return;
	ListCSwordModuleInfo searchList;
	searchList.append(item->moduleInfo());
	
	if (!m_searchDialog)
		m_searchDialog = new CSearchDialog(m_important, 0,0);
	m_searchDialog->setModuleList(&searchList);
  m_searchDialog->setSearchText(text);
	m_searchDialog->show();
}	

/**  */
void CGroupManager::createNewBookmark(CGroupManagerItem* parent, CSwordModuleInfo* module, const QString& ref){
	if (!module)
		return;
	
	CGroupManagerItem* myItem = 0;	
	bool ok = false;
	const QString description = CInputDialog::getText(i18n("Bookmark description - BibleTime"), i18n("Please enter here the description:"), QString::null, &ok);
	if (!ok)
		return;

	myItem = 0;
  if ( parent && (parent->type() == CGroupManagerItem::Group) ) {
   	myItem = new CGroupManagerItem(parent,QString::null,QString::null,module, 0, CGroupManagerItem::Bookmark, m_important);
   	parent->setOpen(true);
  }
	else
		myItem = new CGroupManagerItem(this,QString::null,QString::null,module, 0, CGroupManagerItem::Bookmark);
		
	CSwordModuleInfo* swordModule = dynamic_cast<CSwordModuleInfo*>(module);	
	ASSERT(swordModule);
	qDebug("set keys");
	if (myItem && swordModule) {	//it's a Sword module
		if (swordModule->getType() == CSwordModuleInfo::Bible || swordModule->getType() == CSwordModuleInfo::Commentary) {	//a bible or commentary
			CSwordVerseKey* key = new CSwordVerseKey(swordModule);
			key->key(ref);
			myItem->setBookmarkKey(key);	//the key is deleted by the groupmmanager item
		}
		else if (swordModule->getType() == CSwordModuleInfo::Lexicon) {	//a lexicon module
			CSwordLDKey* key = new CSwordLDKey(swordModule);
			key->key(ref);
			myItem->setBookmarkKey(key);	//the key is deleted by the groupmmanager item
		}			
	}		
  if (myItem)	//set the description		
		myItem->setDescription(description);
	setFocus();		
}


/** Changes the current item */
void CGroupManager::slotChangeBookmark(){
	if (!m_pressedItem)
		return;		
	bool isOk;
	QString description = CInputDialog::getText(i18n("Change bookmark description - BibleTime"),i18n("Please change the description of the item!"), m_pressedItem->description(), &isOk, 0);
	if (isOk)
		m_pressedItem->setDescription(description);
}

/** Changes a group. */
void CGroupManager::slotChangeGroup(){
	if (!m_pressedItem)
		return;		
	
	bool isOk;
	QString description = QInputDialog::getText(i18n("Change folder - BibleTime"),i18n("Please change the name of the group!"), m_pressedItem->text(0), &isOk, 0);	
	if (isOk)
		m_pressedItem->setText( 0, description );
}

/** Is called before the menu is shown */
void CGroupManager::slotPopupAboutToShow(){
	if (!m_useRMBMenu)
		return;
		
	m_menu = true;
	
	
	if (m_pressedItem) {
		if (m_pressedItem->type() == CGroupManagerItem::Module) {
			bool moduleIsEncrypted = m_pressedItem->moduleInfo()->isEncrypted();
			
			m_popupMenu->setItemEnabled(ID_GM_PRESENTER_CREATE, true);			
			
			m_popupMenu->setItemEnabled(ID_GM_GROUP_CREATE, false);
			m_popupMenu->setItemEnabled(ID_GM_GROUP_CHANGE, false);
			
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARK_CHANGE, false);
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARKS_IMPORT, false);
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARKS_EXPORT, false);			
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARK_PRINT, false);			

			m_popupMenu->setItemEnabled(ID_GM_ITEMS_DELETE, true);
						
			m_popupMenu->setItemEnabled(ID_GM_MODULES_SEARCH, true);	
			m_popupMenu->setItemEnabled(ID_GM_MODULE_UNLOCK, moduleIsEncrypted);			
			m_popupMenu->setItemEnabled(ID_GM_MODULE_ABOUT, true);
		}
		else if (m_pressedItem->type() == CGroupManagerItem::Bookmark) {
			m_popupMenu->setItemEnabled(ID_GM_PRESENTER_CREATE, true);
						
			m_popupMenu->setItemEnabled(ID_GM_GROUP_CREATE, false);			
			m_popupMenu->setItemEnabled(ID_GM_GROUP_CHANGE, false);
						
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARK_CHANGE, true);
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARKS_IMPORT, false);
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARKS_EXPORT, false);			
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARK_PRINT, true);
			
			m_popupMenu->setItemEnabled(ID_GM_ITEMS_DELETE, true);						
			
			m_popupMenu->setItemEnabled(ID_GM_MODULES_SEARCH, false);
			m_popupMenu->setItemEnabled(ID_GM_MODULE_UNLOCK, false);			
			m_popupMenu->setItemEnabled(ID_GM_MODULE_ABOUT, false);

		}
		else if (m_pressedItem->type() == CGroupManagerItem::Group) {
			m_popupMenu->setItemEnabled(ID_GM_PRESENTER_CREATE, false);
						
			m_popupMenu->setItemEnabled(ID_GM_GROUP_CREATE, true);
			m_popupMenu->setItemEnabled(ID_GM_GROUP_CHANGE, true);
												
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARK_CHANGE, false);							
			
			//enable importa and export only if a bookmark exists in this group
			bool foundBookmark = false;
	    QListViewItem * myChild = m_pressedItem->firstChild();
	    while( myChild && !foundBookmark) {
	    	CGroupManagerItem* i = dynamic_cast<CGroupManagerItem*>(myChild);
				if (i && i->type() == CGroupManagerItem::Bookmark)
					foundBookmark = true;
				myChild = myChild->nextSibling();
	    }
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARKS_IMPORT, true);
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARKS_EXPORT, foundBookmark);
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARK_PRINT, false);
				
			m_popupMenu->setItemEnabled(ID_GM_ITEMS_DELETE, true);

			m_popupMenu->setItemEnabled(ID_GM_MODULES_SEARCH, false);
			m_popupMenu->setItemEnabled(ID_GM_MODULE_UNLOCK, false);
			m_popupMenu->setItemEnabled(ID_GM_MODULE_ABOUT, false);
		}		
	}
	else { //top level
			m_popupMenu->setItemEnabled(ID_GM_PRESENTER_CREATE, false);
						
			m_popupMenu->setItemEnabled(ID_GM_GROUP_CREATE, true);
			m_popupMenu->setItemEnabled(ID_GM_GROUP_CHANGE, false);
			
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARK_CHANGE, false);				
			
			bool foundBookmark = false;	
	    QListViewItemIterator it( this );
	    for ( ; it.current() && !foundBookmark; ++it ) {
	    	CGroupManagerItem* i = dynamic_cast<CGroupManagerItem*>(it.current());
				if (i && i->type() == CGroupManagerItem::Bookmark)
					foundBookmark = true;
			}			
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARKS_IMPORT, true);
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARKS_EXPORT, foundBookmark);
			m_popupMenu->setItemEnabled(ID_GM_BOOKMARK_PRINT, false);
						
			m_popupMenu->setItemEnabled(ID_GM_ITEMS_DELETE, false);
						
			m_popupMenu->setItemEnabled(ID_GM_MODULES_SEARCH, false);
			m_popupMenu->setItemEnabled(ID_GM_MODULE_UNLOCK, false);			
			m_popupMenu->setItemEnabled(ID_GM_MODULE_ABOUT, false);			
	}
}

/**  */
void CGroupManager::slotShowAbout(){
	if (!m_pressedItem || !m_pressedItem->moduleInfo())
		return;
	
	CHTMLDialog* dlg = new CHTMLDialog(this, "about dialog");
	ASSERT(dlg);
	
	QString text;
	
	CSwordModuleInfo* module = dynamic_cast<CSwordModuleInfo*>(m_pressedItem->moduleInfo());
	if (!module) {
		qWarning("Invalid module");
		return;
	}		
	ASSERT(module);
		
	QString isWritable;
	if (module->module()->isWritable())
		isWritable = i18n("Yes");
	else
		isWritable = i18n("No");			

	QString hasFootnotes = i18n("No");			
	if (module->supportsFeature(CSwordBackend::footnotes))
		hasFootnotes = i18n("Yes");
		
	QString hasStrongNumbers = i18n("No");
	if (module->supportsFeature(CSwordBackend::strongNumbers))
		hasStrongNumbers = i18n("Yes");
		
	QString unlockKey = QString::fromLatin1("<I>%1</I>").arg(i18n("Not necessary"));
	if ( module->isEncrypted() )
		unlockKey = module->getCipherKey();	
		
	QString encoding = i18n("Unicode") ;
	if (!module->isUnicode())
		encoding = i18n("iso8859-1");		
				
	text = QString::fromLatin1("<HTML><HEAD></HEAD><BODY>\
<TABLE border=\"0\" height=\"100%\" width=\"100%\" cellspacing=\"0\" cellpadding=\"2\">\
<TR><TD BGCOLOR=\"#0F86D0\" ALIGN=\"center\" COLSPAN=\"2\"><H2>%1</H2></TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%2:</B></TD><TD BGCOLOR=\"#FFE9C8\">%3</TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%4:</B></TD><TD BGCOLOR=\"#FFE9C8\">%5</TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%6:</B></TD><TD BGCOLOR=\"#FFE9C8\">%7</TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%8:</B></TD><TD BGCOLOR=\"#FFE9C8\">%9</TD></TR>")
	.arg(module->name())
	.arg(i18n("Datapath"))
	.arg(module->getPath())
	.arg(i18n("Version"))
	.arg(module->hasVersion() ? module->getVersion() : QString::fromLatin1("<I>%1</I>").arg(i18n("Not available")))
	.arg(i18n("Unlock key"))
	.arg(unlockKey)
	.arg(i18n("Language"))
	.arg(QString::fromLatin1(module->module()->Lang()));


text += QString::fromLatin1("<TR><TD BGCOLOR=\"#0F86D0\"><B>%1:</B></TD><TD BGCOLOR=\"#FFE9C8\">%2</TD></TR>")
	.arg(i18n("Writable"))
	.arg(isWritable);

		
text += QString::fromLatin1("\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%1:</B></TD><TD BGCOLOR=\"#FFE9C8\">%2</TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%3:</B></TD><TD BGCOLOR=\"#FFE9C8\">%4</TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%5:</B></TD><TD BGCOLOR=\"#FFE9C8\">%6</TD></TR>\
<TR><TD BGCOLOR=\"#0F86D0\"><B>%7:</B></TD><TD BGCOLOR=\"#FFE9C8\">%8</TD></TR>")
	.arg(i18n("Footnotes"))	
	.arg(hasFootnotes)
	.arg(i18n("Strong's numbers"))	
	.arg(hasStrongNumbers)
	.arg(i18n("Encoding"))	
	.arg(encoding)	
	.arg(i18n("Description"))	
	.arg(module->getDescription());

text += QString::fromLatin1("\
<TR><TD VALIGN=\"TOP\" BGCOLOR=\"#0F86D0\"><B>%1:</B></TD><TD BGCOLOR=\"#FFE9C8\">%2</TD></TR>\
</TABLE></BODY></HTML>")
	.arg(i18n("About"))
	.arg(module->getAboutInformation());
	
	dlg->setText(text);
	dlg->exec();
	delete dlg;	
}

/**  */
void CGroupManager::slotCreateNewPresenter(){
	if (m_pressedItem && m_pressedItem->moduleInfo()) {
		if (m_pressedItem->type() == CGroupManagerItem::Module || m_pressedItem->type() == CGroupManagerItem::Bookmark)
			emit createSwordPresenter( m_pressedItem->moduleInfo(), QString::null );
	}
}

/**  */
void CGroupManager::contentsDragEnterEvent( QDragEnterEvent* e){
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
	if (item && item->type() == CGroupManagerItem::Group && !item->isOpen())
		item->setOpen(true);

	m_afterItemDrop = item;
	if (item)
		m_parentItemDrop = (CGroupManagerItem*)item->parent();
	else
		m_parentItemDrop = 0;
	
	QRect tmpRect = drawDropVisualizer(0, m_parentItemDrop, m_afterItemDrop, m_dragType );
	if (tmpRect != m_oldDragRect ) {
		cleanDropVisualizer();
		m_oldDragRect = tmpRect;
		if (tmpRect.isValid())
			viewport()->repaint(tmpRect);
	}
}

/**  */
void CGroupManager::contentsDragLeaveEvent( QDragLeaveEvent* e){	
	KListView::contentsDragLeaveEvent(e);	
	cleanDropVisualizer();
	
	m_oldDragRect = QRect();
	m_dragType = "";
}

/** This define is used to use one block of code for the different sections (bookmarks,
	* references, modules, groups etc.). We have only to maintain this code.
	*/
#define MOVE_ITEMS  \
			for ( item = m_itemList->last(); item != 0; item = m_itemList->prev()){ \
				if ( item && (item != target) ){ \
					parentItem = dynamic_cast<CGroupManagerItem*>(item->parent()); \
					if ( isChild(item, target) ) \
						continue; \
					if (parentItem && parentItem != item) \
						parentItem->takeItem(item); \
					else \
						takeItem(item); \
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
  CGroupManagerItem* target = dynamic_cast<CGroupManagerItem *>(itemAt(contentsToViewport(e->pos())));

  QString str;
  QCString submime;
  if (QTextDrag::decode(e,str,submime=BOOKMARK)){
    //a bookmark was dragged
    if ( e->source() != viewport() ){
      return;
    }
    if ( !(m_itemList) ){
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
    if ( e->source() != this->viewport() )
      return;
    if ( !(m_itemList) )
      return;
    CGroupManagerItem* item = 0;
    CGroupManagerItem* parentItem = 0;
    //move around groups    	
    MOVE_ITEMS
  }//group
  if (QTextDrag::decode(e,str,submime=MODULE)){
    //a group was dragged
    if ( e->source() != this->viewport() ){
      return;
    }
    if ( !(m_itemList) ){
      return;
    }
    CGroupManagerItem* item = 0;
    CGroupManagerItem* parentItem = 0;
    MOVE_ITEMS
  }//module
  else if (QTextDrag::decode(e,str,submime=REFERENCE)){
    //a reference was dragged
		QString ref;
    QString mod;
    CReferenceManager::decodeReference(str,mod,ref);

    CSwordModuleInfo* info = 0;
    for (info = m_swordList->first(); info; info = m_swordList->next())
      if (info->name() == mod)
      	break;
    if ( info && (info->name() == mod) ){
			if (!target){ //Reference was dragged on no item
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
						if (target->parent())
							createNewBookmark(target->parent(),info, ref);
						else
							createNewBookmark(0,info, ref);						
						break;
					case (CGroupManagerItem::Module):
						if (target->moduleInfo()){
							/**
							* In bibles or commentaries, the reference is opened
							* in lexicons, the reference is searched
							*/
							if (dynamic_cast<CSwordModuleInfo*>(target->moduleInfo()) ) {
								if (dynamic_cast<CSwordBibleModuleInfo*>(target->moduleInfo()) )
									emit createSwordPresenter( target->moduleInfo(),ref );
								else if (dynamic_cast<CSwordLexiconModuleInfo*>(target->moduleInfo()))
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
void CGroupManager::contentsMousePressEvent( QMouseEvent* e ) {
	m_pressedPos = e->pos();
  m_pressedItem = dynamic_cast<CGroupManagerItem*>(itemAt(contentsToViewport(m_pressedPos)));
  const bool open = m_pressedItem ? m_pressedItem->isOpen() : false;
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
	else if (e->button() == RightButton & m_useRMBMenu) {	
		m_menu = true;
		m_popupMenu->exec( viewport()->mapToGlobal( contentsToViewport(m_pressedPos) ));
		m_menu = false;			
	}
}

/** Reimplementation. */
void CGroupManager::contentsMouseDoubleClickEvent ( QMouseEvent * e){
	qDebug("CGroupManager::contentsMouseDoubleClickEvent ( QMouseEvent * e)");
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
	KListView::contentsMouseReleaseEvent(e); 	
  if ( !(m_pressedItem = dynamic_cast<CGroupManagerItem*>(itemAt(contentsToViewport(e->pos())))) )
    return;	
	if ((e->state() & ControlButton) || (e->state() & ShiftButton))
		return;

	if (m_pressedItem && (e->button() == LeftButton)) {
		if (m_pressedItem->type() == CGroupManagerItem::Module && m_pressedItem->moduleInfo()) {
	  	//check if module is encrypted and show dialog if it wasn't opened before	  	
			QList<QListViewItem> items = selectedItems();			
			if (items.count() > 1) {
				ListCSwordModuleInfo modules;
				for (items.first(); items.current(); items.next()) {
					CGroupManagerItem* i = dynamic_cast<CGroupManagerItem*>(items.current());
					if (i && i->type() == CGroupManagerItem::Module && i->moduleInfo())
						modules.append(i->moduleInfo());
				}
				emit createSwordPresenter( modules, QString::null );				
			}
			else
				emit createSwordPresenter( m_pressedItem->moduleInfo(), QString::null );
					  	
	  	if (m_pressedItem->moduleInfo()->isEncrypted()) {
  			KConfigGroupSaver groupSaver(m_config, "Groupmanager");
	  		if (m_showHelpDialogs && !m_config->readBoolEntry(QString::fromLatin1("shown %1 encrypted").arg(m_pressedItem->moduleInfo()->name()), false))
	  			HTML_DIALOG(HELPDIALOG_MODULE_LOCKED);
	  		if (m_showHelpDialogs)
	  			m_config->writeEntry(QString::fromLatin1("shown %1 encrypted").arg(m_pressedItem->moduleInfo()->name()), true);
	  	}
		}
		else if  (m_pressedItem && m_pressedItem->type() == CGroupManagerItem::Bookmark) {
			if (m_pressedItem->moduleInfo() && m_pressedItem->getBookmarkKey() )
				emit createSwordPresenter( m_pressedItem->moduleInfo(), m_pressedItem->getKeyText() );
		}
	}
}

/** Reimplementation */
void CGroupManager::contentsMouseMoveEvent ( QMouseEvent * e) {
	KListView::contentsMouseMoveEvent( e );	
	if (!m_useDragDrop)
		return;
	
	CGroupManagerItem* dragItem=(CGroupManagerItem *)itemAt( contentsToViewport(e->pos()) );

	//mouse is pressed, an item is selected and the popup menu isn't opened
	if ( (e->state()&LeftButton) && (e->stateAfter()&LeftButton) && dragItem && !m_menu) {
		//Is it time to start a drag?
   	if (abs(e->pos().x() - m_pressedPos.x()) > KGlobalSettings::dndEventDelay() ||
				abs(e->pos().y() - m_pressedPos.y()) > KGlobalSettings::dndEventDelay() )	{
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
	           	QString mod = dragItem->moduleInfo()->name();
	            	
	          	d = new QTextDrag(CReferenceManager::encodeReference(mod,ref), viewport());
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
	QString groupname = QInputDialog::getText(i18n("Enter name of folder - BibleTime"),i18n("Please enter the name of the folder!"),"", &isOk, 0);
	
	if (isOk) {
		if (m_pressedItem && m_pressedItem->type() == CGroupManagerItem::Group) {
			(void)new CGroupManagerItem(m_pressedItem, groupname, QString::null, 0,0, CGroupManagerItem::Group, m_important);
			m_pressedItem->setOpen(true);
		}
		else if (m_pressedItem && m_pressedItem->parent() && m_pressedItem->parent()->type() == CGroupManagerItem::Group){
			(void)new CGroupManagerItem(m_pressedItem->parent(), groupname, QString::null, 0,0, CGroupManagerItem::Group, m_important);
			m_pressedItem->parent()->setOpen(true);
		}
		else {
			(void)new CGroupManagerItem(this, groupname, QString::null, 0, 0,CGroupManagerItem::Group, m_important);
		}
	}
}

const int CGroupManager::parentId(CGroupManagerItem *item, CGroupManagerItem* parentItem)
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
CGroupManagerItem* CGroupManager::findParent( const int ID, CGroupManagerItem* parentItem){
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

/** Reads in bookmarks from m_config and creates them as subitems of group. If group is 0 we create them a toplevel items. */
const bool CGroupManager::readSwordBookmarks(KConfig* configFile, CGroupManagerItem* group){
	qDebug("CGroupManager::read Sword bookmarks");
	
	//read and create group entries
	CGroupManagerItem* 	parentItem = 0;	
	QStringList				groupList 	= configFile->readListEntry("Groups");

	//read in all bookmarks
	QStringList	bookmarkList 							= configFile->readListEntry("Bookmarks");
	QStringList	bookmarkModulesList 			= configFile->readListEntry("Bookmark modules");
	QStringList	bookmarkDescriptionsList 	= configFile->readListEntry("Bookmark descriptions");
	QValueList<int>	parentList						= configFile->readIntListEntry("Bookmark parents");

	QStringList::Iterator it_bookmarks 	= bookmarkList.begin();
	QStringList::Iterator it_modules 		= bookmarkModulesList.begin();
	QStringList::Iterator it_descriptions	= bookmarkDescriptionsList.begin();
	QValueList<int>::Iterator it_parents = parentList.begin();

	CSwordModuleInfo* moduleInfo = 0;
	CGroupManagerItem *myItem = 0;	
	CGroupManagerItem *oldItem = 0;

	while ( it_bookmarks != bookmarkList.end() && it_parents != parentList.end()
			 		&& it_modules != bookmarkModulesList.end() ) {
		moduleInfo = m_important->swordBackend->findModuleByName((*it_modules));
		if ( (*it_parents) == -1) {
			if (group)
				myItem = new CGroupManagerItem(group, (*it_bookmarks), (*it_modules), moduleInfo, 0, CGroupManagerItem::Bookmark, m_important);
			else
				myItem = new CGroupManagerItem(this, (*it_bookmarks), (*it_modules), moduleInfo, 0, CGroupManagerItem::Bookmark, m_important);
		}
		else {
			parentItem = findParent( (*it_parents), group ? group : 0 );
			if (parentItem)
				myItem = new CGroupManagerItem(parentItem, (*it_bookmarks), (*it_modules), moduleInfo,0, CGroupManagerItem::Bookmark, m_important);
			else if (group)
				myItem = new CGroupManagerItem(group, (*it_bookmarks), (*it_modules), moduleInfo,0, CGroupManagerItem::Bookmark, m_important);
			else
				myItem = new CGroupManagerItem(this, (*it_bookmarks), (*it_modules), moduleInfo,0, CGroupManagerItem::Bookmark, m_important);
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

/** Save items of group to m_config. If grou is 0 we save all items. The  path to the group-item itself is saved, too. */
const bool CGroupManager::saveSwordBookmarks(KConfig* configFile, CGroupManagerItem* group){
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
				SWKey* key = dynamic_cast<SWKey*>(myItem->getBookmarkKey());			
				if (key)
					bookmarkList.append( QString::fromLocal8Bit((const char*)*key) );
			}
			else
				bookmarkList.append( "" );	//invalid key
				
			if (myItem && !myItem->description().isEmpty())	//save description
				bookmarkDescriptionList.append( myItem->description() );
			else
				bookmarkDescriptionList.append( QString::null );
				
			if (myItem->moduleInfo()) {	//save the module
				bookmarkModuleList.append( myItem->moduleInfo()->name() );
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
	if (!m_pressedItem || ( m_pressedItem && m_pressedItem->type() == CGroupManagerItem::Group) ) {
		QString file = KFileDialog::getOpenFileName(QString::null, "*.btb | *.btb", 0, i18n("Import bookmarks ..."));	
		if (!file.isNull()) {
			KSimpleConfig simpleConfig(file, false);
			simpleConfig.setGroup("Bookmarks");
			short int formatVersion = simpleConfig.readNumEntry("Bookmark format version", -1);
			
			//bookmark format of imported file is newer than our version			
			if (formatVersion > BOOKMARK_FORMAT_VERSION) {
				int retValue = KMessageBox::warningContinueCancel(this, i18n("<qt>A problem occurred while importing bookmarks!<BR>\
The bookmarks format of the imported file is newer<BR>\
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


const bool CGroupManager::readSwordModules(KConfig* configFile, CGroupManagerItem* group) {
	if (!m_swordList) {
		qWarning("no sword modules, return false.");
		return false;
	}
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
		
		myModuleInfo = m_important->swordBackend->findModuleByName(*it_modules);
		if (!myModuleInfo) {	//if the module was removed so we don't show it
			++it_parents;
			++it_modules;
			continue;
		}
			
		if ( (*it_parents) == -1) {
		 	if (group)
				myItem = new CGroupManagerItem(group, QString::null, QString::null, myModuleInfo,0, CGroupManagerItem::Module, m_important);
			else
				myItem = new CGroupManagerItem(this, QString::null, QString::null, myModuleInfo,0, CGroupManagerItem::Module, m_important);
		}
		else {
			parentItem = findParent( (*it_parents), group ? group : 0  );
			if (parentItem)
				myItem = new CGroupManagerItem(parentItem, QString::null, QString::null, myModuleInfo, 0,CGroupManagerItem::Module, m_important);	
			else if (group)
				myItem = new CGroupManagerItem(group, QString::null, QString::null, myModuleInfo, 0, CGroupManagerItem::Module, m_important);
			else				
				myItem = new CGroupManagerItem(this, QString::null, QString::null, myModuleInfo, 0, CGroupManagerItem::Module, m_important);
		}
		if (myItem && oldItem)
			myItem->moveToJustAfter(oldItem);
		oldItem = myItem;
			
		++it_parents;
		++it_modules;
	}	
	return true;
};

const bool CGroupManager::saveSwordModules(KConfig* configFile, CGroupManagerItem* group) {
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
					moduleList.append( myItem->moduleInfo()->name() );				
				else {
					moduleList.append( myItem->text(0) );	//first column is the modulename
				}
			}
	}	
	configFile->writeEntry("Modules", moduleList);
	configFile->writeEntry("Module parents", parentList);

	return true;
}

const bool CGroupManager::readGroups(KConfig* configFile, CGroupManagerItem* group) {
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
				newItem = new CGroupManagerItem(group, (*it_groups), QString::null, 0,0, CGroupManagerItem::Group, m_important);
			else
				newItem = new CGroupManagerItem(this, (*it_groups), QString::null, 0,0, CGroupManagerItem::Group, m_important);
		}
		else {
			parentItem = findParent( (*it_parents),group ? group : 0  );			
			if (parentItem)
				newItem = new CGroupManagerItem(parentItem, (*it_groups),QString::null,0,0, CGroupManagerItem::Group, m_important);
			else if (group)
				newItem = new CGroupManagerItem(group, (*it_groups),QString::null,0, 0,CGroupManagerItem::Group, m_important);
			else
				newItem = new CGroupManagerItem(this, (*it_groups),QString::null,0, 0,CGroupManagerItem::Group, m_important);
		}
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

const bool CGroupManager::saveGroups(KConfig* configFile, CGroupManagerItem* group) {
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
const bool CGroupManager::isChild(QListViewItem* parent, QListViewItem* child){
	QListViewItem *myParent = child;
	while (myParent && myParent != parent )
		myParent = myParent->parent();		
	return myParent == parent && parent!=child;
}

/** Reimplementatuiion. */
void CGroupManager::viewportPaintEvent(QPaintEvent* e) {
  KListView::viewportPaintEvent(e);

  if (m_oldDragRect.isValid())
  {
    static bool invalidated=false;
    if (!invalidated)
    {
      invalidated=true;
      viewport()->repaint(m_oldDragRect);
    }
    QPainter painter(viewport());
    drawDropVisualizer(&painter, m_parentItemDrop, m_afterItemDrop, "");
    invalidated=false;
  }
}

/** Reimplementation with different parameters */
const QRect CGroupManager::drawDropVisualizer (QPainter *p, CGroupManagerItem */*parent*/, CGroupManagerItem *after, const QString& /*type*/){
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
		else //if (!after)
			insertmarker = QRect(); //this->visibleRect();		
//		else
//			insertmarker = QRect();
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
  if ( m_oldDragRect.isValid() ) {
    QRect rect = m_oldDragRect;
    m_oldDragRect = QRect();
    viewport()->repaint(rect,true);
  }
}

/** Resets the groupmanager. Recreates the original Bibles, Lexicons and Commentary groups. */
void CGroupManager::slotReset(){
	int result = 0;
	if (m_showHelpDialogs)
		result = KMessageBox:: warningContinueCancel( this, i18n("<qt>This function will reset the main index!<BR>This will recreate the original groups and delete all other items!<BR>Be sure no important bookmarks will be deleted!<BR>Do you want to continue?</qt>"), i18n("BibleTime - Reset main index"), i18n("Continue"));
	else
		result = KMessageBox::Continue;
		
	if (result == KMessageBox::Continue) {
		clear();
		setupStandardSwordTree();
	}
}

/** Prints the selected bookmark. */
void CGroupManager::slotPrintBookmark(){
	if (!m_pressedItem || ( m_pressedItem && m_pressedItem->type() != CGroupManagerItem::Bookmark) )
		return;
	CPrinter*	printer = m_important->printer;	
	ASSERT(printer);
	
	CPrintItem*	printItem = new CPrintItem();
	printItem->setDescription( m_pressedItem->description() );
	CSwordKey* k = m_pressedItem->getBookmarkKey()->copy();
	ASSERT(k);
	qWarning(m_pressedItem->getBookmarkKey()->key().local8Bit());	
	qWarning("wanna print the key");	
	qWarning(k->key().local8Bit());
	printItem->setStartKey( /*m_pressedItem->getBookmarkKey()->copy()*/k );
	printItem->setStopKey( /*m_pressedItem->getBookmarkKey()->copy()*/k );
	printItem->setModule(m_pressedItem->moduleInfo());
	
	printer->addItemToQueue( printItem );
}

/** Deletes the searchdialog. */
void CGroupManager::slotDeleteSearchdialog(){
	if (m_searchDialog)
		m_searchDialog->delayedDestruct();	//delete the search dialog
	m_searchDialog = 0;
}

/** Reimplementation. */
void CGroupManager::resizeEvent( QResizeEvent* e )  {
	KListView::resizeEvent(e);		
	setColumnWidth(0, visibleWidth() );
	triggerUpdate();
	
}

/** Is called when the return key was pressed on a listview item. */
void CGroupManager::slotReturnPressed( QListViewItem* i){
	CGroupManagerItem* item = dynamic_cast<CGroupManagerItem*>(i);
	if (!item)
		return;	
	switch (item->type())	 {
		case CGroupManagerItem::Bookmark:
			if (item->moduleInfo())				
				emit createSwordPresenter(item->moduleInfo(),item->getKeyText() );			
			break;
		case CGroupManagerItem::Module:
			if (item->moduleInfo())
				emit createSwordPresenter(item->moduleInfo(), QString::null);			
			break;
		case CGroupManagerItem::Group:
			item->setOpen( !item->isOpen() );
			break;
		default:
			break;
	}
}

/** Updates the content size of this list view. */
void CGroupManager::resizeContents( int w, int h ){
	if (m_useDragDrop)
		KListView::resizeContents(w,h+25);
	else
		KListView::resizeContents(w,h);
}
