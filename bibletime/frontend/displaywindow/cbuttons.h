/***************************************************************************
                          cbuttons.h  -  description
                             -------------------
    begin                : Sam Jul 13 2002
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

#ifndef CBUTTONS_H
#define CBUTTONS_H

//BibleTime includes
#include "backend/cswordbackend.h"
#include "backend/cswordmoduleinfo.h"

//QT includes
#include <qdict.h>

//KDE includes
#include <ktoolbarbutton.h>

class KPopupMenu;

/** This class manages the transliteration of the selected modules.
  * @author The BibleTime team
  */
class CTransliterationButton : public KToolBarButton  {
   Q_OBJECT
public: 
	CTransliterationButton(QWidget *parent=0, const char *name=0);
	~CTransliterationButton();
};

/** This class manages the display options of the selected modules.
  * @author The BibleTime team
  */
class CDisplaySettingsButton : public KToolBarButton  {
   Q_OBJECT
public:
	CDisplaySettingsButton(CSwordBackend::DisplayOptionsBool *displaySettings, CSwordBackend::FilterOptionsBool *settings, const ListCSwordModuleInfo& useModules, QWidget *parent=0, const char *name=0);
	void reset(const ListCSwordModuleInfo& useModules);
  /**
  * Sets the item at position pos to the satet given as 2nd paramter.
  */
  void setItemStatus( const int pos, const bool checked );
  /**
  * Returns the number of usable menu items in the setttings menu.
  */
  const int menuItemCount();
  /**
  * Returns the status of the item at position "index"
  */
  const bool itemStatus( const int index );
  /**
  * Sets the status to changed. The signal changed will be emitted.
  */
  void setChanged();

signals:
	void sigChanged(void);

protected slots:
	void optionToggled(int ID);

protected:
	CSwordBackend::FilterOptionsBool*  m_moduleSettings;
	CSwordBackend::DisplayOptionsBool* m_displaySettings;
	CSwordBackend::FilterOptionsBool m_available;
	ListCSwordModuleInfo m_modules;

	QDict<bool> m_dict;

	KPopupMenu* m_popup;

  int populateMenu();
	bool isOptionAvailable( const CSwordBackend::FilterOptions option);
  int addMenuEntry( const QString name, const bool* option, const bool available);
};

#endif
