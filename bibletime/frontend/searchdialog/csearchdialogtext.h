/***************************************************************************
                        csearchdialogtext.h  -  description
                             -------------------
    begin                : Mit Jan 19 12:57:15 CET 2000
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

#ifndef CSEARCHDIALOGTEXT_H
#define CSEARCHDIALOGTEXT_H

//Qt includes
#include <qwidget.h>

class CImportantClasses;

class QComboBox;
class QRadioButton;
class QCheckBox;
class KProgress;
class CSearchDialogScopeChooser;

/** First tab page of the searchdialog.
	* This class is the first tab pge of the searchdialog.
	* It provides the interface to define the search text, the search scope and the modules to search in.
	*/
class CSearchDialogText : public QWidget
{
    Q_OBJECT
public:
  /**
  * The constructor of the search dialog search tab page.
  */
	CSearchDialogText(CImportantClasses *IC, QWidget *parent = 0, const char *name = 0);
	~CSearchDialogText();
  /**
  * Returns the scopechooser
  */
  CSearchDialogScopeChooser *scopeChooser;
  /**
  * Returns the text the user want to search for
  */
	const QString getText() const;
  /**
  * Sets the search text
  */
	void setText(const QString);
  /**
  * Returns true if case sensitive search is enabled.
  */
	const bool isCaseSensitive();
  /**
  * Returns the current type of search.
  */
	const int getSearchType();
  /**
  * Returns the string of current type of search.
  */
  const QString getSearchTypeString();
  /**
  * Updates the progress bar for the modulw which is searched at the moment
  */
	void updateCurrentProgress(const unsigned short int);
  /**
  * Updates the progres bar which gives the progress for the whole search
  */
	void updateOverallProgress(const unsigned short int);
  /**
  * Resets the widgets.
  */
  void reset();

private:
  void readSettings();
  void saveSettings();
    	
	CImportantClasses* m_importantClasses;
	QComboBox 		*editSearchText;
	QRadioButton	*radioMultipleWords;
	QRadioButton	*radioExactSearch;
	QRadioButton	*radioRegularExpression;
	QCheckBox			*checkCaseSensitive;
	KProgress			*currentProgressBar;
	KProgress			*overallProgressBar;
};
#endif
