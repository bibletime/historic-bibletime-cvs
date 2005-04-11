/********* Read the file LICENSE for license details. *********/

#ifndef CSWORDMODULELISTVIEW_H
#define CSWORDMODULELISTVIEW_H

//QT includes
#include <qstring.h>
#include <qmap.h>
#include <qlistview.h>

//KDE includes

class CSwordModuleInfo;

namespace InstallationManager {

class CSwordSetupModuleListView : public QListView {
	Q_OBJECT

public:
	CSwordSetupModuleListView(QWidget* parent, bool is_remote);
	void addModule(CSwordModuleInfo *, QString localVersion);
	void finish();
	QStringList selectedModules();
	void clear();
	
private:
	void init();

private:
	bool m_is_remote;
	QListViewItem* m_categoryBible;
	QListViewItem* m_categoryCommentary;
	QListViewItem* m_categoryLexicon;
	QListViewItem* m_categoryBook;
	QListViewItem* m_categoryDevotionals;
	QListViewItem* m_categoryGlossaries;

};

} //NAMESPACE

#endif //CSWORDMODULELISTVIEW_H
