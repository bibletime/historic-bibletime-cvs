/***************************************************************************
                          cbtconfig.cpp  -  description
                             -------------------
    begin                : Mon Nov 19 2001
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

//BibleTime includes
#include "cbtconfig.h"

#include "backend/cswordmoduleinfo.h"
#include "backend/cdisplaytemplatemgr.h"

#include "util/cpointers.h"


//Qt includes
#include <qapplication.h>
#include <qfontdatabase.h>
#include <qstring.h>
#include <qstringlist.h>

//KDE includes
#include <kapplication.h>
#include <kconfig.h>
#include <kcharsets.h>
#include <kglobal.h>
#include <klocale.h>
#include <kaccel.h>


/* 	No constructor and destructor, because this class only contains static methods.
		It won't be instantiated. */

const QString CBTConfig::getKey( const CBTConfig::strings ID){
	switch ( ID ){
		case bibletimeVersion:			        return "bibletimeVersion";
		case language: 							        return "language";
		case displayStyle: 							    return "displayStyle";
	}
	return QString::null;
}

const QString CBTConfig::getDefault( const CBTConfig::strings ID){
	switch ( ID ){
		case bibletimeVersion:							return "NOT YET INSTALLED"; // main() will realize this and set the value to VERSION
		case language: 											return (KGlobal::locale()->language()).local8Bit();
		case displayStyle: 									return CDisplayTemplateMgr::defaultTemplate();
	}
	return QString::null;
}

const QString CBTConfig::getKey( const CBTConfig::modules ID){
	switch ( ID ){
		case standardBible: 				        return "standardBible";
		case standardCommentary: 						return "standardCommentary";
		case standardLexicon: 							return "standardLexicon";
		case standardDailyDevotional:	     	return "standardDailyDevotional";
		case standardHebrewStrongsLexicon: 	return "standardHebrewLexicon";
		case standardGreekStrongsLexicon: 	return "standardGreekLexicon";
		case standardHebrewMorphLexicon:	  return "standardHebrewMorphLexicon";
		case standardGreekMorphLexicon:		  return "standardGreekMorphLexicon";
	}
	
	return QString::null;
}

const QString CBTConfig::getDefault( const CBTConfig::modules ID){
// 	CSwordBackend* b = CPointers::backend();
	switch ( ID ){
		case standardBible: 								return "KJV";
		case standardCommentary: 						return "MHC";
		case standardLexicon: 							return "ISBE";
		case standardDailyDevotional: 			return ""; //no default
		
		case standardHebrewStrongsLexicon:  return "StrongsHebrew";
		case standardGreekStrongsLexicon: 	return "StrongsGreek";
		case standardHebrewMorphLexicon:	  return "StrongsHebrew";
		case standardGreekMorphLexicon:		  return "StrongsGreek";
	}
	
	return QString::null;
}

const QString CBTConfig::getKey( const CBTConfig::bools ID){
	switch ( ID ){
		case firstSearchDialog:		return "firstSearchDialog";
		case readOldBookmarks:		return "readOldBookmarks";

		case toolbar:							return "toolbar";
		case mainIndex:						return "mainIndex";
		case infoDisplay:					return "infoDisplay";

		case autoTile:						return "autoTile";
		case autoCascade:					return "autoCascade";

		case lexiconCache: 				return "lexicon_chache";

		case lineBreaks: 					return "lineBreaks";
		case verseNumbers: 				return "verseNumbers";
		case scroll:			 				return "scroll";

		case tips: 								return "RunOnStart";
		case logo: 								return "logo";
		case restoreWorkspace: 		return "restoreWorkspace";
		case crashedLastTime: 		return "crashedLastTime";
		case crashedTwoTimes: 		return "crashedTwoTimes";
	}
	return QString::null;	
}

const QString CBTConfig::getKey( const CBTConfig::ints ID){
	switch ( ID ){
		case footnotes: 					return "footnotes";
		case strongNumbers:  			return "strongNumbers";
  	case headings: 						return "headings";
  	case morphTags: 					return "morphTags";
		case lemmas: 							return "lemmas";
		case hebrewPoints: 				return "hebrewPoints";
		case hebrewCantillation: 	return "hebrewCantillation";
		case greekAccents: 				return "greekAccents";
		case textualVariants:			return "textualVariants";
//     case transliteration:     return "transliteration";
    case scriptureReferences: return "scriptureReferences";
  }
  return QString::null;
}

const bool CBTConfig::getDefault( const CBTConfig::bools ID){
	switch ( ID ){
		case firstSearchDialog:		return true;
    case readOldBookmarks:    return false;

		case toolbar:							return true;
		case mainIndex:						return true;
		case infoDisplay:					return true;

		case autoTile:						return true;
		case autoCascade:					return false;

		case lexiconCache: 				return true;

		case lineBreaks: 					return false;
		case verseNumbers: 				return true;
		case scroll:			 				return true;

		case tips: 								return true;
		case logo: 								return true;
		case restoreWorkspace: 		return true;
		case crashedLastTime: 		return false;
		case crashedTwoTimes: 		return false;
	}
	return false;
}

const int CBTConfig::getDefault( const CBTConfig::ints ID){
	switch ( ID ){
		case footnotes: 					return true;
		case strongNumbers:  			return false;
  	case headings: 						return true;
  	case morphTags: 					return false;
		case lemmas: 							return false;
		case hebrewPoints: 				return true;
		case hebrewCantillation: 	return true;
		case greekAccents: 				return true;
		case textualVariants:			return false;
//     case transliteration:     return 0;
    case scriptureReferences:	return true;
  }
  return 0;
}

const QString CBTConfig::getKey( const CBTConfig::intLists ID){
	switch ( ID ){
		case splitterSizes: return "splitterSizes";
	}
	return QString::null;
}

const QValueList<int> CBTConfig::getDefault( const CBTConfig::intLists ID){
	switch ( ID ){
		case splitterSizes: return QValueList<int>();
	}
	return QValueList<int>();
}

const QString CBTConfig::getKey( const CBTConfig::stringLists ID){
	switch ( ID ){
		case searchCompletionTexts:
      return QString::fromLatin1("searchCompletionTexts");
		case searchTexts:
      return QString::fromLatin1("searchTexts");
	}
	return QString::null;
}

const QStringList CBTConfig::getDefault( const CBTConfig::stringLists ID){
	switch ( ID ){
		case searchTexts: {
      QStringList list;
      list.append(QString::null);
			return list;
    }
		case searchCompletionTexts:
			return QStringList();
	}
	return QStringList();
}

const QString CBTConfig::getKey( const CBTConfig::stringMaps ID) {
	switch (ID) {
    case searchScopes:
      return QString::fromLatin1("SearchScopes");
  };
	return QString::null;
}

const CBTConfig::StringMap CBTConfig::getDefault( const CBTConfig::stringMaps ID){
	switch ( ID ){
    case searchScopes: {
      CBTConfig::StringMap map;
      map.insert(i18n("Old testament"),         QString::fromLatin1("Gen - Mal"));
  	  map.insert(i18n("Moses/Pentateuch/Torah"),QString::fromLatin1("Gen - Deut"));
  	  map.insert(i18n("History"),               QString::fromLatin1("Jos - Est"));
  	  map.insert(i18n("Prophets"),              QString::fromLatin1("Isa - Mal"));
   	  map.insert(i18n("New testament"),         QString::fromLatin1("Mat - Rev"));
  	  map.insert(i18n("Gospels"), 		          QString::fromLatin1("Mat - Joh"));
  	  map.insert(i18n("Letters/Epistles"), 	    QString::fromLatin1("Rom - Jude"));
  	  map.insert(i18n("Paul's Epistles"), 	    QString::fromLatin1("Rom - Phile"));

      //make the list to the current bookname language!
      CBTConfig::StringMap::Iterator it;
      sword::VerseKey vk;
      vk.setLocale("en");
      
      for (it = map.begin(); it != map.end(); ++it) {
        sword::ListKey list = vk.ParseVerseList(it.data().local8Bit(), "Genesis 1:1", true);
        QString data;
        for (int i = 0; i < list.Count(); ++i) {
          data += QString::fromUtf8(list.GetElement(i)->getRangeText()) + "; ";
     	  }
        map[it.key()] = data; //set the new data
      };      

      return map;
    };
    default:
      return CBTConfig::StringMap();
	}
	
	return CBTConfig::StringMap();
}


const QString CBTConfig::getKey( const CLanguageMgr::Language* const language ){
	return language->name();
}

const QFont CBTConfig::getDefault( const CLanguageMgr::Language* const){
  //language specific lookup of the font name
  return KApplication::font();
}


const QString CBTConfig::get( const CBTConfig::strings ID){
	KConfig* config = CBTConfig::getConfig();
	KConfigGroupSaver groupSaver(config, "strings");
	return config->readEntry(getKey(ID),getDefault(ID));
}

CSwordModuleInfo* const CBTConfig::get( const CBTConfig::modules ID){
	KConfig* config = CBTConfig::getConfig();
	KConfigGroupSaver groupSaver(config, "modules");
	
	QString name = config->readEntry(getKey(ID),getDefault(ID));	
	return CPointers::backend()->findModuleByName(name);
}


const bool CBTConfig::get( const CBTConfig::bools ID){
	//special behaviour for the KTipDialog class
	KConfig* config = CBTConfig::getConfig();	
	KConfigGroupSaver groupSaver(config, (ID == CBTConfig::tips) ? "TipOfDay" : "bools");
	return config->readBoolEntry(getKey(ID),getDefault(ID));
}

const int CBTConfig::get( const CBTConfig::ints ID){
	KConfig* config = CBTConfig::getConfig();	
	KConfigGroupSaver groupSaver(config, "ints");		
	return config->readBoolEntry(getKey(ID), getDefault(ID));
}

const QValueList<int> CBTConfig::get( const CBTConfig::intLists ID ){
	KConfig* config = CBTConfig::getConfig();
	KConfigGroupSaver groupSaver(config, "lists");
	return config->readIntListEntry(getKey(ID));
}

const QStringList	CBTConfig::get( const CBTConfig::stringLists ID ){
	KConfig* config = CBTConfig::getConfig();
	KConfigGroupSaver groupSaver(config, "stringlists");
	return config->readListEntry(getKey(ID));
}

const CBTConfig::StringMap CBTConfig::get( const CBTConfig::stringMaps ID ){
	KConfig* config = CBTConfig::getConfig();
	KConfigGroupSaver groupSaver(config, getKey(ID));

  if (config->hasGroup(getKey(ID))) {
	  CBTConfig::StringMap map = config->entryMap(getKey(ID));
    switch (ID) {
      case searchScopes: { //make sure we return the scopes in the chosen language. saved keys are in english
        CBTConfig::StringMap::Iterator it;
        sword::VerseKey vk;

        for (it = map.begin(); it != map.end(); ++it) {
          sword::ListKey list = vk.ParseVerseList(it.data().utf8(), "Genesis 1:1", true);
          QString data;
          for (int i = 0; i < list.Count(); ++i) {
            data += QString::fromUtf8(list.GetElement(i)->getRangeText()) + "; ";
       	  }
          map[it.key()] = data; //set the new data
        };
        return map;
      }
      default:
        return getDefault(ID);
    }
  }
  return getDefault(ID);
}

const CBTConfig::FontSettingsPair	CBTConfig::get( const CLanguageMgr::Language* const language ){
	KConfig* config = CBTConfig::getConfig();
	KConfigGroupSaver groupSaver(config, "font standard settings");

  FontSettingsPair settings;
  settings.first = config->readBoolEntry(getKey(language));

  config->setGroup("fonts");

  settings.second = settings.first ? config->readFontEntry(getKey(language)) : KApplication::font();

  return settings;
}

void CBTConfig::set( const CBTConfig::strings ID, const QString value ){
	KConfig* config = CBTConfig::getConfig();
	KConfigGroupSaver groupSaver(config, "strings");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set( const CBTConfig::modules ID, CSwordModuleInfo* const value ){
	KConfig* config = CBTConfig::getConfig();
	KConfigGroupSaver groupSaver(config, "modules");
	config->writeEntry(getKey(ID), value ? value->name() : QString::null);
}

void CBTConfig::set( const CBTConfig::modules ID, const QString& value ){
	CSwordModuleInfo* module = CPointers::backend()->findModuleByName(value);
	if (module) {
		CBTConfig::set(ID, module);
	}
}

void CBTConfig::set(const  CBTConfig::bools ID,const  bool value ){
	KConfig* config = CBTConfig::getConfig();
	//special behaviour to work with KTipDialog class of KDE
	KConfigGroupSaver groupSaver(config, (ID == CBTConfig::tips) ? "TipOfDay" : "bools");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set(const CBTConfig::ints ID, const int value ){
	KConfig* config = CBTConfig::getConfig();
	KConfigGroupSaver groupSaver(config, "ints");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set( const CBTConfig::intLists ID, const QValueList<int> value ){
	KConfig* config = CBTConfig::getConfig();
	KConfigGroupSaver groupSaver(config, "lists");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set( const CBTConfig::stringLists ID, const QStringList value ){
	KConfig* config = CBTConfig::getConfig();
	KConfigGroupSaver groupSaver(config, "stringlists");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set( const CBTConfig::stringMaps ID, const CBTConfig::StringMap value ){
	KConfig* config = CBTConfig::getConfig();
	KConfigGroupSaver groupSaver(config, getKey(ID));  
  config->deleteGroup(getKey(ID)); //make sure we only save the new entries and don't use old ones
  config->sync();  
  config->setGroup(getKey(ID));
  
  switch (ID) {
    case searchScopes: {
      /**
      * We want to make sure that the search scopes are saved with english key names so loading them
      * will always work with each locale set.
      */
      CBTConfig::StringMap::ConstIterator it;
      QString data = QString::null;

      sword::VerseKey vk;
      for (it = value.begin(); it != value.end(); ++it) {
        sword::ListKey list = vk.ParseVerseList(it.data().utf8(), "Genesis 1:1", true);
        data = QString::null;
        for (int i = 0; i < list.Count(); ++i) {
          if ( sword::VerseKey* range = dynamic_cast<sword::VerseKey*>(list.GetElement(i)) ) {
            range->setLocale("en");
            data += QString::fromUtf8( range->getRangeText() ) + ";";
          }
        }
        config->writeEntry(it.key(), data);
      }
      break;
    }
    default: {
      for (CBTConfig::StringMap::ConstIterator it = value.begin(); it != value.end(); ++it) {
        config->writeEntry(it.key(), it.data());
      }
      break;
    }
  };
}


void CBTConfig::set( const CLanguageMgr::Language* const language, const FontSettingsPair& value ){
	KConfig* config = CBTConfig::getConfig();

  KConfigGroupSaver groupSaver(config, "fonts");
	config->writeEntry(getKey(language), value.second);
  
  config->setGroup("font standard settings");  
	config->writeEntry(getKey(language), value.first);
}


const CSwordBackend::DisplayOptions CBTConfig::getDisplayOptionDefaults(){
  CSwordBackend::DisplayOptions options;

	options.lineBreaks   = 	get(CBTConfig::lineBreaks);
  options.verseNumbers = 	get(CBTConfig::verseNumbers);

	return options;
}

const CSwordBackend::FilterOptions CBTConfig::getFilterOptionDefaults(){
  CSwordBackend::FilterOptions options;

//   options.footnotes =						get(CBTConfig::footnotes);
	options.footnotes =						true; //required for the info display

  options.strongNumbers = 			true; //get(CBTConfig::strongNumbers);
  options.headings = 						get(CBTConfig::headings);
	
//   options.morphTags = 					get(CBTConfig::morphTags);
   options.morphTags = 					true;//required for the info display

//   options.lemmas = 							get(CBTConfig::lemmas);
  options.lemmas =	 						true;//required for the info display
  
	options.hebrewPoints = 				get(CBTConfig::hebrewPoints);
  options.hebrewCantillation = 	get(CBTConfig::hebrewCantillation);
  options.greekAccents = 				get(CBTConfig::greekAccents);
  options.textualVariants = 		get(CBTConfig::textualVariants);
//   options.transliteration = 		get(CBTConfig::transliteration);
  options.scriptureReferences = get(CBTConfig::scriptureReferences);

	return options;
}

void CBTConfig::setupAccel(const CBTConfig::keys type, KAccel* const accel) {
	switch (type) {
		case writeWindow : {
			accel->setConfigGroup("Writewindow shortcuts");
			break;
		};
		case readWindow : {
			accel->setConfigGroup("Readwindow shortcuts");
			break;
		};
		case bookWindow : {
			accel->setConfigGroup("Book shortcuts");
			break;
		};
		case bibleWindow : {
			accel->setConfigGroup("Bible shortcuts");
			break;
		};
		case commentaryWindow : {
			accel->setConfigGroup("Commentary shortcuts");
			break;
		};
		case lexiconWindow : {
			accel->setConfigGroup("Lexicon shortcuts");					
			break;
		};
		case application : {
			accel->setConfigGroup("Application shortcuts");				
			break;
		};
	};
}

const QString CBTConfig::getModuleEncryptionKey( const QString& module ) {
	KConfig* config = CBTConfig::getConfig();
  KConfigGroupSaver groupSaver(config, "Module keys");

  return config->readEntry(module, QString::null);
}
  
void CBTConfig::setModuleEncryptionKey( const QString& module, const QString& key ) {
//  if (CSwordModuleInfo* const mod = CPointers::backend()->findModuleByName(module) ) {
//    // if an empty key is set for an unencrypted module do nothing
//    if (key.isEmpty() && !mod->isEncrypted()) {
//      return;
//    }
//  }
//  else if (key.isEmpty()) {
//    return;
//  };
  
  KConfig* config = CBTConfig::getConfig();
  KConfigGroupSaver groupSaver(config, "Module keys");

  config->writeEntry(module, key);
};

KConfig* const CBTConfig::getConfig() {
	KConfig* config = KGlobal::config();
	
	if (KApplication::kApplication()->sessionSaving()) {
		qWarning("Using session config");
//  		config = KApplication::kApplication()->sessionConfig();
	}
	else if (KApplication::kApplication()->isRestored()) {
		qWarning("isRestored(): Using session config");
	}
	
	return config;
}
