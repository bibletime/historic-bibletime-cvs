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

//BibleTime includes
#include "util/cpointers.h"
#include "backend/cswordmoduleinfo.h"

//Qt includes
#include <qworkspace.h>
#include <qptrlist.h>
#include <qstrlist.h>
#include <qevent.h>

class KPopupMenu;
class CSwordModuleInfo;
class CSwordKey;

/** The MDI widget we use in BibleTime.
	* Enhances QWorkspace.
  * @author The BibleTime Team
  */
class CMDIArea : public QWorkspace, public CPointers  {
   Q_OBJECT

public:
	/**
	* The options you can set for this widget.
	*/
	enum MDIOption {
		autoTile,
		autoCascade,
		Nothing
	};	
	CMDIArea(QWidget *parent, const char *name = 0 );
  /**
  */
  void readSettings();
  /**
  */
  void saveSettings();
  /**
  * Enable / disable autoCascading
  */
  void setGUIOption( const MDIOption& newOption );
  /**
  * This works around a problem/limitation in QWorkspace. QWorkspace sets every time the 	
  * application caption on its on way. This confuses BibleTime - wrong captions are generated.
	* This function returns the right caption (using the MDI child).
	*/
  inline const QString currentApplicationCaption() const;
  void emitWindowCaptionChanged();
  /**
	* Forces an update of the currently chosen window arrangement.
	*/
	void triggerWindowUpdate();

public slots:
  /**
  * Called whan a client window was activated
  */
	void slotClientActivated(QWidget* client);
  /**
  * Deletes all the presenters in the MDI area.
  */
  void deleteAll();
  /** Our own cascade version which, if only one window is left, shows this maximized.  
 	* Also necessary for autoCasacde feature
 	*/
  void myCascade();
  /** Our own cascade version which, if only one window is left, shows this maximized.
 	* Also necessary for autoTile feature
 	*/
  void myTile();
	/** Horizontal tile function
	* This function was taken from Qt's MDI example.
	*/
	void tileHorizontal();
	/**
  * Emits the signal to create a new display window in the MDI area.
  */
  inline void emitCreateDisplayWindow( ListCSwordModuleInfo modules, const QString keyName );

protected: // Protected methods
  /**
  * Used to make use of the fixedGUIOption part.
  */
  virtual void childEvent (QChildEvent * e);
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
  QPtrList<QWidget> usableWindowList();
	bool eventFilter( QObject *o, QEvent *e );
			
signals: // Signals
  /**
  * Emits a signal to set the acption of the toplevel widget.
  */
  void sigSetToplevelCaption(const QString&);
  /**
  * Is emitted when the last presenter was closed.
  */
  void sigLastPresenterClosed();
  void createReadDisplayWindow(ListCSwordModuleInfo modules, const QString& keyName);
	
private:
	MDIOption m_guiOption;
	bool m_childEvent;
  bool m_deleting;
	QString m_appCaption;
};

/** This works around a problem/limitation in QWorkspace. QWorkspace sets every time the  application caption on its on way. This confuses BibleTime - wrong captions are generated. This function returns the right caption (using the MDI child). */
inline const QString CMDIArea::currentApplicationCaption() const {
	return m_appCaption;
}

/** Emits the signal to create a new display window in the MDI area. */
inline void CMDIArea::emitCreateDisplayWindow( ListCSwordModuleInfo modules, const QString keyName ) {
  emit createReadDisplayWindow(modules, keyName);
}


#endif
