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
#include "resource.h"
#include "backend/cswordbackend.h"

//Qt includes
#include <qstring.h>
#include <qtooltip.h>
#include <qdict.h>

//KDE includes
//#include <kpopupmenu.h>
#include <klocale.h>

CModuleChooserButton::CModuleChooserButton(CSwordModuleInfo* useModule,CSwordModuleInfo::ModuleType type, const int id, QWidget *parent, const char *name )
	: KToolBarButton("", id, parent,name) {
	qDebug("CModuleChooserButton::CModuleCHooserButton");

	m_id = id;
	m_moduleType = type;	
	m_module = useModule;
	if (!m_module)
  	m_hasModule = false;
  else
    m_hasModule = true;
	
	setIcon( iconName() );
	setPopupDelay(0);
	
	//create popup
	m_popup = new KPopupMenu(this);	
	m_popup->insertTitle(i18n("Select additional modules"));	
	m_popup->setCheckable(true);
	m_popup->insertItem(i18n("NONE"));
	m_popup->insertSeparator();	
	connect(m_popup, SIGNAL(activated(int)), this, SLOT(moduleChosen(int)));		
	setPopup(m_popup);

	QStringList languages;
	QDict<KPopupMenu> langdict;

	ListCSwordModuleInfo& modules = backend()->moduleList();
	for (modules.first(); modules.current(); modules.next()) {
		if (modules.current()->type() == m_moduleType) {
			if (languages.find( QString(modules.current()->module()->Lang())) == languages.end() ){
				languages += QString(modules.current()->module()->Lang());
				KPopupMenu* menu = new KPopupMenu;
				langdict.insert(QString(modules.current()->module()->Lang()), menu );
				m_submenus.append(menu);
				connect(menu, SIGNAL(activated(int)), this, SLOT(moduleChosen(int)));		
			}
		}
	}	

	for (modules.first(); modules.current(); modules.next()) {
		if (modules.current()->type() == m_moduleType) {
			int id = langdict[QString(modules.current()->module()->Lang())]->insertItem(
				modules.current()->name() );
			if ( m_module && modules.current()->name() == m_module->name()) {
				langdict[QString(modules.current()->module()->Lang())]->setItemChecked(id,true);
			}
		}
	}	
	//Check the appropriate entry
	if (m_module) {
//		for (unsigned int i = 0; i < m_popup->count(); i++) {
//			if (m_popup->text(m_popup->idAt(i)) == m_module->name()) {
//				m_popup->setItemChecked(m_popup->idAt(i),true);
//	  		QToolTip::add(this, m_module->name() );				
//	  		break;
//			}
//		}
	}
 	else {
		for (unsigned int i = 0; i < m_popup->count(); i++) {
			if (m_popup->text(m_popup->idAt(i)) == i18n("NONE") )
				m_popup->setItemChecked(m_popup->idAt(i),true);
				break;
		}
	}

	languages.sort();

  for ( QStringList::Iterator it = languages.begin(); it != languages.end(); ++it ) {
		m_popup->insertItem( *it, langdict[*it]);
	}


//	KPopupMenu* npopup = new KPopupMenu(this);	
//	npopup->insertTitle(i18n("Select additional modules"));	
//	npopup->setCheckable(true);
//	npopup->insertItem(i18n("TESTTTTTTTTt"));
//	m_popup->insertItem("npopup",npopup);

}	

/** Returns the icon used for the current status. */
const QString CModuleChooserButton::iconName(){
	switch (m_moduleType) {
		case CSwordModuleInfo::Bible:
			if (m_hasModule)
				return QString::fromLatin1(BIBLE_ICON_MC);
			else
				return QString::fromLatin1(BIBLE_ADD_ICON_MC);
		case CSwordModuleInfo::Commentary:
			if (m_hasModule)
				return QString::fromLatin1(COMMENTARY_ICON_MC);
			else
				return QString::fromLatin1(COMMENTARY_ICON_MC);
		case CSwordModuleInfo::Lexicon:
			if (m_hasModule)
				return QString::fromLatin1(LEXICON_ICON_MC);
			else
				return QString::fromLatin1(LEXICON_ADD_ICON_MC);
		case CSwordModuleInfo::GenericBook:
			if (m_hasModule)
				return QString::fromLatin1(BOOK_ICON_MC);
			else
				return QString::fromLatin1(BOOK_ADD_ICON_MC);
		default:
			return QString::fromLatin1(BIBLE_ICON_MC);
	}
}

CSwordModuleInfo* CModuleChooserButton::module() {
	for ( KPopupMenu* popup = m_submenus.first(); popup; popup = m_submenus.next() ) {

  	for (unsigned int i = 0; i < popup->count(); i++) {
  		if ( m_popup->isItemChecked(popup->idAt(i)) )
  			return backend()->findModuleByName( popup->text(popup->idAt(i)) );
  	}	

	}
	return 0; //"none" selected
}

/** Returns the id used for this button. */
int CModuleChooserButton::getId() const{
	return m_id;
}

/** Ís called after a module was selected in the popup */
void CModuleChooserButton::moduleChosen( int ID ){	
	for ( KPopupMenu* popup = m_submenus.first(); popup; popup = m_submenus.next() ) {

   	for (unsigned int i = 0; i < popup->count(); i++){
   		popup->setItemChecked(popup->idAt(i),false);	
   //		qWarning("ID %d, TEXT %s",m_popup->idAt(i), (const char *)m_popup->text(m_popup->idAt(i)).local8Bit() );
   	}	
   	popup->setItemChecked(ID, true);
	}

 	if (m_popup->text(ID) == i18n("NONE")) { // note: this is for m_popup, the toplevel!
 		if (m_hasModule) {
 			emit sigRemoveButton(m_id);
 			return;
		}
 	}
 	else {
 	  if (!m_hasModule)
 	    emit sigAddButton();
 		m_hasModule = true;  	
 		m_module = module();
 		setIcon(iconName());
 //		repaint();  	  	
   	emit sigChanged();
     	
   	QToolTip::remove(this);
   	if (module())
   		QToolTip::add(this, module()->name());
 	}

//	for (unsigned int i = 0; i < m_popup->count(); i++){
//		m_popup->setItemChecked(m_popup->idAt(i),false);	
////		qWarning("ID %d, TEXT %s",m_popup->idAt(i), (const char *)m_popup->text(m_popup->idAt(i)).local8Bit() );
//	}	
//	m_popup->setItemChecked(ID, true);
//	
//	if (m_popup->text(ID) == i18n("NONE")) {
//		if (m_hasModule) {
//  		emit sigRemoveButton(m_id);
//  		return;
//		}
//	}
//	else {
//	  if (!m_hasModule)
//	    emit sigAddButton();
//		m_hasModule = true;  	
//		m_module = module();
//		setIcon(iconName());
////		repaint();  	  	
//  	emit sigChanged();
//  	
//  	QToolTip::remove(this);
//  	if (module())
//  		QToolTip::add(this, module()->name());
//	}
}
