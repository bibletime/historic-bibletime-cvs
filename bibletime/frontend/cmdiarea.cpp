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
#include "backend/cswordmoduleinfo.h"
#include "backend/cswordversekey.h"
#include "frontend/displaywindow/cdisplaywindow.h"
//#include "presenters/cbiblepresenter.h"
//#include "presenters/clexiconpresenter.h"
//#include "presenters/ccommentarypresenter.h"
#include "keychooser/ckeychooser.h"
#include "resource.h"
#include "whatsthisdef.h"
#include "config.h"

//KDE includes
#include <kpopupmenu.h>
#include <kapplication.h>
#include <klocale.h>

//QT includes
#include <qobjectlist.h>
#include <qtimer.h>
#include <qevent.h>
#include <qwhatsthis.h>

CMDIArea::CMDIArea(QWidget *parent, const char *name )
	: QWorkspace(parent, name) {			
	m_childEvent = false;
  m_deleting = false;
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
	setPaletteBackgroundColor( parentWidget()->paletteBackgroundColor() );	
}

/** Initilizes the connectiosn to SIGNALS */
void CMDIArea::initConnections(){
	connect(this, SIGNAL(windowActivated(QWidget*)),
		this, SLOT(slotClientActivated(QWidget*)));
}

/** Called whan a client window was activated */
void CMDIArea::slotClientActivated(QWidget* client){
//	qWarning("CMDIArea::slotClientActivated(QWidget* client)");
	if (!client || !isUpdatesEnabled()) {
//		qWarning("client Activated: return");
    return;
  }

	CDisplayWindow* sp = dynamic_cast<CDisplayWindow*>(client);	
	if (sp && !sp->isReady())
		return;

  QWidgetList windows = windowList();
  for ( QWidget* w = windows.first(); w; w = windows.next() ) {		
   	CDisplayWindow* window = dynamic_cast<CDisplayWindow*>(w);	
		window->windowActivated( (window == sp) ? true : false);
	}	
	
	emit sigSetToplevelCaption( ( m_appCaption = client->caption().stripWhiteSpace() ) );	

#warning Check!
//	CBiblePresenter* p = dynamic_cast<CBiblePresenter*>(client);
//	if (p && p->keyChooser())
//		syncCommentaries( p->keyChooser()->key() );
}

/** Reimplementation. Used to make use of the fixedGUIOption part. */
void CMDIArea::childEvent( QChildEvent * e ){
	QWorkspace::childEvent(e);

	if ( m_childEvent || !isUpdatesEnabled() || !e)
		return;	

	m_childEvent = true;
	
	if (!windowList().count()) {
		m_appCaption = QString::null;
		emit sigSetToplevelCaption( KApplication::kApplication()->makeStdCaption(m_appCaption) );		
		emit sigLastPresenterClosed();
	}	
	
  if (!m_deleting && (e->inserted() || e->removed()) ) {
		switch (guiOption) {
	 		case autoTile:
				if (isUpdatesEnabled())
					tile();
	 			break;
	 		case autoCascade:
				if (isUpdatesEnabled())
					cascade();
	 			break;
	 		default:
	 			break;
		}
	}

  m_childEvent = false;
  if (!windowList().count()) { //already deleted all windows
    m_deleting = false;
  }
}

/** Reimplementation */
void CMDIArea::resizeEvent(QResizeEvent* e){	
	QWorkspace::resizeEvent(e);	
	switch (guiOption) {
 		case autoTile:
			if (isUpdatesEnabled())
				tile();
 			break;
 		case autoCascade:
			if (isUpdatesEnabled())
				cascade();
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
  m_deleting = true;
  QWidgetList windows = windowList();
  windows.setAutoDelete(true);
  windows.clear();
}

/** Enable / disable autoCascading */
void CMDIArea::setGUIOption( mdiOption new_GUIOption){
	guiOption = new_GUIOption;
	//now do the initial action
 	
	switch (guiOption) {
 		case autoTile:
			if (isUpdatesEnabled())
				tile();
 			break;
 		case autoCascade:
			if (isUpdatesEnabled())
				cascade();
 			break;
 		default:
 			break;
 	}
}

/**  */
void CMDIArea::tile(){
//  qWarning("CMDIArea::tile()");
	if (!isUpdatesEnabled() || !windowList().count() )	
		return;

	if (windowList().count() == 1 && windowList().at(0)) {
		m_appCaption = windowList().at(0)->caption();
		windowList().at(0)->showMaximized();
	}
	else
		QWorkspace::tile();
}

/**  */
void CMDIArea::cascade(){
//  qWarning("CMDIArea::cascade()");
	if (!isUpdatesEnabled() || !windowList().count() )
		return;		
	if (windowList().count() == 1 && windowList().at(0)) {	
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
	
#warning check!
  //	for (windows.first(); windows.current(); windows.next()) {
//		CCommentaryPresenter* p = dynamic_cast<CCommentaryPresenter*>(windows.current());
//		if (p)
//			p->synchronize(syncKey);
//	}	
}

/** Look up the text in the module. If the module has already a display window of it opne use it, otherwise create a new one. */
void CMDIArea::lookupInLexicon(const QString& text, const QString& module){
//	qWarning("CMDIArea::lookupInLexicon(const QString& text, const QString& module)");
	CSwordModuleInfo* m = backend()->findModuleByName(module);
	if (!m)
		return;
#warning check!
//	CLexiconPresenter* p = 0;
//	
//	QWidgetList windows = windowList();	
//	if (!windows.count())
//		return;
//	bool found = false;
//	for (windows.first(); windows.current(); windows.next()) {
//		p = dynamic_cast<CLexiconPresenter*>(windows.current());
//		if (p && (m == p->getModuleList().first())) {
//			found = true;
//			break;
//		}
//	}
//	if (!found)
//		emit createDisplayWindow(m, text);
//	else
//		p->lookup(module, text);
}

/** Look up the text in the module. If the module has already a display window of it opne use it, otherwise create a new one. */
void CMDIArea::lookupInModule(const QString& module, const QString& key){
	CSwordModuleInfo* m = backend()->findModuleByName(module);
	if (!m)
		return;

#warning check!
//	CSwordPresenter* p = 0;
//
//	bool found = false;	
//	QWidgetList windows = windowList();	
//	for (windows.first(); windows.current(); windows.next()) {
//		p = dynamic_cast<CSwordPresenter*>(windows.current());
//		if (p && (p->getModuleList().containsRef(m))) {
//			found = true;
//			break;
//		}
//	}
//	if (!found)
//		emit createDisplayWindow(m, key);
//	else
//		p->lookup(module, key);
}

/** This works around a problem/limitation in QWorkspace. QWorkspace sets every time the  application caption on its on way. This confuses BibleTime - wrong captions are generated. This function returns the right caption (using the MDI child). */
const QString CMDIArea::currentApplicationCaption() const {
	return m_appCaption;
}
