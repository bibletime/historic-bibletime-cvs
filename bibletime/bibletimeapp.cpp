/*********
*
* This file is part of BibleTime's source code, http://www.bibletime.info/.
*
* Copyright 1999-2006 by the BibleTime developers.
* The BibleTime source code is licensed under the GNU General Public License version 2.0.
*
**********/



#include "bibletimeapp.h"

#include "frontend/cbtconfig.h"
#include "frontend/cmanageindiceswidget.h"

#include "util/cresmgr.h"

//KDE includes
#include <dcopclient.h>

BibleTimeApp::BibleTimeApp() {
	initDCOP();

	CResMgr::init_i18n();
}

BibleTimeApp::~BibleTimeApp() {

	//clean up orphaned or defect indices
	BookshelfManager::CManageIndicesWidget::deleteOrphanedIndices();

	//we can set this safely now because we close now (hopyfully without crash)
	CBTConfig::set
		(CBTConfig::crashedLastTime, false);
	CBTConfig::set
		(CBTConfig::crashedTwoTimes, false);

	deletePrinter();
	deleteDisplayTemplateMgr();
	deleteLanguageMgr();
	deleteBackend();
}

void BibleTimeApp::initDCOP() {
	const bool dcopOk = dcopClient()->attach();
	Q_ASSERT(dcopOk);

	if (dcopOk) {
		const QCString appId = dcopClient()->registerAs(kapp->name(), false);
		//   dcopClient()->setDefaultObject("BibleTimeInterface");
	}
}
