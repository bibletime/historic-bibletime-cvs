/***************************************************************************
                          cbiblepresenter.h  -  description
                             -------------------
    begin                : Sun Mar 18 2001
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

#ifndef CBIBLEPRESENTER_H
#define CBIBLEPRESENTER_H

#include <qwidget.h>
#include "cswordpresenter.h"

class CSwordVerseKey;

/**
	* The presenter used for Sword Bibles.
  * @author The BibleTime team
  */
class CBiblePresenter : public CSwordPresenter  {
   Q_OBJECT
public: 
	CBiblePresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent=0, const char *name=0);
	~CBiblePresenter();

protected:
  /**
  * Initializes the view (central widget, toolbars etc) of this presenter
  */
  virtual void initView();
  /**
  * Initializes the Signal / Slot connections
	*/
  void initConnections();

protected slots: // Protected slots
  /**
  * This slot is called when the modules selected in
	* the module chooserbar have changed.
	*/
  void modulesChanged();
  /**
  * Displays the chapter using the aparameter.
  */
  void lookup( CKey* );

private slots: // Private slots
  /**
  * No descriptions
  */
  void referenceClicked(const QString&);
  /**
  ** No descriptions
  */
  void popupAboutToShow();

private:
	CSwordVerseKey* m_key;
};

#endif
