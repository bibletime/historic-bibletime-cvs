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
    qWarning("return alternative abbrev %s for language %s", m_altAbbrevs.first().latin1(), name().latin1());
    return m_altAbbrevs.first();
  };
  return m_abbrev;
};

const QString& CLanguageMgr::Language::translatedName() const {
  return m_translatedName;
};

const QString& CLanguageMgr::Language::name() const {
  return m_englishName;
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

const CLanguageMgr::LangMap& CLanguageMgr::languages() {
  return m_langMap;
};

const CLanguageMgr::LangMap CLanguageMgr::availableLanguages() {
  LangMap map;
  
  //collect the languages abbrevs of all modules
  ListCSwordModuleInfo mods = CPointers::backend()->moduleList();
  QStrList abbrevs;

  for (CSwordModuleInfo* m = mods.first(); m; m = mods.next()) {
    if (!abbrevs.contains(m->module()->Lang()))
      abbrevs.append( m->module()->Lang() );
  };

  //no create a ma of available langs
  Language lang;
  char *abbrev;
  for ( abbrev = abbrevs.first(); abbrev; abbrev = abbrevs.next() ) {
    lang = this->languageForAbbrev(abbrev);
    if (lang.isValid()) {
      map.insert( abbrev, lang );
    };
  };

  //debug the found languages
  LangMap::Iterator it;
  for ( it = map.begin(); it != map.end(); ++it ) {
    qWarning("module with language is installed: %s: %s (%s)", it.data().abbrev().latin1(), it.data().name().latin1(),it.data().translatedName().latin1());
  };

  
  return map;
};

const CLanguageMgr::Language CLanguageMgr::languageForAbbrev( const QString& abbrev ) {
  qWarning("looking for abbrev %s", abbrev.latin1());
  if (m_langMap.contains(abbrev)) {
    qWarning("found!!");
    return m_langMap[abbrev];
  };

  //try to search in the alternative abbrevs
  qWarning("searching for %s in alternative abbrevs", abbrev.latin1());
  LangMap::Iterator it;
  for ( it = m_langMap.begin(); it != m_langMap.end(); ++it ) {
    if (it.data().alternativeAbbrevs().contains(abbrev)) {
      qWarning("found language!");      
      return it.data();
    };
  }  
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
//  list.append( Language("aa"  , "Afar"        , i18n("Afar")) );
//  list.append( Language("ab"  , "Abkhazian"   , i18n("Abkhazian")) );
//  list.append( Language("ae"  , "Avestan"     , i18n("Avestan")) );
  list.append( Language("af"  , "Afrikaans"   , i18n("Afrikaans")) );
//  list.append( Language("am"  , "Amharic"     , i18n("Amharic")) );
  list.append( Language("ar"  , "Arabic"      , i18n("Arabic")) );
//  list.append( Language("as"  , "Assamese"    , i18n("Assamese")) );
//  list.append( Language("ay"  , "Aymara"      , i18n("Aymara")) );
//  list.append( Language("az"  , "Azerbaijani" , i18n("Azerbaijani")) );

//  list.append( Language("ba"  , "Bashkir"     , i18n("Bashkir")) );
  list.append( Language("be"  , "Belarusian"  , i18n("Belarusian")) );
  list.append( Language("bg"  , "Bulgarian"   , i18n("Bulgarian")) );
//  list.append( Language("bh"  , "Bihari"      , i18n("Bihari")) );
//  list.append( Language("bi"  , "Bislama"     , i18n("Bislama")) );
//  list.append( Language("bn"  , "Bengali"     , i18n("Bengali")) );
//  list.append( Language("bo"  , "Tibetan"     , i18n("Tibetan")) );
//  list.append( Language("br"  , "Breton"      , i18n("Breton")) );
//  list.append( Language("bs"  , "Bosnian"     , i18n("Bosnian")) );

//  list.append( Language("ca"  , "Catalan"     , i18n("Catalan")) );
//  list.append( Language("ce"  , "Chechen"     , i18n("Chechen")) );
  list.append( Language("ceb"  , "Cebuano"     , i18n("Cebuano")) );
//  list.append( Language("ch"  , "Chamorro"    , i18n("Chamorro")) );
//  list.append( Language("co"  , "Corsican"    , i18n("Corsican")) );
  list.append( Language("cs"  , "Czech"       , i18n("Czech")) );
//  list.append( Language("cu"  , "Church Slavic" , i18n("Church Slavic")) );
//  list.append( Language("cv"  , "Chuvash"     , i18n("Chuvash")) );
//  list.append( Language("cy"  , "Welsh"       , i18n("Welsh")) );

  list.append( Language("da"  , "Danish"      , i18n("Danish")) );
  list.append( Language("de"  , "German"      , i18n("German")) );
//  list.append( Language("dz"  , "Dzongkha"    , i18n("Dzongkha")) );

  list.append( Language("el"  , "Greek, Modern (1453-)" , i18n("Greek, Modern (1453-)"), makeStringList("gre;ell")) );
  list.append( Language("en"  , "English"     , i18n("English")) );
  list.append( Language("en_US"  , "American English"     , i18n("American English")) );  
  list.append( Language("enm" , "English, Middle (1100-1500)"     , i18n("English, Middle (1100-1500)")) );  
  list.append( Language("eo"  , "Esperanto"   , i18n("Esperanto")) );
  list.append( Language("es"  , "Spanish"     , i18n("Spanish")) );
  list.append( Language("et"  , "Estonian"    , i18n("Estonian")) );
//  list.append( Language("eu"  , "Basque"      , i18n("Basque")) );

//  list.append( Language("fa"  , "Persian"     , i18n("Persian")) );
  list.append( Language("fi"  , "Finnish"     , i18n("Finnish")) );
//  list.append( Language("fj"  , "Fijian"      , i18n("Fijian")) );
//  list.append( Language("fo"  , "Faroese"     , i18n("Faroese")) );
  list.append( Language("fr"  , "French"      , i18n("French")) );
//  list.append( Language("fy"  , "Frisian"     , i18n("Frisian")) );

//  list.append( Language("ga"  , "Irish"       , i18n("Irish")) );
//  list.append( Language("gd"  , "Gaelic (Scots)"     , i18n("Gaelic (Scots)")) );
//  list.append( Language("gl"  , "Gallegan"    , i18n("Gallegan")) );
//  list.append( Language("gn"  , "Guarani"     , i18n("Guarani")) );
//  list.append( Language("gn"  , "Gujarati"    , i18n("Gujarati")) );
  list.append( Language("got"  , "Gothic"    , i18n("Gothic")) );
  list.append( Language("gv"  , "Manx"        , i18n("Manx")) );
  list.append( Language("grc"  , "Greek, Ancient (to 1453)" , i18n("Greek, Ancient (to 1453)")) );

  list.append( Language("he"  , "Hebrew"      , i18n("Hebrew")) );
//  list.append( Language("hi"  , "Hindi"       , i18n("Hindi")) );
//  list.append( Language("ho"  , "Hiri Motu"   , i18n("Hiri Motu")) );
//  list.append( Language("hr"  , "Croatian"    , i18n("Croatian")) );
  list.append( Language("hu"  , "Hungarian"   , i18n("Hungarian")) );
//  list.append( Language("hy"  , "Armenian"    , i18n("Armenian")) );
//  list.append( Language("hz"  , "Herero"      , i18n("Herero")) );

//  list.append( Language("ia"  , "Interlingua" , i18n("Interlingua")) );
  list.append( Language("id"  , "Indonesian"  , i18n("Indonesian")) );
//  list.append( Language("ie"  , "Interlingue" , i18n("Interlingue")) );
//  list.append( Language("ik"  , "Inupiaq"     , i18n("Inupiaq")) );
  list.append( Language("is"  , "Icelandic"   , i18n("Icelandic")) );
  list.append( Language("it"  , "Italian"     , i18n("Italian")) );
//  list.append( Language("iu"  , "Inuktitut"   , i18n("Inuktitut")) );

//  list.append( Language("ja"  , "Japanese"    , i18n("Japanese")) );

//  list.append( Language("ka"  , "Georgian"    , i18n("Georgian")) );
//  list.append( Language("ki"  , "Kikuyu"      , i18n("Kikuyu")) );
//  list.append( Language("kj"  , "Kuanyama"    , i18n("Kuanyama")) );
//  list.append( Language("kk"  , "Kazakh"      , i18n("Kazakh")) );
//  list.append( Language("kl"  , "Kalaallisut" , i18n("Kalaallisut")) );
//  list.append( Language("km"  , "Khmer"       , i18n("Khmer")) );
//  list.append( Language("kn"  , "Kannada"     , i18n("Kannada")) );
  list.append( Language("ko"  , "Korean"      , i18n("Korean")) );
//  list.append( Language("ks"  , "Kashmiri"    , i18n("Kashmiri")) );
//  list.append( Language("ku"  , "Kurdish"     , i18n("Kurdish")) );
//  list.append( Language("kv"  , "Komi"        , i18n("Komi")) );
//  list.append( Language("kw"  , "Cornish"     , i18n("Cornish")) );
//  list.append( Language("ky"  , "Kirghiz"     , i18n("Kirghiz")) );

  list.append( Language("la"  , "Latin"       , i18n("Latin")) );
//  list.append( Language("lb"  , "Letzeburgesch" , i18n("Letzeburgesch")) );
//  list.append( Language("ln"  , "Lingala"     , i18n("Lingala")) );
//  list.append( Language("lo"  , "Lao"         , i18n("Lao")) );
//  list.append( Language("lt"  , "Lithuanian"  , i18n("Lithuanian")) );
  list.append( Language("lv"  , "Latvian"     , i18n("Latvian")) );

//  list.append( Language("mg"  , "Malagasy"    , i18n("Malagasy")) );
//  list.append( Language("mh"  , "Marshall"    , i18n("Marshall")) );
  list.append( Language("mi"  , "Maori"       , i18n("Maori")) );
  
//  list.append( Language("mk"  , "Macedonian"  , i18n("Macedonian")) );
//  list.append( Language("ml"  , "Malayalam"   , i18n("Malayalam")) );
//  list.append( Language("mn"  , "Mongolian"   , i18n("Mongolian")) );
//  list.append( Language("mo"  , "Moldavian"   , i18n("Moldavian")) );
//  list.append( Language("mr"  , "Marathi"     , i18n("Marathi")) );
//  list.append( Language("ms"  , "Malay"       , i18n("Malay")) );
//  list.append( Language("mt"  , "Maltese"     , i18n("Maltese")) );
//  list.append( Language("my"  , "Burmese"     , i18n("Burmese")) );

//  list.append( Language("na"  , "Nauru"       , i18n("Nauru")) );
//  list.append( Language("nb"  , "Norwegian Bokmål"       , i18n("Norwegian Bokmål")) );
//  list.append( Language("nd"  , "Ndebele, North" , i18n("Ndebele, North")) );
  list.append( Language("nds"  , "Low German; Low Saxon" , i18n("Low German; Low Saxon")) );
//  list.append( Language("ne"  , "Nepali"      , i18n("Nepali")) );
//  list.append( Language("ng"  , "Ndonga"      , i18n("Ndonga")) );
  list.append( Language("nl"  , "Dutch"       , i18n("Dutch")) );
//  list.append( Language("nn"  , "Norwegian Nynorsk"     , i18n("Norwegian Nynorsk")) );
  list.append( Language("no"  , "Norwegian"   , i18n("Norwegian")) );
//  list.append( Language("nr"  , "Ndebele, South"     , i18n("Ndebele, South")) );
//  list.append( Language("nv"  , "Navajo"      , i18n("Navajo")) );
//  list.append( Language("ny"  , "Chichewa; Nyanja"      , i18n("Chichewa; Nyanja")) );

//  list.append( Language("oc"  , "Occitan (post 1500); Provençal"     , i18n("Occitan (post 1500); Provençal")) );
//  list.append( Language("om"  , "Oromo"       , i18n("Oromo")) );
//  list.append( Language("or"  , "Oriya"       , i18n("Oriya")) );
//  list.append( Language("os"  , "Ossetian; Ossetic"     , i18n("Ossetian; Ossetic")) );
//
//  list.append( Language("pa"  , "Panjabi"     , i18n("Panjabi")) );
//  list.append( Language("pi"  , "Pali"        , i18n("Pali")) );
//  list.append( Language("pl"  , "Polish"      , i18n("Polish")) );
//  list.append( Language("ps"  , "Pushto"      , i18n("Pushto")) );
  list.append( Language("pt"  , "Portuguese"  , i18n("Portuguese")) );

//  list.append( Language("qu"  , "Quechua"     , i18n("Quechua")) );

//  list.append( Language("rm"  , "Raeto-Romance"    , i18n("Raeto-Romance")) );
//  list.append( Language("rn"  , "Rundi"       , i18n("Rundi")) );

  list.append( Language("ro"  , "Romanian"    , i18n("Romanian")) );
  list.append( Language("ru"  , "Russian"     , i18n("Russian")) );
//  list.append( Language("rw"  , "Kinyarwanda" , i18n("Kinyarwanda")) );

//  list.append( Language("sa"  , "Sanskrit"    , i18n("Sanskrit")) );
//  list.append( Language("sc"  , "Sardinian"   , i18n("Sardinian")) );
  list.append( Language("sco"  , "Scots"   , i18n("Scots")) );
//  list.append( Language("sd"  , "Sindhi"      , i18n("Sindhi")) );
//  list.append( Language("se"  , "Northern Sami" , i18n("Northern Sami")) );
//  list.append( Language("sg"  , "Sango"       , i18n("Sango")) );
//  list.append( Language("si"  , "Sinhalese"   , i18n("Sinhalese")) );
//  list.append( Language("sk"  , "Slovak"      , i18n("Slovak")) );
//  list.append( Language("sl"  , "Slovenian"   , i18n("Slovenian")) );
//  list.append( Language("sm"  , "Samoan"      , i18n("Samoan")) );
//  list.append( Language("sn"  , "Shona"       , i18n("Shona")) );
//  list.append( Language("so"  , "Somali"      , i18n("Somali")) );
  list.append( Language("sq"  , "Albanian"    , i18n("Albanian")) );
//  list.append( Language("sr"  , "Serbian"     , i18n("Serbian")) );
//  list.append( Language("ss"  , "Swati"       , i18n("Swati")) );
//  list.append( Language("st"  , "Sotho, Southern"     , i18n("Sotho, Southern")) );
//  list.append( Language("su"  , "Sundanese"   , i18n("Sundanese")) );
  list.append( Language("sv"  , "Swedish"     , i18n("Swedish")) );
  list.append( Language("sw"  , "Swahili"     , i18n("Swahili")) );
  list.append( Language("syr"  , "Syriac"     , i18n("Syriac")) );  

  list.append( Language("ta"  , "Tamil"       , i18n("Tamil")) );
//  list.append( Language("te"  , "Telugu"      , i18n("Telugu")) );
//  list.append( Language("tg"  , "Tajik"       , i18n("Tajik")) );
  list.append( Language("th"  , "Thai"        , i18n("Thai")) );
//  list.append( Language("tk"  , "Turkmen"     , i18n("Turkmen")) );
  list.append( Language("tl"  , "Tagalog"     , i18n("Tagalog")) );
//  list.append( Language("tn"  , "Tswana"      , i18n("Tswana")) );
  list.append( Language("tr"  , "Turkish"     , i18n("Turkish")) );
//  list.append( Language("ts"  , "Tsonga"      , i18n("Tsonga")) );
//  list.append( Language("tt"  , "Tatar"       , i18n("Tatar")) );
//  list.append( Language("tw"  , "Twi"         , i18n("Twi")) );
//  list.append( Language("ty"  , "Tahitian"    , i18n("Tahitian")) );
//
//  list.append( Language("ug"  , "Uighur"      , i18n("Uighur")) );
  list.append( Language("uk"  , "Ukrainian"   , i18n("Ukrainian")) );
//  list.append( Language("ur"  , "Urdu"        , i18n("Urdu")) );
//  list.append( Language("uz"  , "Uzbek"       , i18n("Uzbek")) );

  list.append( Language("vi"  , "Vietnamese"  , i18n("Vietnamese")) );
//  list.append( Language("vo"  , "Volapük"     , i18n("Volapük")) );
//
//  list.append( Language("wo"  , "Wolof"       , i18n("Wolof")) );

  list.append( Language("xh"  , "Xhosa"       , i18n("Xhosa")) );
  
  list.append( Language("x-E-GSW"  , "Alemannisch"  , i18n("Alemannisch")) );
  list.append( Language("x-E-HAT"  , "Haitian Creole French" , i18n("Haitian Creole French")) );
  list.append( Language("x-E-jiv"  , "Shuar"        , i18n("Shuar")) );
  list.append( Language("x-E-KEK"  , "Kekchí"       , i18n("Kekchí")) );
  list.append( Language("x-E-mkj"  , "Macedonian"   , i18n("Macedonian")) );    
  list.append( Language("x-E-PDG"  , "Tok Pisin"    , i18n("Tok Pisin")) );
  list.append( Language("x-E-PPK"  , "Uma"          , i18n("Uma")) );
  list.append( Language("x-E-RMY"  , "Romani, Vlax"    , i18n("Romani, Vlax")) );  
  list.append( Language("x-E-SAJ"  , "Sango"        , i18n("Sango")) );

//  list.append( Language("yi"  , "Yiddish"     , i18n("Yiddish")) );
//
//  list.append( Language("za"  , "Zhuang"      , i18n("Zhuang")) );
  list.append( Language("zh"  , "Chinese"     , i18n("Chinese")) );
//  list.append( Language("zu"  , "Zulu"        , i18n("Zulu")) );
//        
  QValueList<Language>::iterator it;
  for ( it = list.begin(); it != list.end(); ++it ) {
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