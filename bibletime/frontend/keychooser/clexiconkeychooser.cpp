/***************************************************************************
                          clexiconkeychooser.cpp  -  description
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

#include "clexiconkeychooser.h"
#include "ckeychooserwidget.h"
#include "cfx_btn.h"

#include "tooltipdef.h"
#include "whatsthisdef.h"

#include "backend/cswordlexiconmoduleinfo.h"
#include "../cbtconfig.h"

//Qt includes
#include <qcombobox.h>
#include <qlayout.h>
#include <qlistbox.h>

//KDE includes
#include <klocale.h>

CLexiconKeyChooser::CLexiconKeyChooser(ListCSwordModuleInfo modules, CSwordKey *key, QWidget *parent, const char *name )
	: CKeyChooser(modules, key, parent, name), m_key(dynamic_cast<CSwordLDKey*>(key)){

  setModules(modules, false);
	
 //we use a layout because the key chooser should be resized to full size
 	m_layout = new QHBoxLayout(this,QBoxLayout::LeftToRight);

	m_widget = new CKeyChooserWidget(0, false, this);
	m_widget->comboBox()->setMaximumWidth(300);	
	m_widget->setToolTips(TT_PRESENTER_ENTRY_COMBO,TT_PRESENTER_NEXT_ENTRY, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_ENTRY);
	m_widget->setWhatsThis(WT_PRESENTER_ENTRY_COMBO,WT_PRESENTER_NEXT_ENTRY, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_ENTRY);
	
	m_layout->addWidget(m_widget,0,Qt::AlignLeft);
	
	connect(m_widget,SIGNAL(changed(int)),SLOT(activated(int)));
	connect(m_widget,SIGNAL(focusOut(int)),SLOT(activated(int)));

  setModules(modules, true);
}

CSwordKey* const CLexiconKeyChooser::key(){
//  qWarning("CLexiconKeyChooser::key()");
	return m_key;
}

void CLexiconKeyChooser::setKey(CSwordKey* key){	
//	qWarning("CLexiconKeyChooser::setKey(CSwordKey* key)");
 	if (!(m_key = dynamic_cast<CSwordLDKey*>(key)))
		return;		

  QString newKey = m_key->key();
	const int index =	m_widget->comboBox()->listBox()->index(m_widget->comboBox()->listBox()->findItem( newKey ));
  m_widget->comboBox()->setCurrentItem(index);	

  emit keyChanged( m_key );
}

void CLexiconKeyChooser::activated(int index){
//  qWarning("CLexiconKeyChooser::activated(int index)");
	const QString text = m_widget->comboBox()->text(index);	
	/*to prevent from eternal loop, because activated()is emitted again*/
 	if (m_key && m_key->key() != text) {
		m_key->key(text); 	
	 	setKey(m_key);
	}
}

/** Reimplementation. */
void CLexiconKeyChooser::refreshContent(){
  if (m_modules.count() == 1) {
    m_widget->reset(m_modules.first()->entries(), 0, true);	
  }
  else {
/**
* 1. Sort the modules by number of entries (raising)
* 2. Use the modules with the fewest entries and go though all of these entries
* 3. Is the entry in all other modules? Remove the entry from the list if it's not.
*/
    typedef QMap<unsigned int, CSwordLexiconModuleInfo*> LexiconMap;
    LexiconMap lexiconMap;
    for (m_modules.first(); m_modules.current(); m_modules.next()) {
      lexiconMap.insert(m_modules.current()->entries()->count(), m_modules.current(), false);
    }
//    for ( LexiconMap::Iterator lex_it = lexiconMap.begin(); lex_it != lexiconMap.end(); ++lex_it ) {
//      qWarning("%s has %i entries", lex_it.data()->name().latin1(), lex_it.key());
//    }

    CSwordLexiconModuleInfo* referenceModule = lexiconMap.begin().data();
    QStringList entries = QStringList(*(referenceModule->entries()));

//    qWarning("step 1");
    if (lexiconMap.count() == 1) {
//      qWarning("ONLY one module!");
      m_widget->reset(entries, 0, true);	
      return;
    };


    //now see if the entries are in all other modules
    QStringList::Iterator it = entries.begin();
    LexiconMap::Iterator module;
    while (it != entries.end()) {
      for ( module = lexiconMap.begin(), ++module; module.data() != referenceModule && module != lexiconMap.end(); ++module ) {
        if (!module.data()->entries()->contains(*it)) { //entry is not in the module
//          qWarning((*it).latin1());
          it = entries.remove(it);
          break; //no need to look into the other modules because we already removed the entry
        }
        else {
          ++it;
        }
      }
    }

    m_widget->reset(entries, 0, true);	
  }
}

/** Sets the module and refreshes the combo boxes */
void CLexiconKeyChooser::setModules( ListCSwordModuleInfo modules, const bool refresh ) {
  m_modules.clear();
  for (modules.first(); modules.current(); modules.next()) {
    if (CSwordLexiconModuleInfo* lexicon = dynamic_cast<CSwordLexiconModuleInfo*>(modules.current())) {
      m_modules.append(lexicon);
    }
  }
//
//  if (m_modules.first()) {
//    const bool unicode = m_modules.first()->isUnicode() && m_widget;
//    if (unicode)
//      m_widget->comboBox()->setFont( CBTConfig::get(unicode ? CBTConfig::unicode : CBTConfig::standard));
//  };

  if (refresh)
    refreshContent();
}

/** No descriptions */
void CLexiconKeyChooser::updateKey(CSwordKey* key){
}
