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
	: CKeyChooser(module, key, parent,name)
{
	qWarning("CBookKeyChooser::CBookKeyChooser");
	if ( module && (module->type() == CSwordModuleInfo::GenericBook) ) {
		m_module = dynamic_cast<CSwordBookModuleInfo*>(module);		
		m_key = dynamic_cast<CSwordTreeKey*>(/*CSwordKey::createInstance(m_module)*/key);
	}
	else {
		m_module = 0;
		m_key = 0;
	}		
	ASSERT(m_key);
	
	//now setup the keychooser widgets
	if (m_module && m_key) {
		QHBoxLayout* layout = new QHBoxLayout(this);
		for (int i = 0; i < m_module->depth(); ++i) {
			CKeyChooserWidget* w = new CKeyChooserWidget(0, false, this); //empty keychooser
			m_chooserWidgets.append( w );		
			connect(w, SIGNAL(changed(int)), SLOT(keyChooserChanged(int)));			
			layout->addWidget(w);
		}
		layout->addStretch(5);
		
		setKey(m_key);	
	}
}

CBookKeyChooser::~CBookKeyChooser(){
//	delete m_key;
//	m_key = 0;	
}

/** Sets a new key to this keychooser */
void CBookKeyChooser::setKey(CSwordKey* newKey){
	qWarning("CBookKeyChooser::setKey(CSwordKey* newKey)");
	if (m_key != newKey )
		m_key = dynamic_cast<CSwordTreeKey*>(newKey);
	ASSERT(m_key);
	const QString oldKey = m_key->key();
	qWarning("CBookKeyChooser::setKey: %s", oldKey.latin1());
	
	QStringList siblings;
	if (m_key && !oldKey.isEmpty())
		siblings = QStringList::split("/",oldKey,false);
	
	bool ok = true;
	int depth = 0;
	int index = 0;
	
	qWarning("CBookKeyChooser::setKey before setRoot");
	m_key->root();
	m_key->firstChild();
	
	qWarning("CBookKeyChooser::setKey: before loop");	
	do {
		const QString key = m_key->key();
		index = 0;
		QString sibling = siblings[depth];
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
	}	while(m_key->firstChild() && (depth-1 < siblings.count()));
	qWarning("CBookKeyChooser::setKey: after loop");
	
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
	qWarning("CBookKeyChooser::setKey:before emit");		
	emit keyChanged(m_key);
	qWarning("CBookKeyChooser::setKey: after emit");			

	qWarning("CBookKeyChooser::setKey: finished!");					
}

/** Returns the key of this kechooser. */
CSwordKey* CBookKeyChooser::key(){
	qWarning("CBookKeyChooser::key()");
	return m_key;
}

/** Sets another module to this keychooser */
void CBookKeyChooser::setModule(CSwordModuleInfo* module){
	qWarning("CBookKeyChooser::setModule(CSwordModuleInfo* module)");
}

/** Refreshes the content. */
void CBookKeyChooser::refreshContent(){
	qWarning("CBookKeyChooser::refreshContent()");
	if (m_key)
		setKey( m_key ); //refresh with current key
}

void CBookKeyChooser::setupCombo(const QString key, const int depth, const int currentItem){
	qWarning("CBookKeyChooser::setupCombo");
	CKeyChooserWidget* chooserWidget = m_chooserWidgets.at(depth);
	ASSERT(chooserWidget);	
	if (depth == 0 && chooserWidget && chooserWidget->comboBox()->count()) {
//		CKeyChooserWidget* chooserWidget = m_chooserWidgets.at(depth);
//		ASSERT(chooserWidget);
//		if (chooserWidget) {
//			chooserWidget->setItem( chooserWidget->comboBox()->text(currentItem) );
//		};
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
	qWarning("setupCombo: finished while loop");
		
	if (chooserWidget) {
		qWarning("setupCombo: reset chooserWidget");
		chooserWidget->reset(items,currentItem,false);
	}
	
	//restore old key
	qWarning("setupCombo: restore old key!");
	m_key->key(oldKey);	
	qWarning("setupCombo: finished!");
}

/** A keychooser changed. Update and emit a signal if necessary. */
void CBookKeyChooser::keyChooserChanged(int newIndex){
	qWarning("CBookKeyChooser::keyChooserChanged(int newIndex)");
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
	if (newKey.length() > 1)
		newKey.remove(newKey.length(),1); //remove the traling slash
	
	ASSERT(m_key);	
	m_key->key(newKey);	
	setKey(m_key);
	qWarning("CBookKeyChooser::keyChooserChanged finished");	
}
