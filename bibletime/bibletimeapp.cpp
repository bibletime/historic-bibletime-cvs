/***************************************************************************
                          bibletimeapp.cpp  -  description
                             -------------------
    begin                : Sam Jul 12 2003
    copyright            : (C) 2003 by The BibleTime team
    email                : info@bibletime.info
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "bibletimeapp.h"

#include "frontend/cbtconfig.h"

#include "util/cresmgr.h"

BibleTimeApp::BibleTimeApp(){
  CResMgr::init_i18n();
}

BibleTimeApp::~BibleTimeApp(){
  	//we can set this safely now because we close now (hopyfully without crash)
  	CBTConfig::set(CBTConfig::crashedLastTime, false);
  	CBTConfig::set(CBTConfig::crashedTwoTimes, false);
	
	deletePrinter();
	deleteDisplayTemplateMgr();
	deleteLanguageMgr();
	deleteBackend();
}
