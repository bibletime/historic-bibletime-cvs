/***************************************************************************
                          thmlhtml.h  -  description
                             -------------------
    begin                : 1999-10-28

    copyright            : 1999 by Chris Little
    email                : chrislit@chiasma.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef BT_THMLHTML_H
#define BT_THMLHTML_H

#include "cfiltertool.h"

#include <swbuf.h>
#include <thmlhtml.h>

using sword::SWBuf;
//using sword::

/**
* This filter converts ThML text to HTML text
*/
class BT_ThMLHTML : public sword::ThMLHTML, protected CFilterTool {
protected:
  class BT_UserData : public sword::ThMLHTML::MyUserData {
  public:
    BT_UserData(const sword::SWModule *module, const sword::SWKey *key) : sword::ThMLHTML::MyUserData(module, key) {}
    
    bool inscriptRef;
  };
  
  virtual sword::SWBasicFilter::UserData *createUserData(const sword::SWModule* module, const sword::SWKey* key) {
    return new BT_UserData(module, key);
  }

public:
  BT_ThMLHTML ();
  virtual bool handleToken(sword::SWBuf &buf, const char *token, sword::SWBasicFilter::UserData *userData);
//  virtual char ProcessText (char *text, int maxlen = -1);
};

#endif
