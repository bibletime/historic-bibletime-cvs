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
#include "cbtconfig.h"

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
public:
  enum Format {
    HTML,
    Text
  };
  
  CExportManager(const QString& caption, const bool showProgress = true, const QString& progressLabel = QString::null, const CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults(), const CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults());
  const bool saveKey(CSwordKey* key, const Format format, const bool addText);
  const bool saveKeyList(ListKey* list, CSwordModuleInfo* module, const Format format, const bool addText);  
      
//  //save functions
//  static const bool saveKey( CSwordKey* key, const Format format, const bool withText = true, CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults(), const CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults());
//  static const bool saveKeyList( ListKey* list, CSwordModuleInfo* module, const QString& label, const bool withText = true, const bool showProgress = true );
//  static const bool saveKeyList( QPtrList<CSwordKey>& list, CSwordModuleInfo* module, const QString& label, const bool withText = true, const bool showProgress = true );
//
//  //clipboard functions
//  static const bool copyKey( CSwordKey* key, const bool withText = true, const CSwordBackend::FilterOptions = CBTConfig::getFilterOptionDefaults(), const CSwordBackend::DisplayOptions = CBTConfig::getDisplayOptionDefaults() );
//  static const bool copyKeyList( ListKey* list, CSwordModuleInfo* module, const QString& label, const bool withText = true, const bool showProgress = true );
//
//  //print function
//  static const bool printKey( CSwordModuleInfo* module, const QString& startKey, const QString& stopkey = QString::null, const QString& description = QString::null );
//  static const bool printKeyList( ListKey* list, CSwordModuleInfo* module, const QString& label, const bool showProgress = true );
//  static void printKey( const QString& hyperlink );

private:
  QString m_caption;
  QString m_progressLabel;
  bool m_showProgress;
  CSwordBackend::FilterOptions m_filterOptions;
  CSwordBackend::DisplayOptions m_displayOptions;

protected: // Protected methods
  /**
  * Returns the string for the filedialogs to show the correct files.
  */
  const QString filterString( const Format format );
  /**
  * Returns a filename to save a file.
  */
  const QString getSaveFileName(const Format format);
  /**
  * Returns a string containing the linebreak for the current format.
  */
  const QString lineBreak( const Format format );
};

#endif
