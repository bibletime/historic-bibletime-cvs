/***************************************************************************
                          bt_basicfilter.cpp  -  description
                             -------------------
    begin                : Thu Nov 1 2001
    copyright            : (C) 2001 by The BibleTime team
    email                : info@bibletime.de
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

#include "bt_basicfilter.h"
#include "creferencemanager.h"
#include "../frontend/cbtconfig.h"

BT_BASICFILTER::BT_BASICFILTER(){
}

char BT_BASICFILTER::ProcessText (char *text, int maxlen = -1){
	updateSettings();
	SWBasicFilter::ProcessText(text, maxlen);
}

void BT_BASICFILTER::updateSettings(void){
	strcpy(footnote_color,	CBTConfig::get(CBTConfig::footnotesColor	).name().utf8());
	strcpy(strongs_color,		CBTConfig::get(CBTConfig::strongsColor		).name().utf8());
	strcpy(morph_color,			CBTConfig::get(CBTConfig::morphsColor			).name().utf8());
	strcpy(jesuswords_color,CBTConfig::get(CBTConfig::jesuswordsColor	).name().utf8());
	strcpy(standard_bible,	/*CBTConfig::get(CBTConfig::standardBible		).utf8()*/CReferenceManager::preferredModule(CReferenceManager::Bible).utf8());
}


/** This filter converts the RWP #Gen 1:1| style bible references to HTML */
char BT_BASICFILTER::ProcessRWPRefs(char * text, int maxlen){
	char *to, *from, verse_str[500];
	int len;

	len = strlen(text) + 1;	// shift string to right of buffer
	if (len < maxlen) {
		memmove(&text[maxlen - len], text, len);
		from = &text[maxlen - len];
	}
	else
		from = text;	

	for (to = text; *from; from++) {
		if (*from == '#') {
			int i=0;
			bool is_verse = true;
			
			for ( i=0; i < 500 || from[i]=='|'; i++){
				if ( from[i] == '|' )
					break;  //We found a valid verse ref
				if ( !isalnum(from[i])  && !isspace(from[i]) && from[i]!=':'
							&& from[i]!=';' && from[i]!=',' && from[i]!='-' && from[i]!='#'){
					is_verse = false;
					break; // can't be a verseref
				}
			}

			if ( i==500 || !is_verse ){
				*to++ = *from;
				continue;
			}
				
			++from;
			i = 0;
			verse_str[0] = '\0';

			while (*from != '|' && i<500) { /* get the bible reference */
				verse_str[i++] = *from;
				verse_str[i + 1] = '\0';
				from++;
			}
			strcpy(to,"<a href=\"sword://Bible/");
			to += strlen(to);

			strcpy(to, standard_bible);
			to += strlen(to);

			*to++ = '/';

			strcpy(to, verse_str);
			to += strlen(to);
			*to++ = '\"';			
			*to++ = '>';

			strcpy(to, verse_str);
			to += strlen(to);

			strcpy(to,"</a>");
			to += strlen(to);

			continue;
		}

//		if ((*from == '#') || (*from == -81)) {	// verse markings (e.g. "#Mark 1:1|")
//			inverse = true;
//			strcpy(to,"<A HREF=\"sword:///"); //no special module
//			to += strlen(to);			
//			for (int i = 1; from[i] != '|'; ++i) {
//				*to++ = from[i];
//			}
//			strcpy(to,"\">");
//			to += strlen(to);			
//			continue;
//		}
//		if ((*from == '|') && (inverse)) {
//			inverse = false;
//			strcpy(to,"</A>");
//			to += strlen(to);
//			continue;
//		}
		*to++ = *from;
	}
	*to++ = 0;
	*to = 0;
	return 0;
}
