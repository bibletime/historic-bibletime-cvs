/***************************************************************************
                          csearchdialogpages.cpp  -  description
                             -------------------
    begin                : Die Jul 30 2002
    copyright            : (C) 2002 by The BibleTime team
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

#include "csearchdialogpages.h"

#include "csearchdialog.h"

#include "backend/centrydisplay.h"

#include "frontend/ctoolclass.h"
#include "frontend/cpointers.h"
#include "frontend/cbtconfig.h"
#include "frontend/display/cdisplay.h"
#include "frontend/display/creaddisplay.h"

//Qt includes
#include <qlayout.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qsplitter.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>

//KDE includes
#include <klocale.h>
#include <klistview.h>
#include <kcombobox.h>
#include <kprogress.h>
#include <ksqueezedtextlabel.h>

/********************************************
************  ModuleResultList **************
********************************************/

CModuleResultView::CModuleResultView(QWidget* parent) : KListView(parent) {
  initView();
  initConnections();
};

CModuleResultView::~CModuleResultView() {
};

/** Initializes the view of this widget. */
void CModuleResultView::initView(){
  addColumn(i18n("Found items"));
  setFullWidth(true);
  setSorting(-1);
}

/** No descriptions */
void CModuleResultView::initConnections(){
  connect(this, SIGNAL(executed(QListViewItem*)),
    SLOT(executed(QListViewItem*)));
}

/** Setups the list with the given module. */
void CModuleResultView::setupTree(CSwordModuleInfo* m){
  clear();
  Q_ASSERT(m);
  if (!m)
    return;

  ListKey result = m->searchResult();
	const int count = result.Count();		
  if (!count)
    return;

	setUpdatesEnabled(false);

  QListViewItem* oldItem = 0;
  KListViewItem* item = 0;
	for (int index = 0; index < count; index++) {
    item = new KListViewItem(this, oldItem);
    item->setText(0,QString::fromLocal8Bit((const char*)*result.GetElement(index)));
    oldItem = item;
	}	

  setUpdatesEnabled(true);

  setSelected(firstChild(), true);
  executed(currentItem());
}

/** Is connected to the signal executed, which is emitted when a mew item was chosen. */
void CModuleResultView::executed(QListViewItem* item){
  Q_ASSERT(item);
  emit keySelected(item->text(0));
}


/********************************************
************  ModuleResultList **************
********************************************/

CModuleListView::CModuleListView(QWidget* parent) : KListView(parent) {
  initView();
  initConnections();
};

CModuleListView::~CModuleListView() {

};

/** Initializes this widget. */
void CModuleListView::initView(){
  addColumn(i18n("Module"));
  addColumn(i18n("Found items"));
  setFullWidth(true);
  setSorting(0, true);
  setSorting(1, true);
  setAllColumnsShowFocus(true);
}

/** Initializes the connections of this widget, */
void CModuleListView::initConnections(){
  connect(this, SIGNAL(executed(QListViewItem*)),
    SLOT(executed(QListViewItem*)));
}

/** Setups the tree using the given list of modules. */
void CModuleListView::setupTree( ListCSwordModuleInfo modules ){
  clear();
  QListViewItem* item = 0;
  QListViewItem* oldItem = 0;
  ListKey result;
  for (modules.first(); modules.current(); modules.next()) {
		result = modules.current()->searchResult();
		item = new KListViewItem(this, modules.current()->name(), QString::number(result.Count()) );
		item->setPixmap(0,CToolClass::getIconForModule(modules.current()) );
    oldItem = item;
  };

  setSelected(currentItem(), true);
  executed(currentItem());
}


/** Is executed when an item was selected in the list. */
void CModuleListView::executed( QListViewItem* i ){
  if (CSwordModuleInfo* m = CPointers::backend()->findModuleByName(i->text(0))) {
    emit moduleChanged();
    emit moduleSelected(m);
  }
}

/** Returns the currently active module. */
CSwordModuleInfo* const CModuleListView::activeModule(){
  Q_ASSERT(currentItem());
  if (currentItem())
    return CPointers::backend()->findModuleByName(currentItem()->text(0));
  return 0;
}



/********************************************
**********  CSearchDialogResultPage *********
********************************************/

CSearchResultPage::CSearchResultPage(QWidget *parent, const char *name ) : QWidget(parent,name) {
  initView();
  initConnections();
}

CSearchResultPage::~CSearchResultPage(){

}

/** Initializes the view of this widget. */
void CSearchResultPage::initView(){
  QVBoxLayout* mainLayout = new QVBoxLayout(this);
  QSplitter* splitter = new QSplitter(Vertical, this);
  mainLayout->addWidget(splitter);

  QHBox* layoutBox = new QHBox(splitter);
  layoutBox->setSpacing(3);
  m_moduleListBox = new CModuleListView(layoutBox);
  m_resultListBox = new CModuleResultView(layoutBox);
  m_previewDisplay = CDisplay::createReadInstance(0, splitter);

  m_analyseButton = new QPushButton(i18n("Show search analysis")+QString::fromLatin1("..."), this);
  connect(m_analyseButton, SIGNAL(clicked()),
    this, SLOT(showAnalysis()));
  mainLayout->addSpacing(5);
  mainLayout->addWidget(m_analyseButton);
}

/** Sets the modules which contain the result of each. */
void CSearchResultPage::setSearchResult(ListCSwordModuleInfo modules){
  m_modules = modules;
  reset();
  m_moduleListBox->setupTree(modules);

  //have a Bible or commentary in the modules?
  bool isOk = false;
  for (modules.first(); !isOk && modules.current(); modules.next()) {
    isOk = (modules.current()->type() == CSwordModuleInfo::Bible) || (modules.current()->type() == CSwordModuleInfo::Commentary);
  };
  m_analyseButton->setEnabled(isOk);
}


/** Resets the current list of modules and the displayed list of found entries. */
void CSearchResultPage::reset(){
  m_moduleListBox->clear();
  m_resultListBox->clear();
  m_previewDisplay->setText(QString::null);
  m_analyseButton->setEnabled(false);
}


/** Update the preview of the selected key. */
void CSearchResultPage::updatePreview(const QString& key){
  if ( CSwordModuleInfo* module = m_moduleListBox->activeModule() ) {
  	if (CEntryDisplay* display = module->getDisplay()) {	//do we have a display object?
      ListCSwordModuleInfo moduleList;
      moduleList.append(module);
   		m_previewDisplay->setText( display->text( moduleList, key, CBTConfig::getDisplayOptionDefaults(), CBTConfig::getFilterOptionDefaults() ) );
      m_previewDisplay->moveToAnchor( key );
  	}	
    else
      m_previewDisplay->setText(QString::null);
  }
}

/** Initializes the signal slot conections of the child widgets, */
void CSearchResultPage::initConnections(){
  connect(m_resultListBox, SIGNAL(keySelected(const QString&)),
    this, SLOT(updatePreview(const QString&)));
  connect(m_moduleListBox, SIGNAL(moduleSelected(CSwordModuleInfo*)),
    m_resultListBox, SLOT(setupTree(CSwordModuleInfo*)));
  connect(m_moduleListBox, SIGNAL(moduleChanged()),
    m_previewDisplay->connectionsProxy(), SLOT(clear()));
}

/** Shows a dialog with the search analysis of the current search. */
void CSearchResultPage::showAnalysis(){
  CSearchAnalysisDialog dlg(m_modules, this);
  dlg.exec();
}

/*************************/

CSearchOptionsPage::CSearchOptionsPage(QWidget *parent, const char *name ) : QWidget(parent,name) {
  initView();
}

CSearchOptionsPage::~CSearchOptionsPage() {
}

/** Returns the search text set in this page. */
const QString CSearchOptionsPage::searchText() {
  return m_searchTextCombo->currentText();
}

/** Sets the search text used in the page. */
void CSearchOptionsPage::setSearchText(const QString& text) {
  m_searchTextCombo->setCurrentText(text);
}

/** Initializes this page. */
void CSearchOptionsPage::initView(){
  QGridLayout* grid = new QGridLayout(this,11,3);
  grid->setSpacing(3);

  m_chooseModulesButton = new QPushButton(i18n("Choose modules..."), this);
  connect(m_chooseModulesButton, SIGNAL(clicked()),
    this, SLOT(chooseModules()));

//  m_modulesLabel = new KSqueezedTextLabel(this);
  m_modulesLabel = new QLabel(this);
  m_modulesLabel->setTextFormat(RichText);
  m_modulesLabel->setAlignment( AlignLeft | WordBreak );
//  m_modulesLabel->setAutoResize(true);

  grid->addColSpacing(1, 5);

  grid->addWidget(m_chooseModulesButton, 0,0);
//  grid->addMultiCell(new QSpacerItem(1,1), 0,0,1,2);
  grid->addMultiCellWidget(m_modulesLabel, 1,1,0,2);


  m_searchTextCombo = new KHistoryCombo(this);
  QLabel* label = new QLabel(m_searchTextCombo, i18n("Searched text:"), this);
  label->setAutoResize(true);

  grid->addWidget(label, 2, 0);
  grid->addWidget(m_searchTextCombo, 2, 2);

  grid->addRowSpacing(3, 10);

  QButtonGroup* group = new QButtonGroup(3,Vertical,i18n("Search type"),this);
  m_multipleWordsRadio = new QRadioButton(i18n("Multiple words"), group);
	m_multipleWordsRadio->setChecked( true );
  m_exactTextRadio = new QRadioButton(i18n("Exact"), group);
  m_regexpRadio = new QRadioButton(i18n("Regular expression"), group);

//  grid->addMultiCellWidget(group,3,3,0,1 );
  grid->addWidget(group, 4,0);

  group = new QButtonGroup(1,Vertical,i18n("Search options"), this);
  m_caseSensitiveBox = new QCheckBox(i18n("Case sensitive search"), group);

//  grid->addMultiCellWidget(group,3,3,0,2 );
  grid->addWidget(group, 4,2);
  grid->addRowSpacing(5,10);

  m_rangeChooserCombo = new KHistoryCombo(this);
  label = new QLabel(m_rangeChooserCombo, i18n("Choose search range:"),this);
  label->setAutoResize(true);
  m_chooseRangeButton = new QPushButton(i18n("Setup ranges"), this);

  grid->addMultiCellWidget(label, 6,6,0,2);
  grid->addWidget(m_rangeChooserCombo, 7,0);
  grid->addWidget(m_chooseRangeButton, 7,2);

  grid->setRowStretch(8,5);
	
  label = new QLabel(i18n("Current module:"),this);
  grid->addWidget(label, 9,0);
	m_currentProgressBar = new KProgress(this);
  grid->addWidget(m_currentProgressBar, 9,2);

  label = new QLabel(i18n("All modules:"),this);
  grid->addWidget(label, 10,0);
	m_overallProgressBar = new KProgress(this);
  grid->addWidget(m_overallProgressBar, 10,2);

  //set the initial focus
  m_searchTextCombo->setFocus();
}

/** Sets the modules used by the search. */
void CSearchOptionsPage::setModules( ListCSwordModuleInfo modules ) {
  qWarning("CSearchOptionsPage::setModules");
  m_modules = modules;
  QString t = i18n("Searching in: ");
  CSwordModuleInfo* lastModule = modules.last();
  for (modules.first(); modules.current(); modules.next()) {
    t += modules.current()->name();
    if (modules.current() != lastModule)
      t += QString::fromLatin1(" ");
  }
  m_modulesLabel->setText(t);

}

/** Opens the modules chooser dialog. */
void CSearchOptionsPage::chooseModules(){
  CModuleChooserDialog* dlg = new CModuleChooserDialog(this, modules());
  connect(dlg, SIGNAL(modulesChanged(ListCSwordModuleInfo)),
    this, SLOT(setModules(ListCSwordModuleInfo)));
  dlg->show();
}

/** Returns the list of used modules. */
const ListCSwordModuleInfo CSearchOptionsPage::modules(){
  qWarning("ListCSwordModuleInfo CSearchOptionsPage::modules");
  qWarning("modules count: %i; fist: %s", m_modules.count(), m_modules.first()->name().latin1());

  return m_modules;
}

/** Prepares the stuff which is required for a search, e.g. setting back the percentage bars. */
void CSearchOptionsPage::prepareSearch(){
  qWarning("CSearchOptionsPage::prepareSearch");
	m_overallProgressBar->setProgress(0);
	m_currentProgressBar->setProgress(0);
}

/** Sets the value for the "curret module" progress bar. */
void CSearchOptionsPage::setCurrentModuleProgress( const int progress ){
	m_currentProgressBar->setProgress(progress);
}

/** Sets the progress for the "all modules" bar. */
void CSearchOptionsPage::setOverallProgress( const int progress ){
	m_overallProgressBar->setProgress(progress);
}

/** Return the selected search type,. */
const int CSearchOptionsPage::searchFlags() {
  qWarning("CSearchOptionsPage::searchType");
	int ret = CSwordModuleSearch::multipleWords;	//"multiple words" is standard
	if (m_exactTextRadio->isChecked()) {
		ret = CSwordModuleSearch::exactPhrase;
	}
	else if (m_regexpRadio->isChecked()) {
		ret = CSwordModuleSearch::regExp;
	}
  if (m_caseSensitiveBox->isChecked())
    ret |= CSwordModuleSearch::caseSensitive;
	return ret;
}

/** Sets all options back to the default. */
void CSearchOptionsPage::reset(){
  m_multipleWordsRadio->setChecked(true);
}
