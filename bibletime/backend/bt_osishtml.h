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

#ifndef BT_OSISHTML_H
#define BT_OSISHTML_H

#include "cfiltertool.h"

#include <osishtmlhref.h>

/**
* This filter converts ThML text to HTML text
*/
class BT_OSISHTML : public sword::OSISHTMLHref, protected CFilterTool {
protected:
  class BT_UserData : public sword::OSISHTMLHref::MyUserData {
  public:
    BT_UserData(const sword::SWModule *module, const sword::SWKey *key) : sword::OSISHTMLHref::MyUserData(module, key) {}

    bool insertedFootnoteTags;
  };

  virtual sword::SWBasicFilter::UserData *createUserData(const sword::SWModule* module, const sword::SWKey* key) {
    return new BT_UserData(module, key);
  }

public:
  BT_OSISHTML();
  virtual bool handleToken(sword::SWBuf &buf, const char *token, sword::SWBasicFilter::UserData *userData);
};

#endif
