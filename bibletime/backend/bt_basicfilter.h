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


class sword::SWKey;
class sword::SWModule;

using std::string;

/**
  * This is the base class for all filters in BT
  */

class BT_BASICFILTER : public sword::SWBasicFilter  {
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
  const string parseSimpleRef(const string& ref);
  /**
  * Parses the verse reference ref and returns it.
  */
  const string parseRef(const string ref, sword::SWModule*, const bool insertCompleteRef = true);
  /**
  * Parses the verse reference ref and returns it.
  */
  const string parseThMLRef(const string& ref, const char* mod = 0);
	const string thmlRefEnd();
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
