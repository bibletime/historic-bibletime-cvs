/***************************************************************************
                          cbookkeychooser.cpp  -  description
                             -------------------
    begin                : Sat Jan 26 2002
    copyright            : (C) 2002 by The BibleTime team
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

#include "cbookkeychooser.h"
#include "../../backend/cswordtreekey.h"
#include "../../backend/cswordbookmoduleinfo.h"

//Qt includes
#include <qlayout.h>

CBookKeyChooser::CBookKeyChooser(CSwordModuleInfo *module, CSwordKey *key, QWidget *parent, const char *name)
	: CKeyChooser(module, key, parent,name), m_layout(0) {
	if ( module && (module->type() == CSwordModuleInfo::GenericBook) ) {
		m_module = dynamic_cast<CSwordBookModuleInfo*>(module);		
		m_key = dynamic_cast<CSwordTreeKey*>(key);
	}
	else {
		m_module = 0;
		m_key = 0;
	}
	setModule(m_module);
}

CBookKeyChooser::~CBookKeyChooser(){
}

void CBookKeyChooser::setKey(CSwordKey* newKey){
	setKey(newKey, true);
}

/** Sets a new key to this keychooser */
void CBookKeyChooser::setKey(CSwordKey* newKey, const bool emitSignal){
	if (m_key != newKey )
		m_key = dynamic_cast<CSwordTreeKey*>(newKey);
	
	const QString oldKey = m_key->key();
	QStringList siblings;
	if (m_key && !oldKey.isEmpty())
		siblings = QStringList::split("/",oldKey,false);

	unsigned int depth = 0;
	int index = 0;
	
	m_key->root();
	while(m_key->firstChild() && (depth <= siblings.count())) {
		const QString key = m_key->key();
		index = 0;
		const QString sibling = siblings[depth];
		if (!sibling.isEmpty()) { //found it
			bool found = false;			
			do {
				++index;
				found = (m_key->getLocalName() == sibling);
			} while (!found && m_key->nextSibling());			
			if (!found)
				m_key->key( key );
		}	
		setupCombo(key, depth++, index);		
	}
	
	//clear the combos which were not filled
	for (; depth < m_module->depth(); ++depth)  {
		CKeyChooserWidget* chooser = m_chooserWidgets.at(depth);
		if (chooser)
			chooser->reset(0,0,false);
	}
	
	if (oldKey.isEmpty())
		m_key->root();
	else
		m_key->key(oldKey);
	
	if (emitSignal)
		emit keyChanged(m_key);
}

/** Returns the key of this kechooser. */
CSwordKey* const CBookKeyChooser::key(){
//	ASSERT(m_key);
	return m_key;
}

/** Sets another module to this keychooser */
void CBookKeyChooser::setModule(CSwordModuleInfo* module){
	m_module = dynamic_cast<CSwordBookModuleInfo*>(module);	
	//refresh the number of combos
	if (m_module && m_key) {
		if (!m_layout)
			m_layout = new QHBoxLayout(this);

		//delete old widgets
		m_chooserWidgets.setAutoDelete(true);
		m_chooserWidgets.clear();
		m_chooserWidgets.setAutoDelete(false);
		
		for (int i = 0; i < m_module->depth(); ++i) {			
			CKeyChooserWidget* w = new CKeyChooserWidget(0, false, this); //empty keychooser
			m_chooserWidgets.append( w );
			w->show();			
			connect(w, SIGNAL(changed(int)), SLOT(keyChooserChanged(int)));			
			m_layout->addWidget(w);
		}
		updateKey(m_key);		
	}
}

/** Refreshes the content. */
void CBookKeyChooser::refreshContent(){
	if (m_key)
		updateKey( m_key ); //refresh with current key
}

void CBookKeyChooser::setupCombo(const QString key, const int depth, const int currentItem){
	CKeyChooserWidget* chooserWidget = m_chooserWidgets.at(depth);
	if (depth == 0 && chooserWidget && chooserWidget->comboBox()->count()) { //has already items
		//set now the right item		
		CKeyChooserWidget* chooserWidget = m_chooserWidgets.at(depth);
//		ASSERT(chooserWidget);
		if (chooserWidget) {
			chooserWidget->setItem( chooserWidget->comboBox()->text(currentItem) );
		}
		return;
	}
	
	const QString oldKey = m_key->key();	
	m_key->key(key);
	
	//insert an empty item at the top
	QStringList items;	
	items << QString::null;
	do {
		items << QString::fromLocal8Bit(m_key->getLocalName());
	}
	while (m_key->nextSibling());
		
	if (chooserWidget) {
		chooserWidget->reset(items,currentItem,false);
	}
	
	//restore old key
	m_key->key(oldKey);	
}

/** A keychooser changed. Update and emit a signal if necessary. */
void CBookKeyChooser::keyChooserChanged(int newIndex){
	QStringList items;
	CKeyChooserWidget* chooser;
	const int count = m_chooserWidgets.count();
	for (int i = 0; i < count; ++i) {
		chooser = m_chooserWidgets.at(i);
		const QString currentText = (chooser && chooser->comboBox()) ? chooser->comboBox()->currentText() : QString::null;
		if (currentText.isEmpty())
			break;
		items << currentText;
	}
	QString newKey = QString::fromLatin1("/") + items.join("/");
 qWarning("CBookKeyChooser::keyChooserChanged: %s", newKey.latin1());
	if (newKey.length() > 1)
		newKey.remove(newKey.length(),1); //remove the traling slash
	
	m_key->key(newKey);	
	setKey(m_key);
}

/** Updates the keychoosers for the given key but emit no signal. */
void CBookKeyChooser::updateKey(CSwordKey* key){
	setKey(key, false);
}
