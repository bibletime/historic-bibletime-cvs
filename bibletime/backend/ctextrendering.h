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
#include "backend/cswordmoduleinfo.h"
#include "backend/cswordbackend.h"

#include "frontend/cbtconfig.h"

//QT includes
#include <qstring.h>
#include <qvaluelist.h>

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
			Settings() {
				highlight = false;
			};
			bool highlight;
		};
		
		KeyTreeItem();
		KeyTreeItem(const QString& key, CSwordModuleInfo const * module, const Settings settings);
		KeyTreeItem(const QString& key, ListCSwordModuleInfo& modules, const Settings settings);
		~KeyTreeItem();
		
		const ListCSwordModuleInfo& modules() const {
			return m_moduleList;
		};
		const QString& key() const {
			return m_key;
		};
		Settings settings() const {
			return m_settings;
		};
		KeyTree* childList() const;

	
	private:
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

	const QString renderKeyTree( KeyTree& );
	const QString renderKeyRange( const QString& start, const QString& stop, ListCSwordModuleInfo );
	const QString renderSingleKey( const QString& key, ListCSwordModuleInfo );
	
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
	virtual const QString entryLink( const KeyTreeItem& item, CSwordModuleInfo* module );
	virtual const QString finishText( const QString&, KeyTree& tree );
};

#endif
