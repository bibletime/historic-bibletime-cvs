/***************************************************************************
                          cbtconfig.h  -  description
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

#ifndef CBTCONFIG_H
#define CBTCONFIG_H

#include <qstring.h>
#include <qcolor.h>
#include <qfont.h>
#include <qvaluelist.h>

#include "../backend/cswordbackend.h"
/**This class is the interface to the config object of BibleTime
  *@author The BibleTime team
  */

class CBTConfig {
public:

	enum strings{
		language,
		standardBible,
		standardCommentary,
		standardLexicon,
		standardHebrewStrongsLexicon,
		standardGreekStrongsLexicon,
		standardHebrewMorphLexicon,
		standardGreekMorphLexicon
	};
	enum fonts{
		standard,
		unicode
	};
	enum colors{
		textColor,
		backgroundColor,
		highlightedVerseColor,
		footnotesColor,
		strongsColor,
		morphsColor,
		jesuswordsColor
	};
	enum bools{
		firstStartUp,
		firstSearchDialog,
		isConfigured,

		toolbar,
		mainIndex,

		autoTile,
		autoCascade,

		lexiconCache,

  	footnotes,
  	strongNumbers,
  	headings,
  	morphTags,
		lemmas,
		hebrewPoints,
		hebrewCantillation,
		greekAccents,

		lineBreaks,
		verseNumbers,
		scroll,

		tips,
		logo,
		restoreWorkspace
	};
	enum intLists{
		splitterSizes
	};
	enum stringLists{
		searchTexts
	};

  static QString 		get( CBTConfig::strings );
  static QFont 			get( CBTConfig::fonts );
  static bool 			get( CBTConfig::bools );
  static QColor 		get( CBTConfig::colors );
  static QValueList<int>	get( CBTConfig::intLists );
  static QStringList			get( CBTConfig::stringLists );
	

	static void set( CBTConfig::strings, 	QString value );
	static void set( CBTConfig::fonts, 		QFont value );
	static void set( CBTConfig::bools, 		bool value );
	static void set( CBTConfig::colors, 	QColor value );
	static void set( CBTConfig::intLists,	QValueList<int> value );
  static void set( CBTConfig::stringLists, QStringList value);


  static CSwordBackend::moduleOptionsBool getAllModuleOptionDefaults( void );
  static CSwordBackend::displayOptionsBool getAllDisplayOptionDefaults( void );

private:
	static QString getKey( CBTConfig::strings );
	static QString getKey( CBTConfig::fonts );
	static QString getKey( CBTConfig::bools );
	static QString getKey( CBTConfig::colors );
	static QString getKey( CBTConfig::intLists );
	static QString getKey( CBTConfig::stringLists );

	static QString 				 getDefault( CBTConfig::strings );
	static QFont	 				 getDefault( CBTConfig::fonts );
	static bool		 				 getDefault( CBTConfig::bools );
	static QColor	 				 getDefault( CBTConfig::colors );
	static QValueList<int> getDefault( CBTConfig::intLists );
	static QStringList		 getDefault( CBTConfig::stringLists );
};


#endif
