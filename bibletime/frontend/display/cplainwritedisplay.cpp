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

  connect(this, SIGNAL(textChanged()),
    connectionsProxy(), SLOT(emitTextChanged()));
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

const QString CPlainWriteDisplay::text( const CDisplay::TextType format, const CDisplay::TextPart part) {
  return QString::null;
};

void CPlainWriteDisplay::print( const CDisplay::TextPart ) {

};

/** Sets the current status of the edit widget. */
void CPlainWriteDisplay::setModified( const bool modified ){
  QTextEdit::setModified(modified);
}

/** Reimplementation. */
const bool CPlainWriteDisplay::isModified() const{
  return QTextEdit::isModified();
}


/** Returns the text of this edit widget. */
const QString CPlainWriteDisplay::plainText(){
  return QTextEdit::text();
}

/** Reimplementation from QTextEdit. Provides an popup menu for the given position. */
QPopupMenu* CPlainWriteDisplay::createPopupMenu( const QPoint& pos ){
  return installedPopup();
}

/** Reimplementation from QTextEdit. Provides an popup menu for the given position. */
QPopupMenu* CPlainWriteDisplay::createPopupMenu( ){
  return installedPopup();
}
