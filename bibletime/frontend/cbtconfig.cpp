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

QString CBTConfig::getKey( CBTConfig::strings ID){
	switch ( ID ){
		case language: 							return "language";
		case standardBible: 				return "standardBible";
		case standardCommentary: 		return "standardCommentary";
		case standardLexicon: 			return "standardLexicon";
		case standardHebrewLexicon: return "standardHebrewLexicon";
		case standardGreekLexicon: 	return "standardGreekLexicon";
		case standardHebrewMorphLexicon:	return "standardHebrewMorphLexicon";
		case standardGreekMorphLexicon:		return "standardGreekMorphLexicon";
	}
}

QString CBTConfig::getDefault( CBTConfig::strings ID){
	switch ( ID ){
		case language: 							return (KGlobal::locale()->language()).local8Bit();
		case standardBible: 				return "NIV";  // no effect
		case standardCommentary: 		return "MHC";
		case standardLexicon: 			return "ISBE";
		case standardHebrewLexicon: return "StrongsHebrew";
		case standardGreekLexicon: 	return "StrongsGreek";
		case standardHebrewMorphLexicon:	return "StrongsHebrew"; //warning this is wrong
		case standardGreekMorphLexicon:		return "StrongsGreek";  //but imo has no effect
	}
}

QString CBTConfig::getKey( CBTConfig::fonts ID){
	switch ( ID ){
		case standard: 	return "standard";
		case unicode: 	return "unicode";
	}
}
QFont CBTConfig::getDefault( CBTConfig::fonts ID){
	switch ( ID ){
		case standard: 	return QApplication::font();
		case unicode: 	return QApplication::font();
	}
}

QString CBTConfig::getKey( CBTConfig::bools ID){
	switch ( ID ){
		case firstStartUp:				return "firstStartUp";
		case firstSearchDialog:		return "firstSearchDialog";
		case isConfigured:				return "isConfigured";

		case toolbar:							return "toolbar";
		case mainIndex:						return "mainIndex";

		case autoTile:							return "autoTile";
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

		case tips: 								return "tips";
		case logo: 								return "logo";
		case restoreWorkspace: 		return "restoreWorkspace";

	}
}
bool CBTConfig::getDefault( CBTConfig::bools ID){
	switch ( ID ){
		case firstStartUp:				return true;
		case firstSearchDialog:		return true;
		case isConfigured:				return false;

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
QString CBTConfig::getKey( CBTConfig::colors ID){
	switch ( ID ){
		case textColor: 							return "textColor";
		case backgroundColor: 				return "backgroundColor";
		case highlightedVerseColor: 	return "highlightedVerseClolor";
		case footnotesColor: 					return "footnotesColor";
		case strongsColor: 						return "strongsColor";
		case morphsColor: 						return "morphsColor";
		case jesuswordsColor: 				return "jesuswordsColor";
	}
}
QColor CBTConfig::getDefault( CBTConfig::colors ID){
	switch ( ID ){
		case textColor: 							return QColor(Qt::black);
		case backgroundColor: 				return QColor(Qt::white);
		case highlightedVerseColor:		return QColor(Qt::red);
		case footnotesColor: 					return QColor(Qt::black);
		case strongsColor: 						return QColor(Qt::green);
		case morphsColor: 						return QColor(Qt::green);
		case jesuswordsColor: 				return QColor(Qt::red);
	}
}
QString CBTConfig::getKey( CBTConfig::intLists ID){
	switch ( ID ){
		case splitterSizes: return "splitterSizes";
	}
}
QValueList<int> CBTConfig::getDefault( CBTConfig::intLists ID){
	switch ( ID ){
		case splitterSizes: return QValueList<int>();
	}
}
QString CBTConfig::getKey( CBTConfig::stringLists ID){
	switch ( ID ){
		case searchTexts: return "searchTexts";
	}
}
QStringList CBTConfig::getDefault( CBTConfig::stringLists ID){
	switch ( ID ){
		case searchTexts: return QStringList();
	}
}
QString CBTConfig::get( CBTConfig::strings ID){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "strings");
	return config->readEntry(getKey(ID),getDefault(ID));
}
QFont CBTConfig::get(CBTConfig::fonts ID){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "fonts");
	QFont defaultFont = getDefault(ID);
	return config->readFontEntry(getKey(ID), &defaultFont);
}
bool CBTConfig::get( CBTConfig::bools ID){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "bools");
	return config->readBoolEntry(getKey(ID),getDefault(ID));
}
QColor CBTConfig::get( CBTConfig::colors ID){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "colors");
	QColor defaultColor = getDefault(ID);
	return config->readColorEntry(getKey(ID),&defaultColor);
}
QValueList<int>	CBTConfig::get( CBTConfig::intLists ID ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "lists");
	return config->readIntListEntry(getKey(ID));
}
QStringList	CBTConfig::get( CBTConfig::stringLists ID ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "lists");
	return config->readListEntry(getKey(ID));
}
void CBTConfig::set( CBTConfig::strings ID, QString value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "strings");
	config->writeEntry(getKey(ID), value);
}
void CBTConfig::set( CBTConfig::fonts ID, QFont value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "fonts");
	config->writeEntry(getKey(ID), value);
}
void CBTConfig::set( CBTConfig::bools ID, bool value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "bools");
	config->writeEntry(getKey(ID), value);
}
void CBTConfig::set( CBTConfig::colors ID, QColor value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "colors");
	config->writeEntry(getKey(ID), value);
}
void CBTConfig::set( CBTConfig::intLists ID, QValueList<int> value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "lists");
	config->writeEntry(getKey(ID), value);
}
void CBTConfig::set( CBTConfig::stringLists ID, QStringList value ){
	KConfig* config = KGlobal::config();
	KConfigGroupSaver groupSaver(config, "lists");
	config->writeEntry(getKey(ID), value);
}



CSwordBackend::displayOptionsBool CBTConfig::getAllDisplayOptionDefaults( void ){

  CSwordBackend::displayOptionsBool options;

	options.lineBreaks = 		get(CBTConfig::lineBreaks);
  options.verseNumbers = 	get(CBTConfig::verseNumbers);

	return options;
}

CSwordBackend::moduleOptionsBool CBTConfig::getAllModuleOptionDefaults( void ){

  CSwordBackend::moduleOptionsBool options;

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
