/***************************************************************************
                      csearchdialogresult.h  -  description
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

#ifndef CSEARCHDIALOGRESULT_H
#define CSEARCHDIALOGRESULT_H

#include <qwidget.h>

#include "../../backend/sword_backend/cswordmoduleinfo.h"
#include "../chtmlwidget.h"

class CImportantClasses;
class CSearchDialog;
class CSearchDialogResultModuleView;
class CSearchDialogResultView;
class CSearchResultItem;

class CSearchDialogResult : public QWidget
{
  Q_OBJECT
	friend class CSearchDialog;
public:
	/**
	* Standard constructor
	*/
	CSearchDialogResult(CImportantClasses* importantClasses, QWidget *parent = 0, const char *name = NULL);
	/**
	* Sets the module list and clears the result list.
	*/
	void setModuleList(ListCSwordModuleInfo* moduleList);
	/**
	* Removes the listed module items and the current search result from the lists
	*/
	void clearResult();

public slots:
	/**
	* Updates the preview using the parameter as new key.
	*/
	void updatePreview(QString);

private:
	ListCSwordModuleInfo* moduleList;
	CImportantClasses* m_important;
	CSearchDialogResultModuleView *resultModuleTree;
	CSearchDialogResultView	*resultTree;
	CHTMLWidget	*html_widget;
};


#endif
