/***************************************************************************
                          cindexitem.cpp  -  description
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
#include "cindexitem.h"
#include "cmainindex.h"

#include "backend/creferencemanager.h"
#include "backend/cswordmoduleinfo.h"
#include "backend/cswordversekey.h"
#include "backend/clanguagemgr.h"

#include "frontend/cexportmanager.h"
#include "frontend/ctooltipmanager.h"
#include "frontend/cbtconfig.h"
#include "frontend/cinputdialog.h"
#include "frontend/cexportmanager.h"
#include "frontend/cdragdropmgr.h"
#include "frontend/searchdialog/csearchdialog.h"

#include "util/ctoolclass.h"

#include "resource.h"

#include <string.h>

//Qt includes
#include <qdragobject.h>
#include <qstringlist.h>
#include <qfile.h>
#include <qstring.h>
#include <qtextstream.h>

//KDE includes
#include <kconfig.h>
#include <klocale.h>
#include <kfiledialog.h>
#include <kstandarddirs.h>

#define CURRENT_SYNTAX_VERSION 1

using std::string;

CItemBase::CItemBase(CMainIndex* mainIndex, const Type type) : KListViewItem(mainIndex), m_type(type) {
  m_type = type;
}

CItemBase::CItemBase(CItemBase* parentItem, const Type type) : KListViewItem(parentItem), m_type(type) {
  m_type = type;
}

CItemBase::~CItemBase() {
}

const QString CItemBase::toolTip() {
  return QString::null;
}

/** Returns the used main index. */
CMainIndex* CItemBase::listView() const{
  return dynamic_cast<CMainIndex*>( QListViewItem::listView() );
}

void CItemBase::init() {
  update();
}

void CItemBase::update() {
}

const CItemBase::Type& CItemBase::type() const{
  return m_type;
};

void CItemBase::moveAfter( CItemBase* const item ){
	if (!item)
		return;
	
	if ( parent() != item->parent() ) { //different levels
//		if (type == AllowDifferentParents) { //different parents are allowed
//			if (item->parent())
//				item->parent()->insertItem(this); //insert item to the childs
//			else
//				listView()->insertItem(this);
//			moveItem(item);
//		}
	}
	else {
		moveItem(item); //both items are on the same level, so we can use moveItem
	}
}

void CItemBase::dropped( QDropEvent* /*e*/ ) {
}


/** Returns true if the given action should be enabled in the popup menu. */
const bool CItemBase::enableAction( const MenuAction /*action*/ ){
  return false; //this base class has no valif actions
}

/** No descriptions */
const bool CItemBase::isMovable(){
  return false;
}

const bool CItemBase::allowAutoOpen( const QMimeSource* ) const {
  return false;
};

/* ---------------------------------------------- */
/* ---------- new class: CModuleItem ------------ */
/* ---------------------------------------------- */

CModuleItem::CModuleItem(CTreeFolder* parentItem, CSwordModuleInfo* module) : CItemBase(parentItem), m_module(module) {
}

CModuleItem::~CModuleItem() {
}

/** No descriptions */
void CModuleItem::update(){
  if (m_module) {
    setPixmap(0, CToolClass::getIconForModule(m_module));
    setText(0,m_module->name() );
  }
}

void CModuleItem::init(){
  setDragEnabled(false);
  setDropEnabled(true);

  update();
}


/** Reimplementation to handle text drops on a module. In this case open the searchdialog. In the case of a referebnce open the module at the given position. */
bool CModuleItem::acceptDrop( const QMimeSource* src ) const {
  if (CDragDropMgr::canDecode(src)) {
    if (CDragDropMgr::dndType(src) == CDragDropMgr::Item::Bookmark) {
      CDragDropMgr::Item item = CDragDropMgr::decode(src).first();
      CSwordModuleInfo* m = backend()->findModuleByName( item.bookmarkModule() );
      if (m && module()->type() == m->type()) { //it makes only sense
        return true;
      }
      //but we also allow drops from bibles on commentaries and the other way from commentaries
      else if (m && (module()->type() == CSwordModuleInfo::Bible) && (m->type() == CSwordModuleInfo::Commentary)) {
        return true;
      }
      else if (m && (module()->type() == CSwordModuleInfo::Commentary) && (m->type() == CSwordModuleInfo::Bible)) {
        return true;
      }
    }
    else if(CDragDropMgr::dndType(src) == CDragDropMgr::Item::Text) { //text drop on a module
      return true;
    };
  }
  return false; //default return value
}

/** No descriptions */
void CModuleItem::dropped( QDropEvent* e ){
  /* Something was dropped on a module item
  *
  * 1. If the drop type is plain text open the searchdialog for this text and start the search
  * 2. If the type is Bookmark, open the module at the specified position
  *
  * We support only the first drop item, more is not useful
  */

  if (acceptDrop(e)) {
    CDragDropMgr::ItemList dndItems = CDragDropMgr::decode(e);
    CDragDropMgr::Item item = dndItems.first();
    if (CDragDropMgr::dndType(e) == CDragDropMgr::Item::Text) { //open the searchdialog
//      qWarning("Text dropped!");
  		if ( module() ) {
        ListCSwordModuleInfo modules;
        modules.append(module());

        CSearchDialog::openDialog(modules, item.text());
      }
    }
    else if (CDragDropMgr::dndType(e) == CDragDropMgr::Item::Bookmark) { //open the module
//      qWarning("type is Bookmark!");    
      CSwordModuleInfo* m = backend()->findModuleByName( item.bookmarkModule() );
      if (m) { //it makes only sense to create a new window for a module with the same type
        if ((module()->type() == m->type()) ||
            ((module()->type() == CSwordModuleInfo::Bible || module()->type() == CSwordModuleInfo::Commentary)
            && (m->type() == CSwordModuleInfo::Bible || m->type() == CSwordModuleInfo::Commentary)))
        { //same base type of module
          ListCSwordModuleInfo modules;
          modules.append(module());

          listView()->emitModulesChosen(modules, item.bookmarkKey());
        };
      }
    }
//    else {
////      qWarning("type is Unknown!");
//    };
  };
}


/** Reimplementation. */
const QString CModuleItem::toolTip(){
    QString text;
		text = QString::fromLatin1("<DIV STYLE=\"border-bottom:thin solid black;\">")
      + i18n("Module")
      + QString::fromLatin1(": <B>%1</B></DIV>").arg( module()->name() )
      + ((module()->category() == CSwordModuleInfo::Cult) ? QString::fromLatin1("<B>%1</B><BR>").arg(i18n("Take care, this module contains cult / questionable material!")) : QString::null);
		text += QString::fromLatin1("<DIV STYLE=\"border-bottom:thin solid black;\">") + module()->config(CSwordModuleInfo::Description) + QString::fromLatin1("</DIV>");
		text += i18n("Language")+ QString::fromLatin1(": %1<BR>").arg(module()->language().translatedName());
		if (module()->isEncrypted())
			text += i18n("Unlock key") + QString::fromLatin1(": %1<BR>")
				.arg(!module()->config(CSwordModuleInfo::CipherKey).isEmpty() ? module()->config(CSwordModuleInfo::CipherKey) : QString("<FONT COLOR=\"red\">%1</FONT>").arg(i18n("not set")));
		if (module()->hasVersion())
			text += i18n("Version") + QString::fromLatin1(": %1<BR>").arg(module()->config(CSwordModuleInfo::ModuleVersion));
     	     	
   	QString options;
   	unsigned int opts;
   	for (opts = CSwordBackend::filterTypesMIN; opts <= CSwordBackend::filterTypesMAX; ++opts){
   		if (module()->has( static_cast<CSwordBackend::FilterTypes>(opts) )) {
     		if (!options.isEmpty())
     			options += QString::fromLatin1(", ");
     		options += CSwordBackend::translatedOptionName( static_cast<CSwordBackend::FilterTypes>(opts) );
   		}
   	}
   	if (!options.isEmpty())
   		text += i18n("Options") + QString::fromLatin1(": <font size= \"-1\">") + options + QString::fromLatin1("</font>");
     		
		if (text.right(4) == QString::fromLatin1("<BR>"))
			text = text.left(text.length()-4);
		return text;
}

/** Returns the used module. */
CSwordModuleInfo* const CModuleItem::module() const {
  return m_module;
}

/** Returns the about module info of the used module. */
const QString CModuleItem::aboutInfo(){
	QString text;	

  if (module()->hasVersion())
    text += QString::fromLatin1("<b>%1:</b> %2<br>")
    	.arg(i18n("Version"))
    	.arg(module()->config(CSwordModuleInfo::ModuleVersion));

	text += QString::fromLatin1("<b>%1:</b> %2<br><b>%3:</b> %4<br>")
		.arg(i18n("Location"))
		.arg(module()->config(CSwordModuleInfo::AbsoluteDataPath))
		.arg(i18n("Language"))
		.arg(module()->language().translatedName());

	if (module()->isWritable())
		text += QString::fromLatin1("<b>%1:</b> %2<br>")
							.arg(i18n("Writable"))
							.arg(i18n("yes"));

	if ( module()->isEncrypted() )
		text += QString::fromLatin1("<b>%1:</b> %2<br>")
							.arg(i18n("Unlock key"))
							.arg(module()->config(CSwordModuleInfo::CipherKey));	

	QString options;
	unsigned int opts;
	for (opts = CSwordBackend::filterTypesMIN; opts <= CSwordBackend::filterTypesMAX; ++opts){
		if (module()->has( static_cast<CSwordBackend::FilterTypes>(opts) )){
  		if (!options.isEmpty())
  			options += QString::fromLatin1(", ");
  		options += CSwordBackend::translatedOptionName( static_cast<CSwordBackend::FilterTypes>(opts) );
		}
	}
	if (!options.isEmpty())
		text += QString("<b>%1:</b> %2<br>")
			.arg(i18n("Features"))
			.arg(options);

  if (m_module->category() == CSwordModuleInfo::Cult) { //clearly say the module contains cult/questionable materials
    text += QString::fromLatin1("<BR><B>%1</B><BR><BR>")
              .arg(i18n("Take care, this module contains cult / questionable material!"));
  };

	text += QString::fromLatin1("<b>%1:</b><br> <font size=\"-1\">%2</font>")
						.arg("About")
						.arg(module()->config(CSwordModuleInfo::AboutInformation));
  return text;
}


/** Reimplementation from  CItemBase. */
const bool CModuleItem::enableAction( const MenuAction action ){
  if (action == EditModule) {
    if (!module())
      return false;
    return module()->isWritable();
  }

  if (action == SearchInModules || action == AboutModule)
    return true;
  if (module()->isEncrypted() && action == UnlockModule)
    return true;
  return false;
}

/* ----------------------------------------------*/
/* ---------- new class: CBookmarkItem ------------*/
/* ----------------------------------------------*/

CBookmarkItem::CBookmarkItem(CFolderBase* parentItem, CSwordModuleInfo* module, const QString& key, const QString& description) : CItemBase(parentItem), m_key(key), m_description(description), m_module(module) {
//  qWarning("constructor of CVBookamrkItem");
  if (module && (module->type() == CSwordModuleInfo::Bible || module->type() == CSwordModuleInfo::Commentary)  ) {
    CSwordVerseKey vk(0);
    vk = key;
    vk.setLocale("en");
//    qWarning("key is %s", vk.key().latin1());
    m_key = vk.key(); //the m_key member is always the english key!
  }
  else
   m_key = key;
  m_startupXML = QDomElement();
}

CBookmarkItem::CBookmarkItem(CFolderBase* parentItem, QDomElement& xml ) : CItemBase(parentItem), m_key(QString::null), m_description(QString::null), m_module(0) {
//  qWarning("2nd constructor of CBookmarkItem");
  m_startupXML = xml;
}

CBookmarkItem::~CBookmarkItem() {

}

/** No descriptions */
void CBookmarkItem::update(){
//  CItemBase::update();
  const QString title = QString::fromLatin1("%1 (%2)").arg(key()).arg(module() ? module()->name() : i18n("unknown"));
  setText(0, title);
  setPixmap(0,BOOKMARK_ICON_SMALL);
}

void CBookmarkItem::init(){
//  qWarning("CBookmarkItem::init()");
  if (!m_startupXML.isNull())
    loadFromXML(m_startupXML);

//  CItemBase::init();
  update();
  setDropEnabled(false);
  setDragEnabled(false);

//  if (!module())
//    setSelectable(false);
}

/** Reimplementation. */
const QString CBookmarkItem::toolTip(){
  if (!module())
    return QString::null;

  return CTooltipManager::textForReference(module()->name(), key(), description());
}

/** Returns the used module. */
CSwordModuleInfo* const CBookmarkItem::module() {
//  Q_ASSERT(m_module);
  return m_module;
}

/** Returns the used key. */
const QString CBookmarkItem::key(){
//  qWarning("CBookmarkItem::key()");
  QString keyName = englishKey();
  if (!module())
    return keyName;

  if (module()->type() == CSwordModuleInfo::Bible || module()->type() == CSwordModuleInfo::Commentary) {
    CSwordVerseKey vk(0);
    vk = keyName;
    vk.setLocale(CPointers::backend()->booknameLanguage().latin1());
    keyName = vk.key(); //now we're sure the key is in english! All bookname languages support english!
  }
  return keyName;
}

/** Returns the used description. */
const QString& CBookmarkItem::description(){
   return m_description;
}

/** No descriptions */
const bool CBookmarkItem::isMovable(){
  return true;
}

/** Reimplementation to handle  the menu entries of the main index. */
const bool CBookmarkItem::enableAction(const MenuAction action){
  if (action == ChangeBookmark || action == PrintBookmarks || action == DeleteEntries)
    return true;

  return false;
}

void CBookmarkItem::print(){
  CExportManager mgr(i18n("Print bookmark"),false);
  mgr.printKey(module(), key(), key(), description());
}

/** Changes this bookmark. */
void CBookmarkItem::rename(){
  bool ok  = false;
  QString newDescription = CInputDialog::getText(i18n("Change description ..."),i18n("Enter a new description for the chosen bookmark!"), description(), &ok, listView(), true);
  if (ok) {
    m_description = newDescription;
    update();
  }
}

/** Reimplementation of CItemBase::saveToXML. */
QDomElement CBookmarkItem::saveToXML( QDomDocument& doc ){
//  qWarning("CBookmarkItem::saveToXML( QDomDocument& doc )");
  QDomElement elem = doc.createElement("Bookmark");

  QString keyName = key();
  if (module() && (module()->type() == CSwordModuleInfo::Bible || module()->type() == CSwordModuleInfo::Commentary)) {
    CSwordVerseKey vk(0);
    vk = keyName;
    vk.setLocale("en");
    keyName = vk.key(); //now we're sure the key is in english! All bookname languages support english!
  }
  elem.setAttribute("key", keyName);
  elem.setAttribute("description", description());
  elem.setAttribute("modulename", module() ? module()->name() : QString::null);
  elem.setAttribute("moduledescription", module() ? module()->config(CSwordModuleInfo::Description) : QString::null);

  return elem;
}

void CBookmarkItem::loadFromXML( QDomElement& element ) {
//  qWarning("CBookmarkItem::loadFromXML( QDomElement& element )");
  Q_ASSERT(!element.isNull());
  if (element.isNull())
    return;

  //find the right module
  if (element.hasAttribute("modulename") && element.hasAttribute("moduledescription")) {
    m_module = backend()->findModuleByName(element.attribute("modulename"));
//    Q_ASSERT(m_module);
    if (!m_module/*&& m_module->config(CSwordModuleInfo::Description) != element.attribute("moduledescription")*/) {
      qWarning("Can't find module with name %s and description %s",element.attribute("modulename").latin1(), element.attribute("moduledescription").latin1() );
    }
  }

  if (element.hasAttribute("key")) {
    QString key = element.attribute("key");

    if (module() && (module()->type() == CSwordModuleInfo::Bible || module()->type() == CSwordModuleInfo::Commentary)  ) {
      CSwordVerseKey vk(0);
      vk = key;
      m_key = vk.key(); //now we're sure it's the key in the selected bookname language!
    }
    else
     m_key = key;
  }

  if (element.hasAttribute("description"))
    m_description = element.attribute("description");
}

/** Returns the english key. */
const QString& CBookmarkItem::englishKey(){
  return m_key;
}

/** Reimplementation. Returns false everytime because a bookmarks  has not possible drops. */
bool CBookmarkItem::acceptDrop(const QMimeSource* /*src*/){
  return false;
}

/****************************************/
/*****  class: CItemFolder  *************/
/****************************************/

CFolderBase::CFolderBase(CMainIndex* mainIndex, const Type type) : CItemBase(mainIndex, type) {
}

CFolderBase::CFolderBase(CFolderBase* parentItem, const Type type) : CItemBase(parentItem, type) {
}

CFolderBase::CFolderBase(CFolderBase* parentFolder, const QString& caption) : CItemBase(parentFolder){
  setText(0, caption);
}

CFolderBase::~CFolderBase() {
}

const bool CFolderBase::isFolder() {
  return true;
}

void CFolderBase::update() {
  CItemBase::update();
  if (isOpen() && childCount())
    setPixmap(0,GROUP_OPEN_ICON_SMALL);
  else
    setPixmap(0,GROUP_ICON_SMALL);
}

void CFolderBase::init() {
  CItemBase::init();
  setDropEnabled(false);
  setDragEnabled(false);
}

/** No descriptions */
void CFolderBase::setOpen( bool open ){
  KListViewItem::setOpen(open);
  update();
}

/** The function which renames this folder. */
void CFolderBase::rename(){
  startRename(0);
}

/** Creates a new sub folder of this folder. */
void CFolderBase::newSubFolder(){
  if (dynamic_cast<CBookmarkFolder*>(this) || dynamic_cast<CBookmarkFolder::SubFolder*>(this) ) {
    CBookmarkFolder::SubFolder* f = new CBookmarkFolder::SubFolder(this, i18n("New folder"));
    f->init();

    listView()->setCurrentItem(f);
    listView()->ensureItemVisible(f);
    f->rename();
  }
}

/** Reimplementation. Returns true if the drop is accepted. */
const bool CFolderBase::allowAutoOpen( const QMimeSource* ) const{
  return true;
}

/** Reimplementation. Returns false because folders have no use for drops (except for the bookmark folders) */
bool CFolderBase::acceptDrop(const QMimeSource* /*src*/){
  return false;
}

/****************************************/
/*****  class: CTreeFolder  *************/
/****************************************/


CTreeFolder::CTreeFolder(CMainIndex* mainIndex, const Type type, const QString& language) : CFolderBase(mainIndex, type) {
  m_language = language;
}

CTreeFolder::CTreeFolder(CFolderBase* item, const Type type, const QString& language) : CFolderBase(item, type) {
  m_language = language;
}

CTreeFolder::~CTreeFolder(){
}

void CTreeFolder::addGroup(const Type type, const QString language){
  CTreeFolder* i = 0;
  if (type == BookmarkFolder)
    i = new CBookmarkFolder(this);
  else if (type == OldBookmarkFolder)
    i = new COldBookmarkFolder(this);
  else
    i = new CTreeFolder(this, type, language);
  i->init();
  if (!i->childCount())
    delete i;
}

void CTreeFolder::addModule(CSwordModuleInfo* const module){
  CModuleItem* i = new CModuleItem(this, module);
  i->init();
}

void CTreeFolder::addBookmark(CSwordModuleInfo* module, const QString& key, const QString& description){
  CBookmarkItem* i = new CBookmarkItem(this, module, key, description);
  i->init();
}

void CTreeFolder::update(){
  CFolderBase::update();
}

void CTreeFolder::init(){
  if (language() == "*") {
    switch (type()) {
      case BibleModuleFolder:
        setText(0,i18n("Bibles"));
        break;
      case CommentaryModuleFolder:
        setText(0,i18n("Commentaries"));
        break;
      case LexiconModuleFolder:
        setText(0,i18n("Lexicon"));
        break;
      case BookModuleFolder:
        setText(0,i18n("Books"));
        break;
      case DevotionalModuleFolder:
        setText(0,i18n("Daily devotionals"));
        break;
      case GlossaryModuleFolder:
        setText(0,i18n("Glossaries"));
        break;
      case BookmarkFolder:
        setText(0,i18n("Bookmark"));
        break;
      case OldBookmarkFolder:
        setText(0,i18n("Old bookmarks"));
        break;
      default:
        setText(0, i18n("Unknown"));
        break;
    };
  }
  else {
    CLanguageMgr::Language lang = languageMgr()->languageForAbbrev( language() );
    
    setText(0, !language().isEmpty() ? ( lang.isValid() ? lang.translatedName() : language()) : i18n("unknown language"));
  }
  initTree();
  update();
}

void CTreeFolder::initTree(){
//  qWarning("CTreeMgr::initTree");
  if (type() == Unknown)
    return;

  CSwordModuleInfo::ModuleType moduleType = CSwordModuleInfo::Unknown;
  if (type() == BibleModuleFolder)
    moduleType = CSwordModuleInfo::Bible;
  else if (type() == CommentaryModuleFolder)
    moduleType = CSwordModuleInfo::Commentary;
  else if (type() == LexiconModuleFolder || type() == GlossaryModuleFolder || type() == DevotionalModuleFolder)
    moduleType = CSwordModuleInfo::Lexicon;
  else if (type() == BookModuleFolder)
    moduleType = CSwordModuleInfo::GenericBook;

  //get all modules by using the given type
  ListCSwordModuleInfo allModules = backend()->moduleList();
  ListCSwordModuleInfo usedModules;
  for (CSwordModuleInfo* m = allModules.first(); m; m = allModules.next()) {
    if (m->type() == moduleType) { //found a module, check if the type is correct (devotional etc.)
      if (type() == GlossaryModuleFolder && !m->category() == CSwordModuleInfo::Glossary) { //not a gglossary
        continue;
      }
      if (type() == DevotionalModuleFolder && (m->category() != CSwordModuleInfo::DailyDevotional)) {//not a devotional
        continue;
      }
      if (type() == LexiconModuleFolder && ( (m->category() == CSwordModuleInfo::DailyDevotional) || (m->category() == CSwordModuleInfo::Glossary) )) {
        //while looking for lexicons glossaries and devotionals shouldn't be used
        continue;
      }

      if (language() == QString::fromLatin1("*") || (language() != QString::fromLatin1("*") && QString::fromLatin1(m->module()->Lang()) == language()) )//right type and language!
        usedModules.append(m);
    }
  }

  //we have now all modules we want to have
  if (language() == QString::fromLatin1("*")) { //create subfolders for each language
    QStringList usedLangs;
    for (CSwordModuleInfo* m = usedModules.first(); m; m = usedModules.next()) {
      QString lang = QString::fromLatin1(m->module()->Lang());
//      if (lang.isEmpty())
//        lang = ");
      if (!usedLangs.contains(lang)) {
        usedLangs.append(lang);
      }
    }
    QStringList::iterator it;
    for (it = usedLangs.begin(); it != usedLangs.end(); ++it) {
      addGroup(/**it,*/ type(), *it);
    }
  }
  else if (usedModules.count() > 0){ //create subitems with the given type and language
    for (CSwordModuleInfo* m = usedModules.first(); m; m = usedModules.next()) {
      addModule(m);
    }
  }

  sortChildItems(0,true);
}

const QString& CTreeFolder::language() const {
  return m_language;
};

/* ----------------------------------------------*/
/* ---------- new class: COldBookmarkImportItem -*/
/* ----------------------------------------------*/

COldBookmarkFolder::COldBookmarkFolder(CTreeFolder* folder) : CBookmarkFolder(folder, OldBookmarkFolder) {
}

COldBookmarkFolder::~COldBookmarkFolder() {
//  qWarning("descructor of COldBookmarkFolder");
}

/** Reimplementation to handle special bookmark tree. */
void COldBookmarkFolder::initTree(){
  //import the bookmarks of the previous BibleTime versions
  if (!CBTConfig::get( CBTConfig::readOldBookmarks )) { //if we havn't yet loaded the old bookmarks
    OldBookmarkImport import;
    loadBookmarksFromXML( import.oldBookmarksXML() );
  }
}


QDomElement COldBookmarkFolder::saveToXML( QDomDocument& doc ) {
  QDomElement elem = doc.createElement("Folder");
  elem.setAttribute("caption", text(0));

  //append the XML nodes of all child items
  CItemBase* i = dynamic_cast<CItemBase*>(firstChild());
  while( i ) {
    if (i->parent() == this) {
      QDomElement newElem = i->saveToXML( doc );
      if (!newElem.isNull()) {
        elem.appendChild( newElem ); //append to this folder
      }
    }
    i = dynamic_cast<CItemBase*>( i->nextSibling() );
  }

  // save to config, that we imported the old bookmarks and that we have them on disk
  CBTConfig::set( CBTConfig::readOldBookmarks, true );

  return elem;
}

void COldBookmarkFolder::loadFromXML( QDomElement& /*element*/ ) {
  //this function is empty because the folder imports the old 1.2 bookmarks from the bt-groupmanager config file
}

/* --------------------------------------------------*/
/* ---------- new class: CBookmarkFolder::SubFolder--*/
/* --------------------------------------------------*/

CBookmarkFolder::SubFolder::SubFolder(CFolderBase* parentItem, const QString& caption) : CFolderBase(parentItem, caption) {
  m_startupXML = QDomElement();
}

CBookmarkFolder::SubFolder::SubFolder(CFolderBase* parentItem, QDomElement& xml ) : CFolderBase(parentItem, QString::null) {
  m_startupXML = xml;
}

CBookmarkFolder::SubFolder::~SubFolder() {

}

void CBookmarkFolder::SubFolder::init() {
  CFolderBase::init();
  if (!m_startupXML.isNull())
    loadFromXML(m_startupXML);

  setDropEnabled(true);
  setRenameEnabled(0,true);
}

/** Is called when an item was dropped on this subfolder. */
void CBookmarkFolder::SubFolder::dropped(QDropEvent * e) {
  if (acceptDrop(e)) {
    CDragDropMgr::ItemList dndItems = CDragDropMgr::decode(e);
    //until we implemented the rest in CDragDropMgr we copy the items!
    CDragDropMgr::ItemList::Iterator it;
    for( it = dndItems.begin(); it != dndItems.end(); ++it) {
      CSwordModuleInfo* module = backend()->findModuleByName( (*it).bookmarkModule() );
      CBookmarkItem* i = new CBookmarkItem(this, module, (*it).bookmarkKey(), (*it).bookmarkDescription());
      i->init();
    };
  };

//  QString str;
//  QCString submime;
//
//  if (acceptDrop(e) && QTextDrag::decode(e,str,submime=REFERENCE) ) { //a drag object, we can handle
//    QString mod;
//    QString key;
//    CReferenceManager::decodeReference(str,mod,key);
//
//    CSwordModuleInfo* module = backend()->findModuleByName(mod);
//    CBookmarkItem* i = new CBookmarkItem(this, module, key, QString::null);
//    i->init();
//  }
//  else if (acceptDrop(e) && QTextDrag::decode(e,str,submime=BOOKMARK) ) { //a drag object, we can handle
//    qWarning("move boommark items!");
//  }
}

bool CBookmarkFolder::SubFolder::acceptDrop(const QMimeSource * src) const {
//  qWarning("CBookmarkFolder::SubFolder::acceptDrop(const QMimeSource * src)");
  return CDragDropMgr::canDecode(src) && (CDragDropMgr::dndType(src) == CDragDropMgr::Item::Bookmark);
}

/** Reimplementation from  CItemBase. */
const bool CBookmarkFolder::SubFolder::enableAction(const MenuAction action){
  if (action == ChangeFolder || action == NewFolder || action == DeleteEntries || action == ImportBookmarks )
    return true;

  if (action == ExportBookmarks || action == ImportBookmarks )
    return false; //not yet implemented
    
    
//  if (childCount() > 0 && action == ExportBookmarks)
//    return true;

  return false;
}

/** Returns the XML code which represents the content of this folder. */
QDomElement CBookmarkFolder::SubFolder::saveToXML( QDomDocument& doc ) {
  /**
  * Save all subitems (bookmarks and folders) to the XML file.
  * We get the XML code for the items by calling their own saveToXML implementations.
  */
//  qWarning("CBookmarkFolder::SubFolder::saveToXML( QDomDocument& doc )");

  QDomElement elem = doc.createElement("Folder");
  elem.setAttribute("caption", text(0));

  //append the XML nodes of all child items
  CItemBase* i = dynamic_cast<CItemBase*>(firstChild());
  while( i ) {
    if (i->parent() == this) {
      QDomElement newElem = i->saveToXML( doc );
      if (!newElem.isNull()) {
        elem.appendChild( newElem ); //append to this folder
      }
    }
    i = dynamic_cast<CItemBase*>( i->nextSibling() );
  }
//  qWarning("finished");
  return elem;
}

/** Loads the content of this folder from the XML code passed as argument to this function. */
void CBookmarkFolder::SubFolder::loadFromXML( QDomElement& elem ) {
  //get the caption and restore all child items!
  if (elem.hasAttribute("caption"))
    setText(0, elem.attribute("caption"));

  //restore all child items
  QDomElement child = elem.firstChild().toElement();
  CItemBase* oldItem = 0;
  while ( !child.isNull() && child.parentNode() == elem ) {
    CItemBase* i = 0;
    if (child.tagName() == "Folder") {
      i = new CBookmarkFolder::SubFolder(this, child);
    }
    else if (child.tagName() == "Bookmark") {
      i = new CBookmarkItem(this, child);
    }
    i->init();
    if (oldItem)
      i->moveAfter(oldItem);
    oldItem = i;

    child = child.nextSibling().toElement();
  }
}


/* --------------------------------------------------------------------------*/
/* ---------- new class: CBookmarkFolder::OldBookmarkImport -----------------*/
/* --------------------------------------------------------------------------*/


const QString CBookmarkFolder::OldBookmarkImport::oldBookmarksXML( const QString& configFileName ) {
  QString fileName = (configFileName.isEmpty()) ? "bt-groupmanager" : configFileName;
  KConfig* config = new KSimpleConfig( fileName );

  KConfigGroupSaver groupSaver(config, configFileName.isEmpty() ? "Groupmanager" : "Bookmarks");

  QDomDocument doc("DOC");
  doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );

  QDomElement content = doc.createElement("SwordBookmarks");
  content.setAttribute("syntaxVersion", CURRENT_SYNTAX_VERSION);
  doc.appendChild(content);

  //first create the bookmark groups in the XML document, then add the bookmarks to each parent
  QMap<int, QDomElement> parentMap; //maps parent ids to dom elements


	QStringList	groupList = config->readListEntry("Groups");
	QValueList<int>	parentList = config->readIntListEntry("Group parents");

  QStringList::Iterator it_groups = groupList.begin();
	QValueList<int>::Iterator it_parents = parentList.begin();

  int parentIDCounter = 0;
	while ( (it_groups != groupList.end()) && (it_parents != parentList.end()) ) {
    QDomElement parentElement = (*it_parents == -1) ? content : parentMap[*it_parents];
    if (parentElement.isNull()) {
      qWarning("EMPTY PARENT FOUND!");
      parentElement = content;
    };

    QDomElement elem = doc.createElement("Folder");
    elem.setAttribute("caption", (*it_groups));
    parentMap.insert(parentIDCounter, elem);

    parentElement.appendChild( elem );


		++it_parents;
  	++it_groups;
    ++parentIDCounter;
  }

  //groups are now read in, create now the bookmarks
	parentList 	= config->readIntListEntry("Bookmark parents");
  QStringList	bookmarkList 							= config->readListEntry("Bookmarks");
	QStringList	bookmarkModulesList 			= config->readListEntry("Bookmark modules");
	QStringList	bookmarkDescriptionsList 	= config->readListEntry("Bookmark descriptions");

	it_parents  = parentList.begin();
	QStringList::Iterator it_bookmarks 	  = bookmarkList.begin();
	QStringList::Iterator it_modules 		  = bookmarkModulesList.begin();
	QStringList::Iterator it_descriptions	= bookmarkDescriptionsList.begin();

	while ( it_bookmarks != bookmarkList.end()
          && it_parents != parentList.end()
			 		&& it_modules != bookmarkModulesList.end()
        )
  {
    QDomElement parentElement = ((*it_parents) == -1) ? content : parentMap[(*it_parents)];
    if (parentElement.isNull()) {
      qWarning("EMPTY PARENT FOUND!");
      parentElement = content;
    };
    QDomElement elem = doc.createElement("Bookmark");

    elem.setAttribute("key", *it_bookmarks);
    elem.setAttribute("description", *it_descriptions);
    elem.setAttribute("modulename", *it_modules);

    CSwordModuleInfo* m = CPointers::backend()->findModuleByName( *it_modules );
    elem.setAttribute("moduledescription", m ? m->config(CSwordModuleInfo::Description) : QString::null);

    parentElement.appendChild( elem );


		++it_parents;
		++it_modules;
		++it_descriptions;
		++it_bookmarks;
  };

//  qWarning(doc.toString().latin1());

  return doc.toString();
};
/* --------------------------------------------------*/
/* ---------- new class: CBookmarkFolder ------------*/
/* --------------------------------------------------*/

CBookmarkFolder::CBookmarkFolder(CMainIndex* mainIndex, const Type type) : CTreeFolder(mainIndex, type, "*") {
}

CBookmarkFolder::CBookmarkFolder(CFolderBase* parentItem, const Type type) : CTreeFolder(parentItem, type, "*") {
}

CBookmarkFolder::~CBookmarkFolder() {
}

void CBookmarkFolder::initTree(){
  
  addGroup(OldBookmarkFolder, "*");

  KStandardDirs stdDirs;
 	const QString path = stdDirs.saveLocation("data", "bibletime/");	
  if (!path.isEmpty()) {
    loadBookmarks(path + "bookmarks.xml");
  }
}

/** Reimplementation. */
const bool CBookmarkFolder::enableAction(const MenuAction action){
  if (action == NewFolder || action == ImportBookmarks)
    return true;

  if (action == ExportBookmarks && childCount())
    return true;

  return false;
}


void CBookmarkFolder::exportBookmarks(){
  QString fileName = KFileDialog::getSaveFileName(QString::null, i18n("*.btb | BibleTime bookmark files (*.btb)\n*.* | All files (*.*)"), 0, i18n("BibleTime - Export bookmarks"));
  if (!fileName.isEmpty()) {
    saveBookmarks( fileName );
  };
}


void CBookmarkFolder::importBookmarks(){
  QString fileName = KFileDialog::getOpenFileName(QString::null, i18n("*.btb | BibleTime bookmark files (*.btb)\n*.* | All files (*.*)"), 0, i18n("BibleTime - Import bookmarks"));
  if (!fileName.isEmpty()) {
    //we have to decide if we should load an old bookmark file from 1.2 or earlier or the new XML format of > 1.3    
    if ( !loadBookmarks(fileName) ) { //if this fialed try to load it as old bookmark file
      qWarning("use OldBookmarkImport");
      OldBookmarkImport import;
      loadBookmarksFromXML( import.oldBookmarksXML( fileName ) );
    };
  };
}

bool CBookmarkFolder::acceptDrop(const QMimeSource * src) const {
//  qWarning("CBookmarkFolder::acceptDrop(const QMimeSource * src)");
  return (CDragDropMgr::canDecode(src) && (CDragDropMgr::dndType(src) == CDragDropMgr::Item::Bookmark));
}

void CBookmarkFolder::dropped(QDropEvent *e) {
//  qWarning("CBookmarkFolder::dropped(QDropEvent *e)");
  if (acceptDrop(e)) {
    CDragDropMgr::ItemList dndItems = CDragDropMgr::decode(e);
    //until we implemented the rest in CDragDropMgr we copy the items!
    CDragDropMgr::ItemList::Iterator it;
    for( it = dndItems.begin(); it != dndItems.end(); ++it) {
      CSwordModuleInfo* module = backend()->findModuleByName( (*it).bookmarkModule() );
      CBookmarkItem* i = new CBookmarkItem(this, module, (*it).bookmarkKey(), (*it).bookmarkDescription());
      i->init();
    };
  };
}

/** Saves the bookmarks in a file. */
const bool CBookmarkFolder::saveBookmarks( const QString& filename ){
//  qWarning("CBookmarkFolder::saveBookmarks( const QString& filename )");

	bool ret = false;
  QDomDocument doc("DOC");
  doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );

  QDomElement content = doc.createElement("SwordBookmarks");
  content.setAttribute("syntaxVersion", CURRENT_SYNTAX_VERSION);
  doc.appendChild(content);

  //append the XML nodes of all child items
  CItemBase* i = dynamic_cast<CItemBase*>( firstChild() );
  while( i ) {
    if (i->parent() == this) { //only one level under this
      QDomElement newElem = i->saveToXML( doc );
      if (!newElem.isNull()) {
        content.appendChild( newElem ); //append to this folder
      }
    }
    i = dynamic_cast<CItemBase*>( i->nextSibling() );
  }
	
  QFile file(filename);
	if ( file.open(IO_WriteOnly) ) {
		ret = true;
		QTextStream t( &file );
		t.setEncoding(QTextStream::UnicodeUTF8);
		t << doc.toString();
		file.close();
	}
	else
		ret = false;
	return ret;
}

const bool CBookmarkFolder::loadBookmarksFromXML( const QString& xml ){
	QDomDocument doc;
	doc.setContent(xml);
  QDomElement document = doc.documentElement();
  if( document.tagName() != "SwordBookmarks" ) {
    qWarning("Not a BibleTime Bookmark XML file");
		return false;
	}

  CItemBase* oldItem = 0;
  //restore all child items
  QDomElement child = document.firstChild().toElement();
  while ( !child.isNull() && child.parentNode() == document) {
    CItemBase* i = 0;
    if (child.tagName() == "Folder") {
      i = new CBookmarkFolder::SubFolder(this, child);
    }
    else if (child.tagName() == "Bookmark") {
      i = new CBookmarkItem(this, child);
    }
    Q_ASSERT(i);
    if (!i) {
      break;
    }

    i->init();
    if (oldItem)
      i->moveAfter(oldItem);
    oldItem = i;

    if (!child.nextSibling().isNull())
      child = child.nextSibling().toElement();
    else
      break;
  }
  return true;
}

/** Loads bookmarks from a file. */
const bool CBookmarkFolder::loadBookmarks( const QString& filename ){
//	qWarning("load bookmarks");
  QFile file(filename);	
	if (!file.exists())
		return false;

  QString xml;	
	if (file.open(IO_ReadOnly)) {		
		QTextStream t( &file );
		t.setEncoding(QTextStream::UnicodeUTF8);
    xml = t.read();
		file.close();	
	}

  return loadBookmarksFromXML( xml );
}

/* NEW CLASS */

CGlossaryFolder::CGlossaryFolder(CMainIndex* mainIndex, const Type type, const QString& fromLanguage, const QString& toLanguage)
  : CTreeFolder(mainIndex, type, fromLanguage)
{
  m_fromLanguage = fromLanguage;
  m_toLanguage = toLanguage;  
}

CGlossaryFolder::CGlossaryFolder(CFolderBase* item, const Type type, const QString& fromLanguage, const QString& toLanguage)
  : CTreeFolder(item, type, fromLanguage)
{
  m_fromLanguage = fromLanguage;
  m_toLanguage = toLanguage;
}

CGlossaryFolder::~CGlossaryFolder(){
}

void CGlossaryFolder::initTree(){
//  qWarning("CGlossaryFolder::initTree()");
  if (type() == Unknown)
    return;

  //get all modules by using the lexicon type
  ListCSwordModuleInfo allModules = backend()->moduleList();
  ListCSwordModuleInfo usedModules;
  for (CSwordModuleInfo* m = allModules.first(); m; m = allModules.next()) {
    if (m->type() == CSwordModuleInfo::Lexicon) { //found a module, check if the type is correct (devotional etc.)
//      qWarning("found lexicon");
      if ((type() == GlossaryModuleFolder) && (m->category() != CSwordModuleInfo::Glossary)) { //not a glossary
//        qWarning("found lexicon: not a glossary");
        continue;
      }
//      qWarning("FOUND a glossary!");
      //found a glossary
      if (language() == QString::fromLatin1("*")
          || (language() != QString::fromLatin1("*")
               && m->config(CSwordModuleInfo::GlossaryFrom) == fromLanguage()
               && m->config(CSwordModuleInfo::GlossaryTo) == toLanguage()
             )
        )
      { //right type and language!
//        qWarning("append module %s", m->module()->Name());
        usedModules.append(m);
      }
    }
  }

  //we have now all modules we want to have
  if (language() == QString::fromLatin1("*")) { //create subfolders for each language
    typedef std::pair<QString, QString> LanguagePair;
    typedef QValueList<LanguagePair> LanguagePairList;
    
    LanguagePairList usedLangs;
    for (CSwordModuleInfo* m = usedModules.first(); m; m = usedModules.next()) {
      LanguagePair langPair( m->config(CSwordModuleInfo::GlossaryFrom), m->config(CSwordModuleInfo::GlossaryTo) );
      if (!usedLangs.contains(langPair)) {
        usedLangs.append(langPair);
      }
    }
    LanguagePairList::iterator it;
    for (it = usedLangs.begin(); it != usedLangs.end(); ++it) {
      addGroup(type(), (*it).first, (*it).second);
    }
  }
  else if (usedModules.count() > 0){ //create subitems with the given type and languages
    for (CSwordModuleInfo* m = usedModules.first(); m; m = usedModules.next()) {
      addModule(m);
    }
  }

  sortChildItems(0,true);  
}

void CGlossaryFolder::init(){
  if (language() == "*") {
    setText(0,i18n("Glossaries"));
  }
  else {
    CLanguageMgr::Language fromLang = languageMgr()->languageForAbbrev( m_fromLanguage );    
    CLanguageMgr::Language toLang = languageMgr()->languageForAbbrev( m_toLanguage );

    QString fromLangString  = fromLang.translatedName();
    QString toLangString    = toLang.translatedName();
        
    if (fromLangString.isEmpty()) { //use abbrev!
      fromLangString = m_fromLanguage;
    };
    if (toLangString.isEmpty()) { //use abbrev!
      toLangString = m_toLanguage;
    };
    
        
    setText(0, fromLangString + " - " + toLangString );
  }
  initTree();
  update();
}

/** Returns the language this glossary folder maps from. */
const QString& CGlossaryFolder::fromLanguage() const{
  return m_fromLanguage;
}

/** Returns the language this glossary folder maps to. */
const QString& CGlossaryFolder::toLanguage() const{
  return m_toLanguage;
}

void CGlossaryFolder::addGroup(const Type type, const QString& fromLanguage, const QString& toLanguage) {
  CTreeFolder* i = new CGlossaryFolder(this, type, fromLanguage, toLanguage);
  i->init();
  if (!i->childCount())
    delete i;
}
