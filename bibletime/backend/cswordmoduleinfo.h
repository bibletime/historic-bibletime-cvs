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
#include "cswordbackend.h"

//Qt includes
#include <qstring.h>
#include <qfont.h>
#include <qmap.h>


//Sword includes
#include <listkey.h>

class SWModule;
class CSwordBackend;
class CHTMLEntryDisplay;

/**
	* Base class for Sword modules.
	* This is the base class for all Sword modules. Every class handling a special Sword module type
	* does inherit from this class.
	*
  * @author The BibleTime team
  * @version $Id$
  */
class CSwordModuleInfo  {
public:
	enum type { Bible, Commentary, Lexicon, GenericBook, Unknown };
//	enum TextDirection {
//		LTR = 0, /*LTR = Left to right*/		
//		RTL /*RTL = Right To Left*/
//	};
  /**
 	* This enum is used to give
 	* back an error code after unlocking the module
 	*/
  enum unlockErrorCode {
		noError,	/* No error occured, everything worked ok. The key was written to the config*/
		wrongUnlockKey, /* The wrong key was used. Module is not unlocked */				
		notLocked, /* The module was not locked so it can't be unlocked */
		noPermission /* The key was not written to config because we have no permissions*/
	};	
	CSwordModuleInfo( CSwordBackend* backend, SWModule* module );
	CSwordModuleInfo( const CSwordModuleInfo& m );	
	virtual ~CSwordModuleInfo();	
  /**
 	* Returns the backend used by thid module.
 	*/
  CSwordBackend* backend() const;
  /**
 	* Returns the module object so all objects can access the original Sword module.
 	*/
  SWModule* module() const;
  /**
 	* Sets the unlock key of the modules and writes the key into the cofig file.
	* @return True if the unlock process was succesful, if the key was wrong, or if the config file was write protected return false.
	*/
  const CSwordModuleInfo::unlockErrorCode unlock( const QString& unlockKey );
  /**
 	* Returns the display object for this module. Normally every module should have a Display object.
 	* Please don't use module()->Display() because this function does return the Sword display and does
 	* render the text, too.
 	* This function performs some casts to return the correct display. If it returns 0 there's no valid
 	* display object.
 	*/
  CHTMLEntryDisplay* getDisplay() const;
  /**
	* Returns the cipher key if the module is encrypted, if the key is not set return QString::empty,
 	* if the module is not encrypted retur QString::null.
 	*/
  const QString getCipherKey() const;
  /**
 	* This function does return true if the data files of the module are encrypted by the module author
 	* (the on who made the module) no matter if it's locked or not.
 	*
 	*/
  const bool isEncrypted() const;
  /**
 	* This function returns true if this module is locked (encrypted + correct cipher key),
 	* otherwise return false.
 	*/
  const bool isLocked();
  /**
 	* Returns the path to this module.
 	*/
  const QString getPath() const;
  /**
  * Returns the version number of this module. It does access the config file of this module to get
  * the version number.
  */
  const QString getVersion() const;
  /**
  * @return true if this module has a version number and false if it doesn't have one.
  */
  const bool hasVersion() const;
  /**
  * Returns the about information of this module.
  * This function uses the config file to get the about information.
  */
  const QString getAboutInformation() const;
  /**
 	* Returns the description of the module (e.g. the "The World english Bible")
 	*/
  const QString getDescription() const;
  /**
  * Returns true if something was found, otherwise return false.
  * This function does start the Sword functions to search in the module and it does
  * overwrite the variable containing the last search result.
  */
  virtual const bool search( const QString searchedText, const int searchOptions, ListKey scope, void (*percent)(char, void*));
  /**
  * Returns the last search result for this module.
  * The last result is cleared by @ref search
  */
  virtual ListKey& getSearchResult();
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
  * Returns true if the given type is supported by this module.
  *
  * @param type The type which should be checked
  */
  virtual const bool supportsFeature( const CSwordBackend::moduleOptions type );
  /**
  * Returns the type of the module.
  */
  virtual const CSwordModuleInfo::type getType() const;
//  /**
//  * Returns the text direction used in this module.
//  */
//  const CSwordModuleInfo::TextDirection getTextDirection();
  /**
  * Returns the required Sword version for this module.
	* Returns -1 if no special Sword version is required.
	*/
  const float requiredSwordVersion();
  /**
  * Returns the name of the module.
  */
  const QString name() const;
  /**
  * Returns true if this module is Unicode encoded. False if the charset is iso8859-1.
  */
  const bool isUnicode();
  /**
  * Snaps to the closest entry in the module if the current key is
  * not present in the data files.
  */
  virtual const bool snap() {return false;};
	
private:
	SWModule*	m_module;
	ListKey m_searchResult;
	CSwordBackend* m_backend;	
};

typedef QList<CSwordModuleInfo>	ListCSwordModuleInfo;

inline const CSwordModuleInfo::type CSwordModuleInfo::getType() const {
	return CSwordModuleInfo::Unknown;
}

inline SWModule* CSwordModuleInfo::module() const {
	return m_module;
}


#endif
