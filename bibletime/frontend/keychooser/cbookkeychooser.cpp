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
//#include "../../backend/cswordkey.h"
#include "../../backend/cswordtreekey.h"
#include "../../backend/cswordbookmoduleinfo.h"

//Qt includes
#include <qlayout.h>

CBookKeyChooser::CBookKeyChooser(CSwordModuleInfo *module=0, CSwordKey *key=0, QWidget *parent=0, const char *name=0)
	: CKeyChooser(module, key, parent,name), m_key(0)
{
	qWarning("CBookKeyChooser::CBookKeyChooser");
	if ( module && (module->getType() == CSwordModuleInfo::GenericBook) ) {
		m_module = dynamic_cast<CSwordBookModuleInfo*>(module);		
	}
	else {
		m_module = 0;
		m_key = 0;
		qWarning("CBookKeyChooser: module is not a book!");
		return;
	}		
	if (m_module)
		m_key = dynamic_cast<CSwordTreeKey*>(CSwordKey::createInstance(m_module));	
	refreshContent();
}

CBookKeyChooser::~CBookKeyChooser(){
	if (m_key)
		delete m_key;
	m_key = 0;	
}

/** Sets te module and refreshes the combos */
void CBookKeyChooser::setModule(CSwordModuleInfo* module){
	if (module != m_module && module) {
		m_module = dynamic_cast<CSwordBookModuleInfo*>(module);
		delete m_key;
		m_key = dynamic_cast<CSwordTreeKey*>(CSwordKey::createInstance(module));
		refreshContent();
	}
}

CSwordKey*	CBookKeyChooser::getKey() {
	//ToDo: setup getKey in the right way to point to the right entry
	return m_key;
}

void CBookKeyChooser::setKey(CSwordKey *key) {
	qWarning("CBookKeyChooser::setKey(CSwordKey *key)");
	ASSERT(key);

//	qWarning( m_key->getFullName() );
//	
//	emit keyChanged(key);
}

QSize CBookKeyChooser::sizeHint() {
	return QWidget::sizeHint(); // we have to change this eventually
}

void CBookKeyChooser::refreshContent() {
	qWarning("CBookKeyChooser::refreshContent()");
  m_chooserWidgets.setAutoDelete(true);
	m_chooserWidgets.clear();
  m_chooserWidgets.setAutoDelete(false);

  const int maxDepth = m_module->depth();	
  if (!maxDepth)
  	return;
	
	QHBoxLayout *layout = new QHBoxLayout(this);		
	layout->setResizeMode(QLayout::Fixed);		
	for (int i = 0; i < maxDepth; i++) {		
		CKeyChooserWidget* newKeyChooser = new CKeyChooserWidget(0,false,this);
		connect(newKeyChooser, SIGNAL(changed(int)), this, SLOT(keyChooserChanged(int)));
		m_chooserWidgets.append( newKeyChooser );								
		//ToDo: take care for TabOrder
		layout->addWidget(newKeyChooser);		
	}	

	m_key->root();
	if (m_key->firstChild())	{
		do {
			m_topElements.append( QString::fromLocal8Bit(m_key->getLocalName()) );
		}  	
		while (m_key->nextSibling());
	}
	m_chooserWidgets.at(0)->reset(m_topElements,0,false);
			
	//fill the keychoosers
	for (int i = 2; i <= maxDepth; i++) {
		setupKeyChooser(i);
	}
	
	emit keyChanged(m_key);
}

/** Sets up the entries of the given key chooser. */
void CBookKeyChooser::setupKeyChooser(const int number){
	qWarning("CBookKeyChooser::setupKeyChooser");
 	//remember old key
 	const QString oldKey = QString::fromLocal8Bit(m_key->getFullName());
// 	qWarning("oldKey is %s", oldKey.loca8Bit());
 	
 	m_key->root(); //first entry
 	for (int chooser = 1; chooser < number; chooser++)	 {
 		m_key->firstChild();
 		const int currentEntry = m_chooserWidgets.at(chooser-1)->comboBox()->currentItem();
 		for (int sibling = 0; sibling < currentEntry; sibling++) {
 			if (!m_key->nextSibling())
 				break;
 		}
 		//now we are at the parent entry of the items we want to have
 	}
 	
 	CKeyChooserWidget* chooserWidget = m_chooserWidgets.at(number-1);
 	QStringList entries;
 	if (m_key->firstChild()) {
	 	do {
	 		entries.append( QString::fromLocal8Bit(m_key->getLocalName()) );
	 	}
	 	while (m_key->nextSibling());
	}
	else
	 	qWarning("no success calling firstChild()");
 	chooserWidget->reset(entries, 0, false);	
 	
 	//restore old settings
// 	m_key->key( oldKey );
 	qWarning(m_key->key().local8Bit());
}

/** Set the right content to the key choosers */
void CBookKeyChooser::keyChooserChanged(int){
	//find the combobox which changed
	CKeyChooserWidget* keyChooser = (CKeyChooserWidget*)(sender()); //why does dynamic_cast not work here?
	if (!keyChooser)
		return;
	const int pos = m_chooserWidgets.find(keyChooser);
  const int maxDepth = m_module->depth();		

  // i+2 because m_chooserWidgets starts with 0 and the found keyChooser needs no update	
	for (int i = pos+2; i <= maxDepth; i++) {
		setupKeyChooser(i);
	}
	emit keyChanged(m_key);		
}
