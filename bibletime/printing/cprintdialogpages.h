/***************************************************************************
                          cstylelistpage.h  -  description
                             -------------------
    begin                : Fri Feb 8 2002
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

#ifndef CSTYLELISTPAGE_H
#define CSTYLELISTPAGE_H

//Qt includes
#include <qwidget.h>
#include <qtoolbutton.h>

//KDE includes
#include <kdeprint/kprintdialogpage.h>


class QComboBox;
class QSpinBox;
class QListViewItem;

class CPrintItemList;
class CStyleList;
class CPrinter;

/**
  *@author The BibleTime team
  */

class CStyleListPage : public KPrintDialogPage  {
   Q_OBJECT
public: 
	CStyleListPage(CPrinter* printer, QWidget *parent=0, const char *name=0);
	~CStyleListPage();
	virtual void  getOptions (QMap<QString,QString>& opts, bool incldef = false);
	virtual void  setOptions (const QMap<QString,QString>& opts);
	virtual bool  isValid (QString&);

private:
	CPrinter* m_printer;
  QSpinBox *m_marginSpin[4];
  CStyleList*	m_styleList;		
  QToolButton* m_deleteStyleButton;		
  QToolButton* m_newStyleButton;		
  QToolButton* m_editStyleButton;
  /**
  * Is called when the style selection changed in the syle list box.
  */

private slots:
  void currentStyleChanged(QListViewItem*);
};

// --------------------------

/**
  *@author The BibleTime team
  */

class CPrintItemListPage : public KPrintDialogPage  {
   Q_OBJECT
public:
	CPrintItemListPage(CPrinter* printer, QWidget *parent=0, const char *name=0);
	~CPrintItemListPage();
	virtual void  getOptions (QMap<QString,QString>& opts, bool incldef = false);
	virtual void  setOptions (const QMap<QString,QString>& opts);
	virtual bool  isValid (QString&);

private:
	CPrinter* m_printer;
	CPrintItemList* m_printItemList;
	QComboBox* m_styleComboBox;
	QToolButton* m_moveUpButton;
	QToolButton* m_moveDownButton;
	QToolButton* m_deleteButton;		

private slots:
  /**
 	* Applies the given styles to the print item list.
 	*/
  void slotListApplyStyle(const QString&);
protected slots: // Protected slots
  /**
  * Is called when the CPrinter object sends a signal
  * that the styles have changed.
  */
  void stylesChanged();
};


#endif
