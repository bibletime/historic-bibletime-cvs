/***************************************************************************
                          cprofilemgr.h  -  description
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

#ifndef CPROFILEMGR_H
#define CPROFILEMGR_H

#include "cprofile.h"

//Qt includes
#include <qstring.h>

/** The manager for profiles.
	* Provides functions to create, delete, save and load profiles.
  * @author The BibleTime team
  */

class CProfileMgr {
public: 
	CProfileMgr();
	~CProfileMgr();	

  /** Creates a new profile with the name "name" (first parameter).
	* @return The profile object
	*/
  CProfile* create( const QString name );
  /**
  * @return a list of available profiles
  */
  const QList<CProfile> profiles();
  /**
  * Removes the profile from the list and from the directory containg the profile files.
  */
  const bool remove( CProfile* p );
  /**
  * Removes the profile from the list and from the directory containg the profile files.
  */
  const bool remove( const QString& );
  /**
  * Returns the profile with the desired name. If there's no such profile 0 is returned.
  */
  CProfile* profile(const QString&);
  /**
  * Returns the startup profile if it exists, otherwise return 0.
  */
  CProfile* startupProfile();

protected:	
	QList<CProfile> m_profiles;	
	QString m_profilePath;
	CProfile* m_startupProfile;
};

#endif
