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
#include "install.h"
#include "remove.h"

#include "util/ctoolclass.h"

//Qt includes
#include <qlabel.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qpixmap.h>
#include <qlayout.h>
#include <qwidgetstack.h>
#include <qcombobox.h>

//KDE includes
#include <kiconloader.h>
#include <kcmdlineargs.h>
#include <kapplication.h>


BTSetupWizard::BTSetupWizard(QWidget *parent, const char *name ) : KMainWindow(parent,name),
	m_startBibleTimeBox(0){

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


	m_InstallPage_1 = new InstallPage_1(this); //before the addpage calls, or connect() won't work!
	m_RemovePage = new RemovePage(this); //before the addpage calls, or connect() won't work!

	addMainPage();
  m_InstallPage_1->addPage();
	m_RemovePage->addPage();

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

  QPushButton* installButton = new QPushButton(m_mainPage);
	installButton->setPixmap( BarIcon("connect_creating", KIcon::SizeMedium) );
	layout->addWidget(installButton, 1, 0, Qt::AlignCenter);

	QLabel* installLabel = CToolClass::explanationLabel(m_mainPage,
		"Install/update modules",
		"asdf aösdljkfha sdfjha sdkfjhasd lkfjhasd lfkjhasldk fj") ;
	layout->addWidget(installLabel, 1, 2);

  QPushButton* removeButton = new QPushButton(m_mainPage);
	removeButton->setPixmap( BarIcon("editdelete", KIcon::SizeMedium));
	layout->addWidget(removeButton, 2, 0, Qt::AlignCenter);

	QLabel* removeLabel= CToolClass::explanationLabel(m_mainPage,
		"remove installed modules from your system",
		"asdkfjh asdfkjhadsjkfa galkdfj haösdlfkjasdölfkjas dfaölskdjf öa");
	layout->addWidget(removeLabel, 2, 2);

  QPushButton* exportButton = new QPushButton(m_mainPage);
	exportButton->setEnabled(false);
	exportButton->setPixmap( BarIcon("fileexport", KIcon::SizeMedium));
	layout->addWidget(exportButton, 3, 0, Qt::AlignCenter);

	QLabel* exportLabel = CToolClass::explanationLabel(m_mainPage,
		"export sword modules to other formats",
		"not available yet");
	layout->addWidget(exportLabel, 3, 2);

  QPushButton* importButton = new QPushButton(m_mainPage);
	importButton->setEnabled(false);
	importButton->setPixmap( BarIcon("fileimport", KIcon::SizeMedium ));
	layout->addWidget(importButton, 4, 0, Qt::AlignCenter);

	QLabel* importLabel= CToolClass::explanationLabel(m_mainPage,
		"import texts to sword format",
		"not available yet");
	layout->addWidget(importLabel, 4, 2);

  QPushButton* exitButton = new QPushButton(m_mainPage);
	exitButton->setPixmap( BarIcon("exit", KIcon::SizeMedium));
	layout->addWidget(exitButton, 5, 0, Qt::AlignCenter);

	m_startBibleTimeBox = new QCheckBox(m_mainPage);
	m_startBibleTimeBox->setText("Start BibleTime");
	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
	// A binary option (on / off)
 	m_startBibleTimeBox->setChecked( args->isSet("start-bibletime") );
  layout->addWidget(m_startBibleTimeBox, 5, 2);

	connect(installButton, SIGNAL(clicked()), m_InstallPage_1, SLOT( activate() ));
	connect(removeButton,  SIGNAL(clicked()), m_RemovePage,    SLOT( activate() ));
	connect(exitButton,    SIGNAL(clicked()), this,            SLOT( slot_exitRequested() ));
}



/** No descriptions */
void BTSetupWizard::slot_backtoMainPage(){
	m_widgetStack->raiseWidget(m_mainPage);
  m_mainPage->show();
}

/** No descriptions */
void BTSetupWizard::slot_exitRequested(){
	if (m_startBibleTimeBox->isChecked())
		KApplication::kApplication()->startServiceByDesktopName("bibletime");
	KApplication::kApplication()->quit();
}

