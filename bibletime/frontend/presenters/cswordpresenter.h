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

/** Base class for all Sword based presneters
	* The base class for all Sword based presenters.
	*
  * @author The BibleTime team
  */
class CSwordPresenter : public CPresenter  {
   Q_OBJECT
public: 
	enum features {
	 	strongNumbers = 0x000000010, /* Set if Strong's numbers are supported*/
	 	footnotes = 0x000000020, /* Set if footnotes are supported*/
	 	language = 0x000000040, /* Set if different languages are supported (for example in the key chooser)*/
	 	textColor = 0x000000080, /* Set if different text colors are supported*/
	 	highlightedVerseColor = 0x000000100, /* Set if different color for the highlighted verse are supported*/
	 	verseNumberColor = 0x000000200, /* Set if different colors for the versenumber are supported*/
	 	backgroundColor = 0x000000400, /* Set if different background colors are supported*/
	 	font = 0x000000800	/* Set if different fonts are supported*/
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

	/**
	* Default constructor
	*/
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
  virtual void refresh( const int events );
  /**
  * Returns the listof used modules.
  */
  ListCSwordModuleInfo& getModuleList();

public slots: // Public slots
  /**
  * Look up the key given as parameter.
  */
  virtual void lookup(const QString&);
  /**
  * Returns the used keychooser object of this presenter.
  */
  CKeyChooser* getKeyChooser() const;

protected: // Protected methods
  /**
  * Prints the key given as parameter.
  */
  void printKey(CKey*, CKey*, CSwordModuleInfo*);
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
  void lookupWord(int ID);

signals: // Signals
  void lookupInLexicon(const QString& text, const QString& lexicon);

private:
	int m_features;
};

#endif
