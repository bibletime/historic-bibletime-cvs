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
	qDebug("CHTMLChapterDisplay::Display( SWModule& module )");
	ASSERT(module);
	
	if (!module) {
		qDebug("module is NULL!");
		m_htmlText = QString::null;
		return 1;
	}
				
	SWModule* swordModule = module->module();
	VerseKey& key = (*(VerseKey*)(SWKey*)*swordModule);
	const int currentBook = key.Book();
	const int currentChapter = key.Chapter();
	const int currentVerse = key.Verse();	
	int verse = 0;
	m_htmlText = m_htmlHeader;
	
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
		qDebug("Display loop");
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
			m_htmlText.append( QString("<A HREF=\"sword://%1\"><B>%2</B></A><A NAME=\"%3\"><FONT FACE=\"%4\" SIZE=\"%5\"> %6</FONT></A>")
				.arg( keyName )
				.arg( verse )
				.arg( verse )
				.arg( FontName )
				.arg( FontSize )				
				.arg( verseText )
			);		
//		if (m_useLineBreak)
//			m_htmlText.append("<BR>\n");
//		else
			m_htmlText.append("<BR>\n");
	}
	m_htmlText.append(m_htmlBody);	
		
	//clean up, key is the modules key - don't delete it		
	return 0;	//no error	
}

/** Generates code to display the given modules side by side. */
char CHTMLChapterDisplay::Display( QList<CSwordModuleInfo>* moduleList){	
	qDebug("CHTMLChapterDisplay::Display( QList<CSwordModuleInfo>* moduleList)");
	ASSERT(moduleList);
	if (!moduleList || (moduleList && !moduleList->count()) ) {
		qWarning("empty module list");
		m_htmlText = QString::null;
		return 0;
	}		
	SWModule* module = moduleList->first()->module();	
	VerseKey* key = (VerseKey*)(SWKey*)*module;
	key->Persist(1);

	const int currentBook = key->Book();
	const int currentChapter = key->Chapter();
	const int width=(int)((double)97/(double)moduleList->count());
	CSwordModuleInfo *d = 0;
			
	m_htmlText = m_htmlHeader + QString::fromLocal8Bit("<TABLE cellpadding=\"1\" cellspacing=\"0\">");	
 	m_htmlText.append("<TR><TD BGCOLOR=\"#F1F1F1\"></TD>");
	
	SWModule *m= (d = moduleList->first()) ? d->module() : 0;	
	while (m) {
    if (m)
			m_htmlText.append(
				QString("<TD %1 bgcolor=\"#F1F1F1\"><B>%2</B></TD>")
					.arg(/*d!=moduleList->getLast() ?*/ QString("width=\"%1%\"").arg(width) /*: QString()*/)
					.arg(QString::fromLocal8Bit(m->Name()))
				);
		m = (d=moduleList->next()) ? d->module() : 0;			
	}
	m_htmlText.append("</TR>");
		
	VerseKey k = (const char*)*key;
	k.Verse(1);

	m= (d = moduleList->first()) ? d->module() : 0;	
	while (m) {
    m = (d=moduleList->next()) ? d->module() : 0;
    if (m)
			m->SetKey( (const char*)k );
	}
	
	QString rowText   = QString::null;
	QString keyName   = QString::null;
	QString verseText = QString::null;
	for (key->Verse(1); key->Book() == currentBook && key->Chapter() == currentChapter && !module->Error(); (*module)++ ) {	
		keyName = QString::fromLocal8Bit((const char*)*key);
		rowText = QString("<TR><TD bgcolor=\"#F1F1F1\"><B><A HREF=\"sword://%2\">%3</A></B></TD>")
			.arg(keyName)
			.arg(key->Verse());
		
		m = (d = moduleList->first()) ? d->module() : 0;
		while (m) {
			m->SetKey(*key);
			verseText = QString::fromLocal8Bit((const char*)*m);
			if (d->hasFont())
				rowText += QString("<TD %1 BGCOLOR=\"%2\"><FONT FACE=\"%3\" size=\"%4\">%5</FONT></TD>")
					.arg( d!=moduleList->getLast() ? QString("width=\"%1%\"").arg(width) : QString("") )
					.arg(key->Verse() % 2 ? "white" : "#F1F1F1")
					.arg(d->getFont().family())
					.arg(CToolClass::makeLogicFontSize(d->getFont().pointSize()))
					.arg(verseText);
			else
				rowText += QString("<TD %1 BGCOLOR=\"%2\">%3</TD>")
					.arg( (d!=moduleList->getLast()) ? QString("width=\"%1%\"").arg(width) : QString("") )
					.arg(key->Verse() % 2 ? "white" : "#F1F1F1")
					.arg(verseText);			
			m = (d = moduleList->next()) ? d->module() : 0;
		}
		if (!rowText.isEmpty())
			m_htmlText.append(rowText + "</TR>\n");		
	}
	m_htmlText.append( QString("</TABLE>%1").arg(m_htmlBody) );
	
	return 0;		
}
