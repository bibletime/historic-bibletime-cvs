/***************************************************************************
                          cmodulechooserbutton.cpp  -  description
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

#include "cmodulechooserbutton.h"
#include "../../backend/sword_backend/cswordbackend.h"

//Qt includes
#include <qstring.h>

//KDE includes
#include <kpopupmenu.h>
#include <klocale.h>

CModuleChooserButton::CModuleChooserButton(CImportantClasses* importantClasses, CSwordModuleInfo* useModule,CSwordModuleInfo::type type, const int id, QWidget *parent, const char *name )
	: QToolButton(parent,name) {
	qDebug("CModuleChooserButton::CModuleCHooserButton");
		
	m_important = importantClasses;
	m_id = id;
	m_moduleType = type;	
	m_module = useModule;
	if (!m_module)
  	m_hasModule = false;
  else
    m_hasModule = true;
	
//	setPixmap( getIcon() );
//	setDefaultPixmap( getIcon() );
	
	//create popup
	m_popup = new KPopupMenu(this);	
	setPopup(m_popup);
	m_popup->insertTitle(i18n("Select additional modules"));
	m_popup->setCheckable(true);
	m_popup->insertItem(i18n("NONE"));
	m_popup->insertSeparator();	
	connect(m_popup, SIGNAL(activated(int)), this, SLOT(moduleChosen(int)));
	ListCSwordModuleInfo* modules = importantClasses->swordBackend->getModuleList();
	for (modules->first(); modules->current(); modules->next()) {
		if (modules->current()->getType() == m_moduleType) {
			m_popup->insertItem(QString::fromLocal8Bit(modules->current()->module()->Name()));
		}
	}	
	//Check the appropriate entry
	if (m_module) {
		for (int i = 0; i < m_popup->count(); i++) {
			if (m_popup->text(m_popup->idAt(i)) == QString::fromLocal8Bit(m_module->module()->Name()))
				m_popup->setItemChecked(m_popup->idAt(i),true);
		}
	}
 	else {
		for (int i = 0; i < m_popup->count(); i++) {
			if (m_popup->text(m_popup->idAt(i)) == i18n("NONE") )
				m_popup->setItemChecked(m_popup->idAt(i),true);
		}
	}
}	


CModuleChooserButton::~CModuleChooserButton(){
	qDebug("destructor of CModuleChooserButton");
}

/** Returns the icon used for the current status. */
QPixmap CModuleChooserButton::getIcon(){
	qDebug("CModuleChooserButton::getIcon()");	
	if (!m_module) {
		return QPixmap();
	}
	
	switch (m_moduleType) {
		case CSwordModuleInfo::Bible:
			return QPixmap();
		case CSwordModuleInfo::Commentary:
			return QPixmap();
		case CSwordModuleInfo::Lexicon:
			return QPixmap();		
		default:		
			return QPixmap();
	}
}

CSwordModuleInfo* CModuleChooserButton::getModule() {		
  qDebug("CSwordModuleInfo* CModuleChooserButton::getModule()");
	for (int i = 0; i < m_popup->count(); i++) {
	  qDebug("i");
		if ( m_popup->isItemChecked(m_popup->idAt(i)) ){
		  qDebug("return now");
			return m_important->swordBackend->findModuleByName( m_popup->text(m_popup->idAt(i)) );
		}
	}	
	return 0; //"none" selected
}

/** Returns the id used for this button. */
int CModuleChooserButton::getId() const{
	qDebug("CModuleChooserButton::getId()");
	return m_id;
}

/** Ís called after a module was selected in the popup */
void CModuleChooserButton::moduleChosen( int ID ){	
	qDebug("CModuleChooserButton::moduleChosen( int ID )");
	for (int i = 0; i < m_popup->count(); i++) {
		m_popup->setItemChecked(m_popup->idAt(i),false);		
	}	
	m_popup->setItemChecked(ID, true);
	
	if (m_popup->text(ID) == i18n("NONE")) {
		if (m_hasModule)
  		emit sigRemoveButton(m_id);
	}
	else{
	  if (!m_hasModule)
	    emit sigAddButton();
		m_hasModule = true;
  	emit sigChanged();
	}
}
