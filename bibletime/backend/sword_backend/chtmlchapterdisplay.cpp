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
#include "../../frontend/ctoolclass.h"

//Qt includes
#include <qfont.h>

//Sword includes
#include <versekey.h>

/** The constructor */
CHTMLChapterDisplay::CHTMLChapterDisplay(){
	m_useLineBreak = true;	//we use breaks as default, should be changeable at runtime
}

/** Renders the text and puts the result into the member variable m_htmlText */
char CHTMLChapterDisplay::Display( CSwordModuleInfo* module ){
	qDebug("CHTMLChapterDisplay::Display( module )");	
	if (!module) {
		m_htmlText = QString::null;
		return -1; //error
	}
	CSwordVerseKey key(module);
	key.key( module->module()->KeyText() );
	
	const int currentBook = key.Book();
	const int currentChapter = key.Chapter();
	const int currentVerse = key.Verse();	
	int verse = 0;
	m_htmlText = m_htmlHeader + QString::fromLatin1("<BODY>");
	
  QString FontName = m_standardFontName;
  int FontSize = m_standardFontSize;
  if (module->hasFont()){ //use custom font
    QFont font = module->getFont();
    FontName = font.family();
    FontSize = CToolClass::makeLogicFontSize(font.pointSize());
  }
	for (key.Verse(1); key.Book() == currentBook && key.Chapter() == currentChapter && !module->module()->Error(); /*(*swordModule)++*/key.NextVerse()) {
		verse = key.Verse();
		m_htmlText.append( QString::fromLatin1("<A NAME=\"%1\" HREF=\"sword://%2\"><B>%3</B></A>")
			.arg(verse)
			.arg(key.key())
			.arg(verse)
		);
		m_htmlText.append( QString::fromLatin1("<FONT %1 FACE=\"%2\" SIZE=\"%3\"> %4</FONT>")
			.arg((verse == currentVerse) ? QString::fromLatin1("COLOR=\"%1\"").arg(m_highlightedVerseColor) : QString())
			.arg(FontName)
			.arg(FontSize)
			.arg(key.renderedText())
		);
		if (m_useLineBreak)
			m_htmlText.append("<BR>\n");
	}
	m_htmlText.append(m_htmlBody);	

	return 1;	//no error	
}

/** Generates code to display the given modules side by side. */
char CHTMLChapterDisplay::Display( QList<CSwordModuleInfo>* moduleList){	
	qDebug("CHTMLChapterDisplay::Display( QList<CSwordModuleInfo>* moduleList)");
	if (!moduleList || (moduleList && !moduleList->count()) ) {
		m_htmlText = QString::null;
		return 0;
	}

	QMap<CSwordModuleInfo*, QFont> fontMap;	
	SWModule* module = moduleList->first()->module();		
		
	VerseKey* vk = (VerseKey*)(SWKey*)*module;
	CSwordVerseKey key(moduleList->first());
	key.Persist(1);
	key.key((const char*)*vk);

	const int currentBook = key.Book();
	const int currentChapter = key.Chapter();
	const int chosenVerse = key.Verse();	
	const int width=(int)((double)97/(double)moduleList->count());
	CSwordModuleInfo *d = 0;
			
	m_htmlText = m_htmlHeader + QString::fromLatin1("<BODY><TABLE CELLPADDING=\"2\" CELLSPACING=\"0\">");
 	m_htmlText.append(QString::fromLatin1("<TR><TD BGCOLOR=\"#F1F1F1\"></TD>"));
	
	SWModule *m = (d = moduleList->first()) ? d->module() : 0;
	while (m) {
		if (d && d->hasFont())
			fontMap.insert(d, d->getFont());
    if (m)
			m_htmlText.append(
				QString::fromLatin1("<TD width=\"%1\" bgcolor=\"#F1F1F1\"><B>%1</B></TD>").arg((int)((double)100/(double)moduleList->count())).arg(QString::fromLocal8Bit(m->Name()))
			);
		m = (d=moduleList->next()) ? d->module() : 0;			
	}
	m_htmlText.append("</TR>");
		
	QString rowText   = QString::null;
	int currentVerse = 0;
	for (key.Verse(1); key.Book() == currentBook && key.Chapter() == currentChapter && !module->Error(); /*(*module)++*/key.NextVerse() ) {

		currentVerse = key.Verse();
		rowText = QString::fromLatin1("<TR><TD bgcolor=\"#F1F1F1\"><B><A NAME=\"%1\" HREF=\"sword://%2\">%3</A></B></TD>\n")
			.arg(currentVerse)
			.arg(key.key())
			.arg(currentVerse);
		m = (d = moduleList->first()) ? d->module() : 0;
		while (m) {
			m->SetKey(key);
			rowText += QString::fromLatin1("<TD %1 BGCOLOR=\"%2\"><FONT FACE=\"%3\" size=\"%4\" %5>%6</FONT></TD>\n")
				.arg(QString::fromLatin1("width=\"%1%\"").arg(width))
				.arg(currentVerse % 2 ? "white" : "#F1F1F1")
				.arg(fontMap.contains(d) ? fontMap[d].family() : m_standardFontName)
				.arg(fontMap.contains(d) ? CToolClass::makeLogicFontSize(fontMap[d].pointSize()) : m_standardFontSize)
				.arg((currentVerse == chosenVerse) ? QString::fromLatin1("color=\"%1\"").arg(m_highlightedVerseColor) : QString::null)
				.arg(CSwordVerseKey(d).renderedText());
			m = (d = moduleList->next()) ? d->module() : 0;
		}
		m_htmlText.append(rowText + QString::fromLatin1("</TR>\n"));
	}
	m_htmlText.append( QString::fromLatin1("</TABLE>")+m_htmlBody );		
	
	//clean up
	return 0;		
}
