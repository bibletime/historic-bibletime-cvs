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

//BibleTime includes
#include "backend/cswordmoduleinfo.h"

//Qt includes
#include <qwidget.h>

//KDE includes
#include <khtml_part.h>

class CSearchDialog;
class CSearchDialogResultModuleView;
class CSearchDialogResultView;
class CSearchResultItem;

class QSplitter;

class CSearchDialogResult : public QWidget
{
  Q_OBJECT
	friend class CSearchDialog;
public:
	/**
	* Standard constructor
	*/
	CSearchDialogResult(QWidget *parent = 0, const char *name = 0);
	/**
	* Sets the module list and clears the result list.
	*/
	void setModuleList(ListCSwordModuleInfo& moduleList);
	/**
	* Removes the listed module items and the current search result from the lists
	*/
	void clearResult();

public slots:
	/**
	* Updates the preview using the parameter as new key.
	*/
	void updatePreview(const QString);

private:
	ListCSwordModuleInfo moduleList;
	CSearchDialogResultModuleView *resultModuleTree;
	CSearchDialogResultView	*resultTree;
	KHTMLPart* m_displayWidget;
	QSplitter* m_splitter;
};


#endif
