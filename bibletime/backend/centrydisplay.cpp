/***************************************************************************
                          centrydisplay.cpp  -  description
                             -------------------
    begin                : Fre Mai 31 2002
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
#include "centrydisplay.h"

#include "cswordkey.h"
#include "cswordversekey.h"
#include "cswordbookmoduleinfo.h"
#include "creferencemanager.h"
#include "cdisplaytemplatemgr.h"
#include "cdisplayrendering.h"

#include "frontend/cbtconfig.h"

#include "util/scoped_resource.h"

//Qt includes
#include <qapplication.h>
#include <qregexp.h>

using namespace Rendering;

/** Returns the rendered text using the modules in the list and using the key parameter. 
 * The displayoptions and filter options are used, too. 
 */
const QString CEntryDisplay::text( const ListCSwordModuleInfo& modules, const QString& keyName, const CSwordBackend::DisplayOptions displayOptions, const CSwordBackend::FilterOptions filterOptions ) {
	CDisplayRendering render(displayOptions, filterOptions);
	
	//no highlighted key and no extra key link in the text
	CTextRendering::KeyTreeItem::Settings settings(false, CTextRendering::KeyTreeItem::Settings::NoKey);
	
	return render.renderSingleKey(keyName, modules, settings);
}
