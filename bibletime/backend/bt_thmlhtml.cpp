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

#include <stdlib.h>

#include "bt_thmlhtml.h"

BT_ThMLHTML::BT_ThMLHTML() {
	setTokenStart("<");
	setTokenEnd(">");

	setTokenCaseSensitive(true);

	addTokenSubstitute("note", " <small>(");
	addTokenSubstitute("/note", ")</small> ");

//	addTokenSubstitute("foreign lang=\"el\"", "<font face=\"SIL Galatia\">");
//	addTokenSubstitute("foreign lang=\"he\"", "<font face=\"SIL Ezra\">");
//	addTokenSubstitute("/foreign",						"</font>");

}

bool BT_ThMLHTML::handleToken(char **buf, const char *token, DualStringMap &userData) {
	unsigned long i;
	if (!substituteToken(buf, token)) {
	// manually process if it wasn't a simple substitution
		if (!strncmp(token, "sync type=\"Strongs\" value=\"H\"", 29)) {
			pushString(buf," <font color=\"%s%s",strongs_color,"\"><small><em><a href=\"strongs://OT/");
			for (i = 5; i < strlen(token)-1; i++)
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "\">&lt;");
			for (i = 28; i < strlen(token)-2; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];		
			pushString(buf, "&gt;</a></em></small></font> ");
		}
#warning not handled: token[27] == 'A')
		if (!strncmp(token, "sync type=\"Strongs\" value=\"G\"",29)) {
			pushString(buf," <font color=\"%s%s",strongs_color,"\"><small><em><a href=\"strongs://NT/");
			for (i = 5; i < strlen(token)-1; i++)
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "\">&lt;");
			for (i = 28; i < strlen(token)-2; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];		
			pushString(buf, "&gt;</a></em></small></font> ");
		}
		else if (!strncmp(token, "sync type=\"Morph\" value=\"", 25)) {
#warning OT or NT as default???
			pushString(buf," <font color=\"%s%s",morph_color,"\"><small><em><a href=\"morph://OT/");
			for (i = 5; i < strlen(token)-1; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			pushString(buf, "\">(");
			for (i = 28; i < strlen(token)-2; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			pushString(buf, ")</a></em></small></font>");
		}

		else if (!strncmp(token, "scripRef p", 10) || !strncmp(token, "scripRef v", 10)) {
			userData["inscriptRef"] = "true";
#warning make color customizable
			pushString(buf, "<a href=\"sword://Bible/");
			for (i = 9; i < strlen(token)-1; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];
			pushString(buf,"\">");
		}

		// we're starting a scripRef like "<scripRef>John 3:16</scripRef>"
		else if (!strcmp(token, "scripRef")) {
			userData["inscriptRef"] = "false";
			// let's stop text from going to output
			userData["suspendTextPassThru"] = "true";
		}

		// we've ended a scripRef
		else if (!strcmp(token, "/scripRef")) {
			if (userData["inscriptRef"] == "true") { // like  "<scripRef passage="John 3:16">John 3:16</scripRef>"
				userData["inscriptRef"] = "false";
				pushString(buf, "</a>");
			}
			
			else { // like "<scripRef>John 3:16</scripRef>"
				pushString(buf, "<a href=\"sword://Bible/");
				pushString(buf, userData["lastTextNode"].c_str());
				pushString(buf, "\">");
				pushString(buf, userData["lastTextNode"].c_str());
				// let's let text resume to output again
				userData["suspendTextPassThru"] = "false";	
				pushString(buf, "</a>");
			}
		}
			
		else if (!strncmp(token, "div class=\"sechead\"", 19)) {
			userData["SecHead"] = "true";
			pushString(buf, "<br /><b><i>");
		}
		else if (!strncmp(token, "div class=\"title\"", 19)) {
		        userData["SecHead"] = "true";
			pushString(buf, "<br /><b><i>");
		}

		else if (!strncmp(token, "/div", 4)) {
			if (userData["SecHead"] == "true") {
				pushString(buf, "</i></b><br />");
				userData["SecHead"] = "false";
			}
		}
		else if(!strncmp(token, "note", 4)) {
			pushString(buf, "<small>(");
		}
		else { // let token pass thru
			*(*buf)++ = '<';
			for (i = 0; i < strlen(token); i++)
				*(*buf)++ = token[i];
				*(*buf)++ = '>';
		}
	}
	return true;
}