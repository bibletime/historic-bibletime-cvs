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
//#include "chtmlwidget.h"
#include "chtmldialog.h"
#include "ctoolclass.h"

//QT includes
#include <qlayout.h>
#include <qhbox.h>
#include <qfile.h>
#include <qtextbrowser.h>
#include <qtoolbar.h>
#include <qtoolbutton.h>

//KDE includes
#include <klocale.h>
#include <kiconloader.h>
#include <kapp.h>


CHTMLDialog::CHTMLDialog(QWidget* parent, const char *name )
	: QDialog ( parent, name, true, WStyle_MinMax | WStyle_DialogBorder | WStyle_Dialog )
{
	init(false);
}

CHTMLDialog::CHTMLDialog(const QString& url, QWidget* parent, const char *name)
	: QDialog ( parent, name, true, WStyle_MinMax | WStyle_DialogBorder | WStyle_Dialog )
{
	init();
	QString file = CToolClass::locatehtml(url);
	
	m_textBrowser->mimeSourceFactory()->setFilePath(file);	
	m_textBrowser->setSource(file);
}

CHTMLDialog::~CHTMLDialog(){
}

/** Sets the content of the widget */
void CHTMLDialog::setText(const QString& text){
	m_textBrowser->setText(text);
}

/** Initializes this widget. */
void CHTMLDialog::init(const bool enableHistory){
	qDebug("CHTMLDialog::init(const bool enableHistory)");
	resize(600,400);
		
	QVBoxLayout* layout = new QVBoxLayout(this, 5);
		
	m_textBrowser = new QTextBrowser(this);		
	if (enableHistory) {
		QHBox* toolBar = new QHBox(this, "history toolbar");
		
		QToolButton *button = 0;		
		button = new QToolButton(toolBar);
		button->setIconSet(SmallIcon("back"));
//		button->setTextLabel(i18n("Backward"));		
		button->setEnabled(false);		
//		button->setUsesTextLabel(true);
		button->setFixedSize(button->sizeHint());
		connect(button, SIGNAL(clicked()), m_textBrowser, SLOT(backward()));
		connect(m_textBrowser, SIGNAL(backwardAvailable(bool)),button, SLOT(setEnabled(bool)));		

		
		QWidget* spacer = new QWidget(toolBar);
		toolBar->setStretchFactor(spacer, 5);
				
		button = new QToolButton(toolBar);		
		button->setEnabled(false);
//		button->setUsesTextLabel(true);		
		button->setIconSet(SmallIcon("forward"));
//		button->setTextLabel(i18n("Forward"));		
		button->setFixedSize(button->sizeHint());
		connect(button, SIGNAL(clicked()), m_textBrowser, SLOT(forward()));		
		connect(m_textBrowser, SIGNAL(forwardAvailable(bool)),button, SLOT(setEnabled(bool)));
		
	
		layout->addWidget(toolBar);		
	}		
	layout->addSpacing(5);	
	layout->addWidget(m_textBrowser,5);
}
