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
#include "../../backend/ckey.h"

//Qt includes
#include <qcombobox.h>
#include <qlayout.h>
#include <qlistbox.h>

//KDE includes
#include <klocale.h>

CLexiconKeyChooser::CLexiconKeyChooser(CModuleInfo *info, CKey *key, QWidget *parent, const char *name )
	: CKeyChooser(info, key, parent, name){
	
	ASSERT(info);
	if ((CSwordLexiconModuleInfo*)info)
		m_info = (CSwordLexiconModuleInfo*)info;
	else {
		qWarning("Wrong module type! Return.");
		return;
	}	
	//we use a layout because the key chooser should be resized to full size
	m_layout = new QHBoxLayout(this,QBoxLayout::LeftToRight);
	m_widget = new CKeyChooserWidget(m_info->getEntries(), this);
	m_widget->setToolTips(TT_PRESENTER_ENTRY_COMBO,QString::null, QString::null, QString::null);
	m_widget->setWhatsThis(WT_PRESENTER_ENTRY_COMBO,QString::null, QString::null, QString::null);
	m_layout->addWidget(m_widget,0,Qt::AlignLeft);
	
	connect(m_widget,SIGNAL(changed(int)),this,SLOT(activated(int)));
	connect(m_widget,SIGNAL(prev_requested()),this,SLOT(prevRequested()));
	connect(m_widget,SIGNAL(next_requested()),this,SLOT(nextRequested()));

	setKey(key);
}

CLexiconKeyChooser::~CLexiconKeyChooser(){
}

CKey* CLexiconKeyChooser::getKey(){
	ASSERT(m_key);
	return m_key;
}

void CLexiconKeyChooser::setKey(CKey* key){
	qDebug("CLexiconKeyChooser::setKey(CKey* key)");
	if (dynamic_cast<CSwordLDKey*>(key))
		m_key = dynamic_cast<CSwordLDKey*>(key);
	else {
		qWarning("Wrong key type!");
		m_key = 0;
		return;
	}

	m_widget->ComboBox->setCurrentItem(
		m_widget->ComboBox->listBox()->index(
			m_widget->ComboBox->listBox()->findItem( (const char*)*m_key  )));

	m_widget->adjustSize();
	emit keyChanged( m_key );
}

void CLexiconKeyChooser::activated(int index){
	QString text =  m_widget->ComboBox->text(index);	
	/*
	* to prevent from eternal loop, because activated()
	* is emitted again
	*/
 	if (QString::fromLocal8Bit((const char*)*m_key) != text)
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
	if (module != m_info && (CSwordLexiconModuleInfo*)module)
	{
		m_info = (CSwordLexiconModuleInfo*)module;
		refreshContent();
	}
}
