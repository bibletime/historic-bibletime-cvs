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

#include "csearchdialogmodulechooser.h"
#include "../../structdef.h"
#include "../../backend/sword_backend/cswordbackend.h"

CSearchDialogModuleChooser::CSearchDialogModuleChooser(CImportantClasses* importantClasses, QWidget *parent, const char *name ) : QWidget(parent,name) {
	m_importantClasses = importantClasses;
	m_moduleList = 0;
}

CSearchDialogModuleChooser::~CSearchDialogModuleChooser(){
	if (m_moduleList)
		delete m_moduleList;
}

/** Sets the chosen modules for this object. */
void CSearchDialogModuleChooser::setModuleList( ListCSwordModuleInfo* modules){
	if (!m_moduleList)
		m_moduleList = new ListCSwordModuleInfo;
	*m_moduleList = *modules; //copy the items
}

/** Returns the modules chosen in this widget. */
ListCSwordModuleInfo* CSearchDialogModuleChooser::getModuleList(){
	return m_moduleList;
}
