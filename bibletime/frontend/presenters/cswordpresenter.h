/***************************************************************************
                          cswordpresenter.h  -  description
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

#ifndef CSWORDPRESENTER_H
#define CSWORDPRESENTER_H

//BibleTime includes
#include "cpresenter.h"
#include "../../backend/sword_backend/cswordmoduleinfo.h"
#include "../../structdef.h"


//Qt includes
#include <qwidget.h>


class KPopupMenu;
class KToolBar;

class CKeyChooser;
class CHTMLWidget;
class CModuleChooserBar;
class CKey;

/**
	* The base class for all Sword based presenters.
  * @author The BibleTime team
  */
class CSwordPresenter : public CPresenter  {
   Q_OBJECT
public: 
	enum features {
	 	strongNumbers = 0x000000010,
	 	footnotes = 0x000000020,
	 	language = 0x000000040,
	 	textColor = 0x000000080,
	 	highlightedVerseColor = 0x000000100,
	 	verseNumberColor = 0x000000200,
	 	backgroundColor = 0x000000400,
	 	font = 0x000000800	
  };	
	enum events {
  	strongNumbersChanged = strongNumbers,
  	footnotesChanged = footnotes,
  	languageChanged = language,
  	textColorChanged = textColor,
  	highlightedVerseColorChanged = highlightedVerseColor,
  	verseNumberColorChanged = verseNumberColor,
  	backgroundChanged = backgroundColor,
  	fontChanged = font  	
  };

	CSwordPresenter(ListCSwordModuleInfo useModules, CImportantClasses* importantClasses, QWidget *parent=0, const char *name=0);
	/**
	* Destructor of this sword üresenter
	*/
	~CSwordPresenter();
  /**
  * Returns the features used by this presenter.
  */
  int getFeatures();
  /**
  * Refreshes the presenter depending on the events given as parameter.
  */
  virtual void refresh( const int events);

public slots: // Public slots
  /**
  * Look up the key given as parameter.
  */
  virtual void lookup(const QString&);
  /**
  * Returns the listof used modules.
  */
  ListCSwordModuleInfo& getModuleList();

protected: // Protected methods
  /**
  * Prints the key given as parameter.
  */
  void printKey(CKey*);
  /**
  * Refreshes the supported features.
  */
  void refreshFeatures();

	KToolBar* m_mainToolBar;
	KPopupMenu* m_popup;
	QPopupMenu* m_lexiconPopup;
	CKeyChooser* m_keyChooser;
	CHTMLWidget* m_htmlWidget;	
  CModuleChooserBar* m_moduleChooserBar;
  ListCSwordModuleInfo m_moduleList;
 	CImportantClasses* m_important;

 	QString m_highlightedVerse;
 	  	
protected slots: // Protected slots
  /**
  * No descriptions
  */
  void lookupWord(int ID);
  /**
  * No descriptions
  */
  void printHighlightedVerse();

signals: // Signals
  /**
  * No descriptions
  */
  void createSwordPresenter(CSwordModuleInfo*, const QString);

private:
	int m_features;
};

#endif
