/***************************************************************************
                          centrydisplay.h  -  description
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

/**The reimplementation of SWDisplay to for our needs.
  *@author The BibleTime team
  */	
class CEntryDisplay : public sword::SWDisplay, public CPointers  {
public: // Public methods
  /**
  * Returns the rendered text using the modules in the list and using the key parameter.
  *  The displayoptions and filter options are used, too.
  */
  virtual const QString text( const ListCSwordModuleInfo& modules, const QString& key, const CSwordBackend::DisplayOptions displayOptions, const CSwordBackend::FilterOptions filterOptions);
};


}

#endif
