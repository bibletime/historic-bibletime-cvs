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
#include "../cbtconfig.h"

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
//#include <qwheelevent.h>
#include <qtooltip.h>

//bool operator> (const QSize & s1, const QSize & s2)  {
//	/* Return true is s1 is greater than s2		
//	*/
//	if (s1.width() > s2.width() && s1.height()>s2.height()) // s1 is greater than s2
//		return true;
//	else
//		return false;
//}
//
//bool operator>= (const QSize & s1, const QSize & s2)  {
//	/* Return true is s1 is greater than s2		
//	*/
//	if (s1.width() >= s2.width() && s1.height()>=s2.height()) // s1 is greater than s2
//		return true;
//	else
//		return false;
//}

CKCComboBox::CKCComboBox(bool rw,QWidget* parent,const char* name)
  : QComboBox(rw,parent,name){
	setFocusPolicy(QWidget::WheelFocus);
  installEventFilter( lineEdit() );
}

/** Reimplementation. */
bool CKCComboBox::eventFilter( QObject *o, QEvent *e ){			
	if (e->type() == QEvent::FocusOut) {
		QFocusEvent* f = dynamic_cast<QFocusEvent*>(e);
		if (o == lineEdit() && f->reason() == QFocusEvent::Tab) {
	    int index = listBox()->index( listBox()->findItem(currentText()) );
	    if (index == -1)
				index = 0;// return 0 if not found
	  	setCurrentItem( index );	
	    emit focusOut( index );  	
	  }
	  else if (o == lineEdit() && f->reason() == QFocusEvent::Popup) {
			return false;
		}
	  else if (o == lineEdit() && f->reason() == QFocusEvent::ActiveWindow) {
			emit activated(currentText());
			return true;
		}
	  else if (o == lineEdit() && f->reason() == QFocusEvent::Mouse) {
			emit activated(currentText());
			return true;
		}		
	  else if (o == listBox()) {
			return false;
		}
	  else if (o == this) {
			emit activated(currentText());
			return true;
		}		
	}
  return QComboBox::eventFilter(o,e);	
}

/** Scrolls in the list if the wheel of the mouse was used. */
void CKCComboBox::wheelEvent( QWheelEvent* e ) {
	const signed int change = (int)((float)e->delta()/(float)120);
	int current = currentItem();
	
	if ((current+change >= 0) && (current+change<count()) ) {
		setCurrentItem(current+change);
		e->accept();
		emit activated( currentItem() );
	}
	else {
		e->ignore();
	}
}

///** Returns the size this widget would like to have. */
//QSize CKCComboBox::sizeHint() const {
//	// IMHO Qt has a bug: The sizehint is not updated if the list is refreshed with other items
//	if (lineEdit()->sizeHint() > listBox()->sizeHint() && lineEdit()->sizeHint() > QComboBox::sizeHint() ) {
//		qWarning("combo::sizeHint() use lineEdits hint");		
//		return lineEdit()->sizeHint();
//	}
//	else if (listBox()->sizeHint() > QComboBox::sizeHint() ){
//		return QSize(lineEdit()->sizeHint().height(), listBox()->sizeHint().width());
//	}
//	return QComboBox::sizeHint(); //return standard sizeHint	
//}


void CKCComboBox::insertItem ( const QString & text, int index, unsigned long int userData ) {
	insertItem(text, index);
	const int newIndex = ((index == -1) ? count()-1 : index);
	m_userDataMap.insert(newIndex, userData);
}

/** Returns the user data for index. */
const unsigned long int CKCComboBox::userData( const int index ){
	if (index < (count()-1))
		return m_userDataMap[index];
	else
		return 0;
}

//**********************************************************************************/

CKeyChooserWidget::CKeyChooserWidget(int count, const bool useNextPrevSignals,  QWidget *parent, const char *name) : QWidget(parent,name) {
	m_useNextPrevSignals = useNextPrevSignals;
	for (int index=1; index <= count; index++)
		m_list.append( QString::number(index) );	
	init();
};

CKeyChooserWidget::CKeyChooserWidget(QStringList *list, const bool useNextPrevSignals, QWidget *parent, const char *name ) : QWidget(parent,name) {
	m_useNextPrevSignals = useNextPrevSignals;		
	if (list)
		m_list = *list;//copy the items of list
	else
		m_list.clear();
	init();
}

void CKeyChooserWidget::changeCombo(int i){
	qDebug("CKeyChooserWidget::changeCombo(int i)");
	if (!isUpdatesEnabled())
		return;
	setUpdatesEnabled(false);
	
	int current = comboBox()->currentItem();

	//index of highest Item
	const int count = comboBox()->count()-1;
	int j = current + i;
	if (i > 0){
		if (j <= count)
			comboBox()->setCurrentItem(j);
		else
			comboBox()->setCurrentItem(count);
	}
	else if (i < 0){
		if (j>=0)
			comboBox()->setCurrentItem(j);
		else
			comboBox()->setCurrentItem(0);
	}	
	if (!isResetting && !btn_fx->isLocked() && (current != comboBox()->currentItem()))	
		emit changed(comboBox()->currentItem());
		
	setUpdatesEnabled(true);		
}

void CKeyChooserWidget::reset(const int count, int index, bool do_emit){
//	qDebug("CKeyChooserWidget::reset(const int count, int index, bool do_emit)");
	m_list.clear();
	for (int i=1; i <= count; i++)
		m_list.append( QString::number(i) );
	reset(&m_list,index,do_emit);
}

void CKeyChooserWidget::reset(QStringList& list, int index, bool do_emit){
	qDebug("CKeyChooserWidget::reset(QStringList&, int index, bool do_emit)");
	m_list = list;
	reset(&m_list,index,do_emit);
}


void CKeyChooserWidget::reset(QStringList *list, int index, bool do_emit){
	if (isResetting || !isUpdatesEnabled())
		return;
	setUpdatesEnabled(false);
	comboBox()->setUpdatesEnabled(false);
			
	isResetting = true;
//	layout()->invalidate();	
	oldKey = QString::null;
	comboBox()->clear();
	comboBox()->insertStringList(*list);
//	comboBox()->resize(QSize(comboBox()->listBox()->sizeHint().width(), comboBox()->sizeHint().height()));
//	qWarning("comboBox()->sizeHint: %i", comboBox()->sizeHint().width());		
//	qWarning("comboBox()->lineEdit->sizeHint: %i", comboBox()->lineEdit()->sizeHint().width());			
//	qWarning("comboBox()->listBox->sizeHint: %i", comboBox()->listBox()->sizeHint().width());				
//  comboBox()->updateGeometry();
//	updateGeometry();
//	layout()->invalidate();
		
	comboBox()->setCurrentItem(index);
	if (!list || (list && !list->count())) { //nothing in the combobox
		btn_up->setEnabled( true );
		btn_fx->setEnabled( true );
		btn_down->setEnabled( true );		
		setEnabled(false);
	}
	else if (!isEnabled()) { //was disabled
		setEnabled(true);	
		const bool enableButtons = list && (list->count()>=1);
		btn_up->setEnabled( enableButtons );
		btn_fx->setEnabled( enableButtons );
		btn_down->setEnabled( list && (list->count()>1) );
	}
	
	if (do_emit)
		emit changed(comboBox()->currentItem());			
	isResetting = false;
	setUpdatesEnabled(true);
	comboBox()->setUpdatesEnabled(true);
//	qWarning("layout()->sizeHint().width(): %i", layout()->sizeHint().width());			
}

void CKeyChooserWidget::lock(void){
	comboBox()->setEditable(false);
	oldKey = comboBox()->currentText();
}

void CKeyChooserWidget::unlock(void){
	comboBox()->setEditable(true);
	comboBox()->setEditText(comboBox()->text(comboBox()->currentItem()));
	if (comboBox()->currentText() != oldKey);
		emit changed(comboBox()->currentItem());
}

/** Initializes this widget. We need this function because we have more than one constructor. */
void CKeyChooserWidget::init( ){
	oldKey = QString::null;
	btn_up = btn_down = btn_fx = 0;

	setFocusPolicy(QWidget::StrongFocus);			
	QHBoxLayout *m_mainLayout = new QHBoxLayout( this );	
		
	m_comboBox = new CKCComboBox( true, this, "comboBox()" );
	comboBox()->setAutoCompletion( true );
	comboBox()->setInsertionPolicy(QComboBox::NoInsertion);
	comboBox()->insertStringList(m_list, 0);
	comboBox()->setFocusPolicy(QWidget::WheelFocus);
		
	m_mainLayout->addWidget( comboBox() );

	QVBoxLayout *m_buttonLayout = new QVBoxLayout();	
	m_buttonLayout->setAlignment(Qt::AlignHCenter | Qt::AlignCenter);
		
	btn_up = new QToolButton( this, "btn_up" );	
	QIconSet iconSet = getUpIconSet();
	btn_up->setIconSet( iconSet );
	btn_up->setFixedSize(iconSet.pixmap().width(), iconSet.pixmap().height());
	btn_up->setFocusPolicy(QWidget::NoFocus);	
	
	btn_fx = new cfx_btn( this, "btn_fx" );
	iconSet = getMoverIconSet();
	btn_fx->setIconSet( iconSet );	
	btn_fx->setFixedSize(iconSet.pixmap().width(), iconSet.pixmap().height());	
	btn_fx->setFocusPolicy(QWidget::NoFocus);	
	
	btn_down = new QToolButton( this, "btn_down" );	
	iconSet = getDownIconSet();
	btn_down->setIconSet( iconSet );
	btn_down->setFixedSize(iconSet.pixmap().width(), iconSet.pixmap().height());	
	btn_down->setFocusPolicy(QWidget::NoFocus);	
	
	m_buttonLayout->addWidget( btn_up );	
	m_buttonLayout->addWidget( btn_fx );	
	m_buttonLayout->addWidget( btn_down );
	
	m_mainLayout->addLayout( m_buttonLayout );
	m_mainLayout->addSpacing(2);

	setTabOrder(comboBox(), 0);
		
// signals and slots connections
 	if ( CBTConfig::get(CBTConfig::scroll) ) {
		if (m_useNextPrevSignals) {
			connect(btn_up, SIGNAL(clicked()), SIGNAL(prev_requested()) );	
			connect(btn_down, SIGNAL(clicked()), SIGNAL(next_requested()) );
	  }
	  else {
			connect(btn_up, SIGNAL(clicked()), SLOT(previous()) );	
			connect(btn_down, SIGNAL(clicked()), SLOT(next()) );	
	  }
	}
	else {
		if (m_useNextPrevSignals) {
			connect(btn_up, SIGNAL(clicked()), SIGNAL(next_requested()) );	
			connect(btn_down, SIGNAL(clicked()), SIGNAL(prev_requested()) );
	  }
	  else {
			connect(btn_up, SIGNAL(clicked()), SLOT(next()) );	
			connect(btn_down, SIGNAL(clicked()), SLOT(previous()) );	
		}
	}
	
	connect(btn_fx, SIGNAL(lock()), SLOT(lock()) );
	connect(btn_fx, SIGNAL(unlock()), SLOT(unlock()) );
	connect(btn_fx, SIGNAL(change_requested(int)), SLOT(changeCombo(int)) );
	connect(comboBox(), SIGNAL(activated(int)), SLOT(slotComboChanged(int)));
	connect(comboBox(), SIGNAL(activated(const QString&)), SLOT(slotReturnPressed(const QString&)));
 	connect(comboBox(), SIGNAL(focusOut(int)), SIGNAL(focusOut(int)));	
		
	isResetting = false;
}

/** Is called when the return key was presed in the combobox. */
void CKeyChooserWidget::slotReturnPressed( const QString& text){
	for (int index=0; index < comboBox()->count(); index++) {
		if (comboBox()->text(index) == text) {
			if (!oldKey.isNull() && text != oldKey)	//if the key has changed
				emit changed(index);
			break;
		}
	}
}

/** Is called when the current item of the combo box was changed. */
void CKeyChooserWidget::slotComboChanged(int index){
//	qDebug("CKeyChooserWidget::slotComboChanged(int index)");
	if (!isUpdatesEnabled())
		return;
	setUpdatesEnabled(false);	
	
	const QString key = comboBox()->text( index );
	if (oldKey.isNull() || (oldKey != key))
		emit changed(index);
	oldKey = key;

	setUpdatesEnabled(true);		
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
	setUpdatesEnabled(false);
	QSize s = sizeHint();
	if (s.width() > maximumWidth())
		s.setWidth( maximumWidth() );
	
	resize(s);
	setUpdatesEnabled(true);	
}

/** Sets the tooltips for the given entries using the parameters as text. */
void CKeyChooserWidget::setToolTips( const QString comboTip, const QString nextEntryTip, const QString scrollButtonTip, const QString previousEntryTip){
	QToolTip::add(comboBox(),comboTip);
	QToolTip::add(btn_fx,  scrollButtonTip);

 	if ( CBTConfig::get(CBTConfig::scroll) ){
		QToolTip::add(btn_down, nextEntryTip);
		QToolTip::add(btn_up,	previousEntryTip);
	}
	else {
		QToolTip::add(btn_up,   nextEntryTip);
		QToolTip::add(btn_down, previousEntryTip);
	}
}

/** No descriptions */
void CKeyChooserWidget::setWhatsThis(const QString comboTip, const QString nextEntryTip, const QString scrollButtonTip, const QString previousEntryTip){
	QWhatsThis::add(comboBox(),comboTip);
	QWhatsThis::add(btn_fx,  scrollButtonTip);

 	if ( CBTConfig::get(CBTConfig::scroll) ) {
		QWhatsThis::add(btn_down, nextEntryTip);
		QWhatsThis::add(btn_up,   previousEntryTip);
	}
	else {
		QWhatsThis::add(btn_up,   nextEntryTip);
		QWhatsThis::add(btn_down, previousEntryTip);
	}
}

/** Sets the current item to the one with the given text */
bool CKeyChooserWidget::setItem( const QString item ){
//	qDebug("CKeyChooserWidget::setItem( const QString item )");
	bool ret = false;
	const int count = comboBox()->count();
	for (int i = 0; i < count; ++i) {
		if (comboBox()->text(i) == item) {
			comboBox()->setCurrentItem(i);
			ret = true;
			break;
		}
	}
	if (!ret)
		comboBox()->setCurrentItem(-1);
	return ret;
}

/** Jump to the next entry. */
void CKeyChooserWidget::next(){
	if (comboBox()->currentItem() != comboBox()->count()-1) {// not last entry
		comboBox()->setCurrentItem( comboBox()->currentItem()+1 );
		emit changed(comboBox()->currentItem());
	}
}

/** Jump to the previous entry. */
void CKeyChooserWidget::previous(){
	if (comboBox()->currentItem() != 0) {// not last entry
		comboBox()->setCurrentItem( comboBox()->currentItem()-1 );
		emit changed(comboBox()->currentItem());
	}
}

QComboBox* CKeyChooserWidget::comboBox(){
	return m_comboBox;
}

