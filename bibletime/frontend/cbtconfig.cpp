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
		case standardBible: 				        return "standardBible";
		case standardCommentary: 		        return "standardCommentary";
		case standardLexicon: 			        return "standardLexicon";
		case standardDailyDevotional:	      return "standardDailyDevotional";    
		case standardHebrewStrongsLexicon: 	return "standardHebrewLexicon";
		case standardGreekStrongsLexicon: 	return "standardGreekLexicon";
		case standardHebrewMorphLexicon:	  return "standardHebrewMorphLexicon";
		case standardGreekMorphLexicon:		  return "standardGreekMorphLexicon";
	}
	return QString::null;
}

const QString CBTConfig::getDefault( const CBTConfig::strings ID){
	switch ( ID ){
		case bibletimeVersion:			return ( "NOT YET INSTALLED" );
		// main() will realize this and set the value to VERSION
		case language: 							return (KGlobal::locale()->language()).local8Bit();
		case standardBible: 				return "KJV";  // no effect
		case standardCommentary: 		return "MHC";
		case standardLexicon: 			return "ISBE";
		case standardDailyDevotional: 			return ""; //no default
		case standardHebrewStrongsLexicon:  return "StrongsHebrew";
		case standardGreekStrongsLexicon: 	return "StrongsGreek";
		case standardHebrewMorphLexicon:	  return "StrongsHebrew"; //warning this is wrong
		case standardGreekMorphLexicon:		  return "StrongsGreek";  //but imo has no effect
	}
	return QString::null;
}

const QString CBTConfig::getKey( const CBTConfig::bools ID){
	switch ( ID ){
		case firstSearchDialog:		return "firstSearchDialog";
		case readOldBookmarks:		return "readOldBookmarks";

		case toolbar:							return "toolbar";
		case mainIndex:						return "mainIndex";

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
    case transliteration:     return "transliteration";
  }
  return QString::null;
}

const bool CBTConfig::getDefault( const CBTConfig::bools ID){
	switch ( ID ){
		case firstSearchDialog:		return true;
    case readOldBookmarks:    return false;

		case toolbar:							return true;
		case mainIndex:						return true;

		case autoTile:						return true;
		case autoCascade:					return false;

		case lexiconCache: 				return true;

		case lineBreaks: 					return true;
		case verseNumbers: 				return true;
		case scroll:			 				return true;

		case tips: 								return true;
		case logo: 								return true;
		case restoreWorkspace: 		return false;
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
		case hebrewPoints: 				return false;
		case hebrewCantillation: 	return false;
		case greekAccents: 				return false;
		case textualVariants:			return false;
    case transliteration:     return 0;
  }
  return 0;
}

const QString CBTConfig::getKey( const CBTConfig::colors ID){
	switch ( ID ){
		case textColor: 							return "textColor";
		case backgroundColor: 				return "backgroundColor";
		case background2Color: 				return "background2Color";
		case highlightedVerseColor: 	return "highlightedVerseClolor";
		case footnotesColor: 					return "footnotesColor";
		case strongsColor: 						return "strongsColor";
		case morphsColor: 						return "morphsColor";
		case jesuswordsColor: 				return "jesuswordsColor";
		case swordRefColor: 					return "swordRefColor";
	}
	return QString::null;
}

const QColor CBTConfig::getDefault( const CBTConfig::colors ID){
	switch ( ID ){
		case textColor: 							return QColor(Qt::black);
		case backgroundColor: 				return QColor(Qt::white);
		case background2Color: 				return QColor(QString("#f1f1f1"));
		case highlightedVerseColor:		return QColor(Qt::red);
		case footnotesColor: 					return QColor(Qt::black);
		case strongsColor: 						return QColor(Qt::darkBlue);
		case morphsColor: 						return QColor(Qt::darkBlue);
		case jesuswordsColor: 				return QColor(Qt::red);
		case swordRefColor: 					return QColor(Qt::blue);
	}
	return Qt::blue;
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
		case searchCompletionTexts: return QString::fromLatin1("searchCompletionTexts");
		case searchTexts: return QString::fromLatin1("searchTexts");
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
      for (it = map.begin(); it != map.end(); ++it) {
        //VerseKey key(it.data());
        sword::ListKey list = sword::VerseKey().ParseVerseList(it.data().local8Bit(), "Genesis 1:1", true);
        QString data = QString::null;
        for (int i = 0; i < list.Count(); ++i) {
       	  if (sword::VerseKey* element = dynamic_cast<sword::VerseKey*>(list.GetElement(i)))
       			data += QString::fromLatin1("%1 - %2;").arg(QString::fromLocal8Bit((const char*)element->LowerBound())).arg(QString::fromLocal8Bit((const char*)element->UpperBound()));
       		else
       			data += QString::fromLocal8Bit("%1;").arg((const char*)*list.GetElement(i));
     	  }
        map[it.key()] = data; //set the new data
      };      
      return map;
    };        
	}
	return CBTConfig::StringMap();
}


const QString CBTConfig::getKey( const CLanguageMgr::Language& language ){
	return language.name();
}

const QFont CBTConfig::getDefault( const CLanguageMgr::Language& language ){
  //language specific lookup of the font name
  return KApplication::font();
}


const QString CBTConfig::get( const CBTConfig::strings ID){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "strings");
	return config->readEntry(getKey(ID),getDefault(ID));
}

const bool CBTConfig::get( const CBTConfig::bools ID){
	//special behaviour for the KTipDialog class
	KConfig* config = KGlobal::config();	
	KConfigGroupSaver groupSaver(config, (ID == CBTConfig::tips) ? "TipOfDay" : "bools");
	return config->readBoolEntry(getKey(ID),getDefault(ID));
}

const int CBTConfig::get( const CBTConfig::ints ID){
	KConfig* config = KGlobal::config();	
	KConfigGroupSaver groupSaver(config, "ints");		
	return config->readBoolEntry(getKey(ID), getDefault(ID));
}


const QColor CBTConfig::get( const CBTConfig::colors ID){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "colors");
	QColor defaultColor = getDefault(ID);
	return config->readColorEntry(getKey(ID),&defaultColor);
}

const QValueList<int> CBTConfig::get( const CBTConfig::intLists ID ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "lists");
	return config->readIntListEntry(getKey(ID));
}

const QStringList	CBTConfig::get( const CBTConfig::stringLists ID ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "stringlists");
	return config->readListEntry(getKey(ID));
}

const CBTConfig::StringMap CBTConfig::get( const CBTConfig::stringMaps ID ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, getKey(ID));
  CBTConfig::StringMap map = config->entryMap(getKey(ID));
  if (config->hasGroup(getKey(ID)))
    return map;
  return getDefault(ID);
}

const CBTConfig::FontSettingsPair	CBTConfig::get( const CLanguageMgr::Language& language ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "font standard settings");

  FontSettingsPair settings;
  settings.first = config->readBoolEntry(getKey(language));

  config->setGroup("fonts");
 
  settings.second = !settings.first ? KApplication::font() : config->readFontEntry(getKey(language));

  return settings;
}

void CBTConfig::set( const CBTConfig::strings ID, const QString value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "strings");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set(const  CBTConfig::bools ID,const  bool value ){
	KConfig* config = KGlobal::config();
	//special behaviour to work with KTipDialog class of KDE
	KConfigGroupSaver groupSaver(config, (ID == CBTConfig::tips) ? "TipOfDay" : "bools");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set(const CBTConfig::ints ID, const int value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "ints");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set( const CBTConfig::colors ID, const QColor value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "colors");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set( const CBTConfig::intLists ID, const QValueList<int> value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "lists");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set( const CBTConfig::stringLists ID, const QStringList value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "stringlists");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set( const CBTConfig::stringMaps ID, const CBTConfig::StringMap value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, getKey(ID));  
  config->deleteGroup(getKey(ID)); //make sure we only save the new entries and don't use old ones
  config->sync();  
  config->setGroup(getKey(ID));
  
  /**
  * We want to make sure that the search scopes are saved with english key names so loading them
  * will always work with each locale set.
  */
  CBTConfig::StringMap::ConstIterator it;
  QString data = QString::null;  
  for (it = value.begin(); it != value.end(); ++it) {
    sword::ListKey list = sword::VerseKey().ParseVerseList(it.data().local8Bit(), "Genesis 1:1", true);
    data = QString::null;
    for (int i = 0; i < list.Count(); ++i) {
   	  if (sword::VerseKey* element = dynamic_cast<sword::VerseKey*>(list.GetElement(i))) {
        element->setLocale("en");
        element->LowerBound().setLocale("en");
        element->UpperBound().setLocale("en");
   			data += QString::fromLatin1("%1 - %2;").arg(QString::fromLocal8Bit((const char*)element->LowerBound())).arg(QString::fromLocal8Bit((const char*)element->UpperBound()));
      }
   		else
   			data += QString::fromLocal8Bit("%1;").arg((const char*)*list.GetElement(i));
 	  }
    config->writeEntry(it.key(), data);    
  };
}


void CBTConfig::set( const CLanguageMgr::Language& language, const FontSettingsPair& value ){
	KConfig* config = KGlobal::config();

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

  options.footnotes =						get(CBTConfig::footnotes);
  options.strongNumbers = 			get(CBTConfig::strongNumbers);
  options.headings = 						get(CBTConfig::headings);
  options.morphTags = 					get(CBTConfig::morphTags);
  options.lemmas = 							get(CBTConfig::lemmas);
  options.hebrewPoints = 				get(CBTConfig::hebrewPoints);
  options.hebrewCantillation = 	get(CBTConfig::hebrewCantillation);
  options.greekAccents = 				get(CBTConfig::greekAccents);

//  options.textualVariants = 		get(CBTConfig::textualVariants);
  options.textualVariants = 		get(CBTConfig::textualVariants);
  options.transliteration = 		get(CBTConfig::transliteration);

	return options;
}

void CBTConfig::setupAccel(const CBTConfig::keys type, KAccel* const accel) {
//	KConfig* config = KGlobal::config();	
	
	switch (type) {
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
	KConfig* config = KGlobal::config();
  KConfigGroupSaver groupSaver(config, "Module keys");

  return config->readEntry(module, QString::null);
}
  
void CBTConfig::setModuleEncryptionKey( const QString& module, const QString& key ) {
  if (key.isEmpty()) { //if no key is set do nothing
    return;
  };
  
  KConfig* config = KGlobal::config();
  KConfigGroupSaver groupSaver(config, "Module keys");

  config->writeEntry(module, key);
};
