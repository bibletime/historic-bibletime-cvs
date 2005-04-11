/********* Read the file LICENSE for license details. *********/

#ifndef BT_OSISHTML_H
#define BT_OSISHTML_H

#include "cfiltertool.h"

//Sword includes
#include <osishtmlhref.h>

/**
* This filter converts ThML text to HTML text
*/
namespace Filters {

class BT_OSISHTML : public sword::OSISHTMLHREF/*, protected CFilterTool*/ {
protected:
  class BT_UserData : public sword::OSISHTMLHREF::MyUserData {
  public:
    BT_UserData(const sword::SWModule *module, const sword::SWKey *key) : sword::OSISHTMLHREF::MyUserData(module, key) {
      noteType = Unknown;
			swordFootnote = 1;
			inCrossrefNote = false;
    }
		
		unsigned short int swordFootnote;
		bool inCrossrefNote;

    enum NoteType {
      Unknown,
			Alternative,
      CrossReference,
      Footnote,
      StrongsMarkup
    } noteType;

		struct {
			sword::SWBuf who;
		} quote;
  };

  virtual sword::BasicFilterUserData *createUserData(const sword::SWModule* module, const sword::SWKey* key) {
    return new BT_UserData(module, key);
  }

public:
  BT_OSISHTML();
  virtual bool handleToken(sword::SWBuf &buf, const char *token, sword::BasicFilterUserData *userData);
};

} //end of Filters namespace

#endif
