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

/* 	No constructor and destructor, because this class only contains static methods.
		It won't be instantiated. */

QString CBTConfig::getKey( CBTConfig::strings ID){
}
QString CBTConfig::getDefault( CBTConfig::strings ID){
}

QString CBTConfig::getKey( CBTConfig::fonts ID){
}
QFont CBTConfig::getDefault( CBTConfig::fonts ID){
}

QString CBTConfig::getKey( CBTConfig::bools ID){
	switch ( ID ){
		case lexicon_cache: 			return "lexicon_chache";

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
	}
}
bool CBTConfig::getDefault( CBTConfig::bools ID){
	switch ( ID ){
		case lexicon_cache: 			return true;

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
	}
}

QString CBTConfig::getKey( CBTConfig::colors ID){
}
QColor CBTConfig::getDefault( CBTConfig::colors ID){
}



/** No descriptions */
QString CBTConfig::get( CBTConfig::strings ID){
	KConfig* config = KGlobal::config();
	return config->readEntry(getKey(ID),getDefault(ID));
}
/** No descriptions */
QFont CBTConfig::get(CBTConfig::fonts ID){
	KConfig* config = KGlobal::config();
	QFont defaultFont = getDefault(ID);
	return config->readFontEntry(getKey(ID), &defaultFont);
}
/** No descriptions */
bool CBTConfig::get( CBTConfig::bools ID){
	KConfig* config = KGlobal::config();
	return config->readBoolEntry(getKey(ID),getDefault(ID));
}
/** No descriptions */
QColor CBTConfig::get( CBTConfig::colors ID){
	KConfig* config = KGlobal::config();
	QColor defaultColor = getDefault(ID);
	return config->readColorEntry(getKey(ID),&defaultColor);
}

void CBTConfig::set( CBTConfig::strings ID, QString value ){
	KConfig* config = KGlobal::config();
	config->writeEntry(getKey(ID), value);
}
void CBTConfig::set( CBTConfig::fonts ID, QFont value ){
	KConfig* config = KGlobal::config();
	config->writeEntry(getKey(ID), value);
}
void CBTConfig::set( CBTConfig::bools ID, bool value ){
	KConfig* config = KGlobal::config();
	config->writeEntry(getKey(ID), value);
}
void CBTConfig::set( CBTConfig::colors ID, QColor value ){
	KConfig* config = KGlobal::config();
	config->writeEntry(getKey(ID), value);
}

CSwordBackend::displayOptionsBool CBTConfig::getAllDisplayOptionDefaults( void ){
  KConfig* config = KGlobal::config();

  CSwordBackend::displayOptionsBool options;

	options.lineBreaks = 					config->readBoolEntry( "lineBreaks", true );
  options.verseNumbers = 				config->readBoolEntry( "verseNumbers", true );

	return options;
}

CSwordBackend::moduleOptionsBool CBTConfig::getAllModuleOptionDefaults( void ){
  KConfig* config = KGlobal::config();

  CSwordBackend::moduleOptionsBool options;

  options.footnotes = 					config->readBoolEntry( "footnotes", true );
  options.strongNumbers = 			config->readBoolEntry( "strongNumbers", true );
  options.headings = 						config->readBoolEntry( "headings", true );
  options.morphTags = 					config->readBoolEntry( "morphTags", true );
  options.lemmas = 							config->readBoolEntry( "lemmas", true );
  options.hebrewPoints = 				config->readBoolEntry( "hebrewPoints", true );
  options.hebrewCantillation = 	config->readBoolEntry( "hebrewCantillation", true );
  options.greekAccents = 				config->readBoolEntry( "greekAccents", true );

	return options;
}


