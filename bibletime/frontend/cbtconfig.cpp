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
#include <kconfig.h>
#include <kcharsets.h>
#include <kglobal.h>
#include <klocale.h>


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
	return QString::null;
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
	return QString::null;
}

const QString CBTConfig::getKey( const CBTConfig::fonts ID){
	switch ( ID ){
		case standard: 	return "standard";
		case unicode: 	return "unicode";
	}
	return QString::null;	
}

const QFont CBTConfig::getDefault( const CBTConfig::fonts ID){
	switch ( ID ){
		case standard: 	return QApplication::font();
		case unicode: //find an Unicode font
		{	
			KCharsets* cs = KGlobal::charsets();
			QFontDatabase db;
			QFont uniFont;
			if (!cs->isAvailable(QFont::Unicode)) { //no unicode font installed!
				qWarning("NO UNICODE FONT INSTALLED!");
			}			
			cs->setQFont(uniFont, QFont::Unicode);
//			qWarning("the unicode font found by KDE is %s", uniFont.family().latin1());
			
//			QStringList families = KGlobal::charsets()->availableFamilies(QFont::Unicode);
//			for (QStringList::Iterator it = families.begin();it != families.end();++it) {
//				qWarning((*it).latin1());
//				if (!cs->hasUnicode(*it))
//					qWarning("font %s DOESN'T have UNICODE!", (*it).latin1());				
//			}			
//
//			QMap<QString, int> uniFontMap;
//			for (QStringList::Iterator it = families.begin();it != families.end();++it) {
//				const QString font = *it;
//				if (db.isScalable(font)) {
//					if (db.isSmoothlyScalable(font)) {
//						qWarning("font %s is SMOOTHLY scalable!", font.latin1());								
//					}
//					else if (db.isBitmapScalable(font)) {
//						qWarning("font %s is BITMAP scalable!", font.latin1());								
//					}
//					else {
//						qWarning("font %s is SMOOTHLY scalable!", font.latin1());								
//					}					
//				}
//				
//				//find how many charsets are supported!
//				QList<QFont::CharSet> fonts = cs->availableCharsets(font);//db.charSets(font).count()				
//				uniFontMap.insert(font, fonts.count());
//				
//				fonts.setAutoDelete(true);
//				fonts.clear();
//			}
//			
//			//now find the unicode font with the most charsets
//			QString bestFont = uniFont.family();
//			int oldCount = 0;
//			for (QMap<QString, int>::Iterator it = uniFontMap.begin(); it != uniFontMap.end(); ++it) {			
//				if (it.data() > oldCount) {
//					oldCount = it.data();
//					bestFont = it.key();
//				}
//			}
//			qWarning("the best font is %s and supports %i charsets", bestFont.latin1(), oldCount);
			
//			qWarning("--------");			
//			QStringList fonts = db.families(false);
//			for (QStringList::Iterator it = fonts.begin();it != fonts.end();++it) {
//				QStringList charsets = db.charSets(*it);
//				for (QStringList::Iterator cs = charsets.begin(); cs != charsets.end(); ++cs) {
//					qWarning("Font %s has charset %s", (*it).latin1(), (*cs).latin1());
//				}
//			}
//			qWarning("--------");						
			return uniFont;
		}
		
	}
	return QString::null;	
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
		case textualVariants:			return "textualVariants";		

		case lineBreaks: 					return "lineBreaks";
		case verseNumbers: 				return "verseNumbers";
		case scroll:			 				return "scroll";

		case tips: 								return "RunOnStart";
		case logo: 								return "logo";
		case restoreWorkspace: 		return "restoreWorkspace";
	}
	return QString::null;	
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
		case strongNumbers:  			return false;
  	case headings: 						return true;
  	case morphTags: 					return false;
		case lemmas: 							return false;
		case hebrewPoints: 				return false;
		case hebrewCantillation: 	return false;
		case greekAccents: 				return false;
		case textualVariants:			return false;

		case lineBreaks: 					return true;
		case verseNumbers: 				return true;
		case scroll:			 				return true;

		case tips: 								return true;
		case logo: 								return true;
		case restoreWorkspace: 		return false;
	}
	return false;
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
	return QString::null;
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
		case searchTexts:
			return QStringList();
		case searchCompletionTexts:
			return QStringList();
	}
	return QStringList();
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
	//special behaviour for the KTipDialog class
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
