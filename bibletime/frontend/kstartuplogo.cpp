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
#include <qlayout.h>
#include <qapp.h>

//KDE includes
#include <kapp.h>
#include <kstddirs.h>
#include <kimageio.h>

KStartupLogo::KStartupLogo()
	: QWidget(0, "startuplogo", WStyle_Customize | WStyle_NoBorder) {

  QPixmap pm;	
	if ( !pm.load(locate("BT_pic","startuplogo.jpg")) )
  	qWarning("Can't load startuplogo! Check your installation.");

  setBackgroundPixmap(pm);

  textLabel = new QLabel(this);
  textLabel->setGeometry(5,pm.height(),pm.width()-5,textLabel->sizeHint().height()+5);
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
  textLabel->setText( text );
  KApplication::kApplication()->processEvents();
}

