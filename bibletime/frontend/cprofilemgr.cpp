/***************************************************************************
                          cprofilemgr.cpp  -  description
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

#include "cprofilemgr.h"

//Qt includes
#include <qdir.h>
#include <qfile.h>
#include <qlist.h>

//KDE includes
#include <kstddirs.h>

CProfileMgr::CProfileMgr(){
	m_profiles.setAutoDelete(true);
	
	KStandardDirs stdDirs;
	m_profilePath = stdDirs.saveLocation("data", "bibletime/profiles/");	
	
	//load available profiles
	qWarning(m_profilePath.latin1());	
	QDir d( m_profilePath );
	QStringList files = d.entryList("*.xml");
	for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
		CProfile* p = new CProfile(m_profilePath + *it);
		m_profiles.append(p);
	}
}

CProfileMgr::~CProfileMgr(){
	m_profiles.clear();
}

/** Returns a list of available profiles. */
const QList<CProfile> CProfileMgr::profiles(){	
	return m_profiles;
}

/** Creates a new profile with the name "name" (first parameter).  @return The profile object */
CProfile* CProfileMgr::create( const QString name ){
	CProfile* p = new CProfile(QString::null, name);
	m_profiles.append(p);
	
	return p;	
}

/** Removes the profile from the list and from the directory containg the profile files. */
const bool CProfileMgr::remove( CProfile* p ){
	QFile f( p->filename() );
	if (f.exists())
		f.remove();		
	m_profiles.remove(p); //auto delete is enabled
}

const bool CProfileMgr::remove( const QString& profile) {
	for (CProfile* p = m_profiles.first(); p; p = m_profiles.next()) {
		if (p->name() == profile) {
			remove(p);
			break;
		}
	}
}

/** Returns the profile with the desired name. If there's no such profile 0 is returned. */
CProfile* CProfileMgr::profile(const QString& name) {
	for (CProfile* p = m_profiles.first(); p ; p = m_profiles.next()) {
		if (p->name() == name)
			return p;	
	}
	return 0;
}
