/***************************************************************************
                          cgroupmanageritem.cpp  -  description
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

#include "cgroupmanageritem.h"
#include "cgroupmanager.h"
#include "../../backend/cmoduleinfo.h"
#include "../../backend/sword_backend/cswordversekey.h"
#include "../../backend/sword_backend/cswordldkey.h"
#include "../../backend/sword_backend/cswordmoduleinfo.h"
#include "../../backend/sword_backend/cswordbiblemoduleinfo.h"
#include "../../backend/sword_backend/cswordlexiconmoduleinfo.h"
#include "../../backend/ckey.h"
#include "../../ressource.h"
#include "../ctoolclass.h"

//Qt includes
#include <qpixmap.h>

//KDE includes
#include <klocale.h>
#include <kglobal.h>
#include <kiconloader.h>

//Sword includes
#include <swmodule.h>


CGroupManagerItem::CGroupManagerItem(CGroupManager *parent, const QString& caption,
	const QString& modulename, CModuleInfo *module_info, CKey* bookmarkKey, CGroupManagerItem::itemType Type )
	: QListViewItem((QListView*)parent) {	
	
	init(caption, modulename, module_info, bookmarkKey, Type);
}

CGroupManagerItem::CGroupManagerItem(CGroupManagerItem *parent, const QString& caption,
	const QString& modulename, CModuleInfo *module_info, CKey* bookmarkKey, CGroupManagerItem::itemType Type )
	: QListViewItem((QListViewItem*)parent) {
	
	init(caption, modulename, module_info,bookmarkKey, Type);
}

CGroupManagerItem::~CGroupManagerItem(){
	if (m_bookmarkKey && m_createdOwnKey)
		delete m_bookmarkKey;
}

/** Returns the CModuleInfo object for this tree-item. */
CSwordModuleInfo* CGroupManagerItem::moduleInfo(){
	return m_moduleInfo;
}

/** Returns the type of this item */
CGroupManagerItem::itemType CGroupManagerItem::type(){
	return m_type;
}

/** Shows child or not */
void CGroupManagerItem::setOpen( bool open ){
	QListViewItem::setOpen(open);
	if (m_type == Group) {
  	if (open && this->childCount() > 0)
			setPixmap(0,m_openFolderIcon);
		else
			setPixmap(0,m_folderIcon);
	}
}

/** Initializes the item Used by both constructors to share the double used source code */
void CGroupManagerItem::init( const QString& caption, const QString& modulename, CModuleInfo *module_info, CKey* bookmarkKey, CGroupManagerItem::itemType Type){
	m_bookmarkKey = 0;
	m_createdOwnKey = false;
	m_moduleInfo = 0;	
	m_caption = QString::null;
	m_description = QString::null;
	
	setType(Type);	
	if (bookmarkKey)
		setBookmarkKey( bookmarkKey );		
	setModuleInfo( module_info );
	m_caption = caption;
	m_moduleName = modulename;
	
	update();	//refresh the pixmap, texts etc.
}

/** Sets / changes the description */
void CGroupManagerItem::setDescription(const QString& new_description){
	m_description = new_description;
	update();
}

/** Returns the description of the item */
QString CGroupManagerItem::description() const{
	return m_description;
}

/** Returns the parent of this item. Reimplementation. */
CGroupManagerItem* CGroupManagerItem::parent(){
	CGroupManagerItem* myParent = (CGroupManagerItem*)QListViewItem::parent();	
	if (myParent)
		return myParent;
	else
		return 0;
}

/** Returns the listview of this item */
CGroupManager* CGroupManagerItem::listView(){
	CGroupManager* myListView = (CGroupManager*)QListViewItem::listView();
	if (myListView)
		return myListView;
	else
		return 0;	
}

/** Returns the key if it's a Bookmark, othwerwise return 0 */
CKey* CGroupManagerItem::getBookmarkKey(){
	return m_bookmarkKey;
}

/** Sets the key, which is only used if this item is a Bookmark */
void CGroupManagerItem::setBookmarkKey( CKey* key ){
	if (m_bookmarkKey && m_createdOwnKey)
		delete m_bookmarkKey;		
	m_bookmarkKey = key;
	m_createdOwnKey = false;
	update();
}

/** Updates this item (icons, captions, etc.) */
void CGroupManagerItem::update(){
	if (m_type == Group) {
		m_folderIcon = GROUP_ICON_SMALL;
		m_openFolderIcon = GROUP_OPEN_ICON_SMALL;		
		setPixmap(0, m_folderIcon);
		setText(0, m_caption);
	}
  else if (m_type == Bookmark) {
		QString title = QString::null;
		setPixmap(0, BOOKMARK_ICON_SMALL);
		if ( getBookmarkKey() ) {	//if we have a valid key
			SWKey* swKey = (SWKey*)(getBookmarkKey());
			if (swKey)	// a valid Sword key
				title = QString::fromLocal8Bit((const char*)*swKey);
		}
		else if (!m_caption.isEmpty()){	//bookmark key is 0, we use now the m_caption member to create a valid key
			if ( m_moduleInfo && dynamic_cast<CSwordBibleModuleInfo*>(m_moduleInfo) ) {	//a Bible or a commentary module
				CSwordVerseKey* key = new CSwordVerseKey(m_moduleInfo);
				m_createdOwnKey = true;				
				key->setKey(m_caption);
				setBookmarkKey(key);
				update();	// this won't lead to a infinite loop because we have now a valid key
			}
			else if ( m_moduleInfo && dynamic_cast<CSwordLexiconModuleInfo*>(m_moduleInfo) ) {	//a Bible or a commentary module
				CSwordLDKey* key = new CSwordLDKey(m_moduleInfo);
				m_createdOwnKey = true;
				key->setKey(m_caption);
				setBookmarkKey(key);
				update();	// this won't lead to a infinite loop because we have now a valid key
			}			
		}		
		if (!title.isEmpty()) {
			if (m_moduleInfo && m_moduleInfo->module())
				title = QString("%1 (%2)").arg(title).arg(m_moduleInfo->module()->Name());
			else
				title = QString("%1 (unknown)").arg(title);
			setText(0,title);
		}
	}
	else if ( m_type == CGroupManagerItem::Module ) {
		setPixmap(0, CToolClass::getIconForModule(m_moduleInfo));
		if (m_moduleInfo)
			setText(0, m_moduleInfo->module()->Name());
		else if (!m_moduleName.isEmpty())
			setText(0, m_moduleName);			
		else
			setText(0, i18n("unknown"));
	}
}

/** Sets the type of the item. */
void CGroupManagerItem::setType( CGroupManagerItem::itemType type){
	m_type = type;
}

/** Sets the module of ths item. */
void CGroupManagerItem::setModuleInfo( CModuleInfo* moduleInfo ){
//	ASSERT(moduleInfo);
	if (!moduleInfo)
		return;
		
	if ((CSwordModuleInfo*)moduleInfo)
		m_moduleInfo = (CSwordModuleInfo*)moduleInfo;
  else
  	m_moduleInfo = 0;
}

/** Returns a QString version of the key. */
QString CGroupManagerItem::getKeyText(){
	QString ret = QString::null;	
	SWKey* skey = (SWKey*)m_bookmarkKey;	
	if (skey)
		ret = QString::fromLocal8Bit((const char*)*skey);
	return ret;
}

/** Returns the tooltip for this ite, QString::null is returned if this item has no tooltip. */
const QString CGroupManagerItem::getToolTip(){
	QString text = QString::null;;
	switch ( type() ) {
		case Bookmark:
		{
			text = i18n("Bookmark to" ) + QString(" ");
			text.append(QString("<B>") + getKeyText() + "</B><BR>");
			if (!description().stripWhiteSpace().isEmpty())
				text.append("<FONT color=\"#800000\">(" + description().stripWhiteSpace() + ")</FONT><BR>");
			text.append("<HR>");			
				
			CKey* key = getBookmarkKey();			
			if (!key)
				return QString::null;				
			CSwordVerseKey* vk = (CSwordVerseKey*)(key);
			CSwordLDKey* lk = (CSwordLDKey*)(key);
					
			QString bookmarkText = 	vk ? vk->getRenderedText() : (lk ? lk->getRenderedText() : QString());					
			if (bookmarkText.length() > 150 && (moduleInfo()->getType() != CSwordModuleInfo::Bible))
				bookmarkText = bookmarkText.left(150) + "...";
						
			if (moduleInfo() && moduleInfo()->hasFont()) {
				QFont f = moduleInfo()->getFont();
				text.append( QString("<FONT FACE=\"%1\" SIZE=\"%2\">%3</FONT>").arg(f.family()).arg(CToolClass::makeLogicFontSize(f.pointSize())).arg(bookmarkText) );
			}
			else
				text.append(bookmarkText);

      return text;				
		}
		case Module:
		{
			text = i18n("Module") + QString(": <B>%1</B><HR>").arg(QString::fromLocal8Bit(moduleInfo()->module()->Name()));
			text += QString("%1<HR>").arg(moduleInfo()->getDescription());
			text += i18n("Foonotes: %1<BR>").arg(moduleInfo()->supportsFeature(CSwordBackend::strongNumbers) ? i18n("Yes") : i18n("No"));
			text += i18n("Strong's numbers: %1<BR>").arg(moduleInfo()->supportsFeature(CSwordBackend::footnotes) ? i18n("Yes") : i18n("No"));
			if (moduleInfo()->isEncrypted())
				text += QString("Unlock key: %1<BR>").arg(!moduleInfo()->getCipherKey().isEmpty() ? moduleInfo()->getCipherKey() : QString("<FONT COLOR=\"red\">%1</FONT>").arg(i18n("not set")));
			if (!moduleInfo()->getVersion().isEmpty())
				text += QString("Version: %1<BR>").arg(moduleInfo()->getVersion());
			if (moduleInfo()->hasFont())
				text += QString("Font: %1<BR>").arg(moduleInfo()->getFont().family());
			if (text.right(4) == "<BR>")
				text = text.left(text.length()-4);
			return text;
		}
		case Group:
		default:
			return QString::null;
	}
}
