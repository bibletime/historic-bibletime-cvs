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

class sword::SWKey;
class sword::SWModule;

/**
  * This is the base class for all filters in BT
  */

class BT_BASICFILTER : public sword::SWBasicFilter  {
public:
  virtual char ProcessText (char *text, int maxlen, const sword::SWKey *, const sword::SWModule * = 0);

protected:
	/**
	* updates the filter specific settings like colors
	* the variables are used in the inherited filters
	* in HandleToken()
	*/
	void updateSettings();
  /**
	* This filter converts the RWP #Gen 1:1|
	* style bible references to HTML
	*/
  char ProcessRWPRefs(char* text, int maxlen = -1);
  /**
  * Parses the verse reference ref and returns it.
  */
  const char* parseSimpleRef(const char* ref);
  /**
  * Parses the verse reference ref and returns it.
  */
  const char* parseThMLRef(const char* ref, const char* mod = 0);
	const char* thmlRefEnd();
  /**
  * Replaces the token in the substitute map.
  */
  void replaceTokenSubstitute(const char* findString, const char* replaceString);
  /**
  * Updates the token for this filter if settings changed.
  */
  virtual void updateTokens();
	
	char standard_bible[100];

	const sword::SWKey *m_key;
	const sword::SWModule* m_module;
};

#endif
