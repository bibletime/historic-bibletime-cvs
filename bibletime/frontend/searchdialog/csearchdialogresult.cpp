/***************************************************************************
                      csearchdialogresult.cpp  -  description
                             -------------------
    begin                : Mit Jan 19 12:57:15 CET 2000
    copyright            : (C) 2000 by The BibleTime Team
    email                : Info@bibletime.de
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
#include "whatsthisdef.h"

#include "backend/cswordmodulesearch.h"
#include "frontend/ctoolclass.h"
#include "frontend/cbtconfig.h"

#include "csearchdialog.h"
#include "csearchdialogtext.h"
#include "csearchdialogresult.h"
#include "csearchdialogresultview.h"

//Qt includes
#include <qpixmap.h>
#include <qregexp.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qheader.h>
#include <qstringlist.h>
#include <qwidget.h>
#include <qsplitter.h>

// KDE includes
#include <klocale.h>
#include <kstringhandler.h>

CSearchDialogResult::CSearchDialogResult(QWidget *parent, const char *name) : QWidget(parent, name) {	
	
	QHBoxLayout* l = new QHBoxLayout(this);
	m_splitter = new QSplitter(Qt::Vertical, this, "result splitter");	
	l->addWidget(m_splitter);
	
	QWidget* d = new QWidget(m_splitter);
	QHBoxLayout* main_layout = new QHBoxLayout(d);	
	QLabel *label1 = new QLabel(d, "label1");
	label1->setText( i18n( "Modules:" ) );
	label1->setAutoResize(true);
	QLabel* label2 = new QLabel(d, "label2");
	label2->setText( i18n("Entries found:") );
	label2->setAutoResize(true);

	resultModuleTree = new CSearchDialogResultModuleView(d, "resultModuleTree");
	resultTree = new CSearchDialogResultView( d, "resultTree");

  m_displayWidget = new KHTMLPart(m_splitter);

	connect(resultModuleTree, SIGNAL(moduleSelected(CSwordModuleInfo*)), resultTree, SLOT(setModule(CSwordModuleInfo*)));
	connect(resultTree, SIGNAL(newPreviewText(const QString)), this, SLOT(updatePreview(const QString)));

	QVBoxLayout* layout_1 = new QVBoxLayout();
	QVBoxLayout* layout_2 = new QVBoxLayout();	
		
	layout_1->addWidget(label1);
	layout_1->addWidget(resultModuleTree);
	
	layout_2->addWidget(label2);
	layout_2->addWidget(resultTree, 5);
	
	main_layout->addLayout(layout_1);
	main_layout->addSpacing(2);
	main_layout->addLayout(layout_2);
}

void CSearchDialogResult::setModuleList(ListCSwordModuleInfo&	modules) {
	moduleList = modules;
	
	resultModuleTree->setModuleList(moduleList);
	resultModuleTree->clear();
	resultModuleTree->setupTree();	
	
	resultTree->clear();
}


/** preview items data in the HTML previewer */
void CSearchDialogResult::updatePreview(const QString newText) {
	qDebug("CSearchDialogResult::updatePreview(const QString newText) ");
	QString text = newText;
	//find text page
	QString searchedText;
	CSearchDialogText* textPart = 0;
	for (QObject* w = parent(); w; w = w->parent()) {
		if (w->isA("CSearchDialog")) {
			CSearchDialog*	dlg = dynamic_cast<CSearchDialog*>(w);
			searchedText = dlg->getSearchedText().stripWhiteSpace();
			textPart = dlg->m_searchText;
			break;
		}		
	}
	
	const QString part1( "<font color=\"red\"><b>");
	const QString part2("</b></font>");
	const int insertLength(part1.length()+part2.length());

 	int searchType(textPart->getSearchType());
  if (searchType & CSwordModuleSearch::exactPhrase) {
		for (int pos = 0 ; pos != -1;){
 			pos = text.find(searchedText,pos,textPart->isCaseSensitive());
 			if (pos!=-1 && !CToolClass::inHTMLTag(pos, text)) {	//something was found
 				text.insert(pos + searchedText.length(),part2);			
 				text.insert(pos, part1);
 				pos += searchedText.length() + insertLength;
 			}
 		}
 	}
 	else if (searchType & CSwordModuleSearch::multipleWords) {
 		QStringList searchedWords;		
 		int wordIndex = 0;
 		QString word = QString::null;
 		while (!(word = KStringHandler::word(searchedText, wordIndex)).isEmpty()) {
 			searchedWords.append(word);
 			++wordIndex;
 		}

 		QString currentWord;
 		for (unsigned int i=0; i < searchedWords.count(); i++) {
 			currentWord = searchedWords[i];
 			int count = text.contains(currentWord, textPart->isCaseSensitive());	
 			int pos = 0;			
 			int i2 = 0;
 			while (i2 < count) {
 				pos = text.find(currentWord,pos?pos+1:0, textPart->isCaseSensitive());
 				if (pos!=-1 && !CToolClass::inHTMLTag(pos, text)) {	//something was found

 					text.insert(pos + currentWord.length(), part2);
 					text.insert(pos, part1);
 						
 					pos += currentWord.length() + insertLength;
 				}
 				++i2;
 			}
 		}
 	}
 	else if (searchType & CSwordModuleSearch::regExp) {
 		QRegExp regExp(searchedText);
 		regExp.setCaseSensitive( textPart->isCaseSensitive() );
 		int matchLength = 0;
		for (int pos = 0; pos != -1;){
 			pos = regExp.search(text, pos);
			matchLength = regExp.matchedLength();
 			if (pos!=-1 && matchLength > 0 && !CToolClass::inHTMLTag(pos, text)) {
 					text.insert(pos + matchLength, part2);
 					text.insert(pos, part1);
 					pos += matchLength + insertLength;
 			}
 		}			
 	}

	//module is Unicode-based
//	if (resultModuleTree->getCurrentModule()->isUnicode() ) {
//		const QFont f = CBTConfig::get( CBTConfig::unicode);
//		text = QString::fromLatin1("<P dir=\"%1\"><FONT FACE=\"%2\" SIZE=\"%3\">%4</FONT></P>")
//      .arg(resultModuleTree->getCurrentModule()->textDirection() == CSwordModuleInfo::RightToLeft ? "rtol" : "ltor")
//      .arg(f.family())
//      .arg(CToolClass::makeLogicFontSize(f.pointSize()))
//      .arg(text);
//	}
	
	m_displayWidget->begin();	
 	m_displayWidget->write(text
//		QString::fromLatin1("<HTML><HEAD></HEAD><BODY><DIV><FONT color=\"red\">%1</font> <SMALL>(%2)</SMALL></DIV><BR>%3</BODY></HTML>")
//		 .arg(resultTree->currentText())
//		 .arg((resultModuleTree->getCurrentModule()) ? resultModuleTree->getCurrentModule()->config(CSwordModuleInfo::Description) : QString::fromLatin1("<I>%1</I>").arg(i18n("module not set")) )
//		 .arg(text)
	);
	m_displayWidget->end();
}

void CSearchDialogResult::clearResult() {
	resultTree->clear();
	resultModuleTree->clear();
	
 	m_displayWidget->begin();
 	m_displayWidget->write("<HTML><HEAD></HEAD><BODY></BODY></HTML>");
 	m_displayWidget->end();
}
