/***************************************************************************
                          actions.cpp  -  Contains KAction derived classes
                             -------------------
    begin                : Wed May 3 2000
    copyright            : (C) 2000 by Joachim Ansorg
    email                : jansorg@crosswire.org
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <assert.h>

#include <qlabel.h>
#include <qpopupmenu.h>
#include <qguardedptr.h>
#include <qcombobox.h>
#include <qwhatsthis.h>
#include <qwidget.h>

#include <ktoolbar.h>
#include <kcombobox.h>
#include <kanimwidget.h>
#include <kdebug.h>
#include <klineedit.h>

#include "actions.h"

/** This class was originally created by Somon Hausmann <hausmann@kde.org>.
	* The copyright of KonqComboAction and KonqLabelAction is owned by him!
	*	We included it and added the QT exception with his agreement.
	*/
KonqComboAction::KonqComboAction( const QString& text, int accel, const bool autoSized, const int width, const char *signal, const QObject *receiver, const char *member, QObject* parent, const char* name )
    : KAction( text, accel, parent, name )	 {

  m_receiver = receiver;
 	m_signal = signal;
  m_member = member;
  m_combo = 0;
  m_autoSized = autoSized;
  m_width = width;
}

int KonqComboAction::plug( QWidget *w, int index ) {
  qDebug("KonqComboAction: plug()");

  KToolBar *toolbar = (KToolBar*)w;
  int id = getToolButtonID();

  toolbar->insertCombo( m_items, id, true, m_signal, m_receiver, m_member, true, QString::null, m_width, index );
  m_combo = (KComboBox*)toolbar->getCombo( id );
  ASSERT(m_combo);

  QWhatsThis::add( toolbar->getCombo(id), whatsThis() );
  addContainer( toolbar, id );

  connect( toolbar, SIGNAL( destroyed() ), this, SLOT( slotDestroyed() ) );
  toolbar->setItemAutoSized( id, m_autoSized );

  emit plugged();

  return containerCount() - 1;
}

void KonqComboAction::unplug( QWidget *w ) {
//  if ( !w->inherits( "KToolBar" ) )
//    return;

  KToolBar *toolbar = (KToolBar *)w;

  int idx = findContainer( w );

  toolbar->removeItem( menuId( idx ) );

  removeContainer( idx );
}

/**********************************************************************************************/

CLineEditAction::CLineEditAction( const QString& text, int accel, const bool autoSized,const int width,  const char *signal, const QObject *receiver, const char *member, QObject* parent, const char* name )
    : KAction( text, accel, parent, name )	 {

  m_receiver = receiver;
 	m_signal = signal;
  m_member = member;
  m_autoSized = autoSized;
  m_width = width;
}

int CLineEditAction::plug( QWidget *w, int index ) {
  qDebug("CLineEditAction::plug( QWidget *w, int index )");
  KToolBar *toolbar = (KToolBar *)w;
  if (!w)
  	return -1	;

  int id = KAction::getToolButtonID();

  toolbar->insertLined( "", id, m_signal, m_receiver, m_member, true, QString::null, m_width, index );
  m_lineEdit = toolbar->getLined( id );
  ASSERT(m_lineEdit);

  addContainer( toolbar, id );
  QWhatsThis::add( toolbar->getLined(id), whatsThis());
  connect( toolbar, SIGNAL( destroyed() ), this, SLOT( slotDestroyed() ) );

  toolbar->setItemAutoSized( id, m_autoSized );
  emit plugged();

  return containerCount() - 1;
}

void CLineEditAction::unplug( QWidget *w ) {
  KToolBar *toolbar = (KToolBar *)w;
  int idx = findContainer( w );
  toolbar->removeItem( menuId( idx ) );
  removeContainer( idx );
}

//--------------------------

KonqLabelAction::KonqLabelAction( const QString &text, QObject *parent, const char *name )
: KAction( text, 0, parent, name )
{
}

int KonqLabelAction::plug( QWidget *widget, int index )
{
  //do not call the previous implementation here

  if ( widget->inherits( "KToolBar" ) )
  {
    KToolBar *toolbar = (KToolBar *)widget;

    int id = KAction::getToolButtonID();

    QLabel *label = new QLabel( plainText(), widget );
    label->adjustSize();
    toolbar->insertWidget( id, label->width(), label, index );

    addContainer( toolbar, id );
    QWhatsThis::add( toolbar->getWidget(id), whatsThis());
    connect( toolbar, SIGNAL( destroyed() ), this, SLOT( slotDestroyed() ) );

    return containerCount() - 1;
  }

  return -1;
}

void KonqLabelAction::unplug( QWidget *widget )
{
  if ( widget->inherits( "KToolBar" ) )
  {
    KToolBar *bar = (KToolBar *)widget;

    int idx = findContainer( bar );

    if ( idx != -1 )
    {
      bar->removeItem( menuId( idx ) );
      removeContainer( idx );
    }

    return;
  }
}

//-----
/** This class was originally created by Simon Hausmann <hausmann@kde.org>.
	* The copyright of KonqComboAction and KonqLabelAction is owned by him!
	*	We included it and added the QT exception with his agreement.
	*/
/*CColorComboAction::CColorComboAction( QColor color, const bool autoSized, const int width, const char *signal, const QObject *receiver, const char *member, QObject* parent, const char* name )
    : KAction( "nothing", 0, parent, name ) {
  qDebug("constructor of CColorComboAction");
  m_receiver = receiver;
 	m_signal = signal;
  m_member = member;
  m_combo = 0;
  m_color = color;
  m_autoSized = autoSized;
  m_width = width;
}

int CColorComboAction::plug( QWidget *w, int index ) {
  qDebug("CColorComboAction: plug()");

  KToolBar *toolbar = (KToolBar*)w;
  ASSERT(toolbar);
  if (!toolbar) {
		qDebug("toolBar is zero");
		return -1;
  }
  int id = getToolButtonID();

	m_combo = new KColorCombo(w,"CColorComboAction");
	toolbar->insertWidget( id, m_width, m_combo, index );
	connect( m_combo, m_signal, m_receiver, m_member);
  ASSERT(m_combo);
  m_combo->setColor( m_color );

  QWhatsThis::add( m_combo, whatsThis() );
  addContainer( toolbar, id );

  connect( toolbar, SIGNAL( destroyed() ), this, SLOT( slotDestroyed() ) );
  toolbar->setItemAutoSized( id, m_autoSized );

  qDebug("emi plugged");
  emit plugged();

  qDebug("plug finisheed");
  return containerCount() - 1;
}

void CColorComboAction::unplug( QWidget *w ) {
  KToolBar *toolbar = (KToolBar *)w;

  int idx = findContainer( w );

  toolbar->removeItem( menuId( idx ) );

  removeContainer( idx );
}
*/