/***************************************************************************
                          btsetupwizard.h  -  description
                             -------------------
    begin                : Son Jul 14 2002
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

#ifndef BTSETUPWIZARD_H
#define BTSETUPWIZARD_H

#include <qwidget.h>
#include <kmainwindow.h>

/**The main window of the application. Provides the possibilities to install/remove and download modules.
  *@author The BibleTime team
  */

class BTSetupWizard : public KMainWindow  {
   Q_OBJECT
public: 
	BTSetupWizard(QWidget *parent=0, const char *name=0);
	~BTSetupWizard();
};

#endif
