/***************************************************************************
                          cprofilewindow.cpp  -  description
                             -------------------
    begin                : Sun Jul 29 2001
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

#include "cprofilewindow.h"

CProfileWindow::CProfileWindow(CSwordModuleInfo::type type)
	: m_type(type), m_windowGeometry(),m_moduleList(), m_key(QString::null) {

}

CProfileWindow::~CProfileWindow(){

}

/** Returns the size of the window including the x,y coordinates. */
const QRect CProfileWindow::geometry() {
	return m_windowGeometry;
}

/** Sets the size of the window. */
void CProfileWindow::setGeometry( const QRect rect ){
	m_windowGeometry = rect;
}

/** Returns the type of the managed window (bible window, commentary window or lexicon window). */
const CSwordModuleInfo::type CProfileWindow::type(){
	if (m_type != CSwordModuleInfo::Unknown)
		return m_type;
}

/** Sets the modules. */
void CProfileWindow::setModules( QStringList modules ){
	m_moduleList = modules; //copy module pointers into our own list
}

/** Returns a list of module names which are chosen in the managed window profile. */
const QStringList CProfileWindow::modules(){
//	QStringList modules;
//	for (CSwordModuleInfo* m = m_moduleList.first(); m; m = m_moduleList.next()) {
//		modules.append(m->getName());
//	}
//	return modules;
	return m_moduleList;
}

/** Returns the current key set in the modules. */
const QString& CProfileWindow::key(){
	return m_key;
}

/** Set the key used in the modules. */
void CProfileWindow::setKey( const QString& key ){
	m_key = key;
}

/** Sets the current position of the scrollbars. */
void CProfileWindow::setScrollbarPositions(const int horizontal, const int vertical) {
//	m_scrollbarPos = {x,y};
	m_scrollbarPos.horizontal = horizontal;
	m_scrollbarPos.vertical = vertical;
}
/** Returns tghe position of the scrollbars */
const CProfileWindow::ScrollbarPos CProfileWindow::scrollbarPositions(){
	return m_scrollbarPos;
}

/** Sets the type of the used modules. */
void CProfileWindow::setType(CSwordModuleInfo::type type){
	m_type = type;
}
