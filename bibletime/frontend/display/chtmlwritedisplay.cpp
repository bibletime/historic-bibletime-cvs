/***************************************************************************
                          chtmlwritedisplay.cpp  -  description
                             -------------------
    begin                : Fre Nov 29 2002
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

#include "chtmlwritedisplay.h"
#include "frontend/displaywindow/cwritewindow.h"
#include "frontend/cresmgr.h"

//KDE includes
#include <kaction.h>
#include <ktoolbar.h>
#include <klocale.h>

CHTMLWriteDisplay::CHTMLWriteDisplay(CWriteWindow* parentWindow, QWidget* parent)
  : CPlainWriteDisplay(parentWindow,parent)
{
//  qWarning("constructor of CHTMLWriteDisplay");
  setTextFormat(Qt::RichText);
}

CHTMLWriteDisplay::~CHTMLWriteDisplay(){
}

//void CHTMLWriteDisplay::selectAll() {
//  QTextEdit::selectAll(true);
//}
//
//void CHTMLWriteDisplay::setText( const QString& newText ) {
//  QTextEdit::setText(newText);
//};
//
//const bool CHTMLWriteDisplay::hasSelection() {
//  return hasSelectedText();
//};
//
//QWidget* CHTMLWriteDisplay::view() {
//  return this;
//};
//
//const QString CHTMLWriteDisplay::text( const CDisplay::TextType format = CDisplay::HTMLText, const CDisplay::TextPart part = CDisplay::Document ) {
//#warning "Todo!"
//  return QString::null;
//};
//
//void CHTMLWriteDisplay::print( const CDisplay::TextPart ) {
//
//};
//
//const bool CHTMLWriteDisplay::isModified() const {
//  return QTextEdit::isModified();
//};
//
//void CHTMLWriteDisplay::setModified( const bool modified ) {
//  QTextEdit::setModified(modified);
//};

const QString CHTMLWriteDisplay::plainText() {
  return text();
};

//QPopupMenu* CHTMLWriteDisplay::createPopupMenu( const QPoint& pos ) {
//
//};
//
//QPopupMenu* CHTMLWriteDisplay::createPopupMenu() {
//
//}

void CHTMLWriteDisplay::toggleBold() {
  setBold( !bold() );
};

void CHTMLWriteDisplay::toggleItalic() {
  setItalic( !italic() );
};

void CHTMLWriteDisplay::toggleUnderlined() {
  setUnderline( !underline() );
};

void CHTMLWriteDisplay::setupToolbar(KToolBar * bar, KActionCollection * actions) {
 	m_actions.save = new KAction( i18n("Save the text"),
    CResMgr::displaywindows::writewindow::saveText::icon,
    CResMgr::displaywindows::writewindow::saveText::accel,
    parentWindow(), SLOT( saveCurrentText()  ),
    actions
  );
  m_actions.save->setToolTip( CResMgr::displaywindows::writewindow::saveText::tooltip );
  m_actions.save->setWhatsThis( CResMgr::displaywindows::writewindow::saveText::whatsthis );
  m_actions.save->plug(bar);


 	m_actions.bold = new KAction( i18n("Bold"),
    CResMgr::displaywindows::writewindow::saveText::icon,
    CResMgr::displaywindows::writewindow::saveText::accel,
    this, SLOT( toggleBold()  ),
    actions
  );
  m_actions.bold->setToolTip( CResMgr::displaywindows::writewindow::saveText::tooltip );
  m_actions.bold->setWhatsThis( CResMgr::displaywindows::writewindow::saveText::whatsthis );
  m_actions.bold->plug(bar);

 	m_actions.italic = new KAction( i18n("Italic"),
    CResMgr::displaywindows::writewindow::saveText::icon,
    CResMgr::displaywindows::writewindow::saveText::accel,
    this, SLOT( toggleItalic()  ),
    actions
  );
  m_actions.italic->setToolTip( CResMgr::displaywindows::writewindow::saveText::tooltip );
  m_actions.italic->setWhatsThis( CResMgr::displaywindows::writewindow::saveText::whatsthis );
  m_actions.italic->plug(bar);


 	m_actions.underlined = new KAction( i18n("Bold"),
    CResMgr::displaywindows::writewindow::saveText::icon,
    CResMgr::displaywindows::writewindow::saveText::accel,
    this, SLOT( toggleUnderlined()  ),
    actions
  );
  m_actions.underlined->setToolTip( CResMgr::displaywindows::writewindow::saveText::tooltip );
  m_actions.underlined->setWhatsThis( CResMgr::displaywindows::writewindow::saveText::whatsthis );
  m_actions.underlined->plug(bar);    
}
