/***************************************************************************
                          coptionsdialog.cpp  -  description
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

#include "cswordsetupdialog.h"

#include "backend/cswordbackend.h"
#include "backend/cswordmoduleinfo.h"

#include "util/cresmgr.h"
#include "util/ctoolclass.h"
#include "util/scoped_resource.h"


#include <iostream>
//#include <stdlib.h>

//QT includes
#include <qdir.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qwidgetstack.h>
#include <qfileinfo.h>
#include <qpushbutton.h>
#include <qlineedit.h>


//KDE includes
#include <kapplication.h>
#include <kconfig.h>
#include <kdirselectdialog.h>
#include <keditlistbox.h>
#include <klocale.h>
#include <kstandarddirs.h>
#include <kiconloader.h>
#include <kmessagebox.h>
#include <kprogress.h>
#include <kurl.h>

//Sword includes
#include <installmgr.h>
#include <swmodule.h>
#include <swversion.h>


using std::cout;
using std::cerr;
using std::endl;

using namespace sword;

CInstallSourcesMgrDialog::InstallSourceItem::InstallSourceItem( KListView* parent ) :
	QCheckListItem(parent, QString::null, QCheckListItem::CheckBoxController)
{

}

CInstallSourcesMgrDialog::InstallSourceItem::InstallSourceItem( KListView* parent, sword::InstallSource is ) : QCheckListItem(parent, QString::null, QCheckListItem::CheckBoxController)
{
	setCaption( QString::fromLatin1(is.caption.c_str()) );
	m_url.setHost( QString::fromLatin1(is.source.c_str()) );
	m_url.setPath( QString::fromLatin1(is.directory.c_str()) );
	setEnabled(true);

	updateItem();
}

const QUrl& CInstallSourcesMgrDialog::InstallSourceItem::url() const {
	return m_url;
}


void CInstallSourcesMgrDialog::InstallSourceItem::setURL(const QUrl& url) {
	m_url = url;
	updateItem();
}

const QString& CInstallSourcesMgrDialog::InstallSourceItem::caption() const {
	return m_caption;
}

void CInstallSourcesMgrDialog::InstallSourceItem::setCaption( const QString& caption ) {
	if (caption.isEmpty())
		return;

	m_caption = caption;
	updateItem();
}

const QString CInstallSourcesMgrDialog::InstallSourceItem::server() const {
	return m_url.host();
}

void CInstallSourcesMgrDialog::InstallSourceItem::setServer( const QString& server ) {
	if (server.isEmpty())
		return;

	m_url.setHost(server);
	updateItem();
}

const QString CInstallSourcesMgrDialog::InstallSourceItem::path() const {
	return m_url.path();
}

void CInstallSourcesMgrDialog::InstallSourceItem::setPath( const QString& path ) {
	if (path.isEmpty())
		return;

	m_url.setPath(path);
	updateItem();
}

const bool CInstallSourcesMgrDialog::InstallSourceItem::isEnabled() const {
	return isOn();
}

void CInstallSourcesMgrDialog::InstallSourceItem::setEnabled( const bool enabled ) {
	setOn(enabled);
	updateItem();
}

void CInstallSourcesMgrDialog::InstallSourceItem::updateItem() {
	setText(0, m_caption);
	setText(1, m_url.host() + m_url.path());
}

sword::InstallSource CInstallSourcesMgrDialog::InstallSourceItem::swordInstallSource() {
	sword::InstallSource src("FTP");
	src.caption = m_caption.latin1();
	src.source = m_url.host().latin1();
	src.directory = m_url.path().latin1();

	return src;
}

CInstallSourcesMgrDialog::CInstallSourcesMgrDialog(QWidget *parent, const char *name )
	: KDialogBase(IconList, i18n("Manage installation sources"), Ok, Ok, parent, name, true, true, QString::null, QString::null, QString::null) {

	initLocalSourcesPage();
	initRemoteSourcesPage();
}

void CInstallSourcesMgrDialog::initView() {

}

void CInstallSourcesMgrDialog::slotOk() {
	//save the source items to disk

	//save local sources


	//save remote sources
	BTInstallMgr::Tool::RemoteConfig::resetSources(); //we wan't to overwrite old sources, not add to them
	QListViewItemIterator it(m_remoteSourcesList, QListViewItemIterator::Checked );
	while (it.current()) {
		InstallSourceItem* item = dynamic_cast<InstallSourceItem*>(it.current());
		if (!item)
			continue;

		sword::InstallSource is = item->swordInstallSource();
		BTInstallMgr::Tool::RemoteConfig::addSource( &is );
		++it; //next checked item
	}

	KDialogBase::slotOk();
}

void CInstallSourcesMgrDialog::initLocalSourcesPage() {
	m_localSourcesPage = addPage(i18n("Local sources"), QString::null, DesktopIcon("dir",32));
 	m_localSourcesPage->setMinimumSize(500,400);

	QGridLayout* grid = new QGridLayout(m_localSourcesPage, 3,3, 5, 5);

	m_localSourcesList = new KListView( m_localSourcesPage );
	m_localSourcesList->addColumn("Local sources");
	QPushButton* addButton = new QPushButton(i18n("Add new directory"), m_localSourcesPage);
	connect(addButton, SIGNAL(clicked()), SLOT(slot_localAddSource()));

	QPushButton* removeButton = new QPushButton(i18n("Remove directory"), m_localSourcesPage);
	connect(removeButton, SIGNAL(clicked()), SLOT(slot_localRemoveSource()));

	grid->addMultiCellWidget( m_localSourcesList, 0,2, 0,1 );
	grid->setColStretch(0, 5);
	grid->setColStretch(1, 5);

	grid->addWidget( addButton, 0,2 );
	grid->addWidget( removeButton, 1,2 );
	grid->setColStretch(2, 0);
}

void CInstallSourcesMgrDialog::slot_localAddSource() {
	//Add a new dir to the list.
  KURL url = KDirSelectDialog::selectDirectory(QString::null, true);
  if (url.isValid()) {
		new KListViewItem(m_localSourcesList, url.path());
  }
}

void CInstallSourcesMgrDialog::slot_localRemoveSource() {
	if (m_localSourcesList->currentItem())
		delete m_localSourcesList->currentItem();
}

void CInstallSourcesMgrDialog::initRemoteSourcesPage() {
	m_remoteSourcesPage = addPage(i18n("Remote sources"), QString::null, DesktopIcon("html",32));
 	m_remoteSourcesPage->setMinimumSize(500,400);

	QGridLayout* grid = new QGridLayout(m_remoteSourcesPage, 4,5, 5,5);

	m_remoteSourcesList = new KListView( m_remoteSourcesPage );
	m_remoteSourcesList->setAllColumnsShowFocus(true);
	m_remoteSourcesList->addColumn("Name");
	m_remoteSourcesList->addColumn("URL");
	connect(m_remoteSourcesList, SIGNAL(selectionChanged()), SLOT(slot_remoteSourceSelectionChanged()));

 	QPushButton* addButton = new QPushButton(i18n("New"), m_remoteSourcesPage);
	connect(addButton, SIGNAL(clicked()), SLOT(slot_remoteAddSource()));

// 	QPushButton* editButton = new QPushButton(i18n("Edit"), m_remoteSourcesPage);
//	connect(editButton, SIGNAL(clicked()), SLOT(slot_remoteChangeSource()));

	QPushButton* removeButton = new QPushButton(i18n("Remove"), m_remoteSourcesPage);
	connect(removeButton, SIGNAL(clicked()), SLOT(slot_remoteRemoveSource()));

	grid->addMultiCellWidget( m_remoteSourcesList, 0,2, 0,2 );
	grid->addWidget( addButton, 3,0 );
	//grid->addWidget( editButton, 3,1 );
	grid->addWidget( removeButton, 3,2 );

	grid->setColStretch(0, 0);
	grid->setColStretch(1, 0);
	grid->setColStretch(2, 0);
	grid->setColStretch(3, 5);

	//contains the remote sources edit controls, we need boxes for the caption, the server, the dir on the server
	QGroupBox* box = new QGroupBox(m_remoteSourcesPage);
	grid->addMultiCellWidget( box, 0,3, 3,4 );

	QGridLayout* boxGrid = new QGridLayout(box, 4, 3, 5, 5);
	boxGrid->setColStretch(1, 5);

	boxGrid->addWidget(new QLabel(i18n("Name:"), box), 0,0);

	m_remoteCaptionEdit = new QLineEdit(box);
	connect(m_remoteCaptionEdit, SIGNAL(textChanged(const QString&)),
		SLOT(slot_remoteCaptionChanged(const QString&)));
	boxGrid->addWidget(m_remoteCaptionEdit, 0,1);


	m_remoteServerEdit = new QLineEdit(box);;
	connect(m_remoteServerEdit, SIGNAL(textChanged(const QString&)),
		SLOT(slot_remoteServerChanged(const QString&)));
	boxGrid->addWidget(new QLabel(i18n("Server:"), box), 1,0);
	boxGrid->addWidget(m_remoteServerEdit, 1,1);

	m_remotePathEdit = new QLineEdit(box);;
	connect(m_remotePathEdit, SIGNAL(textChanged(const QString&)),
		SLOT(slot_remotePathChanged(const QString&)));
	boxGrid->addWidget(new QLabel(i18n("Path on server:"), box), 2,0);
	boxGrid->addWidget(m_remotePathEdit, 2,1);


	//now setup some reasonable default server entries

	//noe insert existing remote source items, if there are no sources setup with default source
	BTInstallMgr mgr;
	QStringList sources = BTInstallMgr::Tool::RemoteConfig::sourceList( &mgr );
	for (QStringList::iterator it = sources.begin(); it != sources.end(); ++it) {
		sword::InstallSource is = BTInstallMgr::Tool::RemoteConfig::source(&mgr, (*it).latin1());
		if (!BTInstallMgr::Tool::RemoteConfig::isRemoteSource( &is )) { //only use remote sources as items
			continue;
		}

		(void)new InstallSourceItem( m_remoteSourcesList, is );
	}

	if (m_remoteSourcesList->childCount() == 0) {
		InstallSourceItem* i = new InstallSourceItem(m_remoteSourcesList);
		i->setCaption("Crosswire");
		i->setURL(QUrl("ftp://ftp.crosswire.org/pub/sword/raw/"));
	}
}

void CInstallSourcesMgrDialog::slot_remoteAddSource() {
	(void)new CInstallSourcesMgrDialog::InstallSourceItem(m_remoteSourcesList);
}

void CInstallSourcesMgrDialog::slot_remoteChangeSource() {
}

void CInstallSourcesMgrDialog::slot_remoteRemoveSource() {
	delete m_remoteSourcesList->currentItem();
}

void CInstallSourcesMgrDialog::slot_remoteSourceSelectionChanged() {
	//Apply settings of new source to the edit widgets
	InstallSourceItem* i = dynamic_cast<InstallSourceItem*>(m_remoteSourcesList->currentItem());
	if (!i)
		return;

	m_remoteCaptionEdit->setText( i->caption() );
	m_remoteServerEdit->setText( i->server() );
	m_remotePathEdit->setText( i->path() );
}

void CInstallSourcesMgrDialog::slot_remoteCaptionChanged( const QString& t) {
	InstallSourceItem* i = dynamic_cast<InstallSourceItem*>(m_remoteSourcesList->currentItem());
	if (!i)
		return;

	i->setCaption(t);
}

void CInstallSourcesMgrDialog::slot_remoteServerChanged( const QString& t ) {
	InstallSourceItem* i = dynamic_cast<InstallSourceItem*>(m_remoteSourcesList->currentItem());
	if (!i)
		return;

	i->setServer(t);
}

void CInstallSourcesMgrDialog::slot_remotePathChanged( const QString& t) {
	InstallSourceItem* i = dynamic_cast<InstallSourceItem*>(m_remoteSourcesList->currentItem());
	if (!i)
		return;

	i->setPath(t);
}

/*******************************/
/* 									New class												*/
/******************************/


CSwordSetupDialog::CSwordSetupDialog(QWidget *parent, const char *name )
	: KDialogBase(IconList, i18n("Sword configuration"), Ok, Ok, parent, name, true, true, QString::null, QString::null, QString::null) {

  m_progressDialog = 0;
	setIconListAllVisible(true);
  m_refreshedRemoteSources = false;

  initSwordConfig();
	initInstall();
	initRemove();
}


void CSwordSetupDialog::initSwordConfig(){
	QFrame* page = m_swordConfigPage = addPage(i18n("Sword Path"), QString::null, DesktopIcon("bt_sword",32));
 	page->setMinimumSize(500,400);

	QGridLayout* layout = new QGridLayout(page, 6, 4);
	layout->setMargin(5);

	layout->setSpacing(10);
	layout->setColStretch(0,1);
	layout->setRowStretch(5,1);

	QLabel* mainLabel = CToolClass::explanationLabel(page,
		i18n("Configure Sword"),
		i18n("The underlying Sword software uses an own configuration file. This page let you set up this file.")
  );
	layout->addMultiCellWidget(mainLabel, 0, 0, 0, 3);


  QString swordConfPath = BTInstallMgr::Tool::LocalConfig::swordConfigFilename();
	QLabel* confPathLabel = new QLabel(QString::fromLatin1("Your Sword configuration file is <b>%1</b>").arg(swordConfPath), page);
	layout->addMultiCellWidget(confPathLabel, 1,1,0,3);

  m_swordPathListBox = new KListView(page);
  m_swordPathListBox->setFullWidth(true);
  m_swordPathListBox->addColumn(i18n("Path to Sword modules"));
  connect(m_swordPathListBox, SIGNAL(selectionChanged()), this, SLOT(slot_swordPathSelected()));
  layout->addMultiCellWidget(m_swordPathListBox, 2,5,0,1);
  
  m_swordEditPathButton = new QPushButton(i18n("Edit Entry"), page);
  connect(m_swordEditPathButton, SIGNAL(clicked()), this, SLOT(slot_swordEditClicked()));
  layout->addWidget(m_swordEditPathButton, 2, 3);

  m_swordAddPathButton = new QPushButton(i18n("Add Entry"), page);
  connect(m_swordAddPathButton, SIGNAL(clicked()), this, SLOT(slot_swordAddClicked()));
  layout->addWidget(m_swordAddPathButton, 3,3);

  m_swordRemovePathButton = new QPushButton(i18n("Remove Entry"), page);
  connect(m_swordRemovePathButton, SIGNAL(clicked()), this, SLOT(slot_swordRemoveClicked()));
  layout->addWidget(m_swordRemovePathButton, 4,3);


  setupSwordPathListBox();
}

void CSwordSetupDialog::initInstall(){
	QFrame* newpage = m_installPage = addPage(i18n("Install/Update Modules"), QString::null, DesktopIcon("connect_create",32));

	QVBoxLayout* vboxlayout = new QVBoxLayout(newpage);
	QHBoxLayout* hboxlayout = new QHBoxLayout();
  hboxlayout->setAutoAdd( true );

  vboxlayout->addLayout(hboxlayout);

	m_installWidgetStack = new QWidgetStack(newpage);
  hboxlayout->addWidget(m_installWidgetStack);

	m_installSourcePage = new QWidget(0);
  m_installWidgetStack->addWidget(m_installSourcePage);

	m_installSourcePage->setMinimumSize(500,400);

	QGridLayout* layout = new QGridLayout(m_installSourcePage, 8, 2);
	layout->setMargin(5);
	layout->setSpacing(10);
	layout->setRowStretch(6,5);

	QLabel* installLabel = CToolClass::explanationLabel(m_installSourcePage,
		i18n("Install/update modules - Step 1"),
		i18n("Please choose a source and a destination. After that step click on the connect button.")
  );
	layout->addMultiCellWidget(installLabel, 0,0,0,1);

	QLabel* sourceHeadingLabel = new QLabel("<b>Select source location</b>",m_installSourcePage);
	layout->addMultiCellWidget(sourceHeadingLabel, 1,1,0,1);

	m_sourceCombo = new QComboBox(m_installSourcePage);
	layout->addWidget(m_sourceCombo, 2, 0);

	QPushButton* maintainSourcesButton = new QPushButton(m_installSourcePage);
	maintainSourcesButton->setText("Maintain sources");
	connect(maintainSourcesButton, SIGNAL(clicked()), SLOT(slot_installManageSources()));
	layout->addWidget(maintainSourcesButton, 2, 1, Qt::AlignLeft);

	m_sourceLabel = new QLabel(m_installSourcePage);
	layout->addMultiCellWidget(m_sourceLabel, 3,3,0,1);

	QLabel* targetHeadingLabel = new QLabel("<b>Select target location</b>", m_installSourcePage);
	layout->addMultiCellWidget(targetHeadingLabel, 4,4,0,1);

	m_targetCombo = new QComboBox(m_installSourcePage);
	layout->addWidget(m_targetCombo, 5, 0);

	m_targetLabel = new QLabel(m_installSourcePage);
	layout->addMultiCellWidget(m_targetLabel, 6,6,0,1,Qt::AlignTop);

  QHBoxLayout* myHBox = new QHBoxLayout();
  vboxlayout->addLayout(myHBox);

  m_installBackButton = new QPushButton(newpage);
	m_installBackButton->setText(i18n("Back"));
	myHBox->addWidget(m_installBackButton, 7, 0);

  m_installContinueButton = new QPushButton(newpage);
	m_installContinueButton->setText( i18n("Connect to source") );
  connect(m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_connectToSource()));
	myHBox->addWidget(m_installContinueButton, 7, 1);

  m_installBackButton->setEnabled(false);

	connect(m_sourceCombo, SIGNAL( highlighted(const QString&) ), SLOT( slot_sourceSelected( const QString&) ));
	connect(m_targetCombo, SIGNAL( highlighted(const QString&) ), SLOT( slot_targetSelected( const QString&) ));
	populateInstallCombos();

  slot_sourceSelected( m_sourceCombo->currentText() );
}

void CSwordSetupDialog::initRemove(){
	QFrame* page = m_removePage = addPage(i18n("Remove Modules"), QString::null, DesktopIcon("editdelete",32));

	page->setMinimumSize(500,400);

	QGridLayout* layout = new QGridLayout(page, 4, 4);
	layout->setMargin(5);

	layout->setSpacing(10);
	layout->setColStretch(1,1);
	layout->setRowStretch(2,1);

	QLabel* mainLabel= CToolClass::explanationLabel(page,
		i18n("Remove installed module(s)"),
		i18n("This dialog lets you remove installed Sword modules from your system. Choose the modules and then click on the remove button.")
  );
	layout->addMultiCellWidget(mainLabel, 0, 0, 0, 3);

	QLabel* headingLabel = new QLabel(i18n("<b>Select modules to be uninstalled</b>"), page);
	layout->addMultiCellWidget(headingLabel, 1, 1, 0, 3);

	m_populateListNotification = new QLabel("", page);
	layout->addWidget(m_populateListNotification, 3, 2, Qt::AlignCenter);

	m_removeModuleListView = new KListView(page, "remove modules view");
	layout->addMultiCellWidget( m_removeModuleListView, 2,2,0,3);
	m_removeModuleListView->addColumn(i18n("Name"));
  m_removeModuleListView->addColumn(i18n("Location"));
 	m_removeModuleListView->setAllColumnsShowFocus(true);
 	m_removeModuleListView->setFullWidth(true);

  m_removeRemoveButton = new QPushButton(page);
	m_removeRemoveButton->setText( i18n("Remove selected module(s)") );
	layout->addWidget(m_removeRemoveButton, 3, 3, Qt::AlignRight);

//  m_removeBackButton = new QPushButton(page);
//	m_removeBackButton->setText( i18n("Back") );
//  m_removeBackButton->setEnabled(false);
//	layout->addWidget(m_removeBackButton, 3, 0, Qt::AlignRight);

//	connect(m_removeBackButton,   SIGNAL( clicked() ), m_main, SLOT( slot_backtoMainPage() ));
	connect(m_removeRemoveButton, SIGNAL( clicked() ), this, SLOT( slot_doRemoveModules() ));

	populateRemoveModuleListView();
}

/** Called if the OK button was clicked */
void CSwordSetupDialog::slotOk(){
  //save the Sword path configuration here
  if (m_swordPathListBox->childCount()) {
    QStringList targets;

    QListViewItemIterator it( m_swordPathListBox );
    while ( it.current() ) {
      QListViewItem *item = it.current();
      if (!item->text(0).isEmpty()) {
        targets << item->text(0);
      }
      ++it;
    }

    BTInstallMgr::Tool::LocalConfig::setTargetList(targets); //creates new Sword config
  }

  KDialogBase::slotOk();
  emit signalSwordSetupChanged( );
}

/*called if the apply button was clicked*/
void CSwordSetupDialog::slotApply(){
	KDialogBase::slotApply();
  emit signalSwordSetupChanged( );
}

/** Opens the page which contaisn the given part ID. */
const bool CSwordSetupDialog::showPart( CSwordSetupDialog::Parts ID, const bool exclusive ) {
//  if (exlusive) {
//    m_swordConfigPage->setEnabled(false);
//    m_installPage->setEnabled(false)
//    m_removePage->setEnabled(false)
//  }

  bool ret = false;
	switch (ID) {
		case CSwordSetupDialog::Sword:
      showPage( pageIndex(m_swordConfigPage->parentWidget()) );
//      if (exlusive) {
//        m_swordConfigPage->setEnabled(false);
//        m_installPage->setEnabled(false)
//        m_removePage->setEnabled(false)
//      }
			break;
		case CSwordSetupDialog::Install:
      showPage( pageIndex(m_installPage->parentWidget()));
//      if (exlusive) {
//        m_swordConfigPage->setEnabled(false);
//        m_installPage->setEnabled(false)
//        m_removePage->setEnabled(false)
//      }
			break;
		case CSwordSetupDialog::Remove:
      showPage( pageIndex(m_removePage->parentWidget()) );
//      if (exlusive) {
//        m_swordConfigPage->setEnabled(false);
//        m_installPage->setEnabled(false)
//        m_removePage->setEnabled(false)
//      }
			break;
		default:
			break;
	}
  return false;
}

/** No descriptions */
void CSwordSetupDialog::populateInstallCombos(){
	m_sourceCombo->clear();
	m_targetCombo->clear();

  BTInstallMgr::Tool::RemoteConfig::initConfig();
  BTInstallMgr mgr;

  QStringList list = BTInstallMgr::Tool::RemoteConfig::sourceList(&mgr);
  for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
		sword::InstallSource is = BTInstallMgr::Tool::RemoteConfig::source(&mgr, *it);
		if (BTInstallMgr::Tool::RemoteConfig::isRemoteSource(&is)) { //remote source?
    	m_sourceCombo->insertItem( i18n("[Remote]") + " " + *it );
		}
		else {
			m_sourceCombo->insertItem( i18n("[Local]") + " " + *it );
		}
  }

  list = BTInstallMgr::Tool::LocalConfig::targetList();
  for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
    m_targetCombo->insertItem( *it );
  }

	m_targetCombo->setEnabled( (m_targetCombo->count() > 0) );
	slot_sourceSelected( m_sourceCombo->currentText() );
}

/** No descriptions */
void CSwordSetupDialog::slot_sourceSelected(const QString &sourceName){
	//remove status parta
	QString source = sourceName;
	source = source.remove( i18n("[Local]") + " " );
	source = source.remove( i18n("[Remote]") + " " );

  BTInstallMgr mgr;

	// qWarning("%s schosen", source.latin1());

  QString url;
  sword::InstallSource is = BTInstallMgr::Tool::RemoteConfig::source(&mgr, source) ;

  if (BTInstallMgr::Tool::RemoteConfig::isRemoteSource(&is)) {
    url = QString::fromLatin1("ftp://%1%2").arg(is.source.c_str()).arg(is.directory.c_str());
  }
  else {
    url = QString::fromLatin1("%1").arg(is.directory.c_str());
  }
  m_sourceLabel->setText( url );

  m_refreshedRemoteSources = false;
}

/** No descriptions */
void CSwordSetupDialog::slot_targetSelected(const QString &targetName){
	m_targetLabel->setText( m_targetMap[targetName] );
  target = m_targetMap[targetName];
}

/** No descriptions */
void CSwordSetupDialog::slot_doRemoveModules(){
	QStringList moduleList;
	QListViewItem* item1 = 0;
	QListViewItem* item2 = 0;

	for (item1 = m_removeModuleListView->firstChild(); item1; item1 = item1->nextSibling())
		for (item2 = item1->firstChild(); item2; item2 = item2->nextSibling())
			if ( dynamic_cast<QCheckListItem*>(item2) && dynamic_cast<QCheckListItem*>(item2)->isOn() )
				moduleList << item2->text(0);

	QString catList;

	for ( QStringList::Iterator it = moduleList.begin(); it != moduleList.end(); ++it ) {
		if (!catList.isEmpty())
			catList += ", ";
     catList += *it;
  }
	QString message("You selected the following modules: %1.\n\n"
		"Do you really want to remove them from your system?");
	message = message.arg(catList);
	if (catList.isEmpty()){
		KMessageBox::error(0, i18n("No modules selected."), i18n("Error")) ;
	}
	else if ((KMessageBox::warningYesNo(0, message, i18n("Warning")) == KMessageBox::Yes)){  //Yes was pressed.
    //module are removed in this section of code
    sword::InstallMgr installMgr;

  	for ( QStringList::Iterator it = moduleList.begin(); it != moduleList.end(); ++it ) {
      if (CSwordModuleInfo* m = backend()->findModuleByName(*it)) { //module found?
        QString prefixPath = m->config(CSwordModuleInfo::AbsoluteDataPath) + "/";
        QString dataPath = m->config(CSwordModuleInfo::DataPath);
        if (dataPath.left(2) == "./") {
          dataPath = dataPath.mid(2);
        }
        if (prefixPath.contains(dataPath)) {
          prefixPath = prefixPath.replace( dataPath, "");
//          qWarning("removing module in prefix %s with data path %s", prefixPath.latin1(), dataPath.latin1());
        }
        else {
          prefixPath = QString::fromLatin1(backend()->prefixPath);
        }

        sword::SWMgr mgr(prefixPath.latin1());
        installMgr.removeModule(&mgr, m->name().latin1());
//       	qWarning("Removed module: [%s]" , m->name().latin1());
      }
    }

    //remove checklist items of removed modules
//   	for (item1 = m_removeModuleListView->firstChild(); item1; item1 = item1->nextSibling())
//  		for (item2 = item1->firstChild(); item2; item2 = item2->nextSibling())
//	  		if ( dynamic_cast<QCheckListItem*>(item2) && dynamic_cast<QCheckListItem*>(item2)->isOn() )
//		  		delete item2;
    populateRemoveModuleListView(); //rebuild the tree
  }
}

/** No descriptions */
void CSwordSetupDialog::populateRemoveModuleListView(){
//	m_removeBackButton->setEnabled(false);
	m_removeRemoveButton->setEnabled(false);

	CSwordBackend* m_backend = new CSwordBackend();
	KApplication::kApplication()->processEvents();
	m_backend->initModules();

	m_removeModuleListView->clear();

	KListViewItem* categoryBible = new KListViewItem(m_removeModuleListView, i18n("Bibles"));
	KListViewItem* categoryCommentary = new KListViewItem(m_removeModuleListView, i18n("Commentaries"));
	KListViewItem* categoryLexicon = new KListViewItem(m_removeModuleListView, i18n("Lexicons"));
	KListViewItem* categoryBook = new KListViewItem(m_removeModuleListView, i18n("Books"));

  categoryBible->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryCommentary->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryLexicon->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryBook->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));

  categoryBible->setOpen(true);
  categoryCommentary->setOpen(true);
  categoryLexicon->setOpen(true);
  categoryBook->setOpen(true);

	QPtrList<CSwordModuleInfo> list = m_backend->moduleList();
	int modcount = list.count();
	int mod = 0;
	QListViewItem* newItem = 0;
	QListViewItem* parent = 0;
	sword::SWConfig moduleConfig("");

	for ( list.first(), mod = 1; list.current(); list.next(), mod++ ){
		if (mod % 20){
			m_populateListNotification->setText(QString("Scanning your system: %1%").arg((mod*100)/modcount));
			KApplication::kApplication()->processEvents();
		}

		switch (list.current()->type()) {
			case CSwordModuleInfo::Bible:
        parent = categoryBible; break;
			case CSwordModuleInfo::Commentary:
        parent = categoryCommentary; break;
			case CSwordModuleInfo::Lexicon:
        parent = categoryLexicon; break;
			case CSwordModuleInfo::GenericBook:
        parent = categoryBook; break;
      default:
        parent = 0; //shouldn't happen;
        break;
		}

//		m_backend->moduleConfig(name, moduleConfig);
//		QFileInfo file(moduleConfig.filename.c_str());
//		if (file.isWritable()) //only writable modules can be removed
		newItem = new QCheckListItem(parent,list.current()->name(),QCheckListItem::CheckBox);
//		else
//			newItem = new QListViewItem(parent, name);

    newItem->setPixmap(0, CToolClass::getIconForModule(list.current()));
		newItem->setText(1,list.current()->config(CSwordModuleInfo::AbsoluteDataPath));
  }

	m_populateListNotification->setText("");
//	m_removeBackButton->setEnabled(true);
	m_removeRemoveButton->setEnabled(true);

  //clean up groups
  if (!categoryBible->childCount())
    delete categoryBible;
  if (!categoryCommentary->childCount())
    delete categoryCommentary;
  if (!categoryBook->childCount())
    delete categoryBook;
  if (!categoryLexicon->childCount())
    delete categoryLexicon;

}

/** No descriptions */
void CSwordSetupDialog::populateInstallModuleListView( const QString& sourceName ){
  KApplication::kApplication()->processEvents();
	m_installModuleListView->clear();

	QListViewItem* categoryBible = new KListViewItem(m_installModuleListView, i18n("Bibles"));
  categoryBible->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));

	QListViewItem* categoryCommentary = new KListViewItem(m_installModuleListView, i18n("Commentaries"));
  categoryCommentary->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));

	QListViewItem* categoryLexicon = new KListViewItem(m_installModuleListView, i18n("Lexicons"));
  categoryLexicon->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));

  QListViewItem* categoryBook = new KListViewItem(m_installModuleListView, i18n("Books"));
  categoryBook->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));

  categoryBible->setOpen(true);
  categoryCommentary->setOpen(true);
  categoryLexicon->setOpen(true);
  categoryBook->setOpen(true);

  BTInstallMgr iMgr;
  sword::InstallSource is = BTInstallMgr::Tool::RemoteConfig::source(&iMgr, sourceName);

  if (BTInstallMgr::Tool::RemoteConfig::isRemoteSource(&is)) {
    if (!m_refreshedRemoteSources)
      iMgr.refreshRemoteSource( &is );
    m_refreshedRemoteSources = true;
  }

  //kind of a hack to provide a pointer to mgr next line
  util::scoped_ptr<CSwordBackend> backend(BTInstallMgr::Tool::backend(&is) );
  Q_ASSERT(backend);
  if (!backend)
    return;

  QListViewItem* parent = 0;
  ListCSwordModuleInfo mods = backend->moduleList();
  for (CSwordModuleInfo* newModule = mods.first(); newModule; newModule = mods.next()) {
    bool isUpdate = false;
    CSwordModuleInfo* const installedModule = CPointers::backend()->findModuleByName(newModule->name());
    if (installedModule) { //module already installed?
      //check whether it's an uodated module or just the same
      const SWVersion installedVersion( installedModule->config(CSwordModuleInfo::ModuleVersion).latin1() );
      const SWVersion newVersion( newModule->config(CSwordModuleInfo::ModuleVersion).latin1() );
      isUpdate = (newVersion > installedVersion);
      if (!isUpdate)
        continue;
    }

    if (newModule->isLocked() || newModule->isEncrypted()) //encrypted modules have no data files on the server
      continue;
    
    switch (newModule->type()) {
      case CSwordModuleInfo::Bible:
        parent = categoryBible;
        break;
      case CSwordModuleInfo::Commentary:
        parent = categoryCommentary;
        break;
      case CSwordModuleInfo::Lexicon:
        parent = categoryLexicon;
        break;
      case CSwordModuleInfo::GenericBook:
        parent = categoryBook;
        break;
      default:
        parent = 0;
        break;
    }
    
		QListViewItem* newItem = 0;
    if (parent) {
       newItem = new QCheckListItem(parent, newModule->name(), QCheckListItem::CheckBox);      
    }
    else {
      newItem = new QCheckListItem(m_installModuleListView, newModule->name(), QCheckListItem::CheckBox);
    }
    
    newItem->setPixmap(0, CToolClass::getIconForModule(newModule));
    newItem->setText(1, installedModule ? installedModule->config(CSwordModuleInfo::ModuleVersion) : "");
    newItem->setText(2, newModule->config(CSwordModuleInfo::ModuleVersion));
    newItem->setText(3, isUpdate ? i18n("Updated") : i18n("New"));
  }

  //clean up groups
  if (!categoryBible->childCount())
    delete categoryBible;
  if (!categoryCommentary->childCount())
    delete categoryCommentary;
  if (!categoryBook->childCount())
    delete categoryBook;
  if (!categoryLexicon->childCount())
    delete categoryLexicon;
}

/** Connects to the chosen source. */
void CSwordSetupDialog::slot_connectToSource(){
	m_installModuleListPage = new QWidget(0);

	QGridLayout* layout = new QGridLayout(m_installModuleListPage, 8, 2);
	layout->setMargin(5);
	layout->setSpacing(10);
//	layout->setColStretch(1);

  QLabel* installLabel = CToolClass::explanationLabel(m_installModuleListPage,
		i18n("Install/update modules - Step 2"),
		i18n("Please choose the modules which should be installed / updated and click the install button.")
  );
	layout->addMultiCellWidget(installLabel, 0,0,0,1);

  m_installWidgetStack->addWidget(m_installModuleListPage);
	m_installModuleListPage->setMinimumSize(500,400);

  //insert a list box which contains all available remote modules
	m_installModuleListView = new KListView(m_installModuleListPage, "install modules view");
	layout->addMultiCellWidget( m_installModuleListView, 1,6,0,1);
	m_installModuleListView->addColumn("Name");
  m_installModuleListView->addColumn("Installed version");
  m_installModuleListView->addColumn("Remote version");
  m_installModuleListView->addColumn("Status");
 	m_installModuleListView->setAllColumnsShowFocus(true);
 	m_installModuleListView->setFullWidth(true);

  connect( m_installBackButton, SIGNAL(clicked()), this, SLOT(slot_showInstallSourcePage()));
  m_installBackButton->setEnabled(true);

  m_installContinueButton->setEnabled(false);
  disconnect( m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_connectToSource()));
  connect( m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_installModules()));

  populateInstallModuleListView( currentInstallSource() );
  m_installContinueButton->setText(i18n("Install modules"));
  m_installContinueButton->setEnabled(true);

  m_installWidgetStack->raiseWidget(m_installModuleListPage);
}

/** Connects to the chosen source. */
void CSwordSetupDialog::slot_installManageSources() {
	CInstallSourcesMgrDialog* dlg = new CInstallSourcesMgrDialog(this);
	dlg->exec();

	populateInstallCombos(); //make sure the items are updated
}

/** Installs chosen modules */
void CSwordSetupDialog::slot_installModules(){
//  qWarning("install the modules");
  //first get all chosen modules
	QStringList moduleList;
	QListViewItem* item1 = 0;
	QListViewItem* item2 = 0;

	for (item1 = m_installModuleListView->firstChild(); item1; item1 = item1->nextSibling())
		for (item2 = item1->firstChild(); item2; item2 = item2->nextSibling())
			if ( dynamic_cast<QCheckListItem*>(item2) && dynamic_cast<QCheckListItem*>(item2)->isOn() )
				moduleList << item2->text(0);

	QString catList;
	for ( QStringList::Iterator it = moduleList.begin(); it != moduleList.end(); ++it ) {
		if (!catList.isEmpty())
			catList += ", ";
     catList += *it;
  }
	const QString& message = QString::fromLatin1("You selected the following modules: %1.\n\nDo you really want to install them on your system?").arg(catList);

	if (catList.isEmpty()){
		KMessageBox::error(0, "No modules selected.", "Error") ;
	}
	else if ((KMessageBox::warningYesNo(0, message, "Warning") == KMessageBox::Yes)){  //Yes was pressed.
    BTInstallMgr iMgr;
    sword::InstallSource is = BTInstallMgr::Tool::RemoteConfig::source(&iMgr, currentInstallSource());

		qWarning("installung from %s/%s", is.source.c_str(), is.directory.c_str());

    QString target = m_targetCombo->currentText();
    if (target.contains("$HOME"))
      target.replace("$HOME", getenv("HOME"));

    //make sure target/mods.d and target/modules exist
    QDir dir(target.latin1());
    if (!dir.exists()) {
      dir.mkdir(target, true);
    }
    if (!dir.exists("modules")) {
      dir.mkdir("modules");
    }
    if (!dir.exists("mods.d")) {
      dir.mkdir("mods.d");
    }

    qWarning("installing into target %s", target.latin1());
    sword::SWMgr lMgr( target.latin1() );

    //module are removed in this section of code
		m_installedModuleCount = 0;
    m_progressDialog = new KProgressDialog(0,0,i18n("Module installation ..."), QString::null, true);
    m_progressDialog->progressBar()->setTotalSteps(100 * moduleList.count());
    connect(&iMgr, SIGNAL(completed(const int, const int)), SLOT(installCompleted(const int, const int)));

		for ( QStringList::Iterator it = moduleList.begin(); it != moduleList.end(); ++it, ++m_installedModuleCount ) {

			m_installingModule = *it;

      //check whether it's an update. If yes, remove exuisting module first
      if (CSwordModuleInfo* m = backend()->findModuleByName(*it)) { //module found?
        QString prefixPath = m->config(CSwordModuleInfo::AbsoluteDataPath) + "/";
        QString dataPath = m->config(CSwordModuleInfo::DataPath);
        if (dataPath.left(2) == "./") {
          dataPath = dataPath.mid(2);
        }

        if (prefixPath.contains(dataPath)) {
          prefixPath = prefixPath.replace(dataPath, "");
        }
        else {
          prefixPath = QString::fromLatin1(backend()->prefixPath);
        }
        sword::SWMgr mgr(prefixPath.latin1());
        iMgr.removeModule(&mgr, m->name().latin1());
      }

      if (BTInstallMgr::Tool::RemoteConfig::isRemoteSource(&is)) {
        iMgr.installModule(&lMgr, 0, (*it).latin1(), &is);
      }
      else { //local source
        //qWarning("install from local source");
        iMgr.installModule(&lMgr, is.directory.c_str(), (*it).latin1());
      }
    }
		delete m_progressDialog;
		m_progressDialog = 0;

    //reload our backend because modules may have changed
    backend()->reloadModules();
    populateInstallModuleListView( currentInstallSource() ); //rebuild the tree
    populateRemoveModuleListView();
  }
}

/** No descriptions */
void CSwordSetupDialog::installCompleted( const int total, const int file ){
	if (m_progressDialog) {
    m_progressDialog->progressBar()->setProgress(total+100*m_installedModuleCount);
    m_progressDialog->setLabel( i18n("[%1]: %2% complete").arg(m_installingModule).arg(total) );
  }
}

/** No descriptions */
void CSwordSetupDialog::slot_showInstallSourcePage(){
  connect( m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_connectToSource()));
  disconnect( m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_installModules()));
  m_installContinueButton->setText(i18n("Connect"));
  m_installBackButton->setEnabled(false);

  m_installWidgetStack->raiseWidget(m_installSourcePage);
}

/** No descriptions */
void CSwordSetupDialog::slot_swordEditClicked(){
  if (QListViewItem* i = m_swordPathListBox->currentItem()) {
    KURL url = KDirSelectDialog::selectDirectory(i->text(0), true);
    if (url.isValid()) {
      i->setText(0, url.path());
    }
  }
}

/** No descriptions */
void CSwordSetupDialog::slot_swordAddClicked(){
  KURL url = KDirSelectDialog::selectDirectory(QString::null, true);
  if (url.isValid()) {
    new KListViewItem(m_swordPathListBox, url.path());
  }
}

/** No descriptions */
void CSwordSetupDialog::slot_swordRemoveClicked(){
  if (QListViewItem* i = m_swordPathListBox->currentItem()) {
    delete i;
  }
}

/** Setup the path list box */
void CSwordSetupDialog::setupSwordPathListBox(){
  QStringList targets = BTInstallMgr::Tool::LocalConfig::targetList();
  m_swordPathListBox->clear();

  for (QStringList::iterator it = targets.begin(); it != targets.end(); ++it)  {
    if ((*it).isEmpty())
      continue;
    
    new KListViewItem(m_swordPathListBox, *it);
  }
  m_swordPathListBox->setCurrentItem( m_swordPathListBox->firstChild() );
}

/** No descriptions */
void CSwordSetupDialog::slot_swordPathSelected(){
  m_swordEditPathButton->setEnabled( m_swordPathListBox->currentItem() );
}

/*!
    \fn CSwordSetupDialog::remoteCurrentInstallSource()
 */
const QString CSwordSetupDialog::currentInstallSource() {
	QString source = m_sourceCombo->currentText();
	source = source.remove( i18n("[Local]") + " " );
	source = source.remove( i18n("[Remote]") + " " );

	return source;
}

