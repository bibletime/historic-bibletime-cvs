/***************************************************************************
                          install.cpp  -  description
                             -------------------
    begin                : Fri Oct 4 2002
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

#include "install.h"
#include "btsetupwizard.h"

#include "util/ctoolclass.h"

#include <qwidget.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>
#include <qlabel.h>

#include <kconfig.h>
#include <kglobal.h>

InstallPage_1::InstallPage_1(BTSetupWizard* main){
  m_main = main;
}


void InstallPage_1::addPage(){

	loadSourceLocations();
#warning test only
	saveSourceLocations();
	determineTargetLocations();

  m_widget = new QWidget(0);
	m_main->m_widgetStack->addWidget(m_widget);

	m_main->m_mainPage->setMinimumSize(500,400);

	QGridLayout* layout = new QGridLayout(m_widget, 8, 2);
	layout->setMargin(5);
	layout->setSpacing(10);
	layout->setRowStretch(6,5);

	QLabel* installLabel = CToolClass::explanationLabel(m_widget,
		"Install/update modules - Step 1",
		"asdf a�sdljkfha sdfjha sdkfjhasd lkfjhasd lfkjhasldk fj") ;
	layout->addMultiCellWidget(installLabel, 0,0,0,1);

	QLabel* sourceHeadingLabel = new QLabel("<b>Select source location</b>",m_widget);
	layout->addMultiCellWidget(sourceHeadingLabel, 1,1,0,1);

	m_sourceCombo = new QComboBox(m_widget);
	layout->addWidget(m_sourceCombo, 2, 0);

	QPushButton* maintainSourcesButton = new QPushButton(m_widget);
	maintainSourcesButton->setText("Maintain");
	layout->addWidget(maintainSourcesButton, 2, 1, Qt::AlignLeft);

	m_sourceLabel = new QLabel(m_widget);
	layout->addMultiCellWidget(m_sourceLabel, 3,3,0,1);

	QLabel* targetHeadingLabel = new QLabel("<b>Select target location</b>",m_widget);
	layout->addMultiCellWidget(targetHeadingLabel, 4,4,0,1);

	m_targetCombo = new QComboBox(m_widget);
	layout->addWidget(m_targetCombo, 5, 0);

	m_targetLabel = new QLabel(m_widget);
	layout->addMultiCellWidget(m_targetLabel, 6,6,0,1,Qt::AlignTop);

  QPushButton* backButton = new QPushButton(m_widget);
	backButton->setText( "Back");
	layout->addWidget(backButton, 7, 0, Qt::AlignLeft);

  QPushButton* continueButton = new QPushButton(m_widget);
	continueButton->setText( "Connect to source");
	layout->addWidget(continueButton, 7, 1, Qt::AlignRight);

	connect(backButton, SIGNAL(clicked() ), m_main, SLOT(slot_backtoMainPage()));
	connect(m_sourceCombo, SIGNAL( highlighted(const QString&) ), SLOT( slot_sourceSelected( const QString&) ));
	connect(m_targetCombo, SIGNAL( highlighted(const QString&) ), SLOT( slot_targetSelected( const QString&) ));
	populateCombos();
}

/** No descriptions */
void InstallPage_1::activate(){
  m_main->m_widgetStack->raiseWidget(m_widget);
  m_widget->show();
}




