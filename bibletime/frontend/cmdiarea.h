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

#include "../structdef.h"

class KPopupMenu;
class KConfig;
class CBackEnd;
class CModulePresenter;

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
	enum mdiOption { autoTile, autoCascade, Nothing };	
	
	CMDIArea(CImportantClasses* importantClasses, QWidget *parent=0, const char *name = 0 );
	virtual ~CMDIArea();
  /**  */
  virtual void readSettings();
  /**  */
  virtual void saveSettings();
  /**
   * Enable / disable autoCascading
   */
  virtual void setGUIOption( mdiOption );

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
  	* Scrolls all presenters which are able
		* to do parallel scrolling to the given position.
		*/
//  void scrollPresentersTo(const QString& book, const int chapter, const int verse, CModulePresenter* caller);

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
  	* Reimplementation. Used to popup the context menu.
  	*/
//  virtual void mousePressEvent(QMouseEvent *e);
  /**
   * Initializes the connectiosn to SIGNALS
   */
  virtual void initConnections();
  /**
   * Initializes the view of the MDI area
   */
  virtual void initView();

	mdiOption guiOption;
	KConfig* config;
  /**
   * The backend of BibleTime
   */
	CImportantClasses*	m_important;
	bool m_childEvent;
		
signals: // Signals
  /**
   * Emits a signal to set the acption of the toplevel widget.
   */
  void sigSetToplevelCaption(const QString&);
  /**
   * Is emitted when the last presenter was closed.
   */
  void sigLastPresenterClosed();
};
#endif
