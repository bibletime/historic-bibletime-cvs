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


//Qt includes
#include <qfont.h>

//Sword includes
#include <versekey.h>

CHTMLEntryDisplay::CHTMLEntryDisplay(){
	m_htmlHeader = "<html><head></head>";
	m_htmlBody = "</body></html>";
	m_includeHeader = true;
}

void CHTMLEntryDisplay::updateSettings(void){
	qWarning("CHTMLEntryDisplay::updateSettings(void)");
  m_highlightedVerseColorName = CBTConfig::get(CBTConfig::highlightedVerseColor).name();
	m_standardFontColorName 		= CBTConfig::get(CBTConfig::textColor).name();
	m_swordRefColorName 				= CBTConfig::get(CBTConfig::swordRefColor).name();

  m_standardFontName = CBTConfig::get(CBTConfig::standard).family();
  m_standardFontSize = CToolClass::makeLogicFontSize(CBTConfig::get(CBTConfig::standard).pointSize() );

	m_unicodeFontName  = CBTConfig::get(CBTConfig::unicode).family();
  m_unicodeFontSize  = CToolClass::makeLogicFontSize( CBTConfig::get(CBTConfig::unicode).pointSize() );
}

/** Displays the current entry of the module as HTML */
char CHTMLEntryDisplay::Display(CSwordModuleInfo* module) {
	qWarning("CHTMLEntryDisplay::Display(CSwordModuleInfo* module)");
	if (!module) {
		m_htmlText = QString::null;
		return -1;
	}
  //refresh font settings
	updateSettings();
	
	CSwordKey* key = CSwordKey::createInstance(module);
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

		m_htmlText.append(QString::fromLatin1("<body><font color=\"%1\" face=\"%2\" size=\"%3\">")
			.arg(m_standardFontColorName)
      .arg( module->isUnicode() ? m_unicodeFontName : m_standardFontName)
			.arg( module->isUnicode() ? m_unicodeFontSize : m_standardFontSize));

		m_htmlText.append(QString::fromLatin1("<font color=\"%1\"><a href=\"%2\">%3: <b>%4</b></a></font><hr>%5")
			.arg(m_highlightedVerseColorName)
 			.arg(CReferenceManager::encodeHyperlink(module->name(),key->key(), refType ))
			.arg(module->description())
			.arg(key->key())
			.arg(key->renderedText()));

		m_htmlText += QString::fromLatin1("</font>") + m_htmlBody;
	}
	else {
		m_htmlText = key->renderedText();
	}

//clean up
	delete key;
	return 1;
}

/** Generates code to display the given modules side by side. */
char CHTMLEntryDisplay::Display( QList<CSwordModuleInfo>* moduleList) {
//	qDebug("CHTMLEntryDisplay::Display( QList<CSwordModuleInfo>* moduleList)");
	if (!moduleList || (moduleList && !moduleList->count()) ) {
		m_htmlText = QString::null;
		return 0;
	}
	//reload font settings
	updateSettings();

 	CSwordKey* key = 0;
 	if (moduleList->first()->type() == CSwordModuleInfo::Commentary || moduleList->first()->type() == CSwordModuleInfo::Bible)
		key = new CSwordVerseKey(moduleList->first());
	else if (moduleList->first()->type() == CSwordModuleInfo::Lexicon)
		key = new CSwordLDKey(moduleList->first());
	  	
	SWModule* module = moduleList->first()->module();
	QString usedKey = QString::null;
	CSwordModuleInfo *d = 0;	
	
	SWModule *m= (d = moduleList->first()) ? d->module() : 0;		
	if (moduleList->first()->type() == CSwordModuleInfo::Commentary || moduleList->first()->type() == CSwordModuleInfo::Bible) {
		VerseKey* vk = (VerseKey*)(SWKey*)(*module);
//		vk->Persist(1);
		
		key->key((const char*)*vk);
		usedKey = key->key();
		VerseKey k(*vk);
		
		m = (d = moduleList->first()) ? d->module() : 0;	
		while (m) {
	   	m = (d=moduleList->next()) ? d->module() : 0;
	   	if (m)
				m->SetKey( (const char*)k );
		}	
	}
	else { //lexicon
		SWKey* lk = (SWKey*)(*module);
//		lk->Persist(1);
		key->key((const char*)*lk);
		usedKey = key->key();
				
		m = (d = moduleList->first()) ? d->module() : 0;	
		while (m) {
	   	m = (d=moduleList->next()) ? d->module() : 0;
	   	if (m){
				m->SetKey( (const char*)lk );
				(const char*)*m; //snap to entry
			}
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
	m_htmlText.append(QString::fromLatin1("<TR>"));
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
	m_htmlText += QString::fromLatin1("</font></tr></table>") + m_htmlBody;

	//clean up
	delete key;	
	return 1;
}
