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
#include "../frontend/ctoolclass.h"
#include "creferencemanager.h"

//Qt includes
#include <qfont.h>

//Sword includes
#include <versekey.h>

/** Renders the text and puts the result into the member variable m_htmlText */
char CHTMLChapterDisplay::Display( CSwordModuleInfo* module ){
	if (!module) {
		m_htmlText = QString::null;
		return -1; //error
	}
	
	CSwordVerseKey key(0);
	key.key( module->module()->KeyText() );	
	const int currentTestament = key.Testament();	
	const int currentBook = key.Book();
	const int currentChapter = key.Chapter();
	const int currentVerse = key.Verse();	

	key.module(module);	
	int verse = 0;
	
	if (module->isUnicode()) {
		m_htmlHeader = "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"></head>";
	}
	m_htmlText = m_htmlHeader + QString::fromLatin1("<body>");//dir=\"%1\">").arg((module->getTextDirection() == CSwordModuleInfo::RTL) ? "rtl" : "ltr");
	
	//reload font settings
	updateSettings();

	m_htmlText.append(QString::fromLatin1("<font face=\"%1\" size=\"%2\" color=\"%3\">")
		.arg( module->isUnicode() ? m_unicodeFontName : m_standardFontName)
		.arg( module->isUnicode() ? m_unicodeFontSize : m_standardFontSize)
		.arg(m_standardFontColorName)
	);

	for (key.Verse(1); key.Testament() == currentTestament && key.Book() == currentBook && key.Chapter() == currentChapter && !module->module()->Error(); key.NextVerse()) {
		verse = key.Verse();
		if (m_displayOptionsBool.verseNumbers)
			m_htmlText.append( QString::fromLatin1("<font color=\"%1\"><a name=\"%2\" href=\"%3\"><b>%4</b></a></font> ")
				.arg(m_swordRefColorName)
				.arg(verse)
				.arg(CReferenceManager::encodeHyperlink( module->name(), key.key(), CReferenceManager::typeFromModule(module->type()) ))
				.arg(verse)
			);
		if (verse == currentVerse)
		  m_htmlText += QString::fromLatin1("<font color=\"") + m_highlightedVerseColorName + QString::fromLatin1("\">");
		m_htmlText.append(key.renderedText());
		if (verse == currentVerse)
		  m_htmlText += QString::fromLatin1("</font>");

		if (m_displayOptionsBool.lineBreaks)
			m_htmlText += QString::fromLatin1("<br>\n");
		else
			m_htmlText += QString::fromLatin1(" \n");
	}
	m_htmlText.append("</font>");
	m_htmlText.append(m_htmlBody);	

	return 1;	//no error	
}

/** Generates code to display the given modules side by side. */
char CHTMLChapterDisplay::Display( QList<CSwordModuleInfo>* moduleList){	
#warning make table colors configurable
//	qDebug("CHTMLChapterDisplay::Display( QList<CSwordModuleInfo>* moduleList)");
	if (!moduleList || (moduleList && !moduleList->count()) ) {
		m_htmlText = QString::null;
		return 0;
	}

	//reload font settings
	updateSettings();

	SWModule* module = moduleList->first()->module();		
		
	VerseKey* vk = (VerseKey*)((SWKey*)(*module));
	CSwordVerseKey key(/*moduleList->first()*/0);
	key.key((const char*)*vk);

	const int currentTestament = key.Testament();	
	const int currentBook = key.Book();
	const int currentChapter = key.Chapter();
	const int chosenVerse = key.Verse();
	const int width = (int)((double)97/(double)moduleList->count()); //width in per cent!!
	CSwordModuleInfo *d = 0;
			
	m_htmlText = QString::fromLatin1("<qt text=\"%1\">").arg(m_standardFontColorName); //for compatibility with Qt's rendering	
	m_htmlText += m_htmlHeader + QString::fromLatin1("<body text=\"%1\">").arg(m_standardFontColorName);


	m_htmlText +=
		QString::fromLatin1("<table cellpadding=\"2\" cellspacing=\"0\"><td bgcolor=\"#f1f1f1\"></td>");	
	SWModule *m = (d = moduleList->first()) ? d->module() : 0;

	while (m) {
    if (m)
			m_htmlText +=
				QString::fromLatin1("<td width=\"%1%\" bgcolor=\"#f1f1f1\"><b>%1</b></td>").arg(width).arg(d->name());
		m = (d=moduleList->next()) ? d->module() : 0;			
	}
	m_htmlText.append("</tr>");
		
	QString rowText = QString::null;
	int currentVerse = 0;
	for (key.Verse(1); key.Testament() == currentTestament && key.Book() == currentBook && key.Chapter() == currentChapter && !module->Error(); key.NextVerse()) {
		const QString currentKey = key.key();
		currentVerse = key.Verse();
		m = (d = moduleList->first()) ? d->module() : 0;
		
		rowText = QString("<tr><td bgcolor=\"#f1f1f1\"><b><font color=\"%1\"><a name=\"%2\" href=\"%3\">%4</a></b></td>\n")
			.arg(m_swordRefColorName)
			.arg(currentVerse)
			.arg(CReferenceManager::encodeHyperlink( d->name(), currentKey, CReferenceManager::typeFromModule(d->type()) ))
			.arg(currentVerse);
					
		while (m) {
			CSwordVerseKey current(d);
			current.key(currentKey);
			rowText += QString("<td width=\"%1%\" bgcolor=\"%2\">")
				.arg(width).arg(currentVerse % 2 ? "white" : "#f1f1f1");
			if (d->isUnicode())
				rowText += QString("<font face=\"%1\" size=\"%2\">")
					.arg(m_unicodeFontName).arg(m_unicodeFontSize);
			if (currentVerse == chosenVerse)
				rowText += QString("<font color=\"%1\">")
					.arg(m_highlightedVerseColorName);

			rowText += current.renderedText();

			if (currentVerse == chosenVerse)
				rowText += QString::fromLatin1("</font>");
			if (d->isUnicode())
				rowText += QString::fromLatin1("</font");

			m = (d = moduleList->next()) ? d->module() : 0;
		}
		m_htmlText.append(rowText + QString::fromLatin1("</tr>\n"));
	}
	m_htmlText += QString::fromLatin1("</font></table>");	
	m_htmlText += m_htmlBody + QString::fromLatin1("</qt>");
	
	//clean up
	return 1;		
}
