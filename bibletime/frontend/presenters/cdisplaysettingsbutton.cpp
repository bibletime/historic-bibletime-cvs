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
#include "../../backend/cswordmoduleinfo.h"

#include <qstring.h>
#include <qtooltip.h>

#include <kpopupmenu.h>
#include <klocale.h>

CDisplaySettingsButton::CDisplaySettingsButton(CSwordBackend::displayOptionsBool &displaySettings, CSwordBackend::moduleOptionsBool &moduleSettings,
	ListCSwordModuleInfo useModules,QWidget *parent, const char *name ) : QToolButton(parent,name)
{
	m_displaySettings = &displaySettings;
	m_moduleSettings = &moduleSettings;

	m_dict.insert( i18n("Use linebreaks"), 		&(m_displaySettings->lineBreaks) );
	m_dict.insert( i18n("Show versenumbers"), &(m_displaySettings->verseNumbers) );

	m_dict.insert( i18n("Show footnotes"), 									&(m_moduleSettings->footnotes) );
	m_dict.insert( i18n("Show Strong's Numbers"), 					&(m_moduleSettings->strongNumbers) );
	m_dict.insert( i18n("Show headings"), 									&(m_moduleSettings->headings) );

	m_dict.insert( i18n("Show morphologic tags"), 					&(m_moduleSettings->morphTags) );
	m_dict.insert( i18n("Show Hebrew vowel points"), 				&(m_moduleSettings->hebrewPoints) );
	m_dict.insert( i18n("Show Hebrew cantillation marks,"), &(m_moduleSettings->hebrewCantillation) );
	m_dict.insert( i18n("Show Greek accents"), 							&(m_moduleSettings->greekAccents) );

 	m_popup = new KPopupMenu(this);	
	m_popup->insertTitle(i18n("Display options"));
	m_popup->setCheckable(true);

	m_popup->insertSeparator();	
	connect(m_popup, SIGNAL(activated(int)), this, SLOT(optionToggled(int)));

	QDictIterator<bool> it( m_dict ); // iterator for dict
	int ID;
	while ( it.current() ) {
		ID = m_popup->insertItem( it.currentKey() );
		m_popup->setItemChecked( ID, *(m_dict[it.currentKey()]) );
		++it;
	}
}

void CDisplaySettingsButton::optionToggled(int ID){
  *(m_dict[m_popup->text(ID)]) =  m_popup->isItemChecked(ID);
}


CDisplaySettingsButton::~CDisplaySettingsButton(){
}
