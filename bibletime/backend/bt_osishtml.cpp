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

//KDE includes
#include <klocale.h>

//Qt includes
#include <qstring.h>

using sword::SWBuf;
using sword::XMLTag;

BT_OSISHTML::BT_OSISHTML() {
	setTokenStart("<");
	setTokenEnd(">");
	
	addTokenSubstitute("inscription", "<span class=\"inscription\">");
	addTokenSubstitute("/mentioned", "</span>");
	
	addTokenSubstitute("mentioned", "<span class=\"mentioned\">");
	addTokenSubstitute("/mentioned", "</span>");
	
	addTokenSubstitute("devineName", "<span class=\"name\"><span class=\"devine\">");
	addTokenSubstitute("/devineName", "</span></span>");
//	addTokenSubstitute("lg", "<br />");
//	addTokenSubstitute("/lg", "<br />");
	
	setEscapeStart("&");
	setEscapeEnd(";");
	setEscapeStringCaseSensitive(true);
	
//	addEscapeStringSubstitute("amp", "&");
//	addEscapeStringSubstitute("apos", "'");
//	addEscapeStringSubstitute("lt", "<");
//	addEscapeStringSubstitute("gt", ">");
//	addEscapeStringSubstitute("quot", "\"");

	
	
	setPassThruUnknownEscapeString(true); //the HTML widget will render the HTML escape codes

	setTokenCaseSensitive(true);
}

bool BT_OSISHTML::handleToken(sword::SWBuf &buf, const char *token, sword::BasicFilterUserData *userData) {
  // manually process if it wasn't a simple substitution
	if (!substituteToken(buf, token)) {
    BT_UserData* myUserData = dynamic_cast<BT_UserData*>(userData);
		sword::SWModule* myModule = const_cast<sword::SWModule*>(myUserData->module); //hack
		
    XMLTag tag(token);
  	const	bool osisQToTick = ((!userData->module->getConfigEntry("OSISqToTick")) || (strcmp(userData->module->getConfigEntry("OSISqToTick"), "false")));

    if (!tag.getName()) {
      return false;
    }

		// <w> tag
		if (!strcmp(tag.getName(), "w")) {
			if ((!tag.isEmpty()) && (!tag.isEndTag())) { //start tag
				const char *attrib;
				const char *val;
				if ((attrib = tag.getAttribute("xlit"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					//buf.appendFormatted(" %s", val);
				}
				if ((attrib = tag.getAttribute("gloss"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					//buf.appendFormatted(" %s", val);
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

//            if ((!strcmp(val+2, "3588")) && (lastText.length() < 1)) {
//							show = false;
//            }
//						else 
						if (*val == 'H') {
              buf.appendFormatted(" <span strongnumber=\"%s\">", val);
            }
						else if (*val == 'G') {
              buf.appendFormatted(" <span strongnumber=\"%s\">", val);
            }
					} while (++i < count);
				}
				if ((attrib = tag.getAttribute("morph"))) {
					const int count = tag.getAttributePartCount("morph");
					int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
					do {
						attrib = tag.getAttribute("morph", i);
						if (i < 0)
              i = 0;	// to handle our -1 condition
						val = strchr(attrib, ':');
						val = (val) ? (val + 1) : attrib;
 						if ((*val == 'T') && (val[1] == 'H')) {
              buf.appendFormatted(" <a morphcode=\"%s\">", val+1);
            }
						else if ((*val == 'T') && (val[1] == 'G')) {
              buf.appendFormatted(" <span morphcode=\"%s\">", val+1);
            }
            else if ((*val == 'T')) {
              buf.appendFormatted(" <span morphcode=\"%s\">", val+1);
            }
					} while (++i < count);
				}
				if ((attrib = tag.getAttribute("POS"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					//buf.appendFormatted(" %s", val);
				}				
			}
			// end or empty <w> tag
			else if (tag.isEndTag()){
				buf += "</span>";
			}
		}
		// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) {
				const SWBuf type = tag.getAttribute("type");

				//if (type == "crossReference") { //note containing cross references
        //  buf += " <span class=\"footnote\">[";
        //  myUserData->noteType = BT_UserData::CrossReference;
        //}
        //else 
				if (type == "strongsMarkup") {
  				myUserData->suspendTextPassThru = true;
          myUserData->noteType = BT_UserData::StrongsMarkup;
        }
        else {	// leave strong's markup notes out, in the future we'll probably have different option filters to turn different note types on or off
					buf.appendFormatted(" <span class=\"footnote\" footnote=\"%s/%s/%s\">.</span> ", 
					myModule->Name(),
					myUserData->key->getShortText(),
					tag.getAttribute("swordFootnote"));
          myUserData->noteType = BT_UserData::Footnote;
					
					myUserData->suspendTextPassThru = true;
				}
			}
			if (tag.isEndTag()) {
        if (myUserData->noteType == BT_UserData::CrossReference) {
          //buf += "]</span> ";
        }
        else if (myUserData->noteType == BT_UserData::Footnote) {
          //buf += ")</span> ";
        }

        myUserData->noteType = BT_UserData::Unknown;
				myUserData->suspendTextPassThru = false;
			}
		}
		// <p> paragraph tag is handled by OSISHTMLHref
		// <reference> tag
		else if (!strcmp(tag.getName(), "reference")) {
			if (!tag.isEndTag() && !tag.isEmpty() && tag.getAttribute("osisRef")) {
        const char* ref = tag.getAttribute("osisRef");

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

				buf.appendFormatted("<a class=\"reference\" href=\"sword://%s/%s/%s\">",
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
		// <hi> highlighted text
		else if (!strcmp(tag.getName(), "hi")) {
			const SWBuf type = tag.getAttribute("type");
			
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "bold") {
					buf += "<span class=\"bold\">";
				}
				else if (type == "illuminated") {  // all other types
					buf += "<span class=\"illuminated\">";
				}
				else if (type == "italic") {  // all other types
					buf += "<span class=\"italic\">";
				}
				else if (type == "line-through") {  // all other types
					buf += "<span class=\"line-through\">";
				}
				else if (type == "normal") {  // all other types
					buf += "<span class=\"normal\">";
				}
				else if (type == "small-caps") {  // all other types
					buf += "<span class=\"small-caps\">";
				}
				else if (type == "underline") {  // all other types
					buf += "<span class=\"underline\">";
				}
			}
			else if (tag.isEndTag()) { //all hi replacements are html spans
				buf += "</span>";
			}
		}
		
		//name
		else if (!strcmp(tag.getName(), "name")) {
			const SWBuf type = tag.getAttribute("type");

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "geographic") {
					buf += "<span class=\"name\"><span class=\"geographic\">";
				}
				if (type == "holiday") {
					buf += "<span class=\"name\"><span class=\"holiday\">";
				}
				if (type == "nonhuman") {
					buf += "<span class=\"name\"><span class=\"nonhuman\">";
				}
				if (type == "person") {
					buf += "<span class=\"name\"><span class=\"person\">";
				}
				if (type == "ritual") {
					buf += "<span class=\"name\"><span class=\"ritual\">";
				}
			}
			else if (tag.isEndTag()) { //all hi replacements are html spans
				buf += "</span></span> ";
			}
		}

		else if (!strcmp(tag.getName(), "transChange")) {
			const SWBuf type = tag.getAttribute("type");

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "added") {
					buf.appendFormatted("<span class=\"transchange\" title=\"%s\"><span class=\"added\">", (const char*)i18n("Added text").utf8());
				}
				if (type == "amplified") {
					buf += "<span class=\"transchange\"><span class=\"amplified\">";
				}
				if (type == "changed") {
					buf += "<span class=\"transchange\"><span class=\"changed\">";
				}
				if (type == "deleted") {
					buf += "<span class=\"transchange\"><span class=\"deleted\">";
				}
				if (type == "moved") {
					buf += "<span class=\"transchange\"><span class=\"moved\">";
				}
				if (type == "tenseChange") {
					buf += "<span class=\"transchange\"><span class=\"tenseChange\">";
				}
			}
			else if (tag.isEndTag()) { //all hi replacements are html spans
				buf += "</span></span>";
			}
		}
		// <q> quote
		else if (!strcmp(tag.getName(), "q")) {
			SWBuf type = tag.getAttribute("type");
			SWBuf who = tag.getAttribute("who");
			const char *lev = tag.getAttribute("level");
			int level = (lev) ? atoi(lev) : 1;

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				/*buf += "{";*/

				myUserData->quote.who = who;
				//alternate " and '
				if(osisQToTick)
					buf += (level % 2) ? '\"' : '\'';

				if (who == "Jesus") {
					buf += "<span class=\"jesuswords\">";
				}
			}
			else if (tag.isEndTag()) {
        if (myUserData->quote.who == "Jesus")
          buf += "</span>";
        else if (osisQToTick) //alternate " and '
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
      return sword::OSISHTMLHREF::handleToken(buf, token, userData);
    }
	}
  return false;
}
