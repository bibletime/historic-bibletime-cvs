/***************************************************************************
                        csearchdialog.h  -  description
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

/* $HEADER	*/
/* $REVISION	*/

#ifndef CSEARCHDIALOG_H
#define CSEARCHDIALOG_H

//BibleTime includes
#include "../../backend/cswordmoduleinfo.h"
#include "../../backend/cswordmodulesearch.h"
#include "../cpointers.h"

//QT includes

//KDE includes
#include <kdialogbase.h>
#include <kaccel.h>


class QPushButton;
class QHBox;
class QVBox;
class QGrid;
class	CSearchDialogModuleChooser;
class	CSearchDialogText;
class	CSearchDialogScopeEdit;
class	CSearchDialogResult;
class CSearchDialogAnalysis;
class CSearchDialogAnalysisView;

class CSwordModuleInfo;
class CSwordModuleSearch;

/**
	* The searchdialog of BibleTime
  * @author The BibleTime Team
  */
class CSearchDialog : public KDialogBase, public CPointers  {
   Q_OBJECT
public:
	/**
	* The constructor of the CSearchDialog class.
	* @param parent The widget this dialog is centered on
	* @param name The name of this Qt widget
	*/
	CSearchDialog(ListCSwordModuleInfo& modules,QWidget *parent=0, const char *name=0 );
	~CSearchDialog();
	/**
	*
	*/
	ListCSwordModuleInfo& getModuleList();
	/**
	* Sets the module list for this dialog.
	* The module lsit contains the modules where we should search in
	*/
	void setModuleList(ListCSwordModuleInfo&);
	/**
	* Sets the text we should search in the modules
	*/
	void setSearchText(const QString);
	/**
	* Reimplementation from Qt.
	* This is used to refresh the progres bars
	* and to switch to the resultpage if the search was finished.
	*/
//  virtual void timerEvent(QTimerEvent *e);
  /**
 	* Returns the search text. If no text was enetered return QSTring::null.
 	*/
  const QString getSearchedText() const;
	void startSearch();
	
protected: // Protected methods
  /**
 	* Initializs the view
 	*/
  void initView();
  /**
 	* Reads the settings from the configfile
 	*/
  void readSettings();
	/**
	* Saves the settings of this widget to the config file
	*/
  void saveSettings();

private:
	friend class CSearchDialogResult;
	QVBox* m_moduleChooserPage;	
	QVBox* m_searchTextPage;
	QHBox* m_searchResultPage;
	QVBox* m_searchAnalysisPage;
	CSearchDialogModuleChooser *m_moduleChooser;	
	CSearchDialogText	*m_searchText;
	CSearchDialogResult	*m_searchResult;
	CSearchDialogAnalysis* m_searchAnalysis;
	CSearchDialogAnalysisView* 	m_searchAnalysisView;
	CSwordModuleSearch m_searcher;
	ListCSwordModuleInfo m_moduleList;
//	int old_overallProgress;
//	int old_currentProgress;

	QPushButton* m_searchAnalysisSaveButton;

private slots: // Private slots
  void chosenModulesChanged();
	virtual void slotUser1();
	virtual void slotUser2();
	void slotSaveSearchAnalysis();
  void searchFinished();
  void percentUpdate();

public slots: // Public slots
  /**
  * Reimplementation to show the "First time opened" dialog.
  */
  virtual void show();
};

#endif
