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
#include "cprofile.h"
#include "ctoolclass.h"
#include "presenters/cswordpresenter.h"
#include "presenters/cbiblepresenter.h"
#include "presenters/ccommentarypresenter.h"
#include "presenters/clexiconpresenter.h"
#include "../ressource.h"
#include "../whatsthisdef.h"
#include "../backend/cswordbackend.h"
#include "../backend/cswordmoduleinfo.h"
#include "cbtconfig.h"

#include <stdio.h>
#include <stdlib.h>

//QT includes
#include <qhbox.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qdict.h>
#include <qcheckbox.h>
#include <qbuttongroup.h>
#include <qhbuttongroup.h>
#include <qradiobutton.h>
#include <qcolor.h>
#include <qtooltip.h>
#include <qwhatsthis.h>
#include <qstringlist.h>
#include <qinputdialog.h>
#include <qdir.h>


//KDE includes
#include <kapp.h>
#include <klocale.h>
#include <kglobal.h>
#include <kstddirs.h>
#include <kkeydialog.h>
#include <kiconloader.h>
#include <ktabctl.h>
#include <kapp.h>
#include <klistbox.h>
#include <kkeydialog.h>

//Sword includes
#include <localemgr.h>

COptionsDialog::COptionsDialog(CImportantClasses* importantClasses, QWidget *parent, const char *name, KAccel* accel )
	: KDialogBase(IconList, i18n("Optionsdialog"), Ok | Cancel | Apply, Ok, parent, name, true, true, QString::null, QString::null, QString::null) {

	m_important = importantClasses;
	m_settings.keys.application.accel = accel;

	initStartup();		
	initFonts();
	initColors();	
	initProfiles();
	initSword();
	initAccelerators();
}

/**  */
void COptionsDialog::newDisplayWindowFontSelected(const QFont &newFont){
	m_settings.fonts.fontMap.replace(m_settings.fonts.usage->currentText(), newFont);		
}

/** Called when the combobox contents is changed */
void COptionsDialog::newDisplayWindowFontAreaSelected(const QString& usage){
	m_settings.fonts.fontChooser->setFont( m_settings.fonts.fontMap[usage] );
}

/** Called if the OK button was clicked */
void COptionsDialog::slotOk(){
	saveAccelerators();
	saveColors();
	saveFonts();
	saveSword();
	saveStartup();
	saveProfiles();
	
	KDialogBase::slotOk();

  emit signalSettingsChanged( );
}

/*called if the apply button was clicked*/
void COptionsDialog::slotApply(){
	saveAccelerators();
	saveColors();
	saveFonts();
	saveSword();
	saveStartup();
	saveProfiles();
	
	KDialogBase::slotApply();

  emit signalSettingsChanged( );
}

/** Adds a new view profile to the list. */
void COptionsDialog::addNewProfile(){
	bool ok = false;
	QString name = QInputDialog::getText(i18n("Create new profile"), i18n("Please enter the name of the new profile"), QString::null, &ok);
	if (ok && !name.isEmpty()) {
		m_settings.profiles.mgr.create(name);
		m_settings.profiles.profiles->insertItem(name);				
		if (m_settings.profiles.profiles->count() == 1)
			m_settings.profiles.profiles->setEnabled(true);		
	}
}

/** No descriptions */
void COptionsDialog::deleteProfile(){
	const QString profile = m_settings.profiles.profiles->currentText();
	m_settings.profiles.mgr.remove(profile);
	m_settings.profiles.profiles->removeItem( m_settings.profiles.profiles->currentItem() );
}

/** Renames the currently selected profile. */
void COptionsDialog::renameProfile(){
	bool ok = false;
	const QString currentProfile = m_settings.profiles.profiles->currentText();	
	CProfile* profile = m_settings.profiles.mgr.profile(currentProfile);
	if (!profile)
		return;
		
	const QString newName = QInputDialog::getText(i18n("Create new profile"), i18n("Please enter the new name of the profile"),profile->name(), &ok);
	if (ok && !newName.isEmpty()) {
		profile->setName(newName);
		m_settings.profiles.profiles->changeItem(newName, m_settings.profiles.profiles->currentItem());
	}	
}

/** Opens the page which contaisn the given part ID. */
const bool COptionsDialog::showPart(  COptionsDialog::Parts ID ){
#warning "Add the necessary case statements"
	switch (ID) {
		case COptionsDialog::ViewProfiles:
			showPage( pageIndex(m_settings.profiles.profiles->parentWidget()) );
			break;
		default:
			break;
	}
}

/** Initializes the startup section of the OD. */
void COptionsDialog::initStartup(){
	QFrame* page = addPage(i18n("Startup"), QString::null, OD_ICON_GENERAL);
	QVBoxLayout* layout = new QVBoxLayout(page,5);
	{//daily tips
		m_settings.startup.showTips = new QCheckBox(page);
		m_settings.startup.showTips->setText(i18n("Show tip of the day"));
		QToolTip::add(m_settings.startup.showTips, TT_OD_GENERAL_DAILY_TIP);	
		QWhatsThis::add(m_settings.startup.showTips, WT_OD_GENERAL_DAILY_TIP);
		
		m_settings.startup.showTips->setChecked( CBTConfig::get(CBTConfig::tips) );
	}
	layout->addWidget(m_settings.startup.showTips);
	
	{ //startup logo
		m_settings.startup.showLogo = new QCheckBox(page);
		m_settings.startup.showLogo->setText(i18n("Show startuplogo"));
		QWhatsThis::add(m_settings.startup.showLogo, WT_OD_GENERAL_SHOW_STARTUPLOGO);
		QToolTip::add(m_settings.startup.showLogo, TT_OD_GENERAL_SHOW_STARTUPLOGO);		
		
		m_settings.startup.showLogo->setChecked(CBTConfig::get(CBTConfig::logo));			
	}		
	layout->addWidget(m_settings.startup.showLogo);
	{ //workspace
		m_settings.startup.restoreWorkspace = new QCheckBox(page);
		m_settings.startup.restoreWorkspace->setText(i18n("Restore windows in workspace area"));
		QToolTip::add(m_settings.startup.restoreWorkspace, TT_OD_GENERAL_RESTORE_WORKSPACE);		
		QWhatsThis::add(m_settings.startup.restoreWorkspace, WT_OD_GENERAL_RESTORE_WORKSPACE);
		
		m_settings.startup.restoreWorkspace->setChecked(CBTConfig::get(CBTConfig::restoreWorkspace));			
	}		
	layout->addWidget(m_settings.startup.restoreWorkspace);			
	
	layout->addStretch(4);
}

/** Init fonts section. */
void COptionsDialog::initFonts(){
	QFrame* page = addPage(i18n("Fonts"), QString::null, OD_ICON_GENERAL);
	QVBoxLayout* layout = new QVBoxLayout(page,5);

 	layout->addWidget( CToolClass::explanationLabel(page, i18n("Fonts"), i18n("The fonts you can choose here are used in the display windows. Use drop-down box below to choose the area of application. Then select a font for it.<BR>Don't forget to choose the right charset, for example \"iso10646-1\" for unicode fonts!")) );
  layout->addSpacing(5);
 					
 	m_settings.fonts.usage = new QComboBox(page);		
 	QToolTip::add(m_settings.fonts.usage, TT_OD_FONTS_TYPE_CHOOSER);	 	
 	QWhatsThis::add(m_settings.fonts.usage, WT_OD_FONTS_TYPE_CHOOSER);	
 	layout->addWidget(m_settings.fonts.usage);
	 	
 	m_settings.fonts.fontMap.insert(i18n("Standard"), CBTConfig::get(CBTConfig::standard));
 	m_settings.fonts.fontMap.insert(i18n("Unicode"), CBTConfig::get(CBTConfig::unicode));

 	for( QMap<QString, QFont>::Iterator it = m_settings.fonts.fontMap.begin(); it != m_settings.fonts.fontMap.end(); ++it )
 		m_settings.fonts.usage->insertItem(it.key());
		
 	m_settings.fonts.fontChooser = new KFontChooser(page, "fonts", false, QStringList(), true, 6);
 	m_settings.fonts.fontChooser->setSampleText(i18n("The quick brown fox jumps over the lazy dog"));
 	layout->addWidget(m_settings.fonts.fontChooser);
		
  connect(m_settings.fonts.fontChooser, SIGNAL(fontSelected(const QFont&)), SLOT(newDisplayWindowFontSelected(const QFont&)));
  connect(m_settings.fonts.usage, SIGNAL(activated(const QString&)), SLOT(newDisplayWindowFontAreaSelected(const QString&)));
 	QWhatsThis::add(m_settings.fonts.fontChooser, WT_OD_FONTS_CHOOSER);
		 	
 	m_settings.fonts.fontChooser->setFont( m_settings.fonts.fontMap[m_settings.fonts.usage->currentText()] );
 	m_settings.fonts.fontChooser->setMinimumSize(m_settings.fonts.fontChooser->sizeHint());		
}

/** Init color section. */
void COptionsDialog::initColors(){
	QFrame* page = addPage(i18n("Colors"), QString::null, OD_ICON_GENERAL);
	QVBoxLayout* layout = new QVBoxLayout(page);
	
  layout->addWidget( CToolClass::explanationLabel(page, i18n("Choose colors"), i18n("Choose the colors to change the look of the display windows to your own preferences. Some options like \"Words of Jesus\" do only apply to texts which support this special feature.")) );
  layout->addSpacing(4);
  	
  QButtonGroup* group =	new QButtonGroup(2,Qt::Horizontal,"", page, "colorGroup");
	group->setLineWidth(0);
	{
		QLabel* label = new QLabel(i18n("Text"), group);		
		m_settings.colors.text = new KColorButton(CBTConfig::get(CBTConfig::textColor), group);		

		label = new QLabel(i18n("Background"), group);		
//		QToolTip::add(label, TT_OD_COLORS_BACKGROUND );		
//		QWhatsThis::add(label, WT_OD_COLORS_BACKGROUND );	
		m_settings.colors.background = new KColorButton(CBTConfig::get(CBTConfig::backgroundColor), group);		
		
		label = new QLabel(i18n("Highlighted verse"), group);
//		QToolTip::add(label, TT_OD_COLORS_CURRENT_VERSE );	
//		QWhatsThis::add(label, WT_OD_COLORS_CURRENT_VERSE );
		m_settings.colors.highlightedVerse = new KColorButton(CBTConfig::get(CBTConfig::highlightedVerseColor), group);

		label = new QLabel(i18n("Hyperlinks"), group);		
		m_settings.colors.swordrefs = new KColorButton(CBTConfig::get(CBTConfig::swordRefColor), group);		

		label = new QLabel(i18n("Footnotes"), group);		
		m_settings.colors.footnotes = new KColorButton(CBTConfig::get(CBTConfig::footnotesColor), group);		

		label = new QLabel(i18n("Strong's numbers"), group);		
		m_settings.colors.strongs = new KColorButton(CBTConfig::get(CBTConfig::strongsColor), group);		

		label = new QLabel(i18n("Morphologic tags"), group);		
		m_settings.colors.morph = new KColorButton(CBTConfig::get(CBTConfig::morphsColor), group);		

		label = new QLabel(i18n("Words of Jesus"), group);		
		m_settings.colors.jesuswords = new KColorButton(CBTConfig::get(CBTConfig::jesuswordsColor), group);		
	}	
  layout->addWidget(group);
  layout->addStretch(4);
}

/** Init profiles section. */
void COptionsDialog::initProfiles(){
	QFrame* page = addPage(i18n("Profiles"),QString::null, OD_ICON_GENERAL);
	QVBoxLayout* layout = new QVBoxLayout(page,5);

	layout->addWidget(CToolClass::explanationLabel(page, i18n("Profiles"), i18n("Profiles define the appereance of the workspace area, for example which windows are open and which texts should displayed in these windows. Don't forget that new profiles only work after you've saved something in them.")));
	layout->addSpacing(5);
	m_settings.profiles.profiles = new QListBox(page);					
	layout->addWidget(m_settings.profiles.profiles);
	
	QList<CProfile> profiles = m_settings.profiles.mgr.profiles();
	if (profiles.count()) {
		for (CProfile* p = profiles.first(); p; p = profiles.next()) {
			m_settings.profiles.profiles->insertItem(p->name());
		}	
	}
	else {
		m_settings.profiles.profiles->setEnabled(false);
	}
	
	QHButtonGroup* buttonGroup = new QHButtonGroup(page);
	layout->addWidget(buttonGroup);	
	layout->addSpacing(5);
	
	m_settings.profiles.createProfile = new QPushButton(i18n("Create new profile"), buttonGroup);
	connect(m_settings.profiles.createProfile, SIGNAL(clicked()), SLOT(addNewProfile()));
	
	m_settings.profiles.deleteProfile = new QPushButton(i18n("Delete selected profile"), buttonGroup);	
	connect(m_settings.profiles.deleteProfile, SIGNAL(clicked()), SLOT(deleteProfile()));
		
	m_settings.profiles.renameProfile = new QPushButton(i18n("Rename selected profile"), buttonGroup);		
	connect(m_settings.profiles.renameProfile, SIGNAL(clicked()), SLOT(renameProfile()));
}

/** Init accel key section. */
void COptionsDialog::initAccelerators(){
	QVBox* page = addVBoxPage(i18n("Accelerators"),QString::null, OD_ICON_GENERAL);

  KTabCtl* tabCtl = new KTabCtl(page);
  QFrame* currentTab = new QVBox(tabCtl);
	currentTab->setMargin(3);	
  tabCtl->addTab(currentTab, i18n("Application wide"));

	m_settings.keys.application.dict = m_settings.keys.application.accel->keyDict();
 	m_settings.keys.application.keyChooser = new KKeyChooser( &m_settings.keys.application.dict, currentTab, false );	
 	QToolTip::add(m_settings.keys.application.keyChooser, TT_OD_KEYS_CHOOSER);
	QWhatsThis::add(m_settings.keys.application.keyChooser, WT_OD_KEYS_CHOOSER);	

// ----- new tab: All display windows ------ //
//	currentTab = new QVBox(tabCtl);
//	currentTab->setMargin(3);	
//	tabCtl->addTab(currentTab, i18n("All display windows"));
//	
//	m_settings.keys.general.accel = new KAccel(this); //delete in destructor
//	CSwordPresenter::insertKeyboardActions( m_settings.keys.general.accel );		
//	m_settings.keys.general.accel->readSettings();
// 	m_settings.keys.general.dict = m_settings.keys.general.accel->keyDict();
// 	m_settings.keys.general.keyChooser = new KKeyChooser( &m_settings.keys.general.dict, currentTab, false );	
// 	QToolTip::add(m_settings.keys.general.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_GENERAL);
//	QWhatsThis::add(m_settings.keys.general.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_GENERAL);

// ----- new tab: Bible windows ------ //
	currentTab = new QVBox(tabCtl);
	currentTab->setMargin(3);	
	tabCtl->addTab(currentTab, i18n("Bible windows"));

	m_settings.keys.bible.accel = new KAccel(this); //delete in destructor
	CBiblePresenter::insertKeyboardActions( m_settings.keys.bible.accel );
	m_settings.keys.bible.accel->readSettings();		
 	m_settings.keys.bible.dict = m_settings.keys.bible.accel->keyDict();
 	m_settings.keys.bible.keyChooser = new KKeyChooser( &m_settings.keys.bible.dict, currentTab, false );	
	QToolTip::add(m_settings.keys.bible.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_BIBLE);
	QWhatsThis::add(m_settings.keys.bible.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_BIBLE);

// ----- new tab: Commentary windows ------ //
	currentTab = new QVBox(tabCtl);
	currentTab->setMargin(3);	
	tabCtl->addTab(currentTab, i18n("Commentary windows"));

	m_settings.keys.commentary.accel = new KAccel(this); //delete in destructor
	CCommentaryPresenter::insertKeyboardActions( m_settings.keys.commentary.accel );		
	m_settings.keys.commentary.accel->readSettings();	
 	m_settings.keys.commentary.dict = m_settings.keys.commentary.accel->keyDict();
 	m_settings.keys.commentary.keyChooser = new KKeyChooser( &m_settings.keys.commentary.dict, currentTab, false );	
 	QToolTip::add(m_settings.keys.commentary.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_COMMENTARY);
	QWhatsThis::add(m_settings.keys.commentary.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_COMMENTARY);

// ----- new tab: Lexicon windows ------ //
	currentTab = new QVBox(tabCtl);
	currentTab->setMargin(3);	
	tabCtl->addTab(currentTab, i18n("Lexicon windows"));

	m_settings.keys.lexicon.accel = new KAccel(this); //delete in destructor
	CLexiconPresenter::insertKeyboardActions( m_settings.keys.lexicon.accel );		
	m_settings.keys.lexicon.accel->readSettings();	 	
 	m_settings.keys.lexicon.dict = m_settings.keys.lexicon.accel->keyDict(); 	
 	m_settings.keys.lexicon.keyChooser = new KKeyChooser( &m_settings.keys.lexicon.dict, currentTab, false );	
 	QToolTip::add(m_settings.keys.lexicon.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_LEXICON);
	QWhatsThis::add(m_settings.keys.lexicon.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_LEXICON);
					
}

/** Init Sword section. */
void COptionsDialog::initSword(){
	QVBox* page = addVBoxPage(i18n("Sword"),QString::null, OD_ICON_GENERAL);
  KTabCtl* tabCtl = new KTabCtl(page);
  QFrame* currentTab = new QFrame(tabCtl);
  tabCtl->addTab(currentTab, i18n("General"));

  QVBoxLayout* layout = new QVBoxLayout(currentTab,5);
	
	m_settings.sword.lexiconCache = new QCheckBox(currentTab);
	m_settings.sword.lexiconCache->setText(i18n("Create and use lexicon key cache"));
	QToolTip::add(m_settings.sword.lexiconCache, TT_OD_SWORD_USE_LEXICON_CACHE);	
	QWhatsThis::add(m_settings.sword.lexiconCache, WT_OD_SWORD_USE_LEXICON_CACHE);
		
	m_settings.sword.lexiconCache->setChecked( CBTConfig::get(CBTConfig::lexiconCache) );
 	layout->addWidget(m_settings.sword.lexiconCache);	

 		
 	m_settings.sword.useDownArrow = new QCheckBox(currentTab);
 	m_settings.sword.useDownArrow->setText(i18n("Use down arrow to scroll to next verse"));
 	m_settings.sword.useDownArrow->setChecked(CBTConfig::get(CBTConfig::scroll));		
 	QWhatsThis::add(m_settings.sword.useDownArrow, WT_OD_GENERAL_SCROLL_PREVIOUS);
 	QToolTip::add(m_settings.sword.useDownArrow, TT_OD_GENERAL_SCROLL_PREVIOUS);
 	layout->addWidget(m_settings.sword.useDownArrow);

		
 	QHBox* localeLayout = new QHBox(currentTab);		
 	QLabel* label = new QLabel(i18n("Language for booknames"), localeLayout); 	
 	m_settings.sword.localeCombo = new QComboBox(localeLayout);
 	QToolTip::add(m_settings.sword.localeCombo, TT_OD_GENERAL_INTERNATIONAL_BOOKNAMES);				
 	QWhatsThis::add(m_settings.sword.localeCombo, WT_OD_GENERAL_INTERNATIONAL_BOOKNAMES);		
									
 	m_settings.sword.localeCombo->insertItem( i18n("English") );
 	list <string> locales = LocaleMgr::systemLocaleMgr.getAvailableLocales();
 	for (list <string>::iterator it = locales.begin(); it != locales.end(); it++) {
 		m_settings.sword.localeCombo->insertItem( i18n(LocaleMgr::systemLocaleMgr.getLocale((*it).c_str())->getDescription()) );
 	}

 	int current_item = -1;
 	for(int test_item = 0; test_item < m_settings.sword.localeCombo->count(); test_item++) {
 		SWLocale* locale = LocaleMgr::systemLocaleMgr.getLocale(CBTConfig::get(CBTConfig::language).local8Bit());
 		if (locale && m_settings.sword.localeCombo->text(test_item).contains(i18n(locale->getDescription())) )
 			current_item = test_item;
 	}
 	if (current_item!=-1)
 		m_settings.sword.localeCombo->setCurrentItem(current_item);
 	layout->addWidget(localeLayout);					
 	

  layout->addStretch(4);


// ---------- new tab: Default modules -------- //
  currentTab = new QFrame(tabCtl);
  tabCtl->addTab(currentTab, i18n("Default modules"));
  layout = new QVBoxLayout(currentTab,5);

  layout->addWidget(
  	CToolClass::explanationLabel(currentTab, i18n("Default modules"), i18n("Default modules are used, when no module is specified. This may happen with references into modules like Bibles or Lexicons."))
  );
  layout->addSpacing(5);

 	QHBoxLayout* hBox = new QHBoxLayout();				
 	 	
 	m_settings.sword.standardBible = new QComboBox(currentTab);
  label = new QLabel(m_settings.sword.standardBible, i18n("Default Bible"), currentTab);
 	QToolTip::add(m_settings.sword.standardBible, TT_OD_SWORD_STANDARD_BIBLE);	
 	QWhatsThis::add(m_settings.sword.standardBible, WT_OD_SWORD_STANDARD_BIBLE);		
 	hBox->addWidget(label);
 	hBox->addWidget(m_settings.sword.standardBible);		
 	layout->addLayout(hBox);

 	hBox = new QHBoxLayout();				
 	m_settings.sword.standardCommentary = new QComboBox(currentTab);
 	label = new QLabel(m_settings.sword.standardCommentary, i18n("Default Commentary"), currentTab);
 	QToolTip::add(m_settings.sword.standardCommentary, TT_OD_SWORD_STANDARD_COMMENTARY);	
 	QWhatsThis::add(m_settings.sword.standardCommentary, WT_OD_SWORD_STANDARD_COMMENTARY);		
 	hBox->addWidget(label);
 	hBox->addWidget(m_settings.sword.standardCommentary);
 	layout->addLayout(hBox);

 	hBox = new QHBoxLayout();				
 	m_settings.sword.standardLexicon = new QComboBox(currentTab);
 	label = new QLabel(m_settings.sword.standardLexicon, i18n("Default Lexicon"), currentTab);
 	QToolTip::add(m_settings.sword.standardLexicon, TT_OD_SWORD_STANDARD_LEXICON);	
 	QWhatsThis::add(m_settings.sword.standardLexicon, WT_OD_SWORD_STANDARD_LEXICON);		
 	hBox->addWidget(label);
 	hBox->addWidget(m_settings.sword.standardLexicon);
 	layout->addLayout(hBox);
		
 	hBox = new QHBoxLayout();
 	m_settings.sword.standardHebrewStrong = new QComboBox(currentTab);
 	label = new QLabel(m_settings.sword.standardHebrewStrong, i18n("Default Hebrew Strong's Lexicon"), currentTab);
 	QToolTip::add(m_settings.sword.standardHebrewStrong, TT_OD_SWORD_STANDARD_HEBREW_STRONG);
 	QWhatsThis::add(m_settings.sword.standardHebrewStrong, WT_OD_SWORD_STANDARD_HEBREW_STRONG);
 	hBox->addWidget(label);
 	hBox->addWidget(m_settings.sword.standardHebrewStrong);
 	layout->addLayout(hBox);
		
 	hBox = new QHBoxLayout();
 	m_settings.sword.standardGreekStrong = new QComboBox(currentTab);
 	label = new QLabel(m_settings.sword.standardGreekStrong, i18n("Default Greek Strong's Lexicon"), currentTab);
 	QToolTip::add(m_settings.sword.standardGreekStrong, TT_OD_SWORD_STANDARD_GREEK_STRONG);
 	QWhatsThis::add(m_settings.sword.standardGreekStrong, WT_OD_SWORD_STANDARD_GREEK_STRONG);
 	hBox->addWidget(label);
 	hBox->addWidget(m_settings.sword.standardGreekStrong);
 	layout->addLayout(hBox);
		
 	hBox = new QHBoxLayout();
 	m_settings.sword.standardHebrewMorph = new QComboBox(currentTab);
 	label = new QLabel(m_settings.sword.standardHebrewMorph, i18n("Default Hebrew Morphological Lexicon"), currentTab);
 	QToolTip::add(m_settings.sword.standardHebrewMorph, TT_OD_SWORD_STANDARD_HEBREW_MORPH);
 	QWhatsThis::add(m_settings.sword.standardHebrewMorph, WT_OD_SWORD_STANDARD_HEBREW_MORPH);
 	hBox->addWidget(label);
 	hBox->addWidget(m_settings.sword.standardHebrewMorph);
 	layout->addLayout(hBox);

 	hBox = new QHBoxLayout();
 	m_settings.sword.standardGreekMorph = new QComboBox(currentTab);
 	label = new QLabel(m_settings.sword.standardGreekMorph, i18n("Default Greek Morphological Lexicon"), currentTab);
 	QToolTip::add(m_settings.sword.standardGreekMorph, TT_OD_SWORD_STANDARD_GREEK_MORPH);
 	QWhatsThis::add(m_settings.sword.standardGreekMorph, WT_OD_SWORD_STANDARD_GREEK_MORPH);
 	hBox->addWidget(label);
 	hBox->addWidget(m_settings.sword.standardGreekMorph);
 	layout->addLayout(hBox);
							
		
 	//fill the comboboxes with the right modules
 	ListCSwordModuleInfo* modules = m_important->swordBackend->getModuleList();
  for ( modules->first(); modules->current(); modules->next() ) {
		const QString& modDescript = modules->current()->getDescription();
 		switch (modules->current()->getType()) {
 			case CSwordModuleInfo::Bible:
 				m_settings.sword.standardBible->insertItem(modDescript);
 				break;
 			case CSwordModuleInfo::Commentary:
 				m_settings.sword.standardCommentary->insertItem(modDescript);				
 				break;
 			case CSwordModuleInfo::Lexicon:
 			{
				m_settings.sword.standardLexicon->insertItem(modDescript);
 				//place the Hebrew and Greek lexicons accordingly...
 				if (modDescript.contains("Hebrew", false))
					m_settings.sword.standardHebrewStrong->insertItem(modDescript);				
				else if (modDescript.contains("Greek", false) )
					m_settings.sword.standardGreekStrong->insertItem(modDescript);
				else if (modDescript.contains("Morph", false) ) {
 					m_settings.sword.standardHebrewMorph->insertItem(modDescript);
 					m_settings.sword.standardGreekMorph->insertItem(modDescript);
 				}
 				break;
 			} 				
 			default://unknown type					
 				break;
 		}
  }

//using two lists and one loop is better than six loops with almost the same code :)
 	QList<QComboBox> comboList;
 	comboList.setAutoDelete(false);
 	comboList.append(m_settings.sword.standardBible);
 	comboList.append(m_settings.sword.standardCommentary);
 	comboList.append(m_settings.sword.standardLexicon);
 	comboList.append(m_settings.sword.standardHebrewStrong);
 	comboList.append(m_settings.sword.standardGreekStrong);
 	comboList.append(m_settings.sword.standardHebrewMorph);
 	comboList.append(m_settings.sword.standardGreekMorph);

 	QStringList moduleList;
 	moduleList.append(CBTConfig::get(CBTConfig::standardBible));
 	moduleList.append(CBTConfig::get(CBTConfig::standardCommentary));
 	moduleList.append(CBTConfig::get(CBTConfig::standardLexicon));
 	moduleList.append(CBTConfig::get(CBTConfig::standardHebrewStrongsLexicon));
 	moduleList.append(CBTConfig::get(CBTConfig::standardGreekStrongsLexicon));
 	moduleList.append(CBTConfig::get(CBTConfig::standardHebrewMorphLexicon));
 	moduleList.append(CBTConfig::get(CBTConfig::standardGreekMorphLexicon)); 	 	 	 	 	 	

 	for ( QComboBox* combo = comboList.first(); combo != 0; combo = comboList.next() ) {
		const int count = combo->count();
		const QString& module = moduleList[comboList.at()];
 		for (int item = 0; item < count; item++) {
	 		if (combo->text(item) == module ) {
	 		  combo->setCurrentItem(item);
	 		  continue;
	 		}
	 	}
	}
 	
 	layout->addStretch(4);


// ---------- new tab: filters -------- //
  currentTab = new QFrame(tabCtl);
  tabCtl->addTab(currentTab, i18n("Filter settings"));
  layout = new QVBoxLayout(currentTab,5);

  layout->addWidget( CToolClass::explanationLabel(currentTab, i18n("Filter settings"), i18n("Filters control the appereance of the text in the display windows. Choose the default values of the various filter settings. You can change the locally for each window, too.")) );
  layout->addSpacing(5);
  		
 	m_settings.sword.lineBreaks = new QCheckBox(currentTab);
 	m_settings.sword.lineBreaks->setText(i18n("Show line break after each verse"));
 	m_settings.sword.lineBreaks->setChecked(CBTConfig::get(CBTConfig::lineBreaks));		
 	layout->addWidget(m_settings.sword.lineBreaks);

 	m_settings.sword.verseNumbers = new QCheckBox(currentTab);
 	m_settings.sword.verseNumbers->setText(i18n("Show verse numbers"));
 	m_settings.sword.verseNumbers->setChecked(CBTConfig::get(CBTConfig::verseNumbers));		
 	layout->addWidget(m_settings.sword.verseNumbers);

 	m_settings.sword.footnotes = new QCheckBox(currentTab);
 	m_settings.sword.footnotes->setText(i18n("Show footnotes"));
 	m_settings.sword.footnotes->setChecked(CBTConfig::get(CBTConfig::footnotes));		
 	layout->addWidget(m_settings.sword.footnotes);

 	m_settings.sword.strongNumbers = new QCheckBox(currentTab);
 	m_settings.sword.strongNumbers->setText(i18n("Show Strong's Numbers"));
 	m_settings.sword.strongNumbers->setChecked(CBTConfig::get(CBTConfig::strongNumbers));		
 	layout->addWidget(m_settings.sword.strongNumbers);

 	m_settings.sword.headings = new QCheckBox(currentTab);
 	m_settings.sword.headings->setText(i18n("Show headings"));
 	m_settings.sword.headings->setChecked(CBTConfig::get(CBTConfig::headings));		
 	layout->addWidget(m_settings.sword.headings);

 	m_settings.sword.morphTags = new QCheckBox(currentTab);
 	m_settings.sword.morphTags->setText(i18n("Show morphologic tags"));
 	m_settings.sword.morphTags->setChecked(CBTConfig::get(CBTConfig::morphTags));		
 	layout->addWidget(m_settings.sword.morphTags);

 	m_settings.sword.lemmas = new QCheckBox(currentTab);
 	m_settings.sword.lemmas->setText(i18n("Show lemmas"));
 	m_settings.sword.lemmas->setChecked(CBTConfig::get(CBTConfig::lemmas));		
 	layout->addWidget(m_settings.sword.lemmas);
		
 	m_settings.sword.hebrewPoints = new QCheckBox(currentTab);
 	m_settings.sword.hebrewPoints->setText(i18n("Show Hebrew vowel points"));
 	m_settings.sword.hebrewPoints->setChecked(CBTConfig::get(CBTConfig::hebrewPoints));		
 	layout->addWidget(m_settings.sword.hebrewPoints);

 	m_settings.sword.hebrewCantillation = new QCheckBox(currentTab);
 	m_settings.sword.hebrewCantillation->setText(i18n("Show Hebrew cantillation marks"));
 	m_settings.sword.hebrewCantillation->setChecked(CBTConfig::get(CBTConfig::hebrewCantillation));		
 	layout->addWidget(m_settings.sword.hebrewCantillation);

 	m_settings.sword.greekAccents = new QCheckBox(currentTab);
 	m_settings.sword.greekAccents->setText(i18n("Show Greek accents"));
 	m_settings.sword.greekAccents->setChecked(CBTConfig::get(CBTConfig::greekAccents));		
 	layout->addWidget(m_settings.sword.greekAccents);
	
	layout->addStretch(4);	
}

void COptionsDialog::saveAccelerators(){
	m_settings.keys.application.accel->setKeyDict( m_settings.keys.application.dict );	
 	m_settings.keys.application.accel->writeSettings(); 	
 	
// 	m_settings.keys.general.accel->setKeyDict( m_settings.keys.general.dict );			
// 	m_settings.keys.general.accel->writeSettings();
		
 	m_settings.keys.bible.accel->setKeyDict( m_settings.keys.bible.dict );					
 	m_settings.keys.bible.accel->writeSettings();		
		
 	m_settings.keys.commentary.accel->setKeyDict( m_settings.keys.commentary.dict );					
 	m_settings.keys.commentary.accel->writeSettings();
		
 	m_settings.keys.lexicon.accel->setKeyDict( m_settings.keys.lexicon.dict );					
 	m_settings.keys.lexicon.accel->writeSettings();
}

/** No descriptions */
void COptionsDialog::saveColors(){
 	CBTConfig::set(CBTConfig::textColor, m_settings.colors.text->color().name());	
 	CBTConfig::set(CBTConfig::backgroundColor, m_settings.colors.background->color().name());	
 	CBTConfig::set(CBTConfig::highlightedVerseColor, m_settings.colors.highlightedVerse->color().name());		
 	CBTConfig::set(CBTConfig::swordRefColor, m_settings.colors.swordrefs->color().name());		
 	CBTConfig::set(CBTConfig::footnotesColor, m_settings.colors.footnotes->color().name());		
 	CBTConfig::set(CBTConfig::strongsColor, m_settings.colors.strongs->color().name());		
 	CBTConfig::set(CBTConfig::morphsColor, m_settings.colors.morph->color().name());		
 	CBTConfig::set(CBTConfig::jesuswordsColor, m_settings.colors.jesuswords->color().name());		
}

/** No descriptions */
void COptionsDialog::saveFonts(){
	for(QMap<QString, QFont>::Iterator it = m_settings.fonts.fontMap.begin(); it != m_settings.fonts.fontMap.end(); ++it ){
 		if (it.key() == i18n("Standard"))
 			CBTConfig::set(CBTConfig::standard, it.data());
 		else if (it.key() == i18n("Unicode"))
 			CBTConfig::set(CBTConfig::unicode, it.data());
 	}
}

/** No descriptions */
void COptionsDialog::saveProfiles(){
}

/** No descriptions */
void COptionsDialog::saveStartup(){
	CBTConfig::set( CBTConfig::logo, m_settings.startup.showLogo->isChecked() );	
 	CBTConfig::set( CBTConfig::tips, m_settings.startup.showTips->isChecked() );				
 	CBTConfig::set( CBTConfig::restoreWorkspace, m_settings.startup.restoreWorkspace->isChecked() );	
}

/** No descriptions */
void COptionsDialog::saveSword(){
  bool old_lexiconCache = CBTConfig::get(CBTConfig::lexiconCache);
  bool new_lexiconCache = m_settings.sword.lexiconCache->isChecked();
   		
  CBTConfig::set( CBTConfig::lexiconCache, new_lexiconCache );	

  if (old_lexiconCache && !new_lexiconCache){  //delete cache files
  	QString dirname = KGlobal::dirs()->saveLocation("data", "bibletime/cache/");
  	QDir dir = QDir(dirname);
   	QStringList files = QStringList( dir.entryList() );
   	for (QStringList::Iterator it = files.begin(); it != files.end(); ++it)
   		dir.remove((*it),false);			
  }
       	
  CBTConfig::set(CBTConfig::standardBible, m_settings.sword.standardBible->currentText());
  CBTConfig::set(CBTConfig::standardCommentary, m_settings.sword.standardCommentary->currentText());
  CBTConfig::set(CBTConfig::standardLexicon, m_settings.sword.standardLexicon->currentText());
  CBTConfig::set(CBTConfig::standardHebrewStrongsLexicon, m_settings.sword.standardHebrewStrong->currentText());
  CBTConfig::set(CBTConfig::standardGreekStrongsLexicon, m_settings.sword.standardGreekStrong->currentText() );  	
  CBTConfig::set(CBTConfig::standardHebrewMorphLexicon, m_settings.sword.standardHebrewMorph->currentText());
  CBTConfig::set(CBTConfig::standardGreekMorphLexicon, m_settings.sword.standardGreekMorph->currentText() );  	

 	const QString currentText = m_settings.sword.localeCombo->currentText();
 	list <string> locales = LocaleMgr::systemLocaleMgr.getAvailableLocales();
 	QString localeName = QString::null;
 	for (list <string>::iterator it = locales.begin(); it != locales.end(); it++) {
 		if ( i18n(LocaleMgr::systemLocaleMgr.getLocale((*it).c_str())->getDescription()) == currentText ) {
 			localeName = (*it).c_str();	//we found the abbrevation for the current language
 			break;
 		}
 	}
 	if (!localeName.isEmpty())
 		CBTConfig::set(CBTConfig::language, localeName);
 	else
 		CBTConfig::set(CBTConfig::language, currentText);

 	CBTConfig::set(CBTConfig::scroll, m_settings.sword.useDownArrow->isChecked());
 	CBTConfig::set(CBTConfig::lineBreaks, m_settings.sword.lineBreaks->isChecked());
 	CBTConfig::set(CBTConfig::verseNumbers, m_settings.sword.verseNumbers->isChecked());
 	CBTConfig::set(CBTConfig::footnotes, m_settings.sword.footnotes->isChecked());
 	CBTConfig::set(CBTConfig::strongNumbers, m_settings.sword.strongNumbers->isChecked());
 	CBTConfig::set(CBTConfig::headings, m_settings.sword.headings->isChecked());
 	CBTConfig::set(CBTConfig::morphTags, m_settings.sword.morphTags->isChecked());
 	CBTConfig::set(CBTConfig::lemmas, m_settings.sword.lemmas->isChecked());
 	CBTConfig::set(CBTConfig::hebrewPoints, m_settings.sword.hebrewPoints->isChecked());
 	CBTConfig::set(CBTConfig::hebrewCantillation, m_settings.sword.hebrewCantillation->isChecked());
 	CBTConfig::set(CBTConfig::greekAccents, m_settings.sword.greekAccents->isChecked()); 		
}
