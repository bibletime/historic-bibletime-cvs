/***************************************************************************
                          cexportmanager.h  -  description
                             -------------------
    begin                : Mon Feb 25 2002
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

#ifndef CEXPORTMANAGER_H
#define CEXPORTMANAGER_H

//BibleTime includes
#include "cpointers.h"

//Qt includes
#include <qstring.h>
#include <qlist.h>

class ListKey;
class CSwordKey;
class CSwordModuleInfo;


/**Contains the functions to export text to disk, clipboard or printer.
  *@author The BibleTime team
  */
class CExportManager : CPointers {
//protected:
//	CExportManager();
//	~CExportManager();

public: // Public methods
  /**
  * Saves the key to disk.
  */
  static const bool saveKey( CSwordKey* key, const QString& caption, const QString& description,const bool withText = true, const bool showProgress = false );

  /**
  * Saves the key to disk.
  */
  static const bool saveKeyList( ListKey* list, CSwordModuleInfo* module, const QString& caption, const QString& description, const bool withText = true, const bool showProgress = true );
  /**
  * Saves the key to disk.
  */
  static const bool saveKeyList( QList<CSwordKey>& list, CSwordModuleInfo* module, const QString& caption, const QString& description, const bool withText = true, const bool showProgress = true );


  //print
  static const bool printKey( CSwordModuleInfo* module, const QString& startKey, const QString& stopkey = QString::null, const QString& description = QString::null );
  /**
  * Saves the key to disk.
  */
  static const bool printKeyList( ListKey* list, CSwordModuleInfo* module, const QString& caption, const QString& description, const bool showProgress = true );

};

#endif
