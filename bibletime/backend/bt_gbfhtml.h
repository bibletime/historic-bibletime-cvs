/********* Read the file LICENSE for license details. *********/

/* $Header$ */
/* $Revision$ */

#ifndef BT_GBFHTML_H
#define BT_GBFHTML_H

//Backend
#include "cfiltertool.h"

//Sword includes
#include <swkey.h>
#include <swmodule.h>
#include <swbasicfilter.h>
#include <gbfhtml.h>

namespace Filters {

/** GBF to HTML filter,
* This filter converts GBF Text into HTML
*/
class BT_GBFHTML : public sword::GBFHTML/*, protected CFilterTool */{
protected:
  class BT_UserData : public sword::GBFHTML::MyUserData {
  public:
    BT_UserData(const sword::SWModule *module, const sword::SWKey *key) : sword::GBFHTML::MyUserData(module, key) {
			swordFootnote = 1;
			hasFootnotePreTag = false;
		}
		
		short unsigned int swordFootnote;
  };
  virtual sword::BasicFilterUserData *createUserData(const sword::SWModule* module, const sword::SWKey* key) {
    return new BT_UserData(module, key);
  }

public:
  BT_GBFHTML();
  virtual bool handleToken(sword::SWBuf &buf, const char *token, sword::BasicFilterUserData *userData);
  virtual char processText(sword::SWBuf& buf, const sword::SWKey*, const sword::SWModule * = 0);
};

}

#endif
