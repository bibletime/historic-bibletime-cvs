/***************************************************************************
                          ctooltipmanager.h  -  description
                             -------------------
    begin                : Sam Mai 11 2002
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

#ifndef CTOOLTIPMANAGER_H
#define CTOOLTIPMANAGER_H

//BibleTime includes
#include "backend/cswordmoduleinfo.h"
#include "util/cpointers.h"

//Qt includes
#include <qstring.h>

/**Manages the tooltip texts for hyperlinks ad refereences.
  *@author The BibleTime team
  */

class CTooltipManager : public CPointers {
public: 
  /**
  * Returns the tooltip text for the given hyperlink.
  */
  static const QString textForHyperlink( const QString& link );
	static const QString textForReference( const QString& moduleName, const QString& keyName, const QString& description);

protected: // Protected methods
  /**
  * Returns the text for the tooltip beginning.
  */
  static const QString headingText(CSwordModuleInfo* const module, const QString& keyName );
  /**
  * Returns the text for the given moduleName and key name.
  */
  static const QString moduleText( const QString& moduleName, const QString& keyName);
  /**
  * Returns the CSS data used for the tooltips. The difference to the display CSS data is
  * that the backgroundcolors etc. are not used.
  */
  static const QString tooltipCSS(CSwordModuleInfo* module);

private:
	CTooltipManager();
	~CTooltipManager();
};

#endif
