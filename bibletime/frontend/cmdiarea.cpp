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
#include "frontend/keychooser/ckeychooser.h"

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
#include <qtimer.h>


CMDIArea::CMDIArea(QWidget *parent, const char *name )
	: QWorkspace(parent, name),
    m_guiOption(Nothing),
    m_childEvent(false),
    m_deleting(false),
    m_appCaption(QString::null)
{			
	initView();
	initConnections();
	readSettings();
  setAcceptDrops(true);
}

CMDIArea::~CMDIArea(){
}

/** Initializes the view of the MDI area */
void CMDIArea::initView(){
//	QWhatsThis::add(this, WT_MDI_AREA_WIDGET );
	setPaletteBackgroundColor( parentWidget()->paletteBackgroundColor() );	//work around a KDE bug (IMHO was in KDE 2.x)
}

/** Initilizes the connectiosn to SIGNALS */
void CMDIArea::initConnections(){
	connect(this, SIGNAL(windowActivated(QWidget*)),
		this, SLOT(slotClientActivated(QWidget*)));
}

/** Called whan a client window was activated */
void CMDIArea::slotClientActivated(QWidget* client){
	if (!client || !isUpdatesEnabled()) {
    return;
  }

	CDisplayWindow* sp = dynamic_cast<CDisplayWindow*>(client);	
	if (!sp || !sp->isReady())
		return;

  QWidgetList windows = windowList();
  for ( QWidget* w = windows.first(); w; w = windows.next() ) {

//Don't use!! It would disable accel enabling for the active window, see CDisplayWindow::windowActivated
/*    if (w == client)
        continue;
*/

   	CDisplayWindow* window = dynamic_cast<CDisplayWindow*>(w);	
		window->windowActivated( (window == sp) ? true : false);
	}	
	
	emit sigSetToplevelCaption( ( m_appCaption = client->caption().stripWhiteSpace() ) );	
}

/** Reimplementation. Used to make use of the fixedGUIOption part. */
void CMDIArea::childEvent( QChildEvent * e ){
	QWorkspace::childEvent(e);

	if ( m_childEvent || !isUpdatesEnabled() || !e) {
    if (windowList().count() == 0) {
      m_deleting = false;
    }
		return;
  }
  
	m_childEvent = true;
	
	if (!windowList().count()) {
		m_appCaption = QString::null;
		emit sigSetToplevelCaption( KApplication::kApplication()->makeStdCaption(m_appCaption) );
		emit sigLastPresenterClosed();
	}	

  if (!m_deleting && isUpdatesEnabled() && (e->inserted() || e->removed()) ) {
		switch (m_guiOption) {
	 		case autoTile:
				QTimer::singleShot(0, this, SLOT(myTile()));
	 			break;
	 		case autoCascade:
				myCascade();
	 			break;
	 		default:
	 			break;
		}

	}

  m_childEvent = false;

  if (windowList().count() == 0) {
    m_deleting = false;
  }
}

/** Reimplementation */
void CMDIArea::resizeEvent(QResizeEvent* e){
	QWorkspace::resizeEvent(e);	

  if (m_deleting || !isUpdatesEnabled()) {
    return;
  };
  
  switch (m_guiOption) {
 		case autoTile:
			myTile();
 			break;
 		case autoCascade:
			myCascade();
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
  /*
  will be set to false in childEvent, deleting windows is processed in the background, i.e.
  control leaves this function
  */
  m_deleting = true;

  QWidgetList windows( windowList() ); //copy pointers
  windows.setAutoDelete( false );
  const int count = windows.count();
  for (int i = count-1; i >= 0; --i) {
    QWidget* w = windows.at(i);
    windows.remove(i);
    delete w;
  };

//  m_deleting = false;
}

/** Enable / disable autoCascading */
void CMDIArea::setGUIOption( const MDIOption& newOption ){
	//now do the initial action
	switch (( m_guiOption = newOption )) { //set new value and decide what to do
 		case autoTile:
			myTile();
 			break;
 		case autoCascade:
			myCascade();
 			break;
 		default:
 			break;
 	}
}

/**  */
void CMDIArea::myTile(){
	if (m_deleting || !isUpdatesEnabled() || !windowList().count() )	{
    return;
  }

	if ((windowList().count() == 1) && windowList().at(0)) {
		m_appCaption = windowList().at(0)->caption();
		windowList().at(0)->showMaximized();
	}
	else {
	  tile();	
  }
}

/**  */
void CMDIArea::myCascade(){
	if (m_deleting || !isUpdatesEnabled() || !windowList().count() ) {
		return;
  }
    
	if ((windowList().count() == 1) && windowList().at(0)) {
    m_appCaption = windowList().at(0)->caption();
		windowList().at(0)->showMaximized();
	}
 	else {
    QWorkspace::cascade();
  }
}

/** This works around a problem/limitation in QWorkspace. QWorkspace sets every time the  application caption on its on way. This confuses BibleTime - wrong captions are generated. This function returns the right caption (using the MDI child). */
const QString CMDIArea::currentApplicationCaption() const {
	return m_appCaption;
}

/** Emits the signal to create a new display window in the MDI area. */
void CMDIArea::emitCreateDisplayWindow( ListCSwordModuleInfo modules, const QString keyName ) {
  emit createReadDisplayWindow(modules, keyName);
}
