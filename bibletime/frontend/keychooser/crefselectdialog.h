/*************************************************************************** 
    crefselectdialog.h  -  description
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

#ifndef CREFSELECTDIALOG_H
#define CREFSELECTDIALOG_H
//Qt includes
#include <qwidget.h>
#include <qstring.h>
class QLineEdit;

//KDE includes
#include <kdialog.h>
#include <kdialogbase.h>


// BT includes
#include "ckeychooser.h"

class CRefSelectDialog : public KDialogBase  {
  Q_OBJECT
 public:
  CRefSelectDialog(QWidget *parent);
  virtual ~CRefSelectDialog();

  void setChooser(CKeyChooser* const chooser);

 public slots:
   void slotPressedOk();
   void slotPressedCancel(); 


 protected:
   void restoreParent();

 private:
  QWidget*     m_page;
  QWidget*     m_placeholder;
  QWidget*     m_oldParent;
  CKeyChooser* m_chooser;
  QLineEdit*   m_lineedit;
  QPoint       m_oldPos;
};
#endif





