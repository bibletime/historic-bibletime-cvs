/***************************************************************************
                          cplainwritewindow.h  -  description
                             -------------------
    begin                : Fre Okt 11 2002
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

#ifndef CPLAINWRITEWINDOW_H
#define CPLAINWRITEWINDOW_H

//BibleTime includes
#include "cwritewindow.h"

//Qt includes
#include <qwidget.h>


class KAction;
class KToggleAction;

/** The write window class which offers a plain editor for source code editing.
  * @author The BibleTime team
  */
class CPlainWriteWindow : public CWriteWindow  {
   Q_OBJECT
public:
	CPlainWriteWindow( ListCSwordModuleInfo modules, CMDIArea* parent, const char *name = 0 );
	~CPlainWriteWindow();

  /**
  * Store the settings of this window in the given CProfileWindow object.
  */
  virtual void storeProfileSettings( CProfileWindow* profileWindow );
  /**
  * Store the settings of this window in the given profile window.
  */
  virtual void applyProfileSettings( CProfileWindow* profileWindow );

	/**
  * Setups the popup menu of this display widget.
  */
  virtual void setupPopupMenu();
	virtual const bool syncAllowed() const;

protected: // Protected methods
  /**
  * Initialize the state of this widget.
  */
  virtual void initView();
  virtual void initConnections();
  virtual const CDisplayWindow::WriteWindowType writeWindowType() {
    return CDisplayWindow::PlainTextWindow;
  };

	/**
  * Initializes the intern keyboard actions.
  */
  virtual void initKeyboardActions();
  /**
  * Insert the keyboard accelerators of this window into the given KAccel object.
  */
  static void insertKeyboardActions( KAccel* const accel );
//  virtual void setupPopupMenu();

private:
  struct {
    KAction* saveText;
    KAction* deleteEntry;
    KAction* restoreText;
		KToggleAction* syncWindow;
  } m_actions;

protected slots: // Protected slots
  /**
  * Saves the text for the current key. Directly writes the changed text into the module.
  */
  virtual void saveCurrentText( const QString& );
  /**
  * Is called when the current text was changed.
  */
  virtual void textChanged();
  /**
  * Loads the original text from the module.
  */
  virtual void restoreText();
  /**
  * Deletes the module entry and clears the edit widget.
  */
  virtual void deleteEntry();
};

#endif
