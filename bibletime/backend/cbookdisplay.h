//
// C++ Interface: cbookdisplay
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef RENDERINGCBOOKDISPLAY_H
#define RENDERINGCBOOKDISPLAY_H

#include "centrydisplay.h"

class CTextRendering {
	class KeyTree;
};
class CSwordTreeKey;

namespace Rendering {

/**
@author The BibleTime team
*/
class CBookDisplay : public CEntryDisplay  {
public: // Public methods
  /**
  * Returns the rendered text using the modules in the list and using the key parameter.
  *  The displayoptions and filter options are used, too.
  */
  virtual const QString text( const ListCSwordModuleInfo& modules, const QString& key, const CSwordBackend::DisplayOptions displayOptions, const CSwordBackend::FilterOptions filterOptions);

protected:
	void setupRenderTree(CSwordTreeKey* swordTree, CTextRendering::KeyTree* renderTree, const QString& highlightKey);
};

};

#endif
