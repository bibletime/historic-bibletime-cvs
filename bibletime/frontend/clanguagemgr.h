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

#include "util/cpointers.h"

//Qt includes
#include <qstring.h>
#include <qvaluelist.h>
#include <qpixmap.h>
#include <qmap.h>

/**Manages the anguages of BibleTime and provides functions to work with them.
  *@author The BibleTime team
  */

class CLanguageMgr : public CPointers  {
public:
  class Language {
  public:
    /** Constructor of a language object.
    * Uses the abbreviation parameter to lookup the language name and to be able to return the name, flag etc.
    * Possible values for abbrev are de, en, fr, it etc.
    */
    Language();
    Language(const QString& abbrev, const QString& englishName, const QString& translatedName);
    ~Language();
    const QString& abbrev();
    const QString& translatedName();
    const QString& name();
    const QPixmap flag();
    /**
    * Returns true if this language object is valid, i.e. has an abbrev and name.
    */
    const bool isValid();
  private:
    QString m_abbrev;
    QString m_englishName;
    QString m_translatedName;
  };

  typedef QMap<QString, Language> LangMap;

  CLanguageMgr();
	~CLanguageMgr();
  /**
  * Returns the standard languages available as standard. Does nothing for Sword.
  */
  const LangMap& languages();
  /**
  * Returns the languages which are available. The languages cover all available modules, but nothing more.
  */
  const LangMap availableLanguages();
  const Language languageForAbbrev( const QString& abbrev );
  const Language languageForName( const QString& language );  
  const Language languageForTranslatedName( const QString& language );
    
private:
  void init();
  
  QMap<QString, Language> m_langMap;  
};

#endif
