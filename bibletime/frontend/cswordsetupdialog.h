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
#include <qlistview.h>
#include <qurl.h>

//KDE includes
#include <kdialogbase.h>
#include <klistview.h>


//Sword includes
class QWidget;
class QLabel;
class QComboBox;
class QWidgetStack;
class QButton;
class QPushButton;
class QLineEdit;

class KProgressDialog;

namespace InstallationManager {

class CInstallSourcesMgrDialog : public KDialogBase, public CPointers  {
   Q_OBJECT

public:
	CInstallSourcesMgrDialog(QWidget *parent=0, const char *name=0);

protected:

	class InstallSourceItem : public KListViewItem {
	public:
		InstallSourceItem( KListView* parent );
		InstallSourceItem( KListView* parent, sword::InstallSource );

		const QUrl& url() const;
		void setURL(const QUrl& url);

		void setServer(const QString& server);
		const QString server() const;

		void setPath(const QString& server);
		const QString path() const;

		const QString& caption() const;
		void setCaption( const QString& caption );

//		const bool isEnabled() const;
//		void setEnabled( const bool enabled );

		sword::InstallSource swordInstallSource();

	private:
		void updateItem();

		QUrl m_url;
		QString m_caption;
	};

	void initRemoteSourcesPage();
	void initLocalSourcesPage();
	void updateRemoteURLLabel();

protected slots:
	void slot_localAddSource();
	void slot_localRemoveSource();

	void slot_remoteAddSource();
	void slot_remoteChangeSource();
	void slot_remoteRemoveSource();
	void slot_remoteSourceSelectionChanged();
	void slot_remoteCaptionChanged(const QString&);
	void slot_remoteServerChanged(const QString&);
	void slot_remotePathChanged(const QString&);

	void slotOk();

private:
	QFrame* m_localSourcesPage;
	KListView* m_localSourcesList;

	QFrame* m_remoteSourcesPage;
	KListView* m_remoteSourcesList;
	QLineEdit* m_remoteCaptionEdit;
	QLineEdit* m_remoteServerEdit;
	QLineEdit* m_remotePathEdit;
	QLabel* m_remoteURLLabel;
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

	const QString currentInstallSource();

  QFrame* m_swordConfigPage;
  KListView* m_swordPathListBox;
  QPushButton* m_swordEditPathButton;
  QPushButton* m_swordAddPathButton;
  QPushButton* m_swordRemovePathButton;
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
  QPushButton* m_installBackButton;
  QPushButton* m_installContinueButton;

  QWidget* m_installModuleListPage;
  QWidget* m_installSourcePage;

 	KListView* m_installModuleListView;
  KProgressDialog* m_progressDialog;
  QString m_installingModule;
  bool m_refreshedRemoteSources;
	unsigned int m_installedModuleCount;

	BTInstallMgr* m_currentInstallMgr; //pointer to the current installmgr object so we can access it in the cancel install slot

private slots:
  void slot_sourceSelected(const QString &sourceName);
  void slot_targetSelected(const QString &targetName);

	void slot_doRemoveModules();
	void slot_removeModuleItemExecuted(QListViewItem*);

	void slotOk();
	void slotApply();
  void slot_connectToSource();

	void slot_installManageSources();
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
