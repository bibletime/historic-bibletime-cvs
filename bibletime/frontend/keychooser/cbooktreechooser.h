/***************************************************************************
                          cbooktreechooser.h  -  description
                             -------------------
    begin                : Sat Jan 26 2002
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

#ifndef CBOOKTREECHOOSER_H
#define CBOOKTREECHOOSER_H


/** The treechooser implementation for books.
  * @author The BibleTime team
  */
//BibleTime includes
#include "ckeychooser.h"
#include "ckeychooserwidget.h"

//Sword includes

//Qt includes
#include <qwidget.h>
#include <qsize.h>
#include <qmap.h>
#include <qlist.h>
#include <qstringlist.h>

//KDE includes
#include <klistview.h>

class CSwordKey;
class CSwordBookModuleInfo;
class CSwordTreeKey;

class TreeKeyIdx;

/** The keychooser implementeation for books.
  * @author The BibleTime team
  */
class CBookTreeChooser : public CKeyChooser  {
   Q_OBJECT
public:
	CBookTreeChooser(CSwordModuleInfo *module=0, CSwordKey *key=0, QWidget *parent=0, const char *name=0);
	~CBookTreeChooser();
  /**
  * Refreshes the content.
  */
  virtual void refreshContent();
  /**
  * Sets another module to this keychooser
  */
  virtual void setModule(CSwordModuleInfo*);
  /**
  * Returns the key of this kechooser.
  */
  virtual CSwordKey* const key();
  /**
  * Sets a new key to this keychooser
  */
  virtual void setKey(CSwordKey*);
  void setKey(CSwordKey*, const bool emitSinal);

private:
	class TreeItem : public KListViewItem {
		public:
			TreeItem(QListViewItem* parent, QListViewItem* after, const QString caption, const QString key);
			TreeItem(QListViewItem* parent, const QString caption, const QString key);			
			TreeItem(QListView* view,QListViewItem* after, const QString caption, const QString key);						
			const QString& key() const;
		private:
			QString m_key;
	};

	CSwordBookModuleInfo	*m_module;
	CSwordTreeKey *m_key;
	KListView* m_treeView;

protected: // Protected methods
  /** Set up the tree with the current level of key. */
  void setupTree( QListViewItem* parent,QListViewItem* after, CSwordTreeKey* key );

protected slots: // Protected slots
  /** No descriptions */
  void itemClicked( QListViewItem* item );

public slots: // Public slots
  /** No descriptions */
  virtual void updateKey( CSwordKey* );
};

#endif
