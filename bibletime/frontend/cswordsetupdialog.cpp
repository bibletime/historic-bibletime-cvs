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


#include <iostream>
//#include <stdlib.h>

//QT includes
#include <qdir.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <qwidgetstack.h>
#include <qfileinfo.h>
//#include <qvbox.h>
//#include <qdict.h>
//#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlistview.h>

//#include <qbuttongroup.h>
//#include <qhbuttongroup.h>
//#include <qradiobutton.h>
//#include <qcolor.h>
//#include <qtooltip.h>
//#include <qwhatsthis.h>
//#include <qstringlist.h>
//#include <qinputdialog.h>
//#include <qdir.h>


//KDE includes
#include <kapplication.h>
#include <klocale.h>
//#include <kglobal.h>
#include <kstandarddirs.h>
//#include <kkeydialog.h>
#include <kiconloader.h>
//#include <ktabctl.h>
//#include <kapp.h>
//#include <klistbox.h>
//#include <kkeydialog.h>
//#include <kaction.h>
//#include <kconfigbase.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <kprogress.h>

//Sword includes
#include <installmgr.h>
#include <swmodule.h>
#include <swversion.h>

//using std::string;
//using std::list;
using std::cout;
using std::cerr;
using std::endl;

using namespace sword;

CSwordSetupDialog::CSwordSetupDialog(QWidget *parent, const char *name, KAccel* accel )
	: KDialogBase(IconList, i18n("Sword configuration"), Ok/* | Cancel | Apply*/, Ok, parent, name, true, true, QString::null, QString::null, QString::null) {

  m_progressDialog = 0;
	setIconListAllVisible(true);
  m_refreshedRemoteSources = false;

	QFrame* page = addPage(i18n("Sword Path"), QString::null, DesktopIcon(CResMgr::settings::sword::icon,32));
//	QVBoxLayout* layout = new QVBoxLayout(page,5);

	initInstall();
	initRemove();
}


void CSwordSetupDialog::initInstall(){
	QFrame* newpage = addPage(i18n("Install/Update Modules"), QString::null, DesktopIcon("connect_create",32));

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
		i18n("Please choose a source and a destination")
  );
	layout->addMultiCellWidget(installLabel, 0,0,0,1);

	QLabel* sourceHeadingLabel = new QLabel("<b>Select source location</b>",m_installSourcePage);
	layout->addMultiCellWidget(sourceHeadingLabel, 1,1,0,1);

	m_sourceCombo = new QComboBox(m_installSourcePage);
	layout->addWidget(m_sourceCombo, 2, 0);

	QPushButton* maintainSourcesButton = new QPushButton(m_installSourcePage);
	maintainSourcesButton->setText("Maintain");
	layout->addWidget(maintainSourcesButton, 2, 1, Qt::AlignLeft);

	m_sourceLabel = new QLabel(m_installSourcePage);
	layout->addMultiCellWidget(m_sourceLabel, 3,3,0,1);

	QLabel* targetHeadingLabel = new QLabel("<b>Select target location</b>",m_installSourcePage);
	layout->addMultiCellWidget(targetHeadingLabel, 4,4,0,1);

	m_targetCombo = new QComboBox(m_installSourcePage);
	layout->addWidget(m_targetCombo, 5, 0);

	m_targetLabel = new QLabel(m_installSourcePage);
	layout->addMultiCellWidget(m_targetLabel, 6,6,0,1,Qt::AlignTop);

  QHBoxLayout* myHBox = new QHBoxLayout();
  vboxlayout->addLayout(myHBox);
  
  m_installBackButton = new QPushButton(newpage);
	m_installBackButton->setText( "Back");
	myHBox->addWidget(m_installBackButton, 7, 0);

  m_installContinueButton = new QPushButton(newpage);
	m_installContinueButton->setText( i18n("Connect to source") );
  connect(m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_connectToSource()));
	myHBox->addWidget(m_installContinueButton, 7, 1);

//  m_installMgr = new BTInstallMgr();

  m_installBackButton->setEnabled(false);

//	connect(backButton, SIGNAL(clicked() ), m_main, SLOT(slot_backtoMainPage()));
	connect(m_sourceCombo, SIGNAL( highlighted(const QString&) ), SLOT( slot_sourceSelected( const QString&) ));
	connect(m_targetCombo, SIGNAL( highlighted(const QString&) ), SLOT( slot_targetSelected( const QString&) ));
	populateInstallCombos();

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
		"Remove installed module(s)",
		"This dialog lets you remove installed Sword modules from your system. Bla "
		"blas dlkf asldhfkajgha sdlkfja�sldkfj asdlgha�sldkfja sdflkajs dlfhas�lg" );
	layout->addMultiCellWidget(mainLabel, 0, 0, 0, 3);

	QLabel* headingLabel = new QLabel("<b>Select modules to be uninstalled</b>", page);
	layout->addMultiCellWidget(headingLabel, 1, 1, 0, 3);

	m_populateListNotification = new QLabel("", page);
	layout->addWidget(m_populateListNotification, 3, 2, Qt::AlignCenter);

	m_removeModuleListView = new QListView(page, "remove modules view");
	layout->addMultiCellWidget( m_removeModuleListView, 2,2,0,3);
	m_removeModuleListView->addColumn("Name");
  m_removeModuleListView->addColumn("Location");

  m_removeRemoveButton = new QPushButton(page);
	m_removeRemoveButton->setText( "Remove selected module(s)");
	layout->addWidget(m_removeRemoveButton, 3, 3, Qt::AlignRight);

  m_removeBackButton = new QPushButton(page);
	m_removeBackButton->setText( "Back");
  m_removeBackButton->setEnabled(false);
	layout->addWidget(m_removeBackButton, 3, 0, Qt::AlignRight);

//	connect(m_removeBackButton,   SIGNAL( clicked() ), m_main, SLOT( slot_backtoMainPage() ));
	connect(m_removeRemoveButton, SIGNAL( clicked() ), this, SLOT( slot_doRemoveModules() ));

	populateRemoveModuleListView();
}

/** Called if the OK button was clicked */
void CSwordSetupDialog::slotOk(){
	KDialogBase::slotOk();
  emit signalSwordSetupChanged( );
}

/*called if the apply button was clicked*/
void CSwordSetupDialog::slotApply(){
	KDialogBase::slotApply();
  emit signalSwordSetupChanged( );
}

/** Opens the page which contaisn the given part ID. */
const bool CSwordSetupDialog::showPart( CSwordSetupDialog::Parts ID ){
//	bool ret = false;
//	switch (ID) {
//		case CSwordSetupDialog::ViewProfiles:
//			if(showPage(pageIndex(
//					m_settings.profiles.profiles->parentWidget()) ))
//				ret = true;
//			break;
//		default:
//			break;
//	}
//	return ret;

  return false;
}

/** No descriptions */
void CSwordSetupDialog::populateInstallCombos(){
	m_sourceCombo->clear();
	m_targetCombo->clear();

  BTInstallMgr mgr;
  BTInstallMgr::Tool::initConfig();

  QStringList list = BTInstallMgr::Tool::sourceList(&mgr);
  for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
    m_sourceCombo->insertItem( *it );
  }

  list = BTInstallMgr::Tool::targetList();
  for (QStringList::iterator it = list.begin(); it != list.end(); ++it) {
    m_targetCombo->insertItem( *it );
  }

}

/** No descriptions */
void CSwordSetupDialog::slot_sourceSelected(const QString &sourceName){
  BTInstallMgr mgr;

  m_sourceLabel->setText( BTInstallMgr::Tool::source(&mgr, sourceName).directory.c_str() );
//  source = m_sourceMap[sourceName];
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
		KMessageBox::error(0, "No modules selected.", "Error") ;
	}
	else if ((KMessageBox::warningYesNo(0, message, "Warning") == KMessageBox::Yes)){  //Yes was pressed.
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
          qWarning("removing module in prefix %s with data path %s", prefixPath.latin1(), dataPath.latin1());
        }
        else {
          prefixPath = QString::fromLatin1(backend()->prefixPath);
        }

        sword::SWMgr mgr(prefixPath.latin1());
        installMgr.removeModule(&mgr, m->name().latin1());
       	qWarning("Removed module: [%s]" , m->name().latin1());
      }
    }

    //remove checklist items of removed modules
//   	for (item1 = m_removeModuleListView->firstChild(); item1; item1 = item1->nextSibling())
//  		for (item2 = item1->firstChild(); item2; item2 = item2->nextSibling())
//	  		if ( dynamic_cast<QCheckListItem*>(item2) && dynamic_cast<QCheckListItem*>(item2)->isOn() )
//		  		delete item2;
    populateRemoveModuleListView(); //rebuild the tree

  }
//	this->activate();
}

/** No descriptions */
void CSwordSetupDialog::populateRemoveModuleListView(){

	m_removeBackButton->setEnabled(false);
	m_removeRemoveButton->setEnabled(false);

//	if (m_backend){ //Make sure we have a current list of modules
//		m_backend->shutdownModules();
//		delete m_backend;
//		m_backend = 0;
//	}
	CSwordBackend* m_backend = new CSwordBackend();
	KApplication::kApplication()->processEvents();
	m_backend->initModules();

	m_removeModuleListView->clear();

	QListViewItem* categoryBible = new QListViewItem(m_removeModuleListView, "Bibles");
	QListViewItem* categoryCommentary = new QListViewItem(m_removeModuleListView, "Commentaries");
	QListViewItem* categoryLexicon = new QListViewItem(m_removeModuleListView, "Lexicons");
	QListViewItem* categoryBook = new QListViewItem(m_removeModuleListView, "Books");
	categoryBible->setOpen(true);
  categoryCommentary->setOpen(true);
  categoryLexicon->setOpen(true);
  categoryBook->setOpen(true);

	QPtrList<CSwordModuleInfo> list = m_backend->moduleList();
	int modcount = list.count();
	int mod = 0;
	QString location, name;
	QListViewItem* newItem = 0;
	QListViewItem* parent = 0;
	sword::SWConfig moduleConfig("");

	for ( list.first(), mod = 1; list.current(); list.next(), mod++ ){

		if (mod % 20){
			m_populateListNotification->setText(QString("Scanning your system: %1%").arg((mod*100)/modcount));
			KApplication::kApplication()->processEvents();
		}

		location = list.current()->config( CSwordModuleInfo::AbsoluteDataPath ) ;
		name = list.current()->name() ;

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

		m_backend->moduleConfig( name, moduleConfig);
		QFileInfo file(moduleConfig.filename.c_str());
		if (file.isWritable()) //only writable modules can be removed
			newItem = new QCheckListItem(parent, name, QCheckListItem::CheckBox);
		else
			newItem = new QListViewItem(parent, name);
		newItem->setText(1, location);
  }
	m_populateListNotification->setText("");
	m_removeBackButton->setEnabled(true);
	m_removeRemoveButton->setEnabled(true);
}

/** No descriptions */
void CSwordSetupDialog::populateInstallModuleListView( const QString& sourceName ){
  KApplication::kApplication()->processEvents();
	m_installModuleListView->clear();

	QListViewItem* categoryBible = new QListViewItem(m_installModuleListView, "Bibles");
	QListViewItem* categoryCommentary = new QListViewItem(m_installModuleListView, "Commentaries");
	QListViewItem* categoryLexicon = new QListViewItem(m_installModuleListView, "Lexicons");
	QListViewItem* categoryBook = new QListViewItem(m_installModuleListView, "Books");
	categoryBible->setOpen(true);
  categoryCommentary->setOpen(true);
  categoryLexicon->setOpen(true);
  categoryBook->setOpen(true);
  
  BTInstallMgr iMgr;
  sword::InstallSource is = BTInstallMgr::Tool::source(&iMgr, sourceName);

  if (BTInstallMgr::Tool::isRemoteSource(&is)) {        
    if (!m_refreshedRemoteSources)
      iMgr.refreshRemoteSource( &is );
    m_refreshedRemoteSources = true;
  }

  //kind of a hack to provide a pointer to mgr next line
  sword::SWMgr lMgr( BTInstallMgr::Tool::isRemoteSource(&is) ? "" : is.directory.c_str());  
  sword::SWMgr* mgr = BTInstallMgr::Tool::isRemoteSource(&is) ? is.getMgr() : &lMgr;

  QListViewItem* parent = 0;
  for (sword::ModMap::iterator it = mgr->Modules.begin(); it != mgr->Modules.end(); it++) {
    bool isUpdate = false;
    sword::SWModule* module = it->second;
    Q_ASSERT(module);

    CSwordModuleInfo* installedModule = CPointers::backend()->findModuleByName(module->Name());
    if (installedModule) { //module already installed?
      Q_ASSERT(installedModule);

      //check whether it's an uodated module or just the same
      const char* installedVersionString = installedModule->module()->getConfigEntry("Version");
      if (!installedVersionString || (installedVersionString && !strlen(installedVersionString)))
        installedVersionString = "1.0";
      
      const char* versionString = module->getConfigEntry("Version");
      if (!versionString)
        versionString = "1.0";

      qWarning("version for %s are %s / %s", module->Name(), installedVersionString, versionString);

      SWVersion installedVersion( installedVersionString );
      SWVersion newVersion( versionString );
      isUpdate = (newVersion > installedVersion);
      if (!isUpdate)
        continue;
    }

    sword::SectionMap::iterator section = mgr->config->Sections.find(module->Name());
    if ((section != mgr->config->Sections.end()) && (section->second.find("CipherKey") != section->second.end())) //module enciphered
      continue;
    
    Q_ASSERT(module);
    const char* type = module->Type();
    if (!strcmp(type, "Biblical Texts")) {
      parent = categoryBible;
    }
    else if (!strcmp(type, "Commentaries")) {
      parent = categoryCommentary;
    }
    else if (!strcmp(type, "Lexicons / Dictionaries")) {
      parent = categoryLexicon;
    }
    else if (!strcmp(type, "Generic Books")) {
      parent = categoryBook;
    }
    else {
      parent = 0;
    }

		QListViewItem* newItem = 0;
    if (parent) {
       newItem = new QCheckListItem(parent, QString::fromLatin1(module->Name()), QCheckListItem::CheckBox);      
    }
    else {
      newItem = new QCheckListItem(m_installModuleListView, QString::fromLatin1(module->Name()), QCheckListItem::CheckBox);
    }
    Q_ASSERT(newItem);
    const char* installedVersion = installedModule ? installedModule->module()->getConfigEntry("Version") : "";
    if (installedModule && !strlen(installedVersion)) {
      installedVersion = "1.0";
    }
    
    newItem->setText(1, installedVersion);
    newItem->setText(2, module->getConfigEntry("Version") ? module->getConfigEntry("Version") : "1.0");
    newItem->setText(3, isUpdate ? i18n("Update") : i18n("New"));
  }
  m_installWidgetStack->raiseWidget(m_installModuleListPage);
}


/** Connects to the chosen source. */
void CSwordSetupDialog::slot_connectToSource(){
	m_installModuleListPage = new QWidget(0);

	QGridLayout* layout = new QGridLayout(m_installModuleListPage, 8, 2);
	layout->setMargin(5);
	layout->setSpacing(10);
	layout->setRowStretch(6,5);

  QLabel* installLabel = CToolClass::explanationLabel(m_installSourcePage,
		i18n("Install/update modules - Step 2"),
		i18n("Please choose the modules which should be installed / updated and click the install button.")
  );
	layout->addMultiCellWidget(installLabel, 0,0,0,1);
  
  m_installWidgetStack->addWidget(m_installModuleListPage);
	m_installModuleListPage->setMinimumSize(500,400);

  //insert a list box which contains all available remote modules
	m_installModuleListView = new QListView(m_installModuleListPage, "install modules view");
	layout->addMultiCellWidget( m_installModuleListView, 1,6,0,1);
	m_installModuleListView->addColumn("Name");
  m_installModuleListView->addColumn("Installed version");
  m_installModuleListView->addColumn("Remote version");
  m_installModuleListView->addColumn("Status");

  connect( m_installBackButton, SIGNAL(clicked()), this, SLOT(slot_showInstallSourcePage()));
  m_installBackButton->setEnabled(true);

  m_installContinueButton->setEnabled(false);
  disconnect( m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_connectToSource()));
  connect( m_installContinueButton, SIGNAL(clicked()), this, SLOT(slot_installModules()));

  populateInstallModuleListView( m_sourceCombo->currentText() );
  m_installContinueButton->setText(i18n("Install modules"));
  m_installContinueButton->setEnabled(true);

}

/** Installs chosen modules */
void CSwordSetupDialog::slot_installModules(){
  qWarning("install the modules");
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
	QString message("You selected the following modules: %1.\n\n"
		"Do you really want to install them on your system?");
	message = message.arg(catList);
	if (catList.isEmpty()){
		KMessageBox::error(0, "No modules selected.", "Error") ;
	}
	else if ((KMessageBox::warningYesNo(0, message, "Warning") == KMessageBox::Yes)){  //Yes was pressed.
    qWarning("proceeding");
    
    BTInstallMgr iMgr;
    sword::InstallSource is = BTInstallMgr::Tool::source(&iMgr, m_sourceCombo->currentText());

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
  	for ( QStringList::Iterator it = moduleList.begin(); it != moduleList.end(); ++it ) {
      m_progressDialog = new KProgressDialog(0,0,i18n("Module Installastion ..."), QString::null, true);
      m_installingModule = *it;
      m_progressDialog->progressBar()->setTotalSteps(100);
      connect(&iMgr, SIGNAL(completed(const int, const int)), SLOT(installCompleted(const int, const int)));

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
      
      if (BTInstallMgr::Tool::isRemoteSource(&is)) {
        iMgr.installModule(&lMgr, 0, (*it).latin1(), &is);
      }
      else { //local source
        qWarning("install from local source");
        iMgr.installModule(&lMgr, is.directory.c_str(), (*it).latin1());
      }

      delete m_progressDialog;
      m_progressDialog = 0;
    }
    //reload our backend because modules may have changed
    backend()->reloadModules();
    populateInstallModuleListView( m_sourceCombo->currentText() ); //rebuild the tree
    populateRemoveModuleListView();
  }
}

/** No descriptions */
void CSwordSetupDialog::installCompleted( const int total, const int file ){
//  qWarning("percent completed %i", total);
  if (m_progressDialog) {
    m_progressDialog->progressBar()->setProgress(total);
    m_progressDialog->setLabel( QString("[%1] %2% completed").arg(m_installingModule).arg(total) );
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
