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
#include "bt_thmlhtml.h"
#include "versekey.h"

//Sword includes
#include <swmodule.h>

//Qt includes
#include <qstring.h>

BT_ThMLHTML::BT_ThMLHTML() {
	setEscapeStringCaseSensitive(true);
	setPassThruUnknownEscapeString(true); //the HTML widget will render the HTML escape codes	

  setTokenStart("<");
	setTokenEnd(">");
	setTokenCaseSensitive(true);
	addTokenSubstitute("note", " <span class=\"footnote\">(");
	addTokenSubstitute("/note", ")</span> ");

	addTokenSubstitute("foreign lang=\"el\"", "<span lang=\"el\">");
	addTokenSubstitute("foreign lang=\"he\"", "<span lang=\"he\" dir=\"rtl\">");
	addTokenSubstitute("/foreign",						"</span>");
}

bool BT_ThMLHTML::handleToken(sword::SWBuf& buf, const char *token, DualStringMap &userData) {
	unsigned long i = 0;
	const int tokenLength = strlen(token);
	
	if (!substituteToken(buf, token) && !substituteEscapeString(buf, token)) {
		if (!strncasecmp(token, "sync type=\"lemma\" ", 18)) { //LEMMA
      for (i = 18; i < tokenLength; ++i) {
        if ( !strncasecmp(token+i, "value=\"", 7) ) {
          sword::SWBuf value;
    			for (i += 7; (i < tokenLength) && (token[i] != '\"'); ++i) {
    				value += token[i];
          }
    			buf.appendFormatted(" &lt;%s&gt; ",
    				value.c_str()
          );
          break;
        };
      };
    }
		else if (!strncasecmp(token, "sync type=\"morph\" ", 18)) { //Morph
      for (i = 18; i < tokenLength; ++i) {
        if ( !strncasecmp(token+i, "value=\"", 7) ) {
          sword::SWBuf value;
    			for (i += 7; (i < tokenLength) && (token[i] != '\"'); ++i) {
    				value += token[i];
          }
    			buf.appendFormatted(" <a href=\"morph://Greek/%s\"><span class=\"morphcode\">(%s)</span></a> ",
    				value.c_str(),
            value.c_str()
          );
          break;
        };
      };
		}
		else if (!strncasecmp(token, "sync type=\"Strongs\" ", 20)) {
      for (i = 20; i < tokenLength; ++i) {
        if ( !strncasecmp(token+i, "value=\"H", 8) ) {
          sword::SWBuf value;
    			for (i += 8; (i < tokenLength) && (token[i] != '\"'); ++i) {
    				value += token[i];
          }
    			buf.appendFormatted(" <a href=\"strongs://Hebrew/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span></a> ",
    				value.c_str(),
            value.c_str()
          );
          break;
        }
        else if ( !strncasecmp(token+i, "value=\"G", 8) ) {
          sword::SWBuf value;
    			for (i += 8; (i < tokenLength) && (token[i] != '\"'); ++i) {
    				value += token[i];
          }
    			buf.appendFormatted(" <a href=\"strongs://Greek/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span></a> ",
    				value.c_str(),
            value.c_str()
          );
          break;
        };
      };
		}
		else if (!strncasecmp(token, "scripRef p", 10) || !strncmp(token, "scripRef v", 10)) { // a more complicated scripRef
			userData["inscriptRef"] = "true";
			if (!strncasecmp(token, "scripRef v", 10)) { //module given
				sword::SWBuf module_version;
				for (i = 18; (i < tokenLength-1) && (token[i] != '\"'); i++) {
					module_version += token[i];
				}
				userData["lastRefModule"] = module_version.c_str();
			}
			else if (!strncasecmp(token, "scripRef p", 10)) { //passage without module
        string verse_str;
				for (i = 18; (i < tokenLength-1) && (token[i] != '\"'); i++) {
          verse_str += token[i];
				}
 			  buf += parseThMLRef(verse_str).c_str();
      }
      if ( !strncasecmp(token+i+2, "passage=", 8) ) { //passage after module part
				string verse_str;
				i += 11;			
				for (; (i < tokenLength-1) && (token[i] != '\"'); i++)	{
          verse_str += token[i];
				}
				buf += parseThMLRef(verse_str, userData["lastRefModule"].c_str()).c_str();
			}
		}
    // we're starting a scripRef like "<scripRef>John 3:16</scripRef>"
		else if (!strcasecmp(token, "scripRef")) {
			userData["inscriptRef"] = "false";
			// let's stop text from going to output
			userData["suspendTextPassThru"] = "true";
		}
		// we've ended a scripRef
		else if (!strcasecmp(token, "/scripRef")) {
			if (userData["inscriptRef"] == "true") { // like  "<scripRef passage="John 3:16">See John 3:16</scripRef>"
				userData["inscriptRef"] = "false";
				buf += thmlRefEnd().c_str();
			}			
			else { // like "<scripRef>John 3:16</scripRef>"
 			  buf += parseSimpleRef( userData["lastTextNode"] ).c_str();
				userData["suspendTextPassThru"] = "false";
			}
		}
		else if (!strncasecmp(token, "div class=\"sechead\"", 19)) {
			userData["SecHead"] = "true";
			buf += "<div class=\"sectiontitle\">";
		}
		else if (!strncasecmp(token, "div class=\"title\"", 19)) {
      userData["Title"] = "true";
			buf += "<div class=\"booktitle\">";
		}
//		else if (!strncmp(token, "img ", 4)) {
//			const char *src = strstr(token, "src");
//			if (!src)	{	// assert we have a src attribute
//				return false;
//      }
//
//			buf += '<';
//			for (const char *c = token; *c; c++) {
//				if (c == src) {
//					for (;((*c) && (*c != '"')); c++) {
//						buf += *c;
//          }
//
//					if (!*c) {
//            c--;
//            continue;
//          }
//
//					buf += '"';
//					if (*(c+1) == '/') {
//						buf += "file:";
//						buf += module->getConfigEntry("AbsoluteDataPath");
//						if (*(*buf-1) == '/')
//							c++;		// skip '/'
//					}
//					continue;
//				}
//				buf += *c;
//			}
//			buf += '>';
//		}		
		else { // let unknown token pass thru
			buf += '<';
			for (i = 0; i < tokenLength; i++) {
				buf += token[i];
      }
			buf += '>';
    }
  }
	return true;
}
