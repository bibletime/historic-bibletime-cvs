/***************************************************************************
                          cmodulepresenter.h  -  description
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

#ifndef CMODULEPRESENTER_H
#define CMODULEPRESENTER_H

//own includes
#include "../../backend/cmoduleinfo.h"
#include "../../backend/sword_backend/cswordmoduleinfo.h"
#include "../../structdef.h"

//KDE includes
#include <kmainwindow.h>

class KConfig;

/**
	* A modulepresenter displays text or the content of
	* the handled module and offers options to work with it.
  * @author The BibleTime Team
  */
class CModulePresenter : public KMainWindow  {
   Q_OBJECT

public:
	/**
		* The constructor
		*/
	CModulePresenter(CImportantClasses* importantClasses, QWidget *parent=0, const char *name=0, CModuleInfo* module =0, WFlags f = WDestructiveClose  );
	/**
		* The destructor
		*/
	~CModulePresenter();

  /**
  	* This enum contains the features available for presenters.
  	*/
	enum features {
  	autoScrolling = 0x000000001,
	 	canCut = 0x000000002,
	 	canCopy = 0x000000004,
	 	canPaste = 0x000000008,
	 	strongNumbers = 0x000000010,
	 	footnotes = 0x000000020,
	 	language = 0x000000040,
	 	textColor = 0x000000080,
	 	highlightedVerseColor = 0x000000100,
	 	verseNumberColor = 0x000000200,
	 	backgroundColor = 0x000000400,
	 	font = 0x000000800	
  };
		
  /**
  	* Get a pointer to the module we present
  	*/
  virtual CModuleInfo *getModuleInfo() { return m_swordModule; };
  /**
  	* Refresh the content of this module
  	*/
  virtual void refresh(const int useFeatures);
  /**
  	* Reimplementation from KTMainWindow.
  	*/
  virtual QString	caption() const;
  /**
  	* Cuts cuttable text int
  	*/
  virtual void cut();
  /**
  	* Pastes into the presenters. Normally into a edit widget.
  	*/
  virtual void paste();
  /**
  	* Copy selected text of the resenter into the clipboard.
  	*/
  virtual void copy();
  /**
  	* Returns the features currently activated!
  	*/
  virtual int getAvailableFeatures() const;
 	/**
		* Changes the currently activated features!
		*/
 	virtual void setAvailableFeatures( const int );
 	
public slots:
	/**
		*
		*/
 	virtual void setCaption(const QString&);
  /**
  	* Returns true if synchroniziation is enabled for this presenter.
  	*/
  virtual bool synchronizeEnabled();

protected:
 	void saveSettings();
 	void readSettings();
 	int availableFeatures;
	bool	m_doSynchronize;
 	QString Anchor;
	CSwordModuleInfo	*m_swordModule;
	CImportantClasses* m_important;
 	KConfig* config;
 		 	
public slots: // Public slots
};

#endif
