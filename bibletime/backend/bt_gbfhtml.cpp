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

//Qt includes
#include <qregexp.h>
#include <qstring.h>


BT_GBFHTML::BT_GBFHTML(){
  setTokenStart("<");
	setTokenEnd(">");
	
	setTokenCaseSensitive(true);

//	addTokenSubstitute("Rf", ")</small></font>");// end of footnote
	addTokenSubstitute("Rf", ")</span>");// end of footnote

	addTokenSubstitute("FI", "<i>"); // italics begin
	addTokenSubstitute("Fi", "</i>");
  
	addTokenSubstitute("FB", "<b>"); // bold begin
	addTokenSubstitute("Fb", "</b>");
	
//	addTokenSubstitute("FR", QString::fromLatin1("<font color=\"%1\">").arg(jesuswords_color).latin1());		
	addTokenSubstitute("FR", "<span class=\"jesuswords\">");
	addTokenSubstitute("Fr", "</span>");
  
	addTokenSubstitute("FU", "<u>"); // underline begin
	addTokenSubstitute("Fu", "</u>");

//	addTokenSubstitute("FO", "<cite>"); //  Old Testament quote begin
//	addTokenSubstitute("Fo", "</cite>");
	addTokenSubstitute("FO", "<span class=\"quotation\">"); //  Old Testament quote begin
	addTokenSubstitute("Fo", "</span>");
	

  addTokenSubstitute("FS", "<sup>"); // Superscript begin// Subscript begin
	addTokenSubstitute("Fs", "</sup>");

  addTokenSubstitute("FV", "<sub>"); // Subscript begin
	addTokenSubstitute("Fv", "</sub>");

//	addTokenSubstitute("TT", QString::fromLatin1(" <h1><font color=\"%1\">").arg(text_color).local8Bit());
//	addTokenSubstitute("Tt", "</font></h1>");
	addTokenSubstitute("TT", "<div class=\"booktitle\">");
	addTokenSubstitute("Tt", "</div>");
	
	addTokenSubstitute("TS", "<div class=\"sectiontitle\">");
	addTokenSubstitute("Ts", "</div>");
			
	addTokenSubstitute("PP", "<span class=\"poetry\">"); //  poetry  begin
	addTokenSubstitute("Pp", "</span>");

	addTokenSubstitute("Fn", "</font>"); //  font  end
	addTokenSubstitute("CL", "<br>"); //  new line
	addTokenSubstitute("CM", "<!p><br>"); //  paragraph <!P> is a non showing comment that can be changed in the front end to <P> if desired

  addTokenSubstitute("CG", "&gt;"); // literal greater-than sign
	addTokenSubstitute("CT", "&lt;"); // literal less-than sign

  addTokenSubstitute("JR", "<span align=\"right\">"); // right align begin
	addTokenSubstitute("JC", "<span align=\"center\">"); // center align begin
	addTokenSubstitute("JL", "</span>"); // align end
}

/** No descriptions */
char BT_GBFHTML::processText(sword::SWBuf& buf, const sword::SWKey * key, const sword::SWModule * module){
	BT_BASICFILTER::processText(buf, key, module);

  CSwordModuleInfo* const mod = CPointers::backend()->findModuleByPointer(m_module);
  if (!mod || (mod && (mod->type() != CSwordModuleInfo::Bible) && (mod->type() != CSwordModuleInfo::GenericBook))) {
    BT_BASICFILTER::ProcessRWPRefs(buf);
  }

  return 1;
}


bool BT_GBFHTML::handleToken(sword::SWBuf& buf, const char *token, DualStringMap &userData) {
	if (!substituteToken(buf, token)) {  //more than a simple replace
  	const int tokenLength = strlen(token);
		unsigned long i;

//    char *valto;
//	  const char *num;
//    char val[128];
	
 		if (!strncmp(token, "note ", 5)) {
			// let's stop text from going to output
			userData["suspendTextPassThru"] = "true";
		}

		else if (!strncmp(token, "/note", 5)) {
			userData["suspendTextPassThru"] = "false";
		}

//		else if (!strncmp(token, "w", 1)) {
//			// OSIS Word (temporary until OSISRTF is done)
//			valto = val;
//			num = strstr(token, "lemma=\"x-Strongs:");
//			if (num) {
//				for (num+=17; ((*num) && (*num != '\"')); num++)
//					*valto++ = *num;
//				*valto = 0;
//				if (atoi((!isdigit(*val))?val+1:val) < 5627) {
//					buf += " <small><em>&lt;";
//					for (tok = (!isdigit(*val))?val+1:val; *tok; tok++)
//							buf += *tok;
//					buf += "&gt;</em></small> ";
//				}
//			}
//			valto = val;
//			num = strstr(token, "morph=\"x-Robinson:");
//			if (num) {
//				for (num+=18; ((*num) && (*num != '\"')); num++)
//					*valto++ = *num;
//				*valto = 0;
//				// normal robinsons tense
//				buf += " <small><em>(";
//				for (tok = val; *tok; tok++)
//						buf += *tok;
//				buf += ")</em></small> ";
//			}
//		}
    
		if (!strncmp(token, "WG", 2)){ // strong's numbers greek
			char num[6];
			for (i = 2; i < tokenLength; i++)
					num[i-2] = token[i];
			num[i-2]=0;

			buf.appendFormatted(" <a href=\"strongs://Greek/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span></a> ",
				num, num);
		}

		if (!strncmp(token, "WH", 2)){ // strong's numbers hebrew
			char num[6];
			for (i = 2; i < tokenLength; i++)
					num[i-2] = token[i];
			num[i-2]=0;

			buf.appendFormatted(" <a href=\"strongs://Hebrew/%s\"><span class=\"strongnumber\">&lt;%s&gt;</span> </a>",
				num, num);
		}

		else if (!strncmp(token, "WTG", 3)) { // strong's numbers tense greek
			char num[16];
			for (i = 3; i < tokenLength; i++)
					num[i-3] = token[i];
			num[i-3]=0;

			buf.appendFormatted(" <a href=\"morph://Greek/%s\"><span class=\"morphcode\">(%s)</span></a> ",
				num, num);
		}

		else if (!strncmp(token, "WTH", 3)) { // strong's numbers tense hebrew

			char num[6];
			for (i = 3; i < tokenLength; i++)
					num[i-3] = token[i];
			num[i-3]=0;

			buf.appendFormatted(" <a href=\"morph://Hebrew/%s\"><span class=\"morphcode\">(%s)</span></a> ",
				num, num);
		}

		else if (!strncmp(token, "RB", 2)) {
			buf += "<span class=\"footnotepre\">";
			userData["hasFootnotePreTag"] = "true";
		}

		else if (!strncmp(token, "RF", 2)) {
			if(userData["hasFootnotePreTag"] == "true") {
				userData["hasFootnotePreTag"] = "false";
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
			return false;
		}
	}
	return true;
}

