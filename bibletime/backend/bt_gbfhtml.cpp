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
#include <qstring.h>

#include <stdlib.h>
#include <stdio.h>

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
	
	addTokenSubstitute("FR", QString::fromLatin1("<font color=\"%1\">").arg(jesuswords_color).latin1());		
	addTokenSubstitute("Fr", "</font>");
	addTokenSubstitute("FU", "<u>"); // underline begin
	addTokenSubstitute("Fu", "</u>");
	addTokenSubstitute("FO", "<cite>"); //  Old Testament quote begin
	addTokenSubstitute("Fo", "</cite>");
	addTokenSubstitute("FS", "<sup>"); // Superscript begin// Subscript begin
	addTokenSubstitute("Fs", "</sup>");
	addTokenSubstitute("FV", "<sub>"); // Subscript begin
	addTokenSubstitute("Fv", "</sub>");

	addTokenSubstitute("TT", QString::fromLatin1(" <h1><font color=\"%1\">").arg(text_color).local8Bit());
	addTokenSubstitute("Tt", "</font></h1>");
	
	addTokenSubstitute("Ts", "</font></h2>");
	addTokenSubstitute("TS", QString::fromLatin1(" <H2><font color=\"%1\">").arg(text_color).local8Bit());	
			
	addTokenSubstitute("PP", "<cite>"); //  poetry  begin
	addTokenSubstitute("Pp", "</cite>");
	addTokenSubstitute("Fn", "</font>"); //  font  end
	addTokenSubstitute("CL", "<br>"); //  new line
	addTokenSubstitute("CM", "<br>"); //  paragraph <!P> is a non showing comment that can be changed in the front end to <P> if desired
	addTokenSubstitute("CG", ">"); // literal greater-than sign
	addTokenSubstitute("CT", "<"); // literal less-than sign
	addTokenSubstitute("JR", "<div align=\"right\">"); // right align begin
	addTokenSubstitute("JC", "<div align=\"center\">"); // center align begin
	addTokenSubstitute("JL", "</div>"); // align end
}

/** No descriptions */
char BT_GBFHTML::ProcessText(char * text, int maxlen, const SWKey * key, const SWModule * module){
	BT_BASICFILTER::ProcessText(text, maxlen, key, module);
	BT_BASICFILTER::ProcessRWPRefs(text, maxlen);
}


bool BT_GBFHTML::handleToken(char **buf, const char *token, DualStringMap &userData) {
	if (!substituteToken(buf, token)) {  //more than a simple replace
  	const int tokenLength = strlen(token);
		unsigned long i;
		unsigned long len = strlen(token);

		if (!strncmp(token, "WG", 2)){ // strong's numbers greek
			char num[6];
			for (i = 2; i < tokenLength; i++)
					num[i-2] = token[i];
			num[i-2]=0;

			pushString(buf," <font color=\"%s\"><small><em><a href=\"strongs://Greek/%s\">&lt;%s&gt;</a></em></small></font> ",
				strongs_color, num, num);
		}

		if (!strncmp(token, "WH", 2)){ // strong's numbers hebrew
			char num[6];
			for (i = 2; i < tokenLength; i++)
					num[i-2] = token[i];
			num[i-2]=0;

			pushString(buf," <font color=\"%s\"><small><em><a href=\"strongs://Hebrew/%s\">&lt;%s&gt;</a></em></small></font> ",
				strongs_color, num, num);
		}

		else if (!strncmp(token, "WTG", 3)) { // strong's numbers tense greek
			char num[16];
			for (i = 3; i < tokenLength; i++)
					num[i-3] = token[i];
			num[i-3]=0;

			pushString(buf," <font color=\"%s\"><small><em><a href=\"morph://Greek/%s\">(%s)</a></em></small></font> ",
				strongs_color, num, num);
		}

		else if (!strncmp(token, "WTH", 3)) { // strong's numbers tense hebrew

			char num[6];
			for (i = 3; i < tokenLength; i++)
					num[i-3] = token[i];
			num[i-3]=0;

			pushString(buf," <font color=\"%s\"><small><em><a href=\"morph://Hebrew/%s\">(%s)</a></em></small></font> ",
				strongs_color, num, num);
		}

		else if (!strncmp(token, "RB", 2)) {
			pushString(buf, "<i>");
			userData["hasFootnotePreTag"] = "true";
		}

		else if (!strncmp(token, "RF", 2)) {
			if(userData["hasFootnotePreTag"] == "true") {
				userData["hasFootnotePreTag"] = "false";
				pushString(buf, "</i> ");
			}
			pushString(buf,"<font color=\"%s\"><small> (",footnote_color);
		}

		else if (!strncmp(token, "FN", 2)) {
			pushString(buf, "<font face=\"");
			for (i = 2; i < tokenLength; i++)				
				if(token[i] != '\"')
					*(*buf)++ = token[i];
			pushString(buf,"\">");
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

/** Update the tokens which use changeable settings. */
void BT_GBFHTML::updateTokens(){
	replaceTokenSubstitute("TT", QString::fromLatin1(" <h1><font color=\"%1\">").arg(text_color).local8Bit());
	replaceTokenSubstitute("TS", QString::fromLatin1(" <H2><font color=\"%1\">").arg(text_color).local8Bit());
	replaceTokenSubstitute("FR", QString::fromLatin1("<font color=\"%1\">").arg(jesuswords_color).latin1());	
}
