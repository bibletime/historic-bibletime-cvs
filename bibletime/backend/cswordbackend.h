/***************************************************************************
                          cswordbackend.h  -  description
                             -------------------
    begin                : Thu Oct 5 2000
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

#ifndef CSWORDBACKEND_H
#define CSWORDBACKEND_H

//BibleTime includes
//#include "cswordmoduleinfo.h"

//Qt includes
#include <qptrlist.h>
#include <qmap.h>
#include <qstring.h>
#include <qstringlist.h>

//Sword includes
#include <swmgr.h>
#include <swmodule.h>
#include <swversion.h>
#include <localemgr.h>

class CEntryDisplay;
class CChapterDisplay;
class CBookDisplay;

class CSwordModuleInfo;
typedef QPtrList<CSwordModuleInfo>	ListCSwordModuleInfo;

/**
	* This is the implementation of CBackend for Sword. It's additionally derived from SWMgr
	*	to provide functions of Sword.	
  *
  *	@short The backend implementation of Sword
  *	@author The BibleTime team
  * @version $Id$
  */
class CSwordBackend : public sword::SWMgr {
public:
  /**
 	* These are the options which could be supported by modules and by this backend.
 	* It's used in @ref isOptionEnabled and @ref setOption
 	*/
  enum FilterTypes {
  	footnotes,
  	strongNumbers,
  	headings,
  	morphTags,
		lemmas,
		hebrewPoints,
		hebrewCantillation,
		greekAccents,
    scriptureReferences,
    redLetterWords,
    textualVariants,
		filterTypesMIN = footnotes,
		filterTypesMAX = textualVariants,

    /* The following are handled in a special way */
    transliteration
	};

  struct FilterOptions {
  	int footnotes;
  	int strongNumbers;
  	int headings;
  	int morphTags;
		int lemmas;
		int hebrewPoints;
		int hebrewCantillation;
		int greekAccents;
		int textualVariants;
    int redLetterWords;
    int scriptureReferences;
    int transliteration;
	};
	struct DisplayOptions {
		int lineBreaks;
		int verseNumbers;
	};
  enum LoadError { // the values exist to cast from the char return of SWMgr::Load
		NoSwordConfig = -1,
		NoError = 0,
		NoModules = 1
  };
	/**
	* The constructor of the Sword backend.
	* It creates the SWModule objects using SWMgr's methods, it adds the necessary
	* filters for the module format.
	*/
	CSwordBackend();
	/**
	* The destrctor of this backend. This function shuts the modules down using @ref shutdownModules.
	*/
	virtual ~CSwordBackend();
  /**
  * This function returns the list of available modules managed by this backend.
  *	You have to call initModules() first;
  *	
  *	@return The list of modules managed by this backend
  */
  virtual ListCSwordModuleInfo& moduleList();
  /**
  * Initializes the Sword modules.
  *
  *	@return True if the initializiation was succesful, otherwise return false.
  */
  virtual const CSwordBackend::LoadError initModules();
  /**
  * This function deinitializes the modules and deletes them.
  *
  *	@return True if it was succesful, otherwise return false
  */
  virtual const bool shutdownModules();
  /**
  * Sets the given options enabled or disabled depending on the second parameter.
  *
  * @param type This is the type this function should set enabled or disabled
  * @param enable If this is true the option will be enabled, otherwise it will be disabled.
  */
  void setOption( const CSwordBackend::FilterTypes type, const int state );
  void setFilterOptions( const CSwordBackend::FilterOptions options );
  void setDisplayOptions( const CSwordBackend::DisplayOptions options );
  /**
  * Returns true if the given option is enabled.
  *
  * @return Returns true if the options given as aparameter is switched on at this time, otherwise return false.
  */
  virtual const bool isOptionEnabled( const CSwordBackend::FilterTypes type);
  /**
  * Sets the language for the international booknames of Sword.
  */
  const QString booknameLanguage( const QString& = QString::null );
  /**
  * This function searches for a module with the specified description
  * @param description The description of the desired module
  * @return pointer to the desired module; null if no module has the specified description
  */
  virtual CSwordModuleInfo* const findModuleByDescription(const QString& description);
  /**
  * This function searches for a module with the specified description
  * @param description The description of the desired module
  * @return pointer to the desired module; null if no module has the specified description
  */
  static const QString findModuleNameByDescription(const QString& description);
  /**
  * This function searches for a module with the specified name
  * @param description The name of the desired module
  * @return pointer to the desired module; null if no module has the specified name
  */
  CSwordModuleInfo* const findModuleByName(const QString& name);
  /**
  * This function searches for a module with the specified sword module as module() object!
  * @param pointer to a Sword module
  * @return pointer to the desired module; null if no module has the specified name
  */
  CSwordModuleInfo* const findModuleByPointer(const sword::SWModule* const swmodule);
  /**
  * @return Our global config object to store the cipher keys etc.
	*/
  sword::SWConfig* const getConfig() const;
  /**
  * Tries to find the config object for the module. The second paramter will be the found config.
  * @return True if the config was found, false if not. If false is returned the moduleConfig object is in undefined/unknwon state.
	*/
  const bool moduleConfig(const QString& module, sword::SWConfig& moduleConfig);
  /**
  * Returns the text used for the option given as parameter.
  */
  static const QString optionName( const CSwordBackend::FilterTypes option);
  /**
  * Returns the text used for the option given as parameter.
  */
  static const QString configOptionName( const CSwordBackend::FilterTypes option);
  /**
	* Returns the translated name of the option given as parameter.
	*/
  static const QString translatedOptionName(const CSwordBackend::FilterTypes option);
  /**
  * Returns the version of the Sword library.
  */
  virtual const sword::SWVersion Version();
  /**
  * Returns our transliterator object we use. Returns 0 if ICU is not used.
  */
  sword::SWFilter* const transliterator();
  /** Returns true if ICU is being used. */
  const bool useICU() const;
	
protected:
	/**
	* Adds a render filter to the module.
	* @param module Add a render filter to this module!
	*/
	virtual void AddRenderFilters(sword::SWModule *module, sword::ConfigEntMap &section);
	
private:
	//filters
	struct {
		sword::SWFilter *gbf;
		sword::SWFilter *plain;
		sword::SWFilter *thml;
 		sword::SWFilter *osis;
	} m_filters;
	ListCSwordModuleInfo m_moduleList;

	struct {
		CChapterDisplay* 	chapter;
		CEntryDisplay* 		entry;
		CBookDisplay* 		book;
	} m_displays;
};

/**Returns The list of modules managed by this backend*/
inline ListCSwordModuleInfo& CSwordBackend::moduleList() {
	return m_moduleList;
}

/** Returns our local config object to store the cipher keys etc. locally for each user. The values of the config are merged with the global config. */
inline sword::SWConfig* const CSwordBackend::getConfig() const {
	return config;
}

#endif
