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
#include "../backend/cswordmoduleinfo.h"
#include "../backend/cswordversekey.h"
#include "presenters/cbiblepresenter.h"
#include "presenters/clexiconpresenter.h"
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

//QT includes
#include <qobjectlist.h>
#include <qtimer.h>
#include <qevent.h>
#include <qwhatsthis.h>

CMDIArea::CMDIArea(CImportantClasses* importantClasses, QWidget *parent, const char *name )
	: QWorkspace(parent, name) {			
	m_important = importantClasses;
	m_childEvent = false;
	m_currentPresenter = 0;
	guiOption = Nothing;
	m_appCaption = QString::null;
	
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
//	qWarning(client->caption().latin1());
	m_appCaption = client->caption().stripWhiteSpace();	
	emit sigSetToplevelCaption( m_appCaption );	
	
	CBiblePresenter* p = dynamic_cast<CBiblePresenter*>(client);
	if (p)
		syncCommentaries( p->getKeyChooser()->getKey() );
}

/** Reimplementation. Used to make use of the fixedGUIOption part. */
void CMDIArea::childEvent ( QChildEvent * e ){
	if (m_childEvent)
		return;	
	QWorkspace::childEvent(e);		
	m_childEvent = true;
	
	if (!windowList().count()) {
		m_appCaption = QString::null;
		emit sigSetToplevelCaption( KApplication::kApplication()->makeStdCaption(m_appCaption) );		
		emit sigLastPresenterClosed();
	}	
	if (!e) {
		m_childEvent = false;
		return;
	}
	
	if (e->inserted() || e->removed()) {
		switch (guiOption) {
	 		case autoTile:
//				QTimer::singleShot( 0, this, SLOT(tile()) );
				tile();
	 			break;
	 		case autoCascade:
//				QTimer::singleShot( 0, this, SLOT(cascade()) );
				cascade();
	 			break;
	 		default:
	 			break;
		}
	}

	m_childEvent = false;
}

/** Reimplementation */
void CMDIArea::resizeEvent(QResizeEvent* e){	
	QWorkspace::resizeEvent(e);	
	switch (guiOption) {
 		case autoTile:
 			QTimer::singleShot( 0, this, SLOT(tile()) );
//			tile();
 			break;
 		case autoCascade:
 			QTimer::singleShot( 0, this, SLOT(cascade()) );
//			cascade();
 			break;
 		default:
 			break;
	}
}

/**  */
void CMDIArea::saveSettings(){
}

/**  */
void CMDIArea::readSettings(){
}

/** Deletes all the presenters in the MDI area. */
void CMDIArea::deleteAll(){
	QWidgetList windows = windowList();
	
	setUpdatesEnabled(false);
	for ( QWidget* w = windows.first(); w; w = windows.next() ) {
		w->setUpdatesEnabled(false);
		delete w;
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
	if (!isUpdatesEnabled() || !windowList().count() )	
		return;
	if (windowList().count() == 1 /*&& !windowList().at(0)->isHidden() && !windowList().at(0)->isMinimized()*/) {
		m_appCaption = windowList().at(0)->caption();
		windowList().at(0)->showMaximized();
	}
	else
		QWorkspace::tile();
}

/**  */
void CMDIArea::cascade(){
	if (!isUpdatesEnabled() || !windowList().count() )
		return;		
	if (windowList().count() == 1 /*&& !windowList().at(0)->isHidden() && !windowList().at(0)->isMinimized()*/) {
		m_appCaption = windowList().at(0)->caption();		
		windowList().at(0)->showMaximized();
	}
 	else
		QWorkspace::cascade();
}

/** Sync the commentaries to the given key. */
void CMDIArea::syncCommentaries(CSwordKey* syncKey){
	QWidgetList windows = windowList();	
	if (!windows.count())
		return;	
		
	for (windows.first(); windows.current(); windows.next()) {
		CCommentaryPresenter* p = dynamic_cast<CCommentaryPresenter*>(windows.current());
		if (p)
			p->synchronize(syncKey);
	}	
}

/** Look up the text in the module. If the module has already a display window of it opne use it, otherwise create a new one. */
void CMDIArea::lookupInLexicon(const QString& text, const QString& module){
//	qWarning("CMDIArea::lookupInLexicon(const QString& text, const QString& module)");
	CSwordModuleInfo* m = m_important->swordBackend->findModuleByName(module);
	if (!m)
		return;
	CLexiconPresenter* p = 0;
	
	QWidgetList windows = windowList();	
	if (!windows.count())
		return;
	bool found = false;
	for (windows.first(); windows.current(); windows.next()) {
		p = dynamic_cast<CLexiconPresenter*>(windows.current());
		if (p && (m == p->getModuleList().first())) {
//			qWarning("found");
			found = true;
			break;
		}
	}
	if (!found)
		emit createNewSwordPresenter(m, text);
	else
		p->lookup(module, text);
}

/** Look up the text in the module. If the module has already a display window of it opne use it, otherwise create a new one. */
void CMDIArea::lookupInModule(const QString& module, const QString& key){
//	qWarning("CMDIArea::lookupInLexicon(const QString& text, const QString& module)");
	CSwordModuleInfo* m = m_important->swordBackend->findModuleByName(module);
	if (!m)
		return;
	CSwordPresenter* p = 0;

	bool found = false;	
	QWidgetList windows = windowList();	
//	if (!windows.count())
//		return;
	for (windows.first(); windows.current(); windows.next()) {
		p = dynamic_cast<CSwordPresenter*>(windows.current());
		if (p && (p->getModuleList().containsRef(m))) {
			qWarning("found");
			found = true;
			break;
		}
	}
	if (!found)
		emit createNewSwordPresenter(m, key);
	else
		p->lookup(module, key);
}
/** Closes and deletes the presenter given as argument. */
void CMDIArea::closePresenter(CSwordPresenter* p){
	if (!p)
		return;
	m_currentPresenter = p;
  QTimer::singleShot(5000, this, SLOT(deleteCurrentPresenter()) );	
}

/** Delete the presenter. */
void CMDIArea::deleteCurrentPresenter(){
	setUpdatesEnabled(false);
	if (m_currentPresenter) {
		delete m_currentPresenter;
		m_currentPresenter = 0;
	}
	setUpdatesEnabled(true);
//	switch (guiOption) {
// 		case autoTile:
//			tile();
// 			break;
// 		case autoCascade:
//			cascade();
// 			break;
// 		default:
// 			break;
//	}	
	slotClientActivated(activeWindow());
	if (activeWindow())
		m_appCaption = activeWindow()->caption();
}

/** This works around a problem/limitation in QWorkspace. QWorkspace sets every time the  application caption on its on way. This confuses BibleTime - wrong captions are generated. This function returns the right caption (using the MDI child). */
const QString CMDIArea::currentApplicationCaption() const {
	return m_appCaption;
}
