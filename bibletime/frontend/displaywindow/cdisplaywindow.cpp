/***************************************************************************
                          cdisplaywindow.cpp  -  description
                             -------------------
    begin                : Don Mai 9 2002
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

//BibleTime includes
#include "cdisplaywindow.h"
#include "creadwindow.h"
#include "cbiblereadwindow.h"
#include "ccommentaryreadwindow.h"
#include "clexiconreadwindow.h"
#include "cbookreadwindow.h"
#include "cmodulechooserbar.h"
#include "cdisplaysettingsbutton.h"

#include "backend/cswordkey.h"

#include "frontend/keychooser/ckeychooser.h"

#include "frontend/cmdiarea.h"
#include "frontend/cprofilewindow.h"
#include "frontend/cbtconfig.h"


//Qt includes

//KDE includes
#include <kaccel.h>

CReadWindow* CDisplayWindow::createReadInstance(ListCSwordModuleInfo modules, CMDIArea* parent, const char* name = 0) {
  switch (modules.first()->type()) {
		case CSwordModuleInfo::Bible:
			return new CBibleReadWindow(modules, parent);
		case CSwordModuleInfo::Commentary:
			return new CCommentaryReadWindow(modules, parent);
		case CSwordModuleInfo::Lexicon:
			return new CLexiconReadWindow(modules, parent);
		case CSwordModuleInfo::GenericBook:			
			return new CBookReadWindow(modules, parent);
		default:
			qWarning("unknown module type");
			break;
 	}
  return 0;
}


CWriteWindow* CDisplayWindow::createWriteInstance(ListCSwordModuleInfo modules, CMDIArea* parent, const char* name = 0) {
	return 0;
}

CDisplayWindow::CDisplayWindow(ListCSwordModuleInfo modules, CMDIArea *parent, const char *name ) : KMainWindow(parent,name,WDestructiveClose) {
	m_mdi = parent;
 	m_modules = modules;
  m_isReady = false;
  m_moduleChooserBar = 0;
  m_swordKey = 0;
  m_keyChooser = 0;
  m_mainToolBar = 0;
  m_displaySettingsButton = 0;
}

CDisplayWindow::~CDisplayWindow(){
//	qWarning("destructor of CDisplayWindow");
}

CMDIArea* const CDisplayWindow::mdi() {
  Q_ASSERT(m_mdi);
	return m_mdi;
}

/** Returns the right window caption. */
const QString CDisplayWindow::windowCaption(){
//	qWarning("CDisplayWindow::windowCaption()");
 	if (!m_modules.count())
		return QString::null;

	QString ret = m_modules.first()->name();		
	if (m_modules.count() > 1) {
		for (m_modules.next(); m_modules.current(); m_modules.next())	 {
			ret += " | " + m_modules.current()->name();	
		}
	}
	return ret;
}

/** Returns the used modules as a QPtrList */
ListCSwordModuleInfo& CDisplayWindow::modules(){
//  qWarning("CDisplayWindow::modules()");
//  qWarning("count is %i", m_modules.count());
	return m_modules;
}

/** Set the window caption. */
void CDisplayWindow::setCaption( const QString&  ){
	QWidget::setCaption( windowCaption() );
}

void CDisplayWindow::insertKeyboardActions( KAccel* const accel ) {
}

/** Is called when this window gets the focus or looses the focus. */
void CDisplayWindow::windowActivated( const bool hasFocus ){
	if (accel())
		accel()->setEnabled(hasFocus);
}

/** Reimplementation from QWidget. Used to initialize things before the widget is shown. */
void CDisplayWindow::polish(){
//  qWarning("CDisplayWindow::polish()");
  KMainWindow::polish();
}

/** Refresh the settings of this window. */
void CDisplayWindow::refresh() {
//  qWarning("CDisplayWindow::refresh()");
 	m_filterOptions = CBTConfig::getFilterOptionDefaults();
	m_displayOptions = CBTConfig::getDisplayOptionDefaults();

	if (displaySettingsButton())
		displaySettingsButton()->reset(modules());
}

/** Returns the filter options used by this window. */
CSwordBackend::FilterOptionsBool& CDisplayWindow::filterOptions() {
//  qWarning("CDisplayWindow::filterOptions()");
	return m_filterOptions;
}

/** Returns the display options used by this display window. */
CSwordBackend::DisplayOptionsBool& CDisplayWindow::displayOptions() {
//  qWarning("CDisplayWindow::displayOptions()");	
	return m_displayOptions;
}

/** Sets the new display options for this window. */
void CDisplayWindow::setDisplayOptions( const CSwordBackend::DisplayOptionsBool displayOptions ){
	m_displayOptions = displayOptions;
}

/** Sets the new filter options of this window. */
void CDisplayWindow::setFilterOptions( CSwordBackend::FilterOptionsBool filterOptions ){
  m_filterOptions = filterOptions;
}

/** Returns true if the widget is ready for use. */
const bool CDisplayWindow::isReady() const{
	return m_isReady;
}

/** Set the ready status */
void CDisplayWindow::setReady( const bool ready ){
	m_isReady = ready;
}

/** Returns true if the window may be closed. */
bool CDisplayWindow::queryClose(){
	return true;
}

/** Returns the keychooser widget of this display window. */
CKeyChooser* const CDisplayWindow::keyChooser(){
	return m_keyChooser;
}

/** Sets the keychooser widget for this display window. */
void CDisplayWindow::setKeyChooser( CKeyChooser* ck ){
  m_keyChooser = ck;
}

/** Returns the key of this display window. */
CSwordKey* CDisplayWindow::key() {
	return m_swordKey;
}

/** Sets the new sword key. */
void CDisplayWindow::setKey( CSwordKey* key ){
	m_swordKey = key;
}

void CDisplayWindow::modulesChanged(){
  setModules( m_moduleChooserBar->getModuleList() );
  if (!modules().count())
  	close();
  else {
		if (displaySettingsButton())
  		displaySettingsButton()->reset(modules());
	  key()->module(modules().first());
	  keyChooser()->setModule(modules().first());	
	  lookup(key());
	}
}

/** Lookup the given key. */
void CDisplayWindow::lookup( CSwordKey* key ){
}

/** Returns the module chooser bar. */
CModuleChooserBar* const CDisplayWindow::moduleChooserBar() const{
	return m_moduleChooserBar;
}

/** Sets the module chooser bar. */
void CDisplayWindow::setModuleChooserBar( CModuleChooserBar* bar ){
	if (m_moduleChooserBar)
 		disconnect(m_moduleChooserBar, SIGNAL(sigChanged()), this, SLOT(modulesChanged()));
	m_moduleChooserBar = bar;
	connect(bar, SIGNAL(sigChanged()), SLOT(modulesChanged()));
}

/** Sets the modules. */
void CDisplayWindow::setModules( const ListCSwordModuleInfo newModules ){
	m_modules = newModules;
}

/** Initialize the window. Call this method from the outside, because calling this in the constructor is not possible! */
const bool CDisplayWindow::init( const QString& keyName ){
  initView();
  setMinimumSize( 350,300 );   	
  setCaption(windowCaption());
  show();
 	initConnections();
  initKeyboardActions();
  refresh();

  key()->key(keyName);

  return true;
}

/** Returns the main toolbar. */
KToolBar* const CDisplayWindow::mainToolBar() const{
  return m_mainToolBar;
}

/** Sets the main toolbar. */
void CDisplayWindow::setMainToolBar( KToolBar* bar ){
	m_mainToolBar = bar;
}

/** Returns the display settings button */
CDisplaySettingsButton* const CDisplayWindow::displaySettingsButton() const{
  return m_displaySettingsButton;
}

/** Sets the display settings button. */
void CDisplayWindow::setDisplaySettingsButton( CDisplaySettingsButton* button ){
  if (m_displaySettingsButton)
		disconnect(m_displaySettingsButton, SIGNAL( sigChanged() ),this,	SLOT(lookup() ));
  m_displaySettingsButton = button;	
 	connect(m_displaySettingsButton, SIGNAL(sigChanged()),this, SLOT(lookup()));
}

/** Lookup the current key. Used to refresh the display. */
void CDisplayWindow::lookup(){
//	qWarning("CDisplayWindow::lookup()");
 	lookup( key() );
}

void CDisplayWindow::lookup( const QString& moduleName, const QString& keyName ) {
//  qWarning("CDisplayWindow::lookup( const QString&, const QString& )");
//  Q_ASSERT(isReady());
  if (!isReady())
  	return;


	CSwordModuleInfo* m = backend()->findModuleByName(moduleName);
  if (!m)
    return;

	if (m && modules().containsRef(m) && !keyName.isEmpty()) {
		key()->key(keyName);
		keyChooser()->setKey(key()); //the key chooser does send an update signal
	}
	else {
		QWidgetList windows = mdi()->windowList();
  	bool found = false;
		CDisplayWindow* dw = 0;
  	for (windows.first(); windows.current(); windows.next()) {
    	dw = dynamic_cast<CDisplayWindow*>(windows.current());
     	if (dw && dw->modules().containsRef(m)) {
				found = true;
				break;
			}
		}
		if (found) {
			dw->lookup(moduleName, keyName);
  	}
		else {
    	ListCSwordModuleInfo mList;
     	mList.append(m);
			mdi()->emitCreateDisplayWindow(mList, keyName);
  	}
	}	
}

void CDisplayWindow::lookup( const QString& key ) {
//  qWarning("CDisplayWindow::lookup( const QString& key )");
	lookup(modules().first()->name(), key);
}
