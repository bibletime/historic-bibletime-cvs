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
#include "../../structdef.h"
#include "../../ressource.h"
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

CSearchDialog::CSearchDialog( ListCSwordModuleInfo* modules, QWidget *parent, const char *name )
	: KDialogBase(Tabbed, i18n("Search Dialog"), Close | User1 | User2, User1, parent, name,	false, true, i18n("Search"), i18n("Interrupt"), QString::null),
	searcher(new CSwordModuleSearch()),
	moduleList(0), old_currentProgress(0), old_overallProgress(0)			
{
	setIcon(MODULE_SEARCH_ICON_SMALL);
	
	initView();	
	readSettings();	
		
	if (modules && modules->count())
		showPage(pageIndex(searchText_page));	
	setModuleList( modules );
}

CSearchDialog::~CSearchDialog(){
	saveSettings();	
	if (searcher)
		delete searcher;
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

	moduleChooser_page 	= addVBoxPage(i18n("Choose modules"), i18n("Choose the modules for the search"));
	m_moduleChooser			= new CSearchDialogModuleChooser(moduleChooser_page);
	connect(m_moduleChooser, SIGNAL(chosenModulesChanged()), SLOT(chosenModulesChanged()));
 	
	searchText_page = addVBoxPage(i18n("Search Text"), i18n("Enter the text to search for"));
	searchText			= new CSearchDialogText(searchText_page);
	searchText_page->setEnabled(false);
	
	searchResult_page = addHBoxPage(i18n("Search Result"), i18n("The result of your search"));
	searchResult = new CSearchDialogResult(searchResult_page);
	searchResult_page->setEnabled(false);
	
	searchAnalysis_page = addVBoxPage(i18n("Search Analysis"), i18n("Graphical analysis of your search result"));	
	searchAnalysis = new CSearchDialogAnalysis(searchAnalysis_page);
	searchAnalysisView =	new CSearchDialogAnalysisView(searchAnalysis, searchAnalysis_page);
	searchAnalysis_page->setEnabled(false);

	m_searchAnalysisSaveButton = new QPushButton("Save Analysis to Disk", searchAnalysis_page);

	connect(m_searchAnalysisSaveButton, SIGNAL(clicked()), this, SLOT(slotSaveSearchAnalysis()));	
}

ListCSwordModuleInfo* CSearchDialog::getModuleList() {
	*moduleList = m_moduleChooser->getChosenModules();
	return moduleList;
}

void CSearchDialog::setModuleList(ListCSwordModuleInfo *list) {
	if (!list)
		return;	
	if (!moduleList)
		moduleList = new ListCSwordModuleInfo;
		
	if (moduleList != list)
		*moduleList = *list; //copy the items of "list"
	
	m_moduleChooser->blockSignals(true);
	m_moduleChooser->setChosenModules(moduleList);
	m_moduleChooser->blockSignals(false);	
	
	searchText_page->setEnabled(moduleList->count());	
	searchResult->clearResult();
	searchAnalysis->reset();
}

void CSearchDialog::slotSaveSearchAnalysis(){
 	const QString file = KFileDialog::getSaveFileName(QString::null, QString::fromLatin1("*.html | %1").arg(i18n("HTML files")), 0, i18n("Save Search Analysis"));
	if (!file.isNull()) {
		int moduleIndex = 0;
		int count = 0;
		QString countStr = "";
		QString searchAnalysisHTML = "";
		QString tableTitle = "";
	  QString tableTotals = "";
		QString VerseRange = "";
		const QString txtCSS = QString::fromLatin1("<style type='text/css'>\nTD {border: thin solid black;}\nTH {font-size: 130%;text-align: left;vertical-align:top;}\n</style>\n");	
		CSwordVerseKey key(0);
		ListKey m_searchResult;
		
		key.key("Genesis 1:1");
	
		if (searchText->scopeChooser->getScopeType() != CSwordModuleSearch::Scope_NoScope) {
			ListKey verses = searcher->scope();
			for (int i = 0; i < verses.Count(); ++i) {
				VerseKey* element = dynamic_cast<VerseKey*>(verses.GetElement(i));
				if (element) {
					VerseRange += QString("%1 - %2").arg(QString::fromLocal8Bit((const char*)element->LowerBound())).arg(QString::fromLocal8Bit((const char*)element->UpperBound())) + "<br>";
				}
			}
		}

		QDict<CSearchDialogAnalysisItem>* searchAnalysisItems = searchAnalysis->getSearchAnalysisItemList();
		CSearchDialogAnalysisItem* analysisItem = searchAnalysisItems->find( key.book() );
		
    QString text = "<html>\n<head>\n<title>" + i18n("BibleTime Search Analysis") + "</title>\n" + txtCSS + "</head>\n<body>\n";
		text += "<table>\n<tr><th>" + i18n("Search Text :") + "</th><th>" + searchText->getText() + "</th></tr>\n";
		text += "<tr><th>" + i18n("Search Type :") + "</th><th>" + searchText->getSearchTypeString() + "</th></tr>\n";
		text += "<tr><th>" + i18n("Search Scope:") + "</th><th>" + ((searchText->scopeChooser->getScopeType() != CSwordModuleSearch::Scope_NoScope) ? VerseRange : searchText->scopeChooser->getScopeTypeString()) + "</th></tr>\n</table>\n<br>\n";


	  tableTitle = "<tr><th align=\"left\">" + i18n("Book") + "</th>";
		tableTotals = "<tr><td align=\"left\">" + i18n("Total Hits") + "</td>";
		for (moduleIndex = 0,moduleList->first(); moduleList->current(); moduleList->next(),++moduleIndex) {
				tableTitle += QString::fromLatin1("<th align=\"left\">") + moduleList->current()->name() + QString::fromLatin1("</th>");
				m_searchResult = moduleList->current()->getSearchResult();
				countStr.setNum(m_searchResult.Count());
	      tableTotals += QString::fromLatin1("<td align=\"right\">") + countStr + QString::fromLatin1("</td>");
		}
		tableTitle += QString::fromLatin1("</tr>\n");
		tableTotals += QString::fromLatin1("</tr>\n");

		searchAnalysisHTML = "";
		bool ok = true;		
		while (ok) {
			searchAnalysisHTML += QString::fromLatin1("<tr><td>") + key.book() + QString::fromLatin1("</td>");
			analysisItem = searchAnalysisItems->find( key.book() );
			for (moduleIndex = 0,moduleList->first(); moduleList->current(); moduleList->next(), ++moduleIndex) {
				count = analysisItem->getCountForModule(moduleIndex);
				countStr.setNum(count);
				searchAnalysisHTML += QString::fromLatin1("<td align=\"right\">") + countStr + QString::fromLatin1("</td>");
			}
			searchAnalysisHTML += QString::fromLatin1("</tr>\n");
			ok = key.NextBook();
		}
		text += QString::fromLatin1("<table>\n") + tableTitle + tableTotals + searchAnalysisHTML + QString::fromLatin1("</table>\n");
		text += QString::fromLatin1("<center>") + i18n("Created by") + QString::fromLatin1(" <a href=\"http://www.bibletime.de/\">BibleTime</a></center>");
		text += QString::fromLatin1("</body></html>");
		CToolClass::savePlainFile(file, text);
	}
}

void CSearchDialog::slotUser1() {
	startSearch();
}

void CSearchDialog::slotUser2() {
	if (searcher->isSearching())
		searcher->interruptSearch();
}

void CSearchDialog::startSearch(void) {
	int searchFlags = searchText->getSearchType();	
	// set the parameters
	searchText->updateCurrentProgress(0);
	searchText->updateOverallProgress(0);
	searchText->setText( searchText->getText() );
		
	searcher->setModules( getModuleList() );
	searcher->setSearchedText(searchText->getText());

	if (searchText->isCaseSensitive())
		searchFlags |= CSwordModuleSearch::caseSensitive;
	searcher->resetSearchScope();
	CSwordModuleSearch::scopeType scopeType = searchText->scopeChooser->getScopeType();
	
	if (scopeType == CSwordModuleSearch::Scope_LastSearch) {
		searchFlags |= CSwordModuleSearch::useLastResult;
	}
	else if ( scopeType == CSwordModuleSearch::Scope_Bounds ) {
		searchFlags |= CSwordModuleSearch::useScope;	
		searcher->setSearchScope( searchText->scopeChooser->getScope() );
	}
	searcher->setSearchOptions(searchFlags);
	searcher->startSearchThread();
  startTimer(80);
	
	enableButton(User1,false);
	enableButton(User2,true);
}

void CSearchDialog::timerEvent(QTimerEvent *e){
	if (searcher->isSearching()) {	//st searching, so we have to wait
		int newPercentage = searcher->getPercent(CSwordModuleSearch::currentModule);
		if (old_currentProgress != newPercentage) {
			searchText->updateCurrentProgress(newPercentage);
			old_currentProgress = newPercentage;
		}
		newPercentage = searcher->getPercent(CSwordModuleSearch::allModules);
		if (old_overallProgress != newPercentage) {
			searchText->updateOverallProgress(newPercentage);
			old_overallProgress = newPercentage;
		}
	}
	else {	//searching finished!
		killTimer( e->timerId() );
		enableButton(User1,true);
		enableButton(User2,false);
		searchText->updateCurrentProgress(100);		
		searchText->updateOverallProgress(100);					
		searchAnalysis->reset();
		
		//test: call scope function
		ListKey scope = searcher->scope();
		
		if ( searcher->foundItems() ){
			searchResult->setModuleList(getModuleList());			
			searchAnalysis->setModuleList(getModuleList());
			searchAnalysisView->setContentsPos(0,0);
			searchResult_page->setEnabled(true);
			searchAnalysis_page->setEnabled(true);						
			showPage(pageIndex(searchResult_page));	//the result page
						
			searchAnalysis->analyse();			
		}
		else {
			searchResult->clearResult();
			searchAnalysis->reset();
		}
	}
}

void CSearchDialog::setSearchText(const QString text){
  searchText->setText(text);
	if (!text.isEmpty())
		showPage(pageIndex(searchText_page));
}

/** Returns the search text. If no text was entered return QString::null. */
const QString CSearchDialog::getSearchedText() const {
	return searchText->getText();
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
