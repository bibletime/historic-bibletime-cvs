/***************************************************************************
                          btsetupwizard.h  -  description
                             -------------------
    begin                : Son Jul 14 2002
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

#ifndef BTSETUPWIZARD_H
#define BTSETUPWIZARD_H

#include <qwidget.h>
#include <kmainwindow.h>

class QLabel;
class QCheckBox;
class QListView;
class QWidgetStack;
class QButton;

class CSwordBackend;

/** The main window of the application. Provides the possibilities to install/remove and download modules.
  * @author The BibleTime team
  */
class BTSetupWizard : public KMainWindow  {
  Q_OBJECT
public: 
	BTSetupWizard(QWidget *parent=0, const char *name=0);
	~BTSetupWizard();

private: // Private methods
  void addMainPage(void);
  /** No descriptions */
  void addRemovePage();
  /** No descriptions */
  void populateRemoveModuleListView();
  /** No descriptions */
  void addInstall_1Page();

	QWidgetStack* m_widgetStack;

	QCheckBox* m_startBibleTimeBox;

	QLabel* m_installSourceLabel;

	QListView* m_removeModuleListView;
	QLabel* m_populateListNotification;
	QButton* m_removeBackButton;
	QButton* m_removeRemoveButton;

	CSwordBackend* m_backend;

	QWidget* m_mainPage;
	QWidget* m_install_1Page;
	QWidget* m_removePage;

private slots: // Private slots
  void slot_exitRequested();
  /** No descriptions */
  void slot_backtoMainPage();
  /** No descriptions */
  void slot_gotoInstall_1Page();
  /** No descriptions */
  void slot_gotoRemovePage();
  /** No descriptions */
  void slot_doRemoveModules();

};

#endif
