/***************************************************************************
                          remove.cpp  -  description
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

#include "remove.h"
#include "btsetupwizard.h"
#include "backend/cswordbackend.h"
#include "backend/cswordmoduleinfo.h"

#include "util/ctoolclass.h"

#include <qwidget.h>
#include <qlayout.h>
#include <qpushbutton.h>
#include <qwidgetstack.h>
#include <qlabel.h>
#include <qlistview.h>
#include <qfileinfo.h>

#include <kmessagebox.h>
#include <kapplication.h>

RemovePage::RemovePage(BTSetupWizard* main) : m_backend(0){
  m_main = main;
}


void RemovePage::addPage(){
  m_widget = new QWidget(0);//m_mainWidget->addPage(QString("remove page") );
	m_main->m_widgetStack->addWidget(m_widget);
	m_widget->setMinimumSize(500,400);

	QGridLayout* layout = new QGridLayout(m_widget, 4, 4);
	layout->setMargin(5);

	layout->setSpacing(10);
	layout->setColStretch(1,1);
	layout->setRowStretch(2,1);

	QLabel* mainLabel= CToolClass::explanationLabel(m_widget,
		"Remove installed module(s)",
		"This dialog lets you remove installed Sword modules from your system. Bla "
		"blas dlkf asldhfkajgha sdlkfjaösldkfj asdlghaösldkfja sdflkajs dlfhasölg" );
	layout->addMultiCellWidget(mainLabel, 0, 0, 0, 3);

	QLabel* headingLabel = new QLabel("<b>Select modules to be uninstalled</b>", m_widget);
	layout->addMultiCellWidget(headingLabel, 1, 1, 0, 3);

	m_populateListNotification = new QLabel("", m_widget);
	layout->addWidget(m_populateListNotification, 3, 2, Qt::AlignCenter);

	m_removeModuleListView = new QListView(m_widget, "remove modules view");
	layout->addMultiCellWidget( m_removeModuleListView, 2,2,0,3);
	m_removeModuleListView->addColumn("Name");
  m_removeModuleListView->addColumn("Location");

  m_removeRemoveButton = new QPushButton(m_widget);
	m_removeRemoveButton->setText( "Remove selected module(s)");
	layout->addWidget(m_removeRemoveButton, 3, 3, Qt::AlignRight);

  m_removeBackButton = new QPushButton(m_widget);
	m_removeBackButton->setText( "Back");
	layout->addWidget(m_removeBackButton, 3, 0, Qt::AlignLeft);

	connect(m_removeBackButton,   SIGNAL( clicked() ), m_main, SLOT( slot_backtoMainPage() ));
	connect(m_removeRemoveButton, SIGNAL( clicked() ), this,   SLOT( slot_doRemoveModules() ));
}

/** No descriptions */
void RemovePage::slot_doRemoveModules(){
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
	this->activate();
}

/** No descriptions */
void RemovePage::populateRemoveModuleListView(){

	m_removeBackButton->setEnabled(false);
	m_removeRemoveButton->setEnabled(false);

	if (m_backend){ //Make sure we have a current list of modules
		m_backend->shutdownModules();
		delete m_backend;
		m_backend = 0;
	}
	m_backend = new CSwordBackend();
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
void RemovePage::activate(){
  m_main->m_widgetStack->raiseWidget(m_widget);
  m_widget->show();
	populateRemoveModuleListView();
}
