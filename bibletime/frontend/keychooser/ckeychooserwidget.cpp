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
#include <qstring.h>
#include <qstringlist.h>
#include <qcombobox.h>
#include <qtoolbutton.h>
#include <qpoint.h>
#include <qevent.h>
#include <qlayout.h>
#include <qpainter.h>
#include <qpixmap.h>
#include <qapplication.h>
#include <qwhatsthis.h>
#include <qtooltip.h>

//KDE includes
#include <kiconloader.h>

CKeyChooserWidget::CKeyChooserWidget(int count, QWidget *parent, const char *name) : QWidget(parent,name) {
	m_list = new QStringList();
	for (int index=1; index <= count; index++) {
		m_list->append( QString::number(index) );
	}	
	init(m_list);
};

CKeyChooserWidget::CKeyChooserWidget(QStringList *list, QWidget *parent, const char *name ) : QWidget(parent,name) {
	qDebug("CkeyChooserWidget::CkeyChooserWidget");
	m_list = 0;
	
	init(list);
}


CKeyChooserWidget::~CKeyChooserWidget(){
	qDebug("Destructor of CKeyChooserWidget");
	if (m_list)
		delete m_list;
	if (m_buttonLayout)
		delete m_buttonLayout;
	if (m_upIconSet)
		delete m_upIconSet;
	if (m_moverIconSet)
		delete m_moverIconSet;	
	if (m_downIconSet)
		delete m_downIconSet;
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
	if (!m_list)
		m_list = new QStringList();
	oldKey = QString::null;
			
	if (m_list->count() != (unsigned int)count) {	//same count, not necessary to regenerate
		m_list->clear();
		for (int i=1; i <= count; i++) {
			m_list->append( QString::number(i) );
		}	
	}
	reset(m_list,index,do_emit);
}

void CKeyChooserWidget::reset(QStringList *list, int index, bool do_emit){
	if (isResetting)
		return;
		
	isResetting = true;
	
	oldKey = QString::null;
	if (!m_list || (m_list && m_list != list) || (ComboBox->count() != list->count()) ) {
		ComboBox->clear();
		ComboBox->insertStringList(*list);
	}
	ComboBox->setCurrentItem(index);	
	
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
void CKeyChooserWidget::init( QStringList* list ){
	oldKey = QString::null;
	m_upPixmap = m_downPixmap = m_moverPixmap = 0;
	m_upIconSet = m_downIconSet = m_moverIconSet = 0;
	
	m_mainLayout = new QHBoxLayout( this );
		
	ComboBox = new QComboBox( true, this, "ComboBox" );
	ComboBox->setAutoCompletion( true );
//	ComboBox->setMaximumWidth( 300 );
	ComboBox->setInsertionPolicy(QComboBox::NoInsertion);
	
	m_mainLayout->addWidget( ComboBox );

	m_buttonLayout = new QVBoxLayout();	
	QIconSet* iconSet;	
	btn_up = new QToolButton( this, "btn_up" );
	iconSet = getUpIconSet();
	btn_up->setIconSet( *iconSet );
	btn_up->setFixedHeight(iconSet->pixmap().height());
	btn_up->setFixedWidth(iconSet->pixmap().width());
	
	btn_fx = new cfx_btn( this, "btn_fx" );
	iconSet = getMoverIconSet();
	btn_fx->setIconSet( *iconSet );	
	btn_fx->setFixedHeight(iconSet->pixmap().height());
	btn_fx->setFixedWidth(iconSet->pixmap().width());

	btn_down = new QToolButton( this, "btn_down" );
	iconSet = getDownIconSet();
	btn_down->setIconSet( *iconSet );
	btn_down->setFixedHeight(iconSet->pixmap().height());
	btn_down->setFixedWidth(iconSet->pixmap().width());

	m_buttonLayout->addWidget( btn_up );	
	m_buttonLayout->addWidget( btn_fx );	
	m_buttonLayout->addWidget( btn_down );
	
	m_mainLayout->addLayout( m_buttonLayout );
	m_mainLayout->addSpacing(3);
	
	if (list)
		ComboBox->insertStringList(*list, 0);

// signals and slots connections
	connect( btn_up, SIGNAL( clicked() ),SIGNAL( next_requested() ) );	
	connect( btn_down, SIGNAL( clicked() ), SIGNAL( prev_requested() ) );
	connect( btn_fx, SIGNAL( lock() ), this, SLOT( lock() ) );
	connect( btn_fx, SIGNAL( unlock() ), this, SLOT( unlock() ) );
	connect( btn_fx, SIGNAL( change_requested(int) ), this, SLOT( changeCombo(int) ) );
	connect( ComboBox, SIGNAL( activated( int )), SLOT(slotComboChanged(int)));
	connect( ComboBox, SIGNAL( activated(const QString&)), SLOT(slotReturnPressed(const QString&)));
		
//	adjustSize();	
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

/** Is called when the current item of the combo boy was changed. */
void CKeyChooserWidget::slotComboChanged(int index){
	const QString key = ComboBox->text( index );
	if (oldKey != key)
		emit changed(index);
	oldKey = key;
}

/** Returns the icons set which contains the down button. */
QIconSet* CKeyChooserWidget::getUpIconSet(){
	if (!m_upIconSet) {
		if (m_upPixmap)
			delete m_upPixmap;
			
		m_upPixmap = new QPixmap(16,10);
		QPainter p(m_upPixmap);
		p.fillRect(0,0, m_upPixmap->width(), m_upPixmap->height(), Qt::lightGray);		
		p.setPen( QPen(Qt::black,2) );
		p.drawLine(3, m_upPixmap->height()-3, m_upPixmap->width()/2, 3);
		p.drawLine( m_upPixmap->width()/2, 3, m_upPixmap->width()-3, m_upPixmap->height()-3 );
		
		m_upIconSet = new QIconSet(*m_upPixmap);
	}
	ASSERT(m_upIconSet);	
	return m_upIconSet;
}

/** Returns the icons set for the button used to change the current item. */
QIconSet* CKeyChooserWidget::getMoverIconSet(){
	if (!m_moverIconSet) {
		if (m_moverPixmap)
			delete m_moverPixmap;
			
		m_moverPixmap = new QPixmap(16,6);
		
		QPainter p(m_moverPixmap);
		p.setPen( QPen(Qt::black,2) );		
		p.fillRect(0,0, m_moverPixmap->width(), m_moverPixmap->height(), Qt::white);		
		
		m_moverIconSet = new QIconSet(*m_moverPixmap);
	}	
	ASSERT(m_moverIconSet);
	return m_moverIconSet;
}

/** Returns the icons set which contains the down button. */
QIconSet* CKeyChooserWidget::getDownIconSet(){
	if (!m_downIconSet) {
		if (m_downPixmap)
			delete m_downPixmap;
			
		m_downPixmap = new QPixmap(16,10);		
		QPainter p(m_downPixmap);
		p.fillRect(0,0, m_downPixmap->width(), m_downPixmap->height(), Qt::lightGray);
		p.setPen( QPen(Qt::black,2) );
		p.drawLine(3, 3, m_downPixmap->width()/2, m_downPixmap->height()-3);
		p.drawLine(m_downPixmap->width()/2, m_downPixmap->height()-3, m_downPixmap->width()-3, 3 );
		
		m_downIconSet = new QIconSet(*m_downPixmap);
	}
	ASSERT(m_downIconSet);
	return m_downIconSet;
}

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
	if (ComboBox)
		QToolTip::add(ComboBox, comboTip);
	if (btn_up)
		QToolTip::add(btn_up, nextEntryTip);
	if (btn_fx)
		QToolTip::add(btn_fx, scrollButtonTip);
	if (btn_down)
		QToolTip::add(btn_down, previousEntryTip);
}

/** No descriptions */
void CKeyChooserWidget::setWhatsThis(const QString comboTip, const QString nextEntryTip, const QString scrollButtonTip, const QString previousEntryTip){
	if (ComboBox)
		QWhatsThis::add(ComboBox, comboTip);
	if (btn_up)
		QWhatsThis::add(btn_up, nextEntryTip);
	if (btn_fx)
		QWhatsThis::add(btn_fx, scrollButtonTip);
	if (btn_down)
		QWhatsThis::add(btn_down, previousEntryTip);
}
