/***************************************************************************
                          install.h  -  description
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
#include <qmap.h>
#include <qstring.h>

class QWidget;
class BTSetupWizard;
class QLabel;
class QComboBox;

#ifndef INSTALL_H
#define INSTALL_H

/** The install/update part of the application.
  * @author The BibleTime team
  */
class InstallPage_1 : public QObject {
  Q_OBJECT
public:
	InstallPage_1(BTSetupWizard * main);
//	~InstallPage_1();

  /** No descriptions */
  void addPage();


private: // Private methods
	BTSetupWizard* m_main;

	QWidget* m_widget;
	QLabel* m_sourceLabel;
	QLabel* m_targetLabel;
	QComboBox* m_sourceCombo;
	QComboBox* m_targetCombo;

	QMap<QString, QString> m_sourceMap;
	QMap<QString, QString> m_targetMap;

public slots: // Private slots
  /** No descriptions */
  void activate();


};

#endif

