/***************************************************************************
                          gbfhtml.cpp  -  description
                             -------------------
    begin                : Thu Jun 24 1999
    copyright            : (C) 1999 by Torsten Uhlmann
    email                : TUhlmann@gmx.de
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
#include "cswordmoduleinfo.h"
#include "cswordbackend.h"
#include "util/cpointers.h"

#include "bt_gbfhtml.h"

//sytsme includes
#include <stdlib.h>
#include <stdio.h>

//Sword includes
#include <utilxml.h>

//Qt includes
#include <qregexp.h>
#include <qstring.h>


BT_GBFHTML::BT_GBFHTML(){
  setTokenStart("<");
	setTokenEnd(">");
	
	setTokenCaseSensitive(true);

	replaceTokenSubstitute("Rf", ")</span>");// end of footnote

	replaceTokenSubstitute("FI", "<i>"); // italics begin
	replaceTokenSubstitute("Fi", "</i>");

	replaceTokenSubstitute("FB", "<b>"); // bold begin
	replaceTokenSubstitute("Fb", "</b>");

	replaceTokenSubstitute("FR", "<span class=\"jesuswords\">");
	replaceTokenSubstitute("Fr", "</span>");

	replaceTokenSubstitute("FU", "<u>"); // underline begin
	replaceTokenSubstitute("Fu", "</u>");

//	replaceTokenSubstitute("FO", "<cite>"); //  Old Testament quote begin
//	replaceTokenSubstitute("Fo", "</cite>");
	replaceTokenSubstitute("FO", "<span class=\"quotation\">"); //  Old Testament quote begin
	replaceTokenSubstitute("Fo", "</span>");


  replaceTokenSubstitute("FS", "<sup>"); // Superscript begin// Subscript begin
	replaceTokenSubstitute("Fs", "</sup>");

  replaceTokenSubstitute("FV", "<sub>"); // Subscript begin
	replaceTokenSubstitute("Fv", "</sub>");

//	replaceTokenSubstitute("TT", QString::fromLatin1(" <h1><font color=\"%1\">").arg(text_color).local8Bit());
//	replaceTokenSubstitute("Tt", "</font></h1>");
	replaceTokenSubstitute("TT", "<div class=\"booktitle\">");
	replaceTokenSubstitute("Tt", "</div>");

	replaceTokenSubstitute("TS", "<div class=\"sectiontitle\">");
	replaceTokenSubstitute("Ts", "</div>");

	replaceTokenSubstitute("PP", "<span class=\"poetry\">"); //  poetry  begin
	replaceTokenSubstitute("Pp", "</span>");

	replaceTokenSubstitute("Fn", "</font>"); //  font  end
	replaceTokenSubstitute("CL", "<br>"); //  new line
	replaceTokenSubstitute("CM", "<!p><br>"); //  paragraph <!P> is a non showing comment that can be changed in the front end to <P> if desired

  replaceTokenSubstitute("CG", "&gt;"); // literal greater-than sign
	replaceTokenSubstitute("CT", "&lt;"); // literal less-than sign

  replaceTokenSubstitute("JR", "<span align=\"right\">"); // right align begin
	replaceTokenSubstitute("JC", "<span align=\"center\">"); // center align begin
	replaceTokenSubstitute("JL", "</span>"); // align end
}

/** No descriptions */
char BT_GBFHTML::processText(sword::SWBuf& buf, const sword::SWKey * key, const sword::SWModule * module){
	GBFHTML::processText(buf, key, module);

  CSwordModuleInfo* const mod = CPointers::backend()->findModuleByPointer(module);
  if (!mod || (mod && (mod->type() != CSwordModuleInfo::Bible) && (mod->type() != CSwordModuleInfo::GenericBook))) {
    ProcessRWPRefs(buf, mod ? mod->module() : 0);
  }

  return 1;
}

bool BT_GBFHTML::handleToken(sword::SWBuf &buf, const char *token, sword::BasicFilterUserData *userData) {
	if (!substituteToken(buf, token)) {  //more than a simple replace
  	const unsigned int tokenLength = strlen(token);
		unsigned long i;
    sword::SWBuf value;
    BT_UserData* myUserData = dynamic_cast<BT_UserData*>(userData);

    if (!strncmp(token, "WG", 2)){ // strong's numbers greek
			for (i = 2; i < tokenLength; i++) {
					value += token[i];
      }
      
			buf.appendFormatted(" <a href=\"strongs://Greek/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span></a> ",
				value.c_str(),
        value.c_str()
      );
		}
		else if (!strncmp(token, "WH", 2)){ // strong's numbers hebrew
			for (i = 2; i < tokenLength; i++) {
					value += token[i];
      }
      
			buf.appendFormatted(" <a href=\"strongs://Hebrew/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span> </a>",
				value.c_str(),
        value.c_str()
      );
		}
		else if (!strncmp(token, "WTG", 3)) { // strong's numbers tense greek
			for (i = 3; i < tokenLength; i++) {
					value += token[i];
      }

			buf.appendFormatted(" <a href=\"morph://Greek/%s\"><span class=\"morphcode\">(%s)</span></a> ",
				value.c_str(),
				value.c_str()
      );
		}

		else if (!strncmp(token, "WTH", 3)) { // strong's numbers tense hebrew
			for (i = 3; i < tokenLength; i++) {
					value += token[i];
      }

			buf.appendFormatted(" <a href=\"morph://Hebrew/%s\"><span class=\"morphcode\">(%s)</span></a> ",
				value.c_str(),
				value.c_str()
      );
		}

		else if (!strncmp(token, "RB", 2)) {
			buf += "<span class=\"footnotepre\">";
			myUserData->hasFootnotePreTag = true;
		}

		else if (!strncmp(token, "RF", 2)) {
			if (myUserData->hasFootnotePreTag) {
				myUserData->hasFootnotePreTag = false;
				buf += "</span> ";
			}
			buf += "<span class=\"footnote\"> (";
		}

		else if (!strncmp(token, "FN", 2)) {
			buf += "<font face=\"";
			for (i = 2; i < tokenLength; i++) {
				if(token[i] != '\"') {
					buf += token[i];
        }
      }
			buf += "\">";
		}
		else if (!strncmp(token, "CA", 2)) {	// ASCII value
			buf += (char)atoi(&token[2]);
		}		
		else {
			return GBFHTML::handleToken(buf, token, userData);
		}
	}
	return true;
}

