/***************************************************************************
                          ckeychooserwidget.cpp  -  description
                             -------------------
    begin                : Tue Sep 19 2000
    copyright            : (C) 2000 by The BibleTime team
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
//BibleTime includes
#include "ckeychooserwidget.h"
#include "cfx_btn.h"

//Qt includes
#include <qlineedit.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qlistbox.h>
#include <qtoolbutton.h>
#include <qevent.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qstyle.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qwhatsthis.h>
#include <qtooltip.h>

//KDE includes

CKCComboBox::CKCComboBox(bool rw,QWidget* parent,const char* name)
  :QComboBox(rw,parent,name){

//  setFocusPolicy(QWidget::StrongFocus);
  installEventFilter( lineEdit() );
}

/** Reimplementation. */
bool CKCComboBox::eventFilter( QObject *o, QEvent *e ){		
	if ((o == lineEdit()) && (e->type() == QEvent::FocusOut)) {
		QFocusEvent* f = (QFocusEvent*)e;
		if (f->reason() != QFocusEvent::Tab)
			return false;
			
    int index = listBox()->index( listBox()->findItem(currentText()) );
    if (index==-1)
    	index++;// return 0 if not found
    emit focusOut( index );  	
  	setCurrentItem( index );
	}
  QComboBox::eventFilter(o,e);	
}


//**********************************************************************************/

CKeyChooserWidget::CKeyChooserWidget(int count, QWidget *parent, const char *name) : QWidget(parent,name) {
	for (int index=1; index <= count; index++) {
		m_list.append( QString::number(index) );
	}	
	init();
};

CKeyChooserWidget::CKeyChooserWidget(QStringList *list, QWidget *parent, const char *name ) : QWidget(parent,name) {
	m_list = *list;
	init();
}

void CKeyChooserWidget::changeCombo(int i){
	int current = ComboBox->currentItem();

	//index of highest Item
	const int count = ComboBox->count()-1;
	int j = current + i;
	if (i > 0){
		if (j <= count)
			ComboBox->setCurrentItem(j);
		else
			ComboBox->setCurrentItem(count);
	}
	else if (i < 0){
		if (j>=0)
			ComboBox->setCurrentItem(j);
		else
			ComboBox->setCurrentItem(0);
	}	
	if ((!isResetting) && (!btn_fx->isLocked) && (current != ComboBox->currentItem()))	
		emit changed(ComboBox->currentItem());
}

void CKeyChooserWidget::reset(const int count, int index, bool do_emit){
	oldKey = QString::null;			
	if (m_list.count() != (unsigned int)count) {	//equal->same count, not necessary to regenerate
		m_list.clear();
		for (int i=1; i <= count; i++)
			m_list.append( QString::number(i) );
	}
	reset(&m_list,index,do_emit);
}

void CKeyChooserWidget::reset(QStringList *list, int index, bool do_emit){
	if (isResetting)
		return;
		
	isResetting = true;
	
	oldKey = QString::null;
	ComboBox->setUpdatesEnabled(false);
	ComboBox->clear();
	ComboBox->insertStringList(*list);
	ComboBox->setCurrentItem(index);	
	ComboBox->setUpdatesEnabled(true);
			
	if (list && !list->count()) {
		btn_up->setEnabled(false);
		btn_fx->setEnabled(false);
		btn_down->setEnabled(false);		
	}
	else if (list && list->count() == 1) {
		btn_up->setEnabled(true);
		btn_fx->setEnabled(false);
		btn_down->setEnabled(true);		
	}
	else {
		btn_up->setEnabled(true);
		btn_fx->setEnabled(true);
		btn_down->setEnabled(true);			
	}	
	isResetting = false;
	if (do_emit)
		emit changed(ComboBox->currentItem());		
}

void CKeyChooserWidget::lock(void){
	ComboBox->setEditable(false);
	oldKey = ComboBox->currentText();
}

void CKeyChooserWidget::unlock(void){
	ComboBox->setEditable(true);
	ComboBox->setEditText(ComboBox->text(ComboBox->currentItem()));
	if (ComboBox->currentText() != oldKey);
		emit changed(ComboBox->currentItem());
}

/** Initializes this widget. We need this function because we have more than one constructor. */
void CKeyChooserWidget::init( ){
	oldKey = QString::null;
	btn_up = btn_down = btn_fx = 0;

	setFocusPolicy(QWidget::StrongFocus);			
	QHBoxLayout *m_mainLayout = new QHBoxLayout( this );	
	QVBoxLayout *m_buttonLayout = new QVBoxLayout();	
			
	ComboBox = new CKCComboBox( true, this, "ComboBox" );
	ComboBox->setAutoCompletion( true );
	ComboBox->setInsertionPolicy(QComboBox::NoInsertion);
	ComboBox->insertStringList(m_list, 0);
	ComboBox->setFocusPolicy(QWidget::StrongFocus);
		
	m_mainLayout->addWidget( ComboBox );
	
	btn_up = new QToolButton( this, "btn_up" );	
	QIconSet iconSet = getUpIconSet();
	btn_up->setIconSet( iconSet );
	btn_up->setFixedHeight(iconSet.pixmap().height());
	btn_up->setFixedWidth(iconSet.pixmap().width());
	btn_up->setFocusPolicy(QWidget::NoFocus);	
	
	btn_fx = new cfx_btn( this, "btn_fx" );
	iconSet = getMoverIconSet();
	btn_fx->setIconSet( iconSet );	
	btn_fx->setFixedHeight(iconSet.pixmap().height());
	btn_fx->setFixedWidth(iconSet.pixmap().width());
	btn_fx->setFocusPolicy(QWidget::NoFocus);	
	
	btn_down = new QToolButton( this, "btn_down" );	
	iconSet = getDownIconSet();
	btn_down->setIconSet( iconSet );
	btn_down->setFixedHeight(iconSet.pixmap().height());
	btn_down->setFixedWidth(iconSet.pixmap().width());
	btn_down->setFocusPolicy(QWidget::NoFocus);	
	
	m_buttonLayout->addWidget( btn_up );	
	m_buttonLayout->addWidget( btn_fx );	
	m_buttonLayout->addWidget( btn_down );
	
	m_mainLayout->addLayout( m_buttonLayout );
	m_mainLayout->addSpacing(2);
	
// signals and slots connections
	connect( btn_up, SIGNAL( clicked() ),SIGNAL( next_requested() ) );	
	connect( btn_down, SIGNAL( clicked() ), SIGNAL( prev_requested() ) );
	connect( btn_fx, SIGNAL( lock() ), this, SLOT( lock() ) );
	connect( btn_fx, SIGNAL( unlock() ), this, SLOT( unlock() ) );
	connect( btn_fx, SIGNAL( change_requested(int) ), this, SLOT( changeCombo(int) ) );
	connect( ComboBox, SIGNAL(activated( int )), SLOT(slotComboChanged(int)));
	connect( ComboBox, SIGNAL(activated(const QString&)), SLOT(slotReturnPressed(const QString&)));
	connect( ComboBox, SIGNAL(focusOut(int)), SIGNAL(focusOut(int)));	
		
	isResetting = false;
}

/** Is called when the return key was presed in the combobox. */
void CKeyChooserWidget::slotReturnPressed( const QString& text){
	qDebug("CKeyChooserWidget::slotReturnPressed( const QString& text)");
	int index = 0;
	
	for (index=0; index < ComboBox->count(); index++) {
		if (ComboBox->text(index) == text) {
			break;
		}
	}

	if (text != oldKey)	//if the key has changed
		emit changed(index);
}

/** Is called when the current item of the combo box was changed. */
void CKeyChooserWidget::slotComboChanged(int index){
	const QString key = ComboBox->text( index );
	if (oldKey != key)
		emit changed(index);
	oldKey = key;
}
#define WIDTH 17
#define ARROW_HEIGHT 10
#define MOVER_HEIGHT 6

/** Returns the icons set which contains the down button. */
QIconSet CKeyChooserWidget::getUpIconSet(){
  QPixmap pix(WIDTH,ARROW_HEIGHT);
	QPainter p(&pix);
	p.fillRect(0,0, WIDTH-1, ARROW_HEIGHT-1, colorGroup().background());
	style().drawArrow(&p, Qt::UpArrow, false, 1,1, WIDTH-2, ARROW_HEIGHT-2, btn_up ? btn_up->colorGroup() : colorGroup(), btn_up ? btn_up->isEnabled() : true);
	
	return QIconSet(pix);
}

/** Returns the icons set which contains the down button. */
QIconSet CKeyChooserWidget::getDownIconSet(){
  QPixmap pix(WIDTH,ARROW_HEIGHT);
	QPainter p(&pix);
	p.fillRect(0,0, WIDTH-1, ARROW_HEIGHT-1, colorGroup().background());
	style().drawArrow(&p, Qt::DownArrow, false, 1,1, WIDTH-2, ARROW_HEIGHT-2, btn_down ? btn_down->colorGroup() : colorGroup(), btn_down ? btn_down->isEnabled() : true);
	
	return QIconSet(pix);
}

/** Returns the icons set for the button used to change the current item. */
QIconSet CKeyChooserWidget::getMoverIconSet(){
  QPixmap pix(WIDTH,MOVER_HEIGHT);
	QPainter p(&pix);
	p.fillRect(0,0, WIDTH-1, MOVER_HEIGHT-1, colorGroup().background());
	p.fillRect(2,2, WIDTH-3, MOVER_HEIGHT-3, colorGroup().foreground());		
	return QIconSet(pix);
}
#undef WIDTH
#undef ARROW_HEIGHT
#undef MOVER_HEIGHT

/**  */
void CKeyChooserWidget::adjustSize( ){
//	setUpdatesEnabled(false);
	QSize s = sizeHint();
	if (s.width() > maximumWidth())
		s.setWidth( maximumWidth() );
	
	resize(s);
//	setUpdatesEnabled(true);	
}
/** Sets the tooltips for the given entries using the parameters as text. */
void CKeyChooserWidget::setToolTips( const QString comboTip, const QString nextEntryTip, const QString scrollButtonTip, const QString previousEntryTip){
	QToolTip::add(ComboBox,comboTip);
	QToolTip::add(btn_up,  nextEntryTip);
	QToolTip::add(btn_fx,  scrollButtonTip);
	QToolTip::add(btn_down,previousEntryTip);
}

/** No descriptions */
void CKeyChooserWidget::setWhatsThis(const QString comboTip, const QString nextEntryTip, const QString scrollButtonTip, const QString previousEntryTip){
	QWhatsThis::add(ComboBox,comboTip);
	QWhatsThis::add(btn_up,  nextEntryTip);
	QWhatsThis::add(btn_fx,  scrollButtonTip);
	QWhatsThis::add(btn_down,previousEntryTip);
}

