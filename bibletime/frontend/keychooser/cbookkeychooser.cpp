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
	: CKeyChooser(module, key, parent,name)
{
	qWarning("CBookKeyChooser::CBookKeyChooser");
	if ( module && (module->getType() == CSwordModuleInfo::GenericBook) )
		m_module = dynamic_cast<CSwordBookModuleInfo*>(module);
	else {
		m_module = 0;
		qWarning("CBookKeyChooser: module is not a book!");
		return;
	}		
	
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
	// here we have to care for a new module and refresh the tree and the number of the keychoosers
  m_chooserWidgets.setAutoDelete(true);
	m_chooserWidgets.clear(); //delet the existing key chooser widgets
  m_chooserWidgets.setAutoDelete(false);

  const int maxDepth = m_module->depth();	
	QHBoxLayout *layout = new QHBoxLayout(this);		
	layout->setResizeMode(QLayout::Fixed);		
	for (int i = 0; i < maxDepth; i++) {		
		CKeyChooserWidget* newKeyChooser = new CKeyChooserWidget(0,false,this);			
		m_chooserWidgets.append( newKeyChooser );								
		//ToDo: take care for TabOrder
		layout->addWidget(newKeyChooser);		
	}	

	TreeKeyIdx* tree = m_module->getTree();
	tree->root();
	if (tree->firstChild())	{
		do {
			qWarning("append now %s",QString::fromLocal8Bit(tree->getLocalName()).latin1());		
			m_topElements.append( QString::fromLocal8Bit(tree->getLocalName()) );
		}  	
		while (tree->nextSibling());
	}
	else
		qWarning("firstChild() not successful!");	
	m_chooserWidgets.at(0)->reset(m_topElements,0,false);
			
	//fill the keychoosers
	for (int i = 2; i < maxDepth; i++) {
		setupKeyChooser(i, tree);
	}

}

/** Sets up the entries of the given key chooser. */
void CBookKeyChooser::setupKeyChooser(const int number, TreeKeyIdx* tree){
	qWarning("CBookKeyChooser::setupKeyChooser");
 	tree->root(); //first entry
 	for (int chooser = 1; chooser < number; chooser++)	 {
 		tree->firstChild(); 		
 		//get current entry of first and set tree to this entry
 		const int currentEntry = m_chooserWidgets.at(chooser-1)->ComboBox->currentItem();
 		for (int sibling = 1; sibling < currentEntry; sibling++) {
 			tree->nextSibling();
 		}
 		//now we are at the parent entry of the items we want tohave
 	}
 	
 	CKeyChooserWidget* chooserWidget = m_chooserWidgets.at(number-1);
 	QStringList entries;
 	if ( 	tree->firstChild() ) {
	 	do {
	 		entries.append( QString::fromLocal8Bit(tree->getLocalName()) );
	 	}
	 	while (tree->nextSibling());
	 	chooserWidget->reset(entries, 0, false);
	}
}
