/***************************************************************************
                          CLexiconPresenter.h  -  description
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

#ifndef CLEXIKONPRESENTER_H
#define CLEXIKONPRESENTER_H

//BibleTime includes
#include "cmodulepresenter.h"

//Qt includes

class CHTMLWidget;
class KLineEdit;
class KComboBox;
class KonqComboAction;
class KToggleAction;
class CLineEditAction;
class CKeyChooser;
class CSwordLDKey;
class CKey;
class BibleTime;

/** The presenter for Lexikon modules
  * @author The BibleTime Team
  */
class CLexiconPresenter : public CModulePresenter  {
	Q_OBJECT
	
public:
	CLexiconPresenter(CImportantClasses* importantClasses ,QWidget* parent = 0, const char* name = 0,CModuleInfo* module = 0) ;
	~CLexiconPresenter();
  /**
  	*
  	*/
  void refresh(const int useFeatures);
  /**
  	* Reimplementation
  	*/
  void copy();
  /**
  	* Returns the caption for this presenter window.
  	*/
  virtual QString caption() const;
	CKeyChooser* m_keyChooser;	
	
public slots:
	/**
		*
		*/
  void lookup(CKey*);
  /**
  	* Looks up the word given by the first parameter.
  	*/
  void lookupWord( const QString& );
  /**
  	* No descriptions
  	*/
  void synchronizeToggled();	

protected:
	friend class BibleTime;
  /**
  	* Initializes connections between SIGNALs and SLOTs
  	*/
  void initConnections();
  /**
  	* Initializes the view
  	*/
  void initView();
  /**
  	 No descriptions
  	*/
  void initActions();

protected slots:
  /**
  	* This function appends the current entry of the lexicon to the print queue.
  	*/
  void slotPrintCurrentEntry();
  /** This slot is called before the popup is shown. */
  void slotPopupAboutToShow();

private:
	CHTMLWidget* html_widget;
	KToolBar* m_mainToolbar;
	CSwordLDKey* m_key;
	KToggleAction* synchronize_action;
	KPopupMenu* m_anchorPopup;
	KPopupMenu* m_popup;	
};

#endif
