/***************************************************************************
                          cmodulechooserbar.cpp  -  description
                             -------------------
    begin                : Sat Mar 17 2001
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

#include "cmodulechooserbar.h"
#include "cmodulechooserbutton.h"

CModuleChooserBar::CModuleChooserBar(CImportantClasses* important, ListCSwordModuleInfo* useModules, CSwordModuleInfo::type type, QWidget *parent, const char *name )
	: KToolBar(parent,name) {
  qDebug("CModuleChooserBar::CModuleChooserBar(CImportantClasses* important, ListCSwordModuleInfo* useModules, CSwordModuleInfo::type type, QWidget *parent, const char *name )");

  m_important = important;	
	m_moduleType = type;
	m_idCounter = 0;
  //insert buttons if useModules != 0
  if (useModules && useModules->count()) {
		for (useModules->first(); useModules->current(); useModules->next()) {
			CModuleChooserButton* b = new CModuleChooserButton(m_important,useModules->current(),m_moduleType,++m_idCounter,this);
			m_buttonList.append(b);
			b->show();
			insertWidget(m_idCounter, b->sizeHint().width(),b);
			
			connect( b, SIGNAL(sigAddButton()), this, SLOT(addButton()) );
			connect( b, SIGNAL(sigRemoveButton(const int)), this, SLOT(removeButton(const int)) );
			connect( b, SIGNAL(sigChanged()), SIGNAL(sigChanged()) );
		}	
  }
  addButton();
}


CModuleChooserBar::~CModuleChooserBar(){	
	qDebug("destructor of CModuleChooserBar");
}

/** Adds a button to the toolbar */
void CModuleChooserBar::addButton(){
	qDebug("CModuleChooserBar::addButton");
	CModuleChooserButton* b = new CModuleChooserButton(m_important, 0, m_moduleType, ++m_idCounter, this);
	m_buttonList.append(b);
	
	insertWidget(m_idCounter, b->sizeHint().width(),b);			
	b->show();
	
 	connect( b, SIGNAL(sigAddButton()), this, SLOT(addButton()) );
 	connect( b, SIGNAL(sigRemoveButton(const int)), this, SLOT(removeButton(const int)) );
 	connect( b, SIGNAL(sigChanged()), SIGNAL(sigChanged()) );
	
	emit sigChanged();
}

/** Removes a button from the toolbar */
void CModuleChooserBar::removeButton( const int ID ){
	qDebug("CModuleChooserBar::removeButton");	
	for (m_buttonList.first(); m_buttonList.current(); m_buttonList.next()) {	
		if (m_buttonList.current()->getId() == ID) {	//found the right button
			CModuleChooserButton* b = m_buttonList.current();
			m_buttonList.remove(b);
			delete b;
			break;
		}
	}
	emit sigChanged();
}

/** Returns a list of selected modules. */
ListCSwordModuleInfo CModuleChooserBar::getModuleList(){
  qDebug("ListCSwordModuleInfo CModuleChooserBar::getModuleList()");
	ListCSwordModuleInfo list;
	for (m_buttonList.first(); m_buttonList.current(); m_buttonList.next()) {	
#warning delete if()	
	  if (m_buttonList.current()->getModule())
  		list.append( m_buttonList.current()->getModule() );
	}	
	return list;
}