/***************************************************************************
                          cswordpresenter.cpp  -  description
                             -------------------
    begin                : Sun Mar 18 2001
    copyright            : (C) 2001 by The BibleTime team
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

#include "cswordpresenter.h"
#include "../keychooser/ckeychooser.h"
#include "../chtmlwidget.h"
#include "../../backend/cswordkey.h"
#include "../../printing/cprintitem.h"
#include "../../printing/cprinter.h"
#include "../cbtconfig.h"
#include "cdisplaysettingsbutton.h"

//Qt includes
#include <qpopupmenu.h>

//KDE includes
#include <kaccel.h>

CSwordPresenter::CSwordPresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent, const char *name )
	: KMainWindow(parent,name,0), m_moduleList(useModules), m_important(importantClasses), m_htmlWidget(0),
	m_keyChooser(0), m_mainToolBar(0), m_moduleChooserBar(0), m_popup(0),m_savePopup(0),m_copyPopup(0),
	m_printPopup(0), m_features(0), m_accel(0),
	m_lexiconPopup(new QPopupMenu(this)),
	m_moduleOptions( CBTConfig::getAllModuleOptionDefaults() ),
	m_displayOptions( CBTConfig::getAllDisplayOptionDefaults() )

{		
	for (m_important->swordBackend->getModuleList()->first(); m_important->swordBackend->getModuleList()->current(); m_important->swordBackend->getModuleList()->next()) {
		if (m_important->swordBackend->getModuleList()->current()->getType() == CSwordModuleInfo::Lexicon) {
			m_lexiconPopup->insertItem( m_important->swordBackend->getModuleList()->current()->name() );
		}
	}	
	connect(m_lexiconPopup, SIGNAL(activated(int)),this, SLOT(lookupWord(int)));
	resize(350,350);	
	setCaption(windowCaption());	
	
	setFocusPolicy(QWidget::WheelFocus);
}


/** Returns the features used by this presenter. */
int CSwordPresenter::getFeatures(){
	return m_features;
}

/** Refreshes the presenter depending on the events given as parameter. */
//void CSwordPresenter::refresh( ){	
//}

/** Prints the key given as parameter. */
void CSwordPresenter::printKey(CSwordKey* start, CSwordKey* stop, CSwordModuleInfo* module) {
	CPrintItem* printItem = new CPrintItem();
	printItem->setModule(module);
	printItem->setStartKey(start);
	if (stop &&start != stop)
		printItem->setStopKey(stop);
	m_important->printer->addItemToQueue(printItem);
}

/** Refreshes the supported features. */
//void CSwordPresenter::refreshFeatures(){
//	m_features = 0;	
//	for (m_moduleList.first(); m_moduleList.current(); m_moduleList.next()) {
//		if (m_moduleList.current()->supportsFeature( CSwordBackend::strongNumbers )){
//			if (m_features != 0)
//				m_features |= strongNumbers;
//			else
//				m_features = strongNumbers;
//		}
//		if (m_moduleList.current()->supportsFeature( CSwordBackend::footnotes )){
//			if (m_features != 0)
//				m_features |= footnotes;
//			else
//				m_features = footnotes;
//		}
//		if (m_moduleList.current()->supportsFeature( CSwordBackend::headings )){
//			if (m_features != 0)
//				m_features |= headings;
//			else
//				m_features = headings;
//		}
//		if (m_moduleList.current()->supportsFeature( CSwordBackend::morphTags )){
//			if (m_features != 0)
//				m_features |= morphTags;
//			else
//				m_features = morphTags;
//		}		
//	}
//}

void CSwordPresenter::lookupWord(int moduleID){
	const QString module = m_lexiconPopup->text(moduleID);	
	const QString text = m_htmlWidget->selectedText();
	if (!text.isEmpty())
		emit lookupInLexicon(text,module);
}

/** Look up the key given as parameter. */
void CSwordPresenter::lookup(const QString& module, const QString& key){
//	CSwordModuleInfo* m = m_important->swordBackend->findModuleByName(module);
//	if (m && m_moduleList.containsRef(m)) {
//		if (!key.isEmpty())
//			m_key->key(key);
//		m_keyChooser->setKey(m_key); //the key chooser does send an update signal	
//	}
//	else {
//		emit lookupInModule(module, key);
//	}
}

/** Returns the listof used modules. */
ListCSwordModuleInfo& CSwordPresenter::getModuleList() {
	return m_moduleList;
}

/** Returns the used keychooser object of this presenter. */
CKeyChooser* CSwordPresenter::getKeyChooser() const {
	return m_keyChooser;
}

/** No descriptions */
const QString CSwordPresenter::windowCaption() {
	QString ret;
	if (!m_moduleList.count())  {
		return QString::null;
	}
	ret = m_moduleList.first()->name();		
	if (m_moduleList.count() > 1) {
		for (m_moduleList.next(); m_moduleList.current(); m_moduleList.next())	 {
			ret += " | " + m_moduleList.current()->name();	
		}
	}
	return ret;
}

/** Sets the caption of this display window */
void CSwordPresenter::setCaption(const QString&){
//	qWarning("CSwordPresenter::setCaption");
	QWidget::setCaption( windowCaption() ); //set everytime our own caption
	qWarning(caption().local8Bit());
}

/** Applies the settings given in the parameter to the window. */
void CSwordPresenter::applySettings( CProfileWindow* settings ){
	setUpdatesEnabled(false);
	
	if (settings->maximized()) {
		showMaximized();
	}
	else {	
		const QRect rect = settings->geometry();
		resize(rect.width(), rect.height());
		parentWidget()->move(rect.x(), rect.y());
		//setGeometry( settings->geometry() );
	}
	m_htmlWidget->horizontalScrollBar()->setValue( settings->scrollbarPositions().horizontal );
	m_htmlWidget->verticalScrollBar()->setValue( settings->scrollbarPositions().vertical );
	
	setUpdatesEnabled(true);	
}

/** Stores the settings of this window in the CProfileWindow object given as parameter. */
void CSwordPresenter::storeSettings( CProfileWindow* settings ){
	QRect rect;
	rect.setX(parentWidget()->x());
	rect.setY(parentWidget()->y());
	rect.setWidth(width());
	rect.setHeight(height());
	settings->setGeometry(rect);
		
	settings->setScrollbarPositions( m_htmlWidget->horizontalScrollBar()->value(), m_htmlWidget->verticalScrollBar()->value() );
	settings->setType(m_moduleList.first()->getType());
	settings->setMaximized(isMaximized() || parentWidget()->isMaximized());
	CSwordKey* key = getKeyChooser()->getKey();
	if (key)
		settings->setKey( key->key() );
		
	QStringList modules;
	for (CSwordModuleInfo* m = m_moduleList.first(); m; m = m_moduleList.next()) {
		modules.append(m->name());
	}	
	settings->setModules(modules);
}

/** Is called when the presenter should be closed. To delete the presenter it emits "close(CPresenter*)".*/
void CSwordPresenter::closeEvent(QCloseEvent* e) {
	e->accept();
	emit(closePresenter(this));
}

/** Inserts the action used by this display window in the given KAccel object. */
void CSwordPresenter::insertKeyboardActions( KAccel* a ){
	a->setConfigGroup("General window");	
	ASSERT(a);
}

void CSwordPresenter::initAccels(){
	qWarning("CSwordPresenter::initAccels()");
}

/** Initilizes widget before shown and after constructor. */
void CSwordPresenter::polish(){
	qWarning("CSwordPresenter::polish()");
	m_accel = new KAccel(this);	
	
	KMainWindow::polish();	
	
//	refreshFeatures();
	initAccels();
}

/** Is called when this display window looses the focus. */
void CSwordPresenter::focusInEvent( QFocusEvent* e ){
	qDebug("CSwordPresenter::focusInEvent( QFocusEvent* e )");
	KMainWindow::focusInEvent(e);
//	if (m_accel)
//		m_accel->setEnabled(true);
}

/** Is called when this display window looses the focus. */
void CSwordPresenter::focusOutEvent( QFocusEvent* e ){
	qDebug("CSwordPresenter::focusOutEvent( QFocusEvent* e )");	
	KMainWindow::focusOutEvent(e);	
//	if (m_accel)
//		m_accel->setEnabled(false);
}

/** Is called when this display window looses the focus. */
void CSwordPresenter::refresh(){
	m_moduleOptions = CBTConfig::getAllModuleOptionDefaults();
	m_displayOptions = CBTConfig::getAllDisplayOptionDefaults();

	m_displaySettingsButton->reset(m_moduleList);
}

/** Is called when a reference was dropped on this window. */
void CSwordPresenter::referenceDropped(const QString& ref){
	lookup(m_moduleList.first()->name(), ref);
}
