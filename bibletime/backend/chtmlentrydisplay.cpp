/***************************************************************************
                          chtmlentrydisplay.cpp  -  description
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

//BibleTime includes
#include "chtmlentrydisplay.h"
#include "cswordmoduleinfo.h"
#include "cswordkey.h"
#include "cswordldkey.h"
#include "cswordversekey.h"
#include "creferencemanager.h"
#include "../frontend/ctoolclass.h"
#include "../frontend/cbtconfig.h"

#include "../util/scoped_resource.h"


//Qt includes
#include <qfont.h>
#include <qregexp.h>

//Sword includes
#include <versekey.h>

CHTMLEntryDisplay::CHTMLEntryDisplay(){
//	m_htmlHeader = QString::fromLatin1("<html><head>\
//<style type=\"text/css\">\
//a:link {color: %1}\n\
//a:hover {color: %2}\
//</style></head>").arg(m_swordRefColorName).arg(m_swordRefColorName);
//
//  qWarning(m_swordRefColorName.latin1());

	m_includeHeader = true;
}

CHTMLEntryDisplay::~CHTMLEntryDisplay(){
}

void CHTMLEntryDisplay::updateSettings(void){
  m_highlightedVerseColorName = CBTConfig::get(CBTConfig::highlightedVerseColor).name();
	m_standardFontColorName 		= CBTConfig::get(CBTConfig::textColor).name();
	m_swordRefColorName 				= CBTConfig::get(CBTConfig::swordRefColor).name();

  m_standardFontName = CBTConfig::get(CBTConfig::standard).family();
  m_standardFontSize = /*CToolClass::makeLogicFontSize*/(CBTConfig::get(CBTConfig::standard).pointSize() );

	m_unicodeFontName  = CBTConfig::get(CBTConfig::unicode).family();
  m_unicodeFontSize  = /*CToolClass::makeLogicFontSize*/( CBTConfig::get(CBTConfig::unicode).pointSize() );

	m_htmlHeader = header();

}

/** Displays the current entry of the module as HTML */
char CHTMLEntryDisplay::Display(CSwordModuleInfo* module) {
	if (!module) {
		m_htmlText = QString::null;
		return -1;
	}
  //refresh font settings
	updateSettings();
	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
	if (!key) {
		m_htmlText = QString::null;
		return 0;
	}	
	key->key( module->module()->KeyText() );
	
	CReferenceManager::Type refType = CReferenceManager::Unknown;
	if (module->type() == CSwordModuleInfo::Bible)
		refType = CReferenceManager::Bible;
	else if (module->type() == CSwordModuleInfo::Commentary)
		refType = CReferenceManager::Commentary;
	else if (module->type() == CSwordModuleInfo::Lexicon)
		refType = CReferenceManager::Lexicon;
	else if (module->type() == CSwordModuleInfo::GenericBook)
		refType = CReferenceManager::GenericBook;
	
	if (m_includeHeader) {
		m_htmlText = m_htmlHeader;

    m_htmlText += QString::fromLatin1("<body>%1: <span id=\"highlighted\"><A HREF=\"%2\">%3</A></span>")
      .arg(module->config(CSwordModuleInfo::Description))
 			.arg(CReferenceManager::encodeHyperlink(module->name(),key->key(), refType))
      .arg(key->key());

		m_htmlText += QString::fromLatin1("<span %1>%1</span>")
      .arg(module->isUnicode() ? "id=\"unicodetext\"" : "")
//      .arg( module->isUnicode() ? m_unicodeFontName : m_standardFontName)
//			.arg( module->isUnicode() ? m_unicodeFontSize : m_standardFontSize)
      .arg( key->renderedText() );

    m_htmlText += "</body></html>";
	}
	else {
		m_htmlText = key->renderedText();
	}

	return 1;
}

/** Generates code to display the given modules side by side. */
char CHTMLEntryDisplay::Display( QPtrList<CSwordModuleInfo>* moduleList) {
//	qDebug("CHTMLEntryDisplay::Display( QList<CSwordModuleInfo>* moduleList)");
	if (!moduleList || (moduleList && !moduleList->count()) ) {
		m_htmlText = QString::null;
		return 0;
	}
	//reload font settings
	updateSettings();

 	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(moduleList->first()) );
	  	
	SWModule* module = moduleList->first()->module();
//	ASSERT(module);
	QString usedKey = QString::null;
	CSwordModuleInfo *d = 0;	
	
	SWModule *m= (d = moduleList->first()) ? d->module() : 0;
	SWKey* sw = (SWKey*)(*module);
	key->key(*sw);	
	usedKey = key->key();
 	m = (d = moduleList->first()) ? d->module() : 0;	
 	while (m) {
   	m = (d=moduleList->next()) ? d->module() : 0;
   	if (m) {
			m->SetKey( (const char*)usedKey.local8Bit() );
   		d->snap();			
		}
 	}	

	const int width=(int)((double)100/(double)moduleList->count());
	m_htmlText = m_htmlHeader + QString::fromLatin1("<TABLE cellpadding=\"2\" cellspacing=\"0\"><TR>"); 	
	m_htmlText.append(QString("<font face=\"%1\" size=\"%2\" color=\"%3\">")
		.arg(m_standardFontName).arg(m_standardFontSize).arg(m_standardFontColorName));
	
	m = (d = moduleList->first()) ? d->module() : 0;		
	while (m) {
		key->module(d);
    if (m){
    	m_htmlText.append(QString("<td width=\"%1\" bgcolor=\"#f1f1f1\"><b>%2 ")
				.arg(width).arg(d->name()));
			m_htmlText.append(QString("(<font color=\"%1\" face=\"%2\" size=\"%3\">%4</font>)</b></td>")
				.arg(m_highlightedVerseColorName)
        .arg((d && d->isUnicode() ) ? m_unicodeFontName : m_standardFontName)
	  		.arg((d && d->isUnicode() ) ? m_unicodeFontSize : m_standardFontSize)
     		.arg(key->key())
			);
		}
		m = (d=moduleList->next()) ? d->module() : 0;			
	}
	m_htmlText.append(QString::fromLatin1("</TR>"));
			
	m = (d = moduleList->first()) ? d->module() : 0;
	m_htmlText += QString::fromLatin1("<TR>");
	while (m) {
		key->module(d);
		key->key(usedKey);
		
		m_htmlText +=
			QString::fromLatin1("<td width=\"%1%\"><FONT face =\"%2\" size=\"%3\">%4</font></td>")
				.arg(width)
				.arg((d && d->isUnicode() ) ? m_unicodeFontName : m_standardFontName)
				.arg((d && d->isUnicode() ) ? m_unicodeFontSize : m_standardFontSize)
				.arg(key->renderedText());
		m = (d = moduleList->next()) ? d->module() : 0;		
	}
	m_htmlText += QString::fromLatin1("</font></tr></table></body></html>");

	//clean up
//	delete key;	
	return 1;
}

/** Returns the header which should be used for each page. */
const QString& CHTMLEntryDisplay::header(){
  m_htmlHeader = QString::fromLatin1("<HTML><HEAD>\n");

  m_htmlHeader += QString::fromLatin1("\n<style type=\"text/css\">\n\n");

  for (int t = MinType; t < MaxType; ++t) {
    m_htmlHeader += "\t" + formatString(static_cast<CHTMLEntryDisplay::StyleType>(t)) + "\n\n";
  }
  m_htmlHeader += QString::fromLatin1("</style>\n\n");

// #unicodetext { font-family: !unicodefamily!; }\n\
// #highlighted { color: !highlightedcolor!; }\n\
// #reference { color: !refcolor!; font-decoration: none; }\n\
// #jesuswords {color: !jesuswordscolor!; text-weight:bolder;}\n\
// #otquote {font-size: smaller;}\n\
// #poetry  {font-weight: light; text-align: justify;}\n\
// #sectionhead  {font-size: larger; font-weight: bold; color: !textcolor!;}\n\
// #booktitle  {font-weight: x-bold; font-size: x-large; color: !textcolor!; margin-top:1mm;margin-bottom:1mm;}\n\
// #strongnumber  {font-decoration: none; font-size: smaller; font-weight:lighter; font-style:italic; color: !strongscolor!;}\n\
// #morphcode  {font-size: smaller; color: !morphcolor!; font-decoration:none;}\n\
// #footnote  {font-size: smaller; color: !footnotecolor!; font-style:italic;}\n\
// #footnotepre {font-weight: bolder;}\n\

  m_htmlHeader += QString::fromLatin1("</HEAD>");
  return m_htmlHeader;
}

/** Returns the string which is used for the given styleType, */
const QString CHTMLEntryDisplay::formatString( const CHTMLEntryDisplay::StyleType type){
  const QString bgColor = CBTConfig::get(CBTConfig::backgroundColor).name();
  const QString textColor = CBTConfig::get(CBTConfig::textColor).name();
  const QString highlightColor = CBTConfig::get(CBTConfig::highlightedVerseColor).name();
  const QString swordRefColor = CBTConfig::get(CBTConfig::swordRefColor).name();
  const QString footnotesColor = CBTConfig::get(CBTConfig::footnotesColor).name();
  const QString morphsColor = CBTConfig::get(CBTConfig::morphsColor).name();
  const QString strongsColor = CBTConfig::get(CBTConfig::strongsColor).name();
  const QString jesusWordsColor = CBTConfig::get(CBTConfig::jesuswordsColor).name();
//  const QString lemmaColor = CBTConfig::get(CBTConfig::strongsColor).name();

  QString text;
  switch(type) {
    case Body:
      text =  QString::fromLatin1("body {background-color: %1; color: %2; font-size: %3pt; font-family: %4;}")
                .arg(bgColor)
                .arg(textColor)
                .arg(m_standardFontSize)
                .arg(m_standardFontName);
      break;
    case Link:
      text = QString::fromLatin1("a:link {text-decoration:none; color: %1}").arg(swordRefColor);
      break;
    case LinkHover:
      text = QString::fromLatin1("a:hover {text-decoration:none; font-weight:bolder;}");
      break;
    case HighlightedText:
      text = QString::fromLatin1("#highlighted {color: %1;}")
                .arg(highlightColor);
      break;
    case UnicodeText:
      text = QString::fromLatin1("#unicodetext { font-family: %1; font-size:%2pt; }")
                .arg(m_unicodeFontName)
                .arg(m_unicodeFontSize);
      break;
    case Reference:
      text = QString::fromLatin1("#reference { color: %1; font-decoration: none; }")
              .arg(swordRefColor);
      break;
    case MorphCode:
      text = QString::fromLatin1("#morphcode {font-size: smaller; color: %1; font-decoration:none;}")
              .arg(morphsColor);
      break;
    case StrongNumber:
      text = QString::fromLatin1("#strongnumber {font-size: smaller; color: %1; font-decoration:none;}")
              .arg(strongsColor);
      break;
    case Lemma:
//      text = QString::fromLatin1("#lemma {font-size: smaller; color: %1; font-decoration:none;}")
//              .arg(lemmaColor);
      break;
    case Footnote:
      text = QString::fromLatin1("#footnote  {font-size: smaller; color: %1; font-style:italic;}")
              .arg(footnotesColor);
      break;
    case FootnotePre:
      text = QString::fromLatin1("#footnotepre {font-size: smaller; color: %1; font-weight: bolder;}")
              .arg(footnotesColor);
      break;
    case Poetry:
      text = QString::fromLatin1("#poetry {font-weight: lighter; alignment: justify;}");
      break;
    case Quotation:
      text = QString::fromLatin1("#quotation {font-style: italic;}");
      break;
    case JesusWords:
      text = QString::fromLatin1("#jesuswords {font-style: italic; color: %1;}")
              .arg(jesusWordsColor);
      break;

    case BookTitle:
      text = QString::fromLatin1("#booktitle {font-weight: x-bold; font-size: x-large; color: %1; margin-top:1mm; margin-bottom:1mm;}")
              .arg(textColor);
      break;
    case SectionTitle:
      text = QString::fromLatin1("#sectiontitle {font-size: larger; font-weight: bold; color: %1;}")
              .arg(textColor);
      break;
    break;
  };
  return text;
}
