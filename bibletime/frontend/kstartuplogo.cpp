/***************************************************************************
                          kstartuplogo.cpp  -  description
                             -------------------
    begin                : Sat Sep 11 1999
    copyright            : (C) 1999 by Torsten Uhlmann
    email                : TUhlmann@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//Own includes
#include "kstartuplogo.h"

//Qt includes
#include <qapplication.h>
#include <qlabel.h>
#include <qlayout.h>
#include <qpixmap.h>

//KDE includes
#include <kapplication.h>
#include <kstandarddirs.h>
//#include <kimageio.h>

//static objects
static KStartupLogo* startupLogo = 0;

void KStartupLogo::createSplash() {
	deleteSplash();
	startupLogo = new KStartupLogo();
}

void KStartupLogo::showSplash() {
	if (startupLogo)
		startupLogo->show();
}

void KStartupLogo::hideSplash() {
	if (startupLogo)	
		startupLogo->hide();
}

void KStartupLogo::deleteSplash() {
	delete startupLogo;
	startupLogo = 0;
}

	
void KStartupLogo::setStatusMessage(const QString& message) {
	if (startupLogo)
		startupLogo->setText(message);
}



KStartupLogo::KStartupLogo()
	: QWidget(0, "startuplogo", WStyle_Customize | WStyle_NoBorder) {

  QPixmap pm;	
	if ( !pm.load(locate("BT_pic","startuplogo.png")) )
  	qWarning("Can't load startuplogo! Check your installation.");

  setBackgroundPixmap(pm);

  //Please not change the margin
  textLabel = new QLabel(this);
  textLabel->setGeometry(0,pm.height(),pm.width(),textLabel->sizeHint().height());
  textLabel->setBackgroundColor( Qt::black );

	QPalette p = palette();
  p.setColor( QPalette::Inactive, QColorGroup::Text, Qt::white );
  p.setColor( QPalette::Inactive, QColorGroup::Foreground, Qt::white );
  p.setColor( QPalette::Normal, QColorGroup::Text, Qt::white );
  p.setColor( QPalette::Normal, QColorGroup::Foreground, Qt::white );
  setPalette( p );

  setGeometry (
    (KApplication::desktop()->width()-pm.width())/2,
		(KApplication::desktop()->height()-pm.height()-textLabel->height())/2,
    pm.width(),
    pm.height()+textLabel->height());
}

void KStartupLogo::setText(const QString text){
	//Please not make the text bold & let the first character be blank
  textLabel->setText( QString::fromLatin1(" %1").arg(text) );
  KApplication::kApplication()->processEvents();
}

