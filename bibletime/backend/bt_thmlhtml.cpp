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
#include <utilxml.h>

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
	addTokenSubstitute("/foreign", "</span>");
}

bool BT_ThMLHTML::handleToken(sword::SWBuf& buf, const char *token, DualStringMap &userData) {
	if (!substituteToken(buf, token) && !substituteEscapeString(buf, token)) {
    sword::XMLTag tag(token);
    
    if ( !strcasecmp(tag.getName(), "foreign") ) { // a text part in another language, we have to set the right font
      if (tag.getAttribute("lang")) {
        const char* abbrev = tag.getAttribute("lang");

        CLanguageMgr* langMgr = CPointers::languageMgr();
        CLanguageMgr::Language language = langMgr->languageForAbbrev( QString::fromLatin1(abbrev) );

        if (language.isValid()) {
          CBTConfig::FontSettingsPair fontSetting = CBTConfig::get(language);
          if (fontSetting.first) {
            const QFont f = fontSetting.second;
            buf.appendFormatted("<span lang=\"%s\" style=\"font-family:%s;font-size:%ipt;\">",
              abbrev,
              f.family().latin1(),
              f.pointSize()
            );
          }
          else { //CBTConfig says: don't set a special font, so we just set the language flag
            buf.appendFormatted("<span lang=\"%s\">", abbrev);
          }
        }
        else { //invalid language, just set the HTML language attribute
          buf.appendFormatted("<span lang=\"%s\">", abbrev);
        }
      }
    }
    else if (!strcasecmp(tag.getName(), "sync")) { //lemmas, morph codes or strongs
      if (tag.getAttribute("type") && !strcasecmp(tag.getAttribute("type"), "lemma")) { // Lemma
        sword::SWBuf value = tag.getAttribute("value");
        if ( value.length() > 0 ) {
          buf.appendFormatted(" &lt;%s&gt; ",
    				value.c_str()
          );
        };
      }
      else if (tag.getAttribute("type") && !strcasecmp(tag.getAttribute("type"), "morph")) { // Morph
        sword::SWBuf value = tag.getAttribute("value");
        if ( value.length() > 0 ) {
          buf.appendFormatted(" <a href=\"morph://Greek/%s\"><span class=\"morphcode\">(%s)</span></a> ",
            value.c_str(),
            value.c_str()
          );
        };
  		}
		  else if (tag.getAttribute("type") && !strcasecmp(tag.getAttribute("type"), "Strongs")) { // Strongs
        sword::SWBuf value = tag.getAttribute("value");
        if ( value[0] == 'H' ) { //hewbrew strong number
          buf.appendFormatted(" <a href=\"strongs://Hebrew/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span></a> ",
     				value.c_str()+1, //skip the H
            value.c_str()+1 //skip the H
          );
        }
        else if ( value[0] == 'G' ) { //hewbrew strong number
          buf.appendFormatted(" <a href=\"strongs://Greek/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span></a> ",
      			value.c_str()+1, //skip the G
            value.c_str()+1 //skip the G
          );
        };
      };
		}
		else if (!strcasecmp(tag.getName(), "scripRef")) { // a more complicated scripRef
      if (tag.isEndTag()) {
       	if (userData["inscriptRef"] == "true") { // like  "<scripRef passage="John 3:16">See John 3:16</scripRef>"
  				userData["inscriptRef"] = "false";
  				buf += thmlRefEnd().c_str();
  			}
  			else { // like "<scripRef>John 3:16</scripRef>"
   			  buf += parseSimpleRef( userData["lastTextNode"] ).c_str();
  				userData["suspendTextPassThru"] = "false";
  			}
      }
      else if (tag.getAttribute("passage") ) { //the passage was given within the scripRef tag
        userData["inscriptRef"] = "true";
        buf += parseThMLRef(tag.getAttribute("passage"), tag.getAttribute("version")).c_str();
      }
      else if ( !tag.getAttribute("passage") ) { // we're starting a scripRef like "<scripRef>John 3:16</scripRef>"
	  		userData["inscriptRef"] = "false";
  			// let's stop text from going to output
		  	userData["suspendTextPassThru"] = "true";
      }
		}
		else if (!strcasecmp(tag.getName(), "div")) {
      if (tag.isEndTag()) {
        buf += "</div>";
  			userData["SecHead"] = "false";
  			userData["Title"] = "false";
      }
      else if ( tag.getAttribute("class") && !strcasecmp(tag.getAttribute("class"),"sechead") ) {
  			userData["SecHead"] = "true";
  			buf += "<div class=\"sectiontitle\">";
      }
  		else if (tag.getAttribute("class") && !strcasecmp(tag.getAttribute("class"), "title")) {
        userData["Title"] = "true";
		  	buf += "<div class=\"booktitle\">";
      }
    }
		else if (!strcasecmp(tag.getName(), "img") && tag.getAttribute("src")) {
      //we have to remove the trailing / still
      
      buf.appendFormatted("<img src=\"file:%s/%s\" />", //we want to be XML compliant
        module->getConfigEntry("AbsoluteDataPath"),
        tag.getAttribute("src")
      );
 		}
    else { // let unknown token pass thru
			buf += '<';
      buf += token;
      buf += '>';
    }
  }
	return true;
}
