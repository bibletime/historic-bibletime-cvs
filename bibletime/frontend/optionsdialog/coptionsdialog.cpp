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
#include <kconfig.h>
#include <kkeydialog.h>
#include <kiconloader.h>
#include <kconfig.h>
#include <kapp.h>
#include <klistbox.h>
#include <kkeydialog.h>

//Sword includes
#include <localemgr.h>

COptionsDialog::COptionsDialog(CImportantClasses* importantClasses, QWidget *parent, const char *name, KAccel* accel )
	: KDialogBase(TreeList, i18n("Optionsdialog"), Ok | Cancel | Apply, Ok, parent, name, true, true, QString::null, QString::null, QString::null) {

	m_config = KGlobal::config();	
	m_important = importantClasses;
	m_changedSettings = 0;	
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
		
		KConfigGroupSaver groupSaver(m_config, "Startup");
		m_general.startup.showTips->setChecked( m_config->readBoolEntry("show tips", true) );
	}
	layout->addWidget(m_general.startup.showTips);	

	{ //startup logo
		m_general.startup.showLogo = new QCheckBox(page);
		m_general.startup.showLogo->setText(i18n("Show startuplogo"));
		QWhatsThis::add(m_general.startup.showLogo, WT_OD_GENERAL_SHOW_STARTUPLOGO);
		QToolTip::add(m_general.startup.showLogo, TT_OD_GENERAL_SHOW_STARTUPLOGO);		
		
		KConfigGroupSaver groupSaver(m_config, "General");
		m_general.startup.showLogo->setChecked(m_config->readBoolEntry("Logo", true));			
	}		
	layout->addWidget(m_general.startup.showLogo);	

	{ //startup logo
		m_general.startup.restoreWorkspace = new QCheckBox(page);
		m_general.startup.restoreWorkspace->setText(i18n("Restore windows in workspace area"));
		QToolTip::add(m_general.startup.restoreWorkspace, TT_OD_GENERAL_RESTORE_WORKSPACE);		
		QWhatsThis::add(m_general.startup.restoreWorkspace, WT_OD_GENERAL_RESTORE_WORKSPACE);
		
		KConfigGroupSaver groupSaver(m_config, "Startup");
		m_general.startup.restoreWorkspace->setChecked(m_config->readBoolEntry("restore workspace", false));			
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
		
		KConfigGroupSaver groupSaver(m_config, "SWORD");
		m_general.sword.lexiconCache->setChecked( m_config->readBoolEntry("use lexicon cache", true) );
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
						
		
		//fill the comboboxes with the right modules
		ListCSwordModuleInfo* modules = m_important->swordBackend->getModuleList();
    for ( modules->first();modules->current();modules->next() ) {
			switch (modules->current()->getType()) {
				case CSwordModuleInfo::Bible:
					m_general.sword.standardBible->insertItem(modules->current()->getDescription());
					break;
				case CSwordModuleInfo::Commentary:
					m_general.sword.standardCommentary->insertItem(modules->current()->getDescription());				
					break;
				case CSwordModuleInfo::Lexicon:
					m_general.sword.standardLexicon->insertItem(modules->current()->getDescription());				
					break;
				default://unknown type					
					break;
			}
    }
		
		KConfigGroupSaver groupSaver(m_config, "SWORD");
		
		const QString standardBible = m_config->readEntry("standard bible");
		int count = m_general.sword.standardBible->count();
		for (int item=0; item < count; ++item) {
			if (m_general.sword.standardBible->text(item) == standardBible) {
				m_general.sword.standardBible->setCurrentItem(item);
				break;
			}
		}
		
		const QString standardCommentary = m_config->readEntry("standard commentary");
		count = m_general.sword.standardCommentary->count();
		for (int item=0; item < count; ++item) {
			if (m_general.sword.standardCommentary->text(item) == standardCommentary) {
				m_general.sword.standardCommentary->setCurrentItem(item);
				break;
			}
		}

		const QString standardLexicon = m_config->readEntry("standard lexicon");
		count = m_general.sword.standardLexicon->count();
		for (int item=0; item < count; ++item) {
			if (m_general.sword.standardLexicon->text(item) == standardLexicon) {
				m_general.sword.standardLexicon->setCurrentItem(item);
				break;
			}
		}				
	}
  layout2->addStretch(4);
	
	items.clear();	
	items << i18n("General") << i18n("Accelerators");	
	page = addHBoxPage(items, i18n("Configure BibleTime's key bindings"), OD_ICON_KEY_BINDINGS);

 	m_general.keys.dict = m_general.keys.accel->keyDict();

 	m_general.keys.keyChooser = new KKeyChooser( &m_general.keys.dict, page, false );	
 	QToolTip::add(m_general.keys.keyChooser, TT_OD_KEYS_CHOOSER);
 	QWhatsThis::add(m_general.keys.keyChooser, WT_OD_KEYS_CHOOSER);
}

void COptionsDialog::saveGeneral() {
	{
  	KConfigGroupSaver groupSaver(m_config, "Startup");
  	m_config->writeEntry( "Logo", m_general.startup.showLogo->isChecked() );	
  	m_config->writeEntry( "restore workspace", m_general.startup.restoreWorkspace->isChecked() );	
  	m_config->writeEntry( "show tips", m_general.startup.showTips->isChecked() );				
	}
	{
		KConfigGroupSaver groupSaver(m_config, "Keys");  	
  	m_general.keys.accel->setKeyDict( m_general.keys.dict );	
  	m_general.keys.accel->writeSettings( m_config );
	}
	{
    KConfigGroupSaver groupSaver(m_config, "SWORD");
    bool old_lexiconCache = m_config->readBoolEntry("use lexicon cache", false);
    bool new_lexiconCache = m_general.sword.lexiconCache->isChecked();
		
  	m_config->writeEntry( "use lexicon cache", new_lexiconCache );	

  	if (old_lexiconCache && !new_lexiconCache){  //delete cache files
  		QString dirname = KGlobal::dirs()->saveLocation("data", "bibletime/cache/");
  		QDir dir = QDir(dirname);
  		QStringList files = QStringList( dir.entryList() );
			for (QStringList::Iterator it = files.begin(); it != files.end(); ++it)
				dir.remove((*it),false);			
  	}
  	
  	m_config->writeEntry("standard bible", m_general.sword.standardBible->currentText());
  	m_config->writeEntry("standard commentary", m_general.sword.standardCommentary->currentText());
  	m_config->writeEntry("standard lexicon", m_general.sword.standardLexicon->currentText());  	  	
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
		KConfigGroupSaver groupSaver(m_config, "SWORD");
				
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
			SWLocale* locale = LocaleMgr::systemLocaleMgr.getLocale((const char*)m_config->readEntry("Language", KGlobal::locale()->language()).local8Bit());
			if (locale && m_displayWindows.general.localeCombo->text(test_item).contains(i18n(locale->getDescription())) )
				current_item = test_item;
		}
		if (current_item!=-1)
			m_displayWindows.general.localeCombo->setCurrentItem(current_item);
	}
	layout->addLayout(localeLayout);
	
	{
		m_displayWindows.general.useDownArrow = new QCheckBox(page);
  	m_displayWindows.general.useDownArrow->setText(i18n("Use down arrow to scroll to next verse."));
		QWhatsThis::add(m_displayWindows.general.useDownArrow, WT_OD_GENERAL_SCROLL_PREVIOUS);
		QToolTip::add(m_displayWindows.general.useDownArrow, TT_OD_GENERAL_SCROLL_PREVIOUS);
		
		KConfigGroupSaver groupSaver(m_config, "General");		
		m_displayWindows.general.useDownArrow->setChecked(m_config->readBoolEntry("Scroll",true));		
	}
	layout->addWidget(m_displayWindows.general.useDownArrow);
	layout->addStretch(4);	

	items.clear();		
	items << i18n("Display windows") << i18n("Colors");	
	QHBox* hbox_page = addHBoxPage(items, i18n("Colors used in display windows"), OD_ICON_COLORS);	
  QButtonGroup* group =	new QButtonGroup(2,Qt::Horizontal,"", hbox_page, "colorGroup");
	group->setLineWidth(0);
	{
		KConfigGroupSaver groupSaver(m_config, "Colors");			
		QLabel* label = new QLabel(i18n("Background"), group);		
		QToolTip::add(label, TT_OD_COLORS_BACKGROUND );		
		QWhatsThis::add(label, WT_OD_COLORS_BACKGROUND );	
		m_displayWindows.colors.background = new KColorButton(m_config->readColorEntry("Background", &Qt::white), group);		
		
		label = new QLabel(i18n("Highlighted Verse"), group);
		QToolTip::add(label, TT_OD_COLORS_CURRENT_VERSE );	
		QWhatsThis::add(label, WT_OD_COLORS_CURRENT_VERSE );
		m_displayWindows.colors.highlightedVerse = new KColorButton(m_config->readColorEntry("Highlighted Verse", &Qt::red ), group);
	}	
	
	items.clear();
	items << i18n("Display windows") << i18n("Fonts");		
 	QVBox* vbox_page = addVBoxPage(items, i18n("Choose fonts for BibleTime"), OD_ICON_FONTS); 	
 	{// font settings
		KConfigGroupSaver groupSaver(m_config, "Fonts");
		
		QLabel* label = new QLabel(i18n("Choose the area of application and set the font for it"), vbox_page);	
		
		m_displayWindows.fonts.usage = new QComboBox(vbox_page);		
	 	QToolTip::add(m_displayWindows.fonts.usage, TT_OD_FONTS_TYPE_CHOOSER);	 	
	 	QWhatsThis::add(m_displayWindows.fonts.usage, WT_OD_FONTS_TYPE_CHOOSER);	
	 	
		m_displayWindows.fonts.fontMap.insert(i18n("Display window"), m_config->readFontEntry( i18n("Display window") ));
		m_displayWindows.fonts.fontMap.insert(i18n("Display window Unicode"), m_config->readFontEntry( i18n("Display window Unicode") ));

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
		KConfigGroupSaver groupSaver(m_config, "SWORD");		
		const QString currentText = m_displayWindows.general.localeCombo->currentText();
		list <string> locales = LocaleMgr::systemLocaleMgr.getAvailableLocales();
		QString localeName = QString::null;
		for (list <string>::iterator it = locales.begin(); it != locales.end(); it++) {
			if ( i18n(LocaleMgr::systemLocaleMgr.getLocale((*it).c_str())->getDescription()) == currentText ) {
				localeName = (*it).c_str();	//we found the abbrevation for the current language
				break;
			}
		}

		const QString oldValue = m_config->readEntry("Language", KGlobal::locale()->language());
		if (oldValue == QString::null || oldValue != localeName) {	//changed
				m_changedSettings |= CSwordPresenter::language;
		}				
		
		if (!localeName.isEmpty())
			m_config->writeEntry("Language", localeName);
		else
			m_config->writeEntry("Language", currentText);
	}
	
	{//save settings for the scroll button
		KConfigGroupSaver groupSaver(m_config, "General");
		m_config->writeEntry("Scroll", m_displayWindows.general.useDownArrow->isChecked(), true);
	}
	
	{
		KConfigGroupSaver groupSaver(m_config, "Fonts");
		if (m_config->readFontEntry(i18n("Display window")) != m_displayWindows.fonts.fontMap[i18n("Display window")]
      || m_config->readFontEntry(i18n("Display window Unicode")) != m_displayWindows.fonts.fontMap[i18n("Display window Unicode")] )
		m_changedSettings |= CSwordPresenter::font;

		for(QMap<QString, QFont>::Iterator it = m_displayWindows.fonts.fontMap.begin(); it != m_displayWindows.fonts.fontMap.end(); ++it )
			m_config->writeEntry(it.key(), it.data());
	}
	
	{ //save color options
		KConfigGroupSaver groupSaver(m_config, "Colors");
		if ( m_config->readColorEntry("Background") != m_displayWindows.colors.background->color() )
		m_changedSettings |= CSwordPresenter::backgroundColor;

		m_config->writeEntry("Background", m_displayWindows.colors.background->color().name());	
	
		if ( m_config->readColorEntry("Highlighted Verse") != m_displayWindows.colors.highlightedVerse->color() )
				m_changedSettings |= CSwordPresenter::highlightedVerseColor;

		m_config->writeEntry("Highlighted Verse", m_displayWindows.colors.highlightedVerse->color().name());		
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

  emit signalSettingsChanged( getChangedSettings() );
}

/*commenting this out until I can figure out why it doesn't work so hot...*/
void COptionsDialog::slotApply(){
	saveGeneral();
	saveDisplayWindow();
	
	KDialogBase::slotApply();

  emit signalSettingsChanged( getChangedSettings() );
}

/** Returns an integer with ORed feature enum entries of the changed settings. */
const int COptionsDialog::getChangedSettings() const {
	return m_changedSettings;
}

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

