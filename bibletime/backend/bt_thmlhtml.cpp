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
  
	replaceTokenSubstitute("/foreign", "</span>");

  if (tokenSubMap.find("note") != tokenSubMap.end()) { //remove note tag
	  tokenSubMap.erase( tokenSubMap.find("note") );
  }
  if (tokenSubMap.find("/note") != tokenSubMap.end()) { //remove note tag
	  tokenSubMap.erase( tokenSubMap.find("/note") );
  }
}

bool BT_ThMLHTML::handleToken(sword::SWBuf &buf, const char *token, sword::BasicFilterUserData *userData) {  
	if (!substituteToken(buf, token) && !substituteEscapeString(buf, token)) {
    sword::XMLTag tag(token);
    BT_UserData* myUserData = dynamic_cast<BT_UserData*>(userData);
    sword::SWModule* myModule = const_cast<sword::SWModule*>(myUserData->module); //hack to be able to call stuff like Lang()

    if ( tag.getName() && !strcasecmp(tag.getName(), "foreign") ) { // a text part in another language, we have to set the right font
      if (tag.getAttribute("lang")) {
        const char* abbrev = tag.getAttribute("lang");

        CLanguageMgr::Language language = CPointers::languageMgr()->languageForAbbrev( QString::fromLatin1(abbrev) );

        if (language.isValid()) {
          CBTConfig::FontSettingsPair fontSetting = CBTConfig::get(language);
          if (fontSetting.first) {
            const QFont f = fontSetting.second;
            buf.appendFormatted("<span class=\"foreign\" lang=\"%s\" style=\"font-family:%s;font-size:%ipt;\">",
              abbrev,
              f.family().latin1(),
              f.pointSize()
            );
          }
          else { //CBTConfig says: don't set a special font, so we just set the language flag
            buf.appendFormatted("<span class=\"foreign\" lang=\"%s\">", abbrev);
          }
        }
        else { //invalid language, just set the HTML language attribute
          buf.appendFormatted("<span class=\"foreign\" lang=\"%s\">", abbrev);
        }
      }
    }
    else if (tag.getName() && !strcasecmp(tag.getName(), "sync")) { //lemmas, morph codes or strongs
      if (tag.getAttribute("type") && !strcasecmp(tag.getAttribute("type"), "lemma")) { // Lemma
        const char* value = tag.getAttribute("value");
        if ( strlen(value) ) {
          buf.appendFormatted(" <span class=\"lemma\">&lt;%s&gt;</span> ",
            value
          );
        };
      }
      else if (tag.getAttribute("type") && !strcasecmp(tag.getAttribute("type"), "morph")) { // Morph
        const char* value = tag.getAttribute("value");
        if ( value ) {
          buf.appendFormatted(" <a class=\"morphcode\" href=\"morph://Greek/%s\">(%s)</a> ",
            value,
            value
          );
        };
  		}
		  else if (tag.getAttribute("type") && !strcasecmp(tag.getAttribute("type"), "Strongs")) { // Strongs
        const char* value = tag.getAttribute("value");
        if ( value && value[0] == 'H' ) { //hewbrew strong number
          buf.appendFormatted(" <a class=\"strongnumber\" href=\"strongs://Hebrew/%s\">&lt;%s&gt;</a> ",
     				value+1, //skip the H
            value+1 //skip the H
          );
        }
        else if ( value && value[0] == 'G' ) { //hewbrew strong number
          buf.appendFormatted(" <a class=\"strongnumber\" href=\"strongs://Greek/%s\">&lt;%s&gt;</a> ",
      			value+1, //skip the G
            value+1 //skip the G
          );
        };
      };
		}
		else if (tag.getName() && !strcasecmp(tag.getName(), "note")) { // <note> tag
			if (!tag.isEndTag() && !tag.isEmpty()) {
				//SWBuf type = tag.getAttribute("type");
				//SWBuf footnoteNumber = tag.getAttribute("swordFootnote");
				//buf += "<span class=\"footnote\">(";
				
				buf.appendFormatted(" <span class=\"footnote\" footnote=\"%s/%s/%s\">.</span> ", 
					myModule->Name(),
					myUserData->key->getShortText(),
					QString::number(myUserData->swordFootnote++).latin1()
				//tag.getAttribute("swordFootnote")
				);

				userData->suspendTextPassThru = true;
			}
			else if (tag.isEndTag() && !tag.isEmpty()) { //end tag
				//buf += ")</span>";
				userData->suspendTextPassThru = false;
			}
		}
		else if (tag.getName() && !strcasecmp(tag.getName(), "scripRef")) { // a more complicated scripRef
      if (tag.isEndTag()) {
       	if (myUserData->inscriptRef) { // like  "<scripRef passage="John 3:16">See John 3:16</scripRef>"
  				myUserData->inscriptRef = false;
  				buf += thmlRefEnd().c_str();
  			}
  			else { // like "<scripRef>John 3:16</scripRef>"
   			  buf += parseSimpleRef( myUserData->lastTextNode, myModule ? myModule->Lang() : "en" ).c_str();
  				myUserData->suspendTextPassThru = false;
  			}
      }		
      else if (tag.getAttribute("passage") ) { //the passage was given within the scripRef tag
        myUserData->inscriptRef = true;
        buf += parseThMLRef(tag.getAttribute("passage"), tag.getAttribute("version")).c_str();
      }
      else if ( !tag.getAttribute("passage") ) { // we're starting a scripRef like "<scripRef>John 3:16</scripRef>"
	  		myUserData->inscriptRef = false;
  			// let's stop text from going to output
		  	userData->suspendTextPassThru = true;
      }
		}
		else if (tag.getName() && !strcasecmp(tag.getName(), "div")) {                                      
      if (tag.isEndTag()) {
        buf += "</div>";
      }
      else if ( tag.getAttribute("class") && !strcasecmp(tag.getAttribute("class"),"sechead") ) {
  			buf += "<div class=\"sectiontitle\">";
      }
  		else if (tag.getAttribute("class") && !strcasecmp(tag.getAttribute("class"), "title")) {
		  	buf += "<div class=\"booktitle\">";
      }
    }
		else if (tag.getName() && !strcasecmp(tag.getName(), "img") && tag.getAttribute("src")) {
      const char* value = tag.getAttribute("src");
      if (value[0] == '/') {
        value++; //strip the first /
      }
      
      buf.appendFormatted("<img src=\"file:%s/%s\" />",
        myUserData->module->getConfigEntry("AbsoluteDataPath"),
        value
      );
 		}
    else { // let unknown token pass thru
    	return sword::ThMLHTML::handleToken(buf, token, userData);
    }
  }
	return true;
}
