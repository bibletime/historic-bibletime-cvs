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

#include <swbuf.h>
#include <swbasicfilter.h>

#include <string>

using std::string;

class sword::SWKey;
class sword::SWModule;

/**
  * This is the base class for all filters in BT
  */

class BT_BASICFILTER : virtual public sword::SWBasicFilter  {
public:
  virtual char processText(sword::SWBuf& buf, const sword::SWKey*, const sword::SWModule * = 0);

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
  char ProcessRWPRefs(sword::SWBuf& buf);
  /**
  * Parses the verse reference ref and returns it.
  */
  const sword::SWBuf parseSimpleRef(const sword::SWBuf& ref);
  /**
  * Parses the verse reference ref and returns it.
  */
  const sword::SWBuf parseRef(const sword::SWBuf ref, sword::SWModule*, const bool insertFullRef = true);
  /**
  * Parses the verse reference ref and returns it.
  */
  const sword::SWBuf parseThMLRef(const sword::SWBuf& ref, const char* mod = 0);
	const sword::SWBuf thmlRefEnd();
  /**
  * Replaces the token in the substitute map.
  */
  void replaceTokenSubstitute(const char* findString, const char* replaceString);
  /**
  * Updates the token for this filter if settings changed.
  */
  virtual void updateTokens();

	char standard_bible[200];

	sword::SWKey *m_key;
	sword::SWModule* m_module;
};

#endif
