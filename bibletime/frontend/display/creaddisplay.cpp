/***************************************************************************
                          creaddisplay.cpp  -  description
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
#include "creaddisplay.h"

#include "backend/cswordmoduleinfo.h"
#include "backend/cswordbiblemoduleinfo.h"
#include "backend/cswordlexiconmoduleinfo.h"
#include "backend/cswordbookmoduleinfo.h"
#include "backend/cswordkey.h"
#include "backend/cswordversekey.h"
#include "backend/cswordtreekey.h"
#include "backend/cswordldkey.h"

#include "frontend/displaywindow/cdisplaywindow.h"
#include "frontend/displaywindow/creadwindow.h"

#include "frontend/cexportmanager.h"

#include "util/scoped_resource.h"

//Qt includes
#include <qpopupmenu.h>

//KDE includes
#include <klocale.h>

CReadDisplay::CReadDisplay(CReadWindow* readWindow) : CDisplay(readWindow), m_activeAnchor(QString::null) {
//	qWarning("constructor of CReadDisplay");
}

CReadDisplay::~CReadDisplay(){
}

/** Returns the current active anchor. */
const QString& CReadDisplay::activeAnchor(){
  return m_activeAnchor;
}

/** Sets the current anchor to the parameter. */
void CReadDisplay::setActiveAnchor( const QString& anchor ){
  m_activeAnchor = anchor;
}


/** Returns true if the display has an active anchor. */
const bool CReadDisplay::hasActiveAnchor(){
  return !activeAnchor().isEmpty();
}


void CReadDisplay::print(const CDisplay::TextPart type){
  CDisplayWindow* window = parentWindow();
  CSwordKey* const key = window->key();
  CSwordModuleInfo* module = key->module();
  CExportManager mgr(i18n("Print keys"),false);
        
  switch (type) {
		case Document: {
			if (module->type() == CSwordModuleInfo::Bible) {
   			CSwordVerseKey* vk = dynamic_cast<CSwordVerseKey*>(key);

      	CSwordVerseKey startKey(*vk);
				startKey.Verse(1);	
				
    		CSwordVerseKey stopKey(*vk);
				if (CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(module))
					stopKey.Verse( bible->verseCount( bible->bookNumber(startKey.book()), startKey.Chapter() ) );
				mgr.printKey(module, startKey.key(), stopKey.key());
  		}
    	else if (module->type() == CSwordModuleInfo::Lexicon || module->type() == CSwordModuleInfo::Commentary ) {
    		mgr.printKey(module, key->key(), key->key());
      }
    };

    case AnchorWithText: {
      if (hasActiveAnchor()) {
				mgr.printByHyperlink( activeAnchor() );
      };
    };

    default:
    	break;
	}
}

