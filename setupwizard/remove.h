/***************************************************************************
                          remove.h  -  description
                             -------------------
    begin                : Fri Oct 4 2002
    copyright            : (C) 2002 by The BibleTime team
    email                : info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <qobject.h>

class QWidget;
class BTSetupWizard;
class QLabel;
class QListView;
class QPushButton;
class CSwordBackend;

#ifndef REMOVE_H
#define REMOVE_H

/** The install/update part of the application.
  * @author The BibleTime team
  */
class RemovePage : public QObject {
  Q_OBJECT
public:
	RemovePage(BTSetupWizard * main);
//	~InstallPage_1();

  /** No descriptions */
  void addPage();


private: // Private methods
	BTSetupWizard* m_main;

	QWidget* m_widget;

	QListView* m_removeModuleListView;
	QLabel* m_populateListNotification;
	QPushButton* m_removeBackButton;
	QPushButton* m_removeRemoveButton;

	CSwordBackend* m_backend;

  void populateRemoveModuleListView();

public slots:
  /** No descriptions */
  void activate();

private slots:
  /** No descriptions */
  void slot_doRemoveModules();
};

#endif

