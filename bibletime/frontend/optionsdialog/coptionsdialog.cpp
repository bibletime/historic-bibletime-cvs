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
#include "../cprofile.h"
#include "../presenters/cswordpresenter.h"
#include "../presenters/cbiblepresenter.h"
#include "../presenters/ccommentarypresenter.h"
#include "../presenters/clexiconpresenter.h"
#include "../../ressource.h"
#include "../../whatsthisdef.h"
#include "../../backend/cswordbackend.h"
#include "../../backend/cswordmoduleinfo.h"
#include "../../backend/cswordlexiconmoduleinfo.h"
#include "../cbtconfig.h"

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
//#include <kconfig.h>
#include <kkeydialog.h>
#include <kiconloader.h>
#include <kapp.h>
#include <klistbox.h>
#include <kkeydialog.h>

//Sword includes
#include <localemgr.h>

#define STANDARD_FONT_NAME i18n("Standard")
#define UNICODE_FONT_NAME i18n("Unicode")

COptionsDialog::COptionsDialog(CImportantClasses* importantClasses, QWidget *parent, const char *name, KAccel* accel )
	: KDialogBase(TreeList, i18n("Optionsdialog"), Ok | Cancel | Apply, Ok, parent, name, true, true, QString::null, QString::null, QString::null) {

//	m_config = KGlobal::config();	
	m_important = importantClasses;
//	m_changedSettings = 0;	
	m_general.keys.accel = accel;

	setTreeListAutoResize(true);	
	setShowIconsInTreeList(true);
	setInitialSize(sizeHint()/*QSize(400,400)*/);	
	adjustSize();
		
	initGeneral();		
	initDisplayWindow();	
}

void COptionsDialog::initGeneral() {
	QStringList items;	
	items << i18n("General") << i18n("Startup");
	setFolderIcon(i18n("General"), SmallIcon("folder"));
		
	QFrame* page = addPage(items, i18n("Startup options"), OD_ICON_GENERAL);
	QVBoxLayout* layout = new QVBoxLayout(page);
	
	{//daily tips
		m_general.startup.showTips = new QCheckBox(page);
		m_general.startup.showTips->setText(i18n("Show tip of the day"));
		QToolTip::add(m_general.startup.showTips, TT_OD_GENERAL_DAILY_TIP);	
		QWhatsThis::add(m_general.startup.showTips, WT_OD_GENERAL_DAILY_TIP);
		
		m_general.startup.showTips->setChecked( CBTConfig::get(CBTConfig::tips) );
	}
	layout->addWidget(m_general.startup.showTips);	

	{ //startup logo
		m_general.startup.showLogo = new QCheckBox(page);
		m_general.startup.showLogo->setText(i18n("Show startuplogo"));
		QWhatsThis::add(m_general.startup.showLogo, WT_OD_GENERAL_SHOW_STARTUPLOGO);
		QToolTip::add(m_general.startup.showLogo, TT_OD_GENERAL_SHOW_STARTUPLOGO);		
		
		m_general.startup.showLogo->setChecked(CBTConfig::get(CBTConfig::logo));			
	}		
	layout->addWidget(m_general.startup.showLogo);	

	{ //workspace
		m_general.startup.restoreWorkspace = new QCheckBox(page);
		m_general.startup.restoreWorkspace->setText(i18n("Restore windows in workspace area"));
		QToolTip::add(m_general.startup.restoreWorkspace, TT_OD_GENERAL_RESTORE_WORKSPACE);		
		QWhatsThis::add(m_general.startup.restoreWorkspace, WT_OD_GENERAL_RESTORE_WORKSPACE);
		
		m_general.startup.restoreWorkspace->setChecked(CBTConfig::get(CBTConfig::restoreWorkspace));			
	}		
	layout->addWidget(m_general.startup.restoreWorkspace);	
		
	layout->addStretch(4);

  items.clear();
  items << i18n("General") << i18n("SWORD");
		
	page = addPage(items, i18n("SWORD options"), OD_ICON_GENERAL);
	QVBoxLayout* layout2 = new QVBoxLayout(page);	
	{//sword lexicon cache
		m_general.sword.lexiconCache = new QCheckBox(page);
		m_general.sword.lexiconCache->setText(i18n("Create and use lexicon key cache"));
		QToolTip::add(m_general.sword.lexiconCache, TT_OD_SWORD_USE_LEXICON_CACHE);	
		QWhatsThis::add(m_general.sword.lexiconCache, WT_OD_SWORD_USE_LEXICON_CACHE);
		
		m_general.sword.lexiconCache->setChecked( CBTConfig::get(CBTConfig::lexiconCache) );
  	layout2->addWidget(m_general.sword.lexiconCache);	
	}

	{//sword standard modules
		QHBoxLayout* hBox = new QHBoxLayout();				
		m_general.sword.standardBible = new QComboBox(page);
		QLabel* label = new QLabel(m_general.sword.standardBible, i18n("Default Bible"), page);
		QToolTip::add(m_general.sword.standardBible, TT_OD_SWORD_STANDARD_BIBLE);	
		QWhatsThis::add(m_general.sword.standardBible, WT_OD_SWORD_STANDARD_BIBLE);		
		hBox->addWidget(label);
		hBox->addWidget(m_general.sword.standardBible);		
		layout2->addLayout(hBox);

		hBox = new QHBoxLayout();				
		m_general.sword.standardCommentary = new QComboBox(page);
		label = new QLabel(m_general.sword.standardCommentary, i18n("Default Commentary"), page);
		QToolTip::add(m_general.sword.standardCommentary, TT_OD_SWORD_STANDARD_COMMENTARY);	
		QWhatsThis::add(m_general.sword.standardCommentary, WT_OD_SWORD_STANDARD_COMMENTARY);		
		hBox->addWidget(label);
		hBox->addWidget(m_general.sword.standardCommentary);
		layout2->addLayout(hBox);

		hBox = new QHBoxLayout();				
		m_general.sword.standardLexicon = new QComboBox(page);
		label = new QLabel(m_general.sword.standardLexicon, i18n("Default Lexicon"), page);
		QToolTip::add(m_general.sword.standardCommentary, TT_OD_SWORD_STANDARD_LEXICON);	
		QWhatsThis::add(m_general.sword.standardCommentary, WT_OD_SWORD_STANDARD_LEXICON);		
		hBox->addWidget(label);
		hBox->addWidget(m_general.sword.standardLexicon);
		layout2->addLayout(hBox);
		
		hBox = new QHBoxLayout();
		m_general.sword.standardHebrewStrong = new QComboBox(page);
		label = new QLabel(m_general.sword.standardHebrewStrong, i18n("Default Hebrew Lexicon"), page);
		QToolTip::add(m_general.sword.standardHebrewStrong, TT_OD_SWORD_STANDARD_HEBREW_STRONG);
		QWhatsThis::add(m_general.sword.standardHebrewStrong, WT_OD_SWORD_STANDARD_HEBREW_STRONG);
		hBox->addWidget(label);
		hBox->addWidget(m_general.sword.standardHebrewStrong);
		layout2->addLayout(hBox);
		
		hBox = new QHBoxLayout();
		m_general.sword.standardGreekStrong = new QComboBox(page);
		label = new QLabel(m_general.sword.standardGreekStrong, i18n("Default Greek Lexicon"), page);
		QToolTip::add(m_general.sword.standardGreekStrong, TT_OD_SWORD_STANDARD_GREEK_STRONG);
		QWhatsThis::add(m_general.sword.standardGreekStrong, WT_OD_SWORD_STANDARD_GREEK_STRONG);
		hBox->addWidget(label);
		hBox->addWidget(m_general.sword.standardGreekStrong);
		layout2->addLayout(hBox);
	
						
		
		//fill the comboboxes with the right modules
		ListCSwordModuleInfo* modules = m_important->swordBackend->getModuleList();
    for ( modules->first();modules->current();modules->next() ) {
  			QString modDescript = modules->current()->getDescription();
			switch (modules->current()->getType()) {
				case CSwordModuleInfo::Bible:
					m_general.sword.standardBible->insertItem(modDescript);
					break;
				case CSwordModuleInfo::Commentary:
					m_general.sword.standardCommentary->insertItem(modDescript);				
					break;
				case CSwordModuleInfo::Lexicon:
                 	m_general.sword.standardLexicon->insertItem(modDescript);
					//place the Hebrew and Greek lexicons accordingly...
					if (modDescript.contains("Hebrew", FALSE))
                  				m_general.sword.standardHebrewStrong->insertItem(modDescript);				
     				if (modDescript.contains("Greek", FALSE) )
     								m_general.sword.standardGreekStrong->insertItem(modDescript);
					break;
				default://unknown type					
					break;
			}
    }
		
		const QString standardBible = CBTConfig::get(CBTConfig::standardBible);
		int count = m_general.sword.standardBible->count();
		for (int item=0; item < count; ++item) {
			if (m_general.sword.standardBible->text(item) == standardBible) {
				m_general.sword.standardBible->setCurrentItem(item);
				break;
			}
		}
		
		const QString standardCommentary = CBTConfig::get(CBTConfig::standardCommentary);
		count = m_general.sword.standardCommentary->count();
		for (int item=0; item < count; ++item) {
			if (m_general.sword.standardCommentary->text(item) == standardCommentary) {
				m_general.sword.standardCommentary->setCurrentItem(item);
				break;
			}
		}

		const QString standardLexicon = CBTConfig::get(CBTConfig::standardLexicon);
		count = m_general.sword.standardLexicon->count();
		for (int item=0; item < count; ++item) {
			if (m_general.sword.standardLexicon->text(item) == standardLexicon) {
				m_general.sword.standardLexicon->setCurrentItem(item);
				break;
			}
		}			
		
		const QString standardHebrewStrong = CBTConfig::get(CBTConfig::standardHebrewLexicon);
		count = m_general.sword.standardHebrewStrong->count();
		for (int item=0; item < count; ++item) {
			if (m_general.sword.standardHebrewStrong->text(item) == standardHebrewStrong) {
				m_general.sword.standardHebrewStrong->setCurrentItem(item);
				break ;
			}
		}		
		
		const QString standardGreekStrong = CBTConfig::get(CBTConfig::standardGreekLexicon);
		count = m_general.sword.standardGreekStrong->count();
		for(int item=0; item<count; ++item) {
			if(m_general.sword.standardGreekStrong->text(item) == standardGreekStrong) {
				m_general.sword.standardGreekStrong->setCurrentItem(item);
				break;
			}
		}
		
			
	}
  layout2->addStretch(4);
	
	items.clear();	
	items << i18n("General") << i18n("Accelerators");	
	page = addHBoxPage(items, i18n("Configure BibleTimes key bindings"), OD_ICON_KEY_BINDINGS);

 	m_general.keys.dict = m_general.keys.accel->keyDict();

 	m_general.keys.keyChooser = new KKeyChooser( &m_general.keys.dict, page, false );	
 	QToolTip::add(m_general.keys.keyChooser, TT_OD_KEYS_CHOOSER);
 	QWhatsThis::add(m_general.keys.keyChooser, WT_OD_KEYS_CHOOSER);
}

void COptionsDialog::saveGeneral() {
	{
  	CBTConfig::set( CBTConfig::logo, m_general.startup.showLogo->isChecked() );	
  	CBTConfig::set( CBTConfig::tips, m_general.startup.showTips->isChecked() );				
  	CBTConfig::set( CBTConfig::restoreWorkspace, m_general.startup.restoreWorkspace->isChecked() );	
	}
	{
  	m_general.keys.accel->setKeyDict( m_general.keys.dict );	
  	m_general.keys.accel->writeSettings( );
	}
	{
    bool old_lexiconCache = CBTConfig::get(CBTConfig::lexiconCache);
    bool new_lexiconCache = m_general.sword.lexiconCache->isChecked();
		
  	CBTConfig::set( CBTConfig::lexiconCache, new_lexiconCache );	

  	if (old_lexiconCache && !new_lexiconCache){  //delete cache files
  		QString dirname = KGlobal::dirs()->saveLocation("data", "bibletime/cache/");
  		QDir dir = QDir(dirname);
  		QStringList files = QStringList( dir.entryList() );
			for (QStringList::Iterator it = files.begin(); it != files.end(); ++it)
				dir.remove((*it),false);			
  	}
  	
  	CBTConfig::set(CBTConfig::standardBible, m_general.sword.standardBible->currentText());
  	CBTConfig::set(CBTConfig::standardCommentary, m_general.sword.standardCommentary->currentText());
  	CBTConfig::set(CBTConfig::standardLexicon, m_general.sword.standardLexicon->currentText());
  	CBTConfig::set(CBTConfig::standardHebrewLexicon, m_general.sword.standardHebrewStrong->currentText());
	 	CBTConfig::set(CBTConfig::standardGreekLexicon, m_general.sword.standardGreekStrong->currentText() );  	
	}
}


void COptionsDialog::initDisplayWindow() {
	QStringList items;	
	items << i18n("Display windows") << i18n("General");
	setFolderIcon(i18n("Display windows"), SmallIcon("folder"));
	
	QFrame* page = addPage(items, i18n("General settings for display windows"), OD_ICON_GENERAL);
	QVBoxLayout* layout = new QVBoxLayout(page);	
	QHBoxLayout* localeLayout = new QHBoxLayout();
	{//bookname language		
		m_displayWindows.general.localeCombo = new QComboBox(page);
		QToolTip::add(m_displayWindows.general.localeCombo, TT_OD_GENERAL_INTERNATIONAL_BOOKNAMES);				
		QWhatsThis::add(m_displayWindows.general.localeCombo, WT_OD_GENERAL_INTERNATIONAL_BOOKNAMES);		

		QLabel* label = new QLabel(m_displayWindows.general.localeCombo, i18n("Language for booknames"), page);
		
		localeLayout->addWidget(label);
		localeLayout->addWidget(m_displayWindows.general.localeCombo);
							
		m_displayWindows.general.localeCombo->insertItem( i18n("English") );
		list <string> locales = LocaleMgr::systemLocaleMgr.getAvailableLocales();
		for (list <string>::iterator it = locales.begin(); it != locales.end(); it++) {
			m_displayWindows.general.localeCombo->insertItem( i18n(LocaleMgr::systemLocaleMgr.getLocale((*it).c_str())->getDescription()) );
		}

		int current_item = -1;
		for(int test_item = 0; test_item < m_displayWindows.general.localeCombo->count(); test_item++) {
			SWLocale* locale = LocaleMgr::systemLocaleMgr.getLocale(CBTConfig::get(CBTConfig::language).local8Bit());
			if (locale && m_displayWindows.general.localeCombo->text(test_item).contains(i18n(locale->getDescription())) )
				current_item = test_item;
		}
		if (current_item!=-1)
			m_displayWindows.general.localeCombo->setCurrentItem(current_item);
	}
	layout->addLayout(localeLayout);
	
	{
		m_displayWindows.general.useDownArrow = new QCheckBox(page);
  	m_displayWindows.general.useDownArrow->setText(i18n("Use down arrow to scroll to next verse"));
		m_displayWindows.general.useDownArrow->setChecked(CBTConfig::get(CBTConfig::scroll));		
		QWhatsThis::add(m_displayWindows.general.useDownArrow, WT_OD_GENERAL_SCROLL_PREVIOUS);
		QToolTip::add(m_displayWindows.general.useDownArrow, TT_OD_GENERAL_SCROLL_PREVIOUS);
		layout->addWidget(m_displayWindows.general.useDownArrow);

		m_displayWindows.general.lineBreaks = new QCheckBox(page);
  	m_displayWindows.general.lineBreaks->setText(i18n("Show linebreak after each verse"));
		m_displayWindows.general.lineBreaks->setChecked(CBTConfig::get(CBTConfig::lineBreaks));		
		layout->addWidget(m_displayWindows.general.lineBreaks);

		m_displayWindows.general.verseNumbers = new QCheckBox(page);
  	m_displayWindows.general.verseNumbers->setText(i18n("Show versenumbers"));
		m_displayWindows.general.verseNumbers->setChecked(CBTConfig::get(CBTConfig::verseNumbers));		
		layout->addWidget(m_displayWindows.general.verseNumbers);

		m_displayWindows.general.footnotes = new QCheckBox(page);
  	m_displayWindows.general.footnotes->setText(i18n("Show footnotes"));
		m_displayWindows.general.footnotes->setChecked(CBTConfig::get(CBTConfig::footnotes));		
  	layout->addWidget(m_displayWindows.general.footnotes);

		m_displayWindows.general.strongNumbers = new QCheckBox(page);
  	m_displayWindows.general.strongNumbers->setText(i18n("Show Strong's Numbers"));
		m_displayWindows.general.strongNumbers->setChecked(CBTConfig::get(CBTConfig::strongNumbers));		
  	layout->addWidget(m_displayWindows.general.strongNumbers);

		m_displayWindows.general.headings = new QCheckBox(page);
  	m_displayWindows.general.headings->setText(i18n("Show headings"));
		m_displayWindows.general.headings->setChecked(CBTConfig::get(CBTConfig::headings));		
  	layout->addWidget(m_displayWindows.general.headings);

		m_displayWindows.general.morphTags = new QCheckBox(page);
  	m_displayWindows.general.morphTags->setText(i18n("Show morphologic tags"));
		m_displayWindows.general.morphTags->setChecked(CBTConfig::get(CBTConfig::morphTags));		
  	layout->addWidget(m_displayWindows.general.morphTags);

		m_displayWindows.general.lemmas = new QCheckBox(page);
  	m_displayWindows.general.lemmas->setText(i18n("Show lemmas"));
		m_displayWindows.general.lemmas->setChecked(CBTConfig::get(CBTConfig::lemmas));		
  	layout->addWidget(m_displayWindows.general.lemmas);
		
		m_displayWindows.general.hebrewPoints = new QCheckBox(page);
  	m_displayWindows.general.hebrewPoints->setText(i18n("Show Hebrew vowel points"));
		m_displayWindows.general.hebrewPoints->setChecked(CBTConfig::get(CBTConfig::hebrewPoints));		
  	layout->addWidget(m_displayWindows.general.hebrewPoints);

		m_displayWindows.general.hebrewCantillation = new QCheckBox(page);
  	m_displayWindows.general.hebrewCantillation->setText(i18n("Show Hebrew cantillation marks"));
		m_displayWindows.general.hebrewCantillation->setChecked(CBTConfig::get(CBTConfig::hebrewCantillation));		
  	layout->addWidget(m_displayWindows.general.hebrewCantillation);

		m_displayWindows.general.greekAccents = new QCheckBox(page);
  	m_displayWindows.general.greekAccents->setText(i18n("Show Greek accents"));
		m_displayWindows.general.greekAccents->setChecked(CBTConfig::get(CBTConfig::greekAccents));		
  	layout->addWidget(m_displayWindows.general.greekAccents);
	}
	layout->addStretch(4);	

	items.clear();		
	items << i18n("Display windows") << i18n("Colors");	
	QHBox* hbox_page = addHBoxPage(items, i18n("Colors used in display windows"), OD_ICON_COLORS);	
  QButtonGroup* group =	new QButtonGroup(2,Qt::Horizontal,"", hbox_page, "colorGroup");
	group->setLineWidth(0);
	{
		QLabel* label = new QLabel(i18n("Text"), group);		
//		QToolTip::add(label, TT_OD_COLORS_BACKGROUND );		
//		QWhatsThis::add(label, WT_OD_COLORS_BACKGROUND );	
		m_displayWindows.colors.text = new KColorButton(CBTConfig::get(CBTConfig::textColor), group);		

		label = new QLabel(i18n("Background"), group);		
		QToolTip::add(label, TT_OD_COLORS_BACKGROUND );		
		QWhatsThis::add(label, WT_OD_COLORS_BACKGROUND );	
		m_displayWindows.colors.background = new KColorButton(CBTConfig::get(CBTConfig::backgroundColor), group);		
		
		label = new QLabel(i18n("Highlighted verse"), group);
		QToolTip::add(label, TT_OD_COLORS_CURRENT_VERSE );	
		QWhatsThis::add(label, WT_OD_COLORS_CURRENT_VERSE );
		m_displayWindows.colors.highlightedVerse = new KColorButton(CBTConfig::get(CBTConfig::highlightedVerseColor), group);

		label = new QLabel(i18n("Footnotes"), group);		
//		QToolTip::add(label, TT_OD_COLORS_BACKGROUND );		
//		QWhatsThis::add(label, WT_OD_COLORS_BACKGROUND );	
		m_displayWindows.colors.footnotes = new KColorButton(CBTConfig::get(CBTConfig::footnotesColor), group);		

		label = new QLabel(i18n("Strong's numbers"), group);		
//		QToolTip::add(label, TT_OD_COLORS_BACKGROUND );		
//		QWhatsThis::add(label, WT_OD_COLORS_BACKGROUND );	
		m_displayWindows.colors.strongs = new KColorButton(CBTConfig::get(CBTConfig::strongsColor), group);		

		label = new QLabel(i18n("Morphologic tags"), group);		
//		QToolTip::add(label, TT_OD_COLORS_BACKGROUND );		
//		QWhatsThis::add(label, WT_OD_COLORS_BACKGROUND );	
		m_displayWindows.colors.morph = new KColorButton(CBTConfig::get(CBTConfig::morphsColor), group);		

		label = new QLabel(i18n("Words of Jesus"), group);		
//		QToolTip::add(label, TT_OD_COLORS_BACKGROUND );		
//		QWhatsThis::add(label, WT_OD_COLORS_BACKGROUND );	
		m_displayWindows.colors.jesuswords = new KColorButton(CBTConfig::get(CBTConfig::jesuswordsColor), group);		

	}	
	
	items.clear();
	items << i18n("Display windows") << i18n("Fonts");		
 	QVBox* vbox_page = addVBoxPage(items, i18n("Choose fonts for BibleTime"), OD_ICON_FONTS); 	
 	{// font settings
		QLabel* label = new QLabel(i18n("Choose the area of application and set the font for it"), vbox_page);	
		
		m_displayWindows.fonts.usage = new QComboBox(vbox_page);		
	 	QToolTip::add(m_displayWindows.fonts.usage, TT_OD_FONTS_TYPE_CHOOSER);	 	
	 	QWhatsThis::add(m_displayWindows.fonts.usage, WT_OD_FONTS_TYPE_CHOOSER);	
	 	
		m_displayWindows.fonts.fontMap.insert(STANDARD_FONT_NAME, CBTConfig::get(CBTConfig::standard));
		m_displayWindows.fonts.fontMap.insert(UNICODE_FONT_NAME, CBTConfig::get(CBTConfig::unicode));

		for( QMap<QString, QFont>::Iterator it = m_displayWindows.fonts.fontMap.begin(); it != m_displayWindows.fonts.fontMap.end(); ++it )
			m_displayWindows.fonts.usage->insertItem(it.key());
		
		m_displayWindows.fonts.fontChooser = new KFontChooser(vbox_page, "fonts", false, QStringList(), true, 6);
		m_displayWindows.fonts.fontChooser->setSampleText(i18n("The quick brown fox jumps over the lazy dog"));

	  connect(m_displayWindows.fonts.fontChooser, SIGNAL(fontSelected(const QFont&)), SLOT(newDisplayWindowFontSelected(const QFont&)));
	  connect(m_displayWindows.fonts.usage, SIGNAL(activated(const QString&)), SLOT(newDisplayWindowFontAreaSelected(const QString&)));
	 	QWhatsThis::add(m_displayWindows.fonts.fontChooser, WT_OD_FONTS_CHOOSER);
		 	
		m_displayWindows.fonts.fontChooser->setFont( m_displayWindows.fonts.fontMap[m_displayWindows.fonts.usage->currentText()] );
	}
	
	items.clear();
	items << i18n("Display windows") << i18n("Accelerators");
	setFolderIcon(items, SmallIcon("folder"));

	items.clear();
	items << i18n("Display windows") << i18n("Accelerators") << i18n("General");

	page = addHBoxPage(items, i18n("Accelerators used by all windows"), OD_ICON_KEY_BINDINGS);
	m_displayWindows.keys.general.accel = new KAccel(this); //delete in destructor
	CSwordPresenter::insertKeyboardActions( m_displayWindows.keys.general.accel );		
	m_displayWindows.keys.general.accel->readSettings();
 	m_displayWindows.keys.general.dict = m_displayWindows.keys.general.accel->keyDict();
 	m_displayWindows.keys.general.keyChooser = new KKeyChooser( &m_displayWindows.keys.general.dict, page, false );	
 	QToolTip::add(m_displayWindows.keys.general.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_GENERAL);
	QWhatsThis::add(m_displayWindows.keys.general.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_GENERAL);

	items.clear();
	items << i18n("Display windows") << i18n("Accelerators") << i18n("Bible windows");	
	page = addHBoxPage(items, i18n("Accelerators for bible windows"), OD_ICON_KEY_BINDINGS);
	m_displayWindows.keys.bible.accel = new KAccel(this); //delete in destructor
	CBiblePresenter::insertKeyboardActions( m_displayWindows.keys.bible.accel );
	m_displayWindows.keys.bible.accel->readSettings();		
 	m_displayWindows.keys.bible.dict = m_displayWindows.keys.bible.accel->keyDict();
 	m_displayWindows.keys.bible.keyChooser = new KKeyChooser( &m_displayWindows.keys.bible.dict, page, false );	
	QToolTip::add(m_displayWindows.keys.bible.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_BIBLE);
	QWhatsThis::add(m_displayWindows.keys.bible.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_BIBLE);

	items.clear();
	items << i18n("Display windows") << i18n("Accelerators") << i18n("Commentary windows");	
	page = addHBoxPage(items, i18n("Accelerators for commentary windows"), OD_ICON_KEY_BINDINGS);
	m_displayWindows.keys.commentary.accel = new KAccel(this); //delete in destructor
	CCommentaryPresenter::insertKeyboardActions( m_displayWindows.keys.commentary.accel );		
	m_displayWindows.keys.commentary.accel->readSettings();	
 	m_displayWindows.keys.commentary.dict = m_displayWindows.keys.commentary.accel->keyDict();
 	m_displayWindows.keys.commentary.keyChooser = new KKeyChooser( &m_displayWindows.keys.commentary.dict, page, false );	
 	QToolTip::add(m_displayWindows.keys.commentary.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_COMMENTARY);
	QWhatsThis::add(m_displayWindows.keys.commentary.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_COMMENTARY);

	items.clear();
	items << i18n("Display windows") << i18n("Accelerators") << i18n("Lexicon windows");	
	page = addHBoxPage(items, i18n("Accelerators for lexicon windows"), OD_ICON_KEY_BINDINGS);
	m_displayWindows.keys.lexicon.accel = new KAccel(this); //delete in destructor
	CLexiconPresenter::insertKeyboardActions( m_displayWindows.keys.lexicon.accel );		
	m_displayWindows.keys.lexicon.accel->readSettings();	 	
 	m_displayWindows.keys.lexicon.dict = m_displayWindows.keys.lexicon.accel->keyDict(); 	
 	m_displayWindows.keys.lexicon.keyChooser = new KKeyChooser( &m_displayWindows.keys.lexicon.dict, page, false );	
 	QToolTip::add(m_displayWindows.keys.lexicon.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_LEXICON);
	QWhatsThis::add(m_displayWindows.keys.lexicon.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_LEXICON);
					
	items.clear();
	items << i18n("Display windows") << i18n("View profiles");
	vbox_page = addVBoxPage(items, i18n("View profiles of workspace area")/*, OD_ICON_PROFILE*/);		
	m_displayWindows.profiles.profiles = new QListBox(vbox_page);
	QList<CProfile> profiles = m_displayWindows.profiles.mgr.profiles();
	for (CProfile* p = profiles.first(); p; p = profiles.next()) {
		m_displayWindows.profiles.profiles->insertItem(p->name());
	}
	
	QHButtonGroup* buttonGroup = new QHButtonGroup(vbox_page);
	m_displayWindows.profiles.createProfile = new QPushButton(i18n("Create new profile"), buttonGroup);
	connect(m_displayWindows.profiles.createProfile, SIGNAL(clicked()), SLOT(addNewProfile()));
	
	m_displayWindows.profiles.deleteProfile = new QPushButton(i18n("Delete selected profile"), buttonGroup);	
	connect(m_displayWindows.profiles.deleteProfile, SIGNAL(clicked()), SLOT(deleteProfile()));
		
	m_displayWindows.profiles.renameProfile = new QPushButton(i18n("Rename selected profile"), buttonGroup);		
	connect(m_displayWindows.profiles.renameProfile, SIGNAL(clicked()), SLOT(renameProfile()));
}

void COptionsDialog::saveDisplayWindow() {
	{ //Save localisation settings
		const QString currentText = m_displayWindows.general.localeCombo->currentText();
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
	}
	
	{//save settings for the scroll button
		CBTConfig::set(CBTConfig::scroll, m_displayWindows.general.useDownArrow->isChecked());
		CBTConfig::set(CBTConfig::lineBreaks, m_displayWindows.general.lineBreaks->isChecked());
		CBTConfig::set(CBTConfig::verseNumbers, m_displayWindows.general.verseNumbers->isChecked());
		CBTConfig::set(CBTConfig::footnotes, m_displayWindows.general.footnotes->isChecked());
		CBTConfig::set(CBTConfig::strongNumbers, m_displayWindows.general.strongNumbers->isChecked());
		CBTConfig::set(CBTConfig::headings, m_displayWindows.general.headings->isChecked());
		CBTConfig::set(CBTConfig::morphTags, m_displayWindows.general.morphTags->isChecked());
		CBTConfig::set(CBTConfig::lemmas, m_displayWindows.general.lemmas->isChecked());
		CBTConfig::set(CBTConfig::hebrewPoints, m_displayWindows.general.hebrewPoints->isChecked());
		CBTConfig::set(CBTConfig::hebrewCantillation, m_displayWindows.general.hebrewCantillation->isChecked());
		CBTConfig::set(CBTConfig::greekAccents, m_displayWindows.general.greekAccents->isChecked());
	}
	
	{
		for(QMap<QString, QFont>::Iterator it = m_displayWindows.fonts.fontMap.begin(); it != m_displayWindows.fonts.fontMap.end(); ++it ){
			if (it.key() == STANDARD_FONT_NAME)
				CBTConfig::set(CBTConfig::standard, it.data());
			else if (it.key() == UNICODE_FONT_NAME)
				CBTConfig::set(CBTConfig::unicode, it.data());
		}
	}
	
	{ //save color options
		CBTConfig::set(CBTConfig::textColor, m_displayWindows.colors.text->color().name());	
		CBTConfig::set(CBTConfig::backgroundColor, m_displayWindows.colors.background->color().name());	
		CBTConfig::set(CBTConfig::highlightedVerseColor, m_displayWindows.colors.highlightedVerse->color().name());		
		CBTConfig::set(CBTConfig::footnotesColor, m_displayWindows.colors.footnotes->color().name());		
		CBTConfig::set(CBTConfig::strongsColor, m_displayWindows.colors.strongs->color().name());		
		CBTConfig::set(CBTConfig::morphsColor, m_displayWindows.colors.morph->color().name());		
		CBTConfig::set(CBTConfig::jesuswordsColor, m_displayWindows.colors.jesuswords->color().name());		
	}
	
	{//save accel settings
		m_displayWindows.keys.general.accel->setKeyDict( m_displayWindows.keys.general.dict );			
		m_displayWindows.keys.general.accel->writeSettings();
		
		m_displayWindows.keys.bible.accel->setKeyDict( m_displayWindows.keys.bible.dict );					
		m_displayWindows.keys.bible.accel->writeSettings();		
		
		m_displayWindows.keys.commentary.accel->setKeyDict( m_displayWindows.keys.commentary.dict );					
		m_displayWindows.keys.commentary.accel->writeSettings();
		
		m_displayWindows.keys.lexicon.accel->setKeyDict( m_displayWindows.keys.lexicon.dict );					
		m_displayWindows.keys.lexicon.accel->writeSettings();
	}
}

/**  */
void COptionsDialog::newDisplayWindowFontSelected(const QFont &newFont){
	m_displayWindows.fonts.fontMap.replace(m_displayWindows.fonts.usage->currentText(), newFont);		
}

/** Called when the combobox contents is changed */
void COptionsDialog::newDisplayWindowFontAreaSelected(const QString& usage){
	m_displayWindows.fonts.fontChooser->setFont( m_displayWindows.fonts.fontMap[usage] );
}

/** Called if the OK button was clicked */
void COptionsDialog::slotOk(){
	saveGeneral();
	saveDisplayWindow();
	
	KDialogBase::slotOk();

  emit signalSettingsChanged( );
}

/*called if the apply button was clicked*/
void COptionsDialog::slotApply(){
	saveGeneral();
	saveDisplayWindow();
	
	KDialogBase::slotApply();

  emit signalSettingsChanged( );
}

/** Returns an integer with ORed feature enum entries of the changed settings. */
//const int COptionsDialog::getChangedSettings() const {
//	return m_changedSettings;
//}

/** Adds a new view profile to the list. */
void COptionsDialog::addNewProfile(){
	bool ok = false;
	QString name = QInputDialog::getText(i18n("Create new profile"), i18n("Please enter the name of the new profile"), QString::null, &ok);
	if (ok && !name.isEmpty()) {
		m_displayWindows.profiles.mgr.create(name);
		m_displayWindows.profiles.profiles->insertItem(name);				
	}
}

/** No descriptions */
void COptionsDialog::deleteProfile(){
	const QString profile = m_displayWindows.profiles.profiles->currentText();
	m_displayWindows.profiles.mgr.remove(profile);
	m_displayWindows.profiles.profiles->removeItem( m_displayWindows.profiles.profiles->currentItem() );
}

/** Renames the currently selected profile. */
void COptionsDialog::renameProfile(){
	bool ok = false;
	const QString currentProfile = m_displayWindows.profiles.profiles->currentText();	
	CProfile* profile = m_displayWindows.profiles.mgr.profile(currentProfile);
	if (!profile)
		return;
		
	const QString newName = QInputDialog::getText(i18n("Create new profile"), i18n("Please enter the new name of the profile"),profile->name(), &ok);
	if (ok && !newName.isEmpty()) {
		profile->setName(newName);
		m_displayWindows.profiles.profiles->changeItem(newName, m_displayWindows.profiles.profiles->currentItem());
	}	
}

/** Opens the page which contaisn the given part ID. */
const bool COptionsDialog::showPart(  COptionsDialog::Parts ID ){
#warning add the necessary case statements
	switch (ID) {
		case COptionsDialog::ViewProfiles:
			showPage( pageIndex(m_displayWindows.profiles.profiles->parentWidget()) );
			break;
		default:
			break;
	}
}
