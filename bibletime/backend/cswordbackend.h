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

//Qt includes
#include <qlist.h>
#include <qmap.h>
#include <qstring.h>
#include <qstringlist.h>

//Sword includes
#include <swmgr.h>
#include <swmodule.h>
#include <swversion.h>
#include <localemgr.h>

class CHTMLEntryDisplay;
class CHTMLChapterDisplay;

class CSwordModuleInfo;
typedef QList<CSwordModuleInfo>	ListCSwordModuleInfo;

/**
	* This is the implementation of CBackend for Sword. It's additionally derived from SWMgr
	*	to provide functions of Sword.	
  *
  *	@short The backend implementation of Sword
  *	@author The BibleTime team
  * @version $Id$
  */
class CSwordBackend : public SWMgr {
public:
  /**
 	* These are the options which could be supported by modules and by this backend.
 	* It's used in @ref isOptionEnabled and @ref setOption
 	*/
  enum moduleOptions {
  	footnotes,
  	strongNumbers,
  	headings,
  	morphTags,
		lemmas,
		hebrewPoints,
		hebrewCantillation,
		greekAccents,
		moduleOptionsMIN = footnotes,
		moduleOptionsMAX = greekAccents
	};

  struct moduleOptionsBool {
  	bool footnotes;
  	bool strongNumbers;
  	bool headings;
  	bool morphTags;
		bool lemmas;
		bool hebrewPoints;
		bool hebrewCantillation;
		bool greekAccents;
	};
	struct displayOptionsBool {
		bool lineBreaks;
		bool verseNumbers;
	};
  enum errorCode {
		noError,
		noSwordConfigFile,
		noSwordModuleDirectory,
		noSwordModuleConfigDirectory,
		noModulesAvailable,
		unknown
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
  virtual ListCSwordModuleInfo* moduleList() const;
  /**
  * Initializes the Sword modules.
  *
  *	@return True if the initializiation was succesful, otherwise return false.
  */
  virtual const errorCode initModules();
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
  virtual void setOption( const CSwordBackend::moduleOptions type, bool enable);
  void setAllModuleOptions( const CSwordBackend::moduleOptionsBool options);
  void setAllDisplayOptions( const CSwordBackend::displayOptionsBool options);
  /**
  * Returns true if the given option is enabled.
  *
  * @return Returns true if the options given as aparameter is switched on at this time, otherwise return false.
  */
  virtual const bool isOptionEnabled( const CSwordBackend::moduleOptions type);
	/**
	*
	*/
	virtual void Load();
  /**
  * Sets the language for the international booknames of Sword.
  */
  const QString booknameLanguage( const QString& = QString::null );
  /**
  * This function searches for a module with the specified description
  * @param description The description of the desired module
  * @return pointer to the desired module; null if no module has the specified description
  */
  virtual CSwordModuleInfo* findModuleByDescription(const QString& description);
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
  CSwordModuleInfo* findModuleByName(const QString& name);
  /**
  * @return Our global config object to store the cipher keys etc.
	*/
  SWConfig* getConfig() const;
  /**
  * Tries to find the config object for the module. The second paramter will be the found config.
  * @return True if the config was found, false if not. If false is returned the moduleConfig object is in undefined/unknwon state.
  *
	*/
  const bool moduleConfig(const QString& module, SWConfig& moduleConfig);
  /**
  * Returns the path of the module with the name "moduleName".	
	* If no path is found return QString::null
	*/
//  const QString modulePath( const QString moduleName );
  /**
  * Returns the text used for the option given as parameter.
  */
  static const QString optionName( const CSwordBackend::moduleOptions option);
  /**
  * Returns the text used for the option given as parameter.
  */
  static const QString configOptionName( const CSwordBackend::moduleOptions option);
//	/**
//	*	Returns the used options.
//	*/
//	static const CSwordBackend::moduleOptionsBool getAllModuleOptions();
  /** 
	* Returns the translated name of the option given as parameter.
	*/
  static const QString translatedOptionName(const CSwordBackend :: moduleOptions option);
  /** Returns the version of the Sword library. */
  virtual const SWVersion Version();
	

	CHTMLChapterDisplay* 	m_chapterDisplay;	
	CHTMLEntryDisplay* 		m_entryDisplay;

protected:
	/**
	* Adds a render filter to the module.
	* @param module Add a render filter to this module!
	*/
	virtual void AddRenderFilters(SWModule *module, ConfigEntMap &section);
	
private:
	//filters
	SWFilter *m_gbfFilter;
	SWFilter *m_plainTextFilter;
	SWFilter *m_thmlFilter;	
	//SWFilter *m_rwpFilter;
	
	ListCSwordModuleInfo* m_moduleList;
	errorCode m_errorCode;
};

/**Returns The list of modules managed by this backend*/
inline ListCSwordModuleInfo* CSwordBackend::moduleList() const {
	return m_moduleList;
}

/** Returns our local config object to store the cipher keys etc. locally for each user. The values of the config are merged with the global config. */
inline SWConfig* CSwordBackend::getConfig() const {
	return config;
}

#endif
