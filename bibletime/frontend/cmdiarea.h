/***************************************************************************
                          cmdiarea.h  -  description
                             -------------------
    begin                : Wed Jan 19 2000
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

#ifndef CMDIAREA_H
#define CMDIAREA_H

#define private protected
#include "qworkspace.h"
#undef private

#include <qlist.h>
#include <qstrlist.h>
#include <qevent.h>

class KPopupMenu;
class KConfig;
class CBackEnd;
class CImportantClasses;
class CKey;
class CSwordModuleInfo;

/** The MDI widget we use in BibleTime.
	* Enhances QWorkspace.
  * @author The BibleTime Team
  */
class CMDIArea : public QWorkspace  {
   Q_OBJECT

public:
	/**
	* The options you can set for this widget.
	*/
	enum mdiOption {
		autoTile,
		autoCascade,
		Nothing
	};
	
	CMDIArea(CImportantClasses* importantClasses, QWidget *parent=0, const char *name = 0 );
	~CMDIArea();
  /**  */
  void readSettings();
  /**  */
  void saveSettings();
  /**
   * Enable / disable autoCascading
   */
  void setGUIOption( mdiOption );

public slots:
  /**
   * Called whan a client window was activated
   */
	void slotClientActivated(QWidget* client);
  /**
   * Deletes all the presenters in the MDI area.
   */
  void deleteAll();
  /**
  	*
  	*/
  void cascade();
  /**
  	*
  	*/
  void tile();
  /**
  * Sync the commentaries to the given key.
  */
  void syncCommentaries(CKey* syncKey);
  /** Look up the text in the module. If the module has already a display window of it opne use it, otherwise create a new one. */
  void lookupInLexicon(const QString& text, const QString& module);

protected: // Protected methods
  /**
   * Used to make use of the fixedGUIOption part.
   */
  virtual void childEvent ( QChildEvent * e );
  /**
   * Reimplementation
   */
  virtual void resizeEvent(QResizeEvent* e);
  /**
   * Initializes the connectiosn to SIGNALS
   */
  void initConnections();
  /**
   * Initializes the view of the MDI area
   */
  void initView();
		
signals: // Signals
  /**
   * Emits a signal to set the acption of the toplevel widget.
   */
  void sigSetToplevelCaption(const QString&);
  /**
   * Is emitted when the last presenter was closed.
   */
  void sigLastPresenterClosed();
  void createNewSwordPresenter(CSwordModuleInfo*, const QString);

private:
	mdiOption guiOption;
	KConfig* config;
	CImportantClasses*	m_important;
	bool m_childEvent;
};
#endif
