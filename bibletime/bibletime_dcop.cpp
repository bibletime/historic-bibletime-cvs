/********* Read the file LICENSE for license details. *********/

//BibleTime includes
#include "bibletime.h"

//frontend includes
#include "frontend/cmdiarea.h"
#include "frontend/cbtconfig.h"
#include "frontend/searchdialog/csearchdialog.h"

//Sword includes
#include <versekey.h>

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

QStringList BibleTime::searchInModule(const QString& moduleName, const QString& searchText) {
	qDebug("DCOP: searchInModule %s ...", moduleName.latin1());
	QStringList ret;
	CSwordModuleInfo* mod = CPointers::backend()->findModuleByName(moduleName);

	Q_ASSERT(mod);
	if (mod) {
		mod->search(searchText, CSwordModuleSearch::multipleWords, sword::ListKey());

		sword::ListKey result = mod->searchResult();
		const QString lead = QString("[%1] ").arg(moduleName);
		;
		for ( int i = 0; i < result.Count(); ++i ) {
			sword::SWKey* key = result.getElement(i);
			Q_ASSERT(key);


			if (mod->type() == CSwordModuleInfo::Bible || mod->type() == CSwordModuleInfo::Commentary) {
				sword::VerseKey vk(key->getText());
				ret << lead + QString::fromUtf8( vk.getOSISRef() );
			}
			else {
				ret << lead + QString::fromUtf8( key->getText() );
			}
		}
	}

	return ret;

}

QStringList BibleTime::searchInOpenModules(const QString& searchText) {
	qDebug("DCOP: search in open modules ...");
	QStringList ret;

	QWidgetList windows = m_mdi->windowList();
	for ( int i = 0; i < static_cast<int>(windows.count()); ++i ) {
		CDisplayWindow* w = dynamic_cast<CDisplayWindow*>(windows.at(i));
		if (w) {
			ListCSwordModuleInfo windowModules = w->modules();

			ListCSwordModuleInfo::iterator end_it = windowModules.end();
			for (ListCSwordModuleInfo::iterator it(windowModules.begin()); it != end_it; ++it) {
				ret += searchInModule((*it)->name(), searchText);
			};
		};
	};

	return ret;
}

QStringList BibleTime::searchInDefaultBible(const QString& searchText) {
	qDebug("DCOP: search in default bible ...");

	CSwordModuleInfo* bible = CBTConfig::get
								  (CBTConfig::standardBible);
	return searchInModule(bible->name(), searchText);
}
