/***************************************************************************
                          kstartuplogo.cpp  -  description
                             -------------------
    begin                : Sat Sep 11 1999
    copyright            : (C) 1999 by Torsten Uhlmann
    email                : TUhlmann@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "kstartuplogo.h"
#include <kapp.h>
#include <kstddirs.h>
#include <kimageio.h>

KStartupLogo::KStartupLogo()
	: QWidget(0,"startuplogo",WStyle_Customize | WStyle_NoBorder | WStyle_Tool) {
	
  QPixmap pm;	
  bool success = false;

	success = pm.load(locate("BT_pic","startuplogo.jpg"));		
  if ( !success )
  	qWarning("Can't load startuplogo! Check your installation.");
  	  	
  setBackgroundPixmap(pm);
  setFixedSize(pm.width(), pm.height());
  setGeometry( (KApplication::desktop()->width()-pm.width())/2,(KApplication::desktop()->height()-pm.height())/2, pm.width(), pm.height());
}
