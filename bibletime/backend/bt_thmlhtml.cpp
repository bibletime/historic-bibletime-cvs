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
#include "backend/bt_thmlhtml.h"
#include "backend/clanguagemgr.h"
#include "backend/cswordmoduleinfo.h"

#include "frontend/cbtconfig.h"

#include "util/cpointers.h"

#include <iostream>

//Sword includes
#include <swmodule.h>
#include <utilxml.h>
#include "versekey.h"

//Qt includes
#include <qstring.h>
#include <qregexp.h>

//System includes
#include <stdlib.h>


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

char BT_ThMLHTML::processText(sword::SWBuf& buf, const sword::SWKey* key, const sword::SWModule* module) {
	ThMLHTML::processText(buf, key, module);
	
	//process the sync tags and create the right morph/lemma span tags around the right text parts
	int pos = -1;
	int lastPos = -1;
	int posRev = 0;
	QString t = QString::fromUtf8(buf.c_str());
	QRegExp startRE("<sync[^>]+(type|value)=\"([^\"]+)\"[^>]+(type|value)=\"([^\"]+)\"([^<]*)>");
	QRegExp endRE("(^)|>");
	
	CSwordModuleInfo* m = CPointers::backend()->findModuleByName( module->Name() ); 
	if (m && (m->has(CSwordBackend::lemmas) || m->has(CSwordBackend::strongNumbers))) { //only parse if the module has strongs or lemmas
		do {
			pos = startRE.search(t, pos+1);
			posRev = endRE.searchRev(t,pos);
			if (pos == posRev+2) { //two morph/strongs which belong together
 				posRev = endRE.searchRev(t,posRev-1);
			}
			
// 			qWarning("pos=%i, posRev=%i, lastpos=%i", pos, posRev, lastPos);
			
			if ((pos >= 0) && (posRev>= 0) && (posRev < pos) && (posRev > lastPos)) {//we found a valid text span
				const bool isMorph = 
					((startRE.cap(1) == "type") ? startRE.cap(2) : startRE.cap(4)) == "morph";
				const bool isStrongs = 
					((startRE.cap(1) == "type") ? startRE.cap(2) : startRE.cap(4)) == "Strongs";				
				const QString value = 
					(startRE.cap(1) == "value") ? startRE.cap(2) : startRE.cap(4);

// 	 			qWarning("found %s", value.latin1());
				
				posRev = !posRev ? 0 : posRev+1;
				
				QString part = t.mid(posRev, pos-posRev);
				int end = t.find(">", pos+1);
				
				if (end > pos) {
					if (!part.isEmpty()) {
						QString rep;
						if (isStrongs) {
							rep = QString::fromLatin1("<span lemma=\"%1\">%2</span>").arg(value).arg(part);
						}
						else if (isMorph) {
							rep = QString::fromLatin1("<span morph=\"%1\">%2</span>").arg(value).arg(part);
						}
						
						t.replace(posRev, end-posRev+1, rep);
						pos = posRev + part.length() + (end-pos) - rep.length()+1;
						
// 						qWarning("after: pos=%i", pos);
					}
					else {//found text is empty, this morph/lemma belongs to the one before (i.e. two following belonging to the same text)
						//TODO: remove the ThML tag
						
						int attrPos = t.findRev(QRegExp("morph=|lemma="),posRev);
						if ((attrPos > 0) && (attrPos < posRev)) { //insert the new attribute here
							QString attr = 
								QString::fromLatin1("%1=\"%2\" ")
									.arg(isMorph ? "morph" : "lemma")
									.arg(value);
									
							t.insert(attrPos, attr);
							
							pos += attr.length();
						}	
					}
				}
			}
			
			lastPos = pos;
		}
		while (pos >= 0);
	 	
// 		qWarning("replaced: %s", t.latin1());
	}
	
	buf = (const char*)t.utf8();
	return 1;
}


bool BT_ThMLHTML::handleToken(sword::SWBuf &buf, const char *token, sword::BasicFilterUserData *userData) {  
	if (!substituteToken(buf, token) && !substituteEscapeString(buf, token)) {
    sword::XMLTag tag(token);
    BT_UserData* myUserData = dynamic_cast<BT_UserData*>(userData);
    sword::SWModule* myModule = const_cast<sword::SWModule*>(myUserData->module); //hack to be able to call stuff like Lang()

    if ( tag.getName() && !strcasecmp(tag.getName(), "foreign") ) { // a text part in another language, we have to set the right font
      if (tag.getAttribute("lang")) {
        const char* abbrev = tag.getAttribute("lang");
        const CLanguageMgr::Language* const language = CPointers::languageMgr()->languageForAbbrev( QString::fromLatin1(abbrev) );

        if (language->isValid()) {
	        buf.appendFormatted("<span class=\"foreign\" lang=\"%s\">", abbrev);
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
				buf.append("<");
				buf.append(token);
				buf.append(">");
/*        const char* value = tag.getAttribute("value");
        if ( value ) {
          buf.appendFormatted(" <span morph=\"G%s\">%s</span> ",
            value,
            value
          );
        };*/
  		}
		  else if (tag.getAttribute("type") && !strcasecmp(tag.getAttribute("type"), "Strongs")) { // Strongs
				buf.append("<");
				buf.append(token);
				buf.append(">");
/*        const char* value = tag.getAttribute("value");
        if ( value && value[0] == 'H' ) { //hewbrew strong number
          buf.appendFormatted(" <span strong=\"H%s\">%s</span> ",
     				value+1, //skip the H
            value+1 //skip the H
          );
        }
        else if ( value && value[0] == 'G' ) { //hewbrew strong number
          buf.appendFormatted(" <span strong=\"G%s\">%s</span> ",
      			value+1, //skip the G
            value+1 //skip the G
          );
        };*/
      };
		}
		else if (tag.getName() && !strcasecmp(tag.getName(), "note")) { // <note> tag
			if (!tag.isEndTag() && !tag.isEmpty()) {
				//SWBuf type = tag.getAttribute("type");
				//SWBuf footnoteNumber = tag.getAttribute("swordFootnote");
				//buf += "<span class=\"footnote\">(";
				
				buf.appendFormatted(" <span class=\"footnote\" note=\"%s/%s/%s\">.</span> ", 
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
//  				buf += thmlRefEnd().c_str();
  				myUserData->suspendTextPassThru = false;
  			}
  			else { // like "<scripRef>John 3:16</scripRef>"
//   			  buf += parseSimpleRef( myUserData->lastTextNode, myModule ? myModule->Lang() : "en" ).c_str();
          buf.appendFormatted(" <span class=\"crossreference\" crossrefs=\"%s\">-</span>", 	
						myUserData->lastTextNode.c_str()
					);

  				myUserData->suspendTextPassThru = false;
  			}
      }		
      else if (tag.getAttribute("passage") ) { //the passage was given within the scripRef tag
        myUserData->inscriptRef = true;
      //  buf += parseThMLRef(tag.getAttribute("passage"), tag.getAttribute("version")).c_str();
         buf.appendFormatted(" <span class=\"crossreference\" crossrefs=\"%s\">-</span>", 	
						tag.getAttribute("passage")
				);
 				myUserData->suspendTextPassThru = true;
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
