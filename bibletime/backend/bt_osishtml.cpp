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
#include "frontend/cbtconfig.h"
#include "util/cpointers.h"

#include <iostream>

//Sword includes
#include <swmodule.h>
#include <utilxml.h>

//Qt includes
#include <qstring.h>

using sword::SWBuf;

BT_OSISHTML::BT_OSISHTML() {
	setTokenStart("<");
	setTokenEnd(">");

	setEscapeStart("&");
	setEscapeEnd(";");

	setEscapeStringCaseSensitive(true);
	setPassThruUnknownEscapeString(true); //the HTML widget will render the HTML escape codes

	setTokenCaseSensitive(true);
}

bool BT_OSISHTML::handleToken(sword::SWBuf& buf, const char *token, DualStringMap &userData) {
  // manually process if it wasn't a simple substitution
	if (!substituteToken(buf, token)) {
		sword::XMLTag tag(token);
		const bool osisQToTick = ((!module->getConfigEntry("OSISqToTick")) || (strcmp(module->getConfigEntry("OSISqToTick"), "false")));


    //<w> tag
    if (!strcmp(tag.getName(), "w")) {

			// start <w> tag
			if ((!tag.isEmpty()) && (!tag.isEndTag())) {
//				buf += "{";
				userData["w"] = token;
			}

			// end or empty <w> tag
			else {
				bool endTag = tag.isEndTag();
				SWBuf lastText;
				bool show = true;	// to handle unplaced article in kjv2003-- temporary till combined

				if (endTag) {
					tag = userData["w"].c_str();
					lastText = userData["lastTextNode"].c_str();
				}
				else lastText = "stuff";

				const char *attrib;
				const char *val;
				if ((attrib = tag.getAttribute("xlit"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" &lt;%s&gt;", val);
				}
				if ((attrib = tag.getAttribute("gloss"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" &lt;%s&gt;", val);
				}
				if ((attrib = tag.getAttribute("lemma"))) {
					int count = tag.getAttributePartCount("lemma");
					int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
					do {
						attrib = tag.getAttribute("lemma", i);
						if (i < 0) i = 0;	// to handle our -1 condition
						val = strchr(attrib, ':');
						val = (val) ? (val + 1) : attrib;
						if ((strchr("GH", *val)) && (isdigit(val[1])))
							val++;
						if ((!strcmp(val, "3588")) && (lastText.length() < 1))
							show = false;
						else	buf.appendFormatted(" <a href=\"strongs:/Hebrew/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span></a>", val, val);
					} while (++i < count);
				}
				if ((attrib = tag.getAttribute("morph")) && (show)) {
					SWBuf savelemma = tag.getAttribute("savlm");
					if ((strstr(savelemma.c_str(), "3588")) && (lastText.length() < 1))
						show = false;
					if (show) {
						int count = tag.getAttributePartCount("morph");
						int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
						do {
							attrib = tag.getAttribute("morph", i);
							if (i < 0) i = 0;	// to handle our -1 condition
							val = strchr(attrib, ':');
							val = (val) ? (val + 1) : attrib;
							if ((*val == 'T') && (strchr("GH", val[1])) && (isdigit(val[2])))
								val+=2;
							buf.appendFormatted(" (%s)", val);
						} while (++i < count);
					}
				}
				if ((attrib = tag.getAttribute("POS"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					buf.appendFormatted(" %lt;%s&gt;", val);
				}

				if (endTag)
					buf += "";
			}
    }
    // <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEmpty() && !tag.isEndTag()) {
				SWBuf type = tag.getAttribute("type");

				if (type != "strongsMarkup") {	// leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
					SWBuf footnoteNumber = tag.getAttribute("swordFootnote");

          // see if we have a VerseKey * or descendant
//					if (sword::VerseKey *vkey = dynamic_cast<sword::VerseKey*>(key)) {
//						buf.appendFormatted("<a href=\"\">*%c%i.%s</a> ", ((tag.getAttribute("type") && ((!strcmp(tag.getAttribute("type"), "crossReference")) || (!strcmp(tag.getAttribute("type"), "x-cross-ref")))) ? 'x':'n'), vkey->Verse(), footnoteNumber.c_str());
//					}
				}
				userData["suspendTextPassThru"] = "true";
			}
			if (tag.isEndTag()) {
				userData["suspendTextPassThru"] = "false";
			}
		}


	}
  return false;
}
