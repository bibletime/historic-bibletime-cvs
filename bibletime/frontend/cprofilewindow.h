/***************************************************************************
                          cprofilewindow.h  -  description
                             -------------------
    begin                : Sun Jul 29 2001
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

#ifndef CPROFILEWINDOW_H
#define CPROFILEWINDOW_H

#include "backend/cswordmoduleinfo.h"
#include "backend/cswordbackend.h"

//Qt includes
#include <qstring.h>
#include <qrect.h>
#include <qstringlist.h>
#include <qlist.h>

/** Contains the settings for one window saved in the profile.
  * @author The BibleTime team
  */
class CProfileWindow {
public:
  struct ScrollbarPos {
		int horizontal; 	//the position of the horizontal scrollbar
		int vertical;	//the position of the vertical scrollbar
  };

	CProfileWindow(CSwordModuleInfo::ModuleType type = CSwordModuleInfo::Unknown);
	~CProfileWindow();
  /**
  * Sets the modules.
  */
  void setModules( QStringList modules );
  /**
  * Returns the type of the managed window (bible window, commentary window or lexicon window).
  */
  const CSwordModuleInfo::ModuleType type();
  /**
  * Sets the type of the used modules.
  */
  void setType(const CSwordModuleInfo::ModuleType type);
  /**
  * Sets the size of the window.
  */
  void setGeometry( const QRect s );
  /**
  * Returns the size of the window including the x,y coordinates.
  */
  const QRect geometry();
  /**
  * Returns a list of module names which are chosen in the managed window profile.
  */
  const QStringList modules();
  /**
  * Set the key used in the modules.
  */
  void setKey( const QString& );
  /**
  * Returns the current key set in the modules.
  */
  const QString& key();
  /**
  * Sets the current position of the scrollbars.
  */
  void setScrollbarPositions(const int x, const int y);
  /**
  * Sets the windows maximized state to true or false.
  */
  void setMaximized( const bool maximized );
  /**
  * Sets the window specific settings.
  */
  void setWindowSettings( const int settings );
  /**
  * Returns an integer with the window specific settings
  */
  const int windowSettings();
  /**
  * Returns true if the window is maximized.
  */
  const bool maximized();
  /**
  * Returns tghe position of the scrollbars
  */
  const CProfileWindow::ScrollbarPos scrollbarPositions();
  /**
  * Returns whether this profile window represents a write window.
  */
  const int writeWindowType() const;
  /**
  * Tells this profile window to represent a write window.
  */
  void setWriteWindowType( const int writeWindowType );

private:
	CSwordModuleInfo::ModuleType m_type;
	QRect m_windowGeometry;
	QStringList m_moduleList;
	QString m_key;
	ScrollbarPos m_scrollbarPos;
	bool m_maximized;
	int m_windowSettings;
  int m_writeWindowType;
};

#endif

