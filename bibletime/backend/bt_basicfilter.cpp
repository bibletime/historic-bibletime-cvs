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
#include "cswordversekey.h"
#include "../frontend/cbtconfig.h"

char BT_BASICFILTER::ProcessText (char *text, int maxlen, const SWKey *key, const SWModule *module){
	m_module = module;
	ASSERT(m_module);
	
	m_key = key;
	ASSERT(key);	
	
	updateSettings();
	SWBasicFilter::ProcessText(text, maxlen);
	
	return 0;
}

void BT_BASICFILTER::updateSettings(void){
	strcpy(footnote_color,	CBTConfig::get(CBTConfig::footnotesColor	).name().utf8());
	strcpy(strongs_color,		CBTConfig::get(CBTConfig::strongsColor		).name().utf8());
	strcpy(morph_color,			CBTConfig::get(CBTConfig::morphsColor			).name().utf8());
	strcpy(jesuswords_color,CBTConfig::get(CBTConfig::jesuswordsColor	).name().utf8());
	strcpy(swordref_color,	CBTConfig::get(CBTConfig::swordRefColor		).name().utf8());
	strcpy(text_color, 			CBTConfig::get(CBTConfig::textColor				).name().utf8());	
	strcpy(standard_bible,	CReferenceManager::preferredModule(CReferenceManager::Bible).utf8());
}


/** Parses the verse reference ref and returns it. */
const char* BT_BASICFILTER::parseRef(const char* ref) {
 	cerr << "BT_BASICFILTER::parseRef(const char* ref)" << endl;
 	VerseKey parseKey = (m_key ? (const char*)*m_key : "Genesis 1:1");
 	ListKey list = parseKey.ParseVerseList(ref, parseKey, true);

 	//where do I now get a const char* array from??
  char* to = new char[1023]; //not right and doesn't work (refs do not appear)
	char* ret = to;//erstes Zeichen
 	 	
 	for(int i = 0; i < list.Count(); i++) {
 		SWKey* key = list.GetElement(i);
 		VerseKey* vk =  dynamic_cast<VerseKey*>(key);

// 		pushString(&to,"<font color=\""); //to wird erhöht!
// 		pushString(&to, swordref_color);
// 		pushString(&to, "\">");								
// 		pushString(&to, "<a href=\"sword://Bible/");
// 		pushString(&to, standard_bible);
// 		pushString(&to, "/");			

 		pushString(&to,"<font color=\"%s\"><a href=\"sword://Bible/%s/", //to wird erhöht!
			swordref_color, standard_bible);
		//man sollte versuchen so wenig pushString()s wie möglich zu verwenden
		// das muss in unseren Filtern noch gefixt werden, ist mir erst heute aufgefallen ;)
		// ein Zeichen kann man mit
		// to++='a' schnell zufügen. =)
		// pushString geht wie printf(char*, ...), nur ein Argument mehr
												
 		if (vk) {
 			pushString(&to, QString::fromLocal8Bit(vk->LowerBound()).utf8() );
 			pushString(&to, "-");					
 			pushString(&to, QString::fromLocal8Bit(vk->UpperBound()).utf8() );
 			pushString(&to, "\">");
 			pushString(&to, QString::fromLocal8Bit(vk->LowerBound()).utf8() );
 			pushString(&to, "-");
 			pushString(&to, QString::fromLocal8Bit(vk->UpperBound()).utf8() );
 			pushString(&to, "</a>");
 		}
 		else {
 			pushString(&to, QString::fromLocal8Bit((const char*)*key).utf8());
 			pushString(&to, "\">");
 			pushString(&to, QString::fromLocal8Bit((const char*)*key).utf8());
 			pushString(&to, "</a>");
 		}
 		pushString(&to, "</font>");
 		pushString(&to, ", ");
 	}

 	cerr << "return now!" << endl;
 	return ret;  //who deletes to ??
}


/** This filter converts the RWP #Gen 1:1| style bible references to HTML */
char BT_BASICFILTER::ProcessRWPRefs(char* text, int maxlen){
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

			//I want to have pushString(&to, parseRef(verse_str)); working here! How?
		   pushString(&to, parseRef(verse_str));
		
      //now parse the ref
//    	VerseKey parseKey = (m_key ? (const char*)*m_key : "Genesis 1:1");
//    	ListKey list = parseKey.ParseVerseList(verse_str, parseKey, true);
//
//    	for(int i = 0; i < list.Count(); i++) {
//    		SWKey* key = list.GetElement(i);
//    		VerseKey* vk =  dynamic_cast<VerseKey*>(key);
//
//    		pushString(&to,"<font color=\"");
//    		pushString(&to, swordref_color);
//    		pushString(&to, "\">");								
//    		pushString(&to, "<a href=\"sword://Bible/");
//    		pushString(&to, standard_bible);
//    		pushString(&to, "/");			
//   												
//    		if (vk) {
//    			pushString(&to, QString::fromLocal8Bit(vk->LowerBound()).utf8() );
//    			pushString(&to, "-");					
//    			pushString(&to, QString::fromLocal8Bit(vk->UpperBound()).utf8() );
//    			pushString(&to, "\">");
//    			pushString(&to, QString::fromLocal8Bit(vk->LowerBound()).utf8() );
//    			pushString(&to, "-");
//    			pushString(&to, QString::fromLocal8Bit(vk->UpperBound()).utf8() );
//    			pushString(&to, "</a>");
//    		}
//    		else {
//    			pushString(&to, QString::fromLocal8Bit((const char*)*key).utf8());
//    			pushString(&to, "\">");
//    			pushString(&to, QString::fromLocal8Bit((const char*)*key).utf8());
//    			pushString(&to, "</a>");
//    		}
//    		pushString(&to, "</font>");
//    		pushString(&to, ", ");
//    	}
			
			pushString(&to, " ");			
			continue;
		}
		*to++ = *from;
	}
	*to++ = 0;
	*to = 0;
	return 0;
}
