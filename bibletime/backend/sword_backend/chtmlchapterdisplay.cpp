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
	if (!module) {
		m_htmlText = QString::null;
		return 1;
	}
				
	SWModule* swordModule = module->module();
	VerseKey& key = (*(VerseKey*)(SWKey*)*swordModule);
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
	QString verseText = QString::null;	
	QString keyName = QString::null;
	for (key.Verse(1); key.Book() == currentBook && key.Chapter() == currentChapter && !swordModule->Error(); (*swordModule)++) {
		verse = key.Verse();		
		keyName = QString::fromLocal8Bit( (const char*)key );
		verseText = QString::fromLocal8Bit((const char*)*swordModule);		
		if (verse == currentVerse)
			m_htmlText.append( QString("<A HREF=\"sword://%1\">%2</A><A NAME=\"%3\"><FONT COLOR=\"%4\" FACE=\"%5\" SIZE=\"%6\"> %7</FONT></A>")
				.arg( keyName)
				.arg( verse )
				.arg( verse )
				.arg( m_highlightedVerseColor )
				.arg( FontName )
				.arg( FontSize )
				.arg( verseText )
			);
		else
			m_htmlText.append( QString("<A HREF=\"sword://%1\"><B>%2</B></A><A NAME=\"%3\"><FONT FACE=\"%4\" SIZE=\"%5\" > %6</FONT></A>")
				.arg( keyName )
				.arg( verse )
				.arg( verse )
				.arg( FontName )
				.arg( FontSize )				
				.arg( verseText )
			);		
		if (m_useLineBreak)
			m_htmlText.append("<BR>\n");
		else
			m_htmlText.append("\n");
	}
	m_htmlText.append(m_htmlBody);	
		
	//clean up, key is the modules key - don't delete it		
	return 0;	//no error	
}

/** Generates code to display the given modules side by side. */
char CHTMLChapterDisplay::Display( QList<CSwordModuleInfo>* moduleList){	
	ASSERT(moduleList);
	if (!moduleList || (moduleList && !moduleList->count()) ) {
		m_htmlText = QString::null;
		return 0;
	}
	QMap<CSwordModuleInfo*, QFont> fontMap;
	
	SWModule* module = moduleList->first()->module();	
	VerseKey* key = (VerseKey*)(SWKey*)*module;
	key->Persist(1);
	const int currentBook = key->Book();
	const int currentChapter = key->Chapter();
	const int chosenVerse = key->Verse();	
	const int width=(int)((double)97/(double)moduleList->count());
	CSwordModuleInfo *d = 0;
			
	m_htmlText = m_htmlHeader + QString::fromLatin1("<BODY><TABLE CELLPADDING=\"1\" CELLSPACING=\"0\">");
 	m_htmlText.append("<TR><TD BGCOLOR=\"#F1F1F1\"></TD>");
	
	SWModule *m = (d = moduleList->first()) ? d->module() : 0;	
	while (m) {
		if (d && d->hasFont())
			fontMap.insert(d, d->getFont());
    if (m)
			m_htmlText.append(
				QString("<TD width=\"%1\" bgcolor=\"#F1F1F1\"><B>%2</B></TD>")
					.arg(width)
					.arg(QString::fromLocal8Bit(m->Name()))
				);
		m = (d=moduleList->next()) ? d->module() : 0;			
	}
	m_htmlText.append("</TR>");
		
	VerseKey k = (const char*)*key;
	k.Verse(1);
	m = (d = moduleList->first()) ? d->module() : 0;	
	while (m) {
    m = (d=moduleList->next()) ? d->module() : 0;
    if (m)
			m->SetKey( (const char*)k );
	}
	
	QString rowText   = QString::null;
	int currentVerse = 0;
	for (key->Verse(1); key->Book() == currentBook && key->Chapter() == currentChapter && !module->Error(); (*module)++ ) {
		currentVerse = key->Verse();
		rowText = QString("<TR><TD bgcolor=\"#F1F1F1\"><B><A HREF=\"sword://%2\">%3</A></B></TD>\n")
			.arg(QString::fromLocal8Bit((const char*)*key))
			.arg(currentVerse);					
		m = (d = moduleList->first()) ? d->module() : 0;
		while (m) {
			m->SetKey(*key);
			rowText += QString("<TD %1 BGCOLOR=\"%2\"><FONT FACE=\"%3\" size=\"%4\" %5>%6</FONT></TD>\n")
				.arg(/*d!=lastModule ? */QString("width=\"%1%\"").arg(width) /*: QString("")*/ )
				.arg(currentVerse % 2 ? "white" : "#F1F1F1")
				.arg(fontMap.contains(d) ? fontMap[d].family() : m_standardFontName)
				.arg(fontMap.contains(d) ? CToolClass::makeLogicFontSize(fontMap[d].pointSize()) : m_standardFontSize)
				.arg(currentVerse == chosenVerse ? QString("color=\"%1\"").arg(m_highlightedVerseColor) : QString())
				.arg(QString::fromLocal8Bit((const char*)*m));
			m = (d = moduleList->next()) ? d->module() : 0;
		}
		if (!rowText.isEmpty())
			m_htmlText.append(rowText + "</TR>\n");		
	}
	m_htmlText.append( QString("</TABLE>%1").arg(m_htmlBody) );	
	return 0;		
}
