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

//QT includes
//#include <qwidget.h>
//#include <qframe.h>
//#include <qlabel.h>
#include <qstring.h>
#include <qmap.h>
//#include <qcombobox.h>
//#include <qdict.h>
//#include <qmap.h>

//KDE includes
#include <kdialogbase.h>
//#include <kfontdialog.h>
//#include <kaccel.h>
//#include <kcolorbutton.h>
//#include <kapp.h>

class QWidget;
class QLabel;
class QComboBox;
class QWidgetStack;
//class QHBox;
//class QCheckBox;
//class QRadioButton;
//class KListBox;
//class KKeyChooser;
//class KTabCtl;

/**
	* The Sword configuration dialog of BibleTime
  * @author The BibleTime Team
  */
class CSwordSetupDialog : public KDialogBase, public CPointers  {
   Q_OBJECT

public:
	CSwordSetupDialog(QWidget *parent=0, const char *name=0, KAccel* key_accel=0);

  enum Parts {
		Sword,
		Install,		
		Remove
  };

  /** Opens the page which contaisn the given part ID. */
  const bool showPart( CSwordSetupDialog::Parts ID );

private:
  void initInstall();
  /** No descriptions */
  void loadSourceLocations();
  /** No descriptions */
  void saveSourceLocations();
  /** No descriptions */
  void determineTargetLocations();
  /** No descriptions */
  void populateInstallCombos();

	QWidgetStack* m_installWidgetStack;

	QString source;
  QString target;

	QLabel* m_sourceLabel;
	QLabel* m_targetLabel;
	QComboBox* m_sourceCombo;
	QComboBox* m_targetCombo;

	QMap<QString, QString> m_sourceMap;
	QMap<QString, QString> m_targetMap;


private slots:
  /** No descriptions */
  void slot_sourceSelected(const QString &sourceName);
  /** No descriptions */
  void slot_targetSelected(const QString &targetName);

protected slots: // Protected slots
	void slotOk();
	void slotApply();

protected: // Protected methods

signals: // Signals
  void signalSwordSetupChanged();
};

#endif //CSWORDSETUPDIALOG_H
