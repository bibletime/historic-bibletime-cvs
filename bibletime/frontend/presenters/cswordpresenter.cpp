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

CSwordPresenter::CSwordPresenter(ListCSwordModuleInfo useModules, QWidget *parent, const char *name )
	: KMainWindow(parent,name/*,0*/,WDestructiveClose),
	m_mainToolBar(0),
	m_popup(0), m_savePopup(0), m_copyPopup(0), m_printPopup(0),
	m_lexiconPopup(new QPopupMenu(this)),	
	m_keyChooser(0),
	m_htmlWidget(0),
	m_moduleChooserBar(0),	
	m_displaySettingsButton(0),
	m_moduleList(useModules),
	m_accel(0),
	m_moduleOptions( CBTConfig::getFilterOptionDefaults() ),	
	m_displayOptions( CBTConfig::getDisplayOptionDefaults() ),
	m_features(0),
	m_initialized(false)	
{		
	ListCSwordModuleInfo& modules =  backend()->moduleList();
	for (modules.first(); modules.current(); modules.next()) {
		if (modules.current()->type() == CSwordModuleInfo::Lexicon) {
			m_lexiconPopup->insertItem( modules.current()->name() );
		}
	}	
	connect(m_lexiconPopup, SIGNAL(activated(int)),this, SLOT(lookupWord(int)));
	setMinimumWidth(300);
	resize(350,350);	
	setCaption(windowCaption());	
	
	setFocusPolicy(QWidget::WheelFocus);
}

CSwordPresenter::~CSwordPresenter() {
//  m_accel->writeSettings();
}

/** Returns the features used by this presenter. */
int CSwordPresenter::getFeatures(){
	return m_features;
}

void CSwordPresenter::lookupWord(int moduleID){
	const QString module = m_lexiconPopup->text(moduleID);	
	const QString text = m_htmlWidget->selectedText();
	if (!text.isEmpty())
		emit lookupInLexicon(text,module);
}

/** Look up the key given as parameter. */
void CSwordPresenter::lookup(const QString& module, const QString& key){

}

/** Returns the listof used modules. */
ListCSwordModuleInfo& CSwordPresenter::getModuleList() {
	return m_moduleList;
}

/** Returns the used keychooser object of this presenter. */
CKeyChooser* CSwordPresenter::keyChooser() const {
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
	QWidget::setCaption( windowCaption() ); //set everytime our own caption
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
	settings->setType(m_moduleList.first()->type());
	settings->setMaximized(isMaximized() || parentWidget()->isMaximized());
	
	CSwordKey* key = keyChooser()->key();
	if (key) {
		VerseKey* vk = dynamic_cast<VerseKey*>(key);
		QString oldLang;
		if (vk) {
			 oldLang = QString::fromLatin1(vk->getLocale());	
			vk->setLocale("en"); //save english locale names as default!		
		}
		settings->setKey( key->key() );
		if (vk) {
			vk->setLocale(oldLang.latin1());
		}
	}
		
	QStringList modules;
	for (CSwordModuleInfo* m = m_moduleList.first(); m; m = m_moduleList.next()) {
		modules.append(m->name());
	}	
	settings->setModules(modules);
}

/** Is called when the presenter should be closed. To delete the presenter it emits "close(CPresenter*)".*/
void CSwordPresenter::closeEvent(QCloseEvent* e) {
	if (!queryClose()) {
		e->ignore();
	}	
	else {
		e->accept();
//    deleteLater();
//		emit(closePresenter(this));
	}
}

/** Inserts the action used by this display window in the given KAccel object. */
void CSwordPresenter::insertKeyboardActions( KAccel* const a ){
//  qWarning("CSwordPresenter::insertKeyboardActions( KAccel* const a )");
}

void CSwordPresenter::initAccels(){
}

/** Initilizes widget before shown and after constructor. */
void CSwordPresenter::polish(){
	KMainWindow::polish();
	m_accel = new KAccel(this);	
  m_accel->setAutoUpdate(true);
	initAccels();
}

/** Is called when this display window looses the focus. */
void CSwordPresenter::refresh(){
//	qWarning("CSwordPresenter::refresh()");
	m_moduleOptions = CBTConfig::getFilterOptionDefaults();
	m_displayOptions = CBTConfig::getDisplayOptionDefaults();

	if (m_displaySettingsButton)
		m_displaySettingsButton->reset(m_moduleList);
}

/** Is called when a reference was dropped on this window. */
void CSwordPresenter::referenceDropped(const QString& ref){
	lookup(m_moduleList.first()->name(), ref);
}

/** Reimplementation from KMainWindow. */
bool CSwordPresenter::queryClose(){
	return true;
}

/** Returns true if the display window is fully initialized. */
const bool CSwordPresenter::initialized(){
	return m_initialized;
}

/** Sets the display window to the initialized state. */
void CSwordPresenter::setInitialized(){
	m_initialized = true;
}

/** This function is called when the client was activated. */
void CSwordPresenter::activated( const bool enable){
//  qWarning("CSwordPresenter::activated( const bool activated)");
  if (m_accel) {
//    qWarning("set enabled to %i", enable);
    m_accel->setEnabled(enable);
  }
}
