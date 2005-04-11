/********* Read the file LICENSE for license details. *********/

#include "ckeychooser.h"
#include "../../backend/cswordmoduleinfo.h"
#include "../../backend/cswordbiblemoduleinfo.h"
#include "../../backend/cswordcommentarymoduleinfo.h"
#include "../../backend/cswordlexiconmoduleinfo.h"

#include "clexiconkeychooser.h"
#include "cbiblekeychooser.h"
#include "cbookkeychooser.h"

CKeyChooser::CKeyChooser(ListCSwordModuleInfo, CSwordKey *, QWidget *parent, const char *name )
	: QWidget(parent, name){
}

CKeyChooser* CKeyChooser::createInstance(ListCSwordModuleInfo modules, CSwordKey *key, QWidget *parent){	
	if (!modules.count())
		return 0;

  CKeyChooser* ck = 0;
 	switch ( modules.first()->type() ){
 	  case CSwordModuleInfo::Commentary:	 //Bibles and commentaries uise the same key chooser
 	  case CSwordModuleInfo::Bible:
 	  	ck = new CBibleKeyChooser(modules,key,parent);
      break;
 	  case CSwordModuleInfo::Lexicon:
			ck = new CLexiconKeyChooser(modules,key,parent);	
      break;
 	  case CSwordModuleInfo::GenericBook:
 	  	ck = new CBookKeyChooser(modules,key,parent);
      break;
 	  default:
 	  	return 0;
 	}
  return ck;
}
