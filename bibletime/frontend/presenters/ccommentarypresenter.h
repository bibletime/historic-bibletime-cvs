/***************************************************************************
                          ccommentarypresenter.h  -  description
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

#ifndef CCOMMENTARYPRESENTER_H
#define CCOMMENTARYPRESENTER_H

#include "cmodulepresenter.h"

#include <qguardedptr.h>

class KComboBox;
class KonqLabelAction;
class CMDIArea;
class CHTMLWidget;
class KAction;
class KToggleAction;
class	KonqComboAction;
class CLineEditAction;
class CColorComboAction;
class CKeyChooser;
class BibleTime;
class CKey;
class CSwordVerseKey;

/**
* The presenter for Commentary modules
* @author The BibleTime Team
*/
class CCommentaryPresenter : public CModulePresenter  {
	Q_OBJECT
	
public:
	/**
	*	
	*/
	CCommentaryPresenter( CImportantClasses* importantClasses, QWidget* parent, const char* name = 0, CModuleInfo* module =0 );
	/**
	*
	*/
	~CCommentaryPresenter();
  /**
  * Reimplementation
  */
  void copy();
  /**
  * Reimplementation
  */
  void paste();
  /**
  * Reimplementation
  */
  void cut();
  /**
  * Returns the caption for this presenter window.
  */
  QString caption() const;	
	CKeyChooser* m_keyChooser;	
	
public slots:
  /**	
  *
  */
  void lookup(CKey*);

protected:
	friend class BibleTime;
  /**
  *
  */
  void refresh(const int useFeatures);
  /**
  *
  */
  void initView();
  /**
  *
  */
  void initConnections();
  /**
  * Initialize the action objects.
  */
  void initActions();
 	

protected slots:
  /**
  *
  */
  void slotReferenceClicked( const QString& );
  /**
  *
  */
  void editComment();
  /**
  * Deletes the current entry in the module.
  */
  void deleteText();
  /**
  * Saves the document in the module.
  */
  void saveText(const QString);
  /**
  * This function appens the displayed verse to the printer queue.
  */
  void slotPrintCurrentVerse();
  /**
  * Is called before the popup menu is shown
  */
  void slotPopupAboutToShow();

private:
	KPopupMenu* m_popup;
	CSwordVerseKey*	m_key; 	
 	CHTMLWidget* html_widget;
 	KToolBar* m_mainToolbar;	
 	KToolBar* m_editToolbar;	 	
  bool isEditMode;
  KToggleAction* presenterEdit_action;	
  QString currentAnchor;		
};

#endif
