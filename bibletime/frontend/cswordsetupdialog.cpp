/***************************************************************************
                          coptionsdialog.cpp  -  description
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

#include "cswordsetupdialog.h"

#include "backend/cswordbackend.h"
#include "backend/cswordmoduleinfo.h"

//#include <stdio.h>
//#include <stdlib.h>

//QT includes
//#include <qhbox.h>
//#include <qlayout.h>
//#include <qvbox.h>
//#include <qdict.h>
//#include <qcheckbox.h>
//#include <qbuttongroup.h>
//#include <qhbuttongroup.h>
//#include <qradiobutton.h>
//#include <qcolor.h>
//#include <qtooltip.h>
//#include <qwhatsthis.h>
//#include <qstringlist.h>
//#include <qinputdialog.h>
//#include <qdir.h>


//KDE includes
//#include <kapp.h>
#include <klocale.h>
//#include <kglobal.h>
//#include <kstandarddirs.h>
//#include <kkeydialog.h>
//#include <kiconloader.h>
//#include <ktabctl.h>
//#include <kapp.h>
//#include <klistbox.h>
//#include <kkeydialog.h>
//#include <kaction.h>
//#include <kconfigbase.h>
//#include <kconfig.h>

//Sword includes
//#include <localemgr.h>

//using std::string;
//using std::list;

CSwordSetupDialog::CSwordSetupDialog(QWidget *parent, const char *name, KAccel* accel )
	: KDialogBase(IconList, i18n("Sword configuration"), Ok | Cancel | Apply, Ok, parent, name, true, true, QString::null, QString::null, QString::null) {

	setIconListAllVisible(true);

//	initStartup();
//	initFonts();
//	initColors();
//	initProfiles();
//	initSword();
//	initAccelerators();
}


/** Called if the OK button was clicked */
void CSwordSetupDialog::slotOk(){
//	saveAccelerators();
//	saveColors();
//	saveFonts();
//	saveSword();
//	saveStartup();
//	saveProfiles();

	KDialogBase::slotOk();

  emit signalSwordSetupChanged( );
}

/*called if the apply button was clicked*/
void CSwordSetupDialog::slotApply(){
//	saveAccelerators();
//	saveColors();
//	saveFonts();
//	saveSword();
//	saveStartup();
//	saveProfiles();
	
	KDialogBase::slotApply();

  emit signalSwordSetupChanged( );
}

/** Opens the page which contaisn the given part ID. */
const bool CSwordSetupDialog::showPart( CSwordSetupDialog::Parts ID ){
//	bool ret = false;
//	switch (ID) {
//		case CSwordSetupDialog::ViewProfiles:
//			if(showPage(pageIndex(
//					m_settings.profiles.profiles->parentWidget()) ))
//				ret = true;
//			break;
//		default:
//			break;
//	}
//	return ret;
}

