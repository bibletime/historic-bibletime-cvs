/***************************************************************************
                          ccommentarypresenter.h  -  description
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

#ifndef CCOMMENTARYPRESENTER_H
#define CCOMMENTARYPRESENTER_H

#include <qwidget.h>
#include "cswordpresenter.h"

class CSwordVerseKey;

class KToggleAction;
class KToolBar;

/** The presenter used for Sword commentaries.
  * @author The BibleTime team
  */
class CCommentaryPresenter : public CSwordPresenter  {
   Q_OBJECT

public:
	/**
	* Default constructor
	*/
	CCommentaryPresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses,QWidget *parent=0, const char *name=0);
	/**
	* Default destructor
	*/	
	~CCommentaryPresenter();
  /**
  * Synchronizes to the given key if sync is enabled.
  */
  void synchronize( CKey* syncKey );
  /**
  * Reimplementation.
  */
  virtual const QString caption() const;

	
public slots: // Public slots
  /**
  * Saves the given text in the module.
  */
  void saveText(const QString);
  /**
  * Reimplementation.
  */
  virtual void lookup(const QString&);
  /**
  * No descriptions
  */
  virtual void refresh( const int events);

protected slots: // Protected slots
  /**
  * renders text and set it to the HTML widget
  */
  void lookup(CKey*);
  /**
  * Is called when the selected modules changed.
  */
  void modulesChanged();
  /**
  * Deletes the displayed and edited text.
  */
  void deleteText();
	/**
	*
	*/
	void editComment();
	/**
	* Print the current item displayed in the presenter.
	*/
	void printHighlightedVerse();

private: // Private methods
  /**
  * Initializes the connections
  */
  void initConnections();
  /**
  * Initializes our view.
  */
  void initView();
	KToggleAction *presenterEdit_action;
	KToggleAction *presenterSync_action;
  CSwordVerseKey* m_key;
	KToolBar* m_editToolBar;
		
private slots: // Private slots
  /**
  * No descriptions
  */
  void popupAboutToShow();
  /**
  * The sync button was pressed for synchronization.
  */
  void syncToggled();
};

#endif
