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
#include "frontend/cbtconfig.h"

//STL headers
#include <algorithm>
#include <iterator>
#include <map>
//#include <ostream>

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

inline const bool my_cmpEntries(const QString& a, const QString& b) {
//  qWarning("ci_cmpEntries(%s, %s)", a.latin1(),b.latin1());
//  return a.lower() < b.lower();
  return a < b;
};

/** Reimplementation. */
void CLexiconKeyChooser::refreshContent(){
  if (m_modules.count() == 1) {                                                                              
    m_widget->reset(m_modules.first()->entries(), 0, true);	
  }
  else {
    typedef std::multimap<unsigned int, QStringList*> EntryMap;
    EntryMap entryMap;
    QStringList* entries = 0;
    for (m_modules.first(); m_modules.current(); m_modules.next()) {
      entries = m_modules.current()->entries();
      entryMap.insert( std::make_pair(entries->count(), entries) );
    }

    QStringList goodEntries; //The string list which contains the entries which are available in all modules
    
    EntryMap::iterator it = entryMap.begin(); //iterator to go thoigh all selected modules    
    QStringList refEntries = *(it->second); //copy the items for the first time
    QStringList* cmpEntries = ( ++it )->second; //list for comparision, starts with the second module in the map   

    while(it != entryMap.end()) {
      std::set_intersection(
        refEntries.begin(), --(refEntries.end()), //--end() is the last valid entry
        cmpEntries->begin(), --(cmpEntries->end()),
        std::back_inserter(goodEntries), //append valid entries to the end of goodEntries
        my_cmpEntries  //ci_cmpEntries is the comparision function
      );

      cmpEntries = ( ++it )->second; //this is a pointer to the string list of a new module
      
      /*
      * use the good entries for next comparision,
      * because the final list can only have the entries of goodEntries as maxiumum
      */
      refEntries = goodEntries;
    };
    
    m_widget->reset(goodEntries, 0, true); //write down the entries
  } //end of ELSE
}

/** No descriptions */
void CLexiconKeyChooser::adjustFont(){
	//Make sure the entries are displayed correctly.
//	if ( m_modules.first()->isUnicode() )
//		m_widget->comboBox()->setFont( CBTConfig::get(CBTConfig::unicode) );
//	else
//		m_widget->comboBox()->setFont( CBTConfig::get(CBTConfig::standard) );
		m_widget->comboBox()->setFont( CBTConfig::get( m_modules.first()->language() ).second );
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
