/***************************************************************************
                          bt_basicfilter.h  -  description
                             -------------------
    begin                : Thu Nov 1 2001
    copyright            : (C) 2001 by The BibleTime team
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

#ifndef BT_BASICFILTER_H
#define BT_BASICFILTER_H

#include <swbasicfilter.h>

class SWKey;
class SWModule;

/**
  * This is the base class for all filters in BT
  */

class BT_BASICFILTER : public SWBasicFilter  {
public:
  virtual char ProcessText (char *text, int maxlen, const SWKey *, const SWModule * = 0);
  /**
  * Parses the verse reference ref and returns it.
  */
  const char* parseRef(const char* ref);

protected:
	/**
	* updates the filter specific settings like colors
	* the variables are used in the inherited filters
	* in HandleToken()
	*/
	void updateSettings(void);
  /**
	* This filter converts the RWP #Gen 1:1|
	* style bible references to HTML
	*/
  char ProcessRWPRefs(char * text, int maxlen = -1);

	char footnote_color[8];
	char strongs_color[8];
	char morph_color[8];
	char jesuswords_color[8];
	char swordref_color[8];
	char text_color[8];
	char standard_bible[100];

	const SWKey *m_key;
	const SWModule *m_module;
};

#endif
