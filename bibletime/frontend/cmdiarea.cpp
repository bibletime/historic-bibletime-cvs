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
//#include "presenters/cmodulepresenter.h"
//#include "presenters/cbiblepresenter.h"
#include "../ressource.h"
#include "../whatsthisdef.h"
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
	qDebug("constructor of CMDIArea");
	m_important = importantClasses;
	m_childEvent = false;
	
	config = KGlobal::config();	
	guiOption = Nothing;
	initView();
	initConnections();
	readSettings();
}

CMDIArea::~CMDIArea(){
	qDebug("destructor of CMDIArea");
//	saveSettings();
}

/** Initializes the view of the MDI area */
void CMDIArea::initView(){
//	qDebug("CMDIArea::initView");
//QWhatsThis::add(this, WT_MDI_AREA_WIDGET );	
}

/** Initilizes the connectiosn to SIGNALS */
void CMDIArea::initConnections(){
	connect(this, SIGNAL(windowActivated(QWidget*)),
		this, SLOT(slotClientActivated(QWidget*)));
}

/** Called whan a client window was activated */
void CMDIArea::slotClientActivated(QWidget* client){
	qDebug("CMDIArea::slotClientActivated(QWidget* client)");
	ASSERT(client);
	if (!client) {
		qDebug("client is 0, return");
		return;
	};
	if (windowList().count() > 1 ) {
		emit sigSetToplevelCaption( QString("BibleTime - [%1]").arg(client->caption()) );	
	}
}

/** Reimplementation. Used to make use of the fixedGUIOption part. */
void CMDIArea::childEvent ( QChildEvent * e ){
	qDebug("CMDIARea::childEvent");	
	if (m_childEvent) {
		qDebug("already in child event, return");
		return;
	}	
	QWorkspace::childEvent(e);	
	
	m_childEvent = true;
	
	if (windowList().count() == 0) {
		emit sigLastPresenterClosed();
		emit sigSetToplevelCaption( QString("BibleTime " VERSION) );
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
	m_childEvent = false;
}

/** Reimplementation */
void CMDIArea::resizeEvent(QResizeEvent* e){	
	qDebug("## CMDIARea::resizeEvent");	
	QWorkspace::resizeEvent(e);	

	switch (guiOption) {
 		case autoTile:
//			tile();
 			QTimer::singleShot( 0, this, SLOT(tile()) );
 			break;
 		case autoCascade:
//			cascade();	 			
 			QTimer::singleShot( 0, this, SLOT(cascade()) );
 			break;
 		case Nothing:
 			break;
	}
}

/**  */
void CMDIArea::saveSettings(){
	KConfigGroupSaver groupSaver(config, "MDI");
	qDebug("CMDIArea::saveSettings");
}

/**  */
void CMDIArea::readSettings(){
	KConfigGroupSaver groupSaver(config, "MDI");
	qDebug("CMDIArea::readSettings");		
}

/** Deletes all the presenters in the MDI area. */
void CMDIArea::deleteAll(){
	qDebug("CMDIArea::deleteAll");
	QWidgetList windows = windowList();
	qDebug("%d entries in list",windows.count());	

	this->setUpdatesEnabled(false);			
	int count = windows.count();
	for ( int i = 0; i < count; ++i ) {
		if ( windows.at(i) )
			delete windows.at(i);
	}
	this->setUpdatesEnabled(true);
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

///** Scrolls all presenters which are able to do parallel scrolling to the given position. */
//void CMDIArea::scrollPresentersTo(const QString& book, const int chapter, const int verse, CModulePresenter* caller){
//	QWidgetList windows = windowList();
//	
//	if (windows.count() < 2) {
//		return;
//	}
//	
//	for ( int i = 0; i < int(windows.count()); ++i ) {
//		if ( windows.at(i)->inherits("CModulePresenter") ) {
//			CModulePresenter* myPresenter = (CModulePresenter*)windows.at(i);
//			if (!myPresenter) {
//				continue;
//			}
//			else if ( (myPresenter->getAvailableFeatures() & CModulePresenter::autoScrolling) == false ) {
////				qDebug("CMDIArea::scrollPresentersTo: Presenter without set autoScrolling flag. Continue.");
//				continue;
//			}				
//			else if (myPresenter == caller) {
//				continue;
//			}
//			else if ( !myPresenter->hasFocus() ) {
//				myPresenter->scrollToKey(book, chapter, verse);
//			}
//		}
//	}
//}

/**  */
void CMDIArea::tile(){
	qDebug("CMDIArea::tile()");	
	if (!isUpdatesEnabled() || (windowList().count() == 0) )	
		return;
	
	if (windowList().count() == 1)
		windowList().at(0)->showMaximized();
	else
		QWorkspace::tile();
}

/**  */
void CMDIArea::cascade(){
	qDebug("CMDIArea::cascade()");
	if (!isUpdatesEnabled() || (windowList().count() == 0) )	
		return;
		
	if (windowList().count() == 1)
		windowList().at(0)->showMaximized();
 	else
		QWorkspace::cascade(); 		
}
