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
#include "chtmldialog.h"
#include "ctoolclass.h"

//QT includes
#include <qlayout.h>
#include <qvbox.h>
#include <qfile.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>
#include <qpushbutton.h>

//KDE includes
#include <kcombobox.h>
#include <klocale.h>
#include <kiconloader.h>
#include <kapp.h>
#include <khtmlview.h>


CHTMLDialog::CHTMLDialog(QWidget* parent, const char *name )
  : KDialogBase(parent, name, true, i18n("Help"), Close, Close)
{
	init(false);
}

CHTMLDialog::CHTMLDialog(const QString& url, QWidget* parent, const char *name)
  : KDialogBase(parent, name, true, i18n("Help"), Close, Close)
{
  init();
	QString file = CToolClass::locatehtml(url);
  KURL new_url = QString::fromLatin1("file:/") + file;
	m_htmlPart->openURL(new_url);
}

CHTMLDialog::~CHTMLDialog(){
}

/** Sets the content of the widget */
void CHTMLDialog::setText(const QString& text){
  m_htmlPart->begin();
  m_htmlPart->write(text);
 	m_htmlPart->end();
}

/** Initializes this widget. */
void CHTMLDialog::init(const bool enableHistory){
  QVBox* page = makeVBoxMainWidget();
	m_htmlPart = new KHTMLPart(page);
//  m_htmlPart->view()->setHScrollBarMode(QScrollView::AlwaysOff);
  connect(m_htmlPart->browserExtension(), SIGNAL(openURLRequest(const KURL&, const KParts::URLArgs&)),
    SLOT(openURL( const KURL&, const KParts::URLArgs&)));

	resize(600,400);
}

/** No descriptions */
void CHTMLDialog::openURL( const KURL& url, const KParts::URLArgs& args){
  m_htmlPart->closeURL();
  m_htmlPart->openURL(url);
}
