/***************************************************************************
                          cfiltertool.h  -  description
                             -------------------
    begin                : Don Jul 10 2003
    copyright            : (C) 2003 by The BibleTime team
    email                : info@bibletime.info
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CFILTERTOOL_H
#define CFILTERTOOL_H

#include "util/cpointers.h"

#include "swmodule.h"
#include "swkey.h"
#include "swbuf.h"

/**CFilterTool includes some functions to use filters from Sword without multiple virtual inheritance.
  *@author The BibleTime team
  */

class CFilterTool : public CPointers  {
public: 
	CFilterTool();
	virtual ~CFilterTool();

  sword::SWModule* const module(sword::SWModule* const = 0);
  sword::SWKey* const key(sword::SWKey* const = 0);

protected: // Protected methods
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
  char ProcessRWPRefs(sword :: SWBuf & buf);
  /** 
  * parses the verse reference ref and returns it.
  */
  const sword::SWBuf thmlRefEnd();
  /**
  * Parses the verse reference ref and returns it.
  */
  const sword::SWBuf parseRef(const sword :: SWBuf ref, sword :: SWModule * , const bool insertFullRef = true);

private:
  QString m_standardBible;
  sword::SWModule* m_module;
  sword::SWKey* m_key;  
};

#endif
