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
#include "../../frontend/ctoolclass.h"

//Qt includes
#include <qfont.h>

//Sword includes
#include <versekey.h>

CHTMLEntryDisplay::CHTMLEntryDisplay(){
	m_highlightedVerseColor = "red";
	m_textColor = "black";
	m_linkColor = "darkBlue";
	m_htmlHeader = "<HTML><HEAD><TITLE></TITLE></HEAD>";
	m_htmlBody = "</BODY></HTML>";
	m_standardFontName = QFont::defaultFont().family();
	m_standardFontSize = 4; //we use logical font sizes between 1 and 7
	m_includeHeader = true;
}

CHTMLEntryDisplay::~CHTMLEntryDisplay(){
	qDebug("CHTMLEntryDisplay::~CHTMLEntryDisplay()");	
}

/** Displays the current entry of the module as HTML */
char CHTMLEntryDisplay::Display(CSwordModuleInfo* module) {
  QString FontName = m_standardFontName;
  int FontSize = m_standardFontSize;
  if (module->hasFont()){ //use custom font
    QFont font = module->getFont();
    FontName = font.family();
    FontSize = CToolClass::makeLogicFontSize(font.pointSize());
  }
	if (m_includeHeader) {
		m_htmlText =
			m_htmlHeader +
				QString("<FONT color=\"%1\">\
					<A HREF=\"sword://%2\">%3: <B>%4</B></A></FONT>\
					<HR><FONT color=\"%5\" face=\"%6\" size=\"%7\">%8</FONT")
				.arg(m_highlightedVerseColor)
				.arg(QString::fromLocal8Bit(module->module()->KeyText()))
				.arg(QString::fromLocal8Bit(module->module()->Description()))	
				.arg(QString::fromLocal8Bit(module->module()->KeyText()))
				.arg(m_textColor)			
				.arg(FontName)	
				.arg(FontSize)	
				.arg(QString::fromLocal8Bit((const char*)*(module->module())))
			+ m_htmlBody;	
	}
	else {
		m_htmlText = QString::fromLocal8Bit((const char*)*(module->module()) );
	}
	//don't delete the key because it's the module's one!	
	return 0;
}

/** Returns the generated HTML text. */
QString CHTMLEntryDisplay::getHTML() const {
	return m_htmlText;
}

/** Sets the standard font fot this display object. */
void CHTMLEntryDisplay::setStandardFont( QString family, short int logicalSize ) {
	m_standardFontName = family;
	m_standardFontSize = logicalSize;
}

/** Call this with false as argument to switch off the headers (module anme and current position) */
void CHTMLEntryDisplay::setIncludeHeader( const bool includeHeader ){
	m_includeHeader = includeHeader;
}

/** Generates code to display the given modules side by side. */
char CHTMLEntryDisplay::Display( QList<CSwordModuleInfo>* moduleList) {
	ASSERT(moduleList);
	if (!moduleList || (moduleList && !moduleList->count()) ) {
		m_htmlText = QString::null;
		return 0;
	}		
	SWModule* module = moduleList->first()->module();	
	VerseKey* key = (VerseKey*)(SWKey*)*module;
	key->Persist(1);

	const int width=(int)((double)100/(double)moduleList->count());
	CSwordModuleInfo *d = 0;
			
	m_htmlText = m_htmlHeader + QString::fromLocal8Bit("<TABLE cellpadding=\"1\" cellspacing=\"0\">");
 	
 	m_htmlText.append("<TR>");
	SWModule *m= (d = moduleList->first()) ? d->module() : 0;	
	while (m) {
    if (m)
			m_htmlText.append(QString("<TD width=\"%1\" bgcolor=\"#F1F1F1\"><B>%2</B></TD>")
				.arg(width)
				.arg(QString::fromLocal8Bit(m->Name())));
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
	
	m = (d = moduleList->first()) ? d->module() : 0;
	m_htmlText.append("<TR>");
	while (m) {
		m_htmlText.append(
			QString("<TD width=\"%1%\">%2</TD>")
				.arg( width )
				.arg( QString::fromLocal8Bit((const char*)*m))
			);
		m = (d = moduleList->next()) ? d->module() : 0;		
	}
	m_htmlText.append( QString("</TR></TABLE>%1").arg(m_htmlBody) );	

//	ASSERT(module);
//	if (m_includeHeader) {
//		m_htmlText =
//			m_htmlHeader +
//				QString("<FONT color=\"%1\">\
//					<A HREF=\"sword://%2\">%3: <B>%4</B></A></FONT>\
//					<HR><FONT color=\"%5\" face=\"%6\" size=\"%7\">%8</FONT")
//				.arg(m_highlightedVerseColor)
//				.arg(QString::fromLocal8Bit(module->module()->KeyText()))
//				.arg(QString::fromLocal8Bit(module->module()->Description()))	
//				.arg(QString::fromLocal8Bit(module->module()->KeyText()))
//				.arg(m_textColor)			
//				.arg(m_standardFontName)	
//				.arg(m_standardFontSize)	
//				.arg(QString::fromLocal8Bit((const char*)*(module->module())))
//			+ m_htmlBody;	
//	}
//	else {
//		m_htmlText = QString::fromLocal8Bit((const char*)*(module->module()) );
//	}
	//don't delete the key because it's the module's one!	
	return 0;
}
