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
#include <qptrlist.h>

class ListKey;
class CSwordKey;
class CSwordModuleInfo;


/**Contains the functions to export text to disk, clipboard or printer.
  *@author The BibleTime team
  */
class CExportManager : CPointers {
public: // Public methods
  //save functions
  static const bool saveKey( CSwordKey* key, const bool withText = true );
  static const bool saveKeyList( ListKey* list, CSwordModuleInfo* module, const QString& label, const bool withText = true, const bool showProgress = true );
  static const bool saveKeyList( QPtrList<CSwordKey>& list, CSwordModuleInfo* module, const QString& label, const bool withText = true, const bool showProgress = true );

  //clipboard functions
  static const bool copyKey( CSwordKey* key, const bool withText = true );
  static const bool copyKeyList( ListKey* list, CSwordModuleInfo* module, const QString& label, const bool withText = true, const bool showProgress = true );

  //print function
  static const bool printKey( CSwordModuleInfo* module, const QString& startKey, const QString& stopkey = QString::null, const QString& description = QString::null );
  static const bool printKeyList( ListKey* list, CSwordModuleInfo* module, const QString& label, const bool showProgress = true );
  static void printKey( const QString& hyperlink );

};

#endif
