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
		m_key = dynamic_cast<CSwordTreeKey*>(CSwordKey::createInstance(m_module));
	}
	else {
		m_module = 0;
		m_key = 0;
		qWarning("CBookKeyChooser: module is not a book!");
		return;
	}		
	ASSERT(m_key);
	refreshContent();
}

CBookKeyChooser::~CBookKeyChooser(){
	
}

/** Sets te module and refreshes the combos */
void CBookKeyChooser::setModule(CSwordModuleInfo* module){
	if (module != m_module && module) {
		m_module = dynamic_cast<CSwordBookModuleInfo*>(module);
		refreshContent();
	}
}

CSwordKey*	CBookKeyChooser::getKey() {

}

void CBookKeyChooser::setKey(CSwordKey *key) {

}

QSize CBookKeyChooser::sizeHint() {
	return QWidget::sizeHint();
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

//	TreeKeyIdx* tree = m_module->getTree();
//	tree->root();
ASSERT(m_key);
	m_key->root();
	if (m_key->firstChild())	{
		do {
//			qWarning("append now %s",QString::fromLocal8Bit(tree->getLocalName()).latin1());		
			m_topElements.append( QString::fromLocal8Bit(m_key->getLocalName()) );
		}  	
		while (m_key->nextSibling());
	}
	m_chooserWidgets.at(0)->reset(m_topElements,0,false);
			
	//fill the keychoosers
	for (int i = 2; i <= maxDepth; i++) {
		setupKeyChooser(i, m_key);
	}
}

/** Sets up the entries of the given key chooser. */
void CBookKeyChooser::setupKeyChooser(const int number, TreeKeyIdx* tree){
	qWarning("CBookKeyChooser::setupKeyChooser");
 	m_key->root(); //first entry
 	for (int chooser = 1; chooser < number; chooser++)	 {
 		m_key->firstChild(); 		
 		//get current entry of first and set tree to this entry
 		const int currentEntry = m_chooserWidgets.at(chooser-1)->comboBox()->currentItem();
 		qWarning("currentEntry == %i", currentEntry);
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
}

/** Set the right content to the key choosers */
void CBookKeyChooser::keyChooserChanged(int){
	//find the combobox which changed
	CKeyChooserWidget* keyChooser = (CKeyChooserWidget*)(sender()); //why does dynamic_cast not work here?
	ASSERT(keyChooser);
	if (!keyChooser)
		return;
	const int pos = m_chooserWidgets.find(keyChooser);
	qWarning("index of changed keyChooser: %i",pos);
	
	//all combos following to the changed combo have to be cleared and refilled
  const int maxDepth = m_module->depth();		
//	TreeKeyIdx* tree = m_module->getTree();
	for (int i = pos+2; i <= maxDepth; i++) { // i+2 because m_chooserWidgets starts with 0 and the found keyChooser needs no update
		setupKeyChooser(i, m_key);
	}
}
