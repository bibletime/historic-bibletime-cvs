/***************************************************************************
                          chtmldialog.cpp  -  description
                             -------------------
    begin                : Tue Jan 25 2000
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

//own includes
#include "chtmlwidget.h"
#include "chtmldialog.h"
#include "ctoolclass.h"

//QT includes
#include <qwidget.h>
#include <qlayout.h>
#include <qhbox.h>
#include <qfile.h>

//KDE includes
#include <klocale.h>
#include <kapp.h>

CHTMLDialog::CHTMLDialog(QWidget* parent, const char *name )
	: KDialogBase ( parent, name, true, i18n("BibleTime - Help window"), Ok, Ok, true)
{
	html_widget = new CHTMLWidget(0,false, this);
	setMainWidget(html_widget);
	resize(600,480);
}

CHTMLDialog::CHTMLDialog(const QString url, QWidget* parent, const char *name)
	: KDialogBase ( parent, name, true, i18n("BibleTime - Help window"), Ok, Ok, true)
{
	html_widget = new CHTMLWidget(0,false, this);		
	html_widget->setHTMLSource( url );	
	setMainWidget(html_widget);
	resize(500,400);
}

CHTMLDialog::~CHTMLDialog(){
}

/** Sets the content of the widget */
void CHTMLDialog::setText(const QString& text){
	html_widget->setText(text);
}