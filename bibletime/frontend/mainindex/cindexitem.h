/***************************************************************************
                          cindexitem.h  -  description
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

#ifndef CINDEXITEM_H
#define CINDEXITEM_H

//BibleTime includes
#include "frontend/cpointers.h"

#include "backend/cswordmoduleinfo.h"

//KDE includes
#include <klistview.h>

class KConfig;
class CFolderBase;
class CTreeFolder;
class CMainIndex;

class CItemBase : public KListViewItem, public CPointers {
public:
  enum Type {
    Unknown = 0,
    BookmarkFolder,
    Bookmark,
    OldBookmarkFolder, /* Bookmarks in the old 1.x <= 1.2 format*/
    BibleModuleFolder,
    CommentaryModuleFolder,
    LexiconModuleFolder,
    BookModuleFolder,
    DevotionalModuleFolder,
    GlossaryModuleFolder,
    ModuleLanguageFolder,
    Module
  };
  enum MenuAction {
    NewFolder = 0,
    ChangeFolder,

    ChangeBookmark,
    ImportBookmarks,
    ExportBookmarks,
    PrintBookmarks,

    DeleteEntries,

    SearchInModules,
    UnlockModule,
    AboutModule,

    ActionBegin = NewFolder,
    ActionEnd = AboutModule
  };

  CItemBase(CMainIndex* mainIndex, const Type type = Unknown);
  CItemBase(CItemBase* item, const Type type = Unknown);

  virtual ~CItemBase();
  virtual const QString toolTip();
  virtual CMainIndex* listView() const;
  /**
  * Returns if the implementation of this class is a folder item or not.
  * Reimplement this function to return the correct value.
  */
  virtual const bool isFolder() { return false; };
  const Type& type() const;

  virtual void init();
  virtual void update();
  void moveAfter( CItemBase* const item );
  /**
  * Returns true if the given action should be enabled in the popup menu.
  */
  virtual const bool enableAction( const MenuAction action );
  virtual const bool isMovable();

protected:
  friend class CMainIndex;
  virtual void dropped( QDropEvent* e );

private:
  Type m_type;
};

class CModuleItem : public CItemBase {
public:
	CModuleItem(CTreeFolder* item, CSwordModuleInfo* module);
	virtual ~CModuleItem();
  virtual CSwordModuleInfo* const module() const;
  virtual const QString toolTip();

  /** Reimplementation from  CItemBase. */
  virtual const bool enableAction( const MenuAction action );
  virtual void update();
  virtual void init();
  /**
  * Returns the about module info of the used module.
  */
  const QString aboutInfo();

protected: // Protected methods
  /**
  * Reimplementation to handle text drops on a module.
  * In this case open the searchdialog. In the case of a referebnce open the module at the given position.
  */
  virtual bool acceptDrop( const QMimeSource* src ) const;
  virtual void dropped( QDropEvent* e );

private:
  CSwordModuleInfo* m_module;
};

class CBookmarkItem : public CItemBase {
public:
	CBookmarkItem(CFolderBase* parentItem, CSwordModuleInfo* module, const QString& key, const QString& description);
	~CBookmarkItem();
  CSwordModuleInfo* const module();
  const QString& key();
  const QString& description();
  virtual const QString toolTip();

  virtual void update();
  virtual void init();
  virtual const bool isMovable();
  /**
  * Reimplementation to handle  the menu entries of the main index.
  */
  virtual const bool enableAction(const MenuAction action);
  /** Prints this bookmark. */
  void print();
  /** Changes this bookmark. */
  void rename();

private:
  QString m_key;
  QString m_description;
  CSwordModuleInfo* m_module;
};


class CFolderBase : public CItemBase {
public:
  CFolderBase(CMainIndex* mainIndex, const Type type);
  CFolderBase(CFolderBase* parentFolder, const Type type);
  CFolderBase(CFolderBase* parentFolder, const QString& caption);
  virtual ~CFolderBase();

  virtual const bool isFolder();

  virtual void update();
  virtual void init();
  virtual void setOpen( bool open );
  /**
  * The function which renames this folder.
  */
  void rename();
  virtual void newSubFolder();
};

/** The base class for all items in the tree. Subclasses for module folders, modules and bookmarks exist.
  * @author The BibleTime team
  */
class CTreeFolder : public CFolderBase {
public: 
	CTreeFolder(CMainIndex* mainIndex, const Type type, const QString& language );
	CTreeFolder(CFolderBase* parentFolder, const Type type, const QString& language );
	~CTreeFolder();
  virtual void addGroup(const Type type, const QString language);
  virtual void addModule(CSwordModuleInfo* const);
  virtual void addBookmark(CSwordModuleInfo* module, const QString& key, const QString& description);

  virtual void initTree();

  virtual void update();
  virtual void init();

  const QString language() const;

private:
  QString m_language;
};

class CBookmarkFolder : public CTreeFolder {
public:
  class SubFolder : public CFolderBase {
  public:
    SubFolder(CFolderBase* parentItem, const QString& caption);
    virtual ~SubFolder();
    virtual void init();
    /**
    * Reimplementation to handle bookmark drops on this bookmark sub folder.
    * In this case open the searchdialog. In the case of a referebnce open the module at the given position.
    */
    virtual bool acceptDrop(const QMimeSource * src) const;
    /**
    * Is called when an item was dropped on this subfolder.
    */
    virtual void dropped(QDropEvent * e);
    /**
    * Reimplementation from  CItemBase.
    */
    const bool enableAction(const MenuAction action);
  };

	CBookmarkFolder(CMainIndex* mainIndex, const Type type = BookmarkFolder);
	CBookmarkFolder(CFolderBase* parentItem, const Type type = BookmarkFolder);
	virtual ~CBookmarkFolder();
  virtual const bool enableAction(const MenuAction action);
  void exportBookmarks();
  void importBookmarks();
  bool acceptDrop(const QMimeSource * src) const;
  void dropped(QDropEvent * e);

protected: // Protected methods
  virtual void initTree();
};

class COldBookmarkFolder : public CBookmarkFolder {
public:
	COldBookmarkFolder(CTreeFolder* item);
	virtual ~COldBookmarkFolder();
  virtual void initTree();

private:
  CFolderBase* findParent( const int ID );
  const bool readGroups(KConfig* configFile );
  const int parentId(CItemBase *item);
};


#endif