/***************************************************************************
                          chtmlwritedisplay.cpp  -  description
                             -------------------
    begin                : Fre Nov 29 2002
    copyright            : (C) 2002 by The BibleTime team
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

#include "chtmlwritedisplay.h"
#include "frontend/displaywindow/cwritewindow.h"
#include "frontend/cresmgr.h"

//Qt includes
#include <qpopupmenu.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

//KDE includes
#include <kaction.h>
#include <ktoolbar.h>
#include <klocale.h>
#include <kfontcombo.h>
#include <kcolorbutton.h>
#include <kpopupmenu.h>

CHTMLWriteDisplay::CHTMLWriteDisplay(CWriteWindow* parentWindow, QWidget* parent)
  : CPlainWriteDisplay(parentWindow,parent)
{
  m_actions.bold = 0;
  m_actions.italic = 0;
  m_actions.underline = 0;
  
  m_actions.selectAll = 0;
  
//  qWarning("constructor of CHTMLWriteDisplay");
  setTextFormat(Qt::RichText);
}

CHTMLWriteDisplay::~CHTMLWriteDisplay(){
}

//void CHTMLWriteDisplay::print( const CDisplay::TextPart ) {
//
//};
//
//const bool CHTMLWriteDisplay::isModified() const {
//  return QTextEdit::isModified();
//};
//
//void CHTMLWriteDisplay::setModified( const bool modified ) {
//  QTextEdit::setModified(modified);
//};

const QString CHTMLWriteDisplay::plainText() {
  return QTextEdit::text();
};

//QPopupMenu* CHTMLWriteDisplay::createPopupMenu( const QPoint& pos ) {
//
//};
//
//QPopupMenu* CHTMLWriteDisplay::createPopupMenu() {
//
//}

void CHTMLWriteDisplay::toggleBold() {
  setBold( m_actions.bold->isChecked() );
};

void CHTMLWriteDisplay::toggleItalic() {
  setItalic( m_actions.italic->isChecked() );
};

void CHTMLWriteDisplay::toggleUnderline() {
  setUnderline( m_actions.underline->isChecked() );
};


void CHTMLWriteDisplay::alignLeft() {
  setAlignment(Qt::AlignLeft);
};

void CHTMLWriteDisplay::alignCenter() {
  setAlignment(Qt::AlignHCenter);
};

void CHTMLWriteDisplay::alignRight() {
  setAlignment(Qt::AlignRight);
};

//void CHTMLWriteDisplay::alignJustify() {
//  setAlignment(Qt::AlignJustify);
//};



/** The text's alignment changed. Enable the right buttons. */
void CHTMLWriteDisplay::slotAlignmentChanged( int a ){
  bool alignLeft = false;
  bool alignCenter = false;
  bool alignRight = false;
//  bool alignJustify = false;

  if (a & Qt::AlignLeft) {
    alignLeft = true;
  }
  else if ((a & Qt::AlignHCenter) || (a & Qt::AlignCenter)) {
    alignCenter = true;
  }
  else if (a & Qt::AlignRight) {
    alignRight = true;
  }
//  else if (a & Qt::AlignJustify) {
//    alignJustify = true;
//  }
  else {
    alignLeft = true;
    qWarning("unknown alignment %i", a);
  }

  m_actions.alignLeft->setChecked( alignLeft );
  m_actions.alignCenter->setChecked( alignCenter );
  m_actions.alignRight->setChecked( alignRight );    
//  m_actions.alignJustify->setChecked( alignJustify );
}


/** Is called when a new color was selected. */
void CHTMLWriteDisplay::slotColorSelected( const QColor& c){
  setColor( c );
}

/** Is called when a text with another color was selected. */
void CHTMLWriteDisplay::slotColorChanged(const QColor& c){
  m_colorButton->setColor(c);
}


void CHTMLWriteDisplay::slotFontChanged( const QFont& font ) {
  m_actions.fontChooser->setFont( font.family() );
  m_actions.fontSizeChooser->setFontSize( font.pointSize() );

  m_actions.bold->setChecked( font.bold() );
  m_actions.italic->setChecked( font.italic() );
  m_actions.underline->setChecked( font.underline() );
};

void CHTMLWriteDisplay::setupToolbar(KToolBar * bar, KActionCollection * actions) {
  m_actions.fontChooser = new KFontAction( i18n("Choose a font"),
    CResMgr::displaywindows::writeWindow::underlinedText::icon,
    CResMgr::displaywindows::writeWindow::underlinedText::accel,
    actions
  );
  m_actions.fontChooser->setToolTip( CResMgr::displaywindows::writeWindow::fontFamily::tooltip );
  m_actions.fontChooser->setWhatsThis( CResMgr::displaywindows::writeWindow::fontFamily::whatsthis );
  m_actions.fontChooser->plug(bar);
  connect(m_actions.fontChooser, SIGNAL(activated(const QString&)), this, SLOT(setFamily(const QString&)));


  m_actions.fontSizeChooser = new KFontSizeAction( i18n("Choose a font size"),
    CResMgr::displaywindows::writeWindow::underlinedText::icon,
    CResMgr::displaywindows::writeWindow::underlinedText::accel,
    actions
  );
  m_actions.fontSizeChooser->setToolTip( CResMgr::displaywindows::writeWindow::fontSize::tooltip );
  m_actions.fontSizeChooser->setWhatsThis( CResMgr::displaywindows::writeWindow::fontSize::whatsthis );
  m_actions.fontSizeChooser->plug(bar);
  connect(m_actions.fontSizeChooser, SIGNAL(fontSizeChanged(int)), this, SLOT(setPointSize(int)));

  m_colorButton = new KColorButton(bar);
  connect(m_colorButton, SIGNAL(changed(const QColor&)), this, SLOT(slotColorSelected(const QColor&)));
  bar->insertWidget(50, m_colorButton->sizeHint().width(), m_colorButton);
  QToolTip::add(m_colorButton, CResMgr::displaywindows::writeWindow::fontColor::tooltip );
  QWhatsThis::add(m_colorButton, CResMgr::displaywindows::writeWindow::fontColor::whatsthis );


  (new KActionSeparator())->plug(bar); //seperate font options from formatting buttons

 	m_actions.bold = new KToggleAction( i18n("Bold"),
    CResMgr::displaywindows::writeWindow::boldText::icon,
    CResMgr::displaywindows::writeWindow::boldText::accel,
    this, SLOT( toggleBold()  ),
    actions
  );
  m_actions.bold->setToolTip( CResMgr::displaywindows::writeWindow::boldText::tooltip );
  m_actions.bold->setWhatsThis( CResMgr::displaywindows::writeWindow::boldText::whatsthis );
  m_actions.bold->plug(bar);

 	m_actions.italic = new KToggleAction( i18n("Italic"),
    CResMgr::displaywindows::writeWindow::italicText::icon,
    CResMgr::displaywindows::writeWindow::italicText::accel,
    this, SLOT( toggleItalic()  ),
    actions
  );
  m_actions.italic->setToolTip( CResMgr::displaywindows::writeWindow::italicText::tooltip );
  m_actions.italic->setWhatsThis( CResMgr::displaywindows::writeWindow::italicText::whatsthis );
  m_actions.italic->plug(bar);


 	m_actions.underline = new KToggleAction( i18n("Underline"),
    CResMgr::displaywindows::writeWindow::underlinedText::icon,
    CResMgr::displaywindows::writeWindow::underlinedText::accel,
    this, SLOT( toggleUnderline()  ),
    actions
  );
  m_actions.underline->setToolTip( CResMgr::displaywindows::writeWindow::underlinedText::tooltip );
  m_actions.underline->setWhatsThis( CResMgr::displaywindows::writeWindow::underlinedText::whatsthis );
  m_actions.underline->plug(bar);


  (new KActionSeparator())->plug(bar); //seperate formatting from alignment buttons


 	m_actions.alignLeft = new KToggleAction( i18n("Left"),
    CResMgr::displaywindows::writeWindow::alignLeft::icon,
    CResMgr::displaywindows::writeWindow::alignLeft::accel,
    this, SLOT( alignLeft()  ),
    actions
  );
  m_actions.alignLeft->setToolTip( CResMgr::displaywindows::writeWindow::alignLeft::tooltip );
  m_actions.alignLeft->setWhatsThis( CResMgr::displaywindows::writeWindow::alignLeft::whatsthis );
  m_actions.alignLeft->plug(bar);

 	m_actions.alignCenter = new KToggleAction( i18n("Center"),
    CResMgr::displaywindows::writeWindow::alignCenter::icon,
    CResMgr::displaywindows::writeWindow::alignCenter::accel,
    this, SLOT( alignCenter()  ),
    actions
  );
  m_actions.alignCenter->setToolTip( CResMgr::displaywindows::writeWindow::alignCenter::tooltip );
  m_actions.alignCenter->setWhatsThis( CResMgr::displaywindows::writeWindow::alignCenter::whatsthis );
  m_actions.alignCenter->plug(bar);

 	m_actions.alignRight = new KToggleAction( i18n("Right"),
    CResMgr::displaywindows::writeWindow::alignRight::icon,
    CResMgr::displaywindows::writeWindow::alignRight::accel,
    this, SLOT( alignRight()  ),
    actions
  );
  m_actions.alignRight->setToolTip( CResMgr::displaywindows::writeWindow::alignRight::tooltip );
  m_actions.alignRight->setWhatsThis( CResMgr::displaywindows::writeWindow::alignRight::whatsthis );
  m_actions.alignRight->plug(bar);

// 	m_actions.alignJustify = new KToggleAction( i18n("Justify"),
//    CResMgr::displaywindows::writeWindow::alignJustify::icon,
//    CResMgr::displaywindows::writeWindow::alignJustify::accel,
//    this, SLOT( alignJustify()  ),
//    actions
//  );
//  m_actions.alignJustify->setToolTip( CResMgr::displaywindows::writeWindow::alignJustify::tooltip );
//  m_actions.alignJustify->setWhatsThis( CResMgr::displaywindows::writeWindow::alignJustify::whatsthis );
//  m_actions.alignJustify->plug(bar);


  connect(this, SIGNAL(currentFontChanged(const QFont&)), SLOT(slotFontChanged(const QFont&)));
  connect(this, SIGNAL(currentAlignmentChanged(int)), SLOT(slotAlignmentChanged(int)));
  connect(this, SIGNAL(currentColorChanged(const QColor&)), SLOT(slotColorChanged(const QColor&)));


  //set initial values for toolbar items
  slotFontChanged( font() );
  slotAlignmentChanged( alignment() );
  slotColorChanged( color() );
}

/** Reimplementation to show a popup menu if the right mouse butoon was clicked. */
QPopupMenu* CHTMLWriteDisplay::createPopupMenu( const QPoint& /*pos*/ ){
  if (!m_actions.selectAll) {
 	  m_actions.selectAll = new KAction(i18n("Select all"), KShortcut(0), this, SLOT(selectAll()), this);
  }
  
  KPopupMenu* popup = new KPopupMenu(this);
	popup->insertTitle(i18n("HTML editor window"));
  m_actions.selectAll->plug(popup);  

  return popup;
};
