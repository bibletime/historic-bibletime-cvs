/********* Read the file LICENSE for license details. *********/

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
