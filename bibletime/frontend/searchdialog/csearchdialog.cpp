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
#include "../../structdef.h"
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

CSearchDialog::CSearchDialog(CImportantClasses* importantClasses, ListCSwordModuleInfo* modules, QWidget *parent, const char *name )
	: KDialogBase(Tabbed, i18n("Search Dialog"), Close | User1 | User2, User1, parent, name,	false, true, i18n("Search"), i18n("Interrupt"), QString::null) {
	m_important = importantClasses;
	searcher = new CSwordModuleSearch();
	moduleList = 0;
	old_currentProgress = 0;
	old_overallProgress = 0;
	initView();	
	readSettings();	
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
	m_moduleChooser			= new CSearchDialogModuleChooser(m_important, moduleChooser_page);
	connect(m_moduleChooser, SIGNAL(chosenModulesChanged()), SLOT(chosenModulesChanged()));
 	
	searchText_page 	= addVBoxPage(i18n("Search Text"), i18n("Enter the text to search for"));
	searchText			= new CSearchDialogText(m_important, searchText_page);
	searchText_page->setEnabled(false);
	
	searchResult_page = addHBoxPage(i18n("Search Result"), i18n("The result of your search"));
	searchResult = new CSearchDialogResult(m_important, searchResult_page);
	searchResult_page->setEnabled(false);
	
	searchAnalysis_page = addVBoxPage(i18n("Search Analysis"), i18n("Graphical analysis of your search result"));	
	searchAnalysis = new CSearchDialogAnalysis(searchAnalysis_page);
	CSearchDialogAnalysisView* analysisView =	new CSearchDialogAnalysisView(searchAnalysis, searchAnalysis_page);
	searchAnalysis_page->setEnabled(false);	
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
//	if (moduleList->count())
//		showPage(pageIndex(searchText_page));		
	
	searchResult->clearResult();
	searchAnalysis->reset();
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
	searcher->setModules( getModuleList() );
	searcher->setSearchedText(searchText->getText());

	if (searchText->isCaseSensitive())
		searchFlags |= CSwordModuleSearch::caseSensitive;
	searcher->resetSearchScope();
	CSwordModuleSearch::scopeType scopeType = searchText->scopeChooser->getScopeType();
	
	if (scopeType == CSwordModuleSearch::Scope_LastSearch) {
		searchFlags |= CSwordModuleSearch::useLastResult;
		searcher->setSearchScope( searchText->scopeChooser->getScope() );		
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
		
		if ( searcher->foundItems() ){
			searchResult->setModuleList(getModuleList());			
			searchAnalysis->setModuleList(getModuleList());
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

/** Returns the search text. If no text was enetered return QString::null. */
const QString CSearchDialog::getSearchedText() const {
	return searchText->getText();
}

/** No descriptions */
void CSearchDialog::chosenModulesChanged(){
	setModuleList(getModuleList());
}
