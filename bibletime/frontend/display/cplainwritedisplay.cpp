/***************************************************************************
                          cplainwritedisplay.cpp  -  description
                             -------------------
    begin                : Fre Okt 11 2002
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

#include "cplainwritedisplay.h"

#include "frontend/displaywindow/cdisplaywindow.h"
#include "frontend/displaywindow/cwritewindow.h"

CPlainWriteDisplay::CPlainWriteDisplay(CWriteWindow* parentWindow, QWidget* parent) : QTextEdit(parentWindow ? parentWindow : parent), CWriteDisplay(parentWindow) {
  qWarning("constructor of CPlainWriteDisplay");
  setTextFormat(Qt::PlainText);
}

CPlainWriteDisplay::~CPlainWriteDisplay(){

}

/** Reimplementation. */
void CPlainWriteDisplay::selectAll(){
  QTextEdit::selectAll(true);
}

void CPlainWriteDisplay::setText( const QString& newText ) {
  QTextEdit::setText(newText);
};

const bool CPlainWriteDisplay::hasSelection() {
  return hasSelectedText();
};

QWidget* CPlainWriteDisplay::view() {
  qWarning("CPlainWriteDisplay::view()");
  return this;
};

const QString CPlainWriteDisplay::text( const CDisplay::TextType format = CDisplay::HTMLText, const CDisplay::TextPart part = CDisplay::Document ) {
  return QString::null;
};

void CPlainWriteDisplay::print( const CDisplay::TextPart ) {

};
