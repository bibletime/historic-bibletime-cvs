/***************************************************************************
                          cswordmoduleinfo.h  -  description
                             -------------------
    begin                : Wed Oct 11 2000
    copyright            : (C) 2000 by The BibleTime team
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

#ifndef CSWORDMODULEINFO_H
#define CSWORDMODULEINFO_H

//BibleTime includes
#include "centrydisplay.h"
// #include "cswordbackend.h"
#include "clanguagemgr.h"

#include "util/cpointers.h"

//Qt includes
#include <qstring.h>
#include <qfont.h>
#include <qmap.h>

//Sword includes
#include <listkey.h>
#include <swmodule.h>
#include <swversion.h>
#include <swdisp.h>


class CSwordBackend;
class CSwordKey;

// using namespace Rendering;

/**
	* Base class for Sword modules.
	* This is the base class for all Sword modules. Every class handling a special Sword module type
	* does inherit from this class.
	*
  * @author The BibleTime team
  * @version $Id$
  */
class CSwordModuleInfo : public CPointers  {
public:
  enum TextDirection { /* The text direction of the modules's text */
    LeftToRight,
    RightToLeft
  };
  enum ModuleType {
		Bible,
		Commentary,
		Lexicon,
		GenericBook,
		Unknown
	};
  /**
 	* This enum is used to give
 	* back an error code after unlocking the module
 	*/
  enum UnlockErrorCode {
		noError,	/* No error occured, everything worked ok. The key was written to the config*/
		wrongUnlockKey, /* The wrong key was used. Module is not unlocked */				
		notLocked, /* The module was not locked so it can't be unlocked */
		noPermission /* The key was not written to config because we have no permissions*/
	};	
	enum ConfigEntry {
		AboutInformation, /* The about information of a module which is stored in the config file*/
		AbsoluteDataPath, /* The absolute data path stored in the config object */
		CipherKey, /* The cipher key which was used to unlock the module. Not necessarily set.*/
		DataPath, /* The relative path. See AbsoluteDataPath*/
		Description, /* The module description stored in the config file */
		ModuleVersion, /* The module's version.*/
		MinimumSwordVersion, /* The required Sword Version of this module. Otherwise some things may not work (compression etc.).*/
	  TextDir, /* The text direction */	
    DisplayLevel, /* Mostly used for books. Gives the level which should contain the connected entries.*/
    GlossaryFrom, /* lamguage from which the Glosaary tramslates */
    GlossaryTo /* lamguages to which the glossary maps to */
	};
	enum Feature {
		StrongsNumbers, /*Use for Bibles which have embedded strong numbers*/
		GreekDef,
		HebrewDef, 
		GreekParse,
		HebrewParse,
    featureMin = StrongsNumbers,
    featureMax = HebrewParse
	};
  enum Category {
    UnknownCategory = 0, /* The category wasn't set or has an unknwon value */
    Cult, /* The module is a cult / sect / questionable module */
    DailyDevotional,
    Glossary
  };
	
	/**
	* Returns the config entry which is pecified by the parameter.
	*/
	const QString config( const CSwordModuleInfo::ConfigEntry entry );
	
	CSwordModuleInfo( sword::SWModule* module, CSwordBackend* const = CPointers::backend() );
	CSwordModuleInfo( const CSwordModuleInfo& m );	
  virtual CSwordModuleInfo* clone();	
	virtual ~CSwordModuleInfo();	

  /**
 	* Returns the module object so all objects can access the original Sword module.
 	*/
  inline sword::SWModule* const module() const;
  /**
 	* Sets the unlock key of the modules and writes the key into the cofig file.
	* @return True if the unlock process was succesful, if the key was wrong, or if the config file was write protected return false.
	*/
  const bool unlock( const QString& unlockKey );
  /**
 	* Returns the display object for this module. Normally every module should have a Display object.
 	* Please don't use module()->Display() because this function does return the Sword display and does
 	* render the text, too.
 	* This function performs some casts to return the correct display. If it returns 0 there's no valid
 	* display object.
 	*/
  inline Rendering::CEntryDisplay* const getDisplay() const;
  /**
 	* This function does return true if the data files of the module are encrypted by the module author
 	* (the on who made the module) no matter if it's locked or not.
 	*
 	*/
  const bool isEncrypted() /*const*/;
  /**
 	* This function returns true if this module is locked (encrypted + correct cipher key),
 	* otherwise return false.
 	*/
  const bool isLocked();

  /**
  * @return true if this module has a version number and false if it doesn't have one.
  */
  inline const bool hasVersion() const;
  /**
  * Returns true if something was found, otherwise return false.
  * This function does start the Sword functions to search in the module and it does
  * overwrite the variable containing the last search result.
  */
  virtual const bool search( const QString searchedText, const int searchOptions, sword::ListKey scope, void (*percent)(char, void*));
  /**
  * Returns the last search result for this module.
  * The last result is cleared by @ref search
  */
  virtual sword::ListKey& searchResult( const sword::ListKey* newResult = 0 );
  /**
  * This interupts the search if this module is being searched.
  */
  virtual void interruptSearch();
  /**
  * Clears the last search result.
  * This does immediately clean the last search result,
  * no matter if search is in progress or not.
  */
  void clearSearchResult();
  /**
  * Returns the type of the module.
  */
  virtual const CSwordModuleInfo::ModuleType type() const;
  /**
  * Returns the required Sword version for this module.
	* Returns -1 if no special Sword version is required.
	*/
  const sword::SWVersion minimumSwordVersion();
  /**
  * Returns the name of the module.
  */
  inline const QString name() const;
  /**
  * Snaps to the closest entry in the module if the current key is
  * not present in the data files.
  */
  virtual const bool snap() { return false; };
  const bool has( const CSwordModuleInfo::Feature );
	const bool has( const CSwordBackend::FilterTypes option )	;
  /**
  * Returns the text direction of the module's text.,
  */
  virtual const CSwordModuleInfo::TextDirection textDirection();
  /**
  * Writes the new text at the given position into the module. This does only work for writabe modules.
  */
  virtual void write( CSwordKey* key, const QString& newText );
  /**
  * Deletes the current entry and removes it from the module.
  */
  const bool deleteEntry( CSwordKey* const key );
  /**
  * Returns the language of the module.
  */
  inline const CLanguageMgr::Language* const language();
  /**
  * Returns true if this module may be written by the write display windows.
  */
  inline virtual const bool isWritable();
  /**
  * Returns the category of this module. See CSwordModuleInfo::Category for possible values.
  */
  const CSwordModuleInfo::Category category() const;

protected:
  friend class CSwordBackend;
	friend class CInfoDisplay;
  /**
  * Returns true if this module is Unicode encoded. False if the charset is iso8859-1.
	* Protected because it should not be used outside of the CSword*ModuleInfo classes.
  */
  inline const bool isUnicode();

  virtual inline CSwordBackend* backend() const {
    return m_backend;
  }
  virtual inline void backend( CSwordBackend* newBackend ) {
    if (newBackend) {
      m_backend = newBackend;
    }
  }

private:
	sword::SWModule* m_module;
	sword::ListKey m_searchResult;
	mutable struct {
		QString name;
		bool isUnicode;
		CSwordModuleInfo::Category category;
		const CLanguageMgr::Language* language;
		bool hasVersion;
	} m_dataCache;

  CSwordBackend* m_backend;
};

typedef QPtrList<CSwordModuleInfo>	ListCSwordModuleInfo;

inline const CSwordModuleInfo::ModuleType CSwordModuleInfo::type() const {
	return CSwordModuleInfo::Unknown;
}

inline sword::SWModule* const CSwordModuleInfo::module() const {
	return m_module;
}

/** Returns the display object for this module. */
inline Rendering::CEntryDisplay* const CSwordModuleInfo::getDisplay() const {
 	return dynamic_cast<Rendering::CEntryDisplay*>(m_module->Disp());
}

inline const bool CSwordModuleInfo::hasVersion() const {
	return m_dataCache.hasVersion;
}


/** Returns the name of the module. */
inline const QString CSwordModuleInfo::name() const {
	return m_dataCache.name;
}

/** Returns true if this module is Unicode encoded. False if the charset is iso8859-1. */
inline const bool CSwordModuleInfo::isUnicode(){
	return m_dataCache.isUnicode;
}

/** Returns true if this module may be written by the write display windows. */
inline const bool CSwordModuleInfo::isWritable() {
  return false;
}

/** Returns the language of the module. */
inline const CLanguageMgr::Language* const CSwordModuleInfo::language() {
	if (!m_dataCache.language) {
	  if (module()) {
			if (category() == Glossary) {
				//special handling for glossaries, we use the "from language" as language for the module
				m_dataCache.language = languageMgr()->languageForAbbrev( config(GlossaryFrom) );
			}
			else {
				m_dataCache.language = languageMgr()->languageForAbbrev( module()->Lang() );
			}
		}
		else {
			m_dataCache.language = languageMgr()->defaultLanguage(); //default language
		}
	}
	
	return m_dataCache.language;	
}


#endif
