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

CHTMLWriteDisplay::CHTMLWriteDisplay(CWriteWindow* parentWindow, QWidget* parent)
  : CPlainWriteDisplay(parentWindow,parent)
{
//  qWarning("constructor of CHTMLWriteDisplay");
  setTextFormat(Qt::RichText);

//  connect(this, SIGNAL(textChanged()),
//    connectionsProxy(), SLOT(emitTextChanged()));

  //connection for format stuff
  connect(connectionsProxy(), SIGNAL(toggleBold()),
    this, SLOT(toggleBold()));
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
