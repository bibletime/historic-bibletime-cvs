/***************************************************************************
                          chtmlchapterdisplay.h  -  description
                             -------------------
    begin                : Thu Oct 12 2000
    copyright            : (C) 2000 by The BibleTime team
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

#ifndef CHTMLCHAPTERDISPLAY_H
#define CHTMLCHAPTERDISPLAY_H

#include "chtmlentrydisplay.h"

/**
	* The display which renders a chapter
  * @author The BibleTime team
  * @version $Id$
  */
class CHTMLChapterDisplay : public CHTMLEntryDisplay  {
public: 
  /**
 	* Renders the current chapter of the module using HTML tags.
 	* The finished HTML text could be retrieved using the function
	* #getHTML.
 	* This is a reimplementation from CHTMLEntryDisplay
 	*/
  virtual char Display( CSwordModuleInfo* module );
  /**
  * Generates code to display the given modules side by side.
  */
  virtual char Display( QList<CSwordModuleInfo>* moduleList);

protected:
	void updateSettings(void);
  /**
 	* If this variable is true the chapters
 	* displayed in BibleTime will have word wraps.
 	*/
  bool m_useLineBreak;
	bool m_useVerseNumbers;
};

#endif
