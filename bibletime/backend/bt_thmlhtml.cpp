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
#include "versekey.h"

//Qt includes
#include <qstring.h>

BT_ThMLHTML::BT_ThMLHTML() {
	setTokenStart("<");
	setTokenEnd(">");

//	setEscapeStart("&");
//	setEscapeEnd(";");
//  addEscapeStringSubstitute("auml", "ä");
//  addEscapeStringSubstitute("uuml", "ü");
//  addEscapeStringSubstitute("ouml", "ö");
  		
	setTokenCaseSensitive(true);

	addTokenSubstitute("note", " <small>(");
	addTokenSubstitute("/note", ")</small> ");

//	addTokenSubstitute("foreign lang=\"el\"", "<font face=\"SIL Galatia\">");
//	addTokenSubstitute("foreign lang=\"he\"", "<font face=\"SIL Ezra\">");
//	addTokenSubstitute("/foreign",						"</font>");

}

bool BT_ThMLHTML::handleToken(char **buf, const char *token, DualStringMap &userData) {
	unsigned long i;
	const int tokenLength = strlen(token);	
	
	if (!substituteToken(buf, token) || !substituteEscapeString(buf, token)) {

		if (!strncmp(token, "sync type=\"lemma\"", 17)) { //LEMMA
//			pushString(buf," <font color=\"%s%s",strongs_color,"\"><small><em>&lt;");
			pushString(buf," <small><em>&lt;");

			for (unsigned int j = 17; j < tokenLength; j++) {
				if (!strncmp(token+j, "value=\"", 7)) {
					j += 7;
					for (;token[j] != '\"'; j++)
						*(*buf)++ = token[j];
					break;
				}
			}
//			pushString(buf, "&gt;</em></small></font> ");
			pushString(buf, "&gt;</em></small> ");
		}

		else if (!strncmp(token, "sync type=\"morph\"", 17)) { //Morph
			char** oldbuf = buf;
			pushString(buf," <font color=\"%s%s",morph_color,"\"><small><em><a href=\"morph://Greek/");

			for (unsigned int j = 17; j < tokenLength; j++) {
				if (!strncmp(token+j, "value=\"", 7)) {
					j += 7;
					for (;token[j] != '\"'; j++)
						*(*buf)++ = token[j];
					break;
				}
			}
			*(*buf)++ = '"';
			*(*buf)++ = '>';
			*(*buf)++ = '(';
			for (unsigned int j = 17; j < tokenLength; j++) {
				if (!strncmp(token+j, "value=\"", 7)) {
					j += 7;
					for (;token[j] != '\"'; j++)
						*(*buf)++ = token[j];
					break;
				}
			}
			pushString(buf, ")</a></em></small></font> ");
		}
		
		else if (!strncmp(token, "sync type=\"Strongs\" value=\"H\"", 29)) {
			pushString(buf," <font color=\"%s%s",strongs_color,"\"><small><em><a href=\"strongs://Hebrew/");
			for (i = 5; i < tokenLength-1; i++)
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "\">&lt;");
			for (i = 28; i < tokenLength-2; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];		
			pushString(buf, "&gt;</a></em></small></font> ");
		}
//#warning not handled: token[27] == 'A')
		else if (!strncmp(token, "sync type=\"Strongs\" value=\"G\"",29)) {
			pushString(buf," <font color=\"%s%s",strongs_color,"\"><small><em><a href=\"strongs://Greek/");
			for (i = 5; i < tokenLength-1; i++)
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf, "\">&lt;");
			for (i = 28; i < tokenLength-2; i++)				
				if(token[i] != '\"') 			
					*(*buf)++ = token[i];		
			pushString(buf, "&gt;</a></em></small></font> ");
		}
//		else if (!strncmp(token, "sync type=\"Morph\" value=\"", 25)) {
//#warning OT or NT as default???
//			pushString(buf," <font color=\"%s%s",morph_color,"\"><small><em><a href=\"morph://Hebrew/");
//			for (i = 5; i < strlen(token)-1; i++)				
//				if(token[i] != '\"') 			
//					*(*buf)++ = token[i];
//			pushString(buf, "\">(");
//			for (i = 28; i < strlen(token)-2; i++)				
//				if(token[i] != '\"') 			
//					*(*buf)++ = token[i];
//			pushString(buf, ")</a></em></small></font>");
//		}

		else if (!strncmp(token, "scripRef p", 10) || !strncmp(token, "scripRef v", 10)) {
			userData["inscriptRef"] = "true";
			if (!strncmp(token, "scripRef v", 10)) { //module given
				char* module_version =  new char[5000];
				char* c = module_version;						
				for (i = 18; i < tokenLength-1; i++) {
					if(token[i] != '\"')
						*c++ = token[i];						
					else
						break;
				}
				*c++ = '\0';
				//c contains the module
				userData["lastRefModule"] = module_version;
			  delete module_version;
			}
			else if (!strncmp(token, "scripRef p", 10)) { //passage without module
				char* verse_str =  new char[5000];
				char* c = verse_str;							
				for (i = 18; i < tokenLength-1; i++) {
					if(token[i] != '\"') {
						*c++ = token[i];
					}
					else
						break;
				}
				*c++ = '\0';
				
				const char* ref = parseThMLRef(verse_str);
 			  pushString(buf, ref);
 			  delete ref;
 			  delete verse_str;
			}
			if ( !strncmp(token+i+2, "passage=", 8) ) { //passage after module part
				char* verse_str =  new char[5000];
				char* c = verse_str;							
				i+=11;				
				for (; i < tokenLength-1; i++)	{
					if(token[i] != '\"')
						*c++ = token[i];
					else
						break;					
				}
				*c++ = '\0';				
				const char* mod = userData["lastRefModule"].c_str();
				cout << "Module is: " << mod << endl;
				const char* ref = parseThMLRef(verse_str, mod);
				pushString(buf, ref);
				delete ref;
				delete verse_str;
			}
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
				pushString(buf, thmlRefEnd());
//				userData["suspendTextPassThru"] = "false"; 							
			}			
			else { // like "<scripRef>John 3:16</scripRef>"
				const char* ref = parseSimpleRef(userData["lastTextNode"].c_str());
 			  pushString(buf, ref);
 			  delete ref;
				// let's let text resume to output again
				userData["suspendTextPassThru"] = "false";
			}
		}			
		else if (!strncmp(token, "div class=\"sechead\"", 19)) {
			userData["SecHead"] = "true";
			pushString(buf, "<H2><FONT color=\"%s\">", text_color);
		}
		else if (!strncmp(token, "div class=\"title\"", 19)) {
      userData["Title"] = "true";
			pushString(buf, "<H1><FONT color=\"%s\">", text_color);
		}
		else if (!strncmp(token, "/div", 4)) {
			if (userData["SecHead"] == "true") {
				pushString(buf, "</FONT></H2>");
				userData["SecHead"] = "false";
			}
			else if(userData["Title"] == "true") {
				pushString(buf, "</FONT></H1>");
				userData["Title"] = "false";
			}
		}
		else if(!strncmp(token, "note", 4)) {
			pushString(buf, "<small>(");
		}
		else { // let token pass thru
			*(*buf)++ = '<';
			for (i = 0; i < tokenLength; i++)
				*(*buf)++ = token[i];
				*(*buf)++ = '>';
		}
	}
	return true;
}