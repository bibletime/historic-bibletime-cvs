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


//BIbleTime includes
#include "../../backend/cswordmodulesearch.h"
#include "../../structdef.h"
#include "../../whatsthisdef.h"
#include "csearchdialog.h"
#include "csearchdialogtext.h"
#include "csearchdialogresult.h"
#include "csearchdialogresultview.h"
#include "../ctoolclass.h"
#include "../cbtconfig.h"

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

CSearchDialogResult::CSearchDialogResult(CImportantClasses* importantClasses, QWidget *parent, const char *name) : QWidget(parent, name) {	
	m_important = importantClasses;
		
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

	resultModuleTree = new CSearchDialogResultModuleView(m_important, d, "resultModuleTree");
	resultTree = new CSearchDialogResultView(m_important, d, "resultTree");

	html_widget = new CHTMLWidget(m_important, true, m_splitter, "html_widget");
	html_widget->setMinimumHeight(80);

	connect(resultModuleTree, SIGNAL(moduleSelected(CSwordModuleInfo*)), resultTree, SLOT(setModule(CSwordModuleInfo*)));
	connect(resultTree, SIGNAL(keySelected(const QString)), this, SLOT(updatePreview(const QString)));

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

void CSearchDialogResult::setModuleList(ListCSwordModuleInfo*	modules) {
	moduleList = modules;
	
	resultModuleTree->setModuleList(moduleList);
	resultModuleTree->clear();
	resultModuleTree->setupTree();	
	
	resultTree->clear();
}


/** preview items data in the HTML previewer */
void CSearchDialogResult::updatePreview(const QString newText) {
	QString text = newText;
	//find text page
	QString searchedText;
	CSearchDialogText* textPart = 0;
	for (QObject* w = parent(); w; w = w->parent()) {
		if (w->isA("CSearchDialog")) {
			CSearchDialog*	dlg = (CSearchDialog*)w;
			textPart = dlg->searchText;
			searchedText = textPart->getText().stripWhiteSpace();
			break;
		}		
	}
	
	if (textPart) {
		int searchType = textPart->getSearchType();
		if (searchType & CSwordModuleSearch::exactPhrase) {
			const int count = text.contains(searchedText, textPart->isCaseSensitive());
			int i = 0;
			int pos = 0;
			while (i < count) {
				pos = text.find(searchedText,pos,textPart->isCaseSensitive());
				if (pos!=-1) {	//something was found
					text.insert(pos + searchedText.length(),"</B></FONT>");			
					text.insert(pos, "<B><FONT color=\"red\">");
				}
				++i;
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
					if (pos!=-1) {	//something was found
						text.insert(pos + currentWord.length(), "</B></FONT>");
						
						const QString part = "<FONT color=\"red\"><B>";
						text.insert(pos, part);
						
						pos += currentWord.length() + part.length();
					}
					++i2;
				}
			}
		}
		else if (searchType & CSwordModuleSearch::regExp) {
			QRegExp regExp(searchedText);
			regExp.setCaseSensitive( textPart->isCaseSensitive() );
			int matchLength = 0;
			int pos = 0;
			while ( pos != -1 ) {
				pos = regExp.match(text,pos,&matchLength);
				if (pos!=-1 && matchLength > 0) {
						text.insert(pos + matchLength, "</B></FONT>");
						text.insert(pos, "<FONT color=\"red\"><B>");
						pos += matchLength + QString::fromLatin1("</B></FONT>").length() + QString::fromLatin1("<FONT color=\"red\"><B>").length();
				}
			}			
		}
	}
	//module is Unicode-based
	if (resultModuleTree->getCurrentModule()->isUnicode() ) {
		QFont f = CBTConfig::get( CBTConfig::unicode);
		text = QString::fromLatin1("<FONT FACE=\"%1\" SIZE=\"%2\">%3</FONT>").arg(f.family()).arg(f.pointSize()).arg(text);
	}
	html_widget->setText(
		QString::fromLatin1("<HTML><HEAD></HEAD><BODY><FONT color=\"red\">%1 </FONT><SMALL>(%2)</SMALL><BR><HR>%3</BODY></HTML>")
		 .arg(resultTree->currentText())
		 .arg((resultModuleTree->getCurrentModule()) ? resultModuleTree->getCurrentModule()->getDescription() : QString::fromLatin1("<I>%1</I>").arg(i18n("<I>module not set</I>")) )
		 .arg(text)
	);
}

void CSearchDialogResult::clearResult() {
	resultTree->clear();
	resultModuleTree->clear();
	html_widget->setText("<HTML><HEAD></HEAD><BODY></BODY></HTML>");
}
