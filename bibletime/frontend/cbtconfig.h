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
#include <qmap.h>

#include "backend/cswordbackend.h"
#include "backend/clanguagemgr.h"

//Forward declarations
class KAccel;

/**
 * This class is the interface to the config object of BibleTime
 * @author The BibleTime team
 */
class CBTConfig {
public:
  typedef QMap<QString, QString> StringMap;
	enum strings {
		bibletimeVersion,
		language,
		standardBible,
		standardCommentary,
		standardLexicon,
		standardDailyDevotional,    
		standardHebrewStrongsLexicon,
		standardGreekStrongsLexicon,
		standardHebrewMorphLexicon,
		standardGreekMorphLexicon
	};
//	enum fonts {
//		standard,
//		unicode
//	};
	enum colors {
		textColor,
		backgroundColor,
		background2Color,
		highlightedVerseColor,
		footnotesColor,
		strongsColor,
		morphsColor,
		jesuswordsColor,
		swordRefColor  //SWORD hyperlink
	};
	enum bools {
		firstSearchDialog,
    readOldBookmarks,

		toolbar,
		mainIndex,

		autoTile,
		autoCascade,

		lexiconCache,

		lineBreaks,
		verseNumbers,
		scroll,

		tips,
		logo,
		restoreWorkspace,
    crashedLastTime,
    crashedTwoTimes
	};
	enum ints {
  	footnotes,
  	strongNumbers,
  	headings,
  	morphTags,
		lemmas,
		hebrewPoints,
		hebrewCantillation,
		greekAccents,
		textualVariants,
    transliteration,
    scriptureReferences
  };
  enum intLists {
		splitterSizes
	};
	enum stringLists {
		searchCompletionTexts,
		searchTexts
	};
	enum keys {		
	  readWindow,		
		writeWindow,		
	  bookWindow,
		bibleWindow,
		commentaryWindow,
		lexiconWindow,
		application
	};
  enum stringMaps {
    searchScopes
  };
  typedef std::pair<bool, QFont> FontSettingsPair;
  
  static const QString 		get( const CBTConfig::strings );
  static const bool 			get( const CBTConfig::bools );
  static const int   			get( const CBTConfig::ints );
  static const QColor 		get( const CBTConfig::colors );
  static const QValueList<int>	get( const CBTConfig::intLists );
  static const QStringList	get( const CBTConfig::stringLists );
  static const CBTConfig::StringMap	get( const CBTConfig::stringMaps );

  static const FontSettingsPair	get( const CLanguageMgr::Language& );
	
	static const QString 				 getDefault( const CBTConfig::strings );
	static const bool		 				 getDefault( const CBTConfig::bools );
	static const int		 				 getDefault( const CBTConfig::ints );
	static const QColor	 				 getDefault( const CBTConfig::colors );
	static const QValueList<int> getDefault( const CBTConfig::intLists );
	static const QStringList		 getDefault( const CBTConfig::stringLists );
	static const CBTConfig::StringMap  		 getDefault( const CBTConfig::stringMaps );
  static const QFont 		       getDefault( const CLanguageMgr::Language& );
    
	static void set( const CBTConfig::strings, 	const QString value );
	static void set( const CBTConfig::bools, 		const bool value );
	static void set( const CBTConfig::ints, 		const int value );
	static void set( const CBTConfig::colors, 	const QColor value );
	static void set( const CBTConfig::intLists,	const QValueList<int> value );
  static void set( const CBTConfig::stringLists, const QStringList value);
  static void set( const CBTConfig::stringMaps, const CBTConfig::StringMap value);
  static void	set( const CLanguageMgr::Language& language, const FontSettingsPair& fontSettings );    

  static const CSwordBackend::FilterOptions getFilterOptionDefaults();
  static const CSwordBackend::DisplayOptions getDisplayOptionDefaults();
  static void setupAccel(const CBTConfig::keys type, KAccel* const accel);

  static const QString getModuleEncryptionKey( const QString& name );
  static void setModuleEncryptionKey( const QString& name, const QString& key );
  
private:
	static const QString getKey( const CBTConfig::strings );
	static const QString getKey( const CBTConfig::bools );
	static const QString getKey( const CBTConfig::ints );
	static const QString getKey( const CBTConfig::colors );
	static const QString getKey( const CBTConfig::intLists );
	static const QString getKey( const CBTConfig::stringLists );
	static const QString getKey( const CBTConfig::stringMaps );
	static const QString getKey( const CLanguageMgr::Language& );    
};


#endif
