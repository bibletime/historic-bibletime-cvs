/***************************************************************************
                          creferencemanager.h  -  description
                             -------------------
    begin                : Sun Aug 19 2001
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

#ifndef CREFERENCEMANAGER_H
#define CREFERENCEMANAGER_H

//Qt includes
#include <qstring.h>

#include "cswordmoduleinfo.h"

/**Contains static functions to compute with referneces used for Drag&Drop and for hyperlinks used in HTML.
  *@author The BibleTime team
  */
class CReferenceManager {
public:
	enum Type {
		Bible,
		Commentary,
		Lexicon,
		GenericBook,
		MorphHebrew,
		MorphGreek,
		StrongsHebrew,
		StrongsGreek,
		Unknown
	};
  /**
	* Decodes the given hyperlink to module and key.
	*/
  static const bool decodeHyperlink( const QString& hyperlink, QString& module, QString& key,Type& type);
  /**
	* Returns a hyperlink used to be imbedded in the display windows.
	* At the moment the format is sword://module/key
 	*/
  static const QString encodeHyperlink( const QString& module, const QString& key, const Type& type);
  /**
	* Puts a module Name and a Reference together in the 'draggable' form
	* (module)reference
	* @param module The name of the module
	* @param reference The key reference as text
	* @return The encoded reference using module and reference
	* @author Martin Gruner
	*/
	static const QString encodeReference(const QString &module, const QString &reference);
	/**
	* decodes a 'draggable' reference into a modulename and a reference
	* @author Martin Gruner
	*/
	static void decodeReference(QString &dragreference, QString &module, QString &reference);
  /**
  * Returns true if the parameter is a hyperlink.
  */
  static const bool isHyperlink( const QString& hyperlink );
  /**
  * Returns the preferred module name for the given type.
  */
  static const QString preferredModule( const Type type );
  static CReferenceManager::Type typeFromModule( const CSwordModuleInfo::ModuleType type );
  /** Parses the given verse references using the given language and the module.
	* @param ref The verse refernce
	* @param lang The language of the verse reference
	* @param newLang The language of the reference, which will be returned. For example: If BibleTime using an english environment parses a spanish ref (lang=es) the returned ref should be in english (newLang=en), because his english standard module only understands en.
	*/
  static const QString parseVerseReference( const QString ref, const QString& lang, const QString& newLang = "en");
};

#endif

