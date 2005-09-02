/********* Read the file LICENSE for license details. *********/

#ifndef BT_THMLHTML_H
#define BT_THMLHTML_H

#include "cfiltertool.h"

#include <swbuf.h>
#include <thmlhtml.h>

using sword::SWBuf;

namespace Filters {

	/** ThML to HTML filter.
	* This filter converts ThML text to HTML text
	*/

class BT_ThMLHTML : public sword::ThMLHTML {

protected:

class BT_UserData : public sword::ThMLHTML::MyUserData {

public:
BT_UserData(const sword::SWModule *module, const sword::SWKey *key) : sword::ThMLHTML::MyUserData(module, key) {
				inscriptRef = false;
				swordFootnote = 1;
				inFootnoteTag = false;
			}

			bool inscriptRef;
			bool inFootnoteTag;
			unsigned short int swordFootnote;
		};

		virtual sword::BasicFilterUserData *createUserData(const sword::SWModule* module, const sword::SWKey* key) {
			return new BT_UserData(module, key);
		}

public:
		BT_ThMLHTML ();
		virtual bool handleToken(sword::SWBuf &buf, const char *token, sword::BasicFilterUserData *userData);
		virtual char processText(sword::SWBuf& buf, const sword::SWKey*, const sword::SWModule * = 0);
	};

}

#endif
