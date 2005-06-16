/********* Read the file LICENSE for license details. *********/

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
	CTextRendering::KeyTreeItem::Settings settings(
		false,
		CTextRendering::KeyTreeItem::Settings::CompleteShort
	);
	
	return render.renderSingleKey(keyName, modules, settings);
}
