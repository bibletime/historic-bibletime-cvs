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

#include "frontend/cbtconfig.h"
#include "frontend/cresmgr.h"
#include "frontend/cexportmanager.h"
#include "frontend/display/cdisplay.h"
#include "frontend/display/creaddisplay.h"

#include "util/cpointers.h"
#include "util/ctoolclass.h"

#include "resource.h"
#include "whatsthisdef.h"
#include "tooltipdef.h"

//Qt includes
#include <qlayout.h>
#include <qhbox.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qpushbutton.h>
#include <qradiobutton.h>
#include <qsplitter.h>
#include <qtooltip.h>
#include <qbuttongroup.h>
#include <qcheckbox.h>
#include <qwhatsthis.h>
#include <qheader.h>

//KDE includes
#include <klocale.h>
#include <kaction.h>
#include <klistview.h>
#include <kcombobox.h>
#include <kprogress.h>
#include <kpopupmenu.h>
#include <ksqueezedtextlabel.h>

/********************************************
************  ModuleResultList **************
********************************************/

CSearchResultView::CSearchResultView(QWidget* parent) : KListView(parent), m_module(0) {
  initView();
  initConnections();
};

CSearchResultView::~CSearchResultView() {
};

/** Initializes the view of this widget. */
void CSearchResultView::initView(){
  addColumn(i18n("Found items"));
  setFullWidth(true);
  setSorting(-1);
  setSelectionModeExt(KListView::Extended);

  //setup the popup menu
  m_popup = new KPopupMenu(this);
//	m_popup->insertTitle(i18n("Bible window"));

 	m_actions.copyMenu = new KActionMenu(i18n("Copy..."), ICON_EDIT_COPY);
 	m_actions.copy.result = new KAction(i18n("Search result"), KShortcut(0), this, SLOT(copyItems()), this);
 	m_actions.copyMenu->insert(m_actions.copy.result);
 	m_actions.copy.resultWithText = new KAction(i18n("Search result with text"), KShortcut(0), this, SLOT(copyItemsWithText()), this);
 	m_actions.copyMenu->insert(m_actions.copy.resultWithText);
 	m_actions.copyMenu->plug(m_popup);

 	m_actions.saveMenu = new KActionMenu(i18n("Save..."),ICON_FILE_SAVE);
 	m_actions.save.result = new KAction(i18n("Search result"), KShortcut(0), this, SLOT(saveItems()), this);
 	m_actions.saveMenu->insert(m_actions.save.result);
 	m_actions.save.resultWithText = new KAction(i18n("Search result with text"), KShortcut(0), this, SLOT(saveItemsWithText()), this);
 	m_actions.saveMenu->insert(m_actions.save.resultWithText);
 	m_actions.saveMenu->plug(m_popup);

 	m_actions.printMenu = new KActionMenu(i18n("Print..."),ICON_FILE_PRINT);
 	m_actions.print.result = new KAction(i18n("Search result with text"), KShortcut(0), this, SLOT(printItems()), this);
 	m_actions.printMenu->insert(m_actions.print.result);
  m_actions.printMenu->plug(m_popup);  
}

/** No descriptions */
void CSearchResultView::initConnections(){
  connect(this, SIGNAL(selectionChanged(QListViewItem*)),
	  SLOT(executed(QListViewItem*)));
  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
    this, SLOT(showPopup(KListView*, QListViewItem*, const QPoint&)));    
}

/** Setups the list with the given module. */
void CSearchResultView::setupTree(CSwordModuleInfo* m){
  clear();
  if (!m)
    return;
  m_module = m;

  sword::ListKey result = m->searchResult();
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
void CSearchResultView::executed(QListViewItem* item){
//  Q_ASSERT(item);
  emit keySelected(item->text(0));
}

/** Reimplementation to show the popup menu. */
void CSearchResultView::showPopup(KListView*, QListViewItem* i, const QPoint& point){
  m_popup->exec(point);
}

/** No descriptions */
void CSearchResultView::printItems(){
  QPtrList<QListViewItem> items = selectedItems();
  CExportManager mgr(i18n("Print search result ..."), true, i18n("Printing search result"));

  PrintItemList list;
  for (QListViewItem* k = items.first(); k; k = items.next()) {
    list.append( QStringPair(k->text(0), QString::null) );
  };
  mgr.printKeyList( module(), list);
}

/** No descriptions */
void CSearchResultView::saveItems(){
}

/** No descriptions */
void CSearchResultView::saveItemsWithText(){
}

/** No descriptions */
void CSearchResultView::copyItems(){

}

/** No descriptions */
void CSearchResultView::copyItemsWithText(){
}

/** Returns the module which is currently used. */
CSwordModuleInfo* const CSearchResultView::module(){
  return m_module;
}

/********************************************
************  ModuleResultList **************
********************************************/

CModuleResultView::CModuleResultView(QWidget* parent) : KListView(parent) {
  initView();
  initConnections();
};

CModuleResultView::~CModuleResultView() {

};

/** Initializes this widget. */
void CModuleResultView::initView(){
  addColumn(i18n("Module"));
  addColumn(i18n("Found items"));
  
//  setFullWidth(true);
  setSorting(0, true);
  setSorting(1, true);
  setAllColumnsShowFocus(true);


  //setup the popup menu
  m_popup = new KPopupMenu(this);
//	m_popup->insertTitle(i18n("Bible window"));

 	m_actions.copyMenu = new KActionMenu(i18n("Copy..."), ICON_EDIT_COPY);
 	m_actions.copy.result = new KAction(i18n("Search result"), KShortcut(0), this, SLOT(copyResult()), this);
 	m_actions.copyMenu->insert(m_actions.copy.result);
 	m_actions.copy.resultWithText = new KAction(i18n("Search result with text"), KShortcut(0), this, SLOT(copyResultWithText()), this);
 	m_actions.copyMenu->insert(m_actions.copy.resultWithText);
 	m_actions.copyMenu->plug(m_popup);

 	m_actions.saveMenu = new KActionMenu(i18n("Save..."),ICON_FILE_SAVE);
 	m_actions.save.result = new KAction(i18n("Search result"), KShortcut(0), this, SLOT(saveResult()), this);
 	m_actions.saveMenu->insert(m_actions.save.result);
 	m_actions.save.resultWithText = new KAction(i18n("Search result with text"), KShortcut(0), this, SLOT(saveResultWithText()), this);
 	m_actions.saveMenu->insert(m_actions.save.resultWithText);
 	m_actions.saveMenu->plug(m_popup);

 	m_actions.printMenu = new KActionMenu(i18n("Print..."),ICON_FILE_PRINT);
 	m_actions.print.result = new KAction(i18n("Search result with text"), KShortcut(0), this, SLOT(printResult()), this);
 	m_actions.printMenu->insert(m_actions.print.result);


  m_actions.printMenu->plug(m_popup);  
}

/** Initializes the connections of this widget, */
void CModuleResultView::initConnections(){
  connect(this, SIGNAL(executed(QListViewItem*)),
    SLOT(executed(QListViewItem*)));
  connect(this, SIGNAL(contextMenu(KListView*, QListViewItem*, const QPoint&)),
    this, SLOT(showPopup(KListView*, QListViewItem*, const QPoint&)));
}

/** Setups the tree using the given list of modules. */
void CModuleResultView::setupTree( ListCSwordModuleInfo modules ){
  clear();
  QListViewItem* item = 0;
  QListViewItem* oldItem = 0;
  sword::ListKey result;
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
void CModuleResultView::executed( QListViewItem* i ){
  if (CSwordModuleInfo* m = CPointers::backend()->findModuleByName(i->text(0))) {
    emit moduleChanged();
    emit moduleSelected(m);
  }
}

/** Returns the currently active module. */
CSwordModuleInfo* const CModuleResultView::activeModule(){
  Q_ASSERT(currentItem());
  if (currentItem())
    return CPointers::backend()->findModuleByName(currentItem()->text(0));
  return 0;
}

/** No descriptions */
void CModuleResultView::showPopup(KListView*, QListViewItem* i, const QPoint& point){
  //make sure that all entries have the correct status

  m_popup->exec(point);
}

/** Copies the whole search result into the clipboard. */
void CModuleResultView::copyResult(){
  if (CSwordModuleInfo* m = activeModule()) {
    sword::ListKey result = m->searchResult();
    CExportManager mgr(i18n("Copy search result ..."), true, i18n("Copying search result"));
    mgr.copyKeyList(&result,m,CExportManager::Text,false);
  };  
}

/** Copies the whole search result with the text into the clipboard. */
void CModuleResultView::copyResultWithText(){
  if (CSwordModuleInfo* m = activeModule()) {
    sword::ListKey result = m->searchResult();
    CExportManager mgr(i18n("Copy search result ..."), true, i18n("Copying search result"));
    mgr.copyKeyList(&result,m,CExportManager::Text,true);
  };
}

/** Saves the search result keys. */
void CModuleResultView::saveResult(){
  if (CSwordModuleInfo* m = activeModule()) {
    sword::ListKey result = m->searchResult();
    CExportManager mgr(i18n("Save search result ..."), true, i18n("Saving search result"));
    mgr.saveKeyList(&result,m,CExportManager::Text,false);
  };  
}

/** Saves the search result with it's text. */
void CModuleResultView::saveResultWithText(){
  if (CSwordModuleInfo* m = activeModule()) {
    sword::ListKey result = m->searchResult();
    CExportManager mgr(i18n("Save search result ..."), true, i18n("Saving search result"));
    mgr.saveKeyList(&result,m,CExportManager::Text,true);
  };
}

/** Appends the whole search result to the printer queue. */
void CModuleResultView::printResult(){
  if (CSwordModuleInfo* m = activeModule()) {
    sword::ListKey result = m->searchResult();
    CExportManager mgr(i18n("Print search result ..."), true, i18n("Printing search result"));
    mgr.printKeyList(&result,m);
  };
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

//  QHBox* layoutBox = new QHBox(splitter);
//  layoutBox->setSpacing(3);
  QSplitter* hSplitter = new QSplitter(Horizontal, splitter);
  m_moduleListBox = new CModuleResultView(hSplitter);
//  m_moduleListBox->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));
//  layoutBox->setStretchFactor(m_moduleListBox, 0);
  m_resultListBox = new CSearchResultView(hSplitter);
  hSplitter->setResizeMode(m_moduleListBox, QSplitter::FollowSizeHint);
  hSplitter->setResizeMode(m_resultListBox, QSplitter::Stretch);

//  layoutBox->setStretchFactor(m_resultListBox, 5);  

  m_previewDisplay = CDisplay::createReadInstance(0, splitter);  

  m_moduleListBox->resize(m_moduleListBox->sizeHint());
  splitter->setResizeMode(hSplitter, QSplitter::KeepSize);
  splitter->setResizeMode(m_previewDisplay->view(), QSplitter::Stretch);
  
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
  m_moduleListBox->setMinimumWidth(m_moduleListBox->sizeHint().width());
  m_moduleListBox->adjustSize();
  m_moduleListBox->parentWidget()->adjustSize();

  
  //have a Bible or commentary in the modules?
  bool enable = false;
  for (modules.first(); !enable && modules.current(); modules.next()) {
    if (modules.current()->type() == CSwordModuleInfo::Bible ) {
      enable = true;
    };
  };
  m_analyseButton->setEnabled(enable);
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
  
  readSettings();
}

CSearchOptionsPage::~CSearchOptionsPage() {
  saveSettings();
}

/** Returns the search text set in this page. */
const QString CSearchOptionsPage::searchText() {
  // we emulate OR by RegExp
  if (m_multipleWordsORRadio->isChecked()) {
    QString regexp(m_searchTextCombo->currentText());

    regexp = regexp.stripWhiteSpace();
    regexp = regexp.simplifyWhiteSpace();    

    int idx = -1;
    QChar orsymbol('|');
    while ((idx = regexp.find(' ', idx+1)) != -1) {
      // use insert as replace() API is pretty strange
      regexp.insert(idx, orsymbol);
      idx++;
    }

    return regexp;
  }
  return m_searchTextCombo->currentText();
}

/** Sets the search text used in the page. */
void CSearchOptionsPage::setSearchText(const QString& text) {
////  m_searchTextCombo->setEditText(text);
	bool found = false;
  int i = 0;
	for (i = 0; !found && i < m_searchTextCombo->count(); ++i) {
		if (m_searchTextCombo->text(i) == text)
			found = true;
	}

	if (!found) {
    i = 0;
		m_searchTextCombo->insertItem(text,0);
	}

  m_searchTextCombo->reset();
  m_searchTextCombo->setCurrentItem(i);
	m_searchTextCombo->setFocus();	
}

/** Initializes this page. */
void CSearchOptionsPage::initView(){
  
  QGridLayout* grid = new QGridLayout(this,11,3);
  grid->setSpacing(3);

  m_chooseModulesButton = new QPushButton(i18n("Choose modules..."), this);
  connect(m_chooseModulesButton, SIGNAL(clicked()),
	  this, SLOT(chooseModules()));
  QToolTip::add(m_chooseModulesButton, CResMgr::searchdialog::options::moduleChooserButton::tooltip);
  QWhatsThis::add(m_chooseModulesButton, CResMgr::searchdialog::options::moduleChooserButton::whatsthis);

  m_modulesLabel = new QLabel(this);
  m_modulesLabel->setTextFormat(RichText);
  m_modulesLabel->setAlignment( AlignLeft | WordBreak );

  grid->addColSpacing(1, 5);

  grid->addWidget(m_chooseModulesButton, 0,0);
  grid->addMultiCellWidget(m_modulesLabel, 1,1,0,2);


  m_searchTextCombo = new KHistoryCombo(this);
  m_searchTextCombo->setInsertionPolicy( QComboBox::AtBottom );
  m_searchTextCombo->setMaxCount(25);
  m_searchTextCombo->setDuplicatesEnabled(false);
  m_searchTextCombo->setFocusPolicy(QWidget::WheelFocus);
  connect( m_searchTextCombo, SIGNAL(activated( const QString& )),	m_searchTextCombo, SLOT( addToHistory( const QString& )));
  connect( m_searchTextCombo, SIGNAL(returnPressed ( const QString& )),m_searchTextCombo,  SLOT(addToHistory(const QString&)) );

  QToolTip::add(m_searchTextCombo, CResMgr::searchdialog::options::searchedText::tooltip);
  QWhatsThis::add(m_searchTextCombo, CResMgr::searchdialog::options::searchedText::whatsthis);
  
    
  QLabel* label = new QLabel(m_searchTextCombo, i18n("Searched text:"), this);
  label->setAutoResize(true);

  grid->addWidget(label, 2, 0);
  grid->addWidget(m_searchTextCombo, 2, 2);

  grid->addRowSpacing(3, 10);

  
  QButtonGroup* group 
    = new QButtonGroup(4, Vertical,i18n("Search type"), this);
    
  m_multipleWordsRadio = new QRadioButton(i18n("Multiple words (AND)"), group);
  m_multipleWordsRadio->setChecked( true );
  QToolTip::add(m_multipleWordsRadio, CResMgr::searchdialog::options::searchType::multipleWords_and::tooltip);
  QWhatsThis::add(m_multipleWordsRadio, CResMgr::searchdialog::options::searchType::multipleWords_and::whatsthis);

  
  m_multipleWordsORRadio =  new QRadioButton(i18n("Multiple words (OR)"), group);
  QToolTip::add(m_multipleWordsORRadio, CResMgr::searchdialog::options::searchType::multipleWords_or::tooltip);
  QWhatsThis::add(m_multipleWordsORRadio, CResMgr::searchdialog::options::searchType::multipleWords_or::whatsthis);
  
  m_exactTextRadio = new QRadioButton(i18n("Exact"), group);
  QToolTip::add(m_exactTextRadio, CResMgr::searchdialog::options::searchType::exactMatch::tooltip);
  QWhatsThis::add(m_exactTextRadio, CResMgr::searchdialog::options::searchType::exactMatch::whatsthis);
  
  m_regexpRadio = new QRadioButton(i18n("Regular expression"), group);
  QToolTip::add(m_regexpRadio, CResMgr::searchdialog::options::searchType::regExp::tooltip);
  QWhatsThis::add(m_regexpRadio, CResMgr::searchdialog::options::searchType::regExp::whatsthis);

  grid->addWidget(group, 4,0);

  group = new QButtonGroup(1,Vertical,i18n("Search options"), this);
  m_caseSensitiveBox = new QCheckBox(i18n("Case sensitive search"), group);
  QToolTip::add(m_caseSensitiveBox, CResMgr::searchdialog::options::searchOptions::caseSensitive::tooltip);
  QWhatsThis::add(m_caseSensitiveBox, CResMgr::searchdialog::options::searchOptions::caseSensitive::whatsthis);

  grid->addWidget(group, 4,2);
  grid->addRowSpacing(5,10);

  m_rangeChooserCombo = new KComboBox(this);
  QToolTip::add(m_rangeChooserCombo, CResMgr::searchdialog::options::chooseScope::tooltip);
  QWhatsThis::add(m_rangeChooserCombo, CResMgr::searchdialog::options::chooseScope::whatsthis);
  
  refreshRanges();  
  label = new QLabel(m_rangeChooserCombo, i18n("Choose search scope:"),this);
  
  m_chooseRangeButton = new QPushButton(i18n("Setup custom ranges..."), this);
  connect(m_chooseRangeButton, SIGNAL(clicked()),
    this, SLOT(setupRanges()));
  

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
//  qWarning("CSearchOptionsPage::setModules");
  m_modules = modules;
  QString t = i18n("Searching in: ");
  CSwordModuleInfo* lastModule = modules.last();
  for (modules.first(); modules.current(); modules.next()) {
    t += modules.current()->name();
    if (modules.current() != lastModule)
      t += QString::fromLatin1(",");
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
  return m_modules;
}

/** Prepares the stuff which is required for a search, e.g. setting back the percentage bars. */
void CSearchOptionsPage::prepareSearch(){
//  qWarning("CSearchOptionsPage::prepareSearch");
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
//  qWarning("CSearchOptionsPage::searchType");
	int ret = CSwordModuleSearch::multipleWords;	//"multiple words" is standard
	if (m_exactTextRadio->isChecked()) {
		ret = CSwordModuleSearch::exactPhrase;
	}
	else if (m_regexpRadio->isChecked() || m_multipleWordsORRadio->isChecked()) {
		ret = CSwordModuleSearch::regExp;
	}
	if (m_caseSensitiveBox->isChecked())
	  ret |= CSwordModuleSearch::caseSensitive;
	return ret;
}

/** Sets all options back to the default. */
void CSearchOptionsPage::reset(){
  m_multipleWordsRadio->setChecked(true);
  m_rangeChooserCombo->setCurrentItem(0); //no scope
  m_searchTextCombo->clear();
}

/** Reads the settings for the searchdialog from disk. */
void CSearchOptionsPage::saveSettings(){
	QStringList list = m_searchTextCombo->completionObject()->items();
	CBTConfig::set(CBTConfig::searchCompletionTexts, list);

	list = m_searchTextCombo->historyItems();
	CBTConfig::set(CBTConfig::searchTexts, list);	
}

/** Reads the settings of the last searchdialog session. */
void CSearchOptionsPage::readSettings(){
	QStringList list = CBTConfig::get( CBTConfig::searchCompletionTexts );
	m_searchTextCombo->completionObject()->setItems( list );

	list = CBTConfig::get(CBTConfig::searchTexts);
	m_searchTextCombo->setHistoryItems( list );
}

void CSearchOptionsPage::aboutToShow(){
  m_searchTextCombo->setFocus();
}

/** No descriptions */
void CSearchOptionsPage::setupRanges(){
  CRangeChooserDialog* chooser = new CRangeChooserDialog(this);
  chooser->exec();

  refreshRanges();
}

/** refreshes the listof ranges and the range combobox. */
void CSearchOptionsPage::refreshRanges(){
  //the first two options are fixed, the others can be edited using the "Setup ranges" button.
  m_rangeChooserCombo->clear();
  m_rangeChooserCombo->insertItem(i18n("No search scope"));
  m_rangeChooserCombo->insertItem(i18n("Last search result"));

  //insert the user-defined ranges
  CBTConfig::StringMap map = CBTConfig::get(CBTConfig::searchScopes);
  CBTConfig::StringMap::Iterator it;
  for (it = map.begin(); it != map.end(); ++it) {
    m_rangeChooserCombo->insertItem(it.key());
  };
}

/** Returns the selected search scope if a search scope was selected. */
sword::ListKey CSearchOptionsPage::searchScope(){
  if (m_rangeChooserCombo->currentItem() > 1) { //neither "No Scope" nor "Last search result"
    CBTConfig::StringMap map = CBTConfig::get(CBTConfig::searchScopes);
    QString scope = map[ m_rangeChooserCombo->currentText() ];
//    qWarning(scope.latin1());
    if (!scope.isEmpty())
      return sword::VerseKey().ParseVerseList( scope.local8Bit(), "Genesis 1:1", true);
  };
  return sword::ListKey();
}

/** Returns the selected scope type. */
const CSwordModuleSearch::scopeType CSearchOptionsPage::scopeType(){
  if (m_rangeChooserCombo->currentItem() == 0) {
    return CSwordModuleSearch::Scope_NoScope;
  }
  else if (m_rangeChooserCombo->currentItem() == 1) {
    return CSwordModuleSearch::Scope_LastSearch;
  }
  else {
    return CSwordModuleSearch::Scope_Bounds;
  };
  return CSwordModuleSearch::Scope_NoScope;
}

