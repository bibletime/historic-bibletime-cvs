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
#include <string.h>
#include "bt_gbfhtml.h"


BT_GBFHTML::BT_GBFHTML(){
  setTokenStart("<");
	setTokenEnd(">");

	setTokenCaseSensitive(true);

	addTokenSubstitute("Rf", ")</SMALL></FONT>");
	addTokenSubstitute("FI", "<I>"); // italics begin
	addTokenSubstitute("Fi", "</I>");
	addTokenSubstitute("FB", "<B>"); // bold begin
	addTokenSubstitute("Fb", "</B>");
#warning make configurable
	addTokenSubstitute("FR", "<FONT COLOR=\"#FF0000\">"); // words of Jesus begin
	addTokenSubstitute("Fr", "</FONT>");
	addTokenSubstitute("FU", "<U>"); // underline begin
	addTokenSubstitute("Fu", "</U>");
	addTokenSubstitute("FO", "<CITE>"); //  Old Testament quote begin
	addTokenSubstitute("Fo", "</CITE>");
	addTokenSubstitute("FS", "<SUP>"); // Superscript begin// Subscript begin
	addTokenSubstitute("Fs", "</SUP>");
	addTokenSubstitute("FV", "<SUB>"); // Subscript begin
	addTokenSubstitute("Fv", "</SUB>");
	addTokenSubstitute("TT", "<BIG>"); // Book title begin
	addTokenSubstitute("Tt", "</BIG>");
	addTokenSubstitute("PP", "<CITE>"); //  poetry  begin
	addTokenSubstitute("Pp", "</CITE>");
	addTokenSubstitute("Fn", "</FONT>"); //  font  end
	addTokenSubstitute("CL", "<BR>"); //  new line
	addTokenSubstitute("CM", "<!P><BR>"); //  paragraph <!P> is a non showing comment that can be changed in the front end to <P> if desired
	addTokenSubstitute("CG", ""); //  ???
	addTokenSubstitute("CT", ""); // ???
	addTokenSubstitute("JR", "<DIV ALIGN=\"RIGHT\">"); // right align begin
	addTokenSubstitute("JC", "<DIV ALIGN=\"CENTER\">"); // center align begin
	addTokenSubstitute("JL", "</DIV>"); // align end
}

bool BT_GBFHTML::handleToken(char **buf, const char *token, DualStringMap &userData) {
	unsigned long i;
	if (!substituteToken(buf, token)) {  //more than a simple replace

		if (!strncmp(token, "WG", 2)){ // strong's numbers greek
			pushString(buf, " <small><em><a href=\"sword://strongs_greek/");
			for (i = 1; i < strlen(token); i++)
				//if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "\">&lt;");
//			*(*buf)++ = '\"';
//			*(*buf)++ = '>';
			for (i = 2; i < strlen(token); i++)
				//if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "&gt;</A></em></small> ");
		}

		if (!strncmp(token, "WH", 2)){ // strong's numbers hebrew
			pushString(buf, " <small><em><a href=\"sword://strongs_hebrew/");
			for (i = 1; i < strlen(token); i++)
				//if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "\">&lt;");
//			*(*buf)++ = '\"';
//			*(*buf)++ = '>';
			for (i = 2; i < strlen(token); i++)
				//if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "&gt;</A></em></small> ");
		}

		else if (!strncmp(token, "WTG", 3)) { // strong's numbers tense greek
			pushString(buf, " <small><em><a href=\"sword://morph_greek/");
			for (i = 2; i < strlen(token); i++)				
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "\">(");
//			*(*buf)++ = '\"';
//			*(*buf)++ = '>';
			for (i = 3; i < strlen(token); i++)
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			pushString(buf, ")</A></EM></SMALL>");
		}

		else if (!strncmp(token, "WTH", 3)) { // strong's numbers tense hebrew
			pushString(buf, " <small><em><a href=\"sword://morph_hebrew/");
			for (i = 2; i < strlen(token); i++)				
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "\">(");
//			*(*buf)++ = '\"';
//			*(*buf)++ = '>';
			for (i = 3; i < strlen(token); i++)
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			pushString(buf, ")</A></EM></SMALL>");
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
			pushString(buf, "<I>");
			userData["hasFootnotePreTag"] = "true";
		}

		else if (!strncmp(token, "RF", 2)) {
			if(userData["hasFootnotePreTag"] == "true") {
				userData["hasFootnotePreTag"] = "false";
				pushString(buf, "</I> ");
			}
			pushString(buf, "<FONT COLOR=\"#800000\"><SMALL> (");
		}

		else if (!strncmp(token, "FN", 2)) {
			pushString(buf, "<FONT FACE=\"");
			for (i = 2; i < strlen(token); i++)				
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			*(*buf)++ = '\"';
			*(*buf)++ = '>';
		}

		else if (!strncmp(token, "CA", 2)) {	// ASCII value
			*(*buf)++ = (char)atoi(&token[2]);
		}
		
		else {
			return false;
		}
	}
	return true;
}
