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
		"asdf aösdljkfha sdfjha sdkfjhasd lkfjhasd lfkjhasldk fj") ;
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

/** No descriptions */
void InstallPage_1::populateCombos(){
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
void InstallPage_1::slot_sourceSelected(const QString &sourceName){
	m_sourceLabel->setText( m_sourceMap[sourceName] );
  source = m_sourceMap[sourceName];
}
/** No descriptions */
void InstallPage_1::slot_targetSelected(const QString &targetName){
	m_targetLabel->setText( m_targetMap[targetName] );
  target = m_targetMap[targetName];
}

/** No descriptions */
void InstallPage_1::loadSourceLocations(){

	KConfig config("bt-setupwizard");
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
void InstallPage_1::saveSourceLocations(){
	QMap<QString, QString>::Iterator it;
	QString sources;

	for (it=m_sourceMap.begin(); it!=m_sourceMap.end(); it++)
		sources += it.key() + "||" + it.data() + "||";
	
	KConfig config("bt-setupwizard");
	config.writeEntry("sources", sources);
//	qWarning("saveSources: %s", (const char*)sources.utf8());
}

/** No descriptions */
void InstallPage_1::determineTargetLocations(){
	m_targetMap.clear();
#warning hack only for testing
	m_targetMap["Global Sword path"]="file://usr/share/sword/";
	m_targetMap["User's Sword directory"]="file://$HOME/.sword/";
}
