/***************************************************************************
                          csearchdialog.cpp  -  description
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

#include "csearchdialog.h"

#include "resource.h"
#include "whatsthisdef.h"
#include "tooltipdef.h"

#include "backend/cswordkey.h"
#include "backend/cswordversekey.h"

#include "frontend/cbtconfig.h"
#include "frontend/cresmgr.h"

#include "util/ctoolclass.h"

//Qt includes
#include <qhbox.h>
#include <qptrlist.h>
#include <qpainter.h>
#include <qwhatsthis.h>
#include <qlayout.h>
#include <qmap.h>
#include <qlineedit.h>
#include <qtextedit.h>
#include <qlabel.h>
#include <qsizepolicy.h>
#include <qpushbutton.h>
#include <qheader.h>

//KDE includes
#include <kapplication.h>
#include <kfiledialog.h>
#include <klocale.h>

const int SPACE_BETWEEN_PARTS = 5;
const int RIGHT_BORDER = 15;
const int LEFT_BORDER = 15;
const int LOWER_BORDER = 10;
const int UPPER_BORDER = 10;

const int ITEM_TEXT_SIZE = 8;
const int LABEL_TEXT_SIZE = 6;

//used for the shift between the bars
const int BAR_DELTAX = 4;
const int BAR_DELTAY = 2;
const int BAR_WIDTH  = 2 + (2*BAR_DELTAX);  //should be equal or bigger than the label font size
//used for the text below the bars
const int BAR_LOWER_BORDER = 100;

const int LEGEND_INNER_BORDER = 5;
const int LEGEND_DELTAY = 4;
const int LEGEND_WIDTH = 85;

static CSearchDialog* m_staticDialog = 0;

void CSearchDialog::openDialog(const ListCSwordModuleInfo modules, const QString& searchText, QWidget* parentDialog) {
  if (!m_staticDialog) {
    m_staticDialog = new CSearchDialog(parentDialog);
  };
  Q_ASSERT(m_staticDialog);
  m_staticDialog->reset();
  if (modules.count()) {
    m_staticDialog->setModules(modules);
  } else {
    m_staticDialog->showModulesSelector();
  }
  m_staticDialog->setSearchText(searchText);
  if (m_staticDialog->isHidden()) {
    m_staticDialog->show();
  }
  
  m_staticDialog->raise();
  if (modules.count() && !searchText.isEmpty())
    m_staticDialog->startSearch();
};

CSearchDialog* const CSearchDialog::getSearchDialog() {
  Q_ASSERT(m_staticDialog);
  return m_staticDialog;
};

CSearchDialog::CSearchDialog(QWidget *parent)
  : KDialogBase(Tabbed, i18n("Search dialog"), Close | User1 | User2, User1, parent, "CSearchDialog", false, true, i18n("Search"), i18n("Interrupt")) {

	setIcon(MODULE_SEARCH_ICON_SMALL);
	m_searcher.connectPercentUpdate(this, SLOT(percentUpdate()));
	m_searcher.connectFinished(this, SLOT(searchFinished()));

  initView();
  initConnections();
}

CSearchDialog::~CSearchDialog(){
}

/** Starts the search with the set modules and the set search text. */
void CSearchDialog::startSearch(){
//  qWarning("CSearchDialog::startSearch()");
  QString searchText(m_searchOptionsPage->searchText());

  if (searchText.isEmpty()) return;

  m_searchResultPage->reset();
  enableButton(User1,false);
  enableButton(User2,true);
  m_interruptedSearch = false;

  int searchFlags = m_searchOptionsPage->searchFlags();	
  m_searchOptionsPage->prepareSearch();

  const CSwordModuleSearch::scopeType scopeType 
    = m_searchOptionsPage->scopeType();
  if (scopeType == CSwordModuleSearch::Scope_LastSearch) {
    searchFlags |= CSwordModuleSearch::useLastResult;
  } else if ( scopeType == CSwordModuleSearch::Scope_Bounds ) {
    searchFlags |= CSwordModuleSearch::useScope;
    qWarning(m_searchOptionsPage->searchScope());
    m_searcher.setSearchScope( m_searchOptionsPage->searchScope() );
  }  
  
  m_searcher.setModules( modules() );
  m_searcher.setSearchedText(searchText);
  m_searcher.setSearchOptions(searchFlags);

  qWarning("start the search");
  m_searcher.startSearchThread();
}

/** Starts the search with the given module list and given search text. */
void CSearchDialog::startSearch( const ListCSwordModuleInfo modules, const QString& searchText){
  m_searchResultPage->reset();
  m_searchOptionsPage->reset();
  setModules(modules);
  setSearchText(searchText);

  startSearch();
}

/** Returns the list of used modules. */
const ListCSwordModuleInfo CSearchDialog::modules(){
  return m_searchOptionsPage->modules();
}

/** Sets the list of modules for the search. */
void CSearchDialog::setModules( const ListCSwordModuleInfo modules ){
  m_searchOptionsPage->setModules(modules);
}

/** Returns the search text which is set currently. */
const QString CSearchDialog::searchText(){
  return m_searchOptionsPage->searchText();
}

sword::ListKey CSearchDialog::searchScope(){
  qWarning("CSearchDialog::searchScope()");
  return m_searchOptionsPage->searchScope();
};

/** Returns true if the search used a scope, otherwise false. */
const CSwordModuleSearch::scopeType CSearchDialog::searchScopeType() const {
  qWarning("CSearchDialog::searchScopeType()");
  return m_searchOptionsPage->scopeType();
}

/** Returns true if the search used a scope, otherwise false. */
const int CSearchDialog::searchFlags() const {
  qWarning("CSearchDialog::searchFlags()");
  return m_searchOptionsPage->searchFlags();
}
  
/** Returns the search text which is used for the search. */
void CSearchDialog::setSearchText( const QString searchText ){
  m_searchOptionsPage->setSearchText(searchText);
}

/** Initializes this object. */
void CSearchDialog::initView(){
  setButtonTip(User1, CResMgr::searchdialog::searchButton::tooltip);
  setButtonWhatsThis(User1, CResMgr::searchdialog::searchButton::whatsthis);

  setButtonTip(User2, CResMgr::searchdialog::cancelSearchButton::tooltip);
  setButtonWhatsThis(User2, CResMgr::searchdialog::cancelSearchButton::whatsthis);

  QHBox* box = addHBoxPage(i18n("Options"));
  m_index.optionsPage = pageIndex(box);
  m_searchOptionsPage = new CSearchOptionsPage(box);
  
  box = addHBoxPage(i18n("Search result"));
  m_index.resultPage = pageIndex(box);
  m_searchResultPage = new CSearchResultPage(box);
}

/** Updates the percentage bars. */
void CSearchDialog::percentUpdate(){
  updateProgress();
}

/** Updates the percentage bars. */
void CSearchDialog::searchFinished(){
//  qWarning("CSearchDialog::searchFinished()");
 	m_searchOptionsPage->setOverallProgress(100);
 	m_searchOptionsPage->setCurrentModuleProgress(100);

 	if ( !m_interruptedSearch && m_searcher.foundItems() ){
 		m_searchResultPage->setSearchResult(modules());
    showPage(m_index.resultPage);
 	}
 	else
 		m_searchResultPage->reset();

 	enableButton(User2,false);
 	enableButton(User1,true); 	
}

void CSearchDialog::showModulesSelector() {
  m_searchOptionsPage->chooseModules();
}

/** Initializes the signal slot connections */
void CSearchDialog::initConnections(){
//  qWarning("CSearchDialog::initConnection()");
  connect(this, SIGNAL(user1Clicked()), SLOT(startSearch()));
  connect(this, SIGNAL(user2Clicked()), SLOT(interruptSearch()));
  connect(this, SIGNAL(closeClicked()), SLOT(slotDelayedDestruct()));  
  connect(this, SIGNAL(aboutToShowPage(QWidget*)), SLOT(slotShowPage(QWidget*)));
}

/** Updates the progress. */
void CSearchDialog::updateProgress(){
  KApplication::kApplication()->processEvents();
  m_searchOptionsPage->setOverallProgress(m_searcher.getPercent(CSwordModuleSearch::allModules));
  m_searchOptionsPage->setCurrentModuleProgress(m_searcher.getPercent(CSwordModuleSearch::currentModule));
}

/** Interrupts the search. */
void CSearchDialog::interruptSearch(){
  m_interruptedSearch = true;
  m_searcher.interruptSearch();
  m_searchOptionsPage->prepareSearch();
}

/** Resets the parts to the default. */
void CSearchDialog::reset(){
  m_searchOptionsPage->reset();
  m_searchResultPage->reset();
  showPage(m_index.optionsPage);
}

/** Is the slot which is called when a page will be shown. */
void CSearchDialog::slotShowPage(QWidget* page){
//  qWarning("show a page");
  if (pageIndex(page) == m_index.optionsPage) {
//    qWarning("show options page"),
    m_searchOptionsPage->aboutToShow();
  };
}

/** Reimplementation. */
void CSearchDialog::slotClose(){
//  qWarning("delayed destruction");
  delayedDestruct();
  m_staticDialog = 0;
}

/****************************/
/****************************/
/****************************/

CModuleChooser::ModuleCheckBoxItem::ModuleCheckBoxItem(QListViewItem* item, CSwordModuleInfo* module) : QCheckListItem(item, QString::null, QCheckListItem::CheckBox) {
  m_module = module;
  setText(0,m_module->name());
};

CModuleChooser::ModuleCheckBoxItem::~ModuleCheckBoxItem() {

};

/** Returns the used module. */
CSwordModuleInfo* const CModuleChooser::ModuleCheckBoxItem::module() const{
  return m_module;
}


/****************************/
/****************************/
/****************************/

CModuleChooser::CModuleChooser(QWidget* parent) : KListView(parent) {
  initView();
  initTree();
};

CModuleChooser::~CModuleChooser() {

};

/** Initializes this widget and the childs of it.
 */
void CModuleChooser::initView(){
  addColumn("Module Name");
  setRootIsDecorated(true);
//  header()->hide();
  setFullWidth(true);
}

/** Initializes the tree of this widget. */
void CModuleChooser::initTree(){
  ListCSwordModuleInfo mods = backend()->moduleList();
  /**
  * The next steps:
  * 1. Sort by type
  * 2. Sort the modules of this type by their language
  * 3. Create the subfolders for this
  */

  QMap<CSwordModuleInfo::ModuleType, QString> typenameMap;
  typenameMap.insert(CSwordModuleInfo::Bible, i18n("Bibles"));
  typenameMap.insert(CSwordModuleInfo::Commentary, i18n("Commentaries"));
  typenameMap.insert(CSwordModuleInfo::Lexicon, i18n("Lexicons"));
  typenameMap.insert(CSwordModuleInfo::GenericBook, i18n("Books"));

  int type = CSwordModuleInfo::Bible;
  bool ok = true;
  bool addedDevotionals = false;
  bool addedGlossaries = false;
  bool addedLexs = false;
  bool incType = false;

  while (ok) {
    ListCSwordModuleInfo modsForType;
    QString typeFolderCaption = QString::null;
    incType = false;  
    if (static_cast<CSwordModuleInfo::ModuleType>(type) == CSwordModuleInfo::Lexicon) {
      if (!addedLexs) {
        for (mods.first(); mods.current(); mods.next()) {
          if (mods.current()->type() == CSwordModuleInfo::Lexicon && (mods.current()->category() != CSwordModuleInfo::DailyDevotional) && (mods.current()->category() != CSwordModuleInfo::Glossary)) {
            modsForType.append(mods.current());
          };
        };
        addedLexs = true;
        typeFolderCaption = QString::null;
      }
      else if (!addedDevotionals) {
        for (mods.first(); mods.current(); mods.next()) {
          if (mods.current()->category() == CSwordModuleInfo::DailyDevotional) {
            modsForType.append(mods.current());
          };
        };
        addedDevotionals = true;
        typeFolderCaption = i18n("Daily Devotionals");
      }
      else if (!addedGlossaries) {
        for (mods.first(); mods.current(); mods.next()) {
          if (mods.current()->category() == CSwordModuleInfo::Glossary) {
            modsForType.append(mods.current());
          };
        };
        addedGlossaries = true;
        typeFolderCaption = i18n("Glossaries");        
      };

      if (addedLexs && addedDevotionals && addedGlossaries)
        incType = true;
    }
    else if (type == CSwordModuleInfo::Bible || type == CSwordModuleInfo::Commentary || type == CSwordModuleInfo::GenericBook){
      for (mods.first(); mods.current(); mods.next()) {
        if (mods.current()->type() == type) {
          modsForType.append(mods.current());
        };
      };
      incType = true;
    }
    else
      ok = false;

    if (typeFolderCaption.isEmpty())
      typeFolderCaption = typenameMap[static_cast<CSwordModuleInfo::ModuleType>(type)];

    //get the available languages of the selected modules
    QStringList langs;
    for (modsForType.first(); modsForType.current(); modsForType.next()) {
      if ( !langs.contains(QString::fromLatin1( modsForType.current()->module()->Lang() ))) {
        langs.append(QString::fromLatin1( modsForType.current()->module()->Lang() ));
      }
    };
    langs.sort();

    //go through the list of languages and create subfolders for each language and the modules of the language
    QListViewItem* typeFolder = 0;
    if (modsForType.count())
      typeFolder = new QListViewItem(this, typeFolder, typeFolderCaption);
    else {
      if (incType)
        type++;
      continue;
    };

    
    QString language = QString::null;
    CLanguageMgr* langMgr = languageMgr();
    for ( QStringList::Iterator it = langs.begin(); it != langs.end(); ++it ) {
      language = langMgr->languageForAbbrev(*it).name();
      
      QListViewItem* langFolder = new QListViewItem(typeFolder,language);
      langFolder->setPixmap(0,GROUP_ICON_SMALL);
//      const QString currentLang = (*it);

      //create the module items of this lang folder
      for (modsForType.first(); modsForType.current(); modsForType.next()) {
        if (QString::fromLatin1( modsForType.current()->module()->Lang() ) == (*it) ) { //found correct language
          ModuleCheckBoxItem* i = new ModuleCheckBoxItem(langFolder, modsForType.current());
          i->setPixmap(0, CToolClass::getIconForModule(modsForType.current()));
        };
      };
    };
    typeFolder->setPixmap(0,GROUP_ICON_SMALL);

    if (incType)
      ++type;
  };
}

/** Returns a list of selected modules. */
ListCSwordModuleInfo CModuleChooser::modules(){
  ListCSwordModuleInfo mods;
  QListViewItemIterator it( this );
  for ( ; it.current(); ++it ) {
    if ( ModuleCheckBoxItem* i = dynamic_cast<ModuleCheckBoxItem*>(it.current()) ) {
      //add the module if the box is checked
      if (i->isOn()) {
        mods.append(i->module());
      };
    };
  };

  return mods;
}

/** Sets the list of modules and updates the state of the checkbox items. */
void CModuleChooser::setModules( ListCSwordModuleInfo modules ){
//  qWarning("CModuleChooser::setModules( ListCSwordModuleInfo modules )");
  QListViewItemIterator it( this );
  for ( ; it.current(); ++it ) {
    if ( ModuleCheckBoxItem* i = dynamic_cast<ModuleCheckBoxItem*>(it.current()) ) {
      i->setOn(modules.contains(i->module()));
      if (i->isOn())
        ensureItemVisible(i);
    }
  };
}

/****************************/

CModuleChooserDialog::CModuleChooserDialog( QWidget* parentDialog, ListCSwordModuleInfo modules ) : KDialogBase(Plain, i18n("Choose modules ..."), Ok, Ok, parentDialog, "CModuleChooser", false, true) {
  initView();
  initConnections();

  m_moduleChooser->setModules(modules);
};

CModuleChooserDialog::~CModuleChooserDialog() {

};

/** Initializes the view of this dialog */
void CModuleChooserDialog::initView(){
  setButtonOKText(i18n("Use chosen modules"));

  QFrame* page = plainPage();
  QHBoxLayout* layout = new QHBoxLayout(page);
  m_moduleChooser = new CModuleChooser(page);
  m_moduleChooser->setMinimumSize(320,400);
  layout->addWidget(m_moduleChooser);
}

/** Initializes the connections of this dialog. */
void CModuleChooserDialog::initConnections(){

}

/** Reimplementation to handle the modules. */
void CModuleChooserDialog::slotOk(){
  emit modulesChanged( m_moduleChooser->modules() );

  KDialogBase::slotOk();
}

/****************************/
CRangeChooserDialog::RangeItem::RangeItem(QListView* view, QListViewItem* afterThis, const QString caption, const QString range) : KListViewItem(view, afterThis) {
  setCaption(caption);
  setRange(range);
};

CRangeChooserDialog::RangeItem::~RangeItem() {

};

const QString& CRangeChooserDialog::RangeItem::range() {
  return m_range;
};

void CRangeChooserDialog::RangeItem::setRange(QString newRange) {
  m_range = newRange;
};

const QString CRangeChooserDialog::RangeItem::caption() {
  return text(0);
};

void CRangeChooserDialog::RangeItem::setCaption(const QString newCaption) {
  setText(0,newCaption);
};


/**************************/
CRangeChooserDialog::CRangeChooserDialog( QWidget* parentDialog ) : KDialogBase(Plain, i18n("Edit search ranges..."), Default | Ok | Cancel, Ok, parentDialog, "CRangeChooserDialog", false, true) {
  initView();
  initConnections();
  
  //add the existing scopes
  CBTConfig::StringMap map = CBTConfig::get(CBTConfig::searchScopes);
  CBTConfig::StringMap::Iterator it;
  for (it = map.begin(); it != map.end(); ++it) {
    new RangeItem(m_rangeList, 0, it.key(), it.data());
  };

  editRange(0);
  if (RangeItem* i = dynamic_cast<RangeItem*>(m_rangeList->currentItem()))
    nameChanged(i->caption());
};

CRangeChooserDialog::~CRangeChooserDialog() {
};

/** Initializes the view of this object. */
void CRangeChooserDialog::initView(){
//  setButtonOKText(i18n(""));
  
  QGridLayout* grid = new QGridLayout(plainPage(),6,5,0,3);

  m_rangeList = new KListView(plainPage());
  m_rangeList->addColumn(i18n("Search range"));
  m_rangeList->setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Expanding));
  m_rangeList->setFullWidth(true);
  m_rangeList->setSorting(0, true);
  m_rangeList->header()->setClickEnabled(false);
  m_rangeList->header()->setMovingEnabled(false);
  grid->addMultiCellWidget(m_rangeList,0,4,0,1);

  m_newRangeButton = new QPushButton(i18n("Add new range"),plainPage());
  connect(m_newRangeButton, SIGNAL(clicked()), this, SLOT(addNewRange()));
  grid->addWidget(m_newRangeButton,5,0);

  m_deleteRangeButton = new QPushButton(i18n("Delete current range"),plainPage());
  connect(m_deleteRangeButton, SIGNAL(clicked()), this, SLOT(deleteCurrentRange()));
  grid->addWidget(m_deleteRangeButton,5,1);
    
  grid->addColSpacing(2, 5);
  
  QLabel* label = new QLabel(i18n("Name:"), plainPage());
  m_nameEdit = new QLineEdit(plainPage());
  grid->addWidget(label,0,3);
  grid->addWidget(m_nameEdit,0,4);

  label = new QLabel(i18n("Edit the search range:"), plainPage());
  label->setFixedSize(label->sizeHint());
  m_rangeEdit = new QTextEdit(plainPage());
  grid->addMultiCellWidget(label,1,1,3,4);
  grid->addMultiCellWidget(m_rangeEdit,2,2,3,4);

  grid->addRowSpacing(3, 10);
  
  m_resultList = new KListView(plainPage());
  m_resultList->addColumn(i18n("Parsed search range:"));
  m_resultList->setFullWidth(true);
  m_resultList->setSorting(-1);
  m_resultList->setShowSortIndicator(false);  
  m_resultList->header()->setClickEnabled(false);
  m_resultList->header()->setMovingEnabled(false);
  m_resultList->setSelectionModeExt(KListView::NoSelection);
  
  grid->addMultiCellWidget(m_resultList, 4,5,3,4);

  grid->setRowStretch(4,5);
}

/** Initializes the connections of this widget. */
void CRangeChooserDialog::initConnections(){
  connect(m_rangeList, SIGNAL(selectionChanged(QListViewItem*)),
    this, SLOT(editRange(QListViewItem*)));

  connect(m_rangeEdit, SIGNAL(textChanged()),
    this, SLOT(parseRange()));
  connect(m_rangeEdit, SIGNAL(textChanged()),
    this, SLOT(rangeChanged()));

  connect(m_nameEdit, SIGNAL(textChanged(const QString&)),
    this, SLOT(nameChanged(const QString&)));
}

/** Adds a new range to the list. */
void CRangeChooserDialog::addNewRange(){
  RangeItem* i = new RangeItem(m_rangeList, m_rangeList->lastItem(), i18n("New range"));
  m_rangeList->setSelected(i, true);
  m_rangeList->setCurrentItem(i);  
  editRange(i);

  m_nameEdit->setFocus();
}

/** No descriptions */
void CRangeChooserDialog::editRange(QListViewItem* item){
  m_nameEdit->setEnabled( dynamic_cast<RangeItem*>(item) ); //ony if an item is selected enable the edit part
  m_rangeEdit->setEnabled( dynamic_cast<RangeItem*>(item) );
  m_resultList->setEnabled( dynamic_cast<RangeItem*>(item) );
  m_deleteRangeButton->setEnabled( dynamic_cast<RangeItem*>(item) );
  
  if (RangeItem* i = dynamic_cast<RangeItem*>(item)) {
    m_nameEdit->setText(i->caption());
    m_rangeEdit->setText(i->range());
  }
}

/** Parses the entered text and prints out the result in the list box below the edit area. */
void CRangeChooserDialog::parseRange(){
  m_resultList->clear();
  
  sword::VerseKey key;
  sword::ListKey verses = key.ParseVerseList((const char*)m_rangeEdit->text().local8Bit(), key, true);
	for (int i = 0; i < verses.Count(); ++i) {
		sword::VerseKey* element = dynamic_cast<sword::VerseKey*>(verses.GetElement(i));
		if (element)
      new KListViewItem(m_resultList,QString::fromLatin1("%1 - %2").arg(QString::fromLocal8Bit((const char*)element->LowerBound())).arg(QString::fromLocal8Bit((const char*)element->UpperBound())));
		else
      new KListViewItem(m_resultList,QString::fromLocal8Bit((const char*)*verses.GetElement(i)));
	}

}

/** No descriptions */
void CRangeChooserDialog::rangeChanged(){
  if (RangeItem* i = dynamic_cast<RangeItem*>(m_rangeList->currentItem())) {
    i->setRange(m_rangeEdit->text());
  };
}

/** No descriptions */
void CRangeChooserDialog::nameChanged(const QString& newCaption){
  m_rangeEdit->setEnabled(!newCaption.isEmpty());
  m_resultList->setEnabled(!newCaption.isEmpty());
  m_resultList->header()->setEnabled(!newCaption.isEmpty());  

  if (RangeItem* i = dynamic_cast<RangeItem*>(m_rangeList->currentItem())) {
    if (!newCaption.isEmpty()) {
        m_newRangeButton->setEnabled(true);
        i->setCaption(newCaption);        
        m_rangeList->sort();
    }
    else { //invalid name
      i->setCaption(i18n("<invalid name of search range>"));
      m_newRangeButton->setEnabled(false);
    };
  };
}

/** Deletes the selected range. */
void CRangeChooserDialog::deleteCurrentRange(){
  if (RangeItem* i = dynamic_cast<RangeItem*>(m_rangeList->currentItem())) {
    if (QListViewItem* selection = i->itemBelow() ? i->itemBelow() : i->itemAbove()) {
      m_rangeList->setSelected(selection, true);
      m_rangeList->setCurrentItem(selection);
    }
    else {
      m_rangeList->setSelected(m_rangeList->firstChild(), true);
      m_rangeList->setCurrentItem(m_rangeList->firstChild());      
    }
    delete i;
  }
  editRange(m_rangeList->currentItem()); 
}

void CRangeChooserDialog::slotOk(){
  //save the new map of search scopes
  CBTConfig::StringMap map;
  QListViewItemIterator it( m_rangeList );
  for (;it.current(); ++it) {
    if ( RangeItem* i = dynamic_cast<RangeItem*>(it.current()) ){
      map[i->caption()] = i->range();
    };
  };
  CBTConfig::set(CBTConfig::searchScopes, map);

  KDialogBase::slotOk();
}
  
void CRangeChooserDialog::slotDefault(){
  m_rangeList->clear();
  CBTConfig::StringMap map = CBTConfig::getDefault(CBTConfig::searchScopes);
  CBTConfig::StringMap::Iterator it;
  for (it = map.begin(); it != map.end(); ++it) {
    new RangeItem(m_rangeList, 0, it.key(), it.data());
  };
  m_rangeList->setSelected(m_rangeList->selectedItem(), false);
  m_rangeList->setCurrentItem(0);  
  
  editRange(0);
  if (RangeItem* i = dynamic_cast<RangeItem*>(m_rangeList->currentItem()))
    nameChanged(i->caption());

  KDialogBase::slotDefault();  
}

/****************************/

CSearchAnalysisDialog::CSearchAnalysisDialog( ListCSwordModuleInfo modules, QWidget* parentDialog ) : KDialogBase(Plain, i18n("Search analysis"), Close, Close, parentDialog, 0, true){
  initView();
  initConnections();
  m_analysis->reset();
  m_analysis->analyse(modules);
  showMaximized();  
};

CSearchAnalysisDialog::~CSearchAnalysisDialog() {

};

/** Initializes this dialog. */
void CSearchAnalysisDialog::initView(){
  QVBoxLayout* layout = new QVBoxLayout(plainPage(),0);

  QPushButton* button = new QPushButton(plainPage(), "button");
  button->setIconSet(SmallIconSet("filesave"));
  button->setText(i18n("Save search analysis as HTML")+"...");
  button->setFixedSize(button->sizeHint());
  layout->addWidget(button);
  layout->addSpacing(10);

  m_analysis = new CSearchAnalysis(plainPage());
  m_analysisView = new CSearchAnalysisView(m_analysis, plainPage());
  m_analysisView->show();
  layout->addWidget(m_analysisView);

  connect(button, SIGNAL(clicked()), m_analysis, SLOT(saveAsHTML()));  
}

/** Initializes the widgets SIGNAL and SLOT connections,. */
void CSearchAnalysisDialog::initConnections(){
}

/****************************/
/*  CSearchAnalysis         */
/****************************/

CSearchAnalysis::CSearchAnalysis(QObject *parent, const char *name )
	: QCanvas(parent,name) {

  m_scaleFactor = 0.0;
  m_legend = 0;
  setBackgroundColor(Qt::white);	
  m_canvasItemList.resize(67);
  m_canvasItemList.setAutoDelete(true);
  resize(1,1);
  connect(this, SIGNAL(resized()), SLOT(slotResized()));
}

CSearchAnalysis::~CSearchAnalysis(){
}

QDict<CSearchAnalysisItem>* CSearchAnalysis::getSearchAnalysisItemList(){
  // Returns pointer to the search analysis items
  return &m_canvasItemList;
}

/** Starts the analysis of the search result. This should be called only once because QCanvas handles the updates automatically. */
void CSearchAnalysis::analyse(ListCSwordModuleInfo modules){
//	qDebug("void CSearchAnalysis::analyse()");
  /**
	* Steps of analysing our search result;
	*	-Create the items for all available books ("Genesis" - "Revelation")
	* -Iterate through all modules we analyse
	*		-Go through all books of this module
	*			-Find out how many times we found the book
	*			-Set the count to the items which belongs to the book
	*/
  qWarning("CSearchAnalysis::analyse(ListCSwordModuleInfo with %i modules", modules.count());
  setModules(modules);
  qWarning( "moules count: %i", m_moduleList.count() );

	m_lastPosList.clear();		
	const int numberOfModules = m_moduleList.count();
	if (!numberOfModules)
		return;	
	m_legend = new CSearchAnalysisLegendItem(this, &m_moduleList);	
	m_legend->setX(LEFT_BORDER);
	m_legend->setY(UPPER_BORDER);
	m_legend->setSize(LEGEND_WIDTH,
	           LEGEND_INNER_BORDER*2 + ITEM_TEXT_SIZE*numberOfModules + LEGEND_DELTAY*(numberOfModules-1));
  m_legend->show();	

  int xPos = LEFT_BORDER + m_legend->width() + SPACE_BETWEEN_PARTS;			
	int moduleIndex = 0;	
	m_maxCount = 0;	
	int count = 0;
	CSwordVerseKey key(0);	
	key.key("Genesis 1:1");	
	
	CSearchAnalysisItem* analysisItem = m_canvasItemList[key.book()];
	bool ok = true;
	while (ok && analysisItem) {
		for (moduleIndex = 0,m_moduleList.first(); m_moduleList.current(); m_moduleList.next(),++moduleIndex) {
			KApplication::kApplication()->processEvents(10);
			if (!m_lastPosList.contains(m_moduleList.current()))
				m_lastPosList.insert(m_moduleList.current(),0);
			analysisItem->setCountForModule(moduleIndex, (count = getCount(key.book(),m_moduleList.current())));
			m_maxCount = (count > m_maxCount) ? count : m_maxCount;
		}
		analysisItem->setX(xPos);
		analysisItem->setY(UPPER_BORDER);
		analysisItem->show();
		
		xPos += (int)analysisItem->width() + SPACE_BETWEEN_PARTS;
		ok = key.next(CSwordVerseKey::UseBook);		
   	analysisItem = m_canvasItemList[key.book()];
	}
	resize(xPos+BAR_WIDTH+(m_moduleList.count()-1)*BAR_DELTAX+RIGHT_BORDER, height() );	
	slotResized();
}

/** Sets te module list used for the analysis. */
void CSearchAnalysis::setModules(ListCSwordModuleInfo modules){
//	ASSERT(modules);
	m_moduleList.clear();
	for (modules.first(); modules.current(); modules.next()) {
		if ( (modules.current()->type() == CSwordModuleInfo::Bible) || (modules.current()->type() == CSwordModuleInfo::Commentary) ) { //a Bible or an commentary
			m_moduleList.append(modules.current());
    }
  }

	m_canvasItemList.clear();
	CSearchAnalysisItem* analysisItem = 0;	
	CSwordVerseKey key(0);	
	key.key("Genesis 1:1");
	do {
   	analysisItem = new CSearchAnalysisItem(this, m_moduleList.count(), key.book(), &m_scaleFactor, &m_moduleList);
   	analysisItem->hide();
		m_canvasItemList.insert(key.book(), analysisItem);
	} while (key.next(CSwordVerseKey::UseBook));
	update();
}

/** Sets back the items and deletes things to cleanup */
void CSearchAnalysis::reset(){
	m_scaleFactor = 0.0;
  QDictIterator<CSearchAnalysisItem> it( m_canvasItemList ); // iterator for items
	while ( it.current() ) {
		it.current()->hide();
		++it;
	}	
	m_lastPosList.clear();	
	
	if (m_legend)
		m_legend->hide();
	delete m_legend;
	m_legend = 0;	
	update();
}

/** No descriptions */
void CSearchAnalysis::slotResized(){
	m_scaleFactor = (double)( (double)(height()-UPPER_BORDER-LOWER_BORDER-BAR_LOWER_BORDER-(m_moduleList.count()-1)*BAR_DELTAY)
	                                    /(double)m_maxCount);	
	QDictIterator<CSearchAnalysisItem> it( m_canvasItemList );
	while ( it.current() ) {
		it.current()->setSize(BAR_WIDTH + (m_moduleList.count()-1)*BAR_DELTAX, height()-UPPER_BORDER-LOWER_BORDER);
		it.current()->setY(UPPER_BORDER);
  	++it;
	}
	update();
}

/** This function returns a color for each module */
QColor CSearchAnalysis::getColor(int index){
  switch (index){
    case  0: return Qt::red;
    case 	1: return Qt::darkGreen;
    case  2: return Qt::blue;
    case  3: return Qt::cyan;
    case  4: return Qt::magenta;
    case  5: return Qt::darkRed;
    case  6: return Qt::darkGray;
    case  7: return Qt::black;
    case  8: return Qt::darkCyan;
    case  9: return Qt::darkMagenta;
    default: return Qt::red;
  }
}

/** Returns the count of the book in the module */
const unsigned int CSearchAnalysis::getCount( const QString book, CSwordModuleInfo* module ){
	sword::ListKey& result = module->searchResult();
	const int length = book.length();	
	unsigned int i = m_lastPosList[module];
	unsigned int count = 0;
	const unsigned int resultCount = result.Count();
	while (i < resultCount) {
		if ( strncmp(book.local8Bit(), (const char*)*result.GetElement(i), length) )		
			break;
		i++;
		++count;		
	}
	m_lastPosList.contains(module) ? m_lastPosList.replace(module,i) : m_lastPosList.insert(module,i);
//	m_lastPosList.replace(module,i);
	return count;
}


//------------------------------------------------------------------
//------------------------------------------------------------------

CSearchAnalysisItem::CSearchAnalysisItem(QCanvas *parent, const int moduleCount, const QString &bookname, double *scaleFactor, ListCSwordModuleInfo* modules)
	: QCanvasRectangle(parent),
	m_moduleList( modules ),
	m_scaleFactor(scaleFactor),
	m_bookName(bookname),
	m_moduleCount(moduleCount), 	
	m_bufferPixmap(0)
{	
 	m_resultCountArray.resize(m_moduleCount);
 	int index = 0;
 	for (index = 0; index < m_moduleCount; ++index)
 		m_resultCountArray[index] = 0;
}

CSearchAnalysisItem::~CSearchAnalysisItem() {
//	qWarning("CSearchAnalysisItem::~CSearchAnalysisItem()");
//	if (m_bufferPixmap)
		delete m_bufferPixmap;
}

/** Sets the resultcount of this item for the given module */
void CSearchAnalysisItem::setCountForModule( const int moduleIndex, const int count) {
	m_resultCountArray[moduleIndex] = count;
}

/** Returns the resultcount of this item for the given module */
int CSearchAnalysisItem::getCountForModule( const int moduleIndex) {
	return m_resultCountArray[moduleIndex];
}

/** Reimplementation. Draws the content of this item. */
void CSearchAnalysisItem::draw(QPainter& painter) {
	QFont f = painter.font();
	f.setPointSize(ITEM_TEXT_SIZE);
	painter.setFont(f);
	
	setPen(QPen(black,1));
	setBrush(Qt::red);
  /**	
	* We have to paint so many bars as we have modules available (we use m_moduleCount)
	* We paint inside the area which is given by height and widt of this rectangle item
	*/	
	int index = 0;	
	int drawn = 0;
	int Value = 0;
	
	//find out the biggest value
	for (index=0;index < m_moduleCount; index++)
	  if (m_resultCountArray[index] > Value)
	    Value = m_resultCountArray[index];
	
	while (drawn < m_moduleCount){
    for (index=0; index < m_moduleCount; index++){
      if (m_resultCountArray[index] == Value){
    		QPoint p1(x()+(m_moduleCount-drawn-1)*BAR_DELTAX,
    		          height()+y()-BAR_LOWER_BORDER-(m_moduleCount-drawn)*BAR_DELTAY);
    		QPoint p2(p1.x() + BAR_WIDTH,
    		          p1.y() - (!m_resultCountArray[index] ? 0 : ((m_resultCountArray[index])*(*m_scaleFactor))) );
    		QRect r(p1, p2);
    		painter.fillRect(r, QBrush(CSearchAnalysis::getColor(index)) );
    		painter.drawRect(r);
    		drawn++;
      }
    }
    //finds the next smaller value
 	  int newValue = 0;
 		for (index=0;index < m_moduleCount; index++)
   	  if (m_resultCountArray[index] < Value && m_resultCountArray[index] >= newValue)
   	    newValue = m_resultCountArray[index];
   	Value = newValue;
	}		
	if (!m_bufferPixmap) {
		m_bufferPixmap = new QPixmap();
		m_bufferPixmap->resize(width(),BAR_LOWER_BORDER);
		m_bufferPixmap->fill();		
		QPainter p(m_bufferPixmap);				
		f = p.font();
		f.setPointSize(ITEM_TEXT_SIZE);
		p.setFont(f);		
		p.rotate(90);
		p.drawText(QPoint(5,0), m_bookName);
	}
	painter.drawPixmap(QPoint(x(),height()+y()-BAR_LOWER_BORDER), *m_bufferPixmap);
}

/** Returns the width of this item. */
int CSearchAnalysisItem::width(){
	return m_moduleCount*(m_moduleCount>1 ? BAR_DELTAX : 0) + BAR_WIDTH;	
}

/** Returns the tooltip for this item. */
const QString CSearchAnalysisItem::getToolTip(){
	QString ret = QString::fromLatin1("<CENTER><B>%1</B></CENTER><HR>").arg(m_bookName);
	ret += "<TABLE CELLSPACING=\"0\" CELLPADDING=\"3\" WIDTH=\"100%\" HEIGHT=\"100%\" ALIGN=\"center\">";
	for (int i = 0; i < m_moduleCount; ++i) {
		CSwordModuleInfo* info = m_moduleList->at(i);
		const QColor c = CSearchAnalysis::getColor(i);
		ret.append(
			QString::fromLatin1("<TR BGCOLOR=\"white\"><TD><B><FONT COLOR=\"#%1\">%2</FONT></B></TD><TD>%3 (%4%)</TD></TR>")
				.arg(QString().sprintf("%02X%02X%02X",c.red(),c.green(),c.blue()))
				.arg(info ? info->name() : QString::null)
				.arg(m_resultCountArray[i])
        .arg((info && m_resultCountArray[i])? ((double)m_resultCountArray[i] / (double)info->searchResult().Count())*(double)100 : 0.0, 0, 'g', 2)
		);
	}
	ret += "</TABLE>";
	return ret;
}

//------------------------------------------------------------------
//------------------------------------------------------------------

CSearchAnalysisView::CSearchAnalysisView(QCanvas* canvas, QWidget* parent)
	: QCanvasView(canvas, parent) {
//	QWhatsThis::add(this, WT_SD_ANALYSIS_VIEW);
	setFocusPolicy(QWidget::WheelFocus);
	m_toolTip = new ToolTip(this);	
	resize(sizeHint());

}

/** Returns the sizeHint for this view */
QSize CSearchAnalysisView::sizeHint(){
	if ( parentWidget() )
		return parentWidget()->sizeHint();
	return QCanvasView::sizeHint();
}

/** No descriptions */
void CSearchAnalysisView::resizeEvent( QResizeEvent* e){
	QCanvasView::resizeEvent(e);
	canvas()->resize( canvas()->width(), viewport()->height() );
}

CSearchAnalysisView::ToolTip::ToolTip(QWidget* parent) : QToolTip(parent) {
}

void CSearchAnalysisView::ToolTip::maybeTip(const QPoint& p) {
	CSearchAnalysisView* view = dynamic_cast<CSearchAnalysisView*>(parentWidget());
	if (!view)
		return;
	QPoint point(p);
	point = view->viewport()->mapFrom(view, point);
	CSearchAnalysisItem* i = view->itemAt( view->viewportToContents(point) );
	if (!i)
		return;
				
	//get type of item and display correct text
	QString text = i->getToolTip();
	if (text.isEmpty())
		return;
	
	QPoint p1 = view->viewport()->mapTo(view, view->contentsToViewport(i->rect().topLeft()));
	p1.setY(0);	
	QPoint p2 = view->viewport()->mapTo(view, view->contentsToViewport(i->rect().bottomRight()));
	p2.setY(view->height());	
	QRect r = QRect( p1, p2 );	
	if (r.contains(p))
		tip(r, text);
}


/** Returns the item at position p. If there no item at that point return 0. */
CSearchAnalysisItem* CSearchAnalysisView::itemAt( const QPoint& p ){
	QCanvasItemList l = canvas()->collisions(p);
	if (!l.count())
		return 0;
	return dynamic_cast<CSearchAnalysisItem*>(l.first());	
}

//------------------------------------------------------------------
//------------------------------------------------------------------

CSearchAnalysisLegendItem::CSearchAnalysisLegendItem(QCanvas *parent, ListCSwordModuleInfo *list )
	: QCanvasRectangle(parent) {
	m_moduleList = list;
}

/** Reimplementation. Draws the content of this item. */
void CSearchAnalysisLegendItem::draw (QPainter& painter) {	
 	painter.save();
 		
	setPen( QPen(black,2) );
	setBrush( Qt::white );
  //the outer rectangle
  QPoint p1(x(),y());
  QPoint p2(x()+width(), y()+height());
  QRect r(p1, p2);
  r.normalize();
 	painter.drawRect(r);
 	
  QFont f = painter.font();
  f.setPointSize(ITEM_TEXT_SIZE);
  painter.setFont(f);
 	
 	for (unsigned int index=0; index < m_moduleList->count(); index++){
 	  // the module color indicators
 	  QPoint p1(x()+LEGEND_INNER_BORDER, y() + LEGEND_INNER_BORDER + index*(LEGEND_DELTAY + ITEM_TEXT_SIZE));
 	  QPoint p2( p1.x() + ITEM_TEXT_SIZE, p1.y() + ITEM_TEXT_SIZE);
 	  QRect r(p1,p2);
 		painter.fillRect(r, QBrush(CSearchAnalysis::getColor(index)) );
 		r.normalize();
 		painter.drawRect(r);
 		
 		QPoint p3( p2.x() + LEGEND_INNER_BORDER, p2.y() );
  	painter.drawText(p3, m_moduleList->at(index)->name() );
 	}
  painter.restore();
}

/** No descriptions */
void CSearchAnalysis::saveAsHTML(){
 	const QString file = KFileDialog::getSaveFileName(QString::null, QString::fromLatin1("*.html | %1").arg(i18n("HTML files")), 0, i18n("Save Search Analysis"));
	if (file.isNull()) {
    return;
  }
 	int moduleIndex = 0;
 	int count = 0;
 	QString countStr = "";
 	QString m_searchAnalysisHTML = "";
 	QString tableTitle = "";
  QString tableTotals = "";
 	QString VerseRange = "";
 	const QString txtCSS = QString::fromLatin1("<style type='text/css'>\nTD {border: thin solid black;}\nTH {font-size: 130%;text-align: left;vertical-align:top;}\n</style>\n");
 	CSwordVerseKey key(0);
 	sword::ListKey searchResult;

 	key.key("Genesis 1:1");

 	if (CSearchDialog::getSearchDialog()->searchScopeType() != CSwordModuleSearch::Scope_NoScope) { //a search scope was used
 		sword::ListKey verses = CSearchDialog::getSearchDialog()->searchScope();
 		for (int i = 0; i < verses.Count(); ++i) {
 			if ( sword::VerseKey* element = dynamic_cast<sword::VerseKey*>(verses.GetElement(i)) ) {
 				VerseRange += QString("%1 - %2")
          .arg(QString::fromLocal8Bit((const char*)element->LowerBound()))
          .arg(QString::fromLocal8Bit((const char*)element->UpperBound())) + "<br>";
 			}
 		}
 	}

 	CSearchAnalysisItem* analysisItem = m_canvasItemList.find( key.book() );

  QString text = "<html>\n<head>\n<title>" + i18n("BibleTime Search Analysis") + "</title>\n" + txtCSS + "</head>\n<body>\n";
 	text += "<table>\n<tr><th>" + i18n("Search Text :") + "</th><th>" + CSearchDialog::getSearchDialog()->searchText() + "</th></tr>\n";
 	text += "<tr><th>" + i18n("Search Type :") + "</th><th>" + /*m_searchText->getSearchTypeString() +*/ "</th></tr>\n";
 	text += "<tr><th>" + i18n("Search Scope:") + "</th><th>" + ((CSearchDialog::getSearchDialog()->searchScopeType() != CSwordModuleSearch::Scope_NoScope) ? VerseRange : "no") + "</th></tr>\n</table>\n<br>\n";


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
 		analysisItem = m_canvasItemList.find( key.book() );
 		for (moduleIndex = 0, m_moduleList.first(); m_moduleList.current(); m_moduleList.next(), ++moduleIndex) {
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

