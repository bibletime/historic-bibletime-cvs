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

#include <qtimer.h>

CModuleChooserBar::CModuleChooserBar(CImportantClasses* important, ListCSwordModuleInfo useModules, CSwordModuleInfo::type type, QWidget *parent, const char *name )
	: KToolBar(parent,name) {
	qDebug("constructor of CModuleCHooserBar");
	
  m_important = important;	
	m_moduleType = type;
	m_idCounter = 0;
	m_buttonLimit = -1; //-1 means no limit
  //insert buttons if useModules != 0
	for (useModules.first(); useModules.current(); useModules.next())		 {
		if (m_buttonLimit && (unsigned int)m_buttonLimit <= m_buttonList.count())
			break;
			
		CModuleChooserButton* b = new CModuleChooserButton(m_important,useModules.current(),m_moduleType,++m_idCounter,this);
		m_buttonList.append(b);
		insertWidget(m_idCounter, b->sizeHint().width(),b);
		connect( b, SIGNAL(sigAddButton()), this, SLOT(addButton()) );
		connect( b, SIGNAL(sigRemoveButton(const int)), this, SLOT(removeButton(const int)) );
		connect( b, SIGNAL(sigChanged()), SIGNAL(sigChanged()) );
		b->show();
	}
  if (m_buttonLimit && (unsigned int)m_buttonLimit > m_buttonList.count() )
	  addButton();
}


CModuleChooserBar::~CModuleChooserBar(){	
}

/** Adds a button to the toolbar */
void CModuleChooserBar::addButton(){
	CModuleChooserButton* b = new CModuleChooserButton(m_important, 0, m_moduleType, ++m_idCounter, this);
	m_buttonList.append(b);	
	insertWidget(m_idCounter, b->sizeHint().width(),b);			
	
 	connect( b, SIGNAL(sigAddButton()), this, SLOT(addButton()) );
 	connect( b, SIGNAL(sigRemoveButton(const int)), this, SLOT(removeButton(const int)) );
 	connect( b, SIGNAL(sigChanged()), SIGNAL(sigChanged()) );
	
	b->show();
}

/** Removes a button from the toolbar */
void CModuleChooserBar::removeButton( const int ID ){
	for (m_buttonList.first(); m_buttonList.current(); m_buttonList.next()) {	
		if (m_buttonList.current()->getId() == ID) {	//found the right button
			CModuleChooserButton* b = m_buttonList.current();
			m_buttonList.remove(b);
			ASSERT(b);
			b->hide();
	 	  m_deleteID = ID;
	 	  QTimer::singleShot( 5000, this, SLOT(deleteButton()) );
			break;
		}
	}		
	
	emit sigChanged();
}

/** Returns a list of selected modules. */
ListCSwordModuleInfo CModuleChooserBar::getModuleList(){
	ListCSwordModuleInfo list;
	list.setAutoDelete(false);
	list.clear();
	
	for (m_buttonList.first(); m_buttonList.current(); m_buttonList.next()) {	
	  CSwordModuleInfo* m = m_buttonList.current()->getModule();
	  if (m)
  		list.append( m );
	}
	return list;
}

/** No descriptions */
void CModuleChooserBar::deleteButton(){
	removeItem(m_deleteID);
}

/** Sets the number of the maximum count of buttons. */
void CModuleChooserBar::setButtonLimit(const int limit){
	m_buttonLimit = limit;
	if (m_buttonList.count() > (unsigned int)m_buttonLimit ) {	//remove the last buttons
		for (m_buttonList.last(); m_buttonList.current() && (m_buttonList.count() > (unsigned int)m_buttonLimit); m_buttonList.prev() ) {
			CModuleChooserButton* b = m_buttonList.current();
			m_buttonList.remove(b);
			b->hide();
			delete b;
		}
	}
}
