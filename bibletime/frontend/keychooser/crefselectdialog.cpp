/*************************************************************************** 
    crefselectdialog.cpp  -  description
                             -------------------
    begin                : Wed Sep 11 2002
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

#include "crefselectdialog.h"

// QT
#include "qlayout.h"
#include "qlineedit.h"
#include "qlabel.h"

// backend
#include "backend/cswordkey.h"
#include "backend/cswordversekey.h"

//KDE includes
#include <klocale.h>


CRefSelectDialog::CRefSelectDialog(QWidget *parent)
  : KDialogBase(parent, "Refenences", 
		true, i18n("Refenence selector"),
		KDialogBase::Ok|KDialogBase::Cancel, KDialogBase::Ok, true),
  m_oldPos(0,0)
 
{   
   m_page = new QWidget( this );
   m_placeholder = new QWidget( m_page);
   m_layout = new QVBoxLayout( m_page, 0, spacingHint());
   m_lineedit = new QLineEdit( QString::null, m_page, "select" );
   m_lineedit->setMinimumWidth(fontMetrics().maxWidth()*20);
   m_layout->addWidget(m_lineedit);
   m_layout->addWidget(m_placeholder);   
   m_chooser = NULL;
   m_oldParent = NULL;
   setMainWidget(m_page);

   connect(this, SIGNAL(okClicked()), SLOT(slotPressedOk()));
   connect(this, SIGNAL(cancelClicked()), SLOT(slotPressedCancel()));
}

CRefSelectDialog::~CRefSelectDialog() {}


void CRefSelectDialog::setChooser(CKeyChooser* const chooser) {
  if (chooser && chooser != m_chooser) {
    if (m_chooser) restoreParent();

    m_chooser = chooser;

    // store old owner and position
    m_oldParent = chooser->parentWidget(false);   
    m_oldPos = chooser->pos();

    // we temporary borrow chooser widget from the MDI window
    m_placeholder->setMinimumSize(chooser->size());
    chooser->reparent(m_placeholder, QPoint(0,0), true);   
  }
}

void CRefSelectDialog::slotPressedOk() {
  CSwordKey* key = m_chooser->key();
  key->key(m_lineedit->text());
  m_chooser->setKey(key);
  restoreParent();
}

void CRefSelectDialog::slotPressedCancel() {
  restoreParent();
}

void CRefSelectDialog::restoreParent() {
  if (m_chooser && m_oldParent) {   
    m_chooser->reparent(m_oldParent, m_oldPos, true);
  }
  m_chooser = NULL;
}






