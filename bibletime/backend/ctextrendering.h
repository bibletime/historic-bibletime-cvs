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
		KeyTree const * childList() const {
			return m_childList;
		};
		Settings settings() const {
			return m_settings;
		};
	
	private:
		ListCSwordModuleInfo m_moduleList;
		QString m_key;
		KeyTree* m_childList;
		Settings m_settings;
	};
	
	class KeyTree : public KeyTreeItemList {
	public:
		ListCSwordModuleInfo collectModules();
	};

  CTextRendering();
  virtual ~CTextRendering();

	const QString renderKeyTree( KeyTree& );
	const QString renderSingleKey( const QString& key, ListCSwordModuleInfo );
	
protected:
	virtual const QString renderEntry( const KeyTreeItem& ) = 0;
	virtual const QString finishText( const QString&, KeyTree& tree ) = 0;
};


class CDisplayRendering : public CTextRendering {
public:
	CDisplayRendering(CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults(), CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults());
	virtual ~CDisplayRendering();
	
protected:	
	virtual const QString renderEntry( const KeyTreeItem& );
	virtual const QString finishText( const QString&, KeyTree& tree );
	virtual const QString entryLink( const KeyTreeItem& item, CSwordModuleInfo* module );

	CSwordBackend::DisplayOptions m_displayOptions;
	CSwordBackend::FilterOptions m_filterOptions;
};

#endif
