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

#include <stdlib.h>

#include "bt_gbfhtml.h"

BT_GBFHTML::BT_GBFHTML(){
  setTokenStart("<");
	setTokenEnd(">");

	setTokenCaseSensitive(true);

	addTokenSubstitute("Rf", ")</small></font>");// end of footnote
	addTokenSubstitute("FI", "<i>"); // italics begin
	addTokenSubstitute("Fi", "</i>");
	addTokenSubstitute("FB", "<b>"); // bold begin
	addTokenSubstitute("Fb", "</b>");
	addTokenSubstitute("Fr", "</font>");
	addTokenSubstitute("FU", "<u>"); // underline begin
	addTokenSubstitute("Fu", "</u>");
	addTokenSubstitute("FO", "<cite>"); //  Old Testament quote begin
	addTokenSubstitute("Fo", "</cite>");
	addTokenSubstitute("FS", "<sup>"); // Superscript begin// Subscript begin
	addTokenSubstitute("Fs", "</sup>");
	addTokenSubstitute("FV", "<sub>"); // Subscript begin
	addTokenSubstitute("Fv", "</sub>");
	
	addTokenSubstitute("TT", "<big>"); // Book title begin
	addTokenSubstitute("Tt", "</big>");
	addTokenSubstitute("TS", "<H2><FONT color=\"black\">"); // Section title begin
	addTokenSubstitute("Ts", "</FONT></H2>");
	
	addTokenSubstitute("PP", "<cite>"); //  poetry  begin
	addTokenSubstitute("Pp", "</cite>");
	addTokenSubstitute("Fn", "</font>"); //  font  end
	addTokenSubstitute("CL", "<br>"); //  new line
	addTokenSubstitute("CM", "<!p><br>"); //  paragraph <!P> is a non showing comment that can be changed in the front end to <P> if desired
	addTokenSubstitute("CG", ">"); // literal greater-than sign
	addTokenSubstitute("CT", "<"); // literal less-than sign
	addTokenSubstitute("JR", "<div align=\"right\">"); // right align begin
	addTokenSubstitute("JC", "<div align=\"center\">"); // center align begin
	addTokenSubstitute("JL", "</div>"); // align end
}

bool BT_GBFHTML::handleToken(char **buf, const char *token, DualStringMap &userData) {
	unsigned long i;
	if (!substituteToken(buf, token)) {  //more than a simple replace

		if (!strncmp(token, "WG", 2)){ // strong's numbers greek
			pushString(buf," <font color=\"%s%s",strongs_color,"\"><small><em><a href=\"strongs://NT/");
			for (i = 2; i < strlen(token); i++)
					*(*buf)++ = token[i];
			pushString(buf, "\">&lt;");
			for (i = 2; i < strlen(token); i++)
					*(*buf)++ = token[i];
			pushString(buf, "&gt;</A></em></small></font> " );
		}

		if (!strncmp(token, "WH", 2)){ // strong's numbers hebrew
			pushString(buf," <font color=\"%s%s",strongs_color,"\"><small><em><a href=\"strongs://OT/");
			for (i = 2; i < strlen(token); i++)
					*(*buf)++ = token[i];
			pushString(buf, "\">&lt;");
			for (i = 2; i < strlen(token); i++)
					*(*buf)++ = token[i];
			pushString(buf, "&gt;</A></em></small></font> ");
		}

		else if (!strncmp(token, "WTG", 3)) { // strong's numbers tense greek
			pushString(buf," <font color=\"%s%s",morph_color,"\"><small><em><a href=\"morph://NT/");
			for (i = 2; i < strlen(token); i++)				
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "\">(");
			for (i = 3; i < strlen(token); i++)
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			pushString(buf, ")</a></em></small></font>");
		}

		else if (!strncmp(token, "WTH", 3)) { // strong's numbers tense hebrew
			pushString(buf," <font color=\"%s%s",morph_color,"\"><small><em><a href=\"morph://OT/");
			for (i = 2; i < strlen(token); i++)				
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "\">(");
			for (i = 3; i < strlen(token); i++)
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			pushString(buf, ")</a></em></small></font>");
		}

//		else if (!strncmp(token, "WT", 2)) { // morph tags
//			pushString(buf, " <small><em>(<a href=\"sword://morph_/" ERROR);
//			for (i = 2; i < strlen(token); i++)				
//				if(token[i] != '\"')
//					*(*buf)++ = token[i];
//			*(*buf)++ = '\"';
//			*(*buf)++ = '>';
//			for (i = 2; i < strlen(token); i++)
//				if(token[i] != '\"') 			
//					*(*buf)++ = token[i];
//			pushString(buf, "</A>)</EM></SMALL>");
//		}

		else if (!strncmp(token, "RB", 2)) {
			pushString(buf, "<i>");
			userData["hasFootnotePreTag"] = "true";
		}

		else if (!strncmp(token, "RF", 2)) {
			if(userData["hasFootnotePreTag"] == "true") {
				userData["hasFootnotePreTag"] = "false";
				pushString(buf, "</i> ");
			}
			pushString(buf,"<font color=\"%s%s",footnote_color,"\"><small> (");
		}

		else if (!strncmp(token, "FN", 2)) {
			pushString(buf, "<font face=\"");
			for (i = 2; i < strlen(token); i++)				
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf,"\">");
		}

		else if (!strncmp(token, "FR", 2))
			pushString(buf, "<font color=\"%s%s",jesuswords_color,"\">");

		else if (!strncmp(token, "CA", 2)) {	// ASCII value
			*(*buf)++ = (char)atoi(&token[2]);
		}
		
		else {
			return false;
		}
	}
	return true;
}
