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

#include "qlayout.h"

#include "kjanuswidget.h"
#include "kiconloader.h"
#include "kcmdlineargs.h"
#include "kapplication.h"

#include "qlabel.h"
#include "qpushbutton.h"
#include "qcheckbox.h"
#include "qpixmap.h"

BTSetupWizard::BTSetupWizard(QWidget *parent, const char *name ) : KMainWindow(parent,name) {

	setMinimumSize(620,400);

	setIcon(SmallIcon("wizard"));

	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	QLabel* replaceWithImage = new QLabel(" reserved for image", this);
	replaceWithImage->setFixedSize(100,400);
	replaceWithImage->setPaletteBackgroundColor(Qt::darkGray);

	mainLayout->addWidget(replaceWithImage , 50);

	mainLayout->addSpacing(20);

	m_mainWidget = new KJanusWidget(this, 0, KJanusWidget::Plain);
  mainLayout->addWidget(m_mainWidget,500);

	addMainPage();

//	mainWidget->showPage(0);
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

  QFrame* mainPage = m_mainWidget->addPage(QString("main page") );
	mainPage->setMinimumSize(500,400);

	QGridLayout* layout = new QGridLayout(mainPage, 6, 3);
	layout->setMargin(5);

	layout->setSpacing(10);
	layout->setRowStretch(0,10);
	layout->setRowStretch(6,10);
	layout->setColStretch(0,0);
	layout->addColSpacing(1,10);
	layout->setColStretch(2,1);

  QButton* installButton = new QPushButton(mainPage);
	installButton->setPixmap( BarIcon("connect_creating", KIcon::SizeMedium) );
	layout->addWidget(installButton, 1, 0, Qt::AlignCenter);

	QLabel* installLabel = explanationLabel(mainPage,
		"Install/update modules",
		"asdf aösdljkfha sdfjha sdkfjhasd lkfjhasd lfkjhasldk fj") ;
	layout->addWidget(installLabel, 1, 2);

  QButton* removeButton = new QPushButton(mainPage);
	removeButton->setPixmap( BarIcon("editdelete", KIcon::SizeMedium));
	layout->addWidget(removeButton, 2, 0, Qt::AlignCenter);

	QLabel* removeLabel= explanationLabel(mainPage,
		"remove installed modules from your system",
		"asdkfjh asdfkjhadsjkfa galkdfj haösdlfkjasdölfkjas dfaölskdjf öa");
	layout->addWidget(removeLabel, 2, 2);

  QButton* exportButton = new QPushButton(mainPage);
	exportButton->setEnabled(false);
	exportButton->setPixmap( BarIcon("fileexport", KIcon::SizeMedium));
	layout->addWidget(exportButton, 3, 0, Qt::AlignCenter);

	QLabel* exportLabel= explanationLabel(mainPage,
		"export sword modules to other formats",
		"not available yet");
	layout->addWidget(exportLabel, 3, 2);

  QButton* importButton = new QPushButton(mainPage);
	importButton->setEnabled(false);
	importButton->setPixmap( BarIcon("fileimport", KIcon::SizeMedium ));
	layout->addWidget(importButton, 4, 0, Qt::AlignCenter);

	QLabel* importLabel= explanationLabel(mainPage,
		"import texts to sword format",
		"not available yet");
	layout->addWidget(importLabel, 4, 2);

  QButton* exitButton = new QPushButton(mainPage);
	exitButton->setPixmap( BarIcon("exit", KIcon::SizeMedium));
	layout->addWidget(exitButton, 5, 0, Qt::AlignCenter);


	m_startBibleTimeBox = new QCheckBox(mainPage);
	m_startBibleTimeBox->setText("Start BibleTime");
	KCmdLineArgs *args = KCmdLineArgs::parsedArgs();
	// A binary option (on / off)
  	m_startBibleTimeBox->setChecked( args->isSet("start-bibletime") );
  layout->addWidget(m_startBibleTimeBox, 5, 2);

	connect(exitButton, SIGNAL(clicked()), this, SLOT(slot_exitRequested()));


}
/** No descriptions */
void BTSetupWizard::slot_exitRequested(){
	KApplication app;
	if (m_startBibleTimeBox->isChecked())
		app.startServiceByDesktopName("konqueror");
	app.exit();
}
