/***************************************************************************
                          clanguagemgr.h  -  description
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

#ifndef CLANGUAGEMGR_H
#define CLANGUAGEMGR_H

//Qt includes
#include <qstring.h>
#include <qstringlist.h>
#include <qvaluelist.h>
#include <qmap.h>
#include <qdict.h>

/** Manages the anguages of BibleTime and provides functions to work with them.
  * @author The BibleTime team
  */
class CLanguageMgr  {
public:
  class Language {
  public:
    /** Constructor of a language object.
    * Uses the abbreviation parameter to lookup the language name and to be able to return the name, flag etc.
    * Possible values for abbrev are de, en, fr, it etc.
    */
    Language();
		Language(const Language&); //copy constructor
    Language(const QString& abbrev, const QString& englishName, const QString& translatedName, const QStringList& altAbbrevs = QStringList());
    ~Language();
		
		inline const QString& abbrev() const;
    inline const QString& translatedName() const;

		//always define inlines in the header file, or make them not inline.
    inline const QString& name() const {
			return m_englishName;
		}

    inline const QStringList* const alternativeAbbrevs() const {
			return m_altAbbrevs;
		};
   	
		/**
    * Returns true if this language object is valid, i.e. has an abbrev and name.
    */
    inline const bool isValid() const;
		
  private:
    QString m_abbrev;
    QString m_englishName;
    QString m_translatedName;
    QStringList* m_altAbbrevs;
  };

  typedef QDict<Language> LangMap;
  typedef QDictIterator<Language> LangMapIterator;

  CLanguageMgr();
	~CLanguageMgr();
  /**
  * Returns the standard languages available as standard. Does nothing for Sword.
  */
  inline const CLanguageMgr::LangMap* const languages() const;
  /**
  * Returns the languages which are available. The languages cover all available modules, but nothing more.
  */
  const CLanguageMgr::LangMap& availableLanguages();
  const CLanguageMgr::Language* const languageForAbbrev( const QString& abbrev ) const;
  const CLanguageMgr::Language* const languageForName( const QString& language ) const;
  const CLanguageMgr::Language* const languageForTranslatedName( const QString& language ) const;
	
	inline const CLanguageMgr::Language* const defaultLanguage() const;

private:
  void init();
  inline const QStringList makeStringList(const QString& abbrevs) {
	  return QStringList::split( ";", abbrevs, false );
	}
	
  mutable LangMap m_langMap;
	Language m_defaultLanguage;
	
	struct {
		unsigned int moduleCount;
		LangMap availableLanguages;
	} m_availableModulesCache;
};


/** Returns true if this language object is valid, i.e. has an abbrev and name. */
inline const bool CLanguageMgr::Language::isValid() const {
  return (!abbrev().isEmpty() && !name().isEmpty());
}

inline const QString& CLanguageMgr::Language::abbrev() const {
  if (m_altAbbrevs && m_abbrev.isEmpty() && m_altAbbrevs->count()) { //no standard abbrev but alternative ones
    return m_altAbbrevs->first();
  };
  return m_abbrev;
};

inline const QString& CLanguageMgr::Language::translatedName() const {
  return m_translatedName;
};

inline const CLanguageMgr::LangMap* const CLanguageMgr::languages() const {
  return &m_langMap;
};

inline const CLanguageMgr::Language* const CLanguageMgr::defaultLanguage() const {
	return &m_defaultLanguage;
};


#endif
