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
//BIbleTime includes
#include <stdlib.h>
#include "bt_basicfilter.h"
#include "creferencemanager.h"
#include "cswordversekey.h"
#include "../frontend/cbtconfig.h"

//Qt includes
#include <qregexp.h>
#include <qstringlist.h>

char BT_BASICFILTER::ProcessText (char *text, int maxlen, const SWKey *key, const SWModule *module){
	m_module = module;
	m_key = key;
	updateSettings();
	SWBasicFilter::ProcessText(text, maxlen, key, module);
	
	return 0;
}

void BT_BASICFILTER::updateSettings(){
	strcpy(footnote_color,	CBTConfig::get(CBTConfig::footnotesColor	).name().utf8());
	strcpy(strongs_color,		CBTConfig::get(CBTConfig::strongsColor		).name().utf8());
	strcpy(morph_color,			CBTConfig::get(CBTConfig::morphsColor			).name().utf8());
	strcpy(jesuswords_color,CBTConfig::get(CBTConfig::jesuswordsColor	).name().utf8());
	strcpy(swordref_color,	CBTConfig::get(CBTConfig::swordRefColor		).name().utf8());
	strcpy(text_color, 			CBTConfig::get(CBTConfig::textColor				).name().utf8());	
	strcpy(standard_bible,	CReferenceManager::preferredModule(CReferenceManager::Bible).utf8());
	
	updateTokens();
}


/** Parses the verse reference ref and returns it. */
const char* BT_BASICFILTER::parseSimpleRef(const char* ref) {
 	VerseKey parseKey; 	
 	SWModule* m = const_cast<SWModule*>(m_module);
 	const char* lang = m ? m->Lang() : "en";
 	parseKey.setLocale(lang);

 	parseKey = (m_key ? (const char*)*m_key : "Genesis 1:1");
 	ListKey list;
  char* to = new char[5000];
	char* ret = to;
	
	QStringList refList = QStringList::split(QRegExp("[,.;]", false), QString::fromLocal8Bit(ref));
	int pos = 0;
	for ( QStringList::Iterator it = refList.begin(); it != refList.end(); ++it, pos++ ) {
	 	list = parseKey.ParseVerseList((*it).local8Bit(), parseKey, true);
		
	 	const int count = list.Count();
	 	for(int i = 0; i < count; i++) {
	 		SWKey* key = list.GetElement(i);
	 		VerseKey* vk = dynamic_cast<VerseKey*>(key);
 		
  		pushString(&to,"<font color=\"%s\"><a href=\"sword://Bible/%s/",
 				swordref_color,
	 			standard_bible
 			);
 			
 			if (vk) {
 				vk->setLocale(lang);
 				vk->LowerBound().setLocale(lang);
 				vk->UpperBound().setLocale(lang); 				
 			}
 			if (vk && vk->UpperBound() != vk->LowerBound()) { 				
	 			pushString(&to, "%s-%s\">%s</a>",
	 				(const char*)QString::fromLocal8Bit(vk->LowerBound()).utf8(),
	 				(const char*)QString::fromLocal8Bit(vk->UpperBound()).utf8(),
	 				(const char*)(*it).utf8()
	 			); 			
	 		}
	 		else {
	 			pushString(&to, "%s\">%s</a>",
	 				(const char*)QString::fromLocal8Bit((const char*)*key).utf8(),
					(const char*)(*it).utf8()
				);
	 		}
	 		(pos+1 < (int)refList.count()) ? pushString(&to, "</font>, ") : pushString(&to, "</font>");
	 	}
	}	
 	*to++ = '\0';
 	return ret;  //don't forget to delete it!
}

const char* BT_BASICFILTER::parseThMLRef(const char* ref, const char* mod) {
  char* to = new char[5000];
	char* ret = to;
	const char* module = (mod ? mod : standard_bible);	
// 	VerseKey parseKey = (m_key ? (const char*)*m_key : "Genesis 1:1");	
//	ListKey list = parseKey.ParseVerseList(ref, parseKey, false);		
//  const int count = list.Count();

//  for(int i = 0; i < count; i++) {
//	 	SWKey* key = list.GetElement(i);
//	 	VerseKey* vk =  dynamic_cast<VerseKey*>(key);
// 		
	 	pushString(&to,"<font color=\"%s\"><a href=\"sword://Bible/%s/",
	 		swordref_color,
	 		module
	 	);
//	 	if (vk && vk->UpperBound() != vk->LowerBound()) {
//	 		pushString(&to, "%s-%s\">",
//	 			(const char*)QString::fromLocal8Bit(vk->LowerBound()).utf8(),
//	 			(const char*)QString::fromLocal8Bit(vk->UpperBound()).utf8()
//	 		);
//	 	}
//	 	else {
	 		pushString(&to, "%s\">",
	 			(const char*)QString::fromLocal8Bit(ref).utf8()
			);
//	 	}
//	 	(i+1 < refList.count()) ? pushString(&to, "</font>, ") : pushString(&to, "</font>");	
//	}
	*to++ = '\0';
	return ret;
}

const char* BT_BASICFILTER::thmlRefEnd() {
	return "</a></font>";
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
			
//			cerr << verse_str << endl;
			const char* ref = parseSimpleRef(verse_str);			
		  pushString(&to,"%s ", ref);
		  delete [] ref;//delete now because it's unused
		
			continue;
		}
		*to++ = *from;
	}
	*to++ = 0;
	*to = 0;
	return 0;
}

/** Replaces the token in the substitute map. */
void BT_BASICFILTER::replaceTokenSubstitute(const char* findString, const char* replaceString){
	tokenSubMap.erase( tokenSubMap.find(findString) ); //erase entry
	addTokenSubstitute(findString, replaceString);
}

/** Updates the token for this filter if settings changed. */
void BT_BASICFILTER::updateTokens(){
}
