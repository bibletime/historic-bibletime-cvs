/***************************************************************************
                          cdisplaysettingsbutton.cpp  -  description
                             -------------------
    begin                : Thu Nov 8 2001
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

#include "cdisplaysettingsbutton.h"
#include "../../ressource.h"

#include <qstring.h>
#include <qtooltip.h>

#include <kpopupmenu.h>
#include <klocale.h>

CDisplaySettingsButton::CDisplaySettingsButton(CSwordBackend::displayOptionsBool *displaySettings, CSwordBackend::moduleOptionsBool *moduleSettings,
	ListCSwordModuleInfo useModules,QWidget *parent, const char *name ) : QToolButton(parent,name)
{
	m_displaySettings = displaySettings;
	m_moduleSettings = moduleSettings;
	m_modules = useModules;

	setPixmap( BIBLE_ICON_MC );

 	m_popup = new KPopupMenu(this);	
	setPopup(m_popup);
	setPopupDelay(0);

	connect(m_popup, SIGNAL(activated(int)), this, SLOT(optionToggled(int)));

	populateMenu();
//	if (!populateMenu())
//		setEnabled(false);
}

void CDisplaySettingsButton::reset(ListCSwordModuleInfo useModules){
	m_modules = useModules;
	populateMenu();
//	if (!populateMenu())
//		setEnabled(false);
//	else
//		setEnabled(true);
}


void CDisplaySettingsButton::optionToggled(int ID){
	m_popup->setItemChecked( ID, !(m_popup->isItemChecked(ID)));
	if (!m_popup->text(ID).isEmpty())
	  *(m_dict[m_popup->text(ID)]) =  m_popup->isItemChecked(ID);
	emit sigChanged();
}

/** No descriptions */
int CDisplaySettingsButton::populateMenu(void){

	int ret = 0;

	m_popup->clear();
	m_popup->insertTitle(i18n("Display options"));
	m_popup->setCheckable(true);

	ret += addMenuEntry(i18n("Show headings"),	&(m_moduleSettings->headings),
		isOptionAvailable(CSwordBackend::headings));
  ret += addMenuEntry(i18n("Use linebreaks"), &(m_displaySettings->lineBreaks), true);
	ret += addMenuEntry(i18n("Show versenumbers"), &(m_displaySettings->verseNumbers), true);

	ret += addMenuEntry(i18n("Show footnotes"), &(m_moduleSettings->footnotes),
		isOptionAvailable(CSwordBackend::footnotes ));
	ret += addMenuEntry(i18n("Show Strong's Numbers"), &(m_moduleSettings->strongNumbers),
		isOptionAvailable(CSwordBackend::strongNumbers ));

	ret += addMenuEntry(i18n("Show morphologic tags"), &(m_moduleSettings->morphTags),
		isOptionAvailable(CSwordBackend::morphTags ));
	ret += addMenuEntry(i18n("Show lemmas"), &(m_moduleSettings->lemmas),
		isOptionAvailable(CSwordBackend::lemmas ));
	ret += addMenuEntry(i18n("Show Hebrew vowel points"), &(m_moduleSettings->hebrewPoints),
		isOptionAvailable(CSwordBackend::hebrewPoints ));
	ret += addMenuEntry(i18n("Show Hebrew cantillation marks"), &(m_moduleSettings->hebrewCantillation),
		isOptionAvailable(CSwordBackend::hebrewCantillation ));
	ret += addMenuEntry(i18n("Show Greek accents"), &(m_moduleSettings->greekAccents),
		isOptionAvailable(CSwordBackend::greekAccents ));

	return ret;
}

/** No descriptions */
int CDisplaySettingsButton::addMenuEntry( QString name, bool* option, bool available){
	if (available){
		m_dict.insert( name, option);
		m_popup->setItemChecked(m_popup->insertItem( name ), /**(m_dict[name])*/*option );
		return 1;
	}
	return 0;
}

bool CDisplaySettingsButton::isOptionAvailable( CSwordBackend::moduleOptions option){
	bool ret = false;
	for (m_modules.first(); m_modules.current(); m_modules.next())
		ret = ret || m_modules.current()->supportsFeature(option);
	return ret;
}

/** Returns the number of usable menu items in the setttings menu. */
const int CDisplaySettingsButton::menuItemCount(){
	return m_popup->count();
}

/** Sets the item at position pos to the satet given as 2nd paramter. */
void CDisplaySettingsButton::setItemStatus( const int index, const bool checked ){
	qWarning("CDisplaySettingsButton::setItemStatus");
	const int ID = m_popup->idAt(index);
	m_popup->setItemChecked(ID, checked);
	const QString text = m_popup->text(ID);
	qWarning("set %c to %i",text.local8Bit(), checked);
	
	
	if (m_dict[text])
	  *(m_dict[text]) = checked;
}

/** Returns the status of the item at position "index" */
const bool CDisplaySettingsButton::itemStatus( const int index ){
	return m_popup->isItemChecked( m_popup->idAt(index) );
}

/** Sets the status to changed. The signal changed will be emitted. */
void CDisplaySettingsButton::setChanged(){
	emit sigChanged();	
}