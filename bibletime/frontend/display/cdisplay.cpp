/***************************************************************************
                          cdisplay.cpp  -  description
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

#include "cdisplay.h"
#include "chtmlreaddisplay.h"

#include "backend/creferencemanager.h"

#include "util/ctoolclass.h"


//Qt includes
#include <qclipboard.h>

//KDE includes
#include <kapplication.h>
#include <kfiledialog.h>
#include <klocale.h>


CDisplayConnections::CDisplayConnections( CDisplay* display ) : m_display(display) {
}

void CDisplayConnections::selectAll() {
	m_display->selectAll();
}

void CDisplayConnections::saveAsHTML() {
	m_display->save(CDisplay::HTMLText, CDisplay::Document);
}

void CDisplayConnections::saveAsPlain() {
	m_display->save(CDisplay::PlainText, CDisplay::Document);
}

/** Emits the signal. */
void CDisplayConnections::emitReferenceClicked( const QString& module, const QString& key){
	qWarning("CDisplayConnections::emitReferenceClicked( const QString& module, const QString& key)");
 	emit referenceClicked(module, key);
}

/** Emits the signal. */
void CDisplayConnections::emitReferenceDropped( const QString& key){
	qWarning("CDisplayConnections::emitReferenceDropped( const QString& module, const QString& key)");	
 	emit referenceDropped(key);
}

/** No descriptions */
void CDisplayConnections::copyAll(){
	m_display->copy(CDisplay::PlainText, CDisplay::Document);
}

/** No descriptions */
void CDisplayConnections::copySelection(){
	m_display->copy(CDisplay::PlainText, CDisplay::SelectedText);
}

void CDisplayConnections::printAll() {
	m_display->print(CDisplay::Document);
}

void CDisplayConnections::printAnchorWithText() {
	m_display->print(CDisplay::AnchorWithText);
}

void CDisplayConnections::copyAnchorOnly(){
	m_display->copy(CDisplay::PlainText, CDisplay::AnchorOnly);
}

void CDisplayConnections::copyAnchorTextOnly(){
	m_display->copy(CDisplay::PlainText, CDisplay::AnchorTextOnly);
}

void CDisplayConnections::copyAnchorWithText(){
	m_display->copy(CDisplay::PlainText, CDisplay::AnchorWithText);
}

void CDisplayConnections::saveAnchorWithText(){
	m_display->save(CDisplay::PlainText, CDisplay::AnchorWithText);
}

void CDisplayConnections::clear(){
  m_display->setText(QString::null);
}

/*----------------------*/

CReadDisplay* CDisplay::createReadInstance( CReadWindow* readWindow, QWidget* parent ) {
	Q_ASSERT(readWindow);
  Q_ASSERT(parent);
// 	if (readWindow)
	 	return new CHTMLReadDisplay(readWindow, parent);
//  else
//  	return 0;
}

CWriteDisplay* CDisplay::createWriteInstance( CWriteWindow* writeWindow ) {
	return 0;
}


CDisplay::CDisplay(CDisplayWindow* parent) : m_parentWindow(parent), m_connections( new CDisplayConnections( this ) ) {
//  qWarning("constructor of CDisplay");
}

CDisplay::~CDisplay(){
}

const bool CDisplay::copy( const CDisplay::TextType format, const CDisplay::TextPart part  ) {
//  qWarning("CDisplay::copy( const CDisplay::TextType format, const CDisplay::TextPart part  )");
  const QString content = text(format, part);

  QClipboard* cb = KApplication::clipboard();
  cb->setText(content);
  return true;
}

const bool CDisplay::save( const CDisplay::TextType format, const CDisplay::TextPart part ) {
//  qWarning("CDisplay::save( const CDisplay::TextType format, const CDisplay::TextPart part  )");
  const QString content = text(format, part);
  QString filter = QString::null;

  switch (format) {
    case HTMLText:
      filter = QString::fromLatin1("*.html *.htm | ") + i18n("HTML files") + QString::fromLatin1("\n *.* | All files (*.*)");
      break;
    case PlainText:
      filter = QString::fromLatin1("*.txt | ") + i18n("Text files") + QString::fromLatin1("\n *.* | All files (*.*)");
      break;
  };

  const QString filename = KFileDialog::getSaveFileName(QString::null, filter, 0, i18n("Save document ..."));
  if (!filename.isEmpty()) {
    CToolClass::savePlainFile(filename, content);
  }
  return true;
}

/** Emits the signal which used when a reference was clicked. */
void CDisplay::emitReferenceClicked( const QString& reference ){
  QString module;
  QString key;
  CReferenceManager::Type type;
  /*const bool ok = */ CReferenceManager::decodeHyperlink(reference, module, key, type);
  if (module.isEmpty()) {
    module = CReferenceManager::preferredModule( type );
  }
  m_connections->emitReferenceClicked(module, key);
}

/** Used when a reference was dropped onto the widget. */
void CDisplay::emitReferenceDropped( const QString& reference ){
  QString module;
  QString key;
  CReferenceManager::Type type;
  /*const bool ok = */ CReferenceManager::decodeHyperlink(reference, module, key, type);
//  if (module.isEmpty()) {
//    module = CReferenceManager::preferredModule( type );
//  }

  m_connections->emitReferenceDropped(key);
}

/** Returns the connections obect used for signas and slots. */
CDisplayConnections* const CDisplay::connectionsProxy() const{
	return m_connections;
}

CDisplayWindow* const CDisplay::parentWindow() const {
//	qWarning("CDisplay::parentWindow()");
 	return m_parentWindow;
}
