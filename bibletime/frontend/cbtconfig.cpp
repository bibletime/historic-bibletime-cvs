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

#include "cbtconfig.h"

#include <kconfig.h>
#include <kglobal.h>
#include <klocale.h>

#include <qapplication.h>

/* 	No constructor and destructor, because this class only contains static methods.
		It won't be instantiated. */

const QString CBTConfig::getKey( const CBTConfig::strings ID){
	switch ( ID ){
		case bibletimeVersion:			return "bibletimeVersion";
		case language: 							return "language";
		case standardBible: 				return "standardBible";
		case standardCommentary: 		return "standardCommentary";
		case standardLexicon: 			return "standardLexicon";
		case standardHebrewStrongsLexicon: 	return "standardHebrewLexicon";
		case standardGreekStrongsLexicon: 	return "standardGreekLexicon";
		case standardHebrewMorphLexicon:	return "standardHebrewMorphLexicon";
		case standardGreekMorphLexicon:		return "standardGreekMorphLexicon";
	}
}

const QString CBTConfig::getDefault( const CBTConfig::strings ID){
	switch ( ID ){
		case bibletimeVersion:			return ( "NOT YET INSTALLED" );
			// main() will realize this and set the value to VERSION
		case language: 							return (KGlobal::locale()->language()).local8Bit();
		case standardBible: 				return "KJV";  // no effect
		case standardCommentary: 		return "MHC";
		case standardLexicon: 			return "Eastons";
		case standardHebrewStrongsLexicon: return "StrongsHebrew";
		case standardGreekStrongsLexicon: 	return "StrongsGreek";
		case standardHebrewMorphLexicon:	return "StrongsHebrew"; //warning this is wrong
		case standardGreekMorphLexicon:		return "StrongsGreek";  //but imo has no effect
	}
}

const QString CBTConfig::getKey( const CBTConfig::fonts ID){
	switch ( ID ){
		case standard: 	return "standard";
		case unicode: 	return "unicode";
	}
}

const QFont CBTConfig::getDefault( const CBTConfig::fonts ID){
	switch ( ID ){
		case standard: 	return QApplication::font();
		case unicode: 	return QApplication::font();
	}
}

const QString CBTConfig::getKey( const CBTConfig::bools ID){
	switch ( ID ){
		case firstSearchDialog:		return "firstSearchDialog";

		case toolbar:							return "toolbar";
		case mainIndex:						return "mainIndex";

		case autoTile:						return "autoTile";
		case autoCascade:					return "autoCascade";

		case lexiconCache: 				return "lexicon_chache";

		case footnotes: 					return "footnotes";
		case strongNumbers:  			return "strongNumbers";
  	case headings: 						return "headings";
  	case morphTags: 					return "morphTags";
		case lemmas: 							return "lemmas";
		case hebrewPoints: 				return "hebrewPoints";
		case hebrewCantillation: 	return "hebrewCantillation";
		case greekAccents: 				return "greekAccents";

		case lineBreaks: 					return "lineBreaks";
		case verseNumbers: 				return "verseNumbers";
		case scroll:			 				return "scroll";

		case tips: 								return "RunOnStart";
		case logo: 								return "logo";
		case restoreWorkspace: 		return "restoreWorkspace";

	}
}

const bool CBTConfig::getDefault( const CBTConfig::bools ID){
	switch ( ID ){
		case firstSearchDialog:		return true;

		case toolbar:							return true;
		case mainIndex:						return true;

		case autoTile:						return true;
		case autoCascade:					return false;

		case lexiconCache: 				return true;

		case footnotes: 					return true;
		case strongNumbers:  			return true;
  	case headings: 						return true;
  	case morphTags: 					return true;
		case lemmas: 							return true;
		case hebrewPoints: 				return true;
		case hebrewCantillation: 	return true;
		case greekAccents: 				return true;

		case lineBreaks: 					return true;
		case verseNumbers: 				return true;
		case scroll:			 				return true;

		case tips: 								return true;
		case logo: 								return true;
		case restoreWorkspace: 		return false;

	}
}

const QString CBTConfig::getKey( const CBTConfig::colors ID){
	switch ( ID ){
		case textColor: 							return "textColor";
		case backgroundColor: 				return "backgroundColor";
		case highlightedVerseColor: 	return "highlightedVerseClolor";
		case footnotesColor: 					return "footnotesColor";
		case strongsColor: 						return "strongsColor";
		case morphsColor: 						return "morphsColor";
		case jesuswordsColor: 				return "jesuswordsColor";
		case swordRefColor: 					return "swordRefColor";
	}
}

const QColor CBTConfig::getDefault( const CBTConfig::colors ID){
	switch ( ID ){
		case textColor: 							return QColor(Qt::black);
		case backgroundColor: 				return QColor(Qt::white);
		case highlightedVerseColor:		return QColor(Qt::red);
		case footnotesColor: 					return QColor(Qt::black);
		case strongsColor: 						return QColor(Qt::green);
		case morphsColor: 						return QColor(Qt::green);
		case jesuswordsColor: 				return QColor(Qt::red);
		case swordRefColor: 					return QColor(Qt::blue);
	}
}

const QString CBTConfig::getKey( const CBTConfig::intLists ID){
	switch ( ID ){
		case splitterSizes: return "splitterSizes";
	}
}

const QValueList<int> CBTConfig::getDefault( const CBTConfig::intLists ID){
	switch ( ID ){
		case splitterSizes: return QValueList<int>();
	}
}

const QString CBTConfig::getKey( const CBTConfig::stringLists ID){
	switch ( ID ){
		case searchCompletionTexts: return "searchCompletionTexts";
		case searchTexts: return "searchTexts";		
	}
}

const QStringList CBTConfig::getDefault( const CBTConfig::stringLists ID){
	switch ( ID ){
		case searchTexts: return QStringList();
	}
}

const QString CBTConfig::get( const CBTConfig::strings ID){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "strings");
	return config->readEntry(getKey(ID),getDefault(ID));
}

const QFont CBTConfig::get(const CBTConfig::fonts ID){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "fonts");
	QFont defaultFont = getDefault(ID);
	return config->readFontEntry(getKey(ID), &defaultFont);
}

const bool CBTConfig::get( const CBTConfig::bools ID){
	//specila behaviour for the KTipDialog class
	KConfig* config = KGlobal::config();	
	KConfigGroupSaver groupSaver(config, (ID == CBTConfig::tips) ? "TipOfDay" : "bools");		
	return config->readBoolEntry(getKey(ID),getDefault(ID));
}

const QColor CBTConfig::get( const CBTConfig::colors ID){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "colors");
	QColor defaultColor = getDefault(ID);
	return config->readColorEntry(getKey(ID),&defaultColor);
}

const QValueList<int>	CBTConfig::get( const CBTConfig::intLists ID ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "lists");
	return config->readIntListEntry(getKey(ID));
}

const QStringList	CBTConfig::get( const CBTConfig::stringLists ID ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "lists");
	return config->readListEntry(getKey(ID));
}

void CBTConfig::set( const CBTConfig::strings ID, const QString value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "strings");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set( const CBTConfig::fonts ID, const QFont value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "fonts");
	config->writeEntry(getKey(ID), value);
}

void CBTConfig::set(const  CBTConfig::bools ID,const  bool value ){
	KConfig* config = KGlobal::config();
	//special behaviour to work with KTipDialog class of KDE
	KConfigGroupSaver groupSaver(config, (ID == CBTConfig::tips) ? "TipOfDay" : "bools");
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
	KConfigGroupSaver groupSaver(config, "lists");
	config->writeEntry(getKey(ID), value);
}



const CSwordBackend::DisplayOptionsBool CBTConfig::getDisplayOptionDefaults(){

  CSwordBackend::DisplayOptionsBool options;

	options.lineBreaks = 		get(CBTConfig::lineBreaks);
  options.verseNumbers = 	get(CBTConfig::verseNumbers);

	return options;
}

const CSwordBackend::FilterOptionsBool CBTConfig::getFilterOptionDefaults(){

  CSwordBackend::FilterOptionsBool options;

  options.footnotes =						get(CBTConfig::footnotes);
  options.strongNumbers = 			get(CBTConfig::strongNumbers);
  options.headings = 						get(CBTConfig::headings);
  options.morphTags = 					get(CBTConfig::morphTags);
  options.lemmas = 							get(CBTConfig::lemmas);
  options.hebrewPoints = 				get(CBTConfig::hebrewPoints);
  options.hebrewCantillation = 	get(CBTConfig::hebrewCantillation);
  options.greekAccents = 				get(CBTConfig::greekAccents);

	return options;
}
