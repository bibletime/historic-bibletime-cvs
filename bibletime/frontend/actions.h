/* This file is part of the KDE project
   Copyright (C) 1999 Simon Hausmann <hausmann@kde.org>

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.
*/

#ifndef ACTIONS_H
#define ACTIONS_H

#include <qguardedptr.h>

#include <kaction.h>
#include <qcombobox.h>
#include <kcolorcombo.h>
#include <qlist.h>
#include <qcolor.h>

class KComboBox;
class QLabel;
class KLineEdit;

/** This class was originally created by Simon Hausmann <hausmann@kde.org>.
	* The copyright of KonqComboAction and KonqLabelAction is owned by him!
	*	We included it and added the QT exception with his agreement.
	*/
class KonqComboAction : public KAction
{
  Q_OBJECT
public:
    KonqComboAction( const QString& text, int accel, const bool autoSized, const int width, const char *signal, const QObject *receiver, const char *member, QObject* parent, const char* name );
    virtual int plug( QWidget *w, int index = -1 );
    virtual void unplug( QWidget *w );
    QGuardedPtr<KComboBox> combo() { ASSERT(m_combo); return m_combo; }

signals:
    void plugged();

private:
    QGuardedPtr<KComboBox> m_combo;
    QStringList m_items;
    const QObject *m_receiver;
    const char *m_member;
    const char *m_signal;
    bool m_autoSized;
    int m_width;
};

class KonqLabelAction : public KAction
{
  Q_OBJECT
public:
  KonqLabelAction( const QString &text, QObject *parent = 0, const char *name = 0 );

  virtual int plug( QWidget *widget, int index = -1 );
  virtual void unplug( QWidget *widget );
};

// ***************************************************************************
/** This class was created by Joachim Ansorg.
	* It's not part of konqueror.
	*/
class CLineEditAction : public KAction
{
  Q_OBJECT
public:
    CLineEditAction( const QString& text, int accel, const bool autoSized, const int width, const char *signal, const QObject *receiver, const char *member, QObject* parent, const char* name );

    virtual int plug( QWidget *w, int index = -1 );

    virtual void unplug( QWidget *w );

    QGuardedPtr<KLineEdit> lineEdit() { ASSERT(m_lineEdit); return m_lineEdit; }

signals:
    void plugged();

private:
    QGuardedPtr<KLineEdit> m_lineEdit;
    const QObject *m_receiver;
    const char *m_member;
    const char *m_signal;
    bool m_autoSized;
    int m_width;
};

#endif
