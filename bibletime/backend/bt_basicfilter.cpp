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
//using std::string;

char BT_BASICFILTER::processText(sword::SWBuf& buf, const sword::SWKey *key, const sword::SWModule *module){
	m_module = module;
	m_key = key;
	updateSettings();
	sword::SWBasicFilter::processText(buf, key, module);
	
	return 0;
}

void BT_BASICFILTER::updateSettings(){
	strcpy(standard_bible, CReferenceManager::preferredModule(CReferenceManager::Bible).utf8());
	updateTokens();
}

const string BT_BASICFILTER::parseSimpleRef(const string& ref) {
//  sword::SWModule* m = const_cast<sword::SWModule*>(m_module);
  if ( CSwordModuleInfo* m = CPointers::backend()->findModuleByName(standard_bible) ) {
    return parseRef( ref, m->module() );
  }
  return string();
}

/** Parses the verse reference ref and returns it. */
const string BT_BASICFILTER::parseRef(const string ref, sword::SWModule* module, const bool insertFullRef) {
  /**
  * This function should be able to parse references like "John 1:3; 3:1-10; Matthew 1:1-3:3"
  * without problems.
  */
  const string moduleName = string( module ? module->Name() : standard_bible );

  sword::VerseKey parseKey;
 	const char* lang = module ? module->Lang() : "en";
 	parseKey.setLocale( lang ); //we assume that the keys are in english or in the module's language
  
 	parseKey = m_key ? (const char*)*m_key : "Genesis 1:1"; //use the current key if there's any
 	sword::ListKey list;
  string ret;
  	
  QStringList refList = QStringList::split(QRegExp("[,.;]|(?:\\s(?=\\d?[A-Z]))", false), QString::fromLocal8Bit(ref.c_str()));
	int pos = 0;

  if ( !insertFullRef && (refList.count() >= 1) ) {
    /* We can't handle refs which defined their own caption if we would have to insert multiple refs
    * If our own caption should not be inserted and we have more than one ref return so the original
    * ref and caption will be used.
    */
    return "<span id=\"reference\"><a href=\"sword://Bible/" + moduleName + "/" + ref + "\">";
  }
  
	for ( QStringList::Iterator it = refList.begin(); it != refList.end(); ++it, pos++ ) {    
	 	list = parseKey.ParseVerseList((*it).local8Bit(), parseKey, true);
		
	 	const int count = list.Count();
    sword::SWKey* key = 0;
	 	for(int i = 0; i < count; i++) {
	 		key = list.GetElement(i);
  		ret += string("<span id=\"reference\"><a href=\"sword://Bible/") + moduleName + "/"; 
 			if ( sword::VerseKey* vk = dynamic_cast<sword::VerseKey*>(key) ) {
 				vk->setLocale("en");
 				vk->LowerBound().setLocale("en");
 				vk->UpperBound().setLocale("en");

        ret += string(vk->getRangeText()) + "\">";
        parseKey = *vk;
      }
      else {
        sword::VerseKey vk = key->getText();
 				vk.setLocale("en");
 				vk.LowerBound().setLocale("en");
 				vk.UpperBound().setLocale("en");
     
        ret += string(vk.getRangeText()) + "\">";
        parseKey = vk;
      }
      
      /* This is kind of a hack. This will only work if we process simple refs which won't get splitted.
      * But since insertFullRef is true by default, the end markers will be left out only in special cases like ThML refs
      * with own caption.
      */
      if (insertFullRef) { //HTML will only be valid if we hide only the end of one cross reference
        ret += string( (const char*)(*it).utf8() ) + "</a>";
  	 		(pos+1 < (int)refList.count()) ? ret.append("</span>, ") : ret.append("</span>");
      }
	 	}

	}	
 	return ret;
}

const string BT_BASICFILTER::parseThMLRef(const string& ref, const char* mod) {
	const char* moduleName = (mod ? mod : standard_bible);
  sword::SWModule* module = 0;
  if ( CSwordModuleInfo* m = CPointers::backend()->findModuleByName(moduleName) ) {
    module = m->module();
  }
  return parseRef( ref, module, false );
}

const string BT_BASICFILTER::thmlRefEnd() {
	return "</a></span>";
}

/** This filter converts the RWP #Gen 1:1| style bible references to HTML */
char BT_BASICFILTER::ProcessRWPRefs(sword::SWBuf& buf){
  /** Markup verse refs which are marked by #ref1[,;]ref2|
  *
  * 1. Search start marker (#)
  * 2. Search end marker (|)
  * 3. Replace found ref by parsed result!
  */

  sword::SWModule* module = const_cast<sword::SWModule*>(m_module);
  
  std::string target(buf.c_str());
  unsigned int idx_start = target.find_first_of("#",0); //find ref start
  unsigned int idx_end;
  
  while (idx_start != std::string::npos) {
    idx_end = target.find_first_of("|", idx_start); //find end marker

    if ((idx_end != std::string::npos) && (idx_end > idx_start+1)) { //found marker with content
      // Our length of the ref without markers is idx_end - (idx_start+1) = idx_end - idx_start - 1
      
      // Parse ref without start and end markers!
      const string ref = parseRef( target.substr(idx_start + 1, idx_end - idx_start - 1),  module );

      // Replace original ref sourrounded by # and | by the parsed ref in target!
      target.replace( idx_start, idx_end - idx_start + 1, ref ); //remove marker, too

      // Start searching for next ref start behind current one! It's faster!
      idx_start += ref.length();
    }

    idx_start = target.find_first_of("#", idx_start); //find ref start
  };

  buf = target.c_str(); //copy new content back into text!
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
