/***************************************************************************
                          chtmlwidget.h  -  description
                             -------------------
    begin                : Sun Feb 27 2000
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

#ifndef CHTMLWIDGET_H
#define CHTMLWIDGET_H

//BibleTime includes
#include "../structdef.h"
#define private public
#include "thirdparty/qt3stuff/qtextedit.h"
#undef private

//Qt includes
#include <qwidget.h>
#include <qguardedptr.h>

class CHTMLDialog;
class CBiblePresenter;
class KConfig;
class KPopupMenu;
class KAction;
class KToggleAction;
class KFontSizeAction;
class KToolBar;
class QToolButton;
class KFontAction;
class QComboBox;
class KColorButton;

/** This widget enhances the KHTMLWidget to support BibleTime's colour
	* management and to support verse references and the like.
  * @author The BibleTime Team
  */
class CHTMLWidget : public QTextEdit {
   Q_OBJECT
   friend class CHTMLDialog;

public:
	CHTMLWidget(CImportantClasses* importantClasses, QWidget *parent=0, const char *name=0);
	virtual ~CHTMLWidget();
  /**
 	* Reinitialize the colors, fonts etc.
 	*/
  virtual void refresh();
  /**
 	* Loads a HTML file in the widget.
 	*/
  virtual void setHTMLSource(const QString& url);
  /**
 	* Installes a menu which will popup if the right mouse button was pressed on an anchor.
 	*/
  virtual void installAnchorMenu( QPopupMenu* );
  /**
 	* Returns the current anchor.
 	*/
  virtual QString getCurrentAnchor();
  /**
  * No descriptions
  */
  void installPopup( QPopupMenu* popup);
  /**
  * Returns true if this HTML widget is read only, false if it's editable
  */
  virtual bool isReadOnly() const;
  /**
  * Sets the HTML widget ediatable or not
  */
  virtual void setReadOnly( const bool readOnly = true);
  /**
  * This function returns the edit toolbar used for the HTML widget.
	* The items of the toolbar are already connected to the correct slots.
	*/
  void createEditToolbar( KToolBar* bar );
  QPoint m_pressedPos;

public slots: // Public slots
  /**
  * Saves the HTML page
  */
  void slotSaveAsHTML();
  /**
  * Saves the plain text page
  */
  void slotSaveAsText();
  /**
  * Copies the displayed document into the clipboard.
  */
  void copyDocument();
  /**
  * Sets the source of this widget.
  */
  void setSource(const QString& name);


protected: // Protected attributes
  /*
  * Initializes the colors of this widget (backgrouznd, links, text)
  */
  void initColors();
  /**
  * Initializes the fonts of the HTML-widget
  */
  void initFonts();
  /**
  * Initializes the connections to SIGNALs
  */
  void initConnections();
  /**
  * Initializes the view of this widget (childs, popups etc.)
  */
  void initView();
  /**
 	* Reimplementation.
 	*/
  virtual void contentsDragEnterEvent(QDragEnterEvent* e);
  /**
 	* Reimplementation.
 	*/
  virtual void contentsDragMoveEvent (QDragMoveEvent* e);
  /**
 	* Reimplementation.
 	*/
  virtual void contentsDropEvent(QDropEvent* e);
  /**
 	* Reimplementation.
 	*/
  virtual void contentsMousePressEvent(QMouseEvent* e);
  /**
 	* Reimplementation.
 	*/
  virtual void contentsMouseMoveEvent(QMouseEvent* e);
  /**
  * Returns true if the links are enabled.
  */
  virtual bool linksEnabled() const;
  /**
  * Reimplementation of QTextEdit
  */
  virtual void contentsMouseReleaseEvent( QMouseEvent* e);

protected slots: // Protected slots
  /**
  * No descriptions
  */
  void slotSaveDocument();
  /**
  * No descriptions
  */
  void slotToggleBold();
  /**
  * No descriptions
  */
  void slotToggleItalic();
  /**
  * No descriptions	
  */
  void slotToggleUnderline();
	/**
	*
	*/
	void slotSetCurrentFont(const QString& font);	
  /**
  *
  */
  void slotSetCurrentFontSize( int );	
	/**
	*
	*/
	void slotSetCurrentColor( const QColor& color);
  /**
  * No descriptions
  */
  void slotCurrentColorChanged( const QColor& );
  /**
  * No descriptions
  */
  void slotCurrentFontChanged( const QFont& );
  /**
  * No descriptions
  */
  void slotCurrentAlignementChanged( int );
  /**
  * No descriptions
  */
  void slotAlignRight();
  /**
  * No descriptions
  */
  void slotAlignCenter();
  /**
  * No descriptions
  */
  void slotAlignLeft();
  /**
  * No descriptions
  */
  void slotAlignJustify();
  /**
  * No descriptions
  */
  void slotSelectAll();
	
private:
	CImportantClasses* m_important;	

	bool m_readOnly;
	bool m_selectedWord;
	QString m_anchor;
	
  /**
  * Reimplementation from QTextView.
  */
  virtual void emitLinkClicked( const QString& s);

  QPopupMenu* m_popup;
  KConfig* m_config;
  QGuardedPtr<QPopupMenu>	m_anchorMenu;
    		
	KAction* m_docDeleteAction;	
	KAction* m_docSaveAction;
	
	KToggleAction* m_boldAction;
	KToggleAction* m_italicAction;
	KToggleAction* m_underlineAction;
	
	KToggleAction* m_alignLeftAction;			
	KToggleAction* m_alignCenterAction;		
	KToggleAction* m_alignRightAction;	
	KToggleAction* m_alignJustifyAction;
	
	KFontSizeAction* m_fontSizeAction;
	KFontAction* m_fontAction;
	KColorButton* m_colorChooser;
			
signals: // Signals
  void sigDeleteDocument();
  void sigSaveDocument(const QString);
  void referenceClicked(const QString&);
  void linkClicked(const QString&);
	void insertReference(const QString& reference);
};


#endif
