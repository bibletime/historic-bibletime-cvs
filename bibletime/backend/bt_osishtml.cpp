/***************************************************************************
                     thmlhtml.cpp  -  ThML to HTML filter
                             -------------------
    begin                : 1999-10-27
    copyright            : 2001 by CrossWire Bible Society
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//BibleTime includes
#include <stdlib.h>
#include "bt_osishtml.h"
#include "versekey.h"

#include "clanguagemgr.h"
#include "creferencemanager.h"
#include "cswordmoduleinfo.h"
#include "frontend/cbtconfig.h"
#include "util/cpointers.h"

#include <iostream>

//Sword includes
#include <swmodule.h>
#include <swbuf.h>
#include <utilxml.h>

//Qt includes
#include <qstring.h>

using sword::SWBuf;
using sword::XMLTag;

BT_OSISHTML::BT_OSISHTML() {
	setTokenStart("<");
	setTokenEnd(">");

	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);
	setPassThruUnknownEscapeString(true); //the HTML widget will render the HTML escape codes

	setTokenCaseSensitive(true);
}

bool BT_OSISHTML::handleToken(sword::SWBuf &buf, const char *token, sword::SWFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
	if (!substituteToken(buf, token)) {
    BT_UserData* myUserData = dynamic_cast<BT_UserData*>(userData);

    XMLTag tag(token);
  	const	bool osisQToTick = ((!userData->module->getConfigEntry("OSISqToTick")) || (strcmp(userData->module->getConfigEntry("OSISqToTick"), "false")));

    if (!tag.getName()) {
      return false;
    }
    
		// <w> tag
		if (!strcmp(tag.getName(), "w")) {

			// start <w> tag
			if ((!tag.isEmpty()) && (!tag.isEndTag())) {
				myUserData->w = token;
			}
			// end or empty <w> tag
			else {
				bool endTag = tag.isEndTag();
				SWBuf lastText;
				bool show = true;	// to handle unplaced article in kjv2003-- temporary till combined

				if (endTag) {
					tag = myUserData->w.c_str();
					lastText = myUserData->lastTextNode.c_str();
				}
				else lastText = "stuff";

				const char *attrib;
				const char *val;
				if ((attrib = tag.getAttribute("xlit"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" %s", val);
				}
				if ((attrib = tag.getAttribute("gloss"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" %s", val);
				}
				if ((attrib = tag.getAttribute("lemma"))) {
					const int count = tag.getAttributePartCount("lemma");
					int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
					do {
						attrib = tag.getAttribute("lemma", i);
						if (i < 0) // to handle our -1 condition
              i = 0;
						val = strchr(attrib, ':');
						val = (val) ? (val + 1) : attrib;

            if ((!strcmp(val+2, "3588")) && (lastText.length() < 1)) {
							show = false;
            }
						else if (*val == 'H') {
              buf.appendFormatted(" <a href=\"strongs://Hebrew/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span></a> ", val+1, val+1);
            }
						else if (*val == 'G') {
              buf.appendFormatted(" <a href=\"strongs://Greek/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span></a> ", val+1, val+1);
            }
					} while (++i < count);
				}
				if ((attrib = tag.getAttribute("morph")) && (show)) {
					const int count = tag.getAttributePartCount("morph");
					int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
					do {
						attrib = tag.getAttribute("morph", i);
						if (i < 0)
              i = 0;	// to handle our -1 condition
						val = strchr(attrib, ':');
						val = (val) ? (val + 1) : attrib;
 						if ((*val == 'T') && (val[1] == 'H')) {
              buf.appendFormatted(" <a href=\"morph://Hebrew/%s\"><span class=\"morphcode\">(%s)</span></a> ", val+2, val+2);
            }
						else if ((*val == 'T') && (val[1] == 'G')) {
              buf.appendFormatted(" <a href=\"morph://Greek/%s\"><span class=\"morphcode\">(%s)</span></a> ", val+2, val+2);
            }
            else if ((*val == 'T')) {
              buf.appendFormatted(" <a href=\"morph://Greek/%s\"><span class=\"morphcode\">(%s)</span></a> ", val+2, val+2);
            }
					} while (++i < count);
				}
				if ((attrib = tag.getAttribute("POS"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" %s", val);
				}
			}
		}
		// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) {
//				SWBuf footnoteNum = myUserData["fn"];
				SWBuf type = tag.getAttribute("type");

				if (type != "strongsMarkup") {	// leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
          buf += "<span class=\"footnote\">(";
          myUserData->insertedFootnoteTags = true;

//					int footnoteNumber = (footnoteNum.length()) ? atoi(footnoteNum.c_str()) : 1;
					// see if we have a VerseKey * or descendant
					//if (sword::VerseKey *vkey = dynamic_cast<sword::VerseKey*>(key)) {
//						char ch = ((tag.getAttribute("type") && ((!strcmp(tag.getAttribute("type"), "crossReference")) || (!strcmp(tag.getAttribute("type"), "x-cross-ref")))) ? 'x':'n');
						    //buf.appendFormatted(" ", vkey->getText(), ch, footnoteNumber, ch);
//						SWBuf tmp;
//						tmp.appendFormatted("%i", ++footnoteNumber);
//						myUserData["fn"] = tmp.c_str();
//					}
				}
        else {
  				myUserData->suspendTextPassThru = true;
        }
			}
			if (tag.isEndTag()) {
        if (myUserData->insertedFootnoteTags) {
          buf += ")</span>";
          myUserData->insertedFootnoteTags = false;
        }
				myUserData->suspendTextPassThru = false;
			}
		}
		// <p> paragraph tag is handled by OSISHTMLHref
		// <reference> tag
		else if (!strcmp(tag.getName(), "reference")) {
			if (!tag.isEndTag() && !tag.isEmpty() && tag.getAttribute("osisRef")) {
        char* ref = tag.getAttribute("osisRef");

        SWBuf typeName = "Bible";        
        CSwordModuleInfo::ModuleType type = CSwordModuleInfo::Bible;
        if (!strncmp(ref, "Bible:", 6)) {
          type = CSwordModuleInfo::Bible;
          typeName = "Bible";
          ref += 6;
        }        
        else if (!strncmp(ref, "Commentary:", 11)) { //need to check with OSIS tags
          type = CSwordModuleInfo::Commentary;
          typeName = "Commentary";
          ref += 11;
        }
        
				buf.appendFormatted("<a href=\"sword://%s/%s/%s\">",
          typeName.c_str(),
          CReferenceManager::preferredModule( CReferenceManager::typeFromModule(type) ).latin1(),
          ref
        );
			}
			else if (tag.isEndTag()) {
				buf += "</a>";
			}
			else {	// empty reference marker
				// -- what should we do?  nothing for now.
			}
		}
    // <l> is handled by OSISHTMLHref
		// <title>
		else if (!strcmp(tag.getName(), "title")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
  			buf += "<div class=\"sectiontitle\">";
			}
			else if (tag.isEndTag()) {
  			buf += "</div>";
			}
			else {	// empty title marker
				// what to do?  is this even valid?
				buf += "<br />";
			}
		}
		// <hi> is handled by OSISHTMLHRef
		// <q> quote
		else if (!strcmp(tag.getName(), "q")) {
			SWBuf type = tag.getAttribute("type");
			SWBuf who = tag.getAttribute("who");
			const char *lev = tag.getAttribute("level");
			int level = (lev) ? atoi(lev) : 1;

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				/*buf += "{";*/

				//alternate " and '
				if(osisQToTick)
					buf += (level % 2) ? '\"' : '\'';

				if (who == "Jesus") {
					buf += "<span class=\"jesuswords\">";
				}
			}
			else if (tag.isEndTag()) {
				//alternate " and '
        if (who == "Jesus")
          buf += "</span>";
        else if (osisQToTick)
					buf += (level % 2) ? '\"' : '\'';
			}
			else {	// empty quote marker
				//alternate " and '
				if(osisQToTick)
					buf += (level % 2) ? '\"' : '\'';
			}
		}
		// <transChange> is handled by OSISHTMLHref
    else { //all tokens handled by OSISHTMLHref will run through the filter now
      return sword::OSISHTMLHref::handleToken(buf, token, userData);
    }
	}
  return false;
}
