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
  m_layout->setResizeMode(QLayout::FreeResize);

	m_widget = new CKeyChooserWidget(0, false, this);
	m_widget->comboBox()->setMaximumWidth(450);
	m_widget->setToolTips(TT_PRESENTER_ENTRY_COMBO,TT_PRESENTER_NEXT_ENTRY, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_ENTRY);
	m_widget->setWhatsThis(WT_PRESENTER_ENTRY_COMBO,WT_PRESENTER_NEXT_ENTRY, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_ENTRY);
	
	m_layout->addWidget(m_widget,0,Qt::AlignLeft);
	
	connect(m_widget,SIGNAL(changed(int)),SLOT(activated(int)));
	connect(m_widget,SIGNAL(focusOut(int)),SLOT(activated(int)));

  setModules(modules, true);
	adjustFont();
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
    qWarning("return entries of the single module");
    m_widget->reset(m_modules.first()->entries(), 0, true);	
  }
  else {
/**
* 1. Sort the modules by number of entries (raising)
* 2. Use the modules with the fewest entries and go though all of these entries
* 3. Is the entry in all other modules? Remove the entry from the list if it's not.
*/
    typedef QMap<unsigned int, CSwordLexiconModuleInfo*> EntryCountMap;
    EntryCountMap entryCountMap;
    for (m_modules.first(); m_modules.current(); m_modules.next()) {
      entryCountMap.insert(m_modules.current()->entries()->count(), m_modules.current(), false);
    }

    for ( EntryCountMap::Iterator count_it = entryCountMap.begin(); count_it != entryCountMap.end(); ++count_it) {
      qWarning("module %s has count %i", count_it.key(), (*count_it));
    };

    typedef QMap<CSwordLexiconModuleInfo*, QStringList*> LexiconMap;
    LexiconMap entryMap;
    for (m_modules.first(); m_modules.current(); m_modules.next()) {
      entryMap.insert(m_modules.current(), m_modules.current()->entries(), false);
    }
    
    CSwordLexiconModuleInfo* referenceModule = entryCountMap.begin().data();
    QStringList* entries = referenceModule->entries(); //this is a pointer to a string list, only use it for comparision
    QStringList goodEntries; //The string list which contains the entries which are in all modules

    //now see if the entries are in all other modules
    QStringList::ConstIterator it = entries->begin();
    EntryCountMap::ConstIterator module;
    bool inAllModules;
    QStringList* moduleEntries = 0;
    while (it != entries->end()) {
      inAllModules = true;
      for ( module = entryCountMap.begin(), ++module; inAllModules && (module.data() != referenceModule) && (module != entryCountMap.end()); ++module ) {
        moduleEntries = entryMap[ module.data() ];
        inAllModules = moduleEntries && inAllModules && moduleEntries->contains(*it);
      }
      if (inAllModules) { //entry is available everywhere, insert it into the good entries list!
        goodEntries.append( (*it) );
      };
      ++it; //next entry
    } //end of while loop
    m_widget->reset(goodEntries, 0, true);
  } //end of ELSE
}

/** No descriptions */
void CLexiconKeyChooser::adjustFont(){
	//Make sure the entries are displayed correctly.
//	if ( m_modules.first()->isUnicode() )
//		m_widget->comboBox()->setFont( CBTConfig::get(CBTConfig::unicode) );
//	else
//		m_widget->comboBox()->setFont( CBTConfig::get(CBTConfig::standard) );
		m_widget->comboBox()->setFont( CBTConfig::get( m_modules.first()->language() ) );
}

/** Sets the module and refreshes the combo boxes */
void CLexiconKeyChooser::setModules( ListCSwordModuleInfo modules, const bool refresh ) {
  m_modules.clear();
  for (modules.first(); modules.current(); modules.next()) {
    if (CSwordLexiconModuleInfo* lexicon = dynamic_cast<CSwordLexiconModuleInfo*>(modules.current())) {
      m_modules.append(lexicon);
    }
  }
  if (refresh){
    refreshContent();
		adjustFont();
	}
}

/** No descriptions */
void CLexiconKeyChooser::updateKey(CSwordKey* /*key*/){
}
