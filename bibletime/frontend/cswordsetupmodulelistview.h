/***************************************************************************
                          cswordsetupmodulelistview.h  -  description
                             -------------------
    begin                : Wed Jan 19 2000
    copyright            : (C) 2000 by The BibleTime Team
    email                : Info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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
