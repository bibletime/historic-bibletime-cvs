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

#define CURRENT_SYNTAX_VERSION 1

CProfile::CProfile( const QString& file, const QString& name )
	: m_filename(file), m_name(name) {
	
	m_profileWindows.setAutoDelete(true);
}

CProfile::~CProfile(){
	m_profileWindows.clear(); 	//delete all CProfileWindows objects (autodelete is enabled)
}

/** Loads the profile from the file given in the constructor. */
QList<CProfileWindow> CProfile::load(){
	QFile file(m_filename);
	ASSERT(file.exists());
	file.open(IO_ReadOnly);
	
	QDomDocument doc;
	doc.setContent(&file);			
  QDomElement document = doc.documentElement();
  if(document.tagName() != "BibleTime") {
		qWarning("CProfile::load: Missing DOC");
		return m_profileWindows;
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
		qWarning(elem.tagName().latin1());
		ASSERT(p);
		if (p) {
			m_profileWindows.append(p);
			
			
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
				
		elem = doc.nextSibling().toElement();	
	}

	file.close();	
	qWarning("count before return %i", m_profileWindows.count());
	return m_profileWindows;
}

/** Saves the profile to the file given in the constructor. */
const bool CProfile::save(QList<CProfileWindow> windows){
	/** Save the settings using a XML file
	*	Save the CProfileWindow objects using a XML file which name is in m_filename
	*/

  QDomDocument doc("DOC");
  doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );
  QDomElement content = doc.createElement("BibleTime");
  content.setAttribute("syntaxVersion", CURRENT_SYNTAX_VERSION);
  doc.appendChild(content);

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
		}
		if (window.isNull()) {
			qWarning("invalid module type in CProfile::save!");
			break;
		}
		
		//save geomtery
		QRect r = p->geometry();
		QDomElement geometry = doc.createElement("GEOMETRY");
		geometry.setAttribute("x",r.x());
		geometry.setAttribute("y",r.y());		
		geometry.setAttribute("width",r.width());		
		geometry.setAttribute("height",r.height());		
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
	
	QFile file(filename());
	file.open(IO_WriteOnly);
	QTextStream t( &file );        // use a text stream
	t << doc.toString();
	file.close();
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
