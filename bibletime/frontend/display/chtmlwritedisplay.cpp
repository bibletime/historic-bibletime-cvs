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
  : QTextEdit(parentWindow ? parentWindow : parent), CWriteDisplay(parentWindow)
{
  
}

CHTMLWriteDisplay::~CHTMLWriteDisplay(){
}

void CHTMLWriteDisplay::selectAll() {

}

void CHTMLWriteDisplay::setText( const QString& newText ) {

};

const bool CHTMLWriteDisplay::hasSelection() {

};

QWidget* CHTMLWriteDisplay::view() {

};

const QString CHTMLWriteDisplay::text( const CDisplay::TextType format = CDisplay::HTMLText, const CDisplay::TextPart part = CDisplay::Document ) {

};

void CHTMLWriteDisplay::print( const CDisplay::TextPart ) {

};

const bool CHTMLWriteDisplay::isModified() const {

};

void CHTMLWriteDisplay::setModified( const bool modified ) {

};

const QString CHTMLWriteDisplay::plainText() {

};

QPopupMenu* CHTMLWriteDisplay::createPopupMenu( const QPoint& pos ) {

};

QPopupMenu* CHTMLWriteDisplay::createPopupMenu() {

}
