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
#include "csearchdialogtext.h"
#include "csearchdialogresult.h"
#include "csearchdialoganalysis.h"
#include "csearchdialogscope.h"
#include "../../backend/sword_backend/cswordmodulesearch.h"
#include "../../backend/sword_backend/cswordmoduleinfo.h"

//KDE includes
#include <kapp.h>
#include <klocale.h>
#include <kkeydialog.h>
#include <kfontdialog.h>
#include <kconfig.h>
#include <kapp.h>

//QT includes
#include <qlabel.h>
#include <qframe.h>
#include <qdict.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qgrid.h>
#include <qhbuttongroup.h>
#include <qpushbutton.h>
#include <qcanvas.h>

CSearchDialog::CSearchDialog(CImportantClasses* importantClasses, QWidget *parent, const char *name )
	: KDialogBase(Tabbed, i18n("Search Dialog"), Close | User1 | User2, User1,
	parent, name,	false, true, i18n("Search"), i18n("Interrupt"), QString::null) {

	m_important = importantClasses;
	ASSERT(m_important->swordBackend);	
	searcher = new CSwordModuleSearch();
	
	ASSERT(searcher);
	moduleList = 0;
	old_currentProgress = 0;
	old_overallProgress = 0;
	initView();
	readSettings();
}

CSearchDialog::~CSearchDialog(){
	if (searcher)
		delete searcher;
	searcher = 0;
	saveSettings();
}

/** Reads the settings from the configfile */
void CSearchDialog::readSettings(){
 	qDebug("CSearchDialog::readSettings()");
}

/** Saves the settings to the config file */
void CSearchDialog::saveSettings(){
 	qDebug("CSearchDialog::saveSettings()");
}


/** Initializs the view */
void CSearchDialog::initView() {
 	enableButton(User2,false);

	searchText_page 	= addVBoxPage(i18n("Search Text"), i18n("Enter the text to search for"));
	searchText			= new CSearchDialogText(m_important, searchText_page);

	searchResult_page = addHBoxPage(i18n("Search Result"), i18n("The result of your search"));
	searchResult = new CSearchDialogResult(m_important, searchResult_page);

	searchAnalysis_page = addVBoxPage(i18n("Search Analysis"), i18n("Graphical analysis of your search result"));	
//	QHButtonGroup*	buttonGroup = new QHButtonGroup(searchAnalysis_page);
	searchAnalysis = new CSearchDialogAnalysis(searchAnalysis_page);
	ASSERT(searchAnalysis);
	ASSERT(searchAnalysis_page);
	CSearchDialogAnalysisView* analysisView =
		new CSearchDialogAnalysisView(searchAnalysis, searchAnalysis_page);

	//create search analysis buttons
//	QPushButton*	button;
//	button = new QPushButton(buttonGroup);
//	button->setText( i18n("Print ...") );
//	connect(button, SIGNAL(clicked()), searchAnalysis, SLOT(print()));
//
//	button = new QPushButton(buttonGroup);
//	button->setText( i18n("Save as image ...") );
//	connect(button, SIGNAL(clicked()), searchAnalysis, SLOT(saveAsImage()));
}

ListCSwordModuleInfo* CSearchDialog::getModuleList() const {
	ASSERT(moduleList);
	return moduleList;
}

void CSearchDialog::setModuleList(ListCSwordModuleInfo *list) {
	qDebug("CSearchDialog::setModuleList(ListCModuleInfo *list)");
	if (!moduleList)
		moduleList = new ListCSwordModuleInfo;
	moduleList->clear();
	
	ASSERT(list);
	for (list->first(); list->current(); list->next()) {
		moduleList->append(list->current());
	}
	searchResult->clearResult();
	searchAnalysis->reset();
}

void CSearchDialog::slotUser1() {
	qDebug("CSearchDialog::slotUser1()");
	startSearch();
}

void CSearchDialog::slotUser2() {
	qDebug("CSearchDialog::slotUser2()");
	if (searcher->isSearching())
		searcher->interruptSearch();
}

void CSearchDialog::startSearch(void) {
	qDebug("CSearchDialog::search()");
	ASSERT(moduleList);
	int searchFlags = searchText->getSearchType();
	
	// set the parameters
	searcher->setModules(moduleList);
	searcher->setSearchedText(searchText->getText());

	qDebug("check case sensitive");	
	if (searchText->isCaseSensitive())
		searchFlags |= CSwordModuleSearch::caseSensitive;
	qDebug("search is case sensitive");
	
	qDebug("reset search scope");	
	searcher->resetSearchScope();
	
	ASSERT(searchText->scopeChooser);
	CSwordModuleSearch::scopeType scopeType = searchText->scopeChooser->getScopeType();
	
	qDebug("set scope");
	if (scopeType == CSwordModuleSearch::Scope_LastSearch) {
		qDebug("use last scope");		
		searchFlags |= CSwordModuleSearch::useLastResult;
#warning bug?
		searcher->setSearchScope( searchText->scopeChooser->getScope() );		
		qDebug("use last search result");
	}
	else if ( scopeType == CSwordModuleSearch::Scope_Bounds ) {
		qDebug("use own scope");				
		searchFlags |= CSwordModuleSearch::useScope;	
		searcher->setSearchScope( searchText->scopeChooser->getScope() );
		qDebug("use search scope");
	}
	
	qDebug("set search options");		
	searcher->setSearchOptions(searchFlags);
	
	qDebug("start search thead!");	
	searcher->startSearchThread();
  startTimer(80);
	
	enableButton(User1,false);
	enableButton(User2,true);
}

void CSearchDialog::timerEvent(QTimerEvent *e){
	qDebug("CSearchDialog::timerEvent(QTimerEvent *e)");
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
		if ( searcher->foundItems() ){
			searchResult->setModuleList(moduleList);
			searchAnalysis->setModuleList(moduleList);
			showPage(1);	//the result page
			
			searchAnalysis->analyse();
		}
		else {
			searchResult->clearResult();
			searchAnalysis->reset();
		}
	}
}

void CSearchDialog::setSearchText(const QString text){
	qDebug("CSearchDialog::setSearchText(QString text)");
  searchText->setText(text);
}

/** Returns the search text. If no text was enetered return QSTring::null. */
QString CSearchDialog::getSearchedText(){
	return searchText->getText();
}
