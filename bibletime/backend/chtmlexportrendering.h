//
// C++ Interface: chtmlexportrendering
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RENDERINGCHTMLEXPORTRENDERING_H
#define RENDERINGCHTMLEXPORTRENDERING_H

#include "ctextrendering.h"

namespace Rendering {

/**
@author The BibleTime team
*/

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

};

#endif
