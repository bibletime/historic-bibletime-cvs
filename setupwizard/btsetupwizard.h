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

class KJanusWidget;

class QLabel;
class QCheckBox;
class QListView;

class CSwordBackend;

/** The main window of the application. Provides the possibilities to install/remove and download modules.
  * @author The BibleTime team
  */
class BTSetupWizard : public KMainWindow  {
  Q_OBJECT
public: 
	BTSetupWizard(QWidget *parent=0, const char *name=0);
	~BTSetupWizard();

  QLabel* explanationLabel(QWidget* parent, const QString& heading, const QString& text );


private: // Private methods
  /** No descriptions */
  void addMainPage(void);
  /** No descriptions */
  void addRemovePage();
  /** No descriptions */
  void populateRemoveModuleListView();

	KJanusWidget* m_mainWidget;

	QCheckBox* m_startBibleTimeBox;
	QListView* m_removeModuleListView;
	CSwordBackend* m_backend;


private slots: // Private slots
  /** No descriptions */
  void slot_exitRequested();
};

#endif
