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

 	m_popup = new KPopupMenu(this);	
	setPopup(m_popup);
	setPopupDelay(0);

	connect(m_popup, SIGNAL(activated(int)), this, SLOT(optionToggled(int)));

	populateMenu();
}
void CDisplaySettingsButton::reset(ListCSwordModuleInfo useModules){
	m_modules = useModules;
	populateMenu();
}


void CDisplaySettingsButton::optionToggled(int ID){
	m_popup->setItemChecked( ID, !(m_popup->isItemChecked(ID)));
  *(m_dict[m_popup->text(ID)]) =  m_popup->isItemChecked(ID);
}

/** No descriptions */
void CDisplaySettingsButton::populateMenu(void){

	m_popup->clear();
	m_popup->insertTitle(i18n("Display options"));
	m_popup->setCheckable(true);

	addMenuEntry(i18n("Show headings"),	&(m_moduleSettings->headings),
		isOptionAvailable(CSwordBackend::headings));
  addMenuEntry(i18n("Use linebreaks"), &(m_displaySettings->lineBreaks), true);
	addMenuEntry(i18n("Show versenumbers"), &(m_displaySettings->verseNumbers), true);

	addMenuEntry(i18n("Show footnotes"), &(m_moduleSettings->footnotes),
		isOptionAvailable(CSwordBackend::footnotes ));
	addMenuEntry(i18n("Show Strong's Numbers"), &(m_moduleSettings->strongNumbers),
		isOptionAvailable(CSwordBackend::strongNumbers ));

	addMenuEntry(i18n("Show morphologic tags"), &(m_moduleSettings->morphTags),
		isOptionAvailable(CSwordBackend::morphTags ));
	addMenuEntry(i18n("Show lemmas"), &(m_moduleSettings->lemmas),
		isOptionAvailable(CSwordBackend::lemmas ));
	addMenuEntry(i18n("Show Hebrew vowel points"), &(m_moduleSettings->hebrewPoints),
		isOptionAvailable(CSwordBackend::hebrewPoints ));
	addMenuEntry(i18n("Show Hebrew cantillation marks"), &(m_moduleSettings->hebrewCantillation),
		isOptionAvailable(CSwordBackend::hebrewCantillation ));
	addMenuEntry(i18n("Show Greek accents"), &(m_moduleSettings->greekAccents),
		isOptionAvailable(CSwordBackend::greekAccents ));
}

/** No descriptions */
void CDisplaySettingsButton::addMenuEntry( QString name, bool* option, bool available){
	m_dict.insert( name, option);
	if (available)
		m_popup->setItemChecked(   m_popup->insertItem( name ), *(m_dict[name]) );
}

bool CDisplaySettingsButton::isOptionAvailable( CSwordBackend::moduleOptions option){
	bool ret = false;
	for (m_modules.first(); m_modules.current(); m_modules.next())
		ret = ret || m_modules.current()->supportsFeature(option);
	return ret;
}


