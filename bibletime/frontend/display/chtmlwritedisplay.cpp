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
#include <kfontcombo.h>

CHTMLWriteDisplay::CHTMLWriteDisplay(CWriteWindow* parentWindow, QWidget* parent)
  : CPlainWriteDisplay(parentWindow,parent)
{
//  qWarning("constructor of CHTMLWriteDisplay");
  setTextFormat(Qt::RichText);
}

CHTMLWriteDisplay::~CHTMLWriteDisplay(){
}

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
  return QTextEdit::text();
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

void CHTMLWriteDisplay::selectedFontChanged( const QString& fontname ) {
  setFamily( fontname );
};

void CHTMLWriteDisplay::setupToolbar(KToolBar * bar, KActionCollection * actions) {
 	m_actions.bold = new KAction( i18n("Bold"),
    CResMgr::displaywindows::writewindow::boldText::icon,
    CResMgr::displaywindows::writewindow::boldText::accel,
    this, SLOT( toggleBold()  ),
    actions
  );
  m_actions.bold->setToolTip( CResMgr::displaywindows::writewindow::boldText::tooltip );
  m_actions.bold->setWhatsThis( CResMgr::displaywindows::writewindow::boldText::whatsthis );
  m_actions.bold->plug(bar);

 	m_actions.italic = new KAction( i18n("Italic"),
    CResMgr::displaywindows::writewindow::italicText::icon,
    CResMgr::displaywindows::writewindow::italicText::accel,
    this, SLOT( toggleItalic()  ),
    actions
  );
  m_actions.italic->setToolTip( CResMgr::displaywindows::writewindow::italicText::tooltip );
  m_actions.italic->setWhatsThis( CResMgr::displaywindows::writewindow::italicText::whatsthis );
  m_actions.italic->plug(bar);


 	m_actions.underlined = new KAction( i18n("Underline"),
    CResMgr::displaywindows::writewindow::underlinedText::icon,
    CResMgr::displaywindows::writewindow::underlinedText::accel,
    this, SLOT( toggleUnderlined()  ),
    actions
  );
  m_actions.underlined->setToolTip( CResMgr::displaywindows::writewindow::underlinedText::tooltip );
  m_actions.underlined->setWhatsThis( CResMgr::displaywindows::writewindow::underlinedText::whatsthis );
  m_actions.underlined->plug(bar);

  m_fontCombo = new KFontCombo(bar);
  bar->insertWidget(0, m_fontCombo->sizeHint().width(), m_fontCombo);
  connect( m_fontCombo, SIGNAL(activated(const QString&)), this, SLOT(selectedFontChanged(const QString&)) );
}
