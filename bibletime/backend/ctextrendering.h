//
// C++ Interface: ctextrendering
//
// Description:
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

#ifndef CTEXTRENDERING_H
#define CTEXTRENDERING_H

//BT includes
#include "backend/cswordbackend.h"

#include "frontend/cbtconfig.h"

//QT includes
#include <qstring.h>
#include <qptrlist.h>

class CSwordModuleInfo;
class CSwordKey;

/**
This class is responsible for text rendering.
 
@author The BibleTime team
*/
class CTextRendering {
public:
	class KeyTreeItem;
	class KeyTree;
	typedef QPtrList<KeyTreeItem> KeyTreeItemList;
	
	class KeyTreeItem {
	public:
		struct Settings {
			Settings() : highlight(false) {};
			
			bool highlight;
		};
		
		KeyTreeItem();
		KeyTreeItem(const KeyTreeItem& i);
		KeyTreeItem(const QString& key, CSwordModuleInfo const * module, const Settings settings);
		KeyTreeItem(const QString& key, const ListCSwordModuleInfo& modules, const Settings settings);
		virtual ~KeyTreeItem();
		
		inline const ListCSwordModuleInfo& modules() const {
			return m_moduleList;
		};
		inline const QString& key() const {
			return m_key;
		};
		inline const Settings& settings() const {
			return m_settings;
		};
		
		inline KeyTree* const childList() const;
		inline const bool hasChildItems() const;

	protected:
		Settings m_settings;
		ListCSwordModuleInfo m_moduleList;
		QString m_key;
		mutable KeyTree* m_childList;
	};
	
	class KeyTree : public KeyTreeItemList {
	public:
		KeyTree() {
			setAutoDelete(true);
		};
		ListCSwordModuleInfo collectModules();
	};

  CTextRendering();
  virtual ~CTextRendering();

	const QString renderKeyTree( KeyTree& );
	const QString renderKeyRange( const QString& start, const QString& stop, ListCSwordModuleInfo );
	const QString renderSingleKey( const QString& key, ListCSwordModuleInfo );
	
protected:
	virtual const QString renderEntry( const KeyTreeItem&, CSwordKey* = 0 ) = 0;
	virtual const QString finishText( const QString&, KeyTree& tree ) = 0;
	virtual void initRendering() = 0;
};


class CHTMLExportRendering : public CTextRendering {
public:
	struct Settings {
		Settings(const bool text = true) {
			addText = text;
		};
		
		bool addText;
	};
	
	CHTMLExportRendering(
		const Settings& settings, 
		CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults(), 
		CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults()
	);
	virtual ~CHTMLExportRendering();
	
protected:	
	virtual const QString renderEntry( const KeyTreeItem&, CSwordKey* = 0 );
	virtual const QString finishText( const QString&, KeyTree& tree );
	virtual const QString entryLink( const KeyTreeItem& item, CSwordModuleInfo* module );
	virtual void initRendering();
	
	CSwordBackend::DisplayOptions m_displayOptions;
	CSwordBackend::FilterOptions m_filterOptions;
	Settings m_settings;
};

class CPlainTextExportRendering : public CHTMLExportRendering {
public:
	CPlainTextExportRendering(
		const Settings& settings, 
		CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults(), 
		CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults()
	);
	virtual ~CPlainTextExportRendering();
	
protected:	
	virtual const QString renderEntry( const KeyTreeItem& );
	virtual const QString finishText( const QString&, KeyTree& tree );
};

class CDisplayRendering : public CHTMLExportRendering {
public:
	static const QString keyToHTMLAnchor(const QString& key);
	
	CDisplayRendering(
		CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults(), 
		CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults()
	);

protected:	
	virtual const QString entryLink( const KeyTreeItem& item, CSwordModuleInfo* const module );
	virtual const QString finishText( const QString&, KeyTree& tree );
};

inline CTextRendering::KeyTree* const CTextRendering::KeyTreeItem::childList() const {
	if (!m_childList) {
		m_childList = new KeyTree();
		//m_childList->setAutoDelete(true);
	}
		
	return m_childList;
};

inline const bool CTextRendering::KeyTreeItem::hasChildItems() const {
	if (!m_childList)
		return false;
		
	return (m_childList->count() > 0);
}

#endif
