/***************************************************************************
                          cmodulepresenter.cpp  -  description
                             -------------------
    begin                : Wed Jan 19 2000
    copyright            : (C) 2000 by The BibleTime Team
    email                : Info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "cmodulepresenter.h"
#include "../../ressource.h"

//KDE includes
#include <kapp.h>
#include <ktoolbar.h>

CModulePresenter::CModulePresenter(CImportantClasses* importantClasses, QWidget *parent, const char *name, CModuleInfo* module,  WFlags f )
	: KMainWindow(parent, name, f)  {

	m_swordModule = 0;
	m_doSynchronize = false;
	
	if ((CSwordModuleInfo*)module)
		m_swordModule = (CSwordModuleInfo*)module;
	config = kapp->config();
	resize(400,400);
	m_important = importantClasses;	
	ASSERT(m_important->swordBackend);
}

CModulePresenter::~CModulePresenter(){
	qDebug("deleting presenter...");
}

/**  */
void CModulePresenter::setCaption(const QString& caption){
	setPlainCaption(caption);
}

QString	CModulePresenter::caption() const {
	return "Unknown";
}

/** Refresh the content of this module */
void CModulePresenter::refresh( const int useFeatures ){
}

/**  */
void CModulePresenter::readSettings(){
}

/**  */
void CModulePresenter::saveSettings(){
}

/**  */
void CModulePresenter::copy(){
}

/**  */
void CModulePresenter::paste(){
}

/**  */
void CModulePresenter::cut(){
}

/** Returns the features provided by the presenter. */
int CModulePresenter::getAvailableFeatures() const {
//	qDebug( QString::number(availableFeatures) );
	return availableFeatures;
}

/** Sets the features provided by this presenter. */
void CModulePresenter::setAvailableFeatures( const int features ) {
//	qDebug( QString::number(features) );
	availableFeatures = features;
}

/** Returns true if synchroniziation is enabled for this presenter. */
bool CModulePresenter::synchronizeEnabled() {
	qDebug("CModulePresenter::synchronizeEnabled()");
	qDebug(m_doSynchronize ? "true" : "false");
	return m_doSynchronize;
}
