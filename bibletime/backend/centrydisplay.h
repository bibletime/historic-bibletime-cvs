/***************************************************************************
                          centrydisplay.h  -  description
                             -------------------
    begin                : Fre Mai 31 2002
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

#ifndef CENTRYDISPLAY_H
#define CENTRYDISPLAY_H

//BibleTime includes
#include "cswordbackend.h"
#include "cswordmoduleinfo.h"
#include "cswordtreekey.h"

#include "util/cpointers.h"
#include "frontend/cbtconfig.h"

//Sword includes
#include <swdisp.h>

//Qt includes
#include <qstring.h>
#include <qfont.h>
#include <qcolor.h>
#include <qptrlist.h>

/**The reimplementation of SWDisplay to fir our needs.
  *@author The BibleTime team
  */

class CEntryDisplay : public sword::SWDisplay, public CPointers  {
public: // Public methods
  CEntryDisplay();
	virtual ~CEntryDisplay();
  /**
  * Returns the rendered text using the modules in the list and using the key parameter.
  *  The displayoptions and filter options are used, too.
  */
  virtual const QString text( QPtrList <CSwordModuleInfo> modules, const QString& key, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions);
  /**
  * Returns a preview for the given module and key.
  * This is useful for the seatchdialog and perhaps the tooltips.
  */
  void setDisplayOptions(const CSwordBackend::DisplayOptions options);

protected:
  /**
  * Renders one entry using the given modules and the key.
  * This makes chapter rendering more easy.
  */
  virtual const QString entryText( QPtrList<CSwordModuleInfo> modules, const QString& key);
  /**
  * Returns the right reference text which can be incluced in the HTML
  */
  const QString htmlReference( CSwordModuleInfo* module, const QString& keyName, const QString linkText, const QString& anchorText );

  CSwordBackend::DisplayOptions m_displayOptions;
};

class CChapterDisplay : public CEntryDisplay  {
public: // Public methods
  /**
  * Returns the rendered text using the modules in the list and using the key parameter.
  *  The displayoptions and filter options are used, too.
  */
  virtual const QString text( QPtrList <CSwordModuleInfo> modules, const QString& key, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions);
};

class CBookDisplay : public CEntryDisplay  {
public: // Public methods
  /**
  * Returns the rendered text using the modules in the list and using the key parameter.
  *  The displayoptions and filter options are used, too.
  */
  virtual const QString text( QPtrList <CSwordModuleInfo> modules, const QString& key, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions);

protected:
  /**
  * Renders one entry using the given modules and the key.
  * This makes chapter rendering more easy.
  *
  * Please be careful not to change the value of "key"!
  */
  virtual const QString entryText( QPtrList<CSwordModuleInfo> modules, CSwordTreeKey* const key, const int level = 0, const bool activekey = false);
  /**
  * Creates the text by processing all tree items.
  */
  void printTree(CSwordTreeKey* const treeKey, QPtrList<CSwordModuleInfo> modules, const int levelPos = 0);

private:
  QString m_text;
  QString m_chosenKey;
};
#endif
