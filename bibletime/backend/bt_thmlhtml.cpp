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

#include "clanguagemgr.h"
#include "frontend/cbtconfig.h"
#include "util/cpointers.h"

#include <iostream>

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
	addTokenSubstitute("/foreign",						"</span>");
}

bool BT_ThMLHTML::handleToken(sword::SWBuf& buf, const char *token, DualStringMap &userData) {
//	unsigned long i = 0;
//	const int tokenLength = strlen(token);
	
	if (!substituteToken(buf, token) && !substituteEscapeString(buf, token)) {
    if ( !strncmp(token, "foreign ", 8) ) { // a text part in another language, we have to set the right font
      token += 8;
      if ( const char* pos = strstr(token, "lang=\"") ) {
        pos+=6;
        sword::SWBuf abbrev;
        for (const char* end = index(pos, '\"'); pos < end; pos++) {
  				abbrev += *pos;
        }

        CLanguageMgr* langMgr = CPointers::languageMgr();
        CLanguageMgr::Language language = langMgr->languageForAbbrev( QString::fromLatin1(abbrev.c_str()) );

        if (language.isValid()) {
          CBTConfig::FontSettingsPair fontSetting = CBTConfig::get(language);
          if (fontSetting.first) {
            const QFont f = fontSetting.second;
            buf.appendFormatted("<span lang=\"%s\" style=\"font-family:%s;font-size:%ipt;\">",
              abbrev.c_str(),
              f.family().latin1(),
              f.pointSize()
            );
          }
          else { //CBTConfig says: don't set a special font, so we just set the language flag
            buf.appendFormatted("<span lang=\"%s\">", abbrev.c_str());
          }
        }
        else { //invalid language, just set the HTML language attribute
          buf.appendFormatted("<span lang=\"%s\">", abbrev.c_str());
        }
      }
    }
    else if (!strncmp(token, "sync ", 5) ) { //lemmas, morph codes or strongs
      token += 5;
      if (strstr(token, "type=\"lemma\" ")) { // Lemma
        if ( const char* pos = strstr(token, "value=\"") ) {
          pos+=7;

          sword::SWBuf value;
          for (const char* end = index(pos, '\"'); pos < end; pos++) {
    				value += *pos;
          }

          buf.appendFormatted(" &lt;%s&gt; ",
    				value.c_str()
          );
        };
      }
      else if (strstr(token, "type=\"morph\" ")) { // Morph
        if ( const char* pos = strstr(token, "value=\"") ) {
          pos+=7;

          sword::SWBuf value;
          for (const char* end = index(pos, '\"'); pos < end; pos++) {
    				value += *pos;
          }

          buf.appendFormatted(" <a href=\"morph://Greek/%s\"><span class=\"morphcode\">(%s)</span></a> ",
            value.c_str(),
            value.c_str()
          );
        };
  		}
		  else if (strstr(token, "type=\"Strongs\" ")) { // Strongs
        if ( const char* pos = strstr(token, "value=\"H") ) { //hewbrew strong number
          pos+=8;

          sword::SWBuf value;
          for (const char* end = index(pos, '\"'); pos < end; pos++) {
    				value += *pos;
          }

          buf.appendFormatted(" <a href=\"strongs://Hebrew/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span></a> ",
     				value.c_str(),
            value.c_str()
          );
        }
        else if ( const char* pos = strstr(token, "value=\"G") ) { //hewbrew strong number
          pos+=8;

          sword::SWBuf value;
          for (const char* end = index(pos, '\"'); pos < end; pos++) {
    				value += *pos;
          }

          buf.appendFormatted(" <a href=\"strongs://Greek/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span></a> ",
      			value.c_str(),
            value.c_str()
          );
        };
      };
		}
		else if (!strncmp(token, "scripRef ", 9) /*|| !strncmp(token, "scripRef v", 10) */) { // a more complicated scripRef
			userData["inscriptRef"] = "true";
      token += 9;
      sword::SWBuf module;
      
      if ( const char* pos = strstr(token, "version=\"") ) { //a module is given in the scripRef tag!
        pos += 9;
        for (const char* end = index(pos, '\"'); pos < end; pos++ ) {
          module += *pos;
        };
      }

      if ( const char* pos = strstr(token, "passage=\"") ) { //the passage which was given in the scripRef tag
        pos += 9;
        sword::SWBuf passage;
        for (const char* end = index(pos, '\"'); pos < end; pos++ ) {
          passage += *pos;
        }

        buf += parseThMLRef(passage.c_str(), module.c_str()).c_str();
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
		else if (!strncmp(token, "img ", 4)) {
			const char *src = strstr(token, "src");
			if (!src)		// assert we have a src attribute
				return false;

			buf += '<';
			for (const char *c = token; *c; c++) {
				if (c == src) {
					for (;((*c) && (*c != '"')); c++)
						buf += *c;

					if (!*c) { c--; continue; }

					buf += '"';
					if (*(c+1) == '/') {
						buf += "file:";
						buf += module->getConfigEntry("AbsoluteDataPath");
						if (buf[buf.length()-2] == '/')
							c++;		// skip '/'
					}
					continue;
				}
				buf += *c;
			}
			buf += '>';
		}
    else { // let unknown token pass thru
			buf += '<';
      buf += token;
      buf += '>';
    }
  }
	return true;
}
