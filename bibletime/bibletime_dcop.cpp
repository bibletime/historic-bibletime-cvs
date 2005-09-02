/********* Read the file LICENSE for license details. *********/

#include "bibletime.h"

//frontend includes
#include "frontend/cmdiarea.h"


ASYNC BibleTime::closeAllModuleWindows() {
	qWarning("DCOP: close all windows now...");
	m_mdi->deleteAll();
}

ASYNC BibleTime::syncAllBibles(QString key) {
	qWarning("DCOP: syncing all bibles ...");
}

ASYNC BibleTime::syncAllCommentaries(QString key) {
	qWarning("DCOP: syncing all commentaries ...");
}

ASYNC BibleTime::syncAllLexicons(QString key) {
	qWarning("DCOP: syncing all lexicons ...");
}

ASYNC BibleTime::syncAllVerseBasedModules(QString key) {
	qWarning("DCOP: syncing all verse based modules ...");
}

ASYNC BibleTime::openWindow(QString moduleName, QString key) {
	qWarning("DCOP: open window for module %s and key %s...", moduleName.latin1(), key.latin1());
	CSwordModuleInfo* module = CPointers::backend()->findModuleByName(moduleName);
	Q_ASSERT(module);
	if (module) {
		createReadDisplayWindow(module, key);
	}
}

ASYNC BibleTime::openDefaultBible(QString key) {
	qWarning("DCOP: open default bible ...");
}

QStringList BibleTime::searchInOpenModules(QString searchText) {
	qWarning("DCOP: search in open modules ...");
}

QStringList BibleTime::searchIndefaultBible(QString searchText) {
	qWarning("DCOP: search in default bible ...");
}
