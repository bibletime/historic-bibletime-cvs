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
  enum StyleType {
    Body = 0,
    Link,
    LinkHover,
    Background,
//    Background2,
    HighlightedText,
    UnicodeText,

    Reference,

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

  enum FontType {
    UnicodeFont,
    StandardFont
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
  virtual const QString text( QPtrList <CSwordModuleInfo> modules, const QString& key, CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions);
  /**
  * Returns a preview for the given module and key.
  * This is useful for the seatchdialog and perhaps the tooltips.
  */
  virtual const QString previewText( CSwordModuleInfo*  module, const QString& key, const QString& headerText = QString::null,  CSwordBackend::DisplayOptions displayOptions = CBTConfig::getDisplayOptionDefaults(), CSwordBackend::FilterOptions filterOptions = CBTConfig::getFilterOptionDefaults());
  void setDisplayOptions(const CSwordBackend::DisplayOptions options);

  /**
  * Returns the font of the given type.
  */
  const QFont font( const CEntryDisplay::FontType type );
  /**
  * Returns the color of the given type.
  */
  const QColor color( const CEntryDisplay::ColorType type );
  const QString cssString( const CEntryDisplay::StyleType type );


protected:
  /**
  * Renders one entry using the given modules and the key.
  * This makes chapter rendering more easy.
  */
  virtual const QString entryText( QPtrList<CSwordModuleInfo> modules, const QString& key);
  /**
  * Adds the right headers and footers to the page and returns them together.
  */
  virtual const QString finishText( const QString text, QPtrList <CSwordModuleInfo> modules, const QString& keyName);
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

protected:
  /**
  * Renders one entry using the given modules and the key.
  * This makes chapter rendering more easy.
  */
  virtual const QString entryText( QPtrList<CSwordModuleInfo> modules, const QString& key, const QString& chosenKey);
  /**
  * Adds the right headers and footers to the page and returns them together.
  */
  virtual const QString finishText( const QString text, QPtrList <CSwordModuleInfo> modules, const QString& keyName);
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
  */
  virtual const QString entryText( QPtrList<CSwordModuleInfo> modules, const QString& key, const int level = 0, const bool activekey = false);
  void printTree(CSwordTreeKey treeKey, QPtrList<CSwordModuleInfo> modules, const int levelPos = 0);
  virtual const QString finishText( const QString text, QPtrList <CSwordModuleInfo> modules, const QString& keyName);

private:
  QString m_text;
  QString m_chosenKey;
};
#endif
