/***************************************************************************
                          chtmlchapterdisplay.cpp  -  description
                             -------------------
    begin                : Thu Oct 12 2000
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

//BibleTiem includes
#include "chtmlchapterdisplay.h"
#include "cswordmoduleinfo.h"
#include "cswordversekey.h"
#include "frontend/ctoolclass.h"
#include "frontend/cbtconfig.h"
#include "creferencemanager.h"

//Qt includes
#include <qfont.h>
#include <qregexp.h>

//Sword includes
#include <versekey.h>

/** Renders the text and puts the result into the member variable m_htmlText */
char CHTMLChapterDisplay::Display( CSwordModuleInfo* module ){
	if (!module) {
		m_htmlText = QString::null;
		return 0; //error
	}
	
	CSwordVerseKey key(0);
	key.key( module->module()->KeyText() );	
	const int currentTestament = key.Testament();	
	const int currentBook = key.Book();
	const int currentChapter = key.Chapter();
	const int currentVerse = key.Verse();	
	
	key.module(module);	
	int verse = 0;
	
  m_htmlHeader = header();
  if (module->textDirection() == CSwordModuleInfo::RightToLeft)
    m_htmlText = m_htmlHeader + QString::fromLatin1("<body dir=\"rtl\">");
  else
    m_htmlText = m_htmlHeader + QString::fromLatin1("<body>");
	
	//reload font settings
	updateSettings();

//	m_htmlText.append(QString::fromLatin1("<font face=\"%1\" size=\"%2\">")
//		.arg(module->isUnicode() ? m_unicodeFontName : m_standardFontName)
//		.arg(module->isUnicode() ? m_unicodeFontSize : m_standardFontSize)
////		.arg(m_standardFontColorName)
//	);

	bool ok = true;
	for (key.Verse(1); key.Testament() == currentTestament && key.Book() == currentBook && key.Chapter() == currentChapter && ok && !module->module()->Error(); ok = key.next(CSwordVerseKey::UseVerse)) {
		verse = key.Verse();
		if (m_displayOptionsBool.verseNumbers) {
			m_htmlText.append( QString::fromLatin1("<span id=\"reference\"><a name=\"%1\" href=\"%2\">%3</a></span> ")
//				.arg(m_swordRefColorName)
				.arg(verse)
				.arg(CReferenceManager::encodeHyperlink( module->name(), key.key(), CReferenceManager::typeFromModule(module->type()) ))
				.arg(verse)
			);
		}
		if (verse == currentVerse)
		  m_htmlText += QString::fromLatin1("<span id=\"highlighted\">");
		m_htmlText += key.renderedText();
		if (verse == currentVerse)
		  m_htmlText += QString::fromLatin1("</span>");
		if (m_displayOptionsBool.lineBreaks)
			m_htmlText += QString::fromLatin1("<br>\n");
		else
			m_htmlText += QString::fromLatin1(" \n");
	}
	
	m_htmlText += QString::fromLatin1("</body></html>");
	return 1;	//no error	
}

/** Generates code to display the given modules side by side. */
char CHTMLChapterDisplay::Display( QPtrList<CSwordModuleInfo>* moduleList){	
#warning make table colors configurable
	if (!moduleList || (moduleList && !moduleList->count()) ) {
		m_htmlText = QString::null;
		return 0;
	}
	//reload font settings
	updateSettings();

	SWModule* module = moduleList->first()->module();
		
	VerseKey* vk = (VerseKey*)((SWKey*)(*module));
	CSwordVerseKey key(0);
	key.key((const char*)*vk);

	const int currentTestament = key.Testament();	
	const int currentBook = key.Book();
	const int currentChapter = key.Chapter();
	const int chosenVerse = key.Verse();
	const int width = (int)((double)(m_displayOptionsBool.verseNumbers ? 96 : 100)/(double)moduleList->count()); //width in per cent!!
	
	CSwordModuleInfo *d = 0;
	SWModule *m = (d = moduleList->first()) ? d->module() : 0;
			
	m_htmlHeader = header();
  m_htmlText = m_htmlHeader;
  if (m_displayOptionsBool.verseNumbers)
    m_htmlText += QString::fromLatin1("<body><table cellpadding=\"2\" cellspacing=\"0\"><tr><td bgcolor=\"#f1f1f1\"></td>");	
  else
    m_htmlText += QString::fromLatin1("<body><table cellpadding=\"2\" cellspacing=\"0\"><tr>");	

	m = (d = moduleList->first()) ? d->module() : 0;		
	while (m) {
    	m_htmlText.append(QString::fromLatin1("<td bgcolor=\"#f1f1f1\" width=\"%1%\">\
<font face=\"%2\" size=\"%3\" color=\"%4\"><b>%5</b></td>")
				.arg(width)
				.arg(m_standardFontName)
				.arg(m_standardFontSize)
				.arg(m_standardFontColorName)
				.arg(d->name()));
			m = (d=moduleList->next()) ? d->module() : 0;
	}

	CSwordVerseKey current(0);	
	QString rowText = QString::null;
	int currentVerse = 0;

	const QString text = QString::fromLatin1("</tr><tr><td bgcolor=\"#f1f1f1\"><b>\
<font face=\"%1\" size=\"%2\" color=\"%3\"><a name=\"%4\" href=\"%5\">%6</a></font></b></td>\n");
	
 const QString cell = QString::fromLatin1("<td dir=\"%1\" bgcolor=\"%2\" width=\"%3%\">");
	
	for (key.Verse(1); key.Testament() == currentTestament && key.Book() == currentBook && key.Chapter() == currentChapter && !module->Error(); key.next(CSwordVerseKey::UseVerse)) {
		const QString currentKey = key.key();
		currentVerse = key.Verse();
		m = (d = moduleList->first()) ? d->module() : 0;
		if (m_displayOptionsBool.verseNumbers) {
  		 rowText = text.arg(m_standardFontName)
						.arg(m_standardFontSize)
						.arg(m_swordRefColorName)
						.arg(currentVerse)
						.arg(CReferenceManager::encodeHyperlink( d->name(), currentKey, CReferenceManager::typeFromModule(d->type()) ))
						.arg(currentVerse);
    }
    else {
       rowText = QString::fromLatin1("</tr><tr>\n");
    }
		
		current.key(currentKey);	
		while (m) {
			current.module(d);
      rowText += cell.arg(d->textDirection() == CSwordModuleInfo::RightToLeft ? QString::fromLatin1("rtl") : QString::fromLatin1("ltor")  )
                     .arg(currentVerse % 2 ? "white" : "#f1f1f1")
                     .arg(width);

			if (d->isUnicode())
				rowText += QString::fromLatin1("<font face=\"%1\" size=\"%2\" color=\"%3\">")
						.arg(m_unicodeFontName)
						.arg(m_unicodeFontSize)
						.arg(currentVerse == chosenVerse ? m_highlightedVerseColorName : m_standardFontColorName);
			else
				rowText += QString::fromLatin1("<font face=\"%1\" size=\"%2\" color=\"%3\">")
						.arg(m_standardFontName)
						.arg(m_standardFontSize)
						.arg(currentVerse == chosenVerse ? m_highlightedVerseColorName : m_standardFontColorName);

			rowText += current.renderedText() + QString::fromLatin1("</font>");
			m = (d = moduleList->next()) ? d->module() : 0;
		}
		m_htmlText += rowText + QString::fromLatin1("</tr>\n");
	}
	m_htmlText += QString::fromLatin1("</font></table></font></body></html>");
	
	//clean up
	return 1;		
}

/** Returns the header which should be used for each page. */
const QString& CHTMLChapterDisplay::header(){
  m_htmlHeader = QString::fromLatin1("<HTML><HEAD>");

  m_htmlHeader += QString::fromLatin1("<style type=\"text/css\"> \
a:link {text-decoration:none;} \
body {background-color: !backgroundcolor!; color: !textcolor!;} \
#highlighted { color: !highlightedcolor!; } \
#reference { color: !refcolor!; font-decoration: none; } \
#jesuswords {color: !jesuswordscolor!; text-weight:bolder;} \
#otquote {font-size: smaller;} \
#poetry  {font-weight: light; text-align: justify;} \
#sectionhead  {font-size: larger; font-weight: bold; color: !textcolor!;} \
#booktitle  {font-weight: x-bold; font-size: x-large; color: !textcolor!; margi-top:1mm;margin-bottom:1mm;} \
#strongnumber  {font-decoration: none; font-size: smaller; font-weight:lighter; font-style:italic; color: !strongscolor!;} \
#morphcode  {font-size: smaller; color: !morphcolor!; font-decoration:none;} \
#footnote  {font-size: smaller; color: !footnotecolor!;font-style:italic;} \
#footnotepre {font-weight: bolder;} \
</style>");

  m_htmlHeader.replace(QRegExp("!backgroundcolor!"), CBTConfig::get(CBTConfig::backgroundColor).name());
  m_htmlHeader.replace(QRegExp("!highlightedcolor!"), CBTConfig::get(CBTConfig::highlightedVerseColor).name());
  m_htmlHeader.replace(QRegExp("!textcolor!"), CBTConfig::get(CBTConfig::textColor).name());
  m_htmlHeader.replace(QRegExp("!refcolor!"), CBTConfig::get(CBTConfig::swordRefColor).name());
  m_htmlHeader.replace(QRegExp("!jesuswordscolor!"), CBTConfig::get(CBTConfig::jesuswordsColor).name());
  m_htmlHeader.replace(QRegExp("!morphcolor!"), CBTConfig::get(CBTConfig::morphsColor).name());
  m_htmlHeader.replace(QRegExp("!strongscolor!"), CBTConfig::get(CBTConfig::strongsColor).name());
  m_htmlHeader.replace(QRegExp("!footnotecolor!"), CBTConfig::get(CBTConfig::footnotesColor).name());

  m_htmlHeader += QString::fromLatin1("</HEAD>");
  return m_htmlHeader;
}
