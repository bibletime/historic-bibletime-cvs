/***************************************************************************
                          cmodulechooserbutton.h  -  description
                             -------------------
    begin                : Sat Mar 17 2001
    copyright            : (C) 2001 by The BibleTime team
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

#ifndef CMODULECHOOSERBUTTON_H
#define CMODULECHOOSERBUTTON_H

#include "../../backend/sword_backend/cswordmoduleinfo.h"
#include "../../structdef.h"

#include <qwidget.h>
#include <qtoolbutton.h>

class KPopupMenu;

/**
  *@author The BibleTime team
  */

class CModuleChooserButton : public QToolButton  {
   Q_OBJECT
public: 
	CModuleChooserButton(CImportantClasses* importantClasses, CSwordModuleInfo* useModule,CSwordModuleInfo::type type, const int id, QWidget *parent = 0, const char *name = 0 );
	~CModuleChooserButton();
  CSwordModuleInfo* getModule();
  /**
  * Returns the id used for this button.
  */
  int getId() const;
	
private:
	bool m_hasModule;
	CSwordModuleInfo::type m_moduleType;
	CSwordModuleInfo* m_module;
	int m_id;
	CImportantClasses* m_important;
	KPopupMenu* m_popup;
  /**
  * Returns the icon used for the current status.
  */
  QPixmap getIcon();		

private slots:
	void moduleChosen(int ID );

signals:
	void sigRemoveButton(const int ID);
	void sigAddButton();
signals: // Signals
  /** No descriptions */
  void sigChanged();
};
#endif
