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
	: m_filename(file), m_name(name) {
	
	m_profileWindows.setAutoDelete(true);
	if (!m_filename.isEmpty() && name.isEmpty()) {
		load();
	}
	else if (m_filename.isEmpty() && !name.isEmpty()) {
		m_filename = name;
		m_filename.replace(QRegExp("\\s=#."),"_");
		KStandardDirs stdDirs;
		const QString profilePath = stdDirs.saveLocation("data", "bibletime/profiles/");
		m_filename = profilePath + m_filename + ".xml";
		init(m_filename);
	}
	else {
		qWarning("CProfile: empty file name!");
	}
}

CProfile::~CProfile(){
	m_profileWindows.clear(); 	//delete all CProfileWindows objects (autodelete is enabled)
}

/** Loads the profile from the file given in the constructor. */
QList<CProfileWindow> CProfile::load(){
	qWarning("CProfile::load");
		
	QFile file(m_filename);	
	if (!file.exists())
		return QList<CProfileWindow>();

	file.open(IO_ReadOnly);
	
	QDomDocument doc;
	doc.setContent(&file);			
  QDomElement document = doc.documentElement();
  if(document.tagName() != "BibleTime") {
		qWarning("CProfile::load: Missing BibleTime doc");
		return m_profileWindows;
	}
	if (document.hasAttribute("name")) {
		m_name = document.attribute("name");	
//		qWarning("set name to %s", name().latin1());
	}
	
	m_profileWindows.clear();
	QDomElement elem = document.firstChild().toElement();
	while (!elem.isNull()) {
		CProfileWindow* p = 0;
		qWarning(elem.tagName().latin1());
		
		if (elem.tagName() == "BIBLE") {
			p = new CProfileWindow(CSwordModuleInfo::Bible);
		}
		else if (elem.tagName() == "COMMENTARY") {
			p = new CProfileWindow(CSwordModuleInfo::Commentary);
		}
		else if (elem.tagName() == "LEXICON") {
			p = new CProfileWindow(CSwordModuleInfo::Lexicon);
		}			
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
		elem = elem.nextSibling().toElement();	
	}

	file.close();	
	qWarning("load finished");
	return m_profileWindows;
}

/** Saves the profile to the file given in the constructor. */
const bool CProfile::save(QList<CProfileWindow> windows){
	/** Save the settings using a XML file
	*	Save the CProfileWindow objects using a XML file which name is in m_filename
	*/
	qWarning("CProfile::save(QList)");
  QDomDocument doc("DOC");
  doc.appendChild( doc.createProcessingInstruction( "xml", "version=\"1.0\" encoding=\"UTF-8\"" ) );

  qWarning("middle");

  QDomElement content = doc.createElement("BibleTime");
  content.setAttribute("syntaxVersion", CURRENT_SYNTAX_VERSION);
  content.setAttribute("name", name());
  doc.appendChild(content);

  qWarning("before the loop");
	for (CProfileWindow* p = windows.first(); p; p = windows.next()) {
		ASSERT(p);
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
	if (m_name.isEmpty())
		m_name = i18n("unknown");
	return m_name;
}

/** Initializes the XML for the first time (use to create a new profile) */
void CProfile::init(const QString file){
	const QString oldFile = m_filename;
	m_filename = file;
	save(QList<CProfileWindow>());
	m_filename = oldFile;
}

/** Chnages the name of this profile. */
void CProfile::setName( const QString& name ){
	m_name = name;
}
