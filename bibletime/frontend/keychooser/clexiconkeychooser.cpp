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
#include "../../tooltipdef.h"
#include "../../whatsthisdef.h"
#include "../../backend/cswordlexiconmoduleinfo.h"
#include "../cbtconfig.h"

//Qt includes
#include <qcombobox.h>
#include <qlayout.h>
#include <qlistbox.h>

//KDE includes
#include <klocale.h>

CLexiconKeyChooser::CLexiconKeyChooser(CSwordModuleInfo *info, CSwordKey *key, QWidget *parent, const char *name )
	: CKeyChooser(info, key, parent, name), m_key(0){
		
	m_info = dynamic_cast<CSwordLexiconModuleInfo*>(info);
		
	//we use a layout because the key chooser should be resized to full size
 	m_layout = new QHBoxLayout(this,QBoxLayout::LeftToRight);
	
	m_widget = new CKeyChooserWidget(m_info->getEntries(), false, this);
	m_widget->ComboBox->setMaximumWidth(200);
	
  if (info && info->encoding() == QFont::Unicode){
#warning implement reaction to font change in the optionsdialog here
    /*m_widget->ComboBox->*/setFont( CBTConfig::get(CBTConfig::unicode) );
  }

	m_widget->setToolTips(TT_PRESENTER_ENTRY_COMBO,TT_PRESENTER_NEXT_ENTRY, TT_PRESENTER_SCROLL_BUTTON, TT_PRESENTER_PREVIOUS_ENTRY);
	m_widget->setWhatsThis(WT_PRESENTER_ENTRY_COMBO,WT_PRESENTER_NEXT_ENTRY, WT_PRESENTER_SCROLL_BUTTON, WT_PRESENTER_PREVIOUS_ENTRY);
	
	m_layout->addWidget(m_widget,0,Qt::AlignLeft);
	
	connect(m_widget,SIGNAL(changed(int)),SLOT(activated(int)));
	connect(m_widget,SIGNAL(focusOut(int)),SLOT(activated(int)));

	setKey(key);
}

CSwordKey* CLexiconKeyChooser::getKey(){
	return m_key;
}

void CLexiconKeyChooser::setKey(CSwordKey* key){	
	if (!(m_key = dynamic_cast<CSwordLDKey*>(key)))
		return;		
	m_widget->ComboBox->setCurrentItem(
		m_widget->ComboBox->listBox()->index(
			m_widget->ComboBox->listBox()->findItem( m_key->key() )));
//	m_widget->adjustSize();
	emit keyChanged( m_key );
}

void CLexiconKeyChooser::activated(int index){
	const QString text = m_widget->ComboBox->text(index);	
	/*to prevent from eternal loop, because activated()is emitted again*/
 	if (m_key->key() != text) {
		m_key->key(text); 	
	 	setKey(m_key);
	}
}

/** Reimplementation. */
void CLexiconKeyChooser::refreshContent(){
//	m_layout->invalidate();	
	m_widget->reset(m_info->getEntries(), 0, true);	
//	updateGeometry();	
}

/** Sets the module and refreshes the combo boxes */
void CLexiconKeyChooser::setModule( CSwordModuleInfo* module) {
	if (module && module != m_info && (dynamic_cast<CSwordModuleInfo*>(module))->getType() == CSwordLexiconModuleInfo::Lexicon) {
		m_info = dynamic_cast<CSwordLexiconModuleInfo*>(module);
		refreshContent();
	}
}
