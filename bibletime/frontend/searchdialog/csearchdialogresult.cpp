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
#include "../../backend/sword_backend/cswordmodulesearch.h"
#include "../../structdef.h"
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

// KDE includes
#include <klocale.h>
#include <kstringhandler.h>

CSearchDialogResult::CSearchDialogResult(CImportantClasses* importantClasses, QWidget *parent, const char *name) : QWidget(parent, name) {	
	m_important = importantClasses;
	
	QLabel *label1 = new QLabel(this, "label1");
	label1->setText( i18n( "Modules:" ) );
	label1->setAutoResize(true);

	QLabel* label2 = new QLabel(this, "label2");
	label2->setText( i18n("Entries found:") );
	label2->setAutoResize(true);

	resultModuleTree = new CSearchDialogResultModuleView(m_important, this, "resultModuleTree");
	resultTree = new CSearchDialogResultView(m_important, this, "resultTree");

	QLabel* label3 = new QLabel(this, "label3");
	label3->setText( i18n("Preview:") );
	label3->setAutoResize(true);

	html_widget = new CHTMLWidget(this, "html_widget");
	html_widget->setMinimumHeight(75);
	html_widget->setMaximumHeight(130);

	connect(resultModuleTree, SIGNAL(moduleSelected(CModuleInfo*)), resultTree, SLOT(setModule(CModuleInfo*)));
	connect(resultTree, SIGNAL(keySelected(QString)), this, SLOT(updatePreview(QString)));

	QVBoxLayout* layout_1 = new QVBoxLayout( this );
	QVBoxLayout* layout_1_1 = new QVBoxLayout();
	QVBoxLayout* layout_1_2 = new QVBoxLayout();
	QHBoxLayout* layout_1_3 = new QHBoxLayout();
		
	layout_1_1->addWidget(label1);
	layout_1_1->addWidget(resultModuleTree);
	
	layout_1_2->addWidget(label2);
	layout_1_2->addWidget(resultTree, 5);
	layout_1_2->setResizeMode( QLayout::Minimum );
	
	layout_1_3->addLayout(layout_1_1);
	layout_1_3->addSpacing(2);
	layout_1_3->addLayout(layout_1_2);

	layout_1->addLayout(layout_1_3);
	layout_1->addWidget( label3, 0 );
	layout_1->addWidget( html_widget, 2 );

}

void CSearchDialogResult::setModuleList(ListCSwordModuleInfo*	modules) {
	ASSERT(modules);
	moduleList = modules;
	
	resultModuleTree->setModuleList(moduleList);
	resultModuleTree->clear();
	resultModuleTree->setupTree();	
	resultModuleTree->setMinimumSize( resultModuleTree->sizeHint() );	
	
	resultTree->setMinimumWidth( resultTree->sizeHint().width());
	resultTree->clear();
}


/** preview items data in the HTML previewer */
void CSearchDialogResult::updatePreview(QString text) {
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
				qDebug( (const char*)QString("Matched: %1").arg(pos).local8Bit());
				if (pos!=-1 && matchLength > 0) {
						qDebug((const char*)QString("match at %1 with length %2").arg(pos).arg(matchLength).local8Bit());
						text.insert(pos + matchLength, "</B></FONT>");
						text.insert(pos, "<FONT color=\"red\"><B>");
						pos+=matchLength+QString("</B></FONT>").length()+QString("<FONT color=\"red\"><B>").length();
				}
			}			
		}
	}
	html_widget->setText( "<HTML><HEAD></HEAD><BODY>" +
		QString(i18n("<FONT color=\"red\">%1 </FONT><SMALL>(%2)</SMALL><BR><HR>%3"))
		.arg( resultTree->currentText() )
		.arg( (resultModuleTree->getCurrentModule()) ? resultModuleTree->getCurrentModule()->getDescription() : i18n("<I>module not set</I>") )
		.arg(text)
		+ "</BODY></HTML>" );
}

void CSearchDialogResult::clearResult() {
	resultTree->clear();
	resultModuleTree->clear();
	html_widget->setText("<HTML><HEAD></HEAD><BODY></BODY></HTML>");
}
