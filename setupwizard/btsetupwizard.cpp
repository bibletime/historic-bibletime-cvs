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

#include "qlayout.h"
#include "qlistview.h"
#include "qdir.h"
#include "qfileinfo.h"

#include "kjanuswidget.h"
#include "kiconloader.h"
#include "kcmdlineargs.h"
#include "kapplication.h"

#include "qlabel.h"
#include "qpushbutton.h"
#include "qcheckbox.h"
#include "qpixmap.h"

BTSetupWizard::BTSetupWizard(QWidget *parent, const char *name ) : KMainWindow(parent,name),
	m_startBibleTimeBox(0), m_removeModuleListView(0), m_backend(0) {

	setMinimumSize(620,400);

	setIcon(SmallIcon("wizard"));

	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	QLabel* replaceWithImage = new QLabel(" reserved for image", this);
	replaceWithImage->setFixedSize(100,400);
	replaceWithImage->setPaletteBackgroundColor(Qt::darkGray);

	mainLayout->addWidget(replaceWithImage);

	mainLayout->addSpacing(20);

	m_mainWidget = new KJanusWidget(this, 0, KJanusWidget::Plain);
  mainLayout->addWidget(m_mainWidget);

	addMainPage();
	addRemovePage();

	slot_backtoMainPage();

	qWarning("adresses are %d %d %d %d", (int)m_mainPage, (int)m_removePage, m_mainWidget->pageIndex(m_mainPage) );
}

QLabel* BTSetupWizard::explanationLabel(QWidget* parent, const QString& heading, const QString& text ){
  QLabel* label = new QLabel( QString::fromLatin1("<B>%1</B><BR>%2").arg(heading).arg(text),parent );
//  label->setAutoResize(true);
//  label->setMargin(1);
//  label->setFrameStyle(QFrame::Box | QFrame::Plain);
  return label;
}

BTSetupWizard::~BTSetupWizard(){
}
/** No descriptions */
void BTSetupWizard::addMainPage(void){

  m_mainPage = m_mainWidget->addPage(QString("main page") );
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

	QLabel* installLabel = explanationLabel(m_mainPage,
		"Install/update modules",
		"asdf aösdljkfha sdfjha sdkfjhasd lkfjhasd lfkjhasldk fj") ;
	layout->addWidget(installLabel, 1, 2);

  QButton* removeButton = new QPushButton(m_mainPage);
	removeButton->setPixmap( BarIcon("editdelete", KIcon::SizeMedium));
	layout->addWidget(removeButton, 2, 0, Qt::AlignCenter);

	QLabel* removeLabel= explanationLabel(m_mainPage,
		"remove installed modules from your system",
		"asdkfjh asdfkjhadsjkfa galkdfj haösdlfkjasdölfkjas dfaölskdjf öa");
	layout->addWidget(removeLabel, 2, 2);

  QButton* exportButton = new QPushButton(m_mainPage);
	exportButton->setEnabled(false);
	exportButton->setPixmap( BarIcon("fileexport", KIcon::SizeMedium));
	layout->addWidget(exportButton, 3, 0, Qt::AlignCenter);

	QLabel* exportLabel= explanationLabel(m_mainPage,
		"export sword modules to other formats",
		"not available yet");
	layout->addWidget(exportLabel, 3, 2);

  QButton* importButton = new QPushButton(m_mainPage);
	importButton->setEnabled(false);
	importButton->setPixmap( BarIcon("fileimport", KIcon::SizeMedium ));
	layout->addWidget(importButton, 4, 0, Qt::AlignCenter);

	QLabel* importLabel= explanationLabel(m_mainPage,
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
	KApplication app;
	if (m_startBibleTimeBox->isChecked())
		app.startServiceByDesktopName("konqueror");
	app.exit();
}
/** No descriptions */
void BTSetupWizard::addRemovePage(){

  m_removePage = m_mainWidget->addPage(QString("remove page") );
	m_removePage->setMinimumSize(500,400);

	QGridLayout* layout = new QGridLayout(m_removePage, 4, 4);
	layout->setMargin(5);

	layout->setSpacing(10);
	layout->setColStretch(1,1);
	layout->setRowStretch(2,1);

	QLabel* mainLabel= explanationLabel(m_removePage,
		"Remove installed module(s)",
		"This dialog lets you remove installed Sword modules from your system. Bla "
		"blas dlkf asldhfkajgha sdlkfjaösldkfj asdlghaösldkfja sdflkajs dlfhasölg" );
	layout->addMultiCellWidget(mainLabel, 0, 0, 0, 3);

	QLabel* headingLabel= explanationLabel(m_removePage,
		"Select modules to be uninstalled", QString::null);
	layout->addMultiCellWidget(headingLabel, 1, 1, 0, 3);

	m_removeModuleListView = new QListView(m_removePage, "remove modules view");
	layout->addMultiCellWidget( m_removeModuleListView, 2,2,0,3);
	m_removeModuleListView->addColumn("Name");
  m_removeModuleListView->addColumn("Location");

  QButton* removeButton = new QPushButton(m_removePage);
	removeButton->setText( "Remove selected module(s)");
	layout->addWidget(removeButton, 3, 3, Qt::AlignRight);

  QButton* backButton = new QPushButton(m_removePage);
	backButton->setText( "Back");
	layout->addWidget(backButton, 3, 0, Qt::AlignLeft);

	connect(backButton, SIGNAL(clicked()), this, SLOT(slot_backtoMainPage()));
	
	populateRemoveModuleListView();

}
/** No descriptions */
void BTSetupWizard::populateRemoveModuleListView(){

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
	QString location, name;
	QListViewItem* newItem = 0;
	QListViewItem* parent = 0;
	SWConfig moduleConfig("");

	for ( list.first(); list.current(); list.next() ){

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

}
/** No descriptions */
void BTSetupWizard::slot_backtoMainPage(){
	m_mainWidget->setSwallowedWidget(m_mainPage);
}
/** No descriptions */
void BTSetupWizard::slot_gotoRemovePage(){
	m_mainWidget->setSwallowedWidget(m_removePage);
}
