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
struct CImportantClasses;


/**
	* The searchdialog of BibleTime
  * @author The BibleTime Team
  */
class CSearchDialog : public KDialogBase  {
   Q_OBJECT
public:
	/**
	* The constructor of the CSearchDialog class.
	* @param importantClasses The pointer to the structure we use to store the printer and backend pointers
	* @param parent The widget this dialog is centered on
	* @param name The name of this Qt widget
	*/
	CSearchDialog( CImportantClasses* importantClasses, ListCSwordModuleInfo* modules,QWidget *parent=0, const char *name=0 );
	~CSearchDialog();
	/**
	*
	*/
	ListCSwordModuleInfo* getModuleList();
	/**
	* Sets the module list for this dialog.
	* The module lsit contains the modules where we should search in
	*/
	void setModuleList(ListCSwordModuleInfo*);
	/**
	* Sets the text we should search in the modules
	*/
	void setSearchText(const QString);
	/**
	* Reimplementation from Qt.
	* This is used to refresh the progres bars
	* and to switch to the resultpage if the search was finished.
	*/
  virtual void timerEvent(QTimerEvent *e);
  /**
 	* Returns the search text. If no text was enetered return QSTring::null.
 	*/
  const QString getSearchedText() const;

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
	void startSearch();

private:
	friend class CSearchDialogResult;
	QVBox* moduleChooser_page;	
	QVBox* searchText_page;
	QHBox* searchResult_page;
	QVBox* searchAnalysis_page;
	CSearchDialogModuleChooser *m_moduleChooser;	
	CSearchDialogText	*searchText;
	CSearchDialogResult	*searchResult;
	CSearchDialogAnalysis* searchAnalysis;
	CSearchDialogAnalysisView* 	searchAnalysisView;
	CSwordModuleSearch *searcher;
	ListCSwordModuleInfo *moduleList;
	CImportantClasses* m_important;			
	int old_overallProgress;
	int old_currentProgress;

	QPushButton* m_searchAnalysisSaveButton;

private slots: // Private slots
  void chosenModulesChanged();
	virtual void slotUser1();
	virtual void slotUser2();
	void slotSaveSearchAnalysis();

public slots: // Public slots
  /** Reimplementation to show the "First time opened" dialog. */
  virtual void show();
};

#endif
