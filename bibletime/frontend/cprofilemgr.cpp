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

CProfileMgr::CProfileMgr() : m_startupProfile(0) {
	m_profiles.setAutoDelete(true);
	
	KStandardDirs stdDirs;
	m_profilePath = stdDirs.saveLocation("data", "bibletime/profiles/");	
	
	QDir d( m_profilePath );
	QStringList files = d.entryList("*.xml");
	for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
		if ((*it) != "_startup_.xml")
			m_profiles.append(new CProfile(m_profilePath + *it));
		else
			m_startupProfile = new CProfile(m_profilePath + *it);
	}
}

CProfileMgr::~CProfileMgr(){
	m_profiles.clear();//autoDelete is enabled, so all profiles are deleted
}

/** Returns a list of available profiles. */
const QList<CProfile>& CProfileMgr::profiles(){	
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
	bool ret = false;
	QFile f( p->filename() );
	if (f.exists())
		f.remove();		
	if(m_profiles.remove(p)); //auto delete is enabled
		ret = true;
	return ret;
}

const bool CProfileMgr::remove( const QString& profile) {
	bool ret = false;
	for (CProfile* p = m_profiles.first(); p; p = m_profiles.next()) {
		if (p->name() == profile) {
			remove(p);
			ret =  true;
		}
	}
	return ret;
}

/** Returns the profile with the desired name. If there's no such profile 0 is returned. */
CProfile* CProfileMgr::profile(const QString& name) {
	for (m_profiles.first(); m_profiles.current(); m_profiles.next()) {
		qWarning("%s == %s ??", m_profiles.current()->name().latin1(), name.latin1());		
		if (m_profiles.current()->name() == name) {
			return m_profiles.current();	
		}
	}
	qWarning("return 0");
	return 0;
}

/** Returns the startup profile if it exists, otherwise return 0. */
CProfile* CProfileMgr::startupProfile(){
	if (!m_startupProfile)
		m_startupProfile = new CProfile(QString::null, "_startup_");
	return m_startupProfile;	
}

/** Refreshes the profiles available on disk. Use this function to update the list of profiles after another instance of CProfileMgr created a new profile. */
void CProfileMgr::refresh(){
	m_profiles.clear(); //delete all profiles
	QDir d( m_profilePath );
	QStringList files = d.entryList("*.xml");
	for ( QStringList::Iterator it = files.begin(); it != files.end(); ++it ) {
		CProfile p(m_profilePath + *it);
		if (p.name() == "_startup_") { //new startup profile
			if  (!m_startupProfile) //don't put this in the if clause above,it doesn't work!
				m_startupProfile = new CProfile(m_profilePath + *it);
		}
		else if (!profile(p.name())) { //don't have it already
			m_profiles.append(new CProfile(m_profilePath + *it));
		}
	}
}
