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
#include "../../backend/cswordmoduleinfo.h"
#include "../../backend/cswordbiblemoduleinfo.h"
#include "../../backend/cswordcommentarymoduleinfo.h"
#include "../../backend/cswordlexiconmoduleinfo.h"

#include "clexiconkeychooser.h"
#include "cbiblekeychooser.h"

CKeyChooser::CKeyChooser(CSwordModuleInfo *, CSwordKey *, QWidget *parent, const char *name )
	: QWidget(parent, name){
}

CKeyChooser* CKeyChooser::createInstance(CSwordModuleInfo *module, CSwordKey *key, QWidget *parent){	
	ASSERT(module);
	ASSERT(key);
	if (!module)
		return 0;
 	switch ( module->getType() ){
 	  case CSwordModuleInfo::Commentary:	 //Bibles and commentaries uise the same key chooser
 	  case CSwordModuleInfo::Bible:
 	  	return new CBibleKeyChooser(module,key,parent);
 	  case CSwordModuleInfo::Lexicon:
 	  	return new CLexiconKeyChooser(module,key,parent);	
 	  default:
 	  	return 0;
 	}
}
