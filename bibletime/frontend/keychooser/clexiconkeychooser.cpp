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
#include "../../backend/cmoduleinfo.h"
#include "../../backend/sword_backend/cswordlexiconmoduleinfo.h"
#include "../../backend/ckey.h"

//Qt includes
#include <qcombobox.h>
#include <qlayout.h>
#include <qlistbox.h>

//KDE includes
#include <klocale.h>

CLexiconKeyChooser::CLexiconKeyChooser(CModuleInfo *info, CKey *key, QWidget *parent, const char *name )
	: CKeyChooser(info, key, parent, name){
	
	if ((CSwordLexiconModuleInfo*)info && ((CSwordLexiconModuleInfo*)info)->getType()==CSwordModuleInfo::Lexicon)
		m_info = (CSwordLexiconModuleInfo*)info;
	else {
		qWarning("Wrong module type! Return.");
		return;
	}	
	m_key = 0;
		
	//we use a layout because the key chooser should be resized to full size
 	QHBoxLayout *m_layout = new QHBoxLayout(this,QBoxLayout::LeftToRight);
	
	m_widget = new CKeyChooserWidget(m_info->getEntries(), this);
	m_widget->ComboBox->setMaximumWidth(200);
	m_widget->setToolTips(TT_PRESENTER_ENTRY_COMBO,QString::null, QString::null, QString::null);
	m_widget->setWhatsThis(WT_PRESENTER_ENTRY_COMBO,QString::null, QString::null, QString::null);
	
	m_layout->addWidget(m_widget,0,Qt::AlignLeft);
	
	connect(m_widget,SIGNAL(changed(int)),SLOT(activated(int)));
	connect(m_widget,SIGNAL(focusOut(int)),SLOT(activated(int)));
	connect(m_widget,SIGNAL(prev_requested()),SLOT(prevRequested()));
	connect(m_widget,SIGNAL(next_requested()),SLOT(nextRequested()));

	setKey(key);
}

CKey* CLexiconKeyChooser::getKey(){
	return m_key;
}

void CLexiconKeyChooser::setKey(CKey* key){	
	if (!(m_key = dynamic_cast<CSwordLDKey*>(key)))
		return;		
	m_widget->ComboBox->setCurrentItem(
		m_widget->ComboBox->listBox()->index(
			m_widget->ComboBox->listBox()->findItem( m_key->getKey()  )));
	m_widget->adjustSize();
	emit keyChanged( m_key );
}

void CLexiconKeyChooser::activated(int index){
	QString text = m_widget->ComboBox->text(index);	
	/*to prevent from eternal loop, because activated()is emitted again*/
 	if (m_key->getKey() != text)
		m_key->setKey(text); 	
 	setKey(m_key);
}

/**  */
void CLexiconKeyChooser::prevRequested(void){
	m_key->PreviousEntry();
	setKey(m_key);
}

/**  */
void CLexiconKeyChooser::nextRequested(void){
	m_key->NextEntry();
	setKey(m_key);
}

/** Reimplementation. */
void CLexiconKeyChooser::refreshContent(){
	m_widget->reset(m_info->getEntries(), 0, true);		
}

/** Sets the module and refreshes the combo boxes */
void CLexiconKeyChooser::setModule( CModuleInfo* module){
	if (module && module != m_info && ((CSwordLexiconModuleInfo*)module)->getType()==CSwordLexiconModuleInfo::Lexicon)
	{
		m_info = (CSwordLexiconModuleInfo*)module;
		refreshContent();
	}
}
