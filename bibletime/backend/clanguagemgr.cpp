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

#include "cswordmoduleinfo.h"

#include "util/cpointers.h"

//KDE includes
#include <klocale.h>

//initialize static language list
static QValueList<CLanguageMgr::Language> m_langList;
static CLanguageMgr::Language m_defaultLanguage;

CLanguageMgr::Language::Language() {
  m_abbrev = QString::null;
  m_altAbbrevs = QStringList();
  m_englishName = QString::null;
  m_translatedName = QString::null;
};

CLanguageMgr::Language::Language( const QString& abbrev, const QString& name, const QString& translatedName, const QStringList altAbbrevs ) {
  m_abbrev = abbrev;
  m_altAbbrevs = altAbbrevs;
  m_englishName = name;
  m_translatedName = translatedName;
};

CLanguageMgr::Language::~Language() {
};

/** Returns true if this language object is valid, i.e. has an abbrev and name. */
const bool CLanguageMgr::Language::isValid(){
  return (!abbrev().isEmpty() && !name().isEmpty());
}

const QString& CLanguageMgr::Language::abbrev() const {
  if (m_abbrev.isEmpty() && m_altAbbrevs.count()) { //no standard abbrev but alternative ones
    return m_altAbbrevs.first();
  };
  return m_abbrev;
};

const QString& CLanguageMgr::Language::translatedName() const {
  return m_translatedName;
};

const QStringList& CLanguageMgr::Language::alternativeAbbrevs() const {
  return m_altAbbrevs;
};

const QPixmap CLanguageMgr::Language::flag() {
  return QPixmap();
};


/****************************************************/
/******************** CLanguageMgr ******************/
/****************************************************/
CLanguageMgr::CLanguageMgr(){
  init();
}

CLanguageMgr::~CLanguageMgr(){
}

const CLanguageMgr::LangMap& CLanguageMgr::languages() const {
  return m_langMap;
};

const CLanguageMgr::LangMap CLanguageMgr::availableLanguages() {
  LangMap map;
  
  //collect the languages abbrevs of all modules
  ListCSwordModuleInfo mods = CPointers::backend()->moduleList();
  QStrList abbrevs;
  char *abbrev;  

  for (CSwordModuleInfo* m = mods.first(); m; m = mods.next()) {
    abbrev = m->module()->Lang();
    if (abbrev && !abbrevs.contains(abbrev)) {
      abbrevs.append( abbrev );
    }
  };

  //now create a map of available langs
  Language lang;
  for ( abbrev = abbrevs.first(); abbrev; abbrev = abbrevs.next() ) {
    lang = languageForAbbrev(abbrev);
    if (lang.isValid()) {
      map.insert( abbrev, lang );
    }
    else {
      map.insert( abbrev, Language(abbrev, abbrev, abbrev) );
    }
  };

  return map;
};

const CLanguageMgr::Language& CLanguageMgr::languageForAbbrev( const QString& abbrev ) {
  if (m_langMap.contains(abbrev)) {
    return m_langMap[abbrev];
  };

  //try to search in the alternative abbrevs
  LangMap::Iterator it;
  for ( it = m_langMap.begin(); it != m_langMap.end(); ++it ) {
    if (it.data().alternativeAbbrevs().contains(abbrev)) {
      return it.data();
    };
  }  
  return m_defaultLanguage; //invalid language
};

const CLanguageMgr::Language& CLanguageMgr::languageForName( const QString& name ) {
  LangMap::Iterator it;
  for ( it = m_langMap.begin(); it != m_langMap.end(); ++it ) {
    if (it.data().name() == name) {
      return it.data();
    };
  };
  return m_defaultLanguage;//invalid language
};

const CLanguageMgr::Language& CLanguageMgr::languageForTranslatedName( const QString& name ) {
  LangMap::Iterator it;
  for ( it = m_langMap.begin(); it != m_langMap.end(); ++it ) {
    if (it.data().translatedName() == name) {
      return it.data();
    };
  };
  return m_defaultLanguage;//invalid language
};

void CLanguageMgr::init() {
  //if we've already inserted all items we do not proceed
  if (m_langMap.count())
    return;

/*
* Chris explained in an eMail how language codes are build:

Preference order for locale codes are:

ISO 639-1
ISO 639-2
Ethnologue (http://www.ethnologue.org/)

We intend to always follow OSIS locale conventions, which state that
Ethnologue codes will be encoded in the format x-E-??? where ???
represents the 3-letter Ethnologue code in capital letters (though
capitalization really doesn't matter here).

Some older modules maintain a former format for Ethnolgoue codes of
xx-???, including the AleWiesler module.

--Chris

*/    
    
    
//  m_langList.append( Language("aa"  , "Afar"        , i18n("Afar")) );
//  m_langList.append( Language("ab"  , "Abkhazian"   , i18n("Abkhazian")) );
//  m_langList.append( Language("ae"  , "Avestan"     , i18n("Avestan")) );
  m_langList.append( Language("af"  , "Afrikaans"   , i18n("Afrikaans")) );
//  m_langList.append( Language("am"  , "Amharic"     , i18n("Amharic")) );
  m_langList.append( Language("ang", "English, Old (ca.450-1100)", i18n("English, Old (ca.450-1100)")) );
  m_langList.append( Language("ar"  , "Arabic"      , i18n("Arabic")) );
//  m_langList.append( Language("as"  , "Assamese"    , i18n("Assamese")) );
//  m_langList.append( Language("ay"  , "Aymara"      , i18n("Aymara")) );
  m_langList.append( Language("az"  , "Azerbaijani" , i18n("Azerbaijani")) );

//  m_langList.append( Language("ba"  , "Bashkir"     , i18n("Bashkir")) );
  m_langList.append( Language("be"  , "Belarusian"  , i18n("Belarusian")) );
  m_langList.append( Language("bg"  , "Bulgarian"   , i18n("Bulgarian")) );
//  m_langList.append( Language("bh"  , "Bihari"      , i18n("Bihari")) );
//  m_langList.append( Language("bi"  , "Bislama"     , i18n("Bislama")) );
//  m_langList.append( Language("bn"  , "Bengali"     , i18n("Bengali")) );
//  m_langList.append( Language("bo"  , "Tibetan"     , i18n("Tibetan")) );
  m_langList.append( Language("br"  , "Breton"      , i18n("Breton")) );
  m_langList.append( Language("bs"  , "Bosnian"     , i18n("Bosnian")) );

  m_langList.append( Language("ca"  , "Catalan"     , i18n("Catalan")) );
//  m_langList.append( Language("ce"  , "Chechen"     , i18n("Chechen")) );
  m_langList.append( Language("ceb"  , "Cebuano"     , i18n("Cebuano")) );
//  m_langList.append( Language("ch"  , "Chamorro"    , i18n("Chamorro")) );
//  m_langList.append( Language("co"  , "Corsican"    , i18n("Corsican")) );
  m_langList.append( Language("cop" , "Coptic"         , i18n("Coptic")) );
  m_langList.append( Language("cs"  , "Czech"       , i18n("Czech")) );
  m_langList.append( Language("cu"  , "Church Slavic" , i18n("Church Slavic")) );
//  m_langList.append( Language("cv"  , "Chuvash"     , i18n("Chuvash")) );
  m_langList.append( Language("cy"  , "Welsh"       , i18n("Welsh")) );

  m_langList.append( Language("da"  , "Danish"      , i18n("Danish")) );
  m_langList.append( Language("de"  , "German"      , i18n("German")) );
//  m_langList.append( Language("dz"  , "Dzongkha"    , i18n("Dzongkha")) );

  m_langList.append( Language("el"  , "Greek, Modern (1453-)" , i18n("Greek, Modern (1453-)"), makeStringList("gre;ell")) );
  m_langList.append( Language("en"  , "English"     , i18n("English")) );
  m_langList.append( Language("en_US",  "American English"     , i18n("American English")) );
  m_langList.append( Language("enm" , "English, Middle (1100-1500)", i18n("English, Middle (1100-1500)")) );
  m_langList.append( Language("eo"  , "Esperanto"   , i18n("Esperanto")) );
  m_langList.append( Language("es"  , "Spanish"     , i18n("Spanish")) );
  m_langList.append( Language("et"  , "Estonian"    , i18n("Estonian")) );
  m_langList.append( Language("eu"  , "Basque"      , i18n("Basque")) );

//  m_langList.append( Language("fa"  , "Persian"     , i18n("Persian")) );
  m_langList.append( Language("fi"  , "Finnish"     , i18n("Finnish")) );
//  m_langList.append( Language("fj"  , "Fijian"      , i18n("Fijian")) );
//  m_langList.append( Language("fo"  , "Faroese"     , i18n("Faroese")) );
  m_langList.append( Language("fr"  , "French"      , i18n("French")) );
  m_langList.append( Language("fy"  , "Frisian"     , i18n("Frisian")) );

  m_langList.append( Language("ga"  , "Irish"       , i18n("Irish")) );
  m_langList.append( Language("gd"  , "Gaelic (Scots)"     , i18n("Gaelic (Scots)")) );
//  m_langList.append( Language("gl"  , "Gallegan"    , i18n("Gallegan")) );
//  m_langList.append( Language("gn"  , "Guarani"     , i18n("Guarani")) );
//  m_langList.append( Language("gn"  , "Gujarati"    , i18n("Gujarati")) );
  m_langList.append( Language("got"  , "Gothic"    , i18n("Gothic")) );
  m_langList.append( Language("gv"  , "Manx"        , i18n("Manx")) );
  m_langList.append( Language("grc"  , "Greek, Ancient (to 1453)" , i18n("Greek, Ancient (to 1453)")) );

  m_langList.append( Language("he"  , "Hebrew"      , i18n("Hebrew")) );
  m_langList.append( Language("haw"    , "Hawaiian"    , i18n("Hawaiian")) );
//  m_langList.append( Language("hi"  , "Hindi"       , i18n("Hindi")) );
//  m_langList.append( Language("ho"  , "Hiri Motu"   , i18n("Hiri Motu")) );
//  m_langList.append( Language("hr"  , "Croatian"    , i18n("Croatian")) );
  m_langList.append( Language("hu"  , "Hungarian"   , i18n("Hungarian")) );
  m_langList.append( Language("hy"  , "Armenian"    , i18n("Armenian")) );
//  m_langList.append( Language("hz"  , "Herero"      , i18n("Herero")) );

//  m_langList.append( Language("ia"  , "Interlingua" , i18n("Interlingua")) );
  m_langList.append( Language("id"  , "Indonesian"  , i18n("Indonesian")) );
//  m_langList.append( Language("ie"  , "Interlingue" , i18n("Interlingue")) );
//  m_langList.append( Language("ik"  , "Inupiaq"     , i18n("Inupiaq")) );
  m_langList.append( Language("is"  , "Icelandic"   , i18n("Icelandic")) );
  m_langList.append( Language("it"  , "Italian"     , i18n("Italian")) );
//  m_langList.append( Language("iu"  , "Inuktitut"   , i18n("Inuktitut")) );

  m_langList.append( Language("ja"  , "Japanese"    , i18n("Japanese")) );

  m_langList.append( Language("ka"  , "Georgian"    , i18n("Georgian")) );
//  m_langList.append( Language("ki"  , "Kikuyu"      , i18n("Kikuyu")) );
//  m_langList.append( Language("kj"  , "Kuanyama"    , i18n("Kuanyama")) );
//  m_langList.append( Language("kk"  , "Kazakh"      , i18n("Kazakh")) );
//  m_langList.append( Language("kl"  , "Kalaallisut" , i18n("Kalaallisut")) );
//  m_langList.append( Language("km"  , "Khmer"       , i18n("Khmer")) );
//  m_langList.append( Language("kn"  , "Kannada"     , i18n("Kannada")) );
  m_langList.append( Language("ko"  , "Korean"      , i18n("Korean")) );
//  m_langList.append( Language("ks"  , "Kashmiri"    , i18n("Kashmiri")) );
  m_langList.append( Language("ku"  , "Kurdish"     , i18n("Kurdish")) );
//  m_langList.append( Language("kv"  , "Komi"        , i18n("Komi")) );
//  m_langList.append( Language("kw"  , "Cornish"     , i18n("Cornish")) );
  m_langList.append( Language("ky"  , "Kirghiz"     , i18n("Kirghiz")) );

  m_langList.append( Language("la"  , "Latin"       , i18n("Latin")) );
//  m_langList.append( Language("lb"  , "Letzeburgesch" , i18n("Letzeburgesch")) );
//  m_langList.append( Language("ln"  , "Lingala"     , i18n("Lingala")) );
//  m_langList.append( Language("lo"  , "Lao"         , i18n("Lao")) );
//  m_langList.append( Language("lt"  , "Lithuanian"  , i18n("Lithuanian")) );
  m_langList.append( Language("lv"  , "Latvian"     , i18n("Latvian")) );

//  m_langList.append( Language("mg"  , "Malagasy"    , i18n("Malagasy")) );
//  m_langList.append( Language("mh"  , "Marshall"    , i18n("Marshall")) );
  m_langList.append( Language("mi"  , "Maori"       , i18n("Maori")) );

  m_langList.append( Language("mk"  , "Macedonian"  , i18n("Macedonian")) );
//  m_langList.append( Language("ml"  , "Malayalam"   , i18n("Malayalam")) );
//  m_langList.append( Language("mn"  , "Mongolian"   , i18n("Mongolian")) );
//  m_langList.append( Language("mo"  , "Moldavian"   , i18n("Moldavian")) );
//  m_langList.append( Language("mr"  , "Marathi"     , i18n("Marathi")) );
  m_langList.append( Language("ms"  , "Malay"       , i18n("Malay")) );
  m_langList.append( Language("mt"  , "Maltese"     , i18n("Maltese")) );
//  m_langList.append( Language("my"  , "Burmese"     , i18n("Burmese")) );

//  m_langList.append( Language("na"  , "Nauru"       , i18n("Nauru")) );
//  m_langList.append( Language("nb"  , "Norwegian Bokmål"       , i18n("Norwegian Bokmål")) );
//  m_langList.append( Language("nd"  , "Ndebele, North" , i18n("Ndebele, North")) );
  m_langList.append( Language("nds"  , "Low German; Low Saxon" , i18n("Low German; Low Saxon")) );
//  m_langList.append( Language("ne"  , "Nepali"      , i18n("Nepali")) );
//  m_langList.append( Language("ng"  , "Ndonga"      , i18n("Ndonga")) );
  m_langList.append( Language("nl"  , "Dutch"       , i18n("Dutch")) );
//  m_langList.append( Language("nn"  , "Norwegian Nynorsk"     , i18n("Norwegian Nynorsk")) );
  m_langList.append( Language("no"  , "Norwegian"   , i18n("Norwegian")) );
//  m_langList.append( Language("nr"  , "Ndebele, South"     , i18n("Ndebele, South")) );
//  m_langList.append( Language("nv"  , "Navajo"      , i18n("Navajo")) );
//  m_langList.append( Language("ny"  , "Chichewa; Nyanja"      , i18n("Chichewa; Nyanja")) );

//  m_langList.append( Language("oc"  , "Occitan (post 1500); Provençal"     , i18n("Occitan (post 1500); Provençal")) );
//  m_langList.append( Language("om"  , "Oromo"       , i18n("Oromo")) );
//  m_langList.append( Language("or"  , "Oriya"       , i18n("Oriya")) );
//  m_langList.append( Language("os"  , "Ossetian; Ossetic"     , i18n("Ossetian; Ossetic")) );

//  m_langList.append( Language("pa"  , "Panjabi"     , i18n("Panjabi")) );
  m_langList.append( Language("pap"  , "Papiamento"     , i18n("Papiamento")) );
//  m_langList.append( Language("pi"  , "Pali"        , i18n("Pali")) );
  m_langList.append( Language("pl"  , "Polish"      , i18n("Polish")) );
//  m_langList.append( Language("ps"  , "Pushto"      , i18n("Pushto")) );
  m_langList.append( Language("pt"  , "Portuguese"  , i18n("Portuguese")) );

//  m_langList.append( Language("qu"  , "Quechua"     , i18n("Quechua")) );

//  m_langList.append( Language("rm"  , "Raeto-Romance"    , i18n("Raeto-Romance")) );
//  m_langList.append( Language("rn"  , "Rundi"       , i18n("Rundi")) );

  m_langList.append( Language("ro"  , "Romanian"    , i18n("Romanian")) );
  m_langList.append( Language("ru"  , "Russian"     , i18n("Russian")) );
//  m_langList.append( Language("rw"  , "Kinyarwanda" , i18n("Kinyarwanda")) );

//  m_langList.append( Language("sa"  , "Sanskrit"    , i18n("Sanskrit")) );
//  m_langList.append( Language("sc"  , "Sardinian"   , i18n("Sardinian")) );
  m_langList.append( Language("sco"  , "Scots"   , i18n("Scots")) );
//  m_langList.append( Language("sd"  , "Sindhi"      , i18n("Sindhi")) );
//  m_langList.append( Language("se"  , "Northern Sami" , i18n("Northern Sami")) );
//  m_langList.append( Language("sg"  , "Sango"       , i18n("Sango")) );
//  m_langList.append( Language("si"  , "Sinhalese"   , i18n("Sinhalese")) );
  m_langList.append( Language("sk"  , "Slovak"      , i18n("Slovak")) );
  m_langList.append( Language("sl"  , "Slovenian"   , i18n("Slovenian")) );
//  m_langList.append( Language("sm"  , "Samoan"      , i18n("Samoan")) );
//  m_langList.append( Language("sn"  , "Shona"       , i18n("Shona")) );
  m_langList.append( Language("so"  , "Somali"      , i18n("Somali")) );
  m_langList.append( Language("sq"  , "Albanian"    , i18n("Albanian")) );
//  m_langList.append( Language("sr"  , "Serbian"     , i18n("Serbian")) );
//  m_langList.append( Language("ss"  , "Swati"       , i18n("Swati")) );
//  m_langList.append( Language("st"  , "Sotho, Southern"     , i18n("Sotho, Southern")) );
//  m_langList.append( Language("su"  , "Sundanese"   , i18n("Sundanese")) );
  m_langList.append( Language("sv"  , "Swedish"     , i18n("Swedish")) );
  m_langList.append( Language("sw"  , "Swahili"     , i18n("Swahili")) );
  m_langList.append( Language("syr"  , "Syriac"     , i18n("Syriac")) );

  m_langList.append( Language("ta"  , "Tamil"       , i18n("Tamil")) );
//  m_langList.append( Language("te"  , "Telugu"      , i18n("Telugu")) );
//  m_langList.append( Language("tg"  , "Tajik"       , i18n("Tajik")) );
  m_langList.append( Language("th"  , "Thai"        , i18n("Thai")) );
//  m_langList.append( Language("tk"  , "Turkmen"     , i18n("Turkmen")) );
  m_langList.append( Language("tl"  , "Tagalog"     , i18n("Tagalog")) );
  m_langList.append( Language("tn"  , "Tswana"      , i18n("Tswana")) );
  m_langList.append( Language("tr"  , "Turkish"     , i18n("Turkish")) );
//  m_langList.append( Language("ts"  , "Tsonga"      , i18n("Tsonga")) );
//  m_langList.append( Language("tt"  , "Tatar"       , i18n("Tatar")) );
//  m_langList.append( Language("tw"  , "Twi"         , i18n("Twi")) );
  m_langList.append( Language("ty"  , "Tahitian"    , i18n("Tahitian")) );
//
//  m_langList.append( Language("ug"  , "Uighur"      , i18n("Uighur")) );
  m_langList.append( Language("uk"  , "Ukrainian"   , i18n("Ukrainian")) );
//  m_langList.append( Language("ur"  , "Urdu"        , i18n("Urdu")) );
//  m_langList.append( Language("uz"  , "Uzbek"       , i18n("Uzbek")) );

  m_langList.append( Language("vi"  , "Vietnamese"  , i18n("Vietnamese")) );
//  m_langList.append( Language("vo"  , "Volapük"     , i18n("Volapük")) );
//
//  m_langList.append( Language("wo"  , "Wolof"       , i18n("Wolof")) );

  m_langList.append( Language("xh"  , "Xhosa"       , i18n("Xhosa")) );

  m_langList.append( Language("x-E-BAR"  , "Bavarian"     , i18n("Bavarian")) );
  m_langList.append( Language("x-E-GSW"  , "Alemannisch"  , i18n("Alemannisch")) );
  m_langList.append( Language("x-E-HAT"  , "Haitian Creole French" , i18n("Haitian Creole French")) );
  m_langList.append( Language("x-E-ITZ"  , "Itzá"         , i18n("Itzá")) );  
  m_langList.append( Language("x-E-JIV"  , "Shuar"        , i18n("Shuar")) );
  m_langList.append( Language("x-E-KEK"  , "Kekchí"       , i18n("Kekchí")) );
  m_langList.append( Language("x-E-LMO"  , "Lombard"      , i18n("Lombard")) );  
  m_langList.append( Language("x-E-MKJ"  , "Macedonian"   , i18n("Macedonian")) );
  m_langList.append( Language("x-E-PDG"  , "Tok Pisin"    , i18n("Tok Pisin")) );
  m_langList.append( Language("x-E-PPK"  , "Uma"          , i18n("Uma")) );
  m_langList.append( Language("x-E-RMY"  , "Romani, Vlax" , i18n("Romani, Vlax")) );
  m_langList.append( Language("x-E-SAJ"  , "Sango"        , i18n("Sango")) );
  m_langList.append( Language("x-E-SRN"  , "Sranan"       , i18n("Sranan")) );  

//  m_langList.append( Language("yi"  , "Yiddish"     , i18n("Yiddish")) );
//
//  m_langList.append( Language("za"  , "Zhuang"      , i18n("Zhuang")) );
  m_langList.append( Language("zh"  , "Chinese"     , i18n("Chinese")) );
  m_langList.append( Language("zu"  , "Zulu"        , i18n("Zulu")) );

  QValueList<Language>::iterator it;
  for ( it = m_langList.begin(); it != m_langList.end(); ++it ) {
    m_langMap.insert((*it).abbrev(), *it);
  };
};

/** No descriptions */
void CLanguageMgr::debug(){
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

const QStringList CLanguageMgr::makeStringList( const QString& abbrevs ) {
  return QStringList::split( ";", abbrevs, false );
};
