/***************************************************************************
                          cbiblepresenter.h  -  description
                             -------------------
    begin                : Thu Jan 20 2000
    copyright            : (C) 2000 by The BibleTime Team
    email                : Info@bibletime.de
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

//KDE includes
#include "cmodulepresenter.h"
#include "../../structdef.h"

//Qt includes
#include <qguardedptr.h>

class KComboBox;
class CMDIArea;
class CHTMLWidget;
class KAction;
class KToggleAction;
class	KonqComboAction;

class CKeyChooser;
class CKey;
class CSwordVerseKey;
class CModuleChooserBar;

class BibleTime;

/** To present a bible module
  * @author The BibleTime Team
  */
class CBiblePresenter : public CModulePresenter  {
	friend class CMDIArea;
	Q_OBJECT

public:
	CBiblePresenter(CImportantClasses* importantClasses, QWidget* parent, const char* name = 0, CModuleInfo* module = 0 );
	virtual ~CBiblePresenter();
	/**
	*
	*/
  virtual void refresh(const int useFeatures);
  /**
  * Reimplementation
  */
  void copy();
  /**
  * Returns the caption for this presenter window.
  */
  virtual QString caption();
	CKeyChooser* m_keyChooser;
	
public slots: // Public slots
  /**
  * Is called after a module was added/deleted from the popup menu.
  */
  void slotModuleChosen(int);
  /**
  *
  */
	virtual void lookup(CKey* key);

protected:
	friend class BibleTime;	
  /**
  *
  */
  void readSettings();
  /**
  * Initialize the view
  */
  void initView();
  /**
  * Initializes SLOTs and SIGNALs
  */
  void initConnections();
	

protected slots: // Protected slots
  /**
	*
  */
  void slotReferenceClicked( const QString& );
  /**
  * Prints the highlighted verse.
  */
  void slotPrintHighlightedVerse();
  /**
  * This slot is called, when the HTML widget send a word,
	* on which the user clicked.
	*/
  void slotWordClicked( const QString& );
  /**
  * Is called before the popup is shown.
  */
  void slotPopupAboutToShow();

private:
	KPopupMenu* m_anchorPopup;
	KPopupMenu* m_popup;
//	KPopupMenu* m_moduleChooserPopup;
	CModuleChooserBar* m_moduleChooserBar;
 	KToolBar* m_mainToolbar;	
  QString currentAnchor;
  CSwordVerseKey* m_key;
  CHTMLWidget* html_widget;

  QList<CSwordModuleInfo> m_moduleList;
private slots: // Private slots
  /** called when the module chooser was modified */
  void slotModulesChanged();
};

#endif

