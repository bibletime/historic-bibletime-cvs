/***************************************************************************
                          cmdiarea.cpp  -  description
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

#include "cmdiarea.h"
#include "../backend/ckey.h"
#include "../backend/sword_backend/cswordversekey.h"
#include "presenters/cbiblepresenter.h"
#include "presenters/ccommentarypresenter.h"
#include "keychooser/ckeychooser.h"
#include "../ressource.h"
#include "../whatsthisdef.h"
#include "../structdef.h"
#include "../../config.h"

//KDE includes
#include <kpopupmenu.h>
#include <kapp.h>
#include <klocale.h>
#include <kconfig.h>

//QT includes
#include <qobjectlist.h>
#include <qtimer.h>
#include <qevent.h>
#include <qwhatsthis.h>

CMDIArea::CMDIArea(CImportantClasses* importantClasses, QWidget *parent, const char *name )
	: QWorkspace(parent, name) {
	m_important = importantClasses;
	m_childEvent = false;	
	config = KGlobal::config();	
	
	guiOption = Nothing;
	initView();
	initConnections();
	readSettings();
}

CMDIArea::~CMDIArea(){
}

/** Initializes the view of the MDI area */
void CMDIArea::initView(){
	QWhatsThis::add(this, WT_MDI_AREA_WIDGET );	
}

/** Initilizes the connectiosn to SIGNALS */
void CMDIArea::initConnections(){
	connect(this, SIGNAL(windowActivated(QWidget*)),
		this, SLOT(slotClientActivated(QWidget*)));
}

/** Called whan a client window was activated */
void CMDIArea::slotClientActivated(QWidget* client){
	if (!client)
		return;
				
	if (windowList().count())
		emit sigSetToplevelCaption( QString("%1").arg(client->caption().stripWhiteSpace()) );	
	if (client->isA("CBiblePresenter")) {
		CBiblePresenter* p = dynamic_cast<CBiblePresenter*>(client);
		syncCommentaries( p->getKeyChooser()->getKey() );
	}
}

/** Reimplementation. Used to make use of the fixedGUIOption part. */
void CMDIArea::childEvent ( QChildEvent * e ){
	if (m_childEvent) {
		return;
	}	
	QWorkspace::childEvent(e);	
	
	m_childEvent = true;
	
	if (!windowList().count()) {
		emit sigLastPresenterClosed();
		emit sigSetToplevelCaption( QString("BibleTime %1").arg(VERSION) );
	}	
	
	if (e->inserted() || e->removed()) {
		switch (guiOption) {
	 		case autoTile:
	 				resizeEvent(0);
	 			break;
	 		case autoCascade:
	 				resizeEvent(0);
	 			break;
	 		case Nothing:
	 			break;
		}
	}
	
	if (e->type() == QEvent::ShowMaximized) {
		QWidget* c = (QWidget*)e->child();
		if (c)
			emit sigSetToplevelCaption( QString("BibleTime [%1]").arg(c->caption()) );	
	}
	m_childEvent = false;
}

/** Reimplementation */
void CMDIArea::resizeEvent(QResizeEvent* e){	
	QWorkspace::resizeEvent(e);	
	switch (guiOption) {
 		case autoTile:
 			QTimer::singleShot( 0, this, SLOT(tile()) );
 			break;
 		case autoCascade:
 			QTimer::singleShot( 0, this, SLOT(cascade()) );
 			break;
 		case Nothing:
 			break;
	}
}

/**  */
void CMDIArea::saveSettings(){
	KConfigGroupSaver groupSaver(config, "MDI");
}

/**  */
void CMDIArea::readSettings(){
	KConfigGroupSaver groupSaver(config, "MDI");
}

/** Deletes all the presenters in the MDI area. */
void CMDIArea::deleteAll(){
	QWidgetList windows = windowList();

	setUpdatesEnabled(false);			
	int count = windows.count();
	for ( int i = 0; i < count; ++i ) {
		if ( windows.at(i) )
			delete windows.at(i);
	}
	setUpdatesEnabled(true);
}

/** Enable / disable autoCascading */
void CMDIArea::setGUIOption( mdiOption new_GUIOption){
	guiOption = new_GUIOption;
	//now do the initial action
	switch (guiOption) {
 		case autoTile:
			tile();
 			break;
 		case autoCascade:
			cascade();
 			break;
 		case Nothing:
 			break;
	}	
}

/**  */
void CMDIArea::tile(){
	if (!isUpdatesEnabled() || (windowList().count() == 0) )	
		return;
	
	if (windowList().count() == 1)
		windowList().at(0)->showMaximized();
	else
		QWorkspace::tile();
}

/**  */
void CMDIArea::cascade(){
	if (!isUpdatesEnabled() || (windowList().count() == 0) )	
		return;
		
	if (windowList().count() == 1)
		windowList().at(0)->showMaximized();
 	else
		QWorkspace::cascade(); 		
}

/** Sync the commentaries to the given key. */
void CMDIArea::syncCommentaries(CKey* syncKey){
	QWidgetList windows = windowList();	
	if (!windows.count())
		return;
	
	for (windows.first(); windows.current(); windows.next()) {
		if (windows.current()->isA("CCommentaryPresenter")) {
			CCommentaryPresenter* p = (CCommentaryPresenter*)windows.current();
			p->synchronize(syncKey);
		}
	}	
}
