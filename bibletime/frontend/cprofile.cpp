/***************************************************************************
                          cprofile.cpp  -  description
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

#include "cprofile.h"

//Qt includes
#include <qdom.h>
#include <qfile.h>
#include <qstring.h>
#include <qtextstream.h>

//KDE includes
#include <kstddirs.h>
#include <klocale.h>

#define CURRENT_SYNTAX_VERSION 1

CProfile::CProfile( const QString& file, const QString& name )
	: m_filename(file),
		m_name(name.isEmpty() ? i18n("unknown") : name),
		m_fullscreen(false),
		m_geometry(0,0,800,600) {
	
	m_profileWindows.setAutoDelete(true);
	if (!m_filename.isEmpty() && name.isEmpty()) {
		loadBasics();
	}
	else if (m_filename.isEmpty() && !name.isEmpty()) {
		m_filename = name;
		m_filename.replace(QRegExp("\\s=#."),"_");
		KStandardDirs stdDirs;
		m_filename = stdDirs.saveLocation("data", "bibletime/profiles/") + m_filename + ".xml";
		init(m_filename);
	}
	else
		qWarning("CProfile: empty file name!");
}

CProfile::~CProfile(){
	m_profileWindows.clear(); 	//delete all CProfileWindows objects (autodelete is enabled)
}

/** Loads the profile from the file given in the constructor. */
QList<CProfileWindow> CProfile::load(){
	QFile file(m_filename);	
	if (!file.exists())
		return QList<CProfileWindow>();

	file.open(IO_ReadOnly);
	
	QDomDocument doc;
	doc.setContent(&file);
	file.close();	
	
  QDomElement document = doc.documentElement();
  if(document.tagName() != "BibleTime") {
		qWarning("CProfile::load: Missing BibleTime doc");
		return m_profileWindows;
	}
	if (document.hasAttribute("name")) {
		m_name = document.attribute("name");	
	}

	
	//load settings of the main window
	{
		// see if there's a section with the name MAINWINDOW
		QDomElement elem = document.firstChild().toElement();		
		QDomElement mainWindow;
		while (!elem.isNull()) {
			if (elem.tagName() == "MAINWINDOW") {
				mainWindow = elem;
				break; //found the element
			}
			elem = elem.nextSibling().toElement();				
		}		
		if (!mainWindow.isNull()) { //was found
			setFullscreen( (bool)mainWindow.attribute("fullscreen").toInt());
			QDomElement object = mainWindow.namedItem("GEOMETRY").toElement();
			QRect rect;
      if(!object.isNull()) {
				if (object.hasAttribute("x")) {
					rect.setX(object.attribute("x").toInt());
				}
				if (object.hasAttribute("y")) {
					rect.setY(object.attribute("y").toInt());
				}
				if (object.hasAttribute("width")) {
					rect.setWidth(object.attribute("width").toInt());
				}
				if (object.hasAttribute("height")) {
					rect.setHeight(object.attribute("height").toInt());
				}
//				if (object.hasAttribute("isMaximized")) {
//					p->setMaximized( static_cast<bool>(object.attribute("isMaximized").toInt()) );
//				}				
			}
			setGeometry(rect);			
		}
	}
		
	m_profileWindows.clear();
	QDomElement elem = document.firstChild().toElement();
	while (!elem.isNull()) {
		CProfileWindow* p = 0;		
		if (elem.tagName() == "BIBLE") {
			p = new CProfileWindow(CSwordModuleInfo::Bible);
		}
		else if (elem.tagName() == "COMMENTARY") {
			p = new CProfileWindow(CSwordModuleInfo::Commentary);
		}
		else if (elem.tagName() == "LEXICON") {
			p = new CProfileWindow(CSwordModuleInfo::Lexicon);
		}			
		else if (elem.tagName() == "BOOK") {
			p = new CProfileWindow(CSwordModuleInfo::GenericBook);
		}			
		
		if (p) {
			m_profileWindows.append(p);
			
			if (elem.hasAttribute("windowSettings")) {
				p->setWindowSettings( elem.attribute("windowSettings").toInt() );
			}
			
			QRect rect;
						
			QDomElement object = elem.namedItem("GEOMETRY").toElement();
      if(!object.isNull()) {
				if (object.hasAttribute("x")) {
					rect.setX(object.attribute("x").toInt());
				}
				if (object.hasAttribute("y")) {
					rect.setY(object.attribute("y").toInt());
				}
				if (object.hasAttribute("width")) {
					rect.setWidth(object.attribute("width").toInt());
				}
				if (object.hasAttribute("height")) {
					rect.setHeight(object.attribute("height").toInt());
				}
				if (object.hasAttribute("isMaximized")) {
					p->setMaximized( static_cast<bool>(object.attribute("isMaximized").toInt()) );
				}				
			}
			p->setGeometry(rect);
			
			object = elem.namedItem("MODULES").toElement();
      if(!object.isNull()) {				
				if (object.hasAttribute("list")) {
					const QString sep = object.hasAttribute("separator") ? object.attribute("separator") : "|";
					QStringList modules = QStringList::split(sep, object.attribute("list"));
					p->setModules(modules);
				}
      }

			object = elem.namedItem("KEY").toElement();
      if(!object.isNull()) {
      	if (object.hasAttribute("name"))
					p->setKey(object.attribute("name"));
      }				
				
			object = elem.namedItem("SCROLLBARS").toElement();
      if(!object.isNull()) {
      	int horizontal = 0, vertical = 0;
      	if (object.hasAttribute("horizontal"))
      		horizontal = object.attribute("horizontal").toInt();
      	if (object.hasAttribute("vertical"))
      		vertical = object.attribute("vertical").toInt();

				p->setScrollbarPositions(horizontal, vertical);      	
      }				
		}				
		elem = elem.nextSibling().toElement();	
	}
	return m_profileWindows;
}

/** Saves the profile to the file given in the constructor. */
const bool CProfile::save(QList<CProfileWindow> windows){
	/** Save the settings using a XML file
	*	Save the CProfileWindow objects using a XML file which name is in m_filename
	*/
	bool ret = false;
  QDomDocument doc("DOC");
  doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );

  QDomElement content = doc.createElement("BibleTime");
  content.setAttribute("syntaxVersion", CURRENT_SYNTAX_VERSION);
  content.setAttribute("name", m_name);
  doc.appendChild(content);

  //save mainwindow settings
	{
	 	QDomElement mainWindow = doc.createElement("MAINWINDOW");
  	mainWindow.setAttribute("fullscreen", fullscreen());
  	
  	QDomElement geometry = doc.createElement("GEOMETRY");
  	mainWindow.appendChild(geometry);
  	QRect r = this->geometry();
  	geometry.setAttribute("x",r.x());
  	geometry.setAttribute("y",r.y());		
  	geometry.setAttribute("width",r.width());		
  	geometry.setAttribute("height",r.height());		
//  	geometry.setAttribute("isMaximized",static_cast<int>(maximized()));

		content.appendChild(mainWindow);
	}

	for (CProfileWindow* p = windows.first(); p; p = windows.next()) {
		QDomElement window;
		switch (p->type()) {
			case CSwordModuleInfo::Bible:
				window = doc.createElement("BIBLE");	
				break;
			case CSwordModuleInfo::Commentary:
				window = doc.createElement("COMMENTARY");	
				break;
			case CSwordModuleInfo::Lexicon:
				window = doc.createElement("LEXICON");	
				break;
			case CSwordModuleInfo::GenericBook:
				window = doc.createElement("BOOK");	
				break;				
		}
		if (window.isNull())
			break;
		window.setAttribute("windowSettings", p->windowSettings());
		
		//save geomtery
		QRect r = p->geometry();
		QDomElement geometry = doc.createElement("GEOMETRY");
		geometry.setAttribute("x",r.x());
		geometry.setAttribute("y",r.y());		
		geometry.setAttribute("width",r.width());		
		geometry.setAttribute("height",r.height());		
		geometry.setAttribute("isMaximized",static_cast<int>(p->maximized()));
		window.appendChild( geometry );

		QDomElement modules = doc.createElement("MODULES");
		modules.setAttribute("separator", "|");
		modules.setAttribute("list", p->modules().join("|"));
		window.appendChild( modules );		

		QDomElement key = doc.createElement("KEY");
		key.setAttribute("name", p->key());
		window.appendChild( key );		

		QDomElement scrollbars = doc.createElement("SCROLLBARS");
		scrollbars.setAttribute("horizontal", p->scrollbarPositions().horizontal);
		scrollbars.setAttribute("vertical", p->scrollbarPositions().vertical);		
		window.appendChild( scrollbars );		
						
		content.appendChild( window );
	}		
	
	QFile file(m_filename);
	if(file.open(IO_WriteOnly))
		ret = true;
	QTextStream t( &file );        // use a text stream
	t << doc.toString();
	file.close();

	return ret;
}

/** Saves the profile to the file given in the constructor. */
const bool CProfile::save(){
	return save(m_profileWindows);
}

/** Returns the filename used for this profile. */
const QString& CProfile::filename(){
	return m_filename;
}

/** Returns the name of this profile. */
const QString& CProfile::name(){
	return m_name;
}

/** Initializes the XML for the first time (use to create a new profile) */
void CProfile::init(const QString file){
	const QString oldFile = m_filename;
	m_filename = file;
	save(QList<CProfileWindow>());
	m_filename = oldFile;
}

/** Changes the name of this profile. */
void CProfile::setName( const QString& newName ){
	m_name = newName;
	saveBasics(); //save chanegd name
}

/** Loads the basic settings requires for proper operation. */
void CProfile::loadBasics(){
	QFile file(m_filename);	
	if (!file.exists())
		return;

		
	file.open(IO_ReadOnly);
	
	QDomDocument doc;
	doc.setContent(&file);
  QDomElement document = doc.documentElement();
	if (document.hasAttribute("name"))
		m_name = document.attribute("name");	

	file.close();	
}

void CProfile::saveBasics(){
	QFile file(m_filename);	
	if (!file.exists())
		return;

	file.open(IO_ReadOnly);	
	QDomDocument doc;
	doc.setContent(&file);	
	file.close();	
	
  QDomElement document = doc.documentElement();
	document.setAttribute("name", m_name);	
	
	file.open(IO_WriteOnly);	
	QTextStream t( &file );
	t << doc.toString();	
	file.close();	
}

/** Returns true if the main window was in fullscreen mode as the profile was saved. */
const bool CProfile::fullscreen() const {
	return m_fullscreen;
}

/** Set the parameter to true if the main window coveres the full screen size. */
void CProfile::setFullscreen( const bool fullscreen ){
	m_fullscreen = fullscreen;
}

/** Returns the geometry of the main window */
const QRect CProfile::geometry(){
	return m_geometry;
}

/** Stes the geoemtry of the main window */
void CProfile::setGeometry( const QRect rect ){
	m_geometry = rect;
}
