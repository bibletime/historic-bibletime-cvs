/***************************************************************************
                          cwritedisplay.cpp  -  description
                             -------------------
    begin                : Don Mai 9 2002
    copyright            : (C) 2002 by The BibleTime team
    email                : info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//BibleTime includes
#include "cwritedisplay.h"

#include "frontend/displaywindow/cwritewindow.h"

//Qt includes
#include <qwidget.h>

CWriteDisplay::CWriteDisplay( CWriteWindow* writeWindow ) : CDisplay(writeWindow) {
}

CWriteDisplay::~CWriteDisplay(){
}

/** Saves settings */
//bool CCommentaryPresenter::queryClose(){
//	//save the text
//	if (!m_htmlWidget->isReadOnly() && m_htmlWidget->isModified()) {
//		switch (KMessageBox::warningYesNoCancel( this, i18n("Save changes to module?")) ) {
//			case KMessageBox::Yes:
//			{
//	    	//save
//	   		saveText( m_htmlWidget->text() );
//	   		m_htmlWidget->setModified( false );
//	     	return true;
//			}
//	   	case KMessageBox::No :
//	     	return true;
//	   	default: // cancel
//	     	return false;
//		}
//	}
//	return true;
//}
