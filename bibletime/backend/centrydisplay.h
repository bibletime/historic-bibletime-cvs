/********* Read the file LICENSE for license details. *********/

#ifndef CENTRYDISPLAY_H
#define CENTRYDISPLAY_H

//BibleTime includes
#include "ctextrendering.h"
#include "cswordmoduleinfo.h"
#include "cswordbackend.h"

#include "util/cpointers.h"

//Sword includes
#include <swdisp.h>

//Qt includes
#include <qstring.h>

class CSwordModuleInfo;

namespace Rendering {

	/**
	 * The reimplementation of SWDisplay to fit our needs.
	 * @short Display implementation
	 * @author The BibleTime team
	 */

class CEntryDisplay : public sword::SWDisplay, public CPointers {

public:
		virtual ~CEntryDisplay() {}

		;

		/**
		* Returns the rendered text using the modules in the list and using the key parameter.
		* The displayoptions and filter options are used, too.
		*/
		virtual const QString text( const ListCSwordModuleInfo& modules, const QString& key, const CSwordBackend::DisplayOptions displayOptions, const CSwordBackend::FilterOptions filterOptions);
	};


}

#endif
