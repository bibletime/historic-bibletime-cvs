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
#include <qlist.h>
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
	enum mdiOption {
		autoTile,
		autoCascade,
		Nothing
	};	
	CMDIArea(QWidget *parent=0, const char *name = 0 );
	~CMDIArea();
  /**
  */
  void readSettings();
  /**
  */
  void saveSettings();
  /**
  * Enable / disable autoCascading
  */
  void setGUIOption( mdiOption );
  /**
  * This works around a problem/limitation in QWorkspace. QWorkspace sets every time the 	
  * application caption on its on way. This confuses BibleTime - wrong captions are generated.
	* This function returns the right caption (using the MDI child).
	*/
  const QString currentApplicationCaption() const;

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
  * Emits the signal to create a new display window in the MDI area.
  */
  void emitCreateDisplayWindow( ListCSwordModuleInfo modules, const QString keyName );

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
	mdiOption guiOption;
	bool m_childEvent;
  bool m_deleting;
	QString m_appCaption;
};
#endif
