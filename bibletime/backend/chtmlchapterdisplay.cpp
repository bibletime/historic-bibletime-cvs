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

	//reload font settings
	updateSettings();
  m_htmlHeader = header();

  if (module->textDirection() == CSwordModuleInfo::RightToLeft)
    m_htmlText = m_htmlHeader + QString::fromLatin1("\n<body dir=\"rtl\">\n");
  else
    m_htmlText = m_htmlHeader + QString::fromLatin1("\n<body>\n");

	bool ok = true;
	for (key.Verse(1); key.Testament() == currentTestament && key.Book() == currentBook && key.Chapter() == currentChapter && ok && !module->module()->Error(); ok = key.next(CSwordVerseKey::UseVerse)) {
		verse = key.Verse();
		if (m_displayOptionsBool.verseNumbers) {
			m_htmlText.append( QString::fromLatin1("<span id=\"reference\"><a name=\"%1\" href=\"%2\">%3</a></span>\n ")
				.arg(key.key())
				.arg(CReferenceManager::encodeHyperlink( module->name(), key.key(), CReferenceManager::typeFromModule(module->type()) ))
				.arg(verse)
			);
		}

	  m_htmlText += QString::fromLatin1("<span %1>\n<span %2 %3>\n%4\n</span></span>%5\n")
			.arg( verse == currentVerse ? "id=\"highlighted\"" : "" )
			.arg( module->isUnicode()   ? "id=\"unicodetext\"" : "" )
   		.arg( module->textDirection() == CSwordModuleInfo::RightToLeft ? QString::fromLatin1("dir=\"rtl\"") : "")
			.arg( key.renderedText() )
			.arg( m_displayOptionsBool.lineBreaks ? "<br>" : "");
	}
	
	m_htmlText += QString::fromLatin1("</body></html>");
	return 1;	//no error	
}

/** Generates code to display the given modules side by side. */
char CHTMLChapterDisplay::Display( QPtrList<CSwordModuleInfo>* moduleList){	
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
    m_htmlText += QString::fromLatin1("<body><table cellpadding=\"2\" cellspacing=\"0\"><tr><td bgcolor=\"%1\"></td>")
										.arg(m_background2ColorName);	
  else
    m_htmlText += QString::fromLatin1("<body><table cellpadding=\"2\" cellspacing=\"0\"><tr>");	

	m = (d = moduleList->first()) ? d->module() : 0;		
	while (m) {
    	m_htmlText.append(QString::fromLatin1("<td width=\"%1%\" bgcolor=\"%2\" >\
						<span>%3</span></td>")
				.arg(width)
				.arg(m_background2ColorName)	
				.arg(d->name()));
			m = (d=moduleList->next()) ? d->module() : 0;
	}

	CSwordVerseKey current(0);	
	QString rowText = QString::null;
	int currentVerse = 0;

	int currentRow = 0;
	
	for (key.Verse(1); key.Testament() == currentTestament && key.Book() == currentBook && key.Chapter() == currentChapter && !module->Error(); key.next(CSwordVerseKey::UseVerse)) {
		const QString currentKey = key.key();
		currentVerse = key.Verse();
		m = (d = moduleList->first()) ? d->module() : 0;
		if (m_displayOptionsBool.verseNumbers) {

  		 rowText = QString::fromLatin1("</tr><tr><td bgcolor=\"%1\">\
							<span id=\"reference\"><a name=\"%2\" href=\"%3\">%4</a></span></td>\n")
						.arg(m_background2ColorName)	
						.arg(currentVerse)
						.arg(CReferenceManager::encodeHyperlink( d->name(), currentKey, CReferenceManager::typeFromModule(d->type()) ))
						.arg(currentVerse);

    }
    else {
       rowText = QString::fromLatin1("</tr><tr>\n");
    }

		currentRow = 0;
		current.key(currentKey);	
		while (m) {
			current.module(d);

 			rowText += QString::fromLatin1("<td %1 bgcolor=\"%2\" width=\"%3%\">")
 				.arg(d->textDirection() == CSwordModuleInfo::RightToLeft ? "dir=\"rtl\"" : "" )
         .arg((currentVerse + currentRow)% 2 ? m_backgroundColorName : m_background2ColorName)
         .arg(width);

			qWarning("%d %d", currentVerse, currentRow);

 			rowText += QString::fromLatin1("<span %1>\n<span %2>\n%3\n</span>\n</span>\n</td>")
 					.arg(currentVerse == chosenVerse ? "id=\"highlighted\"" : "" )
 					.arg(d->isUnicode() ? "id=\"unicodetext\"" : "")
 					.arg( current.renderedText() );

			m = (d = moduleList->next()) ? d->module() : 0;
			currentRow++;
		}
		m_htmlText += rowText + QString::fromLatin1("</tr>\n");
	}
	m_htmlText += QString::fromLatin1("</font></table></font></body></html>");
	
	//clean up
	return 1;		
}

