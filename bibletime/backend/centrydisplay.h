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


class CTextRendering {
public:
  const QString renderKeyList( ListCSwordModuleInfo const* modules, QPtrList<CSwordKey*> keyList );
  const QString renderKeyRange( ListCSwordModuleInfo const* modules, const QString& start, const QString& stop );
  const QString renderSingleKey( ListCSwordModuleInfo const* modules, CSwordKey* );

protected:
  virtual const QString renderEntry( ListCSwordModuleInfo const * modules, CSwordKey* key) = 0;
  virtual void finishText( const QString& ) = 0;
};


/**The reimplementation of SWDisplay to fir our needs.
  *@author The BibleTime team
  */
class CEntryDisplay : public sword::SWDisplay, public CPointers  {
public: // Public methods
  enum StyleType {
    Body = 0,
    Font,
    Link,
    LinkHover,
    Background,
    HighlightedText,

    Reference,
		Verse,
		CurrentVerse,
		VerseNum,

    MorphCode,
    StrongNumber,
    Lemma,
    Footnote,
    FootnotePre,

    Poetry,
    Quotation,
    JesusWords,

    BookTitle,
    SectionTitle,

    MinType = Body,
    MaxType = SectionTitle
  };

  enum ColorType {
    ReferenceColor,
    TextColor,
    HighlightedTextColor,
    BackgroundColor,
    BackgroundColor2,

    FootnoteColor,
    StrongsNumberColor,
    MorphCodeColor,
    LemmaColor,
    JesusWordColor
  };

  CEntryDisplay();
	virtual ~CEntryDisplay();
  /**
  * Returns the rendered text using the modules in the list and using the key parameter.
  *  The displayoptions and filter options are used, too.
  */
  virtual const QString text( QPtrList <CSwordModuleInfo> modules, const QString& key, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions, const QString& displayTemplateName = CBTConfig::get(CBTConfig::displayStyle));
  /**
  * Returns a preview for the given module and key.
  * This is useful for the seatchdialog and perhaps the tooltips.
  */
//  virtual const QString previewText( CSwordModuleInfo*  module, const QString& key, const QString& headerText = QString::null,  CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults(), CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults());
  void setDisplayOptions(const CSwordBackend::DisplayOptions options);

  /**
  * Returns the font of the given type.
  */
//  const QFont font( const CLanguageMgr::Language& lang );

protected:
	friend class CExportManager;
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

class CChapterDisplay : public CEntryDisplay, public CTextRendering  {
public: // Public methods
  /**
  * Returns the rendered text using the modules in the list and using the key parameter.
  *  The displayoptions and filter options are used, too.
  */
  virtual const QString text( QPtrList <CSwordModuleInfo> modules, const QString& key, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions, const QString& displayTemplateName = CBTConfig::get(CBTConfig::displayStyle));

protected:
  /**
  * Renders one entry using the given modules and the key.
  * This makes chapter rendering more easy.
  */
  //virtual const QString entryText( QPtrList<CSwordModuleInfo> modules, const QString& key, const QString& chosenKey);
	virtual const QString renderEntry(ListCSwordModuleInfo const* list, CSwordKey* key);
};

class CBookDisplay : public CEntryDisplay  {
public: // Public methods
  /**
  * Returns the rendered text using the modules in the list and using the key parameter.
  *  The displayoptions and filter options are used, too.
  */
  virtual const QString text( QPtrList <CSwordModuleInfo> modules, const QString& key, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions, const QString& displayTemplateName = CBTConfig::get(CBTConfig::displayStyle));

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
