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
	
 	CSwordModuleInfo* m = CPointers::backend()->findModuleByName( module->Name() ); 
	if (m && !(m->has(CSwordBackend::lemmas) || m->has(CSwordBackend::strongNumbers))) { //only parse if the module has strongs or lemmas
		return 1;
	}

	QString result;
	
	QString t = QString::fromUtf8(buf.c_str());
	QRegExp tag("(<sync[^>]+(type|value)=\"([^\"]+)\"[^>]+(type|value)=\"([^\"]+)\"([^<]*)>)+");
	
	QStringList list;
	int lastMatchEnd = 0;
	int pos = tag.search(t,0);
	
	while (pos != -1) {
		list.append(t.mid(lastMatchEnd, pos+tag.matchedLength()-lastMatchEnd));
	
		lastMatchEnd = pos+tag.matchedLength();
		pos = tag.search(t,pos+tag.matchedLength());
	}
	
	if (!t.right(t.length() - lastMatchEnd).isEmpty()) {
		list.append(t.right(t.length() - lastMatchEnd));
	}

	tag = QRegExp("<sync[^>]+(type|value)=\"([^\"]+)\"[^>]+(type|value)=\"([^\"]+)\"([^<]*)>");

	for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
		QString e = *it;
		
		const bool textPresent = (e.stripWhiteSpace().remove(QRegExp("[.,;:]")).left(1) != "<");
		if (!textPresent) {
			continue;
		}
		
		int pos = tag.search(e, 0);
		bool insertedTag = false;
		while (pos != -1) {
			const bool isMorph = 
				((tag.cap(1) == "type") ? tag.cap(2) : tag.cap(4)) == "morph";
			const bool isStrongs = 
				((tag.cap(1) == "type") ? tag.cap(2) : tag.cap(4)) == "Strongs";				
			const QString value = 
				(tag.cap(1) == "value") ? tag.cap(2) : tag.cap(4);
			
			if (value.isEmpty()) {
				break;
			}
			
			//insert the span
			if (!insertedTag) {
				e.replace(pos, tag.matchedLength(), "</span>");
 				pos += 7;

				QString rep = QString("<span lemma=\"%1\">").arg(value);
				int startPos = 0;
				QChar c = e[startPos];
				
				while ((startPos < pos) && (c.isSpace() || c.isPunct())) {
					++startPos;					
					c = e[startPos];
				}
				
				e.insert( startPos, rep );
				pos += rep.length();
			}
			else { //add the attribute to the existing tag
				e.remove(pos, tag.matchedLength());
				
				const int attrPos = e.find(QRegExp("morph=|lemma="),0);
				if (attrPos >= 0) {
					QString attr = QString::fromLatin1("%1=\"%2\" ").arg(isMorph ? "morph" : "lemma").arg(value);
					e.insert(attrPos, attr);
					
					pos += attr.length();
				}
			}
				
			insertedTag = true;
			pos = tag.search(e, pos);
		}
		
		result += e;
	}
	
	if (list.count()) {
		buf = (const char*)result.utf8();
	}
  
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
      else if (tag.getAttribute("type") && (!strcasecmp(tag.getAttribute("type"), "morph") || !strcasecmp(tag.getAttribute("type"), "Strongs"))) { // Morph or Strong
				buf.append('<');
				buf.append(token);
				buf.append('>');
  		}
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
