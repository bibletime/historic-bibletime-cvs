
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

#include "backend/cswordbackend.h"

#include "frontend/ctoolclass.h"

#include "whatsthisdef.h"
#include "tooltipdef.h"

//Qt includes
#include <qlabel.h>
#include <qlayout.h>
#include <qtoolbutton.h>
#include <qlist.h>
#include <qlistbox.h>
#include <qwhatsthis.h>
#include <qtooltip.h>

//KDE includes
#include <klocale.h>
#include <kiconloader.h>

CSearchDialogModuleChooser::CSearchDialogModuleChooser(QWidget *parent, const char *name ) : QWidget(parent,name) {
	m_initialized = false;
	
	m_moduleList = 0;
//	m_itemsDict.setAutoDelete(false);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);
	
	mainLayout->addWidget(CToolClass::explanationLabel(this, i18n("Choose modules"), i18n("Choose the module you want to use for the search. The tree on the left contains all available modules, the tree on the right the modules for the search. Use the two arrow buttons to add or remove modules from the right tree.")));
  mainLayout->addSpacing(5);

	QHBoxLayout* chooserLayout = new QHBoxLayout();
	  		
	m_moduleIndex = new CMainIndex(this);
  m_moduleIndex->initTree();
	QWhatsThis::add(m_moduleIndex, WT_SD_MODULECHOOSER);
	
	m_moduleList = new QListBox(this);
	QWhatsThis::add(m_moduleList, WT_SD_MC_MODULELIST);

	QVBoxLayout* buttonLayout = new QVBoxLayout();
	
	QToolButton* addButton = new QToolButton(this);
	addButton->setIconSet(SmallIcon("forward"));
//	addButton->setOffIconSet(SmallIcon("forward"));	
	addButton->setFixedSize( addButton->sizeHint() );	
	QWhatsThis::add(addButton, WT_SD_MC_ADD_MODULE);
	QToolTip::add(addButton, TT_SD_MC_ADD_MODULE);
	connect( addButton, SIGNAL(clicked()), SLOT(addCurrentItem()));

	QToolButton* removeButton = new QToolButton(this);
	removeButton->setIconSet(SmallIcon("back"));
//	removeButton->setOffIconSet(SmallIcon("back"));	
	removeButton->setFixedSize( removeButton->sizeHint() );
	QToolTip::add(removeButton, TT_SD_MC_REMOVE_MODULE);
	QWhatsThis::add(removeButton, WT_SD_MC_REMOVE_MODULE);	
	connect(removeButton, SIGNAL(clicked()), SLOT(removeCurrentItem()));
	
	buttonLayout->addWidget(addButton,0);
	buttonLayout->addSpacing(5);
	buttonLayout->addWidget(removeButton,0);
	buttonLayout->addStretch(5);
		
	chooserLayout->addWidget(m_moduleIndex,2);
	chooserLayout->addLayout(buttonLayout,0);
	chooserLayout->addWidget(m_moduleList,2);

	mainLayout->addLayout(chooserLayout);	
	
//	QListViewItem* child = m_moduleIndex->firstChild();
//	while (child) {
//		QListViewItem* nextChild = child->nextSibling();
//		if (child->childCount())
// 			child->setOpen(true);
// 		else if ( CFolderBase* i = dynamic_cast<CFolderBase*>(child) ) {
////			if (m_moduleIndex->isChild(child, nextChild)) {						
////				delete child;
////		nextChild = m_moduleIndex->firstChild();
////			}
////			else
//  		delete child;
//		}
////		child = nextChild;
//	}
	m_initialized = true;
}

CSearchDialogModuleChooser::~CSearchDialogModuleChooser(){
}

/** Sets the chosen modules for this object. */
void CSearchDialogModuleChooser::setChosenModules(ListCSwordModuleInfo& modules){
//	if (!modules.coun)
//		return;
	if (getChosenModules() == modules)
		return;
		
	m_moduleList->clear();
	m_itemsDict.clear();	
//	m_moduleIndex->clear();
//  moduleIndex->readSettings();	

	for (modules.first(); modules.current(); modules.next()) {
		QListViewItemIterator it( m_moduleIndex );
    CModuleItem* item = 0;
		for ( ; it.current(); ++it )
			if ((item = dynamic_cast<CModuleItem*>(it.current())) && item->module() ==  modules.current()) {
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
		CSwordModuleInfo* m = backend()->findModuleByName(m_moduleList->text(i));
		if (m)
			modules.append(m);
	}	
	return modules;
}

/** Adds the selected item to the list */
void CSearchDialogModuleChooser::addCurrentItem(){
	CModuleItem* i = dynamic_cast<CModuleItem*>(m_moduleIndex->selectedItem());
	QListViewItem* nextItem = 0;
	if (i)
		nextItem = i->itemBelow();
	if (i && !nextItem)
		nextItem = i->itemAbove();

	if ( i && i->module() ) {
		m_moduleList->insertItem( CToolClass::getIconForModule(i->module()), i->module()->name(),-1 );
		QListViewItem* parentItem = i->parent();
		m_itemsDict.insert(i, parentItem ? (const char*)parentItem->text(0).latin1() : "");//I don't know why .local8Bit() doesn't work
		parentItem ? parentItem->takeItem(i) : m_moduleIndex->takeItem(i);			
		
		if (nextItem)
			m_moduleIndex->setSelected(nextItem, true);
	}
	if (m_initialized)
		emit chosenModulesChanged();
}

/** Removes the selected item from the module list */
void CSearchDialogModuleChooser::removeCurrentItem(){
	if (!m_itemsDict.count())
		return;		
	QString text = QString::null;
	if (m_moduleList->currentItem() != -1) {
		text = m_moduleList->text(m_moduleList->currentItem());
		const int currentItem = m_moduleList->currentItem();
		int newIndex = currentItem;
		
		if (currentItem+1 < (int)m_moduleList->count())
			newIndex = currentItem+1;
		else
			newIndex = currentItem-1;		
		m_moduleList->removeItem(m_moduleList->currentItem());
		if (newIndex != -1)
			m_moduleList->setSelected(newIndex, true);
	}
	else
		return;

	QPtrDictIterator<char> m_it( m_itemsDict ); // iterator for dict
	while ( m_it.current() ) {
		QString parentName = QString::fromLatin1(m_it.current());
		CItemBase* item = (CItemBase*)( m_it.currentKey() );
		if (item && item->text(0) == text) {
			CFolderBase* folder = 0;
	    QListViewItemIterator l_it( m_moduleIndex );
			for ( ; l_it.current(); ++l_it ) {
				folder = dynamic_cast<CFolderBase*>(l_it.current());
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
