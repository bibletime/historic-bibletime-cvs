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

#include "backend/cswordbackend.h"

#include "util/cresmgr.h"

//Qt includes
#include <qstring.h>
#include <qtooltip.h>
#include <qdict.h>
#include <qvaluelist.h>

//KDE includes
#include <klocale.h>
#include <kglobal.h>
#include <kiconloader.h>

CModuleChooserButton::CModuleChooserButton(CSwordModuleInfo* useModule,CSwordModuleInfo::ModuleType type, const int id, QWidget *parent, const char *name )
	: KToolBarButton(iconName(), id, parent,name), m_id(id), m_popup(0) {

  m_moduleType = type;	
	m_module = useModule;
	if (!m_module) {
  	m_hasModule = false;
  }
  else {
    m_hasModule = true;
  }
	
	setIcon( iconName() );
	setPopupDelay(0001);

	populateMenu();
}	

/** Returns the icon used for the current status. */
const QString CModuleChooserButton::iconName(){
	switch (m_moduleType) {
		case CSwordModuleInfo::Bible:
			if (m_hasModule)
				return CResMgr::modules::bible::icon_unlocked;
			else
				return CResMgr::modules::bible::icon_add;
		case CSwordModuleInfo::Commentary:
			if (m_hasModule)
				return CResMgr::modules::commentary::icon_unlocked;
			else
				return CResMgr::modules::commentary::icon_add;
		case CSwordModuleInfo::Lexicon:
			if (m_hasModule)
				return CResMgr::modules::lexicon::icon_unlocked;
			else
				return CResMgr::modules::lexicon::icon_add;
		case CSwordModuleInfo::GenericBook:
			if (m_hasModule)
				return CResMgr::modules::book::icon_unlocked;
			else
				return CResMgr::modules::book::icon_add;
		default: //return as default the bible icon
			return CResMgr::modules::bible::icon_unlocked;
	}
}

CSwordModuleInfo* CModuleChooserButton::module() {
	for ( KPopupMenu* popup = m_submenus.first(); popup; popup = m_submenus.next() ) {
  	for (unsigned int i = 0; i < popup->count(); i++) {
  		if ( m_popup->isItemChecked(popup->idAt(i)) ){
				QString mod = popup->text(popup->idAt(i));
  			return backend()->findModuleByName( mod.left(mod.find(" ")) );
			}
  	}	

	}
	return 0; //"none" selected
}

/** Returns the id used for this button. */
int CModuleChooserButton::getId() const{
	return m_id;
}

/** Is called after a module was selected in the popup */
void CModuleChooserButton::moduleChosen( int ID ){	
	for ( KPopupMenu* popup = m_submenus.first(); popup; popup = m_submenus.next() ) {
   	for (unsigned int i = 0; i < popup->count(); i++){
   		popup->setItemChecked(popup->idAt(i),false);	
   	}	
   	popup->setItemChecked(ID, true);
	}

	m_popup->setItemChecked(m_noneId, false); //uncheck the "none" item

 	if (m_popup->text(ID) == i18n("NONE")) { // note: this is for m_popup, the toplevel!
 		if (m_hasModule) {
 			emit sigRemoveButton(m_id);
 			return;
		}
 	}
 	else {
 	  if (!m_hasModule) {
 	    emit sigAddButton();
    }
    
 		m_hasModule = true;  	
 		m_module = module();
  	setIcon( iconName() );
   	emit sigChanged();

    setText( i18n("Select a module") );
    m_popup->changeTitle(m_titleId, i18n("Select a module"));	
     	
   	QToolTip::remove(this);
   	if (module()) {
   		QToolTip::add(this, module()->name());
    }
 	}
}
/** No descriptions */
void CModuleChooserButton::populateMenu(){
	if (m_popup)
		delete m_popup;
	m_submenus.setAutoDelete(true);
	m_submenus.clear();

	//create popup
	m_popup = new KPopupMenu(this);	

	if (m_module) {
	  m_titleId = m_popup->insertTitle(i18n("Select a module"));	
	}
	else {
	  m_titleId = m_popup->insertTitle(i18n("Select an additional module"));	
	}

	m_popup->setCheckable(true);

	m_noneId =	m_popup->insertItem(i18n("NONE"));
  if ( !m_module )
		m_popup->setItemChecked(m_noneId, true);

	m_popup->insertSeparator();	
	connect(m_popup, SIGNAL(activated(int)), this, SLOT(moduleChosen(int)));		
	setPopup(m_popup, true);

	QStringList languages;
	QDict<KPopupMenu> langdict;

  //the modules list contains only the modules we can use, i.e. same type and same features
  ListCSwordModuleInfo modules;
  ListCSwordModuleInfo allMods = backend()->moduleList();

  for (allMods.first(); allMods.current(); allMods.next()) {
    if (allMods.current()->type() != m_moduleType)
      continue;
    modules.append(allMods.current());
  };

	for (modules.first(); modules.current(); modules.next()) {
 		QString lang = modules.current()->language()->translatedName();
 		if (lang.isEmpty()) {
 			lang = QString::fromLatin1("xx"); //unknown language -- do not use English as default!!
		}
		
 	 	if (languages.find( lang ) == languages.end() ){ //not yet added
 			languages += lang;
 			KPopupMenu* menu = new KPopupMenu;
 			langdict.insert(lang, menu );
 			m_submenus.append(menu);
 			connect(menu, SIGNAL(activated(int)), this, SLOT(moduleChosen(int)));		
 		}
	}	

	//Check the appropriate entry
	for (modules.first(); modules.current(); modules.next()) {
 		QString lang = modules.current()->language()->translatedName();
 		if (lang.isEmpty()) {
 			lang = QString::fromLatin1("xx"); //unknown language
		}
		
 		QString name = QString(modules.current()->name()) + QString(" ") + (modules.current()->isLocked() ? i18n("[locked]") : QString::null); 			
 		const int id = langdict[lang]->insertItem( name );
 		if ( m_module && modules.current()->name() == m_module->name()) {
 			langdict[lang]->setItemChecked(id,true);
 		}
	}	

	languages.sort();
  for ( QStringList::Iterator it = languages.begin(); it != languages.end(); ++it ) {
		m_popup->insertItem( *it, langdict[*it]);
	}

 	if (module())
 		QToolTip::add(this, module()->name());
	else
 		QToolTip::add(this, i18n("No module selected"));

}
