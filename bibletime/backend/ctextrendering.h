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
#include <qvaluelist.h>

class CSwordModuleInfo;

/**
This class is responsible for text rendering.
 
@author The BibleTime team
*/
class CTextRendering {
public:
	class KeyTreeItem;
	class KeyTree;
	typedef QValueList<KeyTreeItem> KeyTreeItemList;
	
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
		virtual inline const QString& key() const {
			return m_key;
		};
		inline const Settings& settings() const {
			return m_settings;
		};
		
		inline KeyTree* const childList() const;

//	private:
	protected:
		Settings m_settings;
		ListCSwordModuleInfo m_moduleList;
		QString m_key;
		mutable KeyTree* m_childList;
	};
	
	class KeyTree : public KeyTreeItemList {
	public:
		ListCSwordModuleInfo collectModules();
	};

  CTextRendering();
  virtual ~CTextRendering();

	virtual const QString renderKeyTree( KeyTree& );
	virtual const QString renderKeyRange( const QString& start, const QString& stop, ListCSwordModuleInfo );
	virtual const QString renderSingleKey( const QString& key, ListCSwordModuleInfo );
	
protected:
	virtual const QString renderEntry( const KeyTreeItem& ) = 0;
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
	virtual const QString renderEntry( const KeyTreeItem& );
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
	}
		
	return m_childList;
};

#endif
