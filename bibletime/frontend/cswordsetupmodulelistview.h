/********* Read the file LICENSE for license details. *********/

#ifndef CSWORDMODULELISTVIEW_H
#define CSWORDMODULELISTVIEW_H

//QT includes
#include <qstring.h>
#include <qmap.h>

//KDE includes
#include <klistview.h>

class CSwordModuleInfo;

namespace InstallationManager {

class CSwordSetupModuleListView : public KListView {
	Q_OBJECT

public:
	CSwordSetupModuleListView(QWidget* parent, bool is_remote);
	void addModule(CSwordModuleInfo *, QString localVersion);
	void finish();
	QStringList selectedModules();
	void clear();

protected slots:
	void slotItemClicked(QListViewItem*);
	
private:
	void init();
	
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
