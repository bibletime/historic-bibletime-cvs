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

//Sword includes
#include <swmodule.h>

//Qt includes
#include <qstring.h>

BT_ThMLHTML::BT_ThMLHTML() {
	setEscapeStringCaseSensitive(true);
	setPassThruUnknownEscapeString(true); //the HTML widget will render the HTML escape codes	
	setEscapeStart("&");
	setEscapeEnd(";");	
  addEscapeStringSubstitute("raquo", QString::fromLatin1("»").utf8());
  addEscapeStringSubstitute("laquo", QString::fromLatin1("«").utf8());
//  addEscapeStringSubstitute("uuml", QString::fromLatin1("ü").utf8());
//  addEscapeStringSubstitute("ouml", QString::fromLatin1("ö").utf8());
//  addEscapeStringSubstitute("auml", QString::fromLatin1("ä").utf8());
	
	setTokenStart("<");
	setTokenEnd(">");
	setTokenCaseSensitive(true);
	addTokenSubstitute("note", " <span id=\"footnote\">(");
	addTokenSubstitute("/note", ")</span> ");

	addTokenSubstitute("foreign lang=\"el\"", "<span lang=\"el\">");
	addTokenSubstitute("foreign lang=\"he\"", "<span lang=\"he\" dir=\"rtl\">");
	addTokenSubstitute("/foreign",						"</span>");
}

bool BT_ThMLHTML::handleToken(char **buf, const char *token, DualStringMap &userData) {
	unsigned long i = 0;
	const int tokenLength = strlen(token);
	
	if (!substituteToken(buf, token) && !substituteEscapeString(buf, token)) {

		if (!strncmp(token, "sync type=\"lemma\"", 17)) { //LEMMA
			pushString(buf," <span id=\"lemma\">&lt;");

			for (int j = 17; j < tokenLength; j++) {
				if (!strncmp(token+j, "value=\"", 7)) {
					j += 7;
					for (;token[j] != '\"'; j++)
						*(*buf)++ = token[j];
					break;
				}
			}
			pushString(buf, "&gt;</span> ");
		}

		else if (!strncmp(token, "sync type=\"morph\"", 17)) { //Morph
			char num[12];
			for (int j = 17; j < tokenLength; j++) {
				if (!strncmp(token+j, "value=\"", 7)) {
					j += 7;
					int idx=0;
					for (;token[j] != '\"'; j++,idx++)
						num[idx] = token[j];
					num[idx] = 0;
					break;
				}
			}
			pushString(buf," <span id=\"morphcode\"><a href=\"morph://Greek/%s\">(%s)</a></span> ",
				num, num);
		}
		
		else if (!strncmp(token, "sync type=\"Strongs\" value=\"H\"", 29)) {
			char num[12];
			for (i = 29; i < tokenLength; i++)
				if(token[i] != '\"')
					num[i-29] = token[i];
			num[i-29] = 0;

			pushString(buf," <span id=\"strongnumber\"><a href=\"strongs://Hebrew/%s\">&lt;%s&gt;</a></span> ",
				num, num);
		}
		else if (!strncmp(token, "sync type=\"Strongs\" value=\"G\"",29)) {
			char num[12];
			for (i = 29; i < tokenLength; i++)
				if(token[i] != '\"')
					num[i-29] = token[i];
			num[i-29] = 0;

			pushString(buf," <span id=\"strongnumber\"><a href=\"strongs://Greek/%s\">&lt;%s&gt;</a></em></small></font> ",
				num, num);
		}

		else if (!strncmp(token, "scripRef p", 10) || !strncmp(token, "scripRef v", 10)) {
			userData["inscriptRef"] = "true";
			if (!strncmp(token, "scripRef v", 10)) { //module given

				char module_version[500];
				for (i = 18; i < tokenLength-1; i++) {
					if(token[i] != '\"')
						module_version[i-18] = token[i];						
					else
						break;
				}
				module_version[i-18] = 0;
				//c contains the module
				userData["lastRefModule"] = module_version;
			}
			else if (!strncmp(token, "scripRef p", 10)) { //passage without module
				char verse_str[5000];
				for (i = 18; i < tokenLength-1; i++) {
					if(token[i] != '\"')
						verse_str[i-18] = token[i];
					else
						break;
				}
				verse_str[i-18] = 0;
				const char* ref = parseThMLRef(verse_str);
 			  pushString(buf, ref);
 			  delete [] ref;//delete now because it's unused
			}
			if ( !strncmp(token+i+2, "passage=", 8) ) { //passage after module part
				char verse_str[5000];
				i+=11;			
				int idx = 0;	
				for (; i < tokenLength-1; i++,idx++)	{
					if(token[i] != '\"')
						verse_str[idx] = token[i];
					else
						break;					
				}
				verse_str[idx] = '\0';
				const char* ref = parseThMLRef(verse_str, userData["lastRefModule"].c_str());
				pushString(buf, ref);
				delete [] ref;
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
			}			
			else { // like "<scripRef>John 3:16</scripRef>"
				const char* ref = parseSimpleRef(userData["lastTextNode"].c_str());
 			  pushString(buf, ref);
 			  delete [] ref;//delete now because it's unused
				userData["suspendTextPassThru"] = "false";
			}
		}			
		
//headings should be processed by the ThMLHeadings filter		
		else if (!strncmp(token, "div class=\"sechead\"", 19)) {
			userData["SecHead"] = "true";
			pushString(buf, "<div id=\"sectiontitle\">"/*, text_color*/);
		}
		else if (!strncmp(token, "div class=\"title\"", 19)) {
      userData["Title"] = "true";
			pushString(buf, "<div id=\"booktitle\">");
		}
//		else if (!strncmp(token, "/div", 4)) {
//			if (userData["SecHead"] == "true") {
//				pushString(buf, "</FONT></H2>");
//				userData["SecHead"] = "false";
//			}
//			else if(userData["Title"] == "true") {
//				pushString(buf, "</FONT></H1>");
//				userData["Title"] = "false";
//			}
//		}
		else if (!strncmp(token, "img ", 4)) {
			const char *src = strstr(token, "src");
			if (!src)		// assert we have a src attribute
				return false;

			*(*buf)++ = '<';
			for (const char *c = token; *c; c++) {
				if (c == src) {
					for (;((*c) && (*c != '"')); c++)
						*(*buf)++ = *c;

					if (!*c) { c--; continue; }

					*(*buf)++ = '"';
					if (*(c+1) == '/') {
						pushString(buf, "file:");
						pushString(buf, module->getConfigEntry("AbsoluteDataPath"));
						if (*(*buf-1) == '/')
							c++;		// skip '/'
					}
					continue;
				}
				*(*buf)++ = *c;
			}
			*(*buf)++ = '>';
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