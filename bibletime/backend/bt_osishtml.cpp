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

using namespace Filters;

BT_OSISHTML::BT_OSISHTML() : sword::OSISHTMLHREF() { //TODO: FIX INHERITANCE
	
	setPassThruUnknownEscapeString(true); //the HTML widget will render the HTML escape codes

	replaceTokenSubstitute("inscription", "<span class=\"inscription\">");
	replaceTokenSubstitute("/inscription","</span>");
	
	replaceTokenSubstitute("mentioned", "<span class=\"mentioned\">");
	replaceTokenSubstitute("/mentioned", "</span>");
	
	replaceTokenSubstitute("divineName", "<span class=\"name\"><span class=\"divine\">");
	replaceTokenSubstitute("/divineName", "</span></span>");
	
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
				
				XMLTag outTag("span");
				SWBuf attrValue;
				
				if ((attrib = tag.getAttribute("xlit"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outTag.setAttribute("xlit", val);
				}
				if ((attrib = tag.getAttribute("gloss"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outTag.setAttribute("gloss", val);
				}
				if ((attrib = tag.getAttribute("lemma"))) {
					const int count = tag.getAttributePartCount("lemma");
					int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
					attrValue = "";
					
 					do {
						if (attrValue.length()) {
							attrValue += "|";
						}
						
						attrib = tag.getAttribute("lemma", i);
						if (i < 0) { // to handle our -1 condition
              i = 0;
						}
						val = strchr(attrib, ':');
						val = (val) ? (val + 1) : attrib;

						if ((*val == 'H') || (*val == 'G')) {
            	attrValue.append(val);
						}						
 					} while (++i < count);
					
					if (attrValue.length()) {						
						outTag.setAttribute("lemma", attrValue.c_str());
					}
				}
				if ((attrib = tag.getAttribute("morph"))) {
					const int count = tag.getAttributePartCount("morph");
					int i = (count > 1) ? 0 : -1;		// -1 for whole value cuz it's faster, but does the same thing as 0
					attrValue = "";
					do {
						if (attrValue.length()) {
							attrValue += "|";
						}

						attrib = tag.getAttribute("morph", i);
						if (i < 0) {
              i = 0;	// to handle our -1 condition
						}
						val = strchr(attrib, ':');
						val = (val) ? (val + 1) : attrib;
 						if (!strncmp(attrib, "x-Robinson",10)) { //robinson codes
							attrValue.append(val);
						}
						else if ((*val == 'T') && ((val[1] == 'H') || (val[1] == 'H'))) {
							attrValue.append(val+1);
            }
            else if ((*val == 'T')) {
							attrValue.append(val);
            }
					} while (++i < count);
					
					if (attrValue.length()) {
						outTag.setAttribute("morph", attrValue.c_str());
					}
				}
				if ((attrib = tag.getAttribute("POS"))) {
					val = strchr(attrib, ':');
					val = (val) ? (val + 1) : attrib;
					outTag.setAttribute("pos", val);
				}	
				
				buf.append( outTag.toString() );
// 				qWarning( "appended: ", outTag.toString() );
			}
			else if (tag.isEndTag()){ // end or empty <w> tag
				buf.append("</span>");
			}
		}
		// <note> tag
		else if (!strcmp(tag.getName(), "note")) {
			if (!tag.isEndTag()) {
				const SWBuf type = tag.getAttribute("type");

				if (type == "crossReference") { //note containing cross references
					//get the refList value of the right entry attribute
					AttributeList notes = myModule->getEntryAttributes()["Footnote"];
					bool foundNote = false;
					
					SWBuf id = tag.getAttribute("osisID");
					SWBuf refList;
					
					for (AttributeList::iterator list_it = notes.begin(); (list_it != notes.end()) && !foundNote; ++list_it ) {
						for (AttributeValue::iterator val_it = list_it->second.begin(); (val_it != list_it->second.end()) && !foundNote; ++val_it ) {
							if ((val_it->first == "osisID") && (val_it->second == id)) {
								foundNote = true; //this break the loop
								refList = list_it->second["refList"];
							}
						}
					}

					if (refList.length()) {
						buf.append(" <span class=\"crossreference\" crossrefs=\"");
						buf.append(refList.c_str());
						buf.append("\"> ");
							
          	myUserData->noteType = BT_UserData::CrossReference;
					}
					else {
						myUserData->noteType = BT_UserData::Unknown;
					}
        }
        else if (type == "explanation") {
        }
        else if (type == "strongsMarkup") { 
					/**
					* leave strong's markup notes out, in the future we'll probably have 
					* different option filters to turn different note types on or off
					*/
					
  				myUserData->suspendTextPassThru = true;
          myUserData->noteType = BT_UserData::StrongsMarkup;
        }
        else {	
					buf.append(" <span class=\"footnote\" note=\"");
					buf.append(myModule->Name());
					buf.append('/');
					buf.append(myUserData->key->getShortText());
					buf.append('/');
					buf.append( QString::number(myUserData->swordFootnote++).latin1() );
					buf.append("\">*n</span> ");
					
          myUserData->noteType = BT_UserData::Footnote;
					
					myUserData->suspendTextPassThru = true;
				}
			}
			else { //if (tag.isEndTag()) {
        if (myUserData->noteType == BT_UserData::CrossReference) {
          buf.append("</span> ");
					myUserData->suspendTextPassThru = false;
        }

        myUserData->noteType = BT_UserData::Unknown;
				myUserData->suspendTextPassThru = false;
			}
		}
		// <p> paragraph tag is handled by OSISHTMLHref
		else if (!strcmp(tag.getName(), "reference")) { // <reference> tag
/*			if (!tag.isEndTag() && !tag.isEmpty() && tag.getAttribute("osisRef")) {
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
				
				myUserData->suspendTextPassThru = true;
			}
			else if (tag.isEndTag()) {
 			//	buf.append("</a>");
// 				myUserData->suspendTextPassThru = false;
			}
			else {	// empty reference marker
				// -- what should we do?  nothing for now.
			}*/
		}
    // <l> is handled by OSISHTMLHref
		// <title>
		else if (!strcmp(tag.getName(), "title")) {
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
  			buf.append("<div class=\"sectiontitle\">");
			}
			else if (tag.isEndTag()) {
  			buf.append("</div>");
			}
			else {	// empty title marker
				// what to do?  is this even valid?
				buf.append("<br/>");
			}
		}	
		// <hi> highlighted text
		else if (!strcmp(tag.getName(), "hi")) {
			const SWBuf type = tag.getAttribute("type");
			
			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "bold") {
					buf.append("<span class=\"bold\">");
				}
				else if (type == "illuminated") { 
					buf.append("<span class=\"illuminated\">");
				}
				else if (type == "italic") { 
					buf.append("<span class=\"italic\">");
				}
				else if (type == "line-through") {
					buf.append("<span class=\"line-through\">");
				}
				else if (type == "normal") { 
					buf.append("<span class=\"normal\">");
				}
				else if (type == "small-caps") {
					buf.append("<span class=\"small-caps\">");
				}
				else if (type == "underline") {
					buf.append("<span class=\"underline\">");
				}
			}
			else if (tag.isEndTag()) { //all hi replacements are html spans
				buf.append("</span>");
			}
		}
		
		//name
		else if (!strcmp(tag.getName(), "name")) {
			const SWBuf type = tag.getAttribute("type");

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "geographic") {
					buf.append("<span class=\"name\"><span class=\"geographic\">");
				}
				else if (type == "holiday") {
					buf.append("<span class=\"name\"><span class=\"holiday\">");
				}
				else if (type == "nonhuman") {
					buf.append("<span class=\"name\"><span class=\"nonhuman\">");
				}
				else if (type == "person") {
					buf.append("<span class=\"name\"><span class=\"person\">");
				}
				else if (type == "ritual") {
					buf.append("<span class=\"name\"><span class=\"ritual\">");
				}
				else{
					buf.append("<span class=\"name\"><span>");
				}
			}
			else if (tag.isEndTag()) { //all hi replacements are html spans
				buf.append("</span></span> ");
			}
		}

		else if (!strcmp(tag.getName(), "transChange")) {
			SWBuf type = tag.getAttribute("type");
			if ( !type.length() ) {
			 type = tag.getAttribute("changeType");
			}

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				if (type == "added") {
					buf.appendFormatted("<span class=\"transchange\" title=\"%s\"><span class=\"added\">", (const char*)i18n("Added text").utf8());
				}
				else if (type == "amplified") {
					buf.append("<span class=\"transchange\"><span class=\"amplified\">");
				}
				else if (type == "changed") {
					buf.append("<span class=\"transchange\"><span class=\"changed\">");
				}
				else if (type == "deleted") {
					buf.append("<span class=\"transchange\"><span class=\"deleted\">");
				}
				else if (type == "moved") {
					buf.append("<span class=\"transchange\"><span class=\"moved\">");
				}
				else if (type == "tenseChange") {
					buf.append("<span class=\"transchange\"><span class=\"tenseChange\">");
				}
				else {
					buf.append("<span class=\"transchange\"><span>");
				}
			}
			else if (tag.isEndTag()) { //all hi replacements are html spans
				buf.append("</span></span>");
			}
		}
		// <q> quote
		else if (!strcmp(tag.getName(), "q")) {
			SWBuf type = tag.getAttribute("type");
			SWBuf who = tag.getAttribute("who");
			const char *lev = tag.getAttribute("level");
			int level = (lev) ? atoi(lev) : 1;

			if ((!tag.isEndTag()) && (!tag.isEmpty())) {
				myUserData->quote.who = who;
 				
				if(osisQToTick) //alternate " and '
 					buf.append((level % 2) ? '\"' : '\'');

				if (who == "Jesus") {
					buf.append("<span class=\"jesuswords\">");
				}
			}
			else if (tag.isEndTag()) {
        if (myUserData->quote.who == "Jesus") {
          buf.append("</span>");
				}
        
 				if (osisQToTick) { //alternate " and '
 					buf.append((level % 2) ? '\"' : '\'');
 				}
				
				myUserData->quote.who = "";
			}
		}
 		// <milestone> tag
		else if (!strcmp(tag.getName(), "milestone")) {
			const SWBuf type = tag.getAttribute("type");
			if ((type == "screen") || (type == "line")) {//line break
				buf.append("<br/>");
				userData->supressAdjacentWhitespace = true;
			}
		}   
		else { //all tokens handled by OSISHTMLHref will run through the filter now
      return sword::OSISHTMLHREF::handleToken(buf, token, userData);
    }
	}
  return false;
}
