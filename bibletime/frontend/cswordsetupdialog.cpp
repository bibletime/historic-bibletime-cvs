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
#include <qdict.h>

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

namespace InstallationManager {

CInstallSourcesMgrDialog::InstallSourceItem::InstallSourceItem( KListView* parent ) :
	KListViewItem(parent, QString::null)
{

}

CInstallSourcesMgrDialog::InstallSourceItem::InstallSourceItem( KListView* parent, sword::InstallSource is ) : KListViewItem(parent, QString::null)
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

/*const bool CInstallSourcesMgrDialog::InstallSourceItem::isEnabled() const {
	return isOn();
}

void CInstallSourcesMgrDialog::InstallSourceItem::setEnabled( const bool enabled ) {
	setOn(enabled);
	updateItem();
}*/

void CInstallSourcesMgrDialog::InstallSourceItem::updateItem() {
	setText(0, m_caption);
//	setText(1, m_url.host() + m_url.path());
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

void CInstallSourcesMgrDialog::slotOk() {
	//save local sources
	BTInstallMgr::Tool::RemoteConfig::resetLocalSources(); //we want to overwrite old sources, not add to them
	QListViewItemIterator it(m_localSourcesList);
	while (it.current()) {
		sword::InstallSource is = sword::InstallSource("DIR");
		is.caption = it.current()->text(0).latin1();
		is.source = "local"; //just some placeholder to make InstallSrc's parsing happy
		is.directory = it.current()->text(0).latin1();
		BTInstallMgr::Tool::RemoteConfig::addSource( &is );

		++it; //next local source item
	}


	//save remote sources
	BTInstallMgr::Tool::RemoteConfig::resetRemoteSources(); //we wan't to overwrite old sources, not add to them
	it = QListViewItemIterator(m_remoteSourcesList);
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
	m_localSourcesPage = addPage(i18n("Local sources"), QString::null, DesktopIcon("folder",32));
 	m_localSourcesPage->setMinimumSize(500,400);

	QGridLayout* grid = new QGridLayout(m_localSourcesPage, 4,3, 5,5);

	QLabel* mainLabel = CToolClass::explanationLabel(m_localSourcesPage,
		i18n("Manage local sources"),
		i18n("Here you can setup the local sources for module installation, e.g. the path to your CD-Rom to install from a Sword CD.<br>On many Linux distributions the path to your cdrom is either <i>/media/cdrom</i> or <i>/cdrom</i> Please make sure you mounted the CD-Rom before you use BibleTime to install from CD-Rom.")
  );
	grid->addMultiCellWidget(mainLabel, 0, 0, 0, 2);

	m_localSourcesList = new KListView( m_localSourcesPage );
	m_localSourcesList->addColumn(i18n("Local sources"));
	m_localSourcesList->setFullWidth(true);

	QPushButton* addButton = new QPushButton(i18n("Add new directory"), m_localSourcesPage);
	connect(addButton, SIGNAL(clicked()), SLOT(slot_localAddSource()));

	QPushButton* removeButton = new QPushButton(i18n("Remove directory"), m_localSourcesPage);
	connect(removeButton, SIGNAL(clicked()), SLOT(slot_localRemoveSource()));

	grid->addMultiCellWidget( m_localSourcesList, 1,3, 0,1 );
	grid->setColStretch(0, 5);
	grid->setColStretch(1, 5);

	grid->setRowStretch(0, 0);
	grid->setRowStretch(3, 5);

	grid->addWidget( addButton, 1,2 );
	grid->addWidget( removeButton, 2,2 );
	grid->setColStretch(2, 0);


	//noe insert existing local source items, if there are no sources setup with default source
	BTInstallMgr mgr;
	QStringList sources = BTInstallMgr::Tool::RemoteConfig::sourceList( &mgr );
	for (QStringList::iterator it = sources.begin(); it != sources.end(); ++it) {
		sword::InstallSource is = BTInstallMgr::Tool::RemoteConfig::source(&mgr, (*it).latin1());
		if (BTInstallMgr::Tool::RemoteConfig::isRemoteSource( &is )) { //only use local sources as items
			continue;
		}

		/*QListViewItem* i = */new KListViewItem( m_localSourcesList, *it );
		//i->setText(0, *it);
	}

	if (m_localSourcesList->childCount() == 0) {
//		InstallSourceItem* i = new InstallSourceItem(m_remoteSourcesList);
//		i->setCaption("Crosswire");
//		i->setURL(QUrl("ftp://ftp.crosswire.org/pub/sword/raw/"));
	}

}

void CInstallSourcesMgrDialog::slot_localAddSource() {
	//Add a new dir to the list.
  KURL url = KDirSelectDialog::selectDirectory(QString::null, true);
  if (url.isValid()) {
		new KListViewItem(m_localSourcesList, url.path());
  }
}

void CInstallSourcesMgrDialog::slot_localRemoveSource() {
	if (m_localSourcesList->currentItem()) {
		delete m_localSourcesList->currentItem();
	}
}

void CInstallSourcesMgrDialog::initRemoteSourcesPage() {
	m_remoteSourcesPage = addPage(i18n("Remote sources"), QString::null, DesktopIcon("network",32));
 	m_remoteSourcesPage->setMinimumSize(500,400);

	QGridLayout* grid = new QGridLayout(m_remoteSourcesPage, 5,5, 5,5);

	QLabel* mainLabel = CToolClass::explanationLabel(m_remoteSourcesPage,
		i18n("Manage remote sources"),
		i18n("Setup remote sources like FTP servers which can be used to download Sword modules on your computer.")
  );
	grid->addMultiCellWidget(mainLabel, 0, 0, 0, 4);

	m_remoteSourcesList = new KListView( m_remoteSourcesPage );
	m_remoteSourcesList->setAllColumnsShowFocus(true);
	m_remoteSourcesList->addColumn(i18n("Name"));
//  m_remoteSourcesList->addColumn("URL");
	m_remoteSourcesList->setFullWidth(true);
	connect(m_remoteSourcesList, SIGNAL(selectionChanged()),
		SLOT(slot_remoteSourceSelectionChanged()));

 	QPushButton* addButton = new QPushButton(i18n("New"), m_remoteSourcesPage);
	addButton->setIconSet(DesktopIcon("filenew", 16));
	connect(addButton, SIGNAL(clicked()), SLOT(slot_remoteAddSource()));

	QPushButton* removeButton = new QPushButton(i18n("Remove"), m_remoteSourcesPage);
	removeButton->setIconSet(DesktopIcon("edittrash", 16));
	connect(removeButton, SIGNAL(clicked()), SLOT(slot_remoteRemoveSource()));

	grid->addMultiCellWidget( m_remoteSourcesList, 1,3, 0,2 );
	grid->setRowStretch(0, 0);
	grid->setRowStretch(1, 5);
	grid->addWidget( addButton, 4,0 );
	grid->addWidget( removeButton, 4,1 );

	grid->setColStretch(0, 0);
	grid->setColStretch(1, 0);
	grid->setColStretch(2, 0);
	grid->setColStretch(3, 5);

	//contains the remote sources edit controls, we need boxes for the caption, the server, the dir on the server
	QGroupBox* box = new QGroupBox(m_remoteSourcesPage);
	box->setTitle(i18n("Edit remote source"));
	grid->addMultiCellWidget( box, 1,4, 3,4 );

	QGridLayout* boxGrid = new QGridLayout(box, 4, 3, box->insideMargin() + box->insideSpacing(), 5);
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

	if (m_remoteSourcesList->childCount() == 0) { //setup with the default if no items were present
		InstallSourceItem* i = new InstallSourceItem(m_remoteSourcesList);
		i->setCaption("Crosswire");
		i->setURL(QUrl("ftp://ftp.crosswire.org/pub/sword/raw"));
	}

	//now select the first item in the list
	m_remoteSourcesList->setSelected(m_remoteSourcesList->firstChild(), true);
	m_remoteSourcesList->setCurrentItem( m_remoteSourcesList->firstChild() );
	slot_remoteSourceSelectionChanged();
	m_remoteCaptionEdit->setFocus();
}

void CInstallSourcesMgrDialog::slot_remoteAddSource() {
	CInstallSourcesMgrDialog::InstallSourceItem* i = new CInstallSourcesMgrDialog::InstallSourceItem(m_remoteSourcesList);

	m_remoteSourcesList->setCurrentItem( i );
	m_remoteCaptionEdit->setText(i18n("New remote source"));
	m_remoteServerEdit->setText("ftp.domain.org");
	m_remotePathEdit->setText("/pub/sword/raw");
	m_remoteCaptionEdit->setFocus();
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
/* 			New class							*/
/******************************/


CSwordSetupDialog::CSwordSetupDialog(QWidget *parent, const char *name )
	: KDialogBase(IconList, i18n("Sword configuration"), Ok, Ok, parent, name, true, true, QString::null, QString::null, QString::null),
		m_removeModuleListView(0),
		m_installModuleListPage(0),
		m_installModuleListView(0),
		m_progressDialog(0),
  	m_refreshedRemoteSources(false)
{
	setIconListAllVisible(true);

  initSwordConfig();
	initInstall();
	initRemove();
}


void CSwordSetupDialog::initSwordConfig(){
	QFrame* page = m_swordConfigPage = addPage(i18n("Sword Path"), QString::null, DesktopIcon("bt_swordconfig",32));
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
	QLabel* confPathLabel = new QLabel(i18n("Your Sword configuration file is <b>%1</b>").arg(swordConfPath), page);
	layout->addMultiCellWidget(confPathLabel, 1,1,0,3);

  m_swordPathListBox = new KListView(page);
  m_swordPathListBox->setFullWidth(true);
  m_swordPathListBox->addColumn(i18n("Path to Sword modules"));
  connect(m_swordPathListBox, SIGNAL(selectionChanged()), this, SLOT(slot_swordPathSelected()));
  layout->addMultiCellWidget(m_swordPathListBox, 2,5,0,1);

  m_swordEditPathButton = new QPushButton(i18n("Edit Entry"), page);
  m_swordEditPathButton->setIconSet(DesktopIcon("edit", 16));
  connect(m_swordEditPathButton, SIGNAL(clicked()), this, SLOT(slot_swordEditClicked()));
  layout->addWidget(m_swordEditPathButton, 2, 3);

  m_swordAddPathButton = new QPushButton(i18n("Add Entry"), page);
  m_swordAddPathButton->setIconSet(DesktopIcon("edit_add", 16));
  connect(m_swordAddPathButton, SIGNAL(clicked()), this, SLOT(slot_swordAddClicked()));
  layout->addWidget(m_swordAddPathButton, 3,3);

  m_swordRemovePathButton = new QPushButton(i18n("Remove Entry"), page);
  m_swordRemovePathButton->setIconSet(DesktopIcon("editdelete", 16));
  connect(m_swordRemovePathButton, SIGNAL(clicked()), this, SLOT(slot_swordRemoveClicked()));
  layout->addWidget(m_swordRemovePathButton, 4,3);

  setupSwordPathListBox();
}

void CSwordSetupDialog::initInstall(){
	m_installPage = addPage(i18n("Install/Update Modules"), QString::null, DesktopIcon("bt_bible",32));

	QVBoxLayout* vboxlayout = new QVBoxLayout(m_installPage);
	QHBoxLayout* hboxlayout = new QHBoxLayout();
  hboxlayout->setAutoAdd( true );

  vboxlayout->addLayout(hboxlayout);

	m_installWidgetStack = new QWidgetStack(m_installPage);
  hboxlayout->addWidget(m_installWidgetStack);

	m_installSourcePage = new QWidget(0);
  m_installWidgetStack->addWidget(m_installSourcePage);

	m_installSourcePage->setMinimumSize(500,400);

	QGridLayout* layout = new QGridLayout(m_installSourcePage, 7, 2);
	layout->setMargin(5);
	layout->setSpacing(10);
	layout->setRowStretch(6,5);

	QLabel* installLabel = CToolClass::explanationLabel(m_installSourcePage,
		i18n("Install/update modules - Step 1"),
		i18n("Please choose a source and a destination. After that step click on the connect button.<br/><b>WARNING: If you live in a persecuted country and do not wish to risk detection you should NOT use the module remote installation feature!</b>")
  );
	layout->addMultiCellWidget(installLabel, 0,0,0,1);

	QLabel* sourceHeadingLabel = new QLabel(QString::fromLatin1("<b>%1</b>").arg(i18n("Select source location")), m_installSourcePage);
	layout->addMultiCellWidget(sourceHeadingLabel, 1,1,0,1);

	m_sourceCombo = new QComboBox(m_installSourcePage);
	layout->addWidget(m_sourceCombo, 2, 0);

	QPushButton* maintainSourcesButton = new QPushButton(i18n("Maintain sources"), m_installSourcePage);
	maintainSourcesButton->setIconSet(DesktopIcon("edit", 16));
	connect(maintainSourcesButton, SIGNAL(clicked()), SLOT(slot_installManageSources()));
	layout->addWidget(maintainSourcesButton, 2, 1, Qt::AlignLeft);

	m_sourceLabel = new QLabel(m_installSourcePage);
	layout->addMultiCellWidget(m_sourceLabel, 3,3,0,1);

	QLabel* targetHeadingLabel = new QLabel(QString::fromLatin1("<b>%1</b>").arg(i18n("Select target location")), m_installSourcePage);
	layout->addMultiCellWidget(targetHeadingLabel, 4,4,0,1);

	m_targetCombo = new QComboBox(m_installSourcePage);
	layout->addWidget(m_targetCombo, 5, 0);

	m_targetLabel = new QLabel(m_installSourcePage);
	layout->addMultiCellWidget(m_targetLabel, 6,6,0,1,Qt::AlignTop);

//part beloew main layout with the back/next buttons
	QHBoxLayout* myHBox = new QHBoxLayout();
  vboxlayout->addLayout(myHBox);

  m_installBackButton = new QPushButton(i18n("Back"), m_installPage);
	m_installBackButton->setIconSet(DesktopIcon("back",16));
	myHBox->addWidget(m_installBackButton);

	myHBox->addSpacing(10);
	myHBox->addStretch(5);

  m_installContinueButton = new QPushButton(i18n("Connect to source"), m_installPage);
	m_installContinueButton->setIconSet(DesktopIcon("forward",16));
  connect(m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_connectToSource()));
	myHBox->addWidget(m_installContinueButton);

  m_installBackButton->setEnabled(false);

	connect(m_sourceCombo, SIGNAL( highlighted(const QString&) ), SLOT( slot_sourceSelected( const QString&) ));
	connect(m_targetCombo, SIGNAL( highlighted(const QString&) ), SLOT( slot_targetSelected( const QString&) ));
	populateInstallCombos();

  slot_sourceSelected( m_sourceCombo->currentText() );
}

void CSwordSetupDialog::initRemove(){
	QFrame* page = m_removePage = addPage(i18n("Remove Modules"), QString::null, DesktopIcon("edittrash",32));

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

	QLabel* headingLabel = new QLabel(QString::fromLatin1("<b>%1</b>").arg(i18n("Select modules to be uninstalled")), page);
	layout->addMultiCellWidget(headingLabel, 1, 1, 0, 3);

	m_populateListNotification = new QLabel("", page);
	layout->addWidget(m_populateListNotification, 3, 2, Qt::AlignCenter);

	m_removeModuleListView = new KListView(page, "remove modules view");
	layout->addMultiCellWidget( m_removeModuleListView, 2,2,0,3);
	m_removeModuleListView->addColumn(i18n("Name"));
  m_removeModuleListView->addColumn(i18n("Location"));
 	m_removeModuleListView->setAllColumnsShowFocus(true);
 	m_removeModuleListView->setFullWidth(true);
	m_removeModuleListView->setRootIsDecorated(true);
	connect(m_removeModuleListView, SIGNAL(pressed(QListViewItem*)),
		SLOT(slot_removeModuleItemExecuted(QListViewItem*)));
	connect(m_removeModuleListView, SIGNAL(spacePressed(QListViewItem*)),
		SLOT(slot_removeModuleItemExecuted(QListViewItem*)));

  m_removeRemoveButton = new QPushButton(i18n("Remove selected module(s)"), page);
	m_removeRemoveButton->setIconSet( DesktopIcon("edittrash", 16) );
	layout->addWidget(m_removeRemoveButton, 3, 3, Qt::AlignRight);

	connect(m_removeRemoveButton, SIGNAL(clicked()),
		this, SLOT(slot_doRemoveModules()));

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
  emit signalSwordSetupChanged();
}

/** Opens the page which contaisn the given part ID. */
const bool CSwordSetupDialog::showPart( CSwordSetupDialog::Parts ID, const bool /*exclusive*/ ) {
//  if (exlusive) {
//    m_swordConfigPage->setEnabled(false);
//    m_installPage->setEnabled(false)
//    m_removePage->setEnabled(false)
//  }

  bool ret = false;
	switch (ID) {
		case CSwordSetupDialog::Sword:
      showPage( pageIndex(m_swordConfigPage) );
			break;
		case CSwordSetupDialog::Install:
      showPage( pageIndex(m_installPage) );
			break;
		case CSwordSetupDialog::Remove:
      showPage( pageIndex(m_removePage) );
			break;
		default:
			break;
	}
  return ret;
}

/** No descriptions */
void CSwordSetupDialog::populateInstallCombos(){
	m_sourceCombo->clear();

	BTInstallMgr::Tool::RemoteConfig::initConfig();

  QStringList list;
	{
		BTInstallMgr mgr;
		list = BTInstallMgr::Tool::RemoteConfig::sourceList(&mgr);
	}
	if (!list.count()) { //add Crosswire entry
		InstallSource is("FTP");   //default return value
		is.caption = "Crosswire";
		is.source = "ftp.crosswire.org";
		is.directory = "/pub/sword/raw";
		BTInstallMgr::Tool::RemoteConfig::addSource(&is);

		BTInstallMgr mgr; //make sure we're uptodate
		list = BTInstallMgr::Tool::RemoteConfig::sourceList(&mgr);

		Q_ASSERT( list.count() > 0 );
	}

  BTInstallMgr mgr;
  for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
		sword::InstallSource is = BTInstallMgr::Tool::RemoteConfig::source(&mgr, *it);
		if (BTInstallMgr::Tool::RemoteConfig::isRemoteSource(&is)) { //remote source?
    	m_sourceCombo->insertItem( i18n("[Remote]") + " " + *it );
		}
		else {
			QFileInfo fi(*it);
			if (fi.isDir() && fi.isReadable()) {
				m_sourceCombo->insertItem( i18n("[Local]") + " " + *it );
			}
		}
  }

	//Fill in the targets in the targets combobox
  //list = (m_targetCombo->count()) ? m_swordPathListBox : BTInstallMgr::Tool::LocalConfig::targetList();
	if (m_targetCombo->count()) { //we already read in the list once, we have to use the Sword paths list items now because this list is newer
		list.clear();
		QListViewItemIterator it2( m_swordPathListBox );
		while (it2.current()) {
			list << it2.current()->text(0);

			++it2;
		}
	}
	else {
		list = BTInstallMgr::Tool::LocalConfig::targetList();
	}

	m_targetCombo->clear();
  for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
		QFileInfo fi(*it);
		if (fi.isDir() && fi.isWritable()) {
			m_targetCombo->insertItem( *it );
		}
  }

//init widget states
m_targetCombo->setEnabled( (m_targetCombo->count() > 0) );
m_installContinueButton->setEnabled(
	(m_sourceCombo->count() > 0) && (m_targetCombo->count() > 0)
);

	slot_sourceSelected( m_sourceCombo->currentText() );
}

/** No descriptions */
void CSwordSetupDialog::slot_sourceSelected(const QString &sourceName){
	//remove status parta
	QString source = sourceName;
	//we have to be a bit compilcated here because Qt 3.0.5 doesn't have the QString::replace(QString s) function

	//source = source.remove( i18n("[Local]") + " " );
	QString rep = i18n("[Local]") + " ";
	int i = source.find(rep);
	if (i>=0)
		source.remove(i, rep.length());

	//source = source.remove( i18n("[Remote]") + " " );
	rep = i18n("[Remote]") + " ";
	i = source.find(rep);
	if (i>=0)
		source.remove(i, rep.length());


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

	QListViewItemIterator list_it( m_removeModuleListView );
	while ( list_it.current() ) {
		QCheckListItem* i = dynamic_cast<QCheckListItem*>( list_it.current() );
		if (i && i->isOn()) {
			moduleList << list_it.current()->text(0);
		}

		++list_it;
	}

	const QString message = i18n("You selected the following modules: %1.\n\n"
		"Do you really want to remove them from your system?").arg(moduleList.join(", "));

	if ((KMessageBox::warningYesNo(0, message, i18n("Warning")) == KMessageBox::Yes)){  //Yes was pressed.
    sword::InstallMgr installMgr;
		QDict<sword::SWMgr> mgrDict; //maps config paths to SWMgr objects

  	for ( QStringList::Iterator it = moduleList.begin(); it != moduleList.end(); ++it ) {
      if (CSwordModuleInfo* m = backend()->findModuleByName(*it)) { //module found?
        QString prefixPath = m->config(CSwordModuleInfo::AbsoluteDataPath) + "/";
        QString dataPath = m->config(CSwordModuleInfo::DataPath);

        if (dataPath.left(2) == "./") {
          dataPath = dataPath.mid(2);
        }

				if (prefixPath.contains(dataPath)) { //remove module part to get the prefix path
          prefixPath = prefixPath.remove( prefixPath.find(dataPath), dataPath.length() );
        }
        else { //fall back to default Sword config path
          prefixPath = QString::fromLatin1(backend()->prefixPath);
        }

				sword::SWMgr* mgr = mgrDict[ prefixPath ];
				if (!mgr) { //create new mgr if it's not yet available
					mgrDict.insert(prefixPath, new sword::SWMgr(prefixPath.local8Bit()));
					mgr = mgrDict[ prefixPath ];
				}

        installMgr.removeModule(mgr, m->name().latin1());
      }
    }

		CPointers::backend()->reloadModules();
    populateRemoveModuleListView(); //rebuild the tree
    populateInstallModuleListView( currentInstallSource() ); //rebuild the tree

		//delete all mgrs
		mgrDict.setAutoDelete(true);
		mgrDict.clear();
  }
}

void CSwordSetupDialog::slot_removeModuleItemExecuted(QListViewItem* item) {
	// This function enabled the Install modules button if modules are chosen
	// If an item was clicked to be not chosen look if there are other selected items
	// If the item was clicked to be chosen enable the button without looking at the other items

	QCheckListItem* checkItem = dynamic_cast<QCheckListItem*>(item);
	if (item && !checkItem) //no valid item for us
		return;

	if (checkItem && checkItem->isOn()) {
		m_removeRemoveButton->setEnabled(true);
	}
	else {
		QListViewItemIterator it( m_removeModuleListView );
		QCheckListItem* ci = 0;
		while (it.current()) {
			ci = dynamic_cast<QCheckListItem*>(it.current());
			if (ci && ci->isOn()) {
				break;
			}
			it++;
		}
		if ( ci && ci->isOn() ) { //a module is checked in the list
			m_removeRemoveButton->setEnabled(true);
    }
		else {
			m_removeRemoveButton->setEnabled(false);
		}
	}
}

/** No descriptions */
void CSwordSetupDialog::populateRemoveModuleListView(){
	m_removeRemoveButton->setEnabled(false);

	CSwordBackend myBackend;
	KApplication::kApplication()->processEvents();
	myBackend.initModules();

	m_removeModuleListView->clear();

	QListViewItem* categoryBible = new QListViewItem(m_removeModuleListView, i18n("Bibles"));
	QListViewItem* categoryCommentary = new QListViewItem(m_removeModuleListView, i18n("Commentaries"));
	QListViewItem* categoryLexicon = new QListViewItem(m_removeModuleListView, i18n("Lexicons"));
	QListViewItem* categoryBook = new QListViewItem(m_removeModuleListView, i18n("Books"));
	QListViewItem* categoryDevotionals = new QListViewItem(m_removeModuleListView, i18n("Daily Devotionals"));
	QListViewItem* categoryGlossaries = new QListViewItem(m_removeModuleListView, i18n("Glossaries"));

  categoryBible->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryCommentary->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryLexicon->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryBook->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryDevotionals->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryGlossaries->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));


  categoryBible->setOpen(true);
  categoryCommentary->setOpen(true);
  categoryLexicon->setOpen(true);
  categoryBook->setOpen(true);
  categoryDevotionals->setOpen(true);
  categoryGlossaries->setOpen(true);

	ListCSwordModuleInfo list = myBackend.moduleList();
	int modcount = list.count();
	int mod = 0;
	QListViewItem* newItem = 0;
	QListViewItem* parent = 0;
	sword::SWConfig moduleConfig("");

// 	for ( list.first(), mod = 1; list.current(); list.next(), mod++ ){
	mod = 1;
	ListCSwordModuleInfo::iterator end_it = list.end();
	
	for (ListCSwordModuleInfo::iterator it(list.begin()); it != end_it; ++it, ++mod) {
		if (mod % 20){
			m_populateListNotification->setText(i18n("Scanning your modules: %1%").arg((mod*100)/modcount));
			//KApplication::kApplication()->processEvents();
			m_removeModuleListView->triggerUpdate();
		}

		switch ((*it)->type()) {
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
        parent = 0; //shouldn't happen;
        break;
		}

		//handling for special module types
		if ((parent == categoryLexicon) && ((*it)->category() == CSwordModuleInfo::Glossary)) {
			parent = categoryGlossaries;
		}
		if ((parent == categoryLexicon) && ((*it)->category() == CSwordModuleInfo::DailyDevotional)) {
			parent = categoryDevotionals;
		}

		//now we know the category, find the right language group in that category
		const CLanguageMgr::Language* const lang = (*it)->language();
		QString langName = lang->translatedName();
		if (!lang->isValid()) {
			langName = QString::fromLatin1((*it)->module()->Lang());
		}

		QListViewItem * langFolder = parent->firstChild();
    while( langFolder ) { //try to find language folder if it exsists
			if (langFolder->text(0) == langName) { //found right folder
				break;
			}
			langFolder = langFolder->nextSibling();
		}

		if (!langFolder) { //not yet there
			langFolder = new QListViewItem(parent, langName);
			langFolder->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
			langFolder->setOpen(true);
		}

		newItem = new QCheckListItem(langFolder, (*it)->name(), QCheckListItem::CheckBox);
    newItem->setPixmap(0, CToolClass::getIconForModule(*it));
		newItem->setText(1, (*it)->config(CSwordModuleInfo::AbsoluteDataPath));
  }

	m_populateListNotification->setText("");

  //clean up groups
  if (!categoryBible->childCount())
    delete categoryBible;
  if (!categoryCommentary->childCount())
    delete categoryCommentary;
  if (!categoryBook->childCount())
    delete categoryBook;
  if (!categoryLexicon->childCount())
    delete categoryLexicon;
  if (!categoryDevotionals->childCount())
    delete categoryDevotionals;
  if (!categoryGlossaries->childCount())
    delete categoryGlossaries;
}

const bool CSwordSetupDialog::refreshRemoteModuleCache( const QString& sourceName ) {
	if (m_refreshedRemoteSources) { //the module info is up-to-date
		return true;
	}

  BTInstallMgr iMgr;
	m_currentInstallMgr = &iMgr; //for the progress dialog
	sword::InstallSource is = BTInstallMgr::Tool::RemoteConfig::source(&iMgr, sourceName);
	bool success = false;

	m_progressDialog = new KProgressDialog(0,0,i18n("Download"), QString::null, true);
	
	connect(
		m_progressDialog, SIGNAL(cancelClicked()), 
		SLOT(slot_moduleRefreshProgressCancelClicked())
	);
	connect(
		&iMgr, SIGNAL(completed(const int, const int)), 
		SLOT(slot_moduleRefreshCompleted(const int, const int))
	);
  
	m_progressDialog->progressBar()->setTotalSteps(100);
	m_progressDialog->setMinimumDuration(0); //sow immediately
	m_progressDialog->setLabel( i18n("Downloading module information...") );
	
	if (BTInstallMgr::Tool::RemoteConfig::isRemoteSource(&is)) {
// 		int errorCode = 0;
    if (!m_refreshedRemoteSources) {
      if (!iMgr.refreshRemoteSource( &is ) ){ //make sure the sources were updates sucessfully
				m_refreshedRemoteSources = true;
				success = true;
				m_progressDialog->progressBar()->setProgress(100); //make sure the dialog closes
			}
			else { //an error occurres, the KIO library should display an error message
				qWarning("InstallMgr: refreshRemoteSources returned an error.");
				m_refreshedRemoteSources = false;
				success = false;
			}
		}
  }

	delete m_progressDialog;
	m_progressDialog = 0;
	
	return success;
}


/** No descriptions */
void CSwordSetupDialog::populateInstallModuleListView( const QString& sourceName ){
  KApplication::kApplication()->processEvents();
	if (!m_installModuleListView) { // it may be an update after removing modules, so the widgets we need do not have to exist
		return;
	}
	
  BTInstallMgr iMgr;
	sword::InstallSource is = BTInstallMgr::Tool::RemoteConfig::source(&iMgr, sourceName);

	if (!refreshRemoteModuleCache(sourceName)) {
		return;
	}

  //kind of a hack to provide a pointer to mgr next line
  util::scoped_ptr<CSwordBackend> backend( BTInstallMgr::Tool::backend(&is) );
  if (!backend) {
    return;
	}

	
	m_installModuleListView->clear();

#if QT_VERSION >= 320
	QListViewItem* categoryBible = new QCheckListItem(m_installModuleListView, i18n("Bibles"), QCheckListItem::CheckBoxController);
	QListViewItem* categoryCommentary = new QCheckListItem(m_installModuleListView, i18n("Commentaries"), QCheckListItem::CheckBoxController);
	QListViewItem* categoryLexicon = new QCheckListItem(m_installModuleListView, i18n("Lexicons"), QCheckListItem::CheckBoxController);
	QListViewItem* categoryBook = new QCheckListItem(m_installModuleListView, i18n("Books"), QCheckListItem::CheckBoxController);
	QListViewItem* categoryDevotionals = new QCheckListItem(m_installModuleListView, i18n("Daily Devotionals"), QCheckListItem::CheckBoxController);
	QListViewItem* categoryGlossaries = new QCheckListItem(m_installModuleListView, i18n("Glossaries"), QCheckListItem::CheckBoxController);
#else
	//Qt <= 3.1.x doesn't support the CheckBoxController!, remove the define as soon as we switch to the new Qt
	QListViewItem* categoryBible = new QCheckListItem(m_installModuleListView, i18n("Bibles"), QCheckListItem::Controller);
	QListViewItem* categoryCommentary = new QCheckListItem(m_installModuleListView, i18n("Commentaries"), QCheckListItem::Controller);
	QListViewItem* categoryLexicon = new QCheckListItem(m_installModuleListView, i18n("Lexicons"), QCheckListItem::Controller);
	QListViewItem* categoryBook = new QCheckListItem(m_installModuleListView, i18n("Books"), QCheckListItem::Controller);
	QListViewItem* categoryDevotionals = new QCheckListItem(m_installModuleListView, i18n("Daily Devotionals"), QCheckListItem::Controller);
	QListViewItem* categoryGlossaries = new QCheckListItem(m_installModuleListView, i18n("Glossaries"), QCheckListItem::Controller);
#endif

  categoryBible->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryCommentary->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryLexicon->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryBook->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryDevotionals->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
  categoryGlossaries->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));

  categoryBible->setOpen(true);
  categoryCommentary->setOpen(true);
  categoryLexicon->setOpen(true);
  categoryBook->setOpen(true);
  categoryDevotionals->setOpen(true);
  categoryGlossaries->setOpen(true);

	
  QListViewItem* parent = 0;
  ListCSwordModuleInfo mods = backend->moduleList();
//   for (CSwordModuleInfo* newModule = mods.first(); newModule; newModule = mods.next()) {
	ListCSwordModuleInfo::iterator end_it = mods.end();
	for (ListCSwordModuleInfo::iterator it(mods.begin()); it != end_it; ++it) {
    bool isUpdate = false;
    CSwordModuleInfo* const installedModule = CPointers::backend()->findModuleByName((*it)->name());
    if (installedModule) { //module already installed?
      //check whether it's an uodated module or just the same
      const SWVersion installedVersion( installedModule->config(CSwordModuleInfo::ModuleVersion).latin1() );
      
			const SWVersion newVersion( (*it)->config(CSwordModuleInfo::ModuleVersion).latin1() );
      isUpdate = (newVersion > installedVersion);

			if (!isUpdate) {
        continue;
			}
    }

    if ((*it)->isLocked() || (*it)->isEncrypted()) { //encrypted modules have no data files on the server
      continue;
		}

    switch ((*it)->type()) {
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

		//handling for special module types
		if ((parent == categoryLexicon) && ((*it)->category() == CSwordModuleInfo::Glossary)) {
			parent = categoryGlossaries;
		}
		if ((parent == categoryLexicon) && ((*it)->category() == CSwordModuleInfo::DailyDevotional)) {
			parent = categoryDevotionals;
		}

		//now we know the category, find the right language group in that category
		const CLanguageMgr::Language* const lang = (*it)->language();
		QString langName = lang->translatedName();
		if (!lang->isValid()) {
			langName = QString::fromLatin1((*it)->module()->Lang());
		}

		QListViewItem * langFolder = parent->firstChild();
    while( langFolder ) { //try to find language folder if it exsists
			if (langFolder->text(0) == langName) { //found right folder
				break;
			}
			
			langFolder = langFolder->nextSibling();
		}

		if (!langFolder) { //not yet there
#if QT_VERSION >= 320
			langFolder = new QCheckListItem(parent, langName, QCheckListItem::CheckBoxController);
#else
			langFolder = new QCheckListItem(parent, langName, QCheckListItem::Controller);
#endif
			langFolder->setPixmap(0, SmallIcon(CResMgr::mainIndex::closedFolder::icon, 16));
			langFolder->setOpen(false);
		}

		QListViewItem* newItem = 0;
    if (langFolder) {
    	newItem = new QCheckListItem(langFolder, (*it)->name(), QCheckListItem::CheckBox);
    }
    else { //shouldn't happen
    	newItem = new QCheckListItem(m_installModuleListView, (*it)->name(), QCheckListItem::CheckBox);
    }

    newItem->setPixmap(0, CToolClass::getIconForModule(*it));
    newItem->setText(1, installedModule ? installedModule->config(CSwordModuleInfo::ModuleVersion) : "");
    newItem->setText(2, (*it)->config(CSwordModuleInfo::ModuleVersion));
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
  if (!categoryDevotionals->childCount())
    delete categoryDevotionals;
  if (!categoryGlossaries->childCount())
    delete categoryGlossaries;
}

/** Connects to the chosen source. */
void CSwordSetupDialog::slot_connectToSource(){
//	qWarning("connectToSource - creating new widgets");
	if (!m_installModuleListPage) { //the widgets are not yet created
		m_installModuleListPage = new QWidget(0);

		QGridLayout* layout = new QGridLayout(m_installModuleListPage, 7, 2);
		layout->setMargin(5);
		layout->setSpacing(10);

		QLabel* installLabel = CToolClass::explanationLabel(m_installModuleListPage,
			i18n("Install/update modules - Step 2"),
			i18n("Please choose the modules which should be installed / updated and click the install button.")
		);
		layout->addMultiCellWidget(installLabel, 0,0,0,1);
		layout->setRowStretch(0,0);

		m_installWidgetStack->addWidget(m_installModuleListPage);
		m_installModuleListPage->setMinimumSize(500,400);

		//insert a list box which contains all available remote modules
		m_installModuleListView = new KListView(m_installModuleListPage, "install modules view");
		layout->addMultiCellWidget( m_installModuleListView, 1,6,0,1);
		layout->setColStretch(0,5);
		layout->setRowStretch(1,5);

		m_installModuleListView->addColumn(i18n("Name"));
		m_installModuleListView->addColumn(i18n("Installed version"));
		m_installModuleListView->addColumn(i18n("Remote version"));
		m_installModuleListView->addColumn(i18n("Status"));
		m_installModuleListView->setAllColumnsShowFocus(true);
		m_installModuleListView->setFullWidth(true);
		m_installModuleListView->setRootIsDecorated(true);
		connect(m_installModuleListView, SIGNAL(pressed(QListViewItem*)),
			SLOT(slot_installModuleItemExecuted(QListViewItem*)));
		connect(m_installModuleListView, SIGNAL(spacePressed(QListViewItem*)),
			SLOT(slot_installModuleItemExecuted(QListViewItem*)));
	}

	//code valid for already existing and newly created widgets
  m_installContinueButton->setEnabled(false);
  disconnect( m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_connectToSource()));
  connect( m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_installModules()));

  populateInstallModuleListView( currentInstallSource() );
  m_installContinueButton->setText(i18n("Install modules"));
  m_installContinueButton->setEnabled(false);

  m_installWidgetStack->raiseWidget(m_installModuleListPage);

  connect( m_installBackButton, SIGNAL(clicked()), this, SLOT(slot_showInstallSourcePage()));
  m_installBackButton->setEnabled(true);
}

/** Connects to the chosen source. */
void CSwordSetupDialog::slot_installManageSources() {
	CInstallSourcesMgrDialog* dlg = new CInstallSourcesMgrDialog(this);
	dlg->exec();

	populateInstallCombos(); //make sure the items are updated
}

void CSwordSetupDialog::slot_installModuleItemExecuted(QListViewItem* item) {
	// This function enabled the Install modules button if modules are chosen
	// If an item was clicked to be not chosen look if there are other selected items
	// If the item was clicked to be chosen enable the button without looking at the other items

	QCheckListItem* checkItem = dynamic_cast<QCheckListItem*>(item);
	if (item && !checkItem) //no valid item for us
		return;

	if (checkItem && checkItem->isOn() && (checkItem->type() == QCheckListItem::CheckBox)) {
		m_installContinueButton->setEnabled(true);
	}
	else {
		QListViewItemIterator it( m_installModuleListView );
		QCheckListItem* ci = 0;
		while (it.current()) {
			ci = dynamic_cast<QCheckListItem*>(it.current());
			if (ci && ci->isOn()) {
				break;
			}
			it++;
		}
		if ( ci && ci->isOn() ) { //a module is checked in the list
			m_installContinueButton->setEnabled(true);
    }
		else {
			m_installContinueButton->setEnabled(false);
		}
	}
}

/** Installs chosen modules */
void CSwordSetupDialog::slot_installModules(){
	m_installContinueButton->setEnabled(false);
	m_installBackButton->setEnabled(false);

	//first get all chosen modules
	QStringList moduleList;

	QListViewItemIterator list_it( m_installModuleListView );
	while ( list_it.current() ) {
		QCheckListItem* i = dynamic_cast<QCheckListItem*>( list_it.current() );
		if (i && i->isOn() && (i->type() == QCheckListItem::CheckBox)) {
			moduleList << list_it.current()->text(0);
		}
		++list_it;
	}

	const QString& message = i18n("You selected the following modules: %1.\n\nDo you really want to install them on your system?").arg(moduleList.join(", "));

	if ((KMessageBox::warningYesNo(0, message, i18n("Warning")) == KMessageBox::Yes)){  //Yes was pressed.
    BTInstallMgr iMgr;
		m_currentInstallMgr = &iMgr;
    sword::InstallSource is = BTInstallMgr::Tool::RemoteConfig::source(&iMgr, currentInstallSource());

//		qWarning("installung from %s/%s", is.source.c_str(), is.directory.c_str());
    QString target = m_targetCombo->currentText();

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

    sword::SWMgr lMgr( target.latin1() );

    //module are removed in this section of code
		m_installedModuleCount = 0;
    m_progressDialog = new KProgressDialog(0,0,i18n("Module download"), QString::null, true);
		
		connect(
			m_progressDialog, SIGNAL(cancelClicked()), 
			SLOT(slot_installProgressCancelClicked())
		);
    connect(
			&iMgr, SIGNAL(completed(const int, const int)), 
			SLOT(installCompleted(const int, const int))
		);

    m_progressDialog->progressBar()->setTotalSteps(100 * moduleList.count());
		m_progressDialog->setMinimumDuration(0); //sow immediately
		
		
		for ( QStringList::Iterator it = moduleList.begin(); (it != moduleList.end()) && !m_progressDialog->wasCancelled(); ++it, ++m_installedModuleCount ) {

			m_installingModule = *it;

      //check whether it's an update. If yes, remove exuisting module first
      if (CSwordModuleInfo* m = backend()->findModuleByName(*it)) { //module found?
        QString prefixPath = m->config(CSwordModuleInfo::AbsoluteDataPath) + "/";
        QString dataPath = m->config(CSwordModuleInfo::DataPath);
        if (dataPath.left(2) == "./") {
          dataPath = dataPath.mid(2);
        }

        if (prefixPath.contains(dataPath)) {
					prefixPath.remove( prefixPath.find(dataPath), dataPath.length() );	//complicated to work with Qt 3.0
          //prefixPath = prefixPath.replace(dataPath, ""); //old code working with Qt 3.2
        }
        else {
          prefixPath = QString::fromLatin1(backend()->prefixPath);
        }
        sword::SWMgr mgr(prefixPath.latin1());
        iMgr.removeModule(&mgr, m->name().latin1());
      }

      if (!m_progressDialog->wasCancelled() && BTInstallMgr::Tool::RemoteConfig::isRemoteSource(&is)) {
        iMgr.installModule(&lMgr, 0, (*it).latin1(), &is);
      }
      else if (!m_progressDialog->wasCancelled()) { //local source
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

	m_currentInstallMgr = 0;
	m_installBackButton->setEnabled(true);
	slot_installModuleItemExecuted(0);
}

/** No descriptions */
void CSwordSetupDialog::installCompleted( const int total, const int /* file */){
	if (m_progressDialog) {
    m_progressDialog->progressBar()->setProgress(total+100*m_installedModuleCount);
    m_progressDialog->setLabel( i18n("[%1]: %2% complete").arg(m_installingModule).arg(total) );
  }
}

/** No descriptions */
void CSwordSetupDialog::slot_showInstallSourcePage(){
  connect( m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_connectToSource()));
  disconnect( m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_installModules()));
  m_installBackButton->setEnabled(false);

	m_installContinueButton->setText(i18n("Connect to source"));
  m_installContinueButton->setEnabled(true);

  m_installWidgetStack->raiseWidget(m_installSourcePage);
}

/** No descriptions */
void CSwordSetupDialog::slot_swordEditClicked(){
  if (QListViewItem* i = m_swordPathListBox->currentItem()) {
    KURL url = KDirSelectDialog::selectDirectory(i->text(0), true);
		if (url.isValid()) {
			const QFileInfo fi( url.path() );
			if (fi.exists() && fi.isWritable()) {
				i->setText(0, url.path());
				populateInstallCombos(); //update target list bof on install page
			}
			else {
				const int result = KMessageBox::warningYesNo(this, i18n("This directory is not writable, so modules can not be installed here using BibleTime. Do you want to use this directory instead of the previous value?"));
				if (result == KMessageBox::Yes) {
					i->setText(0, url.path());
					populateInstallCombos(); //update target list bof on install page
				}
			}
		}
  }
}

/** No descriptions */
void CSwordSetupDialog::slot_swordAddClicked(){
  KURL url = KDirSelectDialog::selectDirectory(QString::null, true);
  if (url.isValid()) {
		const QFileInfo fi( url.path() );
		if (fi.exists() && fi.isWritable()) {
	    (void)new KListViewItem(m_swordPathListBox, url.path());
			populateInstallCombos(); //update target list bof on install page
		}
		else {
			const int result = KMessageBox::warningYesNo(this, i18n("This directory is not writable, so modules can not be installed here using BibleTime. Do you want to add it to the list of module directories?"));
			if (result == KMessageBox::Yes) {
		    (void)new KListViewItem(m_swordPathListBox, url.path());
				populateInstallCombos(); //update target list bof on install page
			}
		}
  }
}

/** No descriptions */
void CSwordSetupDialog::slot_swordRemoveClicked(){
  if (QListViewItem* i = m_swordPathListBox->currentItem()) {
    delete i;
		populateInstallCombos(); //update target list bof on install page
  }
}

/** Setup the path list box */
void CSwordSetupDialog::setupSwordPathListBox(){
  QStringList targets = BTInstallMgr::Tool::LocalConfig::targetList();
  m_swordPathListBox->clear();

  for (QStringList::iterator it = targets.begin(); it != targets.end(); ++it)  {
    if ((*it).isEmpty()) {
      continue;
		}
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
	//source = source.remove( i18n("[Local]") + " " );
	QString rep = i18n("[Local]") + " ";
	int i = source.find(rep);
	if (i>=0)
		source.remove(i, rep.length());

	//source = source.remove( i18n("[Remote]") + " " );
	rep = i18n("[Remote]") + " ";
	i = source.find(rep);
	if (i>=0)
		source.remove(i, rep.length());

	return source;
}

void CSwordSetupDialog::slot_installProgressCancelClicked() {
	//the cancel button of the progress dialog was clicked.
	//m_progressDialog->wasCancelled()

	//cancel possible active module installation
	Q_ASSERT(m_currentInstallMgr);
	if (m_currentInstallMgr) {
		m_currentInstallMgr->terminate();
	}
}

void CSwordSetupDialog::slot_moduleRefreshProgressCancelClicked() {
// 	qWarning("Cancelling module refresh");
	Q_ASSERT(m_currentInstallMgr);
	if (m_currentInstallMgr) {
		m_currentInstallMgr->terminate();
	}

}

void CSwordSetupDialog::slot_moduleRefreshCompleted(const int /*total*/, const int current) {
// 	qWarning("progress %d", current);
	if (m_progressDialog) {
    m_progressDialog->progressBar()->setProgress(current);
//     m_progressDialog->setLabel( i18n("Downloading module information...");
  }
}

}
