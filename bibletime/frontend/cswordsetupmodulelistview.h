/********* Read the file LICENSE for license details. *********/

#ifndef CSWORDMODULELISTVIEW_H
#define CSWORDMODULELISTVIEW_H

//Sword includes
#include <installmgr.h>

//QT includes
#include <qstring.h>
#include <qmap.h>

//KDE includes
#include <klistview.h>

class CSwordModuleInfo;
class CSwordBackend;

namespace InstallationManager {

	class CSwordSetupModuleListView : public KListView {
		Q_OBJECT

	public:
		CSwordSetupModuleListView(QWidget* parent, bool is_remote, sword::InstallSource* installSource = 0);
		virtual ~CSwordSetupModuleListView();

		void addModule(CSwordModuleInfo *, QString localVersion);
		void finish();
		QStringList selectedModules();
		void clear();

		virtual QString tooltip(QListViewItem* i, int column) const;
		virtual bool showTooltip(QListViewItem* i, const QPoint& pos, int column) const;

	protected slots:
		void slotItemClicked(QListViewItem*);

	private:
		void init();

		CSwordBackend* m_backend;
		bool m_is_remote;

		QListViewItem* m_categoryBible;
		QListViewItem* m_categoryCommentary;
		QListViewItem* m_categoryLexicon;
		QListViewItem* m_categoryBook;
		QListViewItem* m_categoryDevotionals;
		QListViewItem* m_categoryGlossaries;

	signals:
		void selectedModulesChanged();
	};

} //NAMESPACE

#endif //CSWORDMODULELISTVIEW_H
