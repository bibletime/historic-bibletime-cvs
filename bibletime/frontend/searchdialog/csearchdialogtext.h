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

//BibleTime includes
#include "../../structdef.h"

//Qt includes
#include <qwidget.h>

class QLineEdit;
class QRadioButton;
class QCheckBox;
class KProgress;
class CSearchDialogScopeChooser;

class CSearchDialogText : public QWidget
{
    Q_OBJECT

public:
  /**
  * The constructor of the search dialog search tab page.
  */
	CSearchDialogText(CImportantClasses *IC, QWidget *parent = 0, const char *name = 0);
  /**
  * The destructor
  */	
	~CSearchDialogText();	
  /**
  * Returns the scopechooser
  */
  CSearchDialogScopeChooser *scopeChooser;
  /**
  * Returns the text the user want to search for
  */
	QString getText();
  /**
  * Sets the search text
  */
	void setText(QString);
  /**
  * Returns true if case sensitive search is enabled.
  */
	bool isCaseSensitive();
  /**
  * Returns the current type of search.
  */
	int getSearchType();
  /**
  * Updates the progress bar for the modulw which is searched at the moment
  */
	void updateCurrentProgress(unsigned short int);
  /**
  * Updates the progres bar which gives the progress for the whole search
  */
	void updateOverallProgress(unsigned short int);
  /**
  * Resets the widgets.
  */
  virtual void reset();

protected:
	QLineEdit 		*editSearchText;
	QRadioButton	*radioMultipleWords;
	QRadioButton	*radioExactSearch;
	QRadioButton	*radioRegularExpression;
	QCheckBox			*checkCaseSensitive;
  /**
  *
  */	
	KProgress			*currentProgressBar;
  /**
  *
  */
	KProgress			*overallProgressBar;
};


#endif