/***************************************************************************
                          gbfhtml.h  -  description
                             -------------------
    begin                : Tue Jun 15 15:34:26 CEST 1999

    copyright            : (C) 1999 by Torsten Uhlmann
    email                : TUhlmann@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* $Header$ */
/* $Revision$ */

#ifndef BT_GBFHTML_H
#define BT_GBFHTML_H

#include "cfiltertool.h"

#include <swkey.h>
#include <swmodule.h>
#include <swbasicfilter.h>
#include <gbfhtml.h>

/**
* This filter converts GBF Text into HTML
*/
class BT_GBFHTML : public sword::GBFHTML, protected CFilterTool {
protected:
  class BT_UserData : public sword::GBFHTML::MyUserData {
  public:
    BT_UserData(const sword::SWModule *module, const sword::SWKey *key) : sword::GBFHTML::MyUserData(module, key) {}
    bool hasFootnotePreTag;
  };
  virtual sword::BasicFilterUserData *createUserData(const sword::SWModule* module, const sword::SWKey* key) {
    return new BT_UserData(module, key);
  }

public:
  BT_GBFHTML ();
  virtual bool handleToken(sword::SWBuf &buf, const char *token, sword::BasicFilterUserData *userData);
  virtual char processText(sword::SWBuf& buf, const sword::SWKey*, const sword::SWModule * = 0);
};

#endif
