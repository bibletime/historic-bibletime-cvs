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
	: QWidget(parent, name),
		m_keyHistoryList(0),
		m_currentHistoryPosition(0),
		m_inHistoryFunction(false)
{
	m_keyHistoryList = new QStringList();
	
	connect(this, SIGNAL(keyChanged(CSwordKey*)), SLOT(addToHistory(CSwordKey*)));
}

CKeyChooser::~CKeyChooser() {
 	delete m_keyHistoryList;
// 	m_keyHistoryList = 0;
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

void CKeyChooser::backInHistory() {
// 	qWarning("CKeyChooser::back, %d@%d", m_currentHistoryPosition, m_keyHistoryList.count());
	if (m_inHistoryFunction)
		return;
	m_inHistoryFunction = true;
	
	if (m_currentHistoryPosition < m_keyHistoryList->count()) {
		m_currentHistoryPosition++;
	
		if (key()) {
			CSwordKey* k = key();
			k->key((*m_keyHistoryList)[m_currentHistoryPosition]);
 			setKey(k);
		}
	}

	m_inHistoryFunction = false;
}

void CKeyChooser::forwardInHistory() {
// 	qWarning("CKeyChooser::forward, %d@%d", m_currentHistoryPosition, m_keyHistoryList.count());
	
	if (m_inHistoryFunction)
		return;
		
	m_inHistoryFunction = true;
	
	if (m_currentHistoryPosition > 0) {
		m_currentHistoryPosition--;
		
		if (key()) {
			CSwordKey* k = key();
			k->key((*m_keyHistoryList)[m_currentHistoryPosition]);
 			setKey(k);
		}
	}

	m_inHistoryFunction = false;
}

void CKeyChooser::addToHistory(CSwordKey* key) {
	if (key && !m_inHistoryFunction) {
		m_keyHistoryList->prepend(key->key());
	}
}
