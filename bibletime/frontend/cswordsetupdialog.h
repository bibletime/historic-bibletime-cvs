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
#include <kurl.h>
#include <klistview.h>


//Sword includes
class QWidget;
class QLabel;
class QComboBox;
class QWidgetStack;
class QButton;
class KProgressDialog;
// class KListView;

class CInstallSourcesMgrDialog : public KDialogBase, public CPointers  {
   Q_OBJECT

public:
	CInstallSourcesMgrDialog(QWidget *parent=0, const char *name=0);

protected:

	class InstallSourceItem : public KListViewItem {
		public:
			InstallSourceItem( KListView* parent );
			const KURL& url() const;

		private:
			KURL m_url;
	};

	void initView();
	void initRemoteSourcesPage();
	void initLocalSourcesPage();

protected slots:
	void slot_localAddSource();
	void slot_localRemoveSource();

	void slot_remoteAddSource();
	void slot_remoteChangeSource();
	void slot_remoteRemoveSource();
	void slot_remoteSourceSelectionChanged();

private:
	QFrame* m_localSourcesPage;
	KListView* m_localSourcesList;

	QFrame* m_remoteSourcesPage;
	KListView* m_remoteSourcesList;
};


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
  const bool showPart( CSwordSetupDialog::Parts ID, const bool exclusive = false );

private:
  void initSwordConfig();
  void initInstall();
	void initRemove();

  /** Setup the path list box */
  void setupSwordPathListBox();

  void populateInstallCombos();

  QFrame* m_swordConfigPage;
  KListView* m_swordPathListBox;
  QButton* m_swordEditPathButton;
  QButton* m_swordAddPathButton;
  QButton* m_swordRemovePathButton;
  QStringList m_swordPathList;

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

	KListView* m_removeModuleListView;
	QLabel* m_populateListNotification;
	QPushButton* m_removeRemoveButton;

  void populateRemoveModuleListView();
  void populateInstallModuleListView(const QString& sourceName);

//install module stuff
  QButton* m_installBackButton;
  QButton* m_installContinueButton;

  QWidget* m_installModuleListPage;
  QWidget* m_installSourcePage;

 	KListView* m_installModuleListView;
  KProgressDialog* m_progressDialog;
  QString m_installingModule;
  bool m_refreshedRemoteSources;
	unsigned int m_installedModuleCount;

private slots:
  void slot_sourceSelected(const QString &sourceName);
  void slot_targetSelected(const QString &targetName);
  void slot_doRemoveModules();
	void slotOk();
	void slotApply();
  void slot_connectToSource();
  void slot_installManageSources();
  void slot_installModules();
  void slot_showInstallSourcePage();

  void installCompleted( const int, const int );
  void slot_swordRemoveClicked();
  void slot_swordAddClicked();
  void slot_swordEditClicked();
  void slot_swordPathSelected();

signals: // Signals
  void signalSwordSetupChanged();
};

#endif //CSWORDSETUPDIALOG_H
