/***************************************************************************
                          csearchdialogscope.h  -  description
                             -------------------
    begin                : Sun Nov 12 2000
    copyright            : (C) 2000 by The BibleTime team
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

#ifndef CSEARCHDIALOGSCOPE_H
#define CSEARCHDIALOGSCOPE_H

#include <qwidget.h>
#include <qdialog.h>

#include <listkey.h>
#include "../../backend/sword_backend/cswordmodulesearch.h"

class QComboBox;
class QLineEdit;
class QPushButton;
class QListBox;
class QLabel;
class QRadioButton;
class QButtonGroup;
class KSimpleConfig;

/**
	* This class (CSearchDialogScope) provides the graphical user interface
	* for the scope part of the searchdialog.
	* It provides an easy to use interface with the following functions:
	*
	*	-Possibility to define mltiple scopes for each module
	*	-Possibilty to define wheter we want to use no scope,
	*	 the result of the last search as scope or an own set of ranges
	*
  * @author The BibleTime team
  */



class CSearchDialogScopeChooser : public QWidget {
   Q_OBJECT
public:
	CSearchDialogScopeChooser(QWidget *parent, const char *name=0);
	~CSearchDialogScopeChooser();
  ListKey getScope();
  CSwordModuleSearch::scopeType getScopeType();

private:
  QComboBox*  RangeChooser;
  QButtonGroup* group;
  KSimpleConfig* config;
  QRadioButton *noScope;
  QRadioButton *lastResultScope;
  QRadioButton *useScope;
  QPushButton *editButton;
  QLabel *RangeLabel;


private slots:
  void editButtonClicked();
  /** No descriptions */
  void scopeChanged();
};

class CSearchDialogScopeEdit : public QDialog {
   Q_OBJECT
public:
	CSearchDialogScopeEdit(KSimpleConfig *config, QWidget *parent, const char *name=0);
	~CSearchDialogScopeEdit();

public slots:
  void NewButtonClicked(void);
  void DeleteButtonClicked(void);
  void RangeChooserActivated(int);
  void RangeEditTextChanged(const QString &);

private:
  QComboBox*  RangeChooser;
  QLineEdit*  RangeEdit;
  QListBox*   RangeDisp;
  KSimpleConfig* config;
  QPushButton* newButton;
  QPushButton* deleteButton;
  QPushButton* closeButton;
private slots: // Private slots
  /**
  * No descriptions
  */
  void closeButtonClicked();
};


#endif
