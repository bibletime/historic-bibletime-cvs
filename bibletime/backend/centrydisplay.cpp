/***************************************************************************
                          centrydisplay.cpp  -  description
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

//BibleTime includes
#include "centrydisplay.h"
#include "cswordkey.h"
#include "cswordversekey.h"
#include "cswordbookmoduleinfo.h"
#include "creferencemanager.h"

#include "frontend/cbtconfig.h"

#include "util/scoped_resource.h"

//Qt includes
#include <qapplication.h>

CEntryDisplay::CEntryDisplay(){
}

CEntryDisplay::~CEntryDisplay(){
}

/** Returns the rendered text using the modules in the list and using the key parameter. The displayoptions and filter options are used, too. */
const QString CEntryDisplay::text( QPtrList<CSwordModuleInfo> modules, const QString& keyName, CSwordBackend::DisplayOptionsBool displayOptions, CSwordBackend::FilterOptionsBool filterOptions ) {
  backend()->setDisplayOptions( displayOptions );
  backend()->setFilterOptions( filterOptions );

  const QString entry = entryText(modules, keyName);
  return finishText( entry, modules, keyName );
}

/** Returns a preview for the given module and key. This is useful for the seatchdialog and perhaps the tooltips. */
const QString CEntryDisplay::previewText( CSwordModuleInfo*  module, const QString& keyName, const QString& headerText, CSwordBackend::DisplayOptionsBool displayOptions, CSwordBackend::FilterOptionsBool filterOptions){
  util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
  key->key(keyName);

  QString css = QString::null;
  for (int t = MinType; t <= MaxType; ++t) {
    css += "\t" + cssString( static_cast<CEntryDisplay::StyleType>(t) ) + "\n\n";
  }

  const QString pageStart = QString::fromLatin1("<HTML><HEAD><STYLE type=\"text/css\">%1</STYLE></HEAD><BODY>%2<HR>")
    .arg(css)
    .arg(headerText);

  const QString text = QString::fromLatin1("<DIV %1 %2>%3</DIV>")
    .arg(module->textDirection() == CSwordModuleInfo::RightToLeft ? QString::fromLatin1("dir=\"rtl\"") : QString::null)
    .arg(module->isUnicode() ? QString::fromLatin1("class=\"unicodetext\"") : QString::null)
    .arg(key->renderedText());

  const QString pageEnd = QString::fromLatin1("</BODY></HTML>");
  return pageStart + text + pageEnd;
}

/** Renders one entry using the given modules and the key. This makes chapter rendering more easy. */
const QString CEntryDisplay::entryText( QPtrList<CSwordModuleInfo> modules, const QString& keyName ){
	Q_ASSERT(modules.first());
  util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(modules.first()) );
  key->key(keyName);
  QString renderedText = QString::null;

  renderedText = QString::fromLatin1("<TR valign=\"top\">");
  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key->module(m);
    key->key(keyName); //necessary?
    renderedText += QString::fromLatin1("<TD  valign=\"top\"><SPAN %1>%2</SPAN></TD>")
                      .arg(m->isUnicode() ? "class=\"unicodetext\"" : "")
                      .arg(key->renderedText());
  }
  renderedText += QString::fromLatin1("</TR>");
  return renderedText;
}

/** Returns the color of the given type. */
const QColor CEntryDisplay::color( const CEntryDisplay::ColorType type ) {
  switch (type) {
    case ReferenceColor:
      return CBTConfig::get(CBTConfig::swordRefColor);
    case TextColor:
      return CBTConfig::get(CBTConfig::textColor);
    case HighlightedTextColor:
      return CBTConfig::get(CBTConfig::highlightedVerseColor);
    case BackgroundColor:
      return CBTConfig::get(CBTConfig::backgroundColor);
    case BackgroundColor2:
      return CBTConfig::get(CBTConfig::background2Color);
    case FootnoteColor:
      return CBTConfig::get(CBTConfig::footnotesColor);
    case StrongsNumberColor:
      return CBTConfig::get(CBTConfig::strongsColor);
    case MorphCodeColor:
      return CBTConfig::get(CBTConfig::morphsColor);
//    case LemmaColor:
//      return CBTConfig::get(CBTConfig::lemmaColor);
    case JesusWordColor:
      return CBTConfig::get(CBTConfig::jesuswordsColor);
    default:
      return Qt::white;
  }
}

/** Returns the font of the given type. */
const QFont CEntryDisplay::font( const CEntryDisplay::FontType type ) {
  switch (type) {
    case UnicodeFont:
      return CBTConfig::get(CBTConfig::unicode);
    case StandardFont:
      return CBTConfig::get(CBTConfig::standard);
    default:
      return QApplication::font();
  }
}

/** Adds the right headers and footers to the page and returns them together. */
const QString CEntryDisplay::finishText( const QString text, QPtrList <CSwordModuleInfo> modules, const QString& keyName){
  CSwordModuleInfo* module = modules.first();
  Q_ASSERT(module); //shouldn't happen
  util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(modules.first()) );
  key->key(keyName);

  QString css = "table.maintable {width:100%;} td.tableheading {border-bottom: thin solid black;}";
  for (int t = MinType; t <= MaxType; ++t) {
    css += "\t" + cssString( static_cast<CEntryDisplay::StyleType>(t) ) + "\n\n";
  }

  const int columnWidth = (int)((float)100 / (float)modules.count());

  QString pageStart = QString::fromLatin1("<HTML><HEAD><STYLE type=\"text/css\">%1</STYLE></HEAD><BODY><TABLE cellspacing=\"0\" class=\"maintable\"><TR>")
    .arg(css);

  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key->module(m);
    const QString newKeyName = key->key();
    pageStart += QString::fromLatin1("<TD class=\"tableheading\" width=\"%1%\"><CENTER><B>%2</B> %3</CENTER></TD>")
      .arg(columnWidth)
      .arg(m->name())
      .arg(!newKeyName.isEmpty() ? QString::fromLatin1("(%1)").arg(htmlReference(m, newKeyName, newKeyName, newKeyName)) : QString::null);
  }
  pageStart += QString::fromLatin1("</TR>");

  const QString pageEnd = QString::fromLatin1("</TABLE></BODY></HTML>");

  return pageStart + text + pageEnd;
}

const QString CEntryDisplay::cssString( const CEntryDisplay::StyleType type ){
  const QString bgColor = color(BackgroundColor).name();
  const QString bgColor2 = color(BackgroundColor2).name();

  const QString textColor = color(TextColor).name();
  const QString highlightColor = color(HighlightedTextColor).name();
  const QString swordRefColor = color(ReferenceColor).name();
  const QString footnotesColor = color(FootnoteColor).name();

  const QString strongsColor = color(StrongsNumberColor).name();
  const QString morphsColor = color(MorphCodeColor).name();
//  const QString lemmaColor = color(LemmaColor).name();

  const QString jesusWordsColor = color(JesusWordColor).name();


  QString text;
  switch(type) {
    case Body:
      text =  QString::fromLatin1("body {%1; color: %2; font-size: %3pt; font-family: %4;}")
                .arg(QString::fromLatin1("background-color: %1").arg(bgColor))
                .arg(textColor)
                .arg(font(StandardFont).pointSize())
                .arg(font(StandardFont).family());
      break;
    case Link:
      text = QString::fromLatin1("a:link {text-decoration:none;}");
//              .arg(swordRefColor);
      break;
    case LinkHover:
      text = QString::fromLatin1("a:hover {text-decoration:none;}");
      break;
    case Background:
      text = QString::fromLatin1(".background1 {background-color:%1;}")
              .arg(bgColor);
      break;
    case Background2:
      text = QString::fromLatin1(".background2 {background-color:%1;}")
              .arg(bgColor2);
      break;
    case HighlightedText:
      text = QString::fromLatin1(".highlighted {color: %1;}")
              .arg(highlightColor);
      break;
    case UnicodeText:
      text = QString::fromLatin1(".unicodetext { font-family: %1; font-size:%2pt; }")
                .arg(font(UnicodeFont).family())
                .arg(font(UnicodeFont).pointSize());
      break;
    case Reference:
      text = QString::fromLatin1(".reference { color: %1; font-decoration: none; font-weight:bold; }")
              .arg(swordRefColor);
      break;
    case MorphCode:
      text = QString::fromLatin1(".morphcode {font-size: smaller; color: %1; font-decoration:none;}")
              .arg(morphsColor);
      break;
    case StrongNumber:
      text = QString::fromLatin1(".strongnumber {font-size: smaller; color: %1; font-decoration:none;}")
              .arg(strongsColor);
      break;
    case Lemma:
//      text = QString::fromLatin1(".lemma {font-size: smaller; color: %1; font-decoration:none;}")
//              .arg(lemmaColor);
      break;
    case Footnote:
      text = QString::fromLatin1(".footnote  {font-size: smaller; color: %1; font-style:italic;}")
              .arg(footnotesColor);
      break;
    case FootnotePre:
      text = QString::fromLatin1(".footnotepre {font-size: smaller; color: %1; font-weight: bolder;}")
              .arg(footnotesColor);
      break;
    case Poetry:
      text = QString::fromLatin1(".poetry {font-weight: lighter; alignment: justify;}");
      break;
    case Quotation:
      text = QString::fromLatin1(".quotation {font-style: italic;}");
      break;
    case JesusWords:
      text = QString::fromLatin1(".jesuswords {font-style: italic; color: %1;}")
              .arg(jesusWordsColor);
      break;
    case BookTitle:
      text = QString::fromLatin1(".booktitle {font-weight: x-bold; font-size: x-large; color: %1; margin-top:1mm; margin-bottom:1mm;}")
              .arg(textColor);
      break;
    case SectionTitle:
      text = QString::fromLatin1(".sectiontitle {font-size: larger; font-weight: bold; color: %1;}")
              .arg(textColor);
      break;
    break;
  };
  return QString::fromLatin1("\n\n%1\n\n").arg(text);
}

void CEntryDisplay::setDisplayOptions(const CSwordBackend::DisplayOptionsBool options) {
  m_displayOptions = options;
}

/** Returns the right reference text which can be incluced in the HTML */
const QString CEntryDisplay::htmlReference( CSwordModuleInfo* module, const QString& keyName, const QString linkText, const QString& anchorText ) {
  if (linkText.isEmpty())
    return QString::null;
  else
    return QString::fromLatin1("<A NAME=\"%1\" HREF=\"%2\"><SPAN CLASS=\"reference\" %3>%4</SPAN></A>")
      .arg(anchorText)
      .arg(CReferenceManager::encodeHyperlink(module->name(), keyName, CReferenceManager::typeFromModule(module->type()) ))
      .arg(module->textDirection() == CSwordModuleInfo::RightToLeft ? QString::fromLatin1("dir=\"rtl\"") : QString::null)
      .arg(linkText);
}

/* ----------------------- new class: CChapterDisplay ------------------- */

/** Returns the rendered text using the modules in the list and using the key parameter. The displayoptions and filter options are used, too. */
const QString CChapterDisplay::text( QPtrList <CSwordModuleInfo> modules, const QString& keyName, CSwordBackend::DisplayOptionsBool displayOptions, CSwordBackend::FilterOptionsBool filterOptions ) {	
  backend()->setDisplayOptions( displayOptions );
  backend()->setFilterOptions( filterOptions );


  QString text = QString::null;

  CSwordVerseKey key(0);
  key.key(keyName);
  const int currentTestament = key.Testament();	
	const int currentBook = key.Book();
	const int currentChapter = key.Chapter();
	CSwordModuleInfo* module = modules.first();
  bool ok = true;
	for (key.Verse(1); key.Testament() == currentTestament && key.Book() == currentBook && key.Chapter() == currentChapter && ok && !module->module()->Error(); ok = key.next(CSwordVerseKey::UseVerse)) {
    text += entryText(modules, key.key(), keyName);
	}
  return finishText(text, modules, QString::null);
}

/** Returns a preview for the given module and key. This is useful for the seatchdialog and perhaps the tooltips. */
//const QString CChapterDisplay::previewText( CSwordModuleInfo*  module, const QString& key,const QString& headerText, CSwordBackend::DisplayOptionsBool displayOptions, CSwordBackend::FilterOptionsBool filterOptions){
//  return QString::null;
//}

/** Renders one entry using the given modules and the key. This makes chapter rendering more easy. */
const QString CChapterDisplay::entryText( QPtrList<CSwordModuleInfo> modules, const QString& keyName, const QString& chosenKey ){
  CSwordVerseKey key(modules.first());
  QString renderedText = QString::null;

  renderedText = QString::fromLatin1("<TR valign=\"top\">");
  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key.module(m);
    key.key(keyName); //necessary?
    const QString tdStyle = QString::fromLatin1("style=\"border-bottom:thin solid black; %1 %2\"")
      .arg((modules.at()+1 < modules.count()) ? QString::fromLatin1("padding-right: 2mm; border-right:thin solid black;") : QString::null)
      .arg((modules.at()>0 && modules.at()+1 <= modules.count()) ? QString::fromLatin1("padding-left:2mm;") : QString::null);

    renderedText += QString::fromLatin1("<TD class=\"%1\" %2 %3 valign=\"top\"><SPAN %4><SUP>%5</SUP> %6</SPAN></TD>")
                      .arg((modules.at() % 2) == 0 ? QString::fromLatin1("background1") : QString::fromLatin1("background2") )
                      .arg((m->textDirection() == CSwordModuleInfo::RightToLeft) ? QString::fromLatin1("dir=\"rtl\"") : QString::null)
                      .arg(tdStyle)
                      .arg(m->isUnicode() ? QString::fromLatin1("class=\"unicodetext\"") : QString::null)
                      .arg(m_displayOptions.verseNumbers ? htmlReference(m, key.key(), QString::number(key.Verse()), key.key())  : QString::null)
                      .arg(key.key() == chosenKey ? QString::fromLatin1("<SPAN class=\"highlighted\">%1</SPAN>").arg(key.renderedText()) : key.renderedText());
  }
  renderedText += QString::fromLatin1("</TR>");
  return renderedText;
}

const QString CChapterDisplay::finishText( const QString text, QPtrList <CSwordModuleInfo> modules, const QString& keyName) {
  CSwordModuleInfo* module = modules.first();
  Q_ASSERT(module); //shouldn't happen
  util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(modules.first()) );
  key->key(keyName);

  QString css = "table.maintable {width:100%;} td.tableheading {border-bottom: thin solid black;}";
  for (int t = MinType; t <= MaxType; ++t) {
    css += "\t" + cssString( static_cast<CEntryDisplay::StyleType>(t) ) + "\n\n";
  }

  const int columnWidth = (int)((float)100 / (float)modules.count());

  QString pageStart = QString::fromLatin1("<HTML><HEAD><STYLE type=\"text/css\">%1</STYLE></HEAD><BODY><TABLE cellspacing=\"0\" class=\"maintable\"><TR>")
    .arg(css);
  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key->module(m);
    const QString newKeyName = key->key();
    pageStart += QString::fromLatin1("<TD class=\"tableheading\" width=\"%1%\"><CENTER><B>%2</B> %3</CENTER></TD>")
      .arg(columnWidth)
      .arg(m->name())
      .arg(!keyName.isEmpty() ? htmlReference(m, newKeyName, newKeyName, newKeyName) : QString::null);
  }
  pageStart += QString::fromLatin1("</TR>");

  QString pageEnd = QString::fromLatin1("</TABLE></BODY></HTML>");
  return pageStart + text + pageEnd;
}

/* ----------------------- new class: CBookDisplay ------------------- */

/** Returns the rendered text using the modules in the list and using the key parameter. The displayoptions and filter options are used, too. */
const QString CBookDisplay::text( QPtrList <CSwordModuleInfo> modules, const QString& keyName, CSwordBackend::DisplayOptionsBool displayOptions, CSwordBackend::FilterOptionsBool filterOptions ) {
  qWarning("CBookDisplay::text: %s", keyName.latin1());
  if (keyName.isNull())
    qWarning("null");
  if (keyName.isEmpty())
    qWarning("empty");

  backend()->setDisplayOptions( displayOptions );
  backend()->setFilterOptions( filterOptions );

  QString text;
	CSwordBookModuleInfo* book = dynamic_cast<CSwordBookModuleInfo*>(modules.first());
  util::scoped_ptr<CSwordTreeKey> key( dynamic_cast<CSwordTreeKey*>( CSwordKey::createInstance(book) ) );
	key->key(keyName);

  qWarning("used key is %s", key->key().latin1());

	int displayLevel = book->config( CSwordModuleInfo::DisplayLevel ).toInt();
  if (displayLevel <= 1) { //don't display entries together
    return finishText( entryText(modules, keyName), modules, keyName );
  }

  --displayLevel; //better handling if 1 is to concat the last level
	int moved = 0;
	
	while (key->firstChild())
		++moved; //down

	for (int i = 1; i < displayLevel; i++) {
		if (!key->parent() || key->key() == "/" || key->key().isEmpty()) { //first entry
			break;		
		}
		--moved; //up
	};
	
	if (moved <= 1) { //display entries together
		while(key->previousSibling()); //first entry of it's parent		
		m_text = QString::null;
    printTree(*key, modules);	
	}
	else { //do not display entries together
    return finishText( entryText(modules, keyName), modules, keyName );
	}	
	key->key(keyName);
  return finishText(m_text, modules, keyName);
}

/** Returns a preview for the given module and key. This is useful for the seatchdialog and perhaps the tooltips. */
//const QString CBookDisplay::previewText( CSwordModuleInfo*  module, const QString& key,const QString& headerText, CSwordBackend::DisplayOptionsBool displayOptions, CSwordBackend::FilterOptionsBool filterOptions){
//  return QString::null;
//}

/** Renders one entry using the given modules and the key. This makes chapter rendering more easy. */
const QString CBookDisplay::entryText( QPtrList<CSwordModuleInfo> modules, const QString& keyName, const int level){
	CSwordBookModuleInfo* book = dynamic_cast<CSwordBookModuleInfo*>(modules.first());
  util::scoped_ptr<CSwordTreeKey> key( dynamic_cast<CSwordTreeKey*>( CSwordKey::createInstance(book) ) );

  key->key(keyName);

  return QString::fromLatin1("<TR><TD STYLE=\"padding-left: %1px;\"><SUP>%2</SUP> %3</TD></TR>")
    .arg(level*30)
    .arg(htmlReference(book, keyName, key->getLocalName(), !key->key().isEmpty() ? key->key() : "/" ))
    .arg(key->renderedText());
}

void CBookDisplay::printTree(CSwordTreeKey treeKey, QPtrList<CSwordModuleInfo> modules, const int levelPos){
  m_text += entryText(modules, treeKey.getFullName(), levelPos);

  if (treeKey.hasChildren()) {
    treeKey.firstChild();
    printTree(treeKey, modules, levelPos+1);
    treeKey.parent();
  }
  if (treeKey.nextSibling())
		printTree(treeKey, modules, levelPos);
}

const QString CBookDisplay::finishText( const QString text, QPtrList <CSwordModuleInfo> modules, const QString& keyName){
 	CSwordBookModuleInfo* book = dynamic_cast<CSwordBookModuleInfo*>(modules.first());
  util::scoped_ptr<CSwordTreeKey> key( dynamic_cast<CSwordTreeKey*>( CSwordKey::createInstance(book) ) );
  key->key(keyName);

  QString css = "table.maintable {width:100%;} td.tableheading {border-bottom: thin solid black;}";
  for (int t = MinType; t <= MaxType; ++t) {
    css += "\t" + cssString( static_cast<CEntryDisplay::StyleType>(t) ) + "\n\n";
  }

  const int columnWidth = (int)((float)100 / (float)modules.count());

  QString pageStart = QString::fromLatin1("<HTML><HEAD><STYLE type=\"text/css\">%1</STYLE></HEAD><BODY><TABLE cellspacing=\"0\" class=\"maintable\"><TR>")
    .arg(css);

  for (CSwordModuleInfo* m = modules.first(); m; m = modules.next()) {
    key->module(m);
    const QString newKeyName = !key->key().isEmpty() ? key->key() : "/";
    pageStart += QString::fromLatin1("<TD class=\"tableheading\" width=\"%1%\"><CENTER><B>%2</B> %3</CENTER></TD>")
      .arg(columnWidth)
      .arg(m->name())
      .arg(!newKeyName.isEmpty() ? QString::fromLatin1("(%1)").arg(htmlReference(m, newKeyName, newKeyName, QString::null)) : QString::null);
  }
  pageStart += QString::fromLatin1("</TR>");

  const QString pageEnd = QString::fromLatin1("</TABLE></BODY></HTML>");

  return pageStart + text + pageEnd;
}
