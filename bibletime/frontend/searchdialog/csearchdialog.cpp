/***************************************************************************
                       csearchdialog.cpp  -  description
                             -------------------
    begin                : Wed Jan 19 2000
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

#include "csearchdialog.h"
#include "csearchdialogmodulechooser.h"
#include "csearchdialogtext.h"
#include "csearchdialogresult.h"
#include "csearchdialoganalysis.h"
#include "csearchdialogscope.h"
#include "../chtmldialog.h"
#include "../../resource.h"
#include "../../backend/cswordmodulesearch.h"
#include "../../backend/cswordversekey.h"
#include "../../backend/cswordmoduleinfo.h"
#include "../ctoolclass.h"
#include "../cbtconfig.h"

//KDE includes
#include <kapp.h>
#include <klocale.h>
#include <kkeydialog.h>
#include <kfontdialog.h>
#include <kfiledialog.h>
#include <kapp.h>

//QT includes
#include <qlabel.h>
#include <qframe.h>
#include <qbutton.h>
#include <qdict.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qgrid.h>
#include <qhbuttongroup.h>
#include <qpushbutton.h>
#include <qcanvas.h>

CSearchDialog::CSearchDialog( ListCSwordModuleInfo& modules, QWidget *parent, const char *name )
	: KDialogBase(Tabbed, i18n("Search Dialog"), Close | User1 | User2, User1, parent, name,	false, true, i18n("Search"), i18n("Interrupt"), QString::null)
{
	setIcon(MODULE_SEARCH_ICON_SMALL);
	m_searcher.connectPercentUpdate(this, SLOT(percentUpdate()));
	m_searcher.connectFinished(this, SLOT(searchFinished()));
		
	initView();	
	readSettings();	
		
	setModuleList( modules );
	if (modules.count())
		showPage(pageIndex(m_searchTextPage));		
}

CSearchDialog::~CSearchDialog(){
	saveSettings();	
}

/** Reads the settings from the configfile */
void CSearchDialog::readSettings(){
}

/** Saves the settings to the config file */
void CSearchDialog::saveSettings(){
}

/** Initializs the view */
void CSearchDialog::initView() {
 	enableButton(User2,false);

	m_moduleChooserPage 	= addVBoxPage(i18n("Choose modules"), i18n("Choose the modules for the search"));
	m_moduleChooser			= new CSearchDialogModuleChooser(m_moduleChooserPage);
	connect(m_moduleChooser, SIGNAL(chosenModulesChanged()), SLOT(chosenModulesChanged()));
 	
	m_searchTextPage = addVBoxPage(i18n("Search Text"), i18n("Enter the text to search for"));
	m_searchText			= new CSearchDialogText(m_searchTextPage);
	m_searchTextPage->setEnabled(false);
	
	m_searchResultPage = addHBoxPage(i18n("Search Result"), i18n("The result of your search"));
	m_searchResult = new CSearchDialogResult(m_searchResultPage);
	m_searchResultPage->setEnabled(false);
	
	m_searchAnalysisPage = addVBoxPage(i18n("Search Analysis"), i18n("Graphical analysis of your search result"));	
	m_searchAnalysis = new CSearchDialogAnalysis(m_searchAnalysisPage);
	m_searchAnalysisView =	new CSearchDialogAnalysisView(m_searchAnalysis, m_searchAnalysisPage);
	m_searchAnalysisPage->setEnabled(false);

	m_searchAnalysisSaveButton = new QPushButton("Save Analysis to Disk", m_searchAnalysisPage);

	connect(m_searchAnalysisSaveButton, SIGNAL(clicked()),
		this, SLOT(slotSaveSearchAnalysis()));	
}

ListCSwordModuleInfo& CSearchDialog::getModuleList() {
//	ASSERT(moduleList);
	m_moduleList = m_moduleChooser->getChosenModules();
	return m_moduleList;
}

void CSearchDialog::setModuleList(ListCSwordModuleInfo& list) {
	if (m_moduleList != list)
		m_moduleList = list; //copy the items of "list"
	
	m_moduleChooser->blockSignals(true);
	m_moduleChooser->setChosenModules(m_moduleList);
	m_moduleChooser->blockSignals(false);	
	
	m_searchTextPage->setEnabled(m_moduleList.count()>0);	
	m_searchResult->clearResult();
	m_searchAnalysis->reset();
}

void CSearchDialog::slotSaveSearchAnalysis(){
 	const QString file = KFileDialog::getSaveFileName(QString::null, QString::fromLatin1("*.html | %1").arg(i18n("HTML files")), 0, i18n("Save Search Analysis"));
	if (!file.isNull()) {
		int moduleIndex = 0;
		int count = 0;
		QString countStr = "";
		QString m_searchAnalysisHTML = "";
		QString tableTitle = "";
	  QString tableTotals = "";
		QString VerseRange = "";
		const QString txtCSS = QString::fromLatin1("<style type='text/css'>\nTD {border: thin solid black;}\nTH {font-size: 130%;text-align: left;vertical-align:top;}\n</style>\n");	
		CSwordVerseKey key(0);
		ListKey searchResult;
		
		key.key("Genesis 1:1");
	
		if (m_searchText->scopeChooser->getScopeType() != CSwordModuleSearch::Scope_NoScope) {
			ListKey verses = m_searcher.scope();
			for (int i = 0; i < verses.Count(); ++i) {
				VerseKey* element = dynamic_cast<VerseKey*>(verses.GetElement(i));
				if (element) {
					VerseRange += QString("%1 - %2").arg(QString::fromLocal8Bit((const char*)element->LowerBound())).arg(QString::fromLocal8Bit((const char*)element->UpperBound())) + "<br>";
				}
			}
		}

		QDict<CSearchDialogAnalysisItem>* m_searchAnalysisItems = m_searchAnalysis->getSearchAnalysisItemList();
		CSearchDialogAnalysisItem* analysisItem = m_searchAnalysisItems->find( key.book() );
		
    QString text = "<html>\n<head>\n<title>" + i18n("BibleTime Search Analysis") + "</title>\n" + txtCSS + "</head>\n<body>\n";
		text += "<table>\n<tr><th>" + i18n("Search Text :") + "</th><th>" + m_searchText->getText() + "</th></tr>\n";
		text += "<tr><th>" + i18n("Search Type :") + "</th><th>" + m_searchText->getSearchTypeString() + "</th></tr>\n";
		text += "<tr><th>" + i18n("Search Scope:") + "</th><th>" + ((m_searchText->scopeChooser->getScopeType() != CSwordModuleSearch::Scope_NoScope) ? VerseRange : m_searchText->scopeChooser->getScopeTypeString()) + "</th></tr>\n</table>\n<br>\n";


	  tableTitle = "<tr><th align=\"left\">" + i18n("Book") + "</th>";
		tableTotals = "<tr><td align=\"left\">" + i18n("Total Hits") + "</td>";
		for (moduleIndex = 0,m_moduleList.first(); m_moduleList.current(); m_moduleList.next(),++moduleIndex) {
				tableTitle += QString::fromLatin1("<th align=\"left\">") + m_moduleList.current()->name() + QString::fromLatin1("</th>");
				searchResult = m_moduleList.current()->searchResult();
				countStr.setNum(searchResult.Count());
	      tableTotals += QString::fromLatin1("<td align=\"right\">") + countStr + QString::fromLatin1("</td>");
		}
		tableTitle += QString::fromLatin1("</tr>\n");
		tableTotals += QString::fromLatin1("</tr>\n");

		m_searchAnalysisHTML = "";
		bool ok = true;		
		while (ok) {
			m_searchAnalysisHTML += QString::fromLatin1("<tr><td>") + key.book() + QString::fromLatin1("</td>");
			analysisItem = m_searchAnalysisItems->find( key.book() );
			for (moduleIndex = 0,m_moduleList.first(); m_moduleList.current(); m_moduleList.next(), ++moduleIndex) {
				count = analysisItem->getCountForModule(moduleIndex);
				countStr.setNum(count);
				m_searchAnalysisHTML += QString::fromLatin1("<td align=\"right\">") + countStr + QString::fromLatin1("</td>");
			}
			m_searchAnalysisHTML += QString::fromLatin1("</tr>\n");
			ok = key.next(CSwordVerseKey::UseBook);
		}
		text += QString::fromLatin1("<table>\n") + tableTitle + tableTotals + m_searchAnalysisHTML + QString::fromLatin1("</table>\n");
		text += QString::fromLatin1("<center>") + i18n("Created by") + QString::fromLatin1(" <a href=\"http://www.bibletime.de/\">BibleTime</a></center>");
		text += QString::fromLatin1("</body></html>");
		CToolClass::savePlainFile(file, text);
	}
}

void CSearchDialog::slotUser1() {
	startSearch();
}

void CSearchDialog::slotUser2() {
//	if (searcher->isSearching())
	m_searcher.interruptSearch();
}

void CSearchDialog::startSearch(void) {
	int searchFlags = m_searchText->getSearchType();	
	// set the parameters
	m_searchText->updateCurrentProgress(0);
	m_searchText->updateOverallProgress(0);
	m_searchText->setText( m_searchText->getText() );
		
	m_searcher.setModules( getModuleList() );
	m_searcher.setSearchedText(m_searchText->getText());

	if (m_searchText->isCaseSensitive())
		searchFlags |= CSwordModuleSearch::caseSensitive;
	m_searcher.resetSearchScope();
	CSwordModuleSearch::scopeType scopeType = m_searchText->scopeChooser->getScopeType();
	
	if (scopeType == CSwordModuleSearch::Scope_LastSearch) {
		searchFlags |= CSwordModuleSearch::useLastResult;
	}
	else if ( scopeType == CSwordModuleSearch::Scope_Bounds ) {
		searchFlags |= CSwordModuleSearch::useScope;	
		m_searcher.setSearchScope( m_searchText->scopeChooser->getScope() );
	}
	m_searcher.setSearchOptions(searchFlags);
	enableButton(User1,false);
	enableButton(User2,true);
 	m_searchAnalysis->reset();
 	m_searchResult->clearResult();
 		
	m_searcher.startSearchThread();
}

void CSearchDialog::setSearchText(const QString text){
  m_searchText->setText(text);
	if (!text.isEmpty())
		showPage(pageIndex(m_searchTextPage));
}

/** Returns the search text. If no text was entered return QString::null. */
const QString CSearchDialog::getSearchedText() const {
	qWarning("searchedText: %s", m_searchText->getText().latin1());
	return m_searchText->getText();
}

/** No descriptions */
void CSearchDialog::chosenModulesChanged(){
	setModuleList(getModuleList());
}

/** Reimplementation to show the "First time opened" dialog. */
void CSearchDialog::show(){
	KDialogBase::show();

	if ( CBTConfig::get(CBTConfig::firstSearchDialog) ){
		HTML_DIALOG(HELPDIALOG_FIRSTTIME_SEARCH)
		CBTConfig::set(CBTConfig::firstSearchDialog, false);
	}			
}

/** No descriptions */
void CSearchDialog::searchFinished(){
//	qWarning("CSearchDialog::searchFinished()");
 	m_searchText->updateCurrentProgress(100);
 	m_searchText->updateOverallProgress(100);
 	m_searchAnalysis->reset();
 	if ( m_searcher.foundItems() ){
 		m_searchResult->setModuleList(getModuleList());			
 		m_searchAnalysis->setModuleList(getModuleList());
 		m_searchAnalysisView->setContentsPos(0,0);
 		m_searchResultPage->setEnabled(true);
 		m_searchAnalysisPage->setEnabled(true);						
 		showPage(pageIndex(m_searchResultPage));	//the result page
						
 		m_searchAnalysis->analyse();			
 	}
 	else
 		m_searchResult->clearResult();

 	enableButton(User2,false);
 	enableButton(User1,true); 	
}

/** No descriptions */
void CSearchDialog::percentUpdate(){
//	old_overallProgress = m_searcher.getPercent(CSwordModuleSearch::currentModule);
	m_searchText->updateOverallProgress(m_searcher.getPercent(CSwordModuleSearch::allModules));
	
//	old_currentProgress = m_searcher.getPercent(CSwordModuleSearch::currentModule);
	m_searchText->updateCurrentProgress(m_searcher.getPercent(CSwordModuleSearch::currentModule));
	KApplication::kapp->processEvents();
}
