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

CModuleChooserBar::CModuleChooserBar(ListCSwordModuleInfo useModules, CSwordModuleInfo::ModuleType type, QWidget *parent, const char *name )
  : KToolBar(parent,name),
  m_moduleType(type),
  m_idCounter(0),
	m_buttonLimit(-1) //-1 means no limit
{
  //insert buttons if useModules != 0
	for (useModules.first(); useModules.current(); useModules.next())		 {
		if ((m_buttonLimit != -1) && ( m_buttonLimit <= (int)m_buttonList.count()) ) { //we reached the button limit
			break;
    };
    
		addButton( useModules.current() );
  }

  // We can add a button to choose an additional module
  if ( (m_buttonLimit == -1) || (m_buttonLimit > (int)m_buttonList.count()) ) { 
	  addButton(0); //add a button without module set
  }
}

/** Adds a button to the toolbar */
CModuleChooserButton* const CModuleChooserBar::addButton( CSwordModuleInfo* const module ) {
	CModuleChooserButton* b = new CModuleChooserButton(module, m_moduleType, ++m_idCounter, this);
	m_buttonList.append(b);	
	insertWidget( m_idCounter, b->size().width(),b );			
	
 	connect( b, SIGNAL(sigAddButton()), this, SLOT(addButton()) );
 	connect( b, SIGNAL(sigRemoveButton(const int)), this, SLOT(removeButton(const int)) );
 	connect( b, SIGNAL(sigChanged()), SIGNAL(sigChanged()) );
	
	b->show();

  return b;
}

void CModuleChooserBar::addButton( ) {
  addButton(0);
}

/** Removes a button from the toolbar */
void CModuleChooserBar::removeButton( const int ID ){
	for (m_buttonList.first(); m_buttonList.current(); m_buttonList.next()) {	
		if (m_buttonList.current()->getId() == ID) {	//found the right button
			CModuleChooserButton* b = m_buttonList.current();
			m_buttonList.removeRef(b);
			b->hide();
      b->deleteLater();
			break;
		}
	}		
	
	emit sigChanged();
}

/** Returns a list of selected modules. */
ListCSwordModuleInfo CModuleChooserBar::getModuleList(){
//	qWarning("getModuleList called: %i buttons", m_buttonList.count());
  ListCSwordModuleInfo list;
//	list.setAutoDelete(false);
//	list.clear();
	
	CSwordModuleInfo* m;
	for ( m_buttonList.first(); m_buttonList.current(); m_buttonList.next()) {	
	  if ( (m = m_buttonList.current()->module()) ) {
  		list.append( m );
    }
	}
	return list;
}

/** Sets the number of the maximum count of buttons. */
void CModuleChooserBar::setButtonLimit(const int limit){
	m_buttonLimit = limit;
  if (limit == -1) //no need to delete buttons
    return;

  const int tooMuch = m_buttonList.count() - limit;
  for (int i = 0; i < tooMuch; ++i) {
		CModuleChooserButton* b = m_buttonList.last();
		m_buttonList.remove(b);
		b->hide();
		delete b;
  }
  
//	for (m_buttonList.last(); m_buttonList.current() && ((int)m_buttonList.count() > m_buttonLimit); m_buttonList.prev() ) {
//	}
}

/** Sets the modules which are chosen in this module chooser bar. */
void CModuleChooserBar::setModules( ListCSwordModuleInfo useModules ){
  qWarning("at first %i buttons", m_buttonList.count());
	setButtonLimit(0);	
	setButtonLimit(-1);		//these two lines clear the bar
  qWarning("in the middle %i buttons", m_buttonList.count());

  if (!useModules.count()) {
    qWarning("no modules available");
    return;
  }

  qWarning("chooserBar:setModule:  %i modules", useModules.count());
	for (useModules.first(); useModules.current(); useModules.next())		 {
		if ( (m_buttonLimit != -1) && (m_buttonLimit <= (int)m_buttonList.count()) ) {
			break;
    }

    addButton( useModules.current() );
  }
  if ( (m_buttonLimit == -1) || (m_buttonLimit > (int)m_buttonList.count()) ) {
	  addButton(0);//add button without module set
  }
  qWarning("at the end %i buttons", m_buttonList.count());
}
