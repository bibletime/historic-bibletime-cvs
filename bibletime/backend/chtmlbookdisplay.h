/***************************************************************************
                          chtmlbookdisplay.h  -  description
                             -------------------
    begin                : Mon Mar 11 2002
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

#ifndef CHTMLBOOKDISPLAY_H
#define CHTMLBOOKDISPLAY_H

#include "chtmlentrydisplay.h"
#include "cswordtreekey.h"

class CSwordBookModuleInfo;

/**
	* The display class to display
	* the general book modules.
  * @author The BibleTime team
  */
class CHTMLBookDisplay : public CHTMLEntryDisplay  {
public: 
  /**
  * Renders the current entry of the module using HTML tags.
  * The rendered text is cached so it's faster if it's called for the
  * same key next time.
  */
  virtual char Display(CSwordModuleInfo* module);

private:
  void printTree(CSwordTreeKey treeKey, CSwordBookModuleInfo* module);
};

#endif
