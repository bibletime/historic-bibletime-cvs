/***************************************************************************
                          clanguagemgr.cpp  -  description
                             -------------------
    begin                : Mon Okt 21 2002
    copyright            : (C) 2002 by The BibleTime team
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

#include "clanguagemgr.h"

//KDE includes
#include <klocale.h>

CLanguageMgr::Language::Language() {
  m_abbrev = QString::null;
  m_englishName = QString::null;
  m_translatedName = QString::null;
};

CLanguageMgr::Language::Language( const QString& abbrev, const QString& name, const QString& translatedName ) {
  m_abbrev = abbrev;
  m_englishName = name;
  m_translatedName = translatedName;
};

CLanguageMgr::Language::~Language() {
};

/** Returns true if this language object is valid, i.e. has an abbrev and name. */
const bool CLanguageMgr::Language::isValid(){
  return (!m_abbrev.isEmpty() && !m_englishName.isEmpty());
}

const QString& CLanguageMgr::Language::abbrev() {
  return m_abbrev;
};

const QString& CLanguageMgr::Language::translatedName() {
  return m_translatedName;
};

const QString& CLanguageMgr::Language::name() {
  return m_englishName;
};

const QPixmap CLanguageMgr::Language::flag() {
  return QPixmap();
};


/****************************************************/
/******************** CLanguageMgr ******************/
/****************************************************/
CLanguageMgr::CLanguageMgr(){
  init();

  //some tests
  Language lang = languageForName("German");
  qWarning("abbrev of language German is %s", lang.abbrev().latin1());

  lang = languageForTranslatedName("Deutsch");
  qWarning("abbrev of translated language name Deutsch is %s", lang.abbrev().latin1());

  lang =  languageForAbbrev("ar");
  qWarning("language name of abbrev ar is %s", lang.name().latin1());

  lang =  languageForAbbrev("sd");
  qWarning("language name of abbrev sd is %s and is valid? %i", lang.name().latin1(), lang.isValid());
  
  //print out all languages
  LangMap::Iterator it;
  LangMap map = languages();
  for ( it = map.begin(); it != map.end(); ++it ) {
    qWarning("language %s: %s (%s)", it.data().abbrev().latin1(), it.data().name().latin1(),it.data().translatedName().latin1());
  };
  
}

CLanguageMgr::~CLanguageMgr(){
}

const CLanguageMgr::LangMap& CLanguageMgr::languages() {
  return m_langMap;
};

const CLanguageMgr::LangMap CLanguageMgr::availableLanguages() {

};

const CLanguageMgr::Language CLanguageMgr::languageForAbbrev( const QString& abbrev ) {
  if (m_langMap.contains(abbrev)) {
    return m_langMap[abbrev];
  };
  return Language(); //invalid language
};

const CLanguageMgr::Language CLanguageMgr::languageForName( const QString& name ) {
  LangMap::Iterator it;
  for ( it = m_langMap.begin(); it != m_langMap.end(); ++it ) {
    if (it.data().name() == name) {
      return it.data();
    };
  };
  return Language();//invalid language
};

const CLanguageMgr::Language CLanguageMgr::languageForTranslatedName( const QString& name ) {
  LangMap::Iterator it;
  for ( it = m_langMap.begin(); it != m_langMap.end(); ++it ) {
    if (it.data().translatedName() == name) {
      return it.data();
    };
  };
  return Language();//invalid language
};

void CLanguageMgr::init() {
  QValueList<Language> list;
  list.append( Language("aa"  , "Afar"        , i18n("Afar")) );
  list.append( Language("ab"  , "Abkhazian"   , i18n("Abkhazian")) );
  list.append( Language("ae"  , "Avestan"     , i18n("Avestan")) );
  list.append( Language("af"  , "Afrikaans"   , i18n("Afrikaans")) );
  list.append( Language("am"  , "Amharic"     , i18n("Amharic")) );
  list.append( Language("ar"  , "Arabic"      , i18n("Arabic")) );
  list.append( Language("as"  , "Assamese"    , i18n("Assamese")) );
  list.append( Language("ay"  , "Aymara"      , i18n("Aymara")) );
  list.append( Language("az"  , "Azerbaijani" , i18n("Azerbaijani")) );
  
  list.append( Language("ba"  , "Bashkir"     , i18n("Bashkir")) );
  list.append( Language("be"  , "Belarusian"  , i18n("Belarusian")) );
  list.append( Language("bg"  , "Bulgarian"   , i18n("Bulgarian")) );
  list.append( Language("bh"  , "Bihari"      , i18n("Bihari")) );
  list.append( Language("bi"  , "Bislama"     , i18n("Bislama")) );
  list.append( Language("bn"  , "Bengali"     , i18n("Bengali")) );
  list.append( Language("bo"  , "Tibetan"     , i18n("Tibetan")) );
  list.append( Language("br"  , "Breton"      , i18n("Breton")) );
  list.append( Language("bs"  , "Bosnian"     , i18n("Bosnian")) );

  list.append( Language("ca"  , "Catalan"     , i18n("Catalan")) );  
  list.append( Language("ce"  , "Chechen"     , i18n("Chechen")) );
  list.append( Language("ch"  , "Chamorro"    , i18n("Chamorro")) );
  list.append( Language("co"  , "Corsican"    , i18n("Corsican")) );
  list.append( Language("cs"  , "Czech"       , i18n("Czech")) );
  list.append( Language("cu"  , "Church Slavic" , i18n("Church Slavic")) );
  list.append( Language("cv"  , "Chuvash"     , i18n("Chuvash")) );
  list.append( Language("cy"  , "Welsh"       , i18n("Welsh")) );

  list.append( Language("da"  , "Danish"      , i18n("Danish")) );
  list.append( Language("de"  , "German"      , i18n("German")) );
  list.append( Language("dz"  , "Dzongkha"    , i18n("Dzongkha")) );

  list.append( Language("el"  , "Greek, Modern (1453-)"     , i18n("Greek, Modern (1453-)")) );
  list.append( Language("en"  , "English"     , i18n("English")) );
  list.append( Language("eo"  , "Esperanto"   , i18n("Esperanto")) );
  list.append( Language("es"  , "Spanish"     , i18n("Spanish")) );
  list.append( Language("et"  , "Estonian"    , i18n("Estonian")) );
  list.append( Language("eu"  , "Basque"      , i18n("Basque")) );

  list.append( Language("fa"  , "Persian"     , i18n("Persian")) );
  list.append( Language("fi"  , "Finnish"     , i18n("Finnish")) );
  list.append( Language("fj"  , "Fijian"      , i18n("Fijian")) );
  list.append( Language("fo"  , "Faroese"     , i18n("Faroese")) );
  list.append( Language("fr"  , "French"      , i18n("French")) );
  list.append( Language("fy"  , "Frisian"     , i18n("Frisian")) );

  list.append( Language("ga"  , "Irish"       , i18n("Irish")) );
  list.append( Language("gd"  , "Gaelic (Scots)"     , i18n("Gaelic (Scots)")) );
  list.append( Language("gl"  , "Gallegan"    , i18n("Gallegan")) );
  list.append( Language("gn"  , "Guarani"     , i18n("Guarani")) );
  list.append( Language("gn"  , "Gujarati"    , i18n("Gujarati")) );
  list.append( Language("gv"  , "Manx"        , i18n("Manx")) );

  list.append( Language("he"  , "Hebrew"      , i18n("Hebrew")) );
  list.append( Language("hi"  , "Hindi"       , i18n("Hindi")) );
  list.append( Language("ho"  , "Hiri Motu"   , i18n("Hiri Motu")) );
  list.append( Language("hr"  , "Croatian"    , i18n("Croatian")) );
  list.append( Language("hu"  , "Hungarian"   , i18n("Hungarian")) );
  list.append( Language("hy"  , "Armenian"    , i18n("Armenian")) );
  list.append( Language("hz"  , "Herero"      , i18n("Herero")) );

  list.append( Language("ia"  , "Interlingua" , i18n("Interlingua")) );
  list.append( Language("id"  , "Indonesian"  , i18n("Indonesian")) );
  
    
  QValueList<Language>::iterator it;
  for ( it = list.begin(); it != list.end(); ++it ) {
    m_langMap.insert((*it).abbrev(), *it);
  };
};
