/***************************************************************************
                          ckeychooser.cpp  -  description
                             -------------------
    begin                : Wed Sep 20 2000
    copyright            : (C) 2000 by The BibleTime team
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

#include "ckeychooser.h"
#include "../../backend/cmoduleinfo.h"
#include "../../backend/sword_backend/cswordmoduleinfo.h"
#include "../../backend/sword_backend/cswordbiblemoduleinfo.h"
#include "../../backend/sword_backend/cswordcommentarymoduleinfo.h"
#include "../../backend/sword_backend/cswordlexiconmoduleinfo.h"

#include "clexiconkeychooser.h"
#include "cbiblekeychooser.h"

CKeyChooser::CKeyChooser(CModuleInfo *info, CKey *key, QWidget *parent, const char *name )
	: QWidget(parent, name){
}

CKeyChooser* CKeyChooser::createInstance(CModuleInfo *info, CKey *key, QWidget *parent){	
	if (info && (CSwordModuleInfo*)info)
  	switch ( ((CSwordModuleInfo*)info)->getType() ){
  	  case CSwordModuleInfo::Commentary:	
  	  case CSwordModuleInfo::Bible:	  return new CBibleKeyChooser(info,key,parent);
  	  case CSwordModuleInfo::Lexicon:	return new CLexiconKeyChooser(info,key,parent);	
  	  default: return 0;
  	}
  return 0;
}
