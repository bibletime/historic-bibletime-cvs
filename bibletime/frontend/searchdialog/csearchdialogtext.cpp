/***************************************************************************
                       csearchdialogtext.cpp  -  description
                             -------------------
    begin                : Mit Jan 19 12:57:15 CET 2000
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


//BibleTime includes
#include "csearchdialogtext.h"
#include "csearchdialogscope.h"
#include "../../whatsthisdef.h"
#include "../../tooltipdef.h"
#include "../../backend/cswordmodulesearch.h"
#include "../cbtconfig.h"

//QT includes
#include <qpixmap.h>
#include <qlayout.h>
#include <qvbuttongroup.h>
#include <qhbuttongroup.h>
#include <qradiobutton.h>
#include <qcheckbox.h>
#include <qlabel.h>
#include <qgroupbox.h>
#include <qhbox.h>
#include <qvbox.h>
#include <qcombobox.h>
#include <qtooltip.h>
#include <qwhatsthis.h>

//KDE includes
#include <kcombobox.h>
#include <kiconloader.h>
#include <kprogress.h>
#include <klocale.h>


CSearchDialogText::CSearchDialogText(QWidget *parent, const char *name)
	: QWidget(parent, name, 48)
{
	/* the first main Group - search text, options etc.*/
	QGroupBox *textBox = new QGroupBox(2,Qt::Vertical,i18n("Search properties"),this,"textBox");

	m_editCombo = new KHistoryCombo(textBox);
	m_editCombo->setInsertionPolicy(QComboBox::AtTop);	
	m_editCombo->setMaxCount(25);		
	m_editCombo->setDuplicatesEnabled(false);				
	m_editCombo->setFocusPolicy(QWidget::StrongFocus);
	m_editCombo->setFocus();
	connect( m_editCombo, SIGNAL( activated( const QString& )),	m_editCombo, SLOT( addToHistory( const QString& )));
	connect( m_editCombo, SIGNAL(returnPressed ( const QString& )),m_editCombo,  SLOT(addToHistory(const QString&)) );
	
	QToolTip::add(m_editCombo, TT_SD_SEARCH_TEXT_EDIT);
	QWhatsThis::add(m_editCombo, WT_SD_SEARCH_TEXT_EDIT);
	
	QHBox *settingsBox = new QHBox(textBox,"settingsBox");
	QVBox *radioBox = new QVBox(settingsBox,"radioBox");

	m_radioMultipleWords = new QRadioButton( radioBox,"RadioButton_1");
	m_radioMultipleWords->setFocusPolicy(QWidget::TabFocus);
	m_radioMultipleWords->setText( i18n( "Multiple Words" ) );
//	m_radioMultipleWords->setAutoResize( true );
	m_radioMultipleWords->setChecked( true );
	QToolTip::add(m_radioMultipleWords, TT_SD_SEARCH_MULTIPLE_WORDS);
	QWhatsThis::add(m_radioMultipleWords, WT_SD_SEARCH_MULTIPLE_WORDS);	
	
	m_radioExactSearch = new QRadioButton(radioBox, "RadioButton_2");
	m_radioExactSearch->setFocusPolicy(QWidget::TabFocus);
	m_radioExactSearch->setText( i18n( "Exact Search" ) );
//	m_radioExactSearch->setAutoResize( true );
	QToolTip::add(m_radioExactSearch, TT_SD_SEARCH_EXACT_MATCH);
	QWhatsThis::add(m_radioExactSearch, WT_SD_SEARCH_EXACT_MATCH);	

	m_radioRegularExpression = new QRadioButton(radioBox, "RadioButton_3");
	m_radioRegularExpression->setFocusPolicy(QWidget::TabFocus);
	m_radioRegularExpression->setText( i18n( "Regular Expression" ) );
//	m_radioRegularExpression->setAutoResize( true );
	QToolTip::add(m_radioRegularExpression, TT_SD_SEARCH_REGEXP );
	QWhatsThis::add(m_radioRegularExpression, WT_SD_SEARCH_REGEXP );
	
	/* this widget is hidden - it is only used for exclusive radio button management*/
	QVButtonGroup *bgroup1 = new QVButtonGroup();
	bgroup1->insert(m_radioMultipleWords); //they are automatically exclusive
	bgroup1->insert(m_radioExactSearch);
	bgroup1->insert(m_radioRegularExpression);

	m_checkCaseSensitive = new QCheckBox(settingsBox, "CheckBox_1");
	m_checkCaseSensitive->setFocusPolicy(QWidget::TabFocus);
	m_checkCaseSensitive->setText( i18n("Case Sensitive") );
	QToolTip::add(m_checkCaseSensitive, TT_SD_SEARCH_CASE_SENSITIVE);
	QWhatsThis::add(m_checkCaseSensitive, WT_SD_SEARCH_CASE_SENSITIVE);
	
  QGroupBox *scopeBox = new QGroupBox(2,Qt::Horizontal,i18n("Search scope"),this,"scopeBox");
  scopeChooser = new CSearchDialogScopeChooser(scopeBox, "scopeChooser");
	
	/*the third main groupbox - progress indication */
	QGroupBox *progressBox = new QGroupBox(2,Qt::Horizontal,i18n("Search progress"),this,"progressBox");

	/* manages the 2 buttons (vertically)*/
	QVBox *buttonBox = new QVBox(progressBox,"currentBox");

	(void)new QLabel(m_editCombo,i18n("Current module:"),buttonBox);
	(void)new QLabel(m_editCombo,i18n("Overall:"),buttonBox);

	/* manages the 2 bars (vertically) */
	QVBox *barBox = new QVBox(progressBox,"overallBox");
	m_currentProgressBar = new KProgress(barBox, "m_currentProgressBar",0);
	m_overallProgressBar = new KProgress(barBox, "m_overallProgressBar",0);
//	m_currentProgressBar = new KProgress(0,100,0,Horizontal,barBox, "m_currentProgressBar");
//	m_overallProgressBar = new KProgress(0,100,0,Horizontal,barBox, "m_overallProgressBar");

	/* the main layout for the 3 groupboxes (vertical)*/
	QVBoxLayout* layout_1 = new QVBoxLayout( this );
	layout_1->setAutoAdd(false);
	
	textBox->setFixedHeight( textBox->sizeHint().height() );	
	layout_1->addWidget( textBox,1, Qt::AlignTop );
	
	layout_1->addWidget( scopeBox, Qt::AlignVCenter);
	scopeBox->setFixedHeight( scopeBox->sizeHint().height() );	
	
	progressBox->setFixedHeight( progressBox->sizeHint().height() );	
	layout_1->addWidget( progressBox,1,Qt::AlignBottom );
	
		
	readSettings();
}

CSearchDialogText::~CSearchDialogText() {
	saveSettings();
}
	
const QString CSearchDialogText::getText() const {
	return m_editCombo->currentText();
}

void CSearchDialogText::setText(const QString text){
	//only insert the item to the list if it's not yet included
	bool found = false;
	for (int i = 0; !found && i < m_editCombo->count(); ++i) {
		if (m_editCombo->text(i) == text)
			found = true;
	}
	if (!found) {
		m_editCombo->insertItem(text,0);
		m_editCombo->setCurrentItem(0);
	}
	m_editCombo->setFocus();	
}

const bool CSearchDialogText::isCaseSensitive() {
	return m_checkCaseSensitive->isChecked();
}

const int CSearchDialogText::getSearchType() {
	int ret = CSwordModuleSearch::multipleWords;	//"multiple words" is standard
	if (m_radioExactSearch->isChecked()) {
		ret = CSwordModuleSearch::exactPhrase;
	}
	else if (m_radioRegularExpression->isChecked()) {
		ret = CSwordModuleSearch::regExp;
	}
	return ret;
}

const QString CSearchDialogText::getSearchTypeString() {
	QString ret = m_radioMultipleWords->text();	//"multiple words" is standard
	if (m_radioExactSearch->isChecked()) {
		ret = m_radioExactSearch->text();
	}
	else if (m_radioRegularExpression->isChecked()) {
		ret = m_radioRegularExpression->text();
	}
	return ret;

}
void CSearchDialogText::updateCurrentProgress(const unsigned short int percent){
	m_currentProgressBar->setValue(percent);
}

void CSearchDialogText::updateOverallProgress(const unsigned short int percent){
	m_overallProgressBar->setValue(percent);
}

/** resets the widgets. */
void CSearchDialogText::reset(){
	m_currentProgressBar->setValue(0);
	m_overallProgressBar->setValue(0);
	m_editCombo->clear();
}

/** Reads settings to restore the last used state. */
void CSearchDialogText::readSettings(){
	QStringList list = CBTConfig::get( CBTConfig::searchCompletionTexts );
	m_editCombo->completionObject()->setItems( list );
	
	list = CBTConfig::get(CBTConfig::searchTexts);
	m_editCombo->setHistoryItems( list );
}

void CSearchDialogText::saveSettings(){
	QStringList list = m_editCombo->completionObject()->items();
	CBTConfig::set(CBTConfig::searchCompletionTexts, list);
	
	list = m_editCombo->historyItems();
	CBTConfig::set(CBTConfig::searchTexts, list);	
}
