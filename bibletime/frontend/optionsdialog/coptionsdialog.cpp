/***************************************************************************
                          coptionsdialog.cpp  -  description
                             -------------------
    begin                : Wed Jan 19 2000
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

#include "coptionsdialog.h"
#include "../presenters/cmodulepresenter.h"
#include "../../ressource.h"
#include "../../whatsthisdef.h"
#include "../../backend/sword_backend/cswordlexiconmoduleinfo.h"
#include "../../backend/sword_backend/cswordbackend.h"

#include <stdio.h>

//KDE includes
#include <kapp.h>
#include <klocale.h>
#include <kglobal.h>
#include <kconfig.h>
#include <kkeydialog.h>
#include <kiconloader.h>
#include <kconfig.h>
#include <kapp.h>
#include <klistbox.h>
#include <kkeydialog.h>

//QT includes
#include <qhbox.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qdict.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qradiobutton.h>
#include <qcolor.h>
#include <qwhatsthis.h>

//Sword includes
#include <localemgr.h>

COptionsDialog::COptionsDialog(CImportantClasses* importantClasses, QWidget *parent, const char *name, KAccel* accel )
	: KDialogBase(IconList, i18n("Optionsdialog"), Ok | Apply | Cancel, Ok, parent, name, true, true,
	QString::null, QString::null, QString::null) {

	config = KGlobal::config();	
	m_important = importantClasses;
	m_changedSettings = 0;
	
	initGeneralPage();		
	initKeyPage(accel);
	initFontPage();
	initFontManagerPage();
	initColorsPage();	
}

COptionsDialog::~COptionsDialog(){
}

/** Initialize the view */
void COptionsDialog::initKeyPage(KAccel* accel){
	key_accel = accel;
	keyaccel_page = addHBoxPage(i18n("Accelerators"), i18n("Configure BibleTime's key bindings"), OD_ICON_KEY_BINDINGS);

 	dict = key_accel->keyDict();

 	KKeyChooser* keyChooser = new KKeyChooser( &dict, keyaccel_page, false );	
 	QWhatsThis::add(keyChooser, i18n("<B>Key chooser</B><BR><BR>Select a action and change the key for the action.") );
}

void COptionsDialog::initFontPage(){
 	font_page = addVBoxPage(i18n("Fonts"), i18n("Select BibleTime fonts"), OD_ICON_FONTS); 	
	KConfigGroupSaver groupSaver(config, "Fonts");

  QFont dummy = QFont::defaultFont();
  currentFonts[0] = config->readFontEntry("Presenter", &dummy );
//  currentFonts[2] = config->readFontEntry("Presenter Fixed");
//  currentFonts[3] = config->readFontEntry("Printer");
//  currentFonts[4] = config->readFontEntry("Printer Fixed");

 	presenterFont = config->readFontEntry("Presenter");
	fontButtonGroup	=	new QButtonGroup (2,Qt::Horizontal,i18n("Usage"), font_page, "fontGroup");
	
	font1vButton = new QRadioButton(i18n("Presenter"), fontButtonGroup);
	QWhatsThis::add(font1vButton, WT_OD_FONTS_PRESENTER );
	
//	font1fButton = new QRadioButton(i18n("Presenter Fixed"), fontButtonGroup);
//	QWhatsThis::add(font1fButton, WT_OD_FONTS_PRESENTER_FIXED );	
//	
//	font2vButton = new QRadioButton(i18n("Printer"), fontButtonGroup);
//	QWhatsThis::add(font2vButton, WT_OD_FONTS_PRINTING );	
//	
//	font2fButton = new QRadioButton(i18n("Printer Fixed"), fontButtonGroup);
//	QWhatsThis::add(font2fButton, WT_OD_FONTS_PRINTING_FIXED );	
		
	fontButtonGroup->insert( font1vButton, 1);
//	fontButtonGroup->insert( font1fButton, 2);
//	fontButtonGroup->insert( font2vButton, 3);
//	fontButtonGroup->insert( font2fButton, 4);
	fontButtonGroup->setButton(1);
	
	fonts = new KFontChooser(font_page, "fonts", false, QStringList(), true, 6);
	fonts->setSampleText(i18n("The quick brown fox jumped over the lazy dog"));
	fonts->setFont(currentFonts[0], false);
// 	QWhatsThis::add(fonts, i18n("<B>Key chooser</B><BR><BR>Select a action and change the key for the action.") ); 		
 		
	currentFont = 0;
  connect(fontButtonGroup, SIGNAL(clicked(int)), SLOT(fontButtonClicked(int)));
  connect(fonts, SIGNAL(fontSelected(const QFont &)), SLOT(newFontSelected(const QFont &)));

	newFontSelected(presenterFont);
}

void COptionsDialog::initFontManagerPage(){
	foreign_font_page = this->addVBoxPage(i18n("Font Manager"), i18n("Configure BibleTime's foreign fonts"), DesktopIcon("kfontmanager"));	
	
	fontModuleList = new KListBox(foreign_font_page);
 	QWhatsThis::add(fontModuleList, WT_OD_FFM_FONTS_LIST );		
	foreignFonts = new KFontChooser(foreign_font_page, "foreignFonts", false, QStringList(), true, 6);
 	
  connect(foreignFonts, SIGNAL(fontSelected(const QFont &)), SLOT(newForeignFontSelected(const QFont &)));	
  connect(fontModuleList, SIGNAL(selectionChanged(QListBoxItem*)), SLOT(foreignFontModuleChanged(QListBoxItem*)));		

	foreign_font_page->setEnabled( true );
	
	ListCSwordModuleInfo* mainlist = m_important->swordBackend->getModuleList();

	//Build a list of modules with foreign fonts
	for (mainlist->first(); mainlist->current(); mainlist->next())
		if (mainlist->current()->hasFont())
		  fontModuleList->insertItem( mainlist->current()->getDescription() );
	if (fontModuleList->count() > 0)
		fontModuleList->setCurrentItem(0);

	
}

/** Initialize page to set colors for use in Bibletime */
void COptionsDialog::initColorsPage(){
	colors_page = addHBoxPage(i18n("Colors"), i18n("Configure BibleTime's colors"), OD_ICON_COLORS);	
	colorGroup	=	new QButtonGroup (2,Qt::Horizontal,i18n("Choose colors"), colors_page, "colorGroup");
	
	KConfigGroupSaver groupSaver(config, "Colors");
	
	QLabel* label;
	label = new QLabel(i18n("Background"), colorGroup);
	QWhatsThis::add(label, WT_OD_COLORS_BACKGROUND );	
	backgroundButton = new KColorButton(config->readColorEntry("Background", &Qt::lightGray), colorGroup);
	
	label = new QLabel(i18n("Normal Text"), colorGroup);
	QWhatsThis::add(label, WT_OD_COLORS_NORMAL_TEXT );
	normalTextButton = new KColorButton(config->readColorEntry("Normal Text", &Qt::black), colorGroup);
	
	label = new QLabel(i18n("Verse number / URL"), colorGroup);
	QWhatsThis::add(label, WT_OD_COLORS_VERSENUMBER );
	URLLinkButton = new KColorButton(config->readColorEntry("Versenumber/URL", &Qt::darkBlue), colorGroup);

	label = new QLabel(i18n("Highlighted Verse"), colorGroup);
	QWhatsThis::add(label, WT_OD_COLORS_CURRENT_VERSE );
	highlightedVerseButton = new KColorButton(config->readColorEntry("Highlighted Verse", &Qt::red ), colorGroup);
}

/** Initialize General page */
void COptionsDialog::initGeneralPage(){
	general_page = addPage(i18n("General"), i18n("Configure BibleTime's general options"), OD_ICON_GENERAL);
  QVBoxLayout*	main_layout = new QVBoxLayout(general_page);
			
	tipCheckBox = new QCheckBox(general_page);
	tipCheckBox->setText(i18n("Show tip of the day"));
	QWhatsThis::add(tipCheckBox, WT_OD_GENERAL_DAILY_TIP );

	{
		KConfigGroupSaver groupSaver(config, "Daily tip");
		tipCheckBox->setChecked( config->readBoolEntry("TipsOnStart", true) );
	}

	{
		KConfigGroupSaver groupSaver(config, "SWORD");
		localeCheckBox = new QCheckBox(general_page);
		localeCheckBox->setText(i18n("Use international booknames"));
		QWhatsThis::add(localeCheckBox, WT_OD_GENERAL_INTERNATIONAL_BOOKNAMES );
		
		localeComboBox = new QComboBox(general_page);
		localeCheckBox->setChecked(config->readBoolEntry("useLocalisation", true));
		localeComboBox->setEnabled(config->readBoolEntry("useLocalisation", true));
  		connect(localeCheckBox, SIGNAL(stateChanged(int)), SLOT(localeStateChanged(int)));		  			
		QWhatsThis::add(localeComboBox, WT_OD_GENERAL_INTERNATIONAL_BOOKNAMES_LIST );	
			
		list <string> locales = LocaleMgr::systemLocaleMgr.getAvailableLocales();
		for (list <string>::iterator it = locales.begin(); it != locales.end(); it++) {
			localeComboBox->insertItem( LocaleMgr::systemLocaleMgr.getLocale((*it).c_str())->getDescription() );
		}

		int current_item = -1;
		qDebug("restore locale");
		for(int test_item = 0; test_item < localeComboBox->count(); test_item++) {
			SWLocale* locale = LocaleMgr::systemLocaleMgr.getLocale((const char*)config->readEntry("Language", QString::null).local8Bit());
			if (locale && localeComboBox->text(test_item).contains(locale->getDescription()) )
				current_item = test_item;
		}
		if (current_item!=-1)
			localeComboBox->setCurrentItem(current_item);
	}
	
	{
		KConfigGroupSaver groupSaver(config, "General");  	
		logoCheckBox = new QCheckBox(general_page);
		logoCheckBox->setText(i18n("Show startuplogo"));
		logoCheckBox->setChecked(config->readBoolEntry("Logo", true));	
		QWhatsThis::add(logoCheckBox, WT_OD_GENERAL_SHOW_STARTUPLOGO );	
	}
	
	main_layout->addWidget(tipCheckBox);
	main_layout->addWidget(logoCheckBox);
	main_layout->addWidget(localeCheckBox);
	main_layout->addWidget(localeComboBox);	
	main_layout->addStretch(4);
}

/**  */
void COptionsDialog::localeStateChanged(int state){
	switch (state) {
		case 2:
			localeComboBox->setEnabled(true);
			break;
		case 0:
			localeComboBox->setEnabled(false);		
			break;
	}
}

/**  */
void COptionsDialog::newFontSelected(const QFont &newFont){
	currentFonts[currentFont] = newFont;
}

/**  */
void COptionsDialog::fontButtonClicked(int fontChosen){
//	if ((fontChosen == 2) || (fontChosen == 4)) {
//	   fixedFonts = true;
//	} else {
//	   fixedFonts = false;
//	}
	currentFonts[currentFont] = fonts->font();
	fonts->setFont(currentFonts[fontChosen-1], false);
	currentFont = fontChosen-1;
}


/** Called if the OK button was clicked */
void COptionsDialog::slotOk(){
	qDebug("COptionsDialog::slotOk");
	saveGeneralOptions();
	saveFontOptions();
	saveKeyOptions();
	saveColorsOptions();
	
	KDialogBase::slotOk();
}

/**  */
void COptionsDialog::saveGeneralOptions(){
	{
		KConfigGroupSaver groupSaver(config, "Daily tip");
		config->writeEntry("TipsOnStart", tipCheckBox->isChecked());
	}
	
	{
		KConfigGroupSaver groupSaver(config, "General");
		config->writeEntry("Logo", logoCheckBox->isChecked());
	}

	//Now save localisation settings
	{
		KConfigGroupSaver groupSaver(config, "SWORD");
		config->writeEntry("useLocalisation",localeCheckBox->isChecked());	
	
		const QString currentText = (localeCheckBox->isChecked()) ? localeComboBox->currentText() : QString("");
		const QString oldValue = config->readEntry("Language", QString::null);	//default nonexisting language
		if (oldValue == QString::null || oldValue != currentText) {	//changed
			if (m_changedSettings)
				m_changedSettings |= CModulePresenter::language;
			else
				m_changedSettings = CModulePresenter::language;
		}
		
		list <string> locales = LocaleMgr::systemLocaleMgr.getAvailableLocales();
		QString localeName = QString::null;
		for (list <string>::iterator it = locales.begin(); it != locales.end(); it++) {
			if ( LocaleMgr::systemLocaleMgr.getLocale((*it).c_str())->getDescription() == currentText ) {
				localeName = (*it).c_str();	//we found the abbrevation for the current language
				break;
			}
		}
		if (!localeName.isEmpty())
			config->writeEntry("Language", localeName);
		else
			config->writeEntry("Language", currentText);
	}
}

/** Used by optionsdialog to save font settings for presenters and printing */
void COptionsDialog::saveFontOptions(){
	KConfigGroupSaver groupSaver(config, "Fonts");
	if (config->readFontEntry("Presenter").family() != currentFonts[0].family() || config->readFontEntry("Presenter").pointSize() != currentFonts[0].pointSize() )
	{
		if (m_changedSettings)
			m_changedSettings |= CModulePresenter::font;
		else
			m_changedSettings = CModulePresenter::font;
	}
	config->writeEntry("Presenter", currentFonts[0]);
//	config->writeEntry("Presenter Fixed", currentFonts[2]);
 //	config->writeEntry("Printer", currentFonts[3]);
 //	config->writeEntry("Printer Fixed", currentFonts[4]);
}
/**  */
void COptionsDialog::saveKeyOptions(){
	KConfigGroupSaver groupSaver(config, "Keys");	
	key_accel->setKeyDict( dict );	
	key_accel->writeSettings(config);
}

/**  */
void COptionsDialog::saveColorsOptions() {
	KConfigGroupSaver groupSaver(config, "Colors");
	if ( config->readColorEntry("Background") != backgroundButton->color() ) {
		if (m_changedSettings)
			m_changedSettings |= CModulePresenter::backgroundColor;
		else
			m_changedSettings = CModulePresenter::backgroundColor;
	}	
	config->writeEntry("Background", backgroundButton->color().name());	
	
	if ( config->readColorEntry("Normal Text") != normalTextButton->color() ) {
		if (m_changedSettings)
			m_changedSettings |= CModulePresenter::textColor;
		else
			m_changedSettings = CModulePresenter::textColor;
	}		
	config->writeEntry("Normal Text", normalTextButton->color().name());	
	
	if ( config->readColorEntry("Versenumber/URL") != URLLinkButton->color() ) {
		if (m_changedSettings)
			m_changedSettings |= CModulePresenter::verseNumberColor;
		else
			m_changedSettings = CModulePresenter::verseNumberColor;
	}	
	config->writeEntry("Versenumber/URL", URLLinkButton->color().name());
	
	if ( config->readColorEntry("Highlighted Verse") != highlightedVerseButton->color() ) {
		if (m_changedSettings)
			m_changedSettings |= CModulePresenter::highlightedVerseColor;
		else
			m_changedSettings = CModulePresenter::highlightedVerseColor;
	}		
	config->writeEntry("Highlighted Verse", highlightedVerseButton->color().name());
}

/**  */
void COptionsDialog::slotApply(){
	saveGeneralOptions();
	saveFontOptions();
	saveKeyOptions();
	saveColorsOptions();
	
	KDialogBase::slotApply();
}

/** Is called when a new font was selected in the  foreign font manager dialog. */
void COptionsDialog::newForeignFontSelected( const QFont& font){
	qDebug("COptionsDialog::newForeignFontSelected()");
	QString selectedModule = fontModuleList->currentText();
	
	ListCSwordModuleInfo* mainlist = m_important->swordBackend->getModuleList();

	//Build a list of modules with foreign fonts
	for (mainlist->first(); mainlist->current(); mainlist->next())
		if (mainlist->current()->getDescription() == selectedModule)
      mainlist->current()->setFont(font);
}

/** Is called when the user select a new module in te foreign font management dialog. */
void COptionsDialog::foreignFontModuleChanged( QListBoxItem* item) {
	qDebug("COptionsDialog::foreignFontModuleChaged()");
	QString selectedModule = item->text();	//selected modules
	
	ListCSwordModuleInfo* mainlist = m_important->swordBackend->getModuleList();
	//Build a list of modules with foreign fonts
	for (mainlist->first(); mainlist->current(); mainlist->next()) {
		if (mainlist->current()->getDescription() == selectedModule)
      foreignFonts->setFont(mainlist->current()->getFont());
	}
}

/** Returns an integer with ORed feature enum entries of the changed settings. */
int COptionsDialog::getChangedSettings() {
	qDebug("COptionsDialog::getChangedSettings() ");
	qDebug((const char*)QString::number(m_changedSettings).local8Bit());
	return m_changedSettings;
}
