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


//#include <stdio.h>
//#include <stdlib.h>

//QT includes
//#include <qhbox.h>
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
//#include <kstandarddirs.h>
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

//Sword includes
#include <installmgr.h>

//using std::string;
//using std::list;

CSwordSetupDialog::CSwordSetupDialog(QWidget *parent, const char *name, KAccel* accel )
	: KDialogBase(IconList, i18n("Sword configuration"), Ok/* | Cancel | Apply*/, Ok, parent, name, true, true, QString::null, QString::null, QString::null) {

	setIconListAllVisible(true);

	QFrame* page = addPage(i18n("Sword Path"), QString::null, DesktopIcon(CResMgr::settings::sword::icon,32));
//	QVBoxLayout* layout = new QVBoxLayout(page,5);

	initInstall();
	initRemove();
}


void CSwordSetupDialog::initInstall(){

	QFrame* newpage = addPage(i18n("Install/Update Modules"), QString::null, DesktopIcon("connect_create",32));

	QHBoxLayout* hboxlayout = new QHBoxLayout( newpage );
  hboxlayout->setAutoAdd( true );

	QWidgetStack* m_installWidgetStack = new QWidgetStack(newpage);

	QWidget* page = new QWidget(0);
  m_installWidgetStack->addWidget(page);

//Add step 1
	loadSourceLocations();
#warning test only
	saveSourceLocations();
	determineTargetLocations();

	page->setMinimumSize(500,400);

	QGridLayout* layout = new QGridLayout(page, 8, 2);
	layout->setMargin(5);
	layout->setSpacing(10);
	layout->setRowStretch(6,5);

	QLabel* installLabel = CToolClass::explanationLabel(page,
		"Install/update modules - Step 1",
		"asdf aösdljkfha sdfjha sdkfjhasd lkfjhasd lfkjhasldk fj") ;
	layout->addMultiCellWidget(installLabel, 0,0,0,1);

	QLabel* sourceHeadingLabel = new QLabel("<b>Select source location</b>",page);
	layout->addMultiCellWidget(sourceHeadingLabel, 1,1,0,1);

	m_sourceCombo = new QComboBox(page);
	layout->addWidget(m_sourceCombo, 2, 0);

	QPushButton* maintainSourcesButton = new QPushButton(page);
	maintainSourcesButton->setText("Maintain");
	layout->addWidget(maintainSourcesButton, 2, 1, Qt::AlignLeft);

	m_sourceLabel = new QLabel(page);
	layout->addMultiCellWidget(m_sourceLabel, 3,3,0,1);

	QLabel* targetHeadingLabel = new QLabel("<b>Select target location</b>",page);
	layout->addMultiCellWidget(targetHeadingLabel, 4,4,0,1);

	m_targetCombo = new QComboBox(page);
	layout->addWidget(m_targetCombo, 5, 0);

	m_targetLabel = new QLabel(page);
	layout->addMultiCellWidget(m_targetLabel, 6,6,0,1,Qt::AlignTop);

  QPushButton* backButton = new QPushButton(page);
	backButton->setText( "Back");
	layout->addWidget(backButton, 7, 0, Qt::AlignLeft);

  QPushButton* continueButton = new QPushButton(page);
	continueButton->setText( "Connect to source");
	layout->addWidget(continueButton, 7, 1, Qt::AlignRight);

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
		"blas dlkf asldhfkajgha sdlkfjaösldkfj asdlghaösldkfja sdflkajs dlfhasölg" );
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
	layout->addWidget(m_removeBackButton, 3, 0, Qt::AlignLeft);

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
}

/** No descriptions */
void CSwordSetupDialog::populateInstallCombos(){
	QMap<QString, QString>::Iterator it;
	m_sourceCombo->clear();
	m_targetCombo->clear();

	for (it=m_sourceMap.begin(); it!=m_sourceMap.end(); it++)
		m_sourceCombo->insertItem(it.key());
	slot_sourceSelected(m_sourceCombo->currentText());

	for (it=m_targetMap.begin(); it!=m_targetMap.end(); it++)
		m_targetCombo->insertItem(it.key());
	slot_targetSelected(m_targetCombo->currentText());
}

/** No descriptions */
void CSwordSetupDialog::slot_sourceSelected(const QString &sourceName){
	m_sourceLabel->setText( m_sourceMap[sourceName] );
  source = m_sourceMap[sourceName];
}
/** No descriptions */
void CSwordSetupDialog::slot_targetSelected(const QString &targetName){
	m_targetLabel->setText( m_targetMap[targetName] );
  target = m_targetMap[targetName];
}

/** No descriptions */
void CSwordSetupDialog::loadSourceLocations(){

	KConfig config("bibletime-swordsetup");
  QString sources = config.readEntry("sources");
	m_sourceMap.clear();

	if ( sources.isEmpty() ){ //default values
		m_sourceMap["Crosswire ftp server"]="ftp://crosswire.org/pub/sword/";
		m_sourceMap["Crosswire ftp server beta area"]="ftp://crosswire.org/pub/sword/betaraw/";
		m_sourceMap["Sword CD"]="file://cdrom/";
	}
  else{ //insert saved values
    QStringList sourceList = QStringList::split("||", sources, true /*allowEmptyEntries*/);
		int count = sourceList.size();
		if (count % 2) count--; //make count even, cutting off uneven entries
		for (int i=0; i < count; i += 2){
			m_sourceMap[ sourceList[i] ] = sourceList[i+1];
//			qWarning("current items: %s %s",(const char*)sourceList[i].utf8(),(const char*)sourceList[i+1].utf8());
		}
  }
}

/** No descriptions */
void CSwordSetupDialog::saveSourceLocations(){
	QMap<QString, QString>::Iterator it;
	QString sources;

	for (it=m_sourceMap.begin(); it!=m_sourceMap.end(); it++)
		sources += it.key() + "||" + it.data() + "||";

	KConfig config("bibletime-swordsetup");
	config.writeEntry("sources", sources);
//	qWarning("saveSources: %s", (const char*)sources.utf8());
}

/** No descriptions */
void CSwordSetupDialog::determineTargetLocations(){
	m_targetMap.clear();
#warning hack only for testing
	m_targetMap["Global Sword path"]="file://usr/share/sword/";
	m_targetMap["User's Sword directory"]="file://$HOME/.sword/";
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
        	installMgr.removeModule(backend(), m->name().latin1());
        	qWarning("Removed module: [%s]" , m->name().latin1());
      }
    }
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
