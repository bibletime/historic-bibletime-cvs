/***************************************************************************
                          csearchdialogpages.h  -  description
                             -------------------
    begin                : Die Jul 30 2002
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

#ifndef CSEARCHDIALOGPAGES_H
#define CSEARCHDIALOGPAGES_H

//BibleTime includes
#include "backend/cswordmoduleinfo.h"
#include "backend/cswordbackend.h"
#include "backend/cswordmodulesearch.h"

//Qt includes
#include <qwidget.h>

//KDE includes
#include <klistview.h>

//forward declarations
class QLabel;
class QCheckBox;
class KComboBox;
class KHistoryCombo;
class KProgress;
class QPushButton;
class QRadioButton;
class CReadDisplay;

class CModuleListView : public KListView {
  Q_OBJECT
public:
  CModuleListView(QWidget* parent);
  ~CModuleListView();
  /**
  * Setups the tree using the given list of modules.
  */
  void setupTree( ListCSwordModuleInfo modules );
  /**
  * Returns the currently active module.
  */
  CSwordModuleInfo* const activeModule();

protected: // Protected methods
  /**
  * Initializes this widget.
  */
  void initView();
  /**
  * Initializes the connections of this widget
  */
  void initConnections();

protected slots: // Protected slots
  /**
  * Is executed when an item was selected in the list.
  */
  void executed( QListViewItem* );

signals:
  void moduleSelected(CSwordModuleInfo*);
  void moduleChanged();
};

class CModuleResultView  : public KListView {
  Q_OBJECT
public:
  CModuleResultView(QWidget* parent);
  ~CModuleResultView();

protected: // Protected methods
  /**
  * Initializes the view of this widget.
  */
  void initView();
  void initConnections();

public slots: // Public slots
  /**
  * Setups the list with the given module.
  */
  void setupTree(CSwordModuleInfo*);

protected slots: // Protected slots
  /**
  * Is connected to the signal executed, which is emitted when a mew item was chosen.
  */
  void executed(QListViewItem*);

signals: // Signals
  void keySelected(const QString&);
};


/** The page of the search dialog which contains the search result part.
  * @author The BibleTime team
  */
class CSearchResultPage : public QWidget  {
   Q_OBJECT
public:
	CSearchResultPage(QWidget *parent=0, const char *name=0);
	~CSearchResultPage();
  /**
  * Sets the modules which contain the result of each.
  */
  void setSearchResult(ListCSwordModuleInfo modules);

public slots: // Public slots
  /**
  * Resets the current löist of modules and the displayed list of found entries.
  */
  void reset();

protected: // Protected methods
  /**
  * Initializes the view of this widget.
  */
  void initView();
  /**
  * Initializes the signal slot conections of the child widgets
  */
  void initConnections();

private:
  CModuleListView* m_moduleListBox;
  CModuleResultView* m_resultListBox;
  CReadDisplay* m_previewDisplay;
  ListCSwordModuleInfo m_modules;
  QPushButton* m_analyseButton;
  struct Actions {
//    KAction*
  } m_actions;
  
protected slots: // Protected slots
  /**
  * Update the preview of the selected key.
  */
  void updatePreview(const QString& key);
  /**
  * Shows a dialog with the search analysis of the current search.
  */
  void showAnalysis();
};

class CSearchOptionsPage : public QWidget  {
   Q_OBJECT
public:
	CSearchOptionsPage(QWidget *parent=0, const char *name=0);
	~CSearchOptionsPage();
  /**
  * Sets the search text used in the page.
  */
  void setSearchText(const QString& text);
  /**
  * Returns the search text set in this page.
  */
  const QString searchText();
  /**
  * Returns the list of used modules.
  */
  const ListCSwordModuleInfo modules();
  /**
  * Prepares the stuff which is required for a search, e.g. setting back the percentage bars.
  */
  void prepareSearch();
  /**
  * Return the selected search type,.
  */
  const int searchFlags();
  /**
  * Sets all options back to the default.
  */
  void reset();
  /**
  * Returns the selected search scope if a search scope was selected.
  */
  ListKey searchScope();
  /**
  * Returns the selected scope type.
  */
  const CSwordModuleSearch::scopeType scopeType();

private:
  QLabel* m_modulesLabel;
  KHistoryCombo* m_searchTextCombo;
  QPushButton* m_chooseModulesButton;
  ListCSwordModuleInfo m_modules;

  KComboBox* m_rangeChooserCombo;
  QPushButton* m_chooseRangeButton;
  QRadioButton* m_multipleWordsRadio;
  QRadioButton* m_exactTextRadio;
  QRadioButton* m_regexpRadio;
  QCheckBox* m_caseSensitiveBox;
  KProgress* m_currentProgressBar;
  KProgress* m_overallProgressBar;

protected: // Protected methods
  /**
  * Initializes this page.
  */
  void initView();
  /**
  * Reads the settings of the last searchdialog session.
  */
  void readSettings();
  /**
  * Reads the settings for the searchdialog from disk.
  */
  void saveSettings();

public slots: // Public slots
  /**
  * Sets the modules used by the search.
  */
  void setModules( ListCSwordModuleInfo modules );
  /**
  * Sets the progress for the "all modules" bar.
  */
  void setOverallProgress( const int );
  /**
  * Sets the value for the "curret module" progress bar.
  */
  void setCurrentModuleProgress( const int );
  void aboutToShow();
  /** refreshes the listof ranges and the range combobox. */
  void refreshRanges();
  
protected slots: // Protected slots
  /**
  * Opens the modules chooser dialog.
  */
  void chooseModules();
  void setupRanges();
};

#endif
