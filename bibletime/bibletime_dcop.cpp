/********* Read the file LICENSE for license details. *********/

#include "bibletime.h"

//frontend includes
#include "frontend/cmdiarea.h"
#include "frontend/cbtconfig.h"
#include "frontend/searchdialog/csearchdialog.h"

//helper function
void BibleTime::syncAllModulesByType(const CSwordModuleInfo::ModuleType type, const QString& key) {
	qDebug("Syncing modules by type to key %s", key.latin1());

	QPtrList<QWidget> windows = m_mdi->usableWindowList();
	for (QWidget* w = windows.first(); w; w = windows.next()) {
		CDisplayWindow* d = dynamic_cast<CDisplayWindow*>(w);
		Q_ASSERT(d);

		if (d && d->modules().count() && d->modules().first()->type() == type) {
			d->lookup(key);
		}
	}
}

void BibleTime::closeAllModuleWindows() {
	qDebug("DCOP: close all windows now...");

	m_mdi->deleteAll();
}

void BibleTime::syncAllBibles(QString key) {
	qDebug("DCOP: syncing all bibles ...");
	syncAllModulesByType(CSwordModuleInfo::Bible, key);
}

void BibleTime::syncAllCommentaries(QString key) {
	qDebug("DCOP: syncing all commentaries ...");
	syncAllModulesByType(CSwordModuleInfo::Commentary, key);
}

void BibleTime::syncAllLexicons(QString key) {
	qDebug("DCOP: syncing all lexicons ...");
	syncAllModulesByType(CSwordModuleInfo::Lexicon, key);
}

void BibleTime::syncAllVerseBasedModules(QString key) {
	qDebug("DCOP: syncing all verse based modules ...");
	syncAllModulesByType(CSwordModuleInfo::Bible, key);
	syncAllModulesByType(CSwordModuleInfo::Commentary, key);
}

void BibleTime::openWindow(QString moduleName, QString key) {
	qDebug("DCOP: open window for module %s and key %s...", moduleName.latin1(), key.latin1());

	CSwordModuleInfo* module = CPointers::backend()->findModuleByName(moduleName);
	Q_ASSERT(module);
	if (module) {
		createReadDisplayWindow(module, key);
	}
}

void BibleTime::openDefaultBible(QString key) {
	qDebug("DCOP: open default bible ...");
	CSwordModuleInfo* mod = CBTConfig::get
								(CBTConfig::standardBible);
	if (mod) {
		openWindow(mod->name(), key);
	}
}

QStringList BibleTime::searchInOpenModules(QString searchText) {
	qDebug("DCOP: search in open modules ...");
	slotSearchModules(); //opens the search dialog with the currently open modules
	CSearchDialog* dlg = CSearchDialog::getSearchDialog();
	dlg->setSearchText(searchText);
	dlg->startSearch();
}

QStringList BibleTime::searchInDefaultBible(QString searchText) {
	qDebug("DCOP: search in default bible ...");
	CSwordModuleInfo* mod = CBTConfig::get(CBTConfig::standardBible);
	mod->search(searchText, CSwordModuleSearch::multipleWords, sword::ListKey());

	sword::ListKey result = mod->searchResult();
	QStringList ret;

	for ( int i = 0; i < result.Count(); ++i ) {
		sword::SWKey* key = result.getElement(i);
		Q_ASSERT(key);

		ret << QString::fromUtf8( key->getText() );
	}

	return ret;
}
