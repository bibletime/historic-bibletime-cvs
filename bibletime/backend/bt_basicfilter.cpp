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

#include "frontend/cbtconfig.h"
#include "util/cpointers.h"

//System includes
#include <iostream>
#include <string>

//Qt includes
#include <qregexp.h>
#include <qstringlist.h>

using std::cout;
using std::cerr;
using std::endl;

char BT_BASICFILTER::ProcessText (char *text, int maxlen, const sword::SWKey *key, const sword::SWModule *module){
	m_module = module;
	m_key = key;
	updateSettings();
	sword::SWBasicFilter::ProcessText(text, maxlen, key, module);
	
	return 0;
}

void BT_BASICFILTER::updateSettings(){
	strcpy(standard_bible,	CReferenceManager::preferredModule(CReferenceManager::Bible).utf8());
	updateTokens();
}


/** Parses the verse reference ref and returns it. */
const char* BT_BASICFILTER::parseSimpleRef(const char* ref) {
  /**
  * This function should be able to parse references like "John 1:3; 3:1-10; Matthew 1:1-3:3"
  * without problems.
  */
 	sword::VerseKey parseKey; 	
 	sword::SWModule* m = const_cast<sword::SWModule*>(m_module);
 	const char* lang = m ? m->Lang() : "en";
 	parseKey.setLocale(lang); //we assume that the keys are in english or in the module's language

 	parseKey = (m_key ? (const char*)*m_key : "Genesis 1:1"); //use the current key if there's any
 	sword::ListKey list;
  char* to = new char[5000];
	char* ret = to;
	
	QStringList refList = QStringList::split(QRegExp("[,.;]|(?:\\s(?=\\d?[A-Z]))", false), QString::fromLocal8Bit(ref));
//	QStringList refList = QStringList::split(QRegExp("[,.;]", false), QString::fromLocal8Bit(ref));
	int pos = 0;
	for ( QStringList::Iterator it = refList.begin(); it != refList.end(); ++it, pos++ ) {    
	 	list = parseKey.ParseVerseList((*it).local8Bit(), parseKey, true);
		
	 	const int count = list.Count();
    sword::SWKey* key = 0;
	 	for(int i = 0; i < count; i++) {
	 		key = list.GetElement(i);
      parseKey = *key;
  		pushString(&to,"<span id=\"reference\"><a href=\"sword://Bible/%s/", standard_bible); 
 			if ( sword::VerseKey* vk = dynamic_cast<sword::VerseKey*>(key) ) {
 				vk->setLocale(lang);
 				vk->LowerBound().setLocale(lang);
 				vk->UpperBound().setLocale(lang);
	 		}
 			pushString(&to, "%s\">%s</a>", key->getRangeText(), (const char*)(*it).utf8() );
	 		(pos+1 < (int)refList.count()) ? pushString(&to, "</span>, ") : pushString(&to, "</span>");
	 	}

	}	
 	*to++ = '\0';
 	return ret;  //don't forget to delete it!
}

const char* BT_BASICFILTER::parseThMLRef(const char* ref, const char* mod) {
  char* to = new char[5000];
	char* ret = to;
	const char* module = (mod ? mod : standard_bible);	

	CReferenceManager::Type type = CReferenceManager::Unknown;
	if (CSwordModuleInfo* m = CPointers::backend()->findModuleByName(module)) {
		type = CReferenceManager::typeFromModule(m->type());
  }

// 	VerseKey parseKey = (m_key ? (const char*)*m_key : "Genesis 1:1");	
//	ListKey list = parseKey.ParseVerseList(ref, parseKey, false);		
//  const int count = list.Count();

//  for(int i = 0; i < count; i++) {
//	 	SWKey* key = list.GetElement(i);
//	 	VerseKey* vk =  dynamic_cast<VerseKey*>(key);
// 		
	 	pushString(&to,"<span id=\"reference\"><a href=\"%s\">",
			(const char*)CReferenceManager::encodeHyperlink(QString::fromLatin1(module),QString::fromLocal8Bit(ref).utf8(), type).utf8()
	 	);
//	 	if (vk && vk->UpperBound() != vk->LowerBound()) {
//	 		pushString(&to, "%s-%s\">",
//	 			(const char*)QString::fromLocal8Bit(vk->LowerBound()).utf8(),
//	 			(const char*)QString::fromLocal8Bit(vk->UpperBound()).utf8()
//	 		);
//	 	}
//	 	else {
//	 		pushString(&to, "%s\">",
//	 			(const char*)
//			);
//	 	}
//	 	(i+1 < refList.count()) ? pushString(&to, "</font>, ") : pushString(&to, "</font>");	
//	}
	*to++ = '\0';
	return ret;
}

const char* BT_BASICFILTER::thmlRefEnd() {
	return "</a></span>";
}

/** This filter converts the RWP #Gen 1:1| style bible references to HTML */
char BT_BASICFILTER::ProcessRWPRefs(char* text, int maxlen){
  /** Markup verse refs which are marked by #ref1[,;]ref2|
  *
  * 1. Search start marker (#)
  * 2. Search end marker (|)
  * 3. Replace found ref by parsed result!
  */

  std::string target(text);
  int idx_start = target.find_first_of("#",0); //find ref start
  int idx_end;
  
  while (idx_start != std::string::npos) {
    idx_end = target.find_first_of("|", idx_start); //find end marker

    if ((idx_end != std::string::npos) && (idx_end > idx_start+1)) { //found marker with content
      // Our length of the ref without markers is idx_end - (idx_start+1) = idx_end - idx_start - 1
      
      // Parse ref without start and end markers!
      const char* ref = parseSimpleRef( target.substr(idx_start + 1, idx_end - idx_start - 1).c_str() );

      // Replace original ref sourrounded by # and | by the parsed ref in target!
      target.replace( idx_start, idx_end - idx_start + 1, ref ); //remove marker, too

      // Start searching for next ref start behind current one! It's faster!
      idx_start += strlen( ref );

		  delete [] ref; //delete now because it's unused, was created by parseSimpleRef!
    }

    idx_start = target.find_first_of("#", idx_start); //find ref start
  };

  strncpy(text, target.c_str(), maxlen); //copy new content back into text!
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
