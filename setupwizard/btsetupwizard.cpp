/***************************************************************************
                          btsetupwizard.cpp  -  description
                             -------------------
    begin                : Son Jul 14 2002
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

#include "btsetupwizard.h"
#include "backend/cswordbackend.h"
#include "backend/cswordmoduleinfo.h"
#include "frontend/ctoolclass.h"

//Qt includes
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qlistview.h>
#include <qfileinfo.h>
#include <qwidgetstack.h>

//KDE includes
#include <kiconloader.h>
#include <kcmdlineargs.h>
#include <kapplication.h>
#include <kmessagebox.h>


BTSetupWizard::BTSetupWizard(QWidget *parent, const char *name ) : KMainWindow(parent,name),
	m_startBibleTimeBox(0), m_removeModuleListView(0), m_backend(0) {

	setMinimumSize(620,400);

	setIcon(SmallIcon("wizard"));

  QWidget* main = new QWidget(this);
  QHBoxLayout* mainLayout = new QHBoxLayout(main);

  QLabel* replaceWithImage = new QLabel(" reserved for image", main);
	replaceWithImage->setFixedSize(100,400);
	replaceWithImage->setPaletteBackgroundColor(Qt::darkGray);

	mainLayout->addWidget(replaceWithImage);

	mainLayout->addSpacing(20);
	m_widgetStack = new QWidgetStack(main);
  mainLayout->addWidget(m_widgetStack);
  setCentralWidget(main);
  
	addMainPage();
	addRemovePage();

	slot_backtoMainPage();  
//	qWarning("adresses are %d %d %d %d", (int)m_mainPage, (int)m_removePage, m_mainWidget->pageIndex(m_mainPage) );
}

BTSetupWizard::~BTSetupWizard(){
}


/** No descriptions */
void BTSetupWizard::addMainPage(void){

  m_mainPage = new QWidget(0);
	m_widgetStack->addWidget(m_mainPage);

	m_mainPage->setMinimumSize(500,400);

	QGridLayout* layout = new QGridLayout(m_mainPage, 6, 3);
	layout->setMargin(5);

	layout->setSpacing(10);
	layout->setRowStretch(0,10);
	layout->setRowStretch(6,10);
	layout->setColStretch(0,0);
	layout->addColSpacing(1,10);
	layout->setColStretch(2,1);

  QButton* installButton = new QPushButton(m_mainPage);
	installButton->setPixmap( BarIcon("connect_creating", KIcon::SizeMedium) );
	layout->addWidget(installButton, 1, 0, Qt::AlignCenter);

	QLabel* installLabel = CToolClass::explanationLabel(m_mainPage,
		"Install/update modules",
		"asdf aösdljkfha sdfjha sdkfjhasd lkfjhasd lfkjhasldk fj") ;
	layout->addWidget(installLabel, 1, 2);

  QButton* removeButton = new QPushButton(m_mainPage);
	removeButton->setPixmap( BarIcon("editdelete", KIcon::SizeMedium));
	layout->addWidget(removeButton, 2, 0, Qt::AlignCenter);

	QLabel* removeLabel= CToolClass::explanationLabel(m_mainPage,
		"remove installed modules from your system",
		"asdkfjh asdfkjhadsjkfa galkdfj haösdlfkjasdölfkjas dfaölskdjf öa");
	layout->addWidget(removeLabel, 2, 2);

  QButton* exportButton = new QPushButton(m_mainPage);
	exportButton->setEnabled(false);
	exportButton->setPixmap( BarIcon("fileexport", KIcon::SizeMedium));
	layout->addWidget(exportButton, 3, 0, Qt::AlignCenter);

	QLabel* exportLabel = CToolClass::explanationLabel(m_mainPage,
		"export sword modules to other formats",
		"not available yet");
	layout->addWidget(exportLabel, 3, 2);

  QButton* importButton = new QPushButton(m_mainPage);
	importButton->setEnabled(false);
	importButton->setPixmap( BarIcon("fileimport", KIcon::SizeMedium ));
	layout->addWidget(importButton, 4, 0, Qt::AlignCenter);

	QLabel* importLabel= CToolClass::explanationLabel(m_mainPage,
		"import texts to sword format",
		"not available yet");
	layout->addWidget(importLabel, 4, 2);

  QButton* exitButton = new QPushButton(m_mainPage);
	exitButton->setPixmap( BarIcon("exit", KIcon::SizeMedium));
	layout->addWidget(exitButton, 5, 0, Qt::AlignCenter);

	m_startBibleTimeBox = new QCheckBox(m_mainPage);
	m_startBibleTimeBox->setText("Start BibleTime");
	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
	// A binary option (on / off)
 	m_startBibleTimeBox->setChecked( args->isSet("start-bibletime") );
  layout->addWidget(m_startBibleTimeBox, 5, 2);

	connect(exitButton, SIGNAL(clicked()), this, SLOT(slot_exitRequested()));
	connect(removeButton, SIGNAL(clicked()), this, SLOT(slot_gotoRemovePage()));
}

/** No descriptions */
void BTSetupWizard::slot_exitRequested(){
	if (m_startBibleTimeBox->isChecked())
		KApplication::kApplication()->startServiceByDesktopName("konqueror");
	KApplication::kApplication()->quit();    
}


/** No descriptions */
void BTSetupWizard::addRemovePage(){

  m_removePage = new QWidget(0);//m_mainWidget->addPage(QString("remove page") );
	m_widgetStack->addWidget(m_removePage);
	m_removePage->setMinimumSize(500,400);

	QGridLayout* layout = new QGridLayout(m_removePage, 4, 4);
	layout->setMargin(5);

	layout->setSpacing(10);
	layout->setColStretch(1,1);
	layout->setRowStretch(2,1);

	QLabel* mainLabel= CToolClass::explanationLabel(m_removePage,
		"Remove installed module(s)",
		"This dialog lets you remove installed Sword modules from your system. Bla "
		"blas dlkf asldhfkajgha sdlkfjaösldkfj asdlghaösldkfja sdflkajs dlfhasölg" );
	layout->addMultiCellWidget(mainLabel, 0, 0, 0, 3);

	QLabel* headingLabel = new QLabel("<b>Select modules to be uninstalled</b>", m_removePage);
	layout->addMultiCellWidget(headingLabel, 1, 1, 0, 3);

	m_populateListNotification = new QLabel("", m_removePage);
	layout->addWidget(m_populateListNotification, 3, 2, Qt::AlignCenter);

	m_removeModuleListView = new QListView(m_removePage, "remove modules view");
	layout->addMultiCellWidget( m_removeModuleListView, 2,2,0,3);
	m_removeModuleListView->addColumn("Name");
  m_removeModuleListView->addColumn("Location");

  m_removeRemoveButton = new QPushButton(m_removePage);
	m_removeRemoveButton->setText( "Remove selected module(s)");
	layout->addWidget(m_removeRemoveButton, 3, 3, Qt::AlignRight);

  m_removeBackButton = new QPushButton(m_removePage);
	m_removeBackButton->setText( "Back");
	layout->addWidget(m_removeBackButton, 3, 0, Qt::AlignLeft);

	connect(m_removeBackButton, SIGNAL(clicked()), this, SLOT(slot_backtoMainPage()));
	connect(m_removeRemoveButton, SIGNAL(clicked()), this, SLOT(slot_doRemoveModules()));

}
/** No descriptions */
void BTSetupWizard::populateRemoveModuleListView(){

	m_removeBackButton->setEnabled(false);
	m_removeRemoveButton->setEnabled(false);

	if (m_backend){ //Make sure we have a current list of modules
		m_backend->shutdownModules();
		delete m_backend;
		m_backend = 0;
	}
	m_backend = new CSwordBackend();
	m_backend->initModules();

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
	SWConfig moduleConfig("");

	for ( list.first(), mod = 1; list.current(); list.next(), mod++ ){

		if (mod % 20){			
			m_populateListNotification->setText(QString("Scanning your system: %1%").arg((mod*100)/modcount));
			KApplication::kApplication()->processEvents();
		}

		location = list.current()->config( CSwordModuleInfo::AbsoluteDataPath ) ;
		name = list.current()->name() ;

		switch (list.current()->type()) {
			case CSwordModuleInfo::Bible: parent = categoryBible; break;
			case CSwordModuleInfo::Commentary: parent = categoryCommentary; break;
			case CSwordModuleInfo::Lexicon: parent = categoryLexicon; break;
			case CSwordModuleInfo::GenericBook:parent = categoryBook; break;
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
void BTSetupWizard::slot_doRemoveModules(){
	QStringList list;
	QListViewItem* item1 = 0;
	QListViewItem* item2 = 0;
	
	for (item1 = m_removeModuleListView->firstChild(); item1; item1 = item1->nextSibling())
		for (item2 = item1->firstChild(); item2; item2 = item2->nextSibling())
			if ( dynamic_cast<QCheckListItem*>(item2) && dynamic_cast<QCheckListItem*>(item2)->isOn() )
				list << item2->text(0);

	QString catList;

	for ( QStringList::Iterator it = list.begin(); it != list.end(); ++it ) {
		if (!catList.isEmpty())
			catList += ", ";
     catList += *it;
  }
	QString Message("You selected the following modules: %1.\n\n"
		"Do you really want to remove them from your system?");
	Message = Message.arg(catList);
	if (catList.isEmpty()){
		KMessageBox::error(0, "No modules selected.", "Error") ;
	}
	else if ((KMessageBox::warningYesNo(0, Message, "Warning") == KMessageBox::Yes)){  //Yes was pressed.
//
// Perform actual removal here.
//
	}
}

/** No descriptions */
void BTSetupWizard::slot_backtoMainPage(){
	m_widgetStack->raiseWidget(m_mainPage);
  m_mainPage->show();
}

/** No descriptions */
void BTSetupWizard::slot_gotoRemovePage(){
  m_widgetStack->raiseWidget(m_removePage);
  m_removePage->show();
	populateRemoveModuleListView();
}
