/***************************************************************************
                          coptionsdialog.h  -  description
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

#ifndef CSWORDSETUPDIALOG_H
#define CSWORDSETUPDIALOG_H

#include "cbtconfig.h"
#include "util/cpointers.h"
#include "btinstallmgr.h"

//QT includes
#include <qstring.h>
#include <qmap.h>

//KDE includes
#include <kdialogbase.h>

class QWidget;
class QLabel;
class QComboBox;
class QWidgetStack;
class QButton;
class QPushButton;
class QLineEdit;
class QListView;

class KProgressDialog;

namespace InstallationManager {

class CSwordSetupModuleListView;

/**
	* The Sword configuration dialog of BibleTime
  * @author The BibleTime Team
  */
class CSwordSetupDialog : public KDialogBase, public CPointers  {
   Q_OBJECT

public:
	CSwordSetupDialog(QWidget *parent=0, const char *name=0);

  enum Parts {
		Sword,
		Install,
		Remove
  };

  /** Opens the page which contaisn the given part ID. */
  const bool showPart( CSwordSetupDialog::Parts ID );

private:
  void initSwordConfig();
  void initInstall();
	void initRemove();
	
	void writeSwordConfig();

	void setupSwordPathListBox();
	void populateInstallCombos();

	const QString currentInstallSource();

  QFrame* m_swordConfigPage;
  QListView* m_swordPathListBox;
  QPushButton* m_swordEditPathButton;
  QPushButton* m_swordAddPathButton;
  QPushButton* m_swordRemovePathButton;
  QStringList m_swordPathList;
	bool m_swordSetupChanged;

  QFrame* m_removePage;

  QFrame* m_installPage;
	QWidgetStack* m_installWidgetStack;

	QString source;
  QString target;

	QLabel* m_sourceLabel;
	QLabel* m_targetLabel;
	QComboBox* m_sourceCombo;
	QComboBox* m_targetCombo;

	QMap<QString, QString> m_targetMap;

	CSwordSetupModuleListView* m_removeModuleListView;
	QPushButton* m_removeRemoveButton;

	const bool refreshRemoteModuleCache( const QString& sourceName );
  void populateRemoveModuleListView();
  void populateInstallModuleListView(const QString& sourceName);

//install module stuff
  QPushButton* m_installBackButton;
  QPushButton* m_installContinueButton;

  QWidget* m_installModuleListPage;
  QWidget* m_installSourcePage;

 	CSwordSetupModuleListView* m_installModuleListView;
  KProgressDialog* m_progressDialog;
  QString m_installingModule;
  bool m_refreshedRemoteSources;
	unsigned int m_installedModuleCount;

	BTInstallMgr* m_currentInstallMgr; //pointer to the current installmgr object so we can access it in the cancel install slot

private slots:
  void slot_sourceSelected(const QString &sourceName);
  void slot_targetSelected(const QString &targetName);

	void slot_doRemoveModules();

	void slotOk();
		
  void slot_connectToSource();
	void slot_moduleRefreshProgressCancelClicked();
	void slot_moduleRefreshCompleted(const int, const int);
	
	void slot_installAddSource();
	void slot_installDeleteSource();
  void slot_installModules();
	void slot_installModuleItemExecuted(QListViewItem*);
	void slot_installProgressCancelClicked();
  void slot_showInstallSourcePage();

  void installCompleted( const int, const int );
  void slot_swordRemoveClicked();
  void slot_swordAddClicked();
  void slot_swordEditClicked();
  void slot_swordPathSelected();

signals: // Signals
  void signalSwordSetupChanged();
};

}

#endif //CSWORDSETUPDIALOG_H
