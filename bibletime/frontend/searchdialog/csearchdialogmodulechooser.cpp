/***************************************************************************
                          csearchdialogmodulechooser.cpp  -  description
                             -------------------
    begin                : Fri Mar 30 2001
    copyright            : (C) 2001 by The BibleTime team
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

//BibleTime's includes
#include "csearchdialogmodulechooser.h"
#include "../groupmanager/cgroupmanager.h"
#include "../groupmanager/cgroupmanageritem.h"
#include "../ctoolclass.h"
#include "../../structdef.h"
#include "../../backend/sword_backend/cswordbackend.h"

//Qt includes
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qlist.h>
#include <qlistbox.h>

//KDE includes
#include <klocale.h>
#include <kiconloader.h>

CSearchDialogModuleChooser::CSearchDialogModuleChooser(CImportantClasses* importantClasses, QWidget *parent, const char *name ) : QWidget(parent,name) {
	m_initialized = false;
	
	m_importantClasses = importantClasses;
	m_moduleList = 0;
	m_itemsDict.setAutoDelete(false);

	QHBoxLayout* mainLayout = new QHBoxLayout(this);
	m_moduleIndex = new CGroupManager(importantClasses, this, "module index", importantClasses->swordBackend->getModuleList(), false, false, false, false, false, false);	
	m_moduleList = new QListBox(this);

	QVBoxLayout* buttonLayout = new QVBoxLayout();
	
	QToolButton* addButton = new QToolButton(this);
	addButton->setOnIconSet(SmallIcon("forward"));
	addButton->setOffIconSet(SmallIcon("forward"));	
	addButton->setFixedSize( addButton->sizeHint() );	
	connect( addButton, SIGNAL(clicked()), SLOT(addCurrentItem()));

	QToolButton* removeButton = new QToolButton(this);
	removeButton->setOnIconSet(SmallIcon("back"));
	removeButton->setOffIconSet(SmallIcon("back"));	
	removeButton->setFixedSize( removeButton->sizeHint() );
	connect(removeButton, SIGNAL(clicked()), SLOT(removeCurrentItem()));
	
	buttonLayout->addWidget(addButton);
	buttonLayout->addSpacing(50);
	buttonLayout->addWidget(removeButton);
		
	mainLayout->addWidget(m_moduleIndex,2);
	mainLayout->addLayout(buttonLayout,0);
	mainLayout->addWidget(m_moduleList,2);

	m_initialized = true;
}

CSearchDialogModuleChooser::~CSearchDialogModuleChooser(){
}

/** Sets the chosen modules for this object. */
void CSearchDialogModuleChooser::setChosenModules(ListCSwordModuleInfo* modules){
	if (!modules)
		return;
		
	m_moduleList->clear();
	m_itemsDict.clear();	
	m_moduleIndex->clear();
	m_moduleIndex->readSettings();	

	for (modules->first(); modules->current(); modules->next()) {
		const QString module = QString::fromLocal8Bit( modules->current()->module()->Name() );
		QListViewItemIterator it( m_moduleIndex );
		for ( ; it.current(); ++it )
			if (it.current()->text(0) == module) {
				m_moduleIndex->setCurrentItem(it.current());
				m_moduleIndex->setSelected(it.current(), true);
				addCurrentItem();
			}
	}
}

/** Returns the modules chosen in this widget. */
ListCSwordModuleInfo CSearchDialogModuleChooser::getChosenModules(){
	ListCSwordModuleInfo modules;
	for (unsigned int i=0; i < m_moduleList->count(); ++i) {
		CSwordModuleInfo* m = m_importantClasses->swordBackend->findModuleByName(m_moduleList->text(i));
		if (m)
			modules.append(m);
	}	
	return modules;
}

/** Adds the selected item to the list */
void CSearchDialogModuleChooser::addCurrentItem(){
	qWarning("CSearchDialogModuleChooser::addCurrentItem()");
	CGroupManagerItem* i = dynamic_cast<CGroupManagerItem*>(m_moduleIndex->selectedItem());
	if ( i && (i->type() == CGroupManagerItem::Module) && i->moduleInfo() ) {
		m_moduleList->insertItem( CToolClass::getIconForModule(i->moduleInfo()), QString::fromLocal8Bit(i->moduleInfo()->module()->Name()),-1 );
		QListViewItem* parentItem = i->parent();
		char* charText = 0;
		if (parentItem)
			charText = (const char*)parentItem->text(0).latin1();
		m_itemsDict.insert(i, charText ? charText : "");//I don't know why .local8Bit() doesn't work
		parentItem ? parentItem->takeItem(i) : m_moduleIndex->takeItem(i);			
	}
	if (m_initialized)	
		emit chosenModulesChanged();	
}

/** Removes the selected item from the module list */
void CSearchDialogModuleChooser::removeCurrentItem(){
	qWarning("CSearchDialogModuleChooser::removeCurrentItem()");
	ASSERT(m_itemsDict.count());
	if (!m_itemsDict.count())
		return;		
	QString text = QString::null;
	if (m_moduleList->currentItem() != -1) {
		text = m_moduleList->text(m_moduleList->currentItem());
		m_moduleList->removeItem(m_moduleList->currentItem());
	}
	else
		return;

	QPtrDictIterator<char> m_it( m_itemsDict ); // iterator for dict
	while ( m_it.current() ) {
		QString parentName = QString::fromLatin1(m_it.current());
		qWarning(text.local8Bit());
		CGroupManagerItem* item = (CGroupManagerItem*)m_it.currentKey();
		ASSERT(item);
		if (item && item->text(0) == text) {
			CGroupManagerItem* folder = 0;
	    QListViewItemIterator l_it( m_moduleIndex );
			for ( ; l_it.current(); ++l_it ) {
				folder = dynamic_cast<CGroupManagerItem*>(l_it.current());
				if (folder && folder->text(0) == parentName)
					break;
			}
			if (folder)
				folder->insertItem(item);
			else
				m_moduleIndex->insertItem(item);
			m_itemsDict.remove(item);
			break;
		}
		++m_it;
	}			
	if (m_initialized)
		emit chosenModulesChanged();
}
