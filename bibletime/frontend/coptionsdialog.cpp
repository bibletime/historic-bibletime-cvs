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

#include "backend/cswordbackend.h"
#include "backend/cswordmoduleinfo.h"

#include "frontend/displaywindow/cbiblereadwindow.h"
#include "frontend/displaywindow/ccommentaryreadwindow.h"
#include "frontend/displaywindow/clexiconreadwindow.h"
#include "frontend/displaywindow/cbookreadwindow.h"
#include "frontend/displaywindow/creadwindow.h"
#include "frontend/cresmgr.h"

#include "util/ctoolclass.h"

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
#include <kstandarddirs.h>
#include <kkeydialog.h>
#include <kiconloader.h>
#include <ktabctl.h>
#include <kapp.h>
#include <klistbox.h>
#include <kkeydialog.h>
#include <kaction.h>
#include <kconfigbase.h>
#include <kconfig.h>

//Sword includes
#include <localemgr.h>

using std::string;
using std::list;

COptionsDialog::COptionsDialog(QWidget *parent, const char *name, KAccel* accel )
	: KDialogBase(IconList, i18n("Preferences"), Ok | Cancel | Apply, Ok, parent, name, true, true, QString::null, QString::null, QString::null) {

	m_settings.keys.application.accel = accel;
	setIconListAllVisible(true);

	initStartup();		
	initFonts();
	initColors();	
	initProfiles();
	initSword();
	initAccelerators();
}

/**  */
void COptionsDialog::newDisplayWindowFontSelected(const QFont &newFont){
  CBTConfig::FontSettingsPair oldSettings = m_settings.fonts.fontMap[ m_settings.fonts.usage->currentText() ];
	m_settings.fonts.fontMap.replace( m_settings.fonts.usage->currentText(), CBTConfig::FontSettingsPair(oldSettings.first, newFont) );		
}

/** Called when the combobox contents is changed */
void COptionsDialog::newDisplayWindowFontAreaSelected(const QString& usage){
  useOwnFontClicked( m_settings.fonts.fontMap[usage].first );
  m_settings.fonts.useOwnFontBox->setChecked( m_settings.fonts.fontMap[usage].first );

  m_settings.fonts.fontChooser->setFont( m_settings.fonts.fontMap[usage].second );
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
	QString name = QInputDialog::getText(i18n("Create new profile"), i18n("Please enter the name of the new profile"), QLineEdit::Normal, QString::null, &ok);
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
	const QString newName = QInputDialog::getText(i18n("Rename profile"), i18n("Please enter the new name of the profile"), QLineEdit::Normal, profile->name(), &ok);
	if (ok && !newName.isEmpty()) {
		profile->setName(newName);
		m_settings.profiles.profiles->changeItem(newName, m_settings.profiles.profiles->currentItem());
	}	
}

/** Opens the page which contaisn the given part ID. */
const bool COptionsDialog::showPart( COptionsDialog::Parts ID ){
	bool ret = false;
	switch (ID) {
		case COptionsDialog::ViewProfiles:
			if(showPage(pageIndex(
					m_settings.profiles.profiles->parentWidget()) ))
				ret = true;
			break;
		default:
			break;
	}
	return ret;
}

/** Initializes the startup section of the OD. */
void COptionsDialog::initStartup(){
	QFrame* page = addPage(i18n("Startup"), QString::null, DesktopIcon(CResMgr::settings::startup::icon,32));
	QVBoxLayout* layout = new QVBoxLayout(page,5);
	{//daily tips
		m_settings.startup.showTips = new QCheckBox(page);
		m_settings.startup.showTips->setText(i18n("Show tip of the day"));
		QToolTip::add(m_settings.startup.showTips, CResMgr::settings::startup::dailyTip::tooltip );	
		QWhatsThis::add(m_settings.startup.showTips, CResMgr::settings::startup::dailyTip::whatsthis );
		
		m_settings.startup.showTips->setChecked( CBTConfig::get(CBTConfig::tips) );
	}
	layout->addWidget(m_settings.startup.showTips);
	
	{ //startup logo
		m_settings.startup.showLogo = new QCheckBox(page);
		m_settings.startup.showLogo->setText(i18n("Show startuplogo"));
		QToolTip::add(m_settings.startup.showLogo, CResMgr::settings::startup::showLogo::tooltip);		
    QWhatsThis::add(m_settings.startup.showLogo, CResMgr::settings::startup::showLogo::whatsthis);
		
		m_settings.startup.showLogo->setChecked(CBTConfig::get(CBTConfig::logo));			
	}		
	layout->addWidget(m_settings.startup.showLogo);
	{ //workspace
		m_settings.startup.restoreWorkspace = new QCheckBox(page);
		m_settings.startup.restoreWorkspace->setText(i18n("Restore windows in workspace area"));
		QToolTip::add(m_settings.startup.restoreWorkspace, CResMgr::settings::startup::restoreWorkingArea::tooltip);		
		QWhatsThis::add(m_settings.startup.restoreWorkspace, CResMgr::settings::startup::restoreWorkingArea::whatsthis);

		m_settings.startup.restoreWorkspace->setChecked(CBTConfig::get(CBTConfig::restoreWorkspace));
	}
	layout->addWidget(m_settings.startup.restoreWorkspace);			
	
	layout->addStretch(4);
}

/** Init fonts section. */
void COptionsDialog::initFonts(){
	QFrame* page = addPage(i18n("Fonts"), QString::null, DesktopIcon(CResMgr::settings::fonts::icon, 32));
	QVBoxLayout* layout = new QVBoxLayout(page,5);

 	layout->addWidget(
    CToolClass::explanationLabel(
      page,
      i18n("Choose fonts for modules"),
      i18n("The drop down box lists all languages of the installed modules. First choose a language and decide if the modules of this language need another font as the application's default font. Then choose the font for the selected language.")
    )
  );
  layout->addSpacing(5);


  //horizontal layout box to contain the chooser box and use own font checkbox
  QHBoxLayout* hLayout = new QHBoxLayout();

 	m_settings.fonts.usage = new QComboBox(page);
 	QToolTip::add(m_settings.fonts.usage, CResMgr::settings::fonts::typeChooser::tooltip);
 	QWhatsThis::add(m_settings.fonts.usage, CResMgr::settings::fonts::typeChooser::whatsthis);

  hLayout->addWidget(m_settings.fonts.usage);

  CLanguageMgr::LangMap langMap = languageMgr()->availableLanguages();
  CLanguageMgr::LangMap::Iterator it;

  for ( it = langMap.begin(); it != langMap.end(); ++it ) {
    m_settings.fonts.fontMap.insert(it.data().translatedName(), CBTConfig::get(it.data()) );
  }

  for( QMap<QString, CBTConfig::FontSettingsPair>::Iterator it = m_settings.fonts.fontMap.begin(); it != m_settings.fonts.fontMap.end(); ++it ) {
		if ( m_settings.fonts.fontMap[it.key()].first ) //show font icon
	 		m_settings.fonts.usage->insertItem(SmallIcon("fonts"), it.key() );
		else		//don't show
	 		m_settings.fonts.usage->insertItem(it.key());

  }


  m_settings.fonts.useOwnFontBox = new QCheckBox(i18n("Use own font settings"), page, "font checkbox");
  connect(m_settings.fonts.useOwnFontBox, SIGNAL(toggled(bool)), SLOT(useOwnFontClicked(bool)));
 	hLayout->addWidget(m_settings.fonts.useOwnFontBox);


  layout->addLayout(hLayout);
#warning TODO: remember the last selected font and jump there.

 	m_settings.fonts.fontChooser = new KFontChooser(page, "fonts", false, QStringList(), true, 5);
 	m_settings.fonts.fontChooser->setSampleText(i18n("The quick brown fox jumps over the lazy dog"));
 	layout->addWidget(m_settings.fonts.fontChooser);

  connect(m_settings.fonts.fontChooser, SIGNAL(fontSelected(const QFont&)), SLOT(newDisplayWindowFontSelected(const QFont&)));
  connect(m_settings.fonts.usage, SIGNAL(activated(const QString&)), SLOT(newDisplayWindowFontAreaSelected(const QString&)));

 	m_settings.fonts.fontChooser->setFont( m_settings.fonts.fontMap[m_settings.fonts.usage->currentText()].second );
  useOwnFontClicked( m_settings.fonts.fontMap[m_settings.fonts.usage->currentText()].first );
	m_settings.fonts.useOwnFontBox->setChecked( m_settings.fonts.fontMap[m_settings.fonts.usage->currentText()].first );
 	m_settings.fonts.fontChooser->setMinimumSize(m_settings.fonts.fontChooser->sizeHint());
}

/** Init color section. */
void COptionsDialog::initColors(){
	QFrame* page = addPage(i18n("Colors"), QString::null, DesktopIcon(CResMgr::settings::colors::icon,32));
//	QVBoxLayout* layout = new QVBoxLayout(page);
	QGridLayout* gridLayout = new QGridLayout(page,8,5,5,5);
  gridLayout->setResizeMode(QLayout::Minimum);

  gridLayout->addMultiCellWidget(
  	CToolClass::explanationLabel(page,
  		i18n("Choose colors"),
  		i18n("Choose the colors to alter the apperance of the display windows. Some options like \"Words of Jesus\" only apply to texts which support this special feature.")
  	),
  	0,0,0,-1
  );

	m_settings.colors.text = new KColorButton(CBTConfig::get(CBTConfig::textColor), page);
  QToolTip::add(m_settings.colors.text, CResMgr::settings::colors::text::tooltip);
  QWhatsThis::add(m_settings.colors.text, CResMgr::settings::colors::text::whatsthis);

	QLabel* label = new QLabel(m_settings.colors.text, i18n("Text"), page);
	gridLayout->addWidget(label,1,0);
	gridLayout->addWidget(m_settings.colors.text,1,1);
  QToolTip::add(label, CResMgr::settings::colors::text::tooltip);
  QWhatsThis::add(label, CResMgr::settings::colors::text::whatsthis);


	m_settings.colors.highlightedVerse = new KColorButton(CBTConfig::get(CBTConfig::highlightedVerseColor), page);
  QToolTip::add(m_settings.colors.highlightedVerse, CResMgr::settings::colors::highlightedVerse::tooltip);
  QWhatsThis::add(m_settings.colors.highlightedVerse, CResMgr::settings::colors::highlightedVerse::whatsthis);

	label = new QLabel(m_settings.colors.highlightedVerse, i18n("Highlighted verse"), page);
  QToolTip::add(label, CResMgr::settings::colors::highlightedVerse::tooltip);
  QWhatsThis::add(label, CResMgr::settings::colors::highlightedVerse::whatsthis);

  gridLayout->addWidget(label,1,3);
	gridLayout->addWidget(m_settings.colors.highlightedVerse,1,4);


	m_settings.colors.background = new KColorButton(CBTConfig::get(CBTConfig::backgroundColor), page);
  QToolTip::add(m_settings.colors.background, CResMgr::settings::colors::background::tooltip);
  QWhatsThis::add(m_settings.colors.background, CResMgr::settings::colors::background::whatsthis);

  label = new QLabel(m_settings.colors.background, i18n("Background"), page);
  QToolTip::add(label, CResMgr::settings::colors::background::tooltip);
  QWhatsThis::add(label, CResMgr::settings::colors::background::whatsthis);

  gridLayout->addWidget(label,2,0);
	gridLayout->addWidget(m_settings.colors.background,2,1);


  m_settings.colors.swordrefs = new KColorButton(CBTConfig::get(CBTConfig::swordRefColor), page);
  QToolTip::add(m_settings.colors.swordrefs, CResMgr::settings::colors::links::tooltip);
  QWhatsThis::add(m_settings.colors.swordrefs, CResMgr::settings::colors::links::whatsthis);

	label = new QLabel(m_settings.colors.swordrefs,i18n("Hyperlinks"), page);
  QToolTip::add(label, CResMgr::settings::colors::links::tooltip);
  QWhatsThis::add(label, CResMgr::settings::colors::links::whatsthis);

  gridLayout->addWidget(label,4,0);
	gridLayout->addWidget(m_settings.colors.swordrefs,4,1);

	m_settings.colors.footnotes = new KColorButton(CBTConfig::get(CBTConfig::footnotesColor), page);
  QToolTip::add(m_settings.colors.footnotes, CResMgr::settings::colors::footnotes::tooltip);
  QWhatsThis::add(m_settings.colors.footnotes, CResMgr::settings::colors::footnotes::whatsthis);

  label = new QLabel(m_settings.colors.footnotes,i18n("Footnotes"), page);
  QToolTip::add(label, CResMgr::settings::colors::footnotes::tooltip);
  QWhatsThis::add(label, CResMgr::settings::colors::footnotes::whatsthis);

	gridLayout->addWidget(label,6,0);
	gridLayout->addWidget(m_settings.colors.footnotes,6,1);

	m_settings.colors.jesuswords = new KColorButton(CBTConfig::get(CBTConfig::jesuswordsColor), page);
  QToolTip::add(m_settings.colors.jesuswords, CResMgr::settings::colors::jesusWords::tooltip);
  QWhatsThis::add(m_settings.colors.jesuswords, CResMgr::settings::colors::jesusWords::whatsthis);

  label = new QLabel(m_settings.colors.jesuswords, i18n("Words of Jesus"), page);
  QToolTip::add(label, CResMgr::settings::colors::jesusWords::tooltip);
  QWhatsThis::add(label, CResMgr::settings::colors::jesusWords::whatsthis);

	gridLayout->addWidget(label,6,3);
	gridLayout->addWidget(m_settings.colors.jesuswords,6,4);


	m_settings.colors.strongs = new KColorButton(CBTConfig::get(CBTConfig::strongsColor), page);
  QToolTip::add(m_settings.colors.strongs, CResMgr::settings::colors::strongNumbers::tooltip);
  QWhatsThis::add(m_settings.colors.strongs, CResMgr::settings::colors::strongNumbers::whatsthis);

  label = new QLabel(m_settings.colors.strongs, i18n("Strong's numbers"), page);
	QToolTip::add(label, CResMgr::settings::colors::strongNumbers::tooltip);
  QWhatsThis::add(label, CResMgr::settings::colors::strongNumbers::whatsthis);

  gridLayout->addWidget(label,7,0);
	gridLayout->addWidget(m_settings.colors.strongs,7,1);


	m_settings.colors.morph = new KColorButton(CBTConfig::get(CBTConfig::morphsColor), page);
  QToolTip::add(m_settings.colors.morph, CResMgr::settings::colors::morphTags::tooltip);
  QWhatsThis::add(m_settings.colors.morph, CResMgr::settings::colors::morphTags::whatsthis);

	label = new QLabel(m_settings.colors.morph, i18n("Morphologic tags"), page);
  QToolTip::add(label, CResMgr::settings::colors::morphTags::tooltip);
  QWhatsThis::add(label, CResMgr::settings::colors::morphTags::whatsthis);

  gridLayout->addWidget(label,7,3);
	gridLayout->addWidget(m_settings.colors.morph,7,4);

	gridLayout->setRowStretch(9, 5);
	gridLayout->addColSpacing(3, 5);
	gridLayout->addRowSpacing(3, 10);
	gridLayout->addRowSpacing(5, 10);
}

/** Init profiles section. */
void COptionsDialog::initProfiles(){
	QFrame* page = addPage(i18n("Profiles"),QString::null, DesktopIcon(CResMgr::settings::profiles::icon,32));
	QGridLayout* gridLayout = new QGridLayout(page, 3,3,5,5);

	gridLayout->addMultiCellWidget(
		CToolClass::explanationLabel(page,
			i18n("Manage your profiles"),
			i18n("Profiles define the appereance of the work area, \
for example which windows are open and which texts should displayed in these windows. \
Don't forget that new profiles only work after you've saved something in them.")
		),
		0,0,0,-1
	);

	m_settings.profiles.profiles = new QListBox(page);
	gridLayout->addMultiCellWidget(m_settings.profiles.profiles, 1,1,0,-1);
	gridLayout->setRowStretch(1,10);

	m_settings.profiles.createProfile = new QPushButton(i18n("Create new profile"), page);
	connect(m_settings.profiles.createProfile, SIGNAL(clicked()), SLOT(addNewProfile()));
  gridLayout->addWidget(m_settings.profiles.createProfile,2,0);

	m_settings.profiles.deleteProfile = new QPushButton(i18n("Delete selected profile"), page);
	connect(m_settings.profiles.deleteProfile, SIGNAL(clicked()), SLOT(deleteProfile()));
  gridLayout->addWidget(m_settings.profiles.deleteProfile,2,1);

	m_settings.profiles.renameProfile = new QPushButton(i18n("Rename selected profile"), page);
	connect(m_settings.profiles.renameProfile, SIGNAL(clicked()), SLOT(renameProfile()));
  gridLayout->addWidget(m_settings.profiles.renameProfile,2,2);

  //fill the profile list box
	QPtrList<CProfile> profiles = m_settings.profiles.mgr.profiles();
	if (profiles.count()) {
		for (CProfile* p = profiles.first(); p; p = profiles.next()) {
			m_settings.profiles.profiles->insertItem(p->name());
		}
	}
	else {
		m_settings.profiles.profiles->setEnabled(false);
	}
}

/** Init accel key section. */
void COptionsDialog::initAccelerators(){
  QVBox* page = addVBoxPage(i18n("Accelerators"),QString::null, DesktopIcon(CResMgr::settings::keys::icon,32));

  KTabCtl* tabCtl = new KTabCtl(page);

// ----- new tab: All display windows ------ //
  QFrame* currentTab = new QVBox(tabCtl);
	currentTab->setMargin(3);
  tabCtl->addTab(currentTab, i18n("Application wide"));

	CBTConfig::setupAccel( CBTConfig::application, m_settings.keys.application.accel  );
//	CSwordPresenter::insertKeyboardActions( m_settings.keys.application.accel );
	m_settings.keys.application.accel->readSettings();

 	m_settings.keys.application.keyChooser = new KKeyChooser( m_settings.keys.application.accel, currentTab, false );
// 	QToolTip::add(m_settings.keys.application.keyChooser, TT_OD_KEYS_CHOOSER);
//	QWhatsThis::add(m_settings.keys.application.keyChooser, WT_OD_KEYS_CHOOSER);

// ----- new tab: All display windows ------ //
	currentTab = new QVBox(tabCtl);
	currentTab->setMargin(3);
	tabCtl->addTab(currentTab, i18n("All display windows"));

	m_settings.keys.general.accel = new KAccel(this); //delete in destructor
	CBTConfig::setupAccel( CBTConfig::readWindow, m_settings.keys.general.accel  );
	CReadWindow::insertKeyboardActions( m_settings.keys.general.accel );
	m_settings.keys.general.accel->readSettings();

 	m_settings.keys.general.keyChooser = new KKeyChooser( m_settings.keys.general.accel, currentTab );
//	QToolTip::add(m_settings.keys.general.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_GENERAL);
//	QWhatsThis::add(m_settings.keys.general.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_GENERAL);

// ----- new tab: Bible windows ------ //
	currentTab = new QVBox(tabCtl);
	currentTab->setMargin(3);
	tabCtl->addTab(currentTab, i18n("Bible windows"));

	m_settings.keys.bible.accel = new KAccel(this); //delete in destructor
	CBTConfig::setupAccel( CBTConfig::bibleWindow, m_settings.keys.bible.accel  );
	CBibleReadWindow::insertKeyboardActions( m_settings.keys.bible.accel );
	m_settings.keys.bible.accel->readSettings();

 	m_settings.keys.bible.keyChooser = new KKeyChooser( m_settings.keys.bible.accel, currentTab/*, false*/ );
//	QToolTip::add(m_settings.keys.bible.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_BIBLE);
//	QWhatsThis::add(m_settings.keys.bible.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_BIBLE);

// ----- new tab: Commentary windows ------ //
	currentTab = new QVBox(tabCtl);
	currentTab->setMargin(3);
	tabCtl->addTab(currentTab, i18n("Commentary windows"));

	m_settings.keys.commentary.accel = new KAccel(this); //delete in destructor
	CBTConfig::setupAccel( CBTConfig::commentaryWindow, m_settings.keys.commentary.accel  );
	CCommentaryReadWindow::insertKeyboardActions( m_settings.keys.commentary.accel );
//	m_settings.keys.commentary.accel->setConfigGroup("Lexicon shortcuts");
	m_settings.keys.commentary.accel->readSettings();

 	m_settings.keys.commentary.keyChooser = new KKeyChooser( m_settings.keys.commentary.accel, currentTab, false );
// 	QToolTip::add(m_settings.keys.commentary.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_COMMENTARY);
//	QWhatsThis::add(m_settings.keys.commentary.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_COMMENTARY);

// ----- new tab: Lexicon windows ------ //
	currentTab = new QVBox(tabCtl);
	currentTab->setMargin(3);
	tabCtl->addTab(currentTab, i18n("Lexicon windows"));

	m_settings.keys.lexicon.accel = new KAccel(this); //delete in destructor
	CBTConfig::setupAccel( CBTConfig::lexiconWindow, m_settings.keys.lexicon.accel  );
	CLexiconReadWindow::insertKeyboardActions( m_settings.keys.lexicon.accel );
//	m_settings.keys.lexicon.accel->setConfigGroup("Lexicon shortcuts");
	m_settings.keys.lexicon.accel->readSettings();

 	m_settings.keys.lexicon.keyChooser = new KKeyChooser( m_settings.keys.lexicon.accel, currentTab, false );
// 	QToolTip::add(m_settings.keys.lexicon.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_LEXICON);
//	QWhatsThis::add(m_settings.keys.lexicon.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_LEXICON);


// ----- new tab: Book windows ------ //
	currentTab = new QVBox(tabCtl);
	currentTab->setMargin(3);
	tabCtl->addTab(currentTab, i18n("Book windows"));

	m_settings.keys.book.accel = new KAccel(this); //delete in destructor
	CBTConfig::setupAccel( CBTConfig::bookWindow, m_settings.keys.book.accel  );
	CBookReadWindow::insertKeyboardActions( m_settings.keys.book.accel );
//	m_settings.keys.book.accel->setConfigGroup("Book shortcuts");
	m_settings.keys.book.accel->readSettings();

	m_settings.keys.book.keyChooser = new KKeyChooser( m_settings.keys.book.accel, currentTab, false );
// 	QToolTip::add(m_settings.keys.book.keyChooser, TT_OD_DISPLAY_WINDOW_KEYS_LEXICON);
//	QWhatsThis::add(m_settings.keys.book.keyChooser, WT_OD_DISPLAY_WINDOW_KEYS_LEXICON);
}

/** Init Sword section. */
void COptionsDialog::initSword(){
	QVBox* page = addVBoxPage(i18n("Sword"),QString::null, DesktopIcon(CResMgr::settings::sword::icon,32));
  KTabCtl* tabCtl = new KTabCtl(page);
  QFrame* currentTab = new QFrame(tabCtl);
  tabCtl->addTab(currentTab, i18n("General"));

  QGridLayout* gridLayout = new QGridLayout(currentTab,7,2,5,5);
	gridLayout->setResizeMode(QLayout::Minimum);

  gridLayout->addMultiCellWidget(
  	CToolClass::explanationLabel(currentTab, i18n("Use key cache for lexicons"),
			i18n("BibleTime can create a key cache for lexicons. This significantly increases the speed of opening large lexicon modules. \
Note: These files consume some disk space (usually not much), and reside in \
$KDEHOME/share/apps/bibletime/cache.")),
		0,0,0,-1
  );

	m_settings.swords.lexiconCache = new QCheckBox(currentTab);
	m_settings.swords.lexiconCache->setText(i18n("Use key cache for lexicons"));
	QToolTip::add(m_settings.swords.lexiconCache, CResMgr::settings::sword::general::cacheKeys::tooltip);	
	QWhatsThis::add(m_settings.swords.lexiconCache, CResMgr::settings::sword::general::cacheKeys::whatsthis);
		
	m_settings.swords.lexiconCache->setChecked( CBTConfig::get(CBTConfig::lexiconCache) );
 	gridLayout->addMultiCellWidget(m_settings.swords.lexiconCache,1,1,0,-1);

  gridLayout->addMultiCellWidget(
  	CToolClass::explanationLabel(currentTab, i18n("Scrolling behaviour"),
			i18n("The down arrow moves to the <i>next</i> verse by default. Uncheck this box \
if you want it to move to the <i>previous</i> verse.")),
		2,2,0,-1
  );
 		
 	m_settings.swords.useDownArrow = new QCheckBox(currentTab);
 	m_settings.swords.useDownArrow->setText(i18n("Use down arrow to scroll to next verse"));
 	m_settings.swords.useDownArrow->setChecked(CBTConfig::get(CBTConfig::scroll));		
 	QToolTip::add(m_settings.swords.useDownArrow, CResMgr::settings::sword::general::scrolling::tooltip);
 	QWhatsThis::add(m_settings.swords.useDownArrow, CResMgr::settings::sword::general::scrolling::whatsthis);
 	gridLayout->addMultiCellWidget(m_settings.swords.useDownArrow,3,3,0,-1);

  gridLayout->addMultiCellWidget(
  	CToolClass::explanationLabel(currentTab, i18n("Specify a language for biblical booknames"),
			i18n("Sword has a number of locales available which can be used to internationalize the \
booknames of the bible. You can specify which locale to choose. If you want to \
create a new locale, see http://www.crosswire.org/sword/develop for details.")),
		4,4,0,-1
  );
		
 	m_settings.swords.localeCombo = new QComboBox(currentTab);
 	QLabel* label = new QLabel(m_settings.swords.localeCombo, i18n("Language for booknames"), currentTab); 	
 	QToolTip::add(m_settings.swords.localeCombo, CResMgr::settings::sword::general::language::tooltip);				
 	QWhatsThis::add(m_settings.swords.localeCombo, CResMgr::settings::sword::general::language::whatsthis);
 	gridLayout->addWidget(label, 5,0);
 	gridLayout->addWidget(m_settings.swords.localeCombo, 5,1);

 	gridLayout->setRowStretch(6,5); //eat up remaining space :)
 	 	 										
 	m_settings.swords.localeCombo->insertItem( i18n("English") );
 	list <string> locales = sword::LocaleMgr::systemLocaleMgr.getAvailableLocales();
 	for (list <string>::iterator it = locales.begin(); it != locales.end(); it++) {
 		m_settings.swords.localeCombo->insertItem( i18n(sword::LocaleMgr::systemLocaleMgr.getLocale((*it).c_str())->getDescription()) );
 	}

 	int current_item = -1;
 	for(int test_item = 0; test_item < m_settings.swords.localeCombo->count(); test_item++) {
 		sword::SWLocale* locale = sword::LocaleMgr::systemLocaleMgr.getLocale(CBTConfig::get(CBTConfig::language).local8Bit());
 		if (locale && m_settings.swords.localeCombo->text(test_item).contains(i18n(locale->getDescription())) )
 			current_item = test_item;
 	}
 	if (current_item!=-1)
 		m_settings.swords.localeCombo->setCurrentItem(current_item);


// ---------- new tab: Default modules -------- //
  currentTab = new QFrame(tabCtl);
  tabCtl->addTab(currentTab, i18n("Default modules"));
	gridLayout = new QGridLayout(currentTab,10,2, 5,5); //the last row is for stretching available space
	gridLayout->setResizeMode(QLayout::Minimum);

  gridLayout->addMultiCellWidget(
  	CToolClass::explanationLabel(currentTab, i18n("Default modules"), i18n("Default modules are used, when no module is specified, for example when a hyperlink is clicked into a Bible or Lexicon.")),
  	0,0,0,-1 /*fill the horizontal space*/
  );

 	m_settings.swords.standardBible = new QComboBox(currentTab);
  label = new QLabel(m_settings.swords.standardBible, i18n("Default Bible"), currentTab);
  label->setAutoResize(true);
 	QToolTip::add(m_settings.swords.standardBible, CResMgr::settings::sword::modules::bible::tooltip);	
 	QWhatsThis::add(m_settings.swords.standardBible, CResMgr::settings::sword::modules::bible::whatsthis);
 	gridLayout->addWidget(label,1,0); 	
 	gridLayout->addWidget(m_settings.swords.standardBible,1,1);
 	 	
 	m_settings.swords.standardCommentary = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardCommentary, i18n("Default Commentary"), currentTab);
  label->setAutoResize(true); 	
 	QToolTip::add(m_settings.swords.standardCommentary, CResMgr::settings::sword::modules::commentary::tooltip);	
 	QWhatsThis::add(m_settings.swords.standardCommentary, CResMgr::settings::sword::modules::commentary::whatsthis);	
 	gridLayout->addWidget(label,2,0);
 	gridLayout->addWidget(m_settings.swords.standardCommentary,2,1);


 	m_settings.swords.standardLexicon = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardLexicon, i18n("Default Lexicon"), currentTab);
  label->setAutoResize(true); 	
 	QToolTip::add(m_settings.swords.standardLexicon, CResMgr::settings::sword::modules::lexicon::tooltip);	
 	QWhatsThis::add(m_settings.swords.standardLexicon, CResMgr::settings::sword::modules::lexicon::whatsthis);
 	gridLayout->addWidget(label,3,0); 	
 	gridLayout->addWidget(m_settings.swords.standardLexicon,3,1); 	

 	m_settings.swords.standardDailyDevotional = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardDailyDevotional, i18n("Default Daily Devotional"), currentTab);
  label->setAutoResize(true);
 	QToolTip::add(m_settings.swords.standardDailyDevotional, CResMgr::settings::sword::modules::dailyDevotional::tooltip);
 	QWhatsThis::add(m_settings.swords.standardDailyDevotional, CResMgr::settings::sword::modules::dailyDevotional::whatsthis);
 	gridLayout->addWidget(label,4,0);
 	gridLayout->addWidget(m_settings.swords.standardDailyDevotional,4,1);
    		
 	m_settings.swords.standardHebrewStrong = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardHebrewStrong, i18n("Default Hebrew Strong's Lexicon"), currentTab);
  label->setAutoResize(true); 	 	
 	QToolTip::add(m_settings.swords.standardHebrewStrong, CResMgr::settings::sword::modules::hebrewStrongs::tooltip);
 	QWhatsThis::add(m_settings.swords.standardHebrewStrong, CResMgr::settings::sword::modules::hebrewStrongs::whatsthis);
 	gridLayout->addWidget(label,5,0);
 	gridLayout->addWidget(m_settings.swords.standardHebrewStrong,5,1);
		
 	m_settings.swords.standardGreekStrong = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardGreekStrong, i18n("Default Greek Strong's Lexicon"), currentTab);
  label->setAutoResize(true); 	 	
 	QToolTip::add(m_settings.swords.standardGreekStrong, CResMgr::settings::sword::modules::greekStrongs::tooltip); 	
 	QWhatsThis::add(m_settings.swords.standardGreekStrong, CResMgr::settings::sword::modules::greekStrongs::whatsthis);
 	gridLayout->addWidget(label,6,0);
 	gridLayout->addWidget(m_settings.swords.standardGreekStrong,6,1);
		
 	m_settings.swords.standardHebrewMorph = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardHebrewMorph, i18n("Default Hebrew Morphological Lexicon"), currentTab);
 	QToolTip::add(m_settings.swords.standardHebrewMorph, CResMgr::settings::sword::modules::hebrewMorph::tooltip);
 	QWhatsThis::add(m_settings.swords.standardHebrewMorph, CResMgr::settings::sword::modules::hebrewMorph::whatsthis);
 	gridLayout->addWidget(label,7,0);
 	gridLayout->addWidget(m_settings.swords.standardHebrewMorph,7,1);

 	m_settings.swords.standardGreekMorph = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardGreekMorph, i18n("Default Greek Morphological Lexicon"), currentTab);
  label->setAutoResize(true); 	 	
 	QToolTip::add(m_settings.swords.standardGreekMorph, CResMgr::settings::sword::modules::greekMorph::tooltip);
 	QWhatsThis::add(m_settings.swords.standardGreekMorph, CResMgr::settings::sword::modules::greekMorph::whatsthis);
 	gridLayout->addWidget(label,8,0);
 	gridLayout->addWidget(m_settings.swords.standardGreekMorph,8,1);

 	gridLayout->setRowStretch(9,5);							
		
 	//fill the comboboxes with the right modules
 	ListCSwordModuleInfo& modules = backend()->moduleList();
	QString modDescript;
  for ( modules.first(); modules.current(); modules.next() ) {
		modDescript = modules.current()->config(CSwordModuleInfo::Description);
 		switch (modules.current()->type()) {
 			case CSwordModuleInfo::Bible:
 				m_settings.swords.standardBible->insertItem(modDescript);
 				break;
 			case CSwordModuleInfo::Commentary:
 				m_settings.swords.standardCommentary->insertItem(modDescript);				
 				break;
 			case CSwordModuleInfo::Lexicon:
 			{
        bool inserted = false;
 				if (modules.current()->has(CSwordModuleInfo::HebrewDef)) {
					m_settings.swords.standardHebrewStrong->insertItem(modDescript);
          inserted = true;          
 				}
 				if (modules.current()->has(CSwordModuleInfo::GreekDef)) {
					m_settings.swords.standardGreekStrong->insertItem(modDescript);
          inserted = true;
 				}
 				if (modules.current()->has(CSwordModuleInfo::HebrewParse)) {
					m_settings.swords.standardHebrewMorph->insertItem(modDescript);				
          inserted = true;
        }
 				if (modules.current()->has(CSwordModuleInfo::GreekParse)) {       
					m_settings.swords.standardGreekMorph->insertItem(modDescript);
          inserted = true;
 				}
 				if (modules.current()->category() == CSwordModuleInfo::DailyDevotional) {
					m_settings.swords.standardDailyDevotional->insertItem(modDescript);
          inserted = true;
 				}
				if (!inserted)//daily dvotionals, striong lexicons etc. are not very useful for word lookups
          m_settings.swords.standardLexicon->insertItem(modDescript);
 				break;
 			} 				
 			default://unknown type					
 				break;
 		}
  }

//using two lists and one loop is better than six loops with almost the same code :)
 	QPtrList<QComboBox> comboList;
 	comboList.setAutoDelete(false);//don't delete the combos accidentally
 	comboList.append(m_settings.swords.standardBible);
 	comboList.append(m_settings.swords.standardCommentary);
 	comboList.append(m_settings.swords.standardLexicon);
 	comboList.append(m_settings.swords.standardDailyDevotional);  
 	comboList.append(m_settings.swords.standardHebrewStrong);
 	comboList.append(m_settings.swords.standardGreekStrong);
 	comboList.append(m_settings.swords.standardHebrewMorph);
 	comboList.append(m_settings.swords.standardGreekMorph);

 	QStringList moduleList;
 	moduleList
 		<< CBTConfig::get(CBTConfig::standardBible)
	 	<< CBTConfig::get(CBTConfig::standardCommentary)
	 	<< CBTConfig::get(CBTConfig::standardLexicon)
	 	<< CBTConfig::get(CBTConfig::standardDailyDevotional)    
	 	<< CBTConfig::get(CBTConfig::standardHebrewStrongsLexicon)
	 	<< CBTConfig::get(CBTConfig::standardGreekStrongsLexicon)
	 	<< CBTConfig::get(CBTConfig::standardHebrewMorphLexicon)
	 	<< CBTConfig::get(CBTConfig::standardGreekMorphLexicon);

 	QString module = QString::null;
 	int item = 0;
	int count = 0; 	
 	for (QComboBox* combo = comboList.first(); combo; combo = comboList.next() ) {		
		module = moduleList[comboList.at()];
		count = combo->count();
	  combo->setMaximumWidth(300);
 		for (item = 0; item < count; item++) {
	 		if (combo->text(item) == module ) {
	 		  combo->setCurrentItem(item);
	 		  break;
	 		}
	 	}
	}

	

// ---------- new tab: filters -------- //
  currentTab = new QFrame(tabCtl);
  tabCtl->addTab(currentTab, i18n("Filter settings"));
  QVBoxLayout* layout = new QVBoxLayout(currentTab,5);

  layout->addWidget( CToolClass::explanationLabel(currentTab, i18n("Filter settings"), i18n("Filters control the appereance of the text in the display windows. Here you can choose the settings of the default filter. You can also change the settings in each display window.")) );
  layout->addSpacing(5);
  		
 	m_settings.swords.lineBreaks = new QCheckBox(currentTab);
 	m_settings.swords.lineBreaks->setText(i18n("Show line break after each verse"));
 	m_settings.swords.lineBreaks->setChecked(CBTConfig::get(CBTConfig::lineBreaks));
  QToolTip::add(m_settings.swords.lineBreaks, CResMgr::settings::sword::filters::lineBreaks::tooltip);
  QWhatsThis::add(m_settings.swords.lineBreaks, CResMgr::settings::sword::filters::lineBreaks::whatsthis);  
 	layout->addWidget(m_settings.swords.lineBreaks);

 	m_settings.swords.verseNumbers = new QCheckBox(currentTab);
 	m_settings.swords.verseNumbers->setText(i18n("Show verse numbers"));
 	m_settings.swords.verseNumbers->setChecked(CBTConfig::get(CBTConfig::verseNumbers));		
  QToolTip::add(m_settings.swords.verseNumbers, CResMgr::settings::sword::filters::verseNumbers::tooltip);
  QWhatsThis::add(m_settings.swords.verseNumbers, CResMgr::settings::sword::filters::verseNumbers::whatsthis);
 	layout->addWidget(m_settings.swords.verseNumbers);

 	m_settings.swords.footnotes = new QCheckBox(currentTab);
 	m_settings.swords.footnotes->setText(i18n("Show footnotes"));
 	m_settings.swords.footnotes->setChecked(CBTConfig::get(CBTConfig::footnotes));		
  QToolTip::add(m_settings.swords.footnotes, CResMgr::settings::sword::filters::footnotes::tooltip);
  QWhatsThis::add(m_settings.swords.footnotes, CResMgr::settings::sword::filters::footnotes::whatsthis);
 	layout->addWidget(m_settings.swords.footnotes);

 	m_settings.swords.strongNumbers = new QCheckBox(currentTab);
 	m_settings.swords.strongNumbers->setText(i18n("Show Strong's Numbers"));
 	m_settings.swords.strongNumbers->setChecked(CBTConfig::get(CBTConfig::strongNumbers));
  QToolTip::add(m_settings.swords.strongNumbers, CResMgr::settings::sword::filters::strongsNumbers::tooltip);
  QWhatsThis::add(m_settings.swords.strongNumbers, CResMgr::settings::sword::filters::strongsNumbers::whatsthis);  
 	layout->addWidget(m_settings.swords.strongNumbers);

 	m_settings.swords.morphTags = new QCheckBox(currentTab);
 	m_settings.swords.morphTags->setText(i18n("Show morphologic tags"));
 	m_settings.swords.morphTags->setChecked(CBTConfig::get(CBTConfig::morphTags));
  QToolTip::add(m_settings.swords.morphTags, CResMgr::settings::sword::filters::morphTags::tooltip);
  QWhatsThis::add(m_settings.swords.morphTags, CResMgr::settings::sword::filters::morphTags::whatsthis);
 	layout->addWidget(m_settings.swords.morphTags);

 	m_settings.swords.headings = new QCheckBox(currentTab);
 	m_settings.swords.headings->setText(i18n("Show headings"));
 	m_settings.swords.headings->setChecked(CBTConfig::get(CBTConfig::headings));
  QToolTip::add(m_settings.swords.headings, CResMgr::settings::sword::filters::headings::tooltip);
  QWhatsThis::add(m_settings.swords.headings, CResMgr::settings::sword::filters::headings::whatsthis);  
 	layout->addWidget(m_settings.swords.headings);

 	m_settings.swords.lemmas = new QCheckBox(currentTab);
 	m_settings.swords.lemmas->setText(i18n("Show lemmas"));
 	m_settings.swords.lemmas->setChecked(CBTConfig::get(CBTConfig::lemmas));
  QToolTip::add(m_settings.swords.lemmas, CResMgr::settings::sword::filters::lemmas::tooltip);
  QWhatsThis::add(m_settings.swords.lemmas, CResMgr::settings::sword::filters::lemmas::whatsthis);
 	layout->addWidget(m_settings.swords.lemmas);
		
 	m_settings.swords.hebrewPoints = new QCheckBox(currentTab);
 	m_settings.swords.hebrewPoints->setText(i18n("Show Hebrew vowel points"));
 	m_settings.swords.hebrewPoints->setChecked(CBTConfig::get(CBTConfig::hebrewPoints));
  QToolTip::add(m_settings.swords.hebrewPoints, CResMgr::settings::sword::filters::hebrewVowelPoints::tooltip);
  QWhatsThis::add(m_settings.swords.hebrewPoints, CResMgr::settings::sword::filters::hebrewVowelPoints::whatsthis);  
 	layout->addWidget(m_settings.swords.hebrewPoints);

 	m_settings.swords.hebrewCantillation = new QCheckBox(currentTab);
 	m_settings.swords.hebrewCantillation->setText(i18n("Show Hebrew cantillation marks"));
 	m_settings.swords.hebrewCantillation->setChecked(CBTConfig::get(CBTConfig::hebrewCantillation));		
  QToolTip::add(m_settings.swords.hebrewCantillation, CResMgr::settings::sword::filters::hebrewCantillation::tooltip);
  QWhatsThis::add(m_settings.swords.hebrewCantillation, CResMgr::settings::sword::filters::hebrewCantillation::whatsthis);
 	layout->addWidget(m_settings.swords.hebrewCantillation);

 	m_settings.swords.greekAccents = new QCheckBox(currentTab);
 	m_settings.swords.greekAccents->setText(i18n("Show Greek accents"));
 	m_settings.swords.greekAccents->setChecked(CBTConfig::get(CBTConfig::greekAccents));		
  QToolTip::add(m_settings.swords.greekAccents, CResMgr::settings::sword::filters::greekAccents::tooltip);
  QWhatsThis::add(m_settings.swords.greekAccents, CResMgr::settings::sword::filters::greekAccents::whatsthis);
 	layout->addWidget(m_settings.swords.greekAccents);

 	m_settings.swords.textualVariants = new QCheckBox(currentTab);
 	m_settings.swords.textualVariants->setText(i18n("Use textual variants"));
 	m_settings.swords.textualVariants->setChecked(CBTConfig::get(CBTConfig::textualVariants));		
  QToolTip::add(m_settings.swords.textualVariants, CResMgr::settings::sword::filters::textualVariants::tooltip);
  QWhatsThis::add(m_settings.swords.textualVariants, CResMgr::settings::sword::filters::textualVariants::whatsthis);
 	layout->addWidget(m_settings.swords.textualVariants);
 		
	layout->addStretch(4);	
}

void COptionsDialog::saveAccelerators(){
//#warning check
// 	m_settings.keys.application.accel->writeSettings();
// 	m_settings.keys.application.keyChooser->save();
 	m_settings.keys.application.keyChooser->commitChanges();
	m_settings.keys.application.accel->writeSettings(); 	

// 	m_settings.keys.general.accel->writeSettings();
//		
// 	m_settings.keys.bible.accel->writeSettings();		
// 	m_settings.keys.bible.keyChooser->save();
 	m_settings.keys.bible.keyChooser->commitChanges();
	m_settings.keys.bible.accel->writeSettings();

//		
// 	m_settings.keys.commentary.accel->writeSettings();
// 	m_settings.keys.commentary.keyChooser->save();
 	m_settings.keys.commentary.keyChooser->commitChanges();
	m_settings.keys.commentary.accel->writeSettings(); 	

		
// 	m_settings.keys.lexicon.accel->writeSettings();
// 	m_settings.keys.lexicon.keyChooser->save();
 	m_settings.keys.lexicon.keyChooser->commitChanges();
	m_settings.keys.lexicon.accel->writeSettings(); 	

// 	m_settings.keys.book.accel->writeSettings(); 	 	
// 	m_settings.keys.book.keyChooser->save();
 	m_settings.keys.book.keyChooser->commitChanges();
	m_settings.keys.book.accel->writeSettings();
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
	for(QMap<QString, CBTConfig::FontSettingsPair>::Iterator it = m_settings.fonts.fontMap.begin(); it != m_settings.fonts.fontMap.end(); ++it ) {
    CLanguageMgr::Language lang = languageMgr()->languageForTranslatedName(it.key());
//    if (it.data().first) {//true == use own font settings
      CBTConfig::set(lang, it.data());
//    }
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
  bool new_lexiconCache = m_settings.swords.lexiconCache->isChecked();
   		
  CBTConfig::set( CBTConfig::lexiconCache, new_lexiconCache );	

  if (old_lexiconCache && !new_lexiconCache){  //delete cache files
  	QString dirname = KGlobal::dirs()->saveLocation("data", "bibletime/cache/");
  	QDir dir = QDir(dirname);
   	QStringList files = QStringList( dir.entryList() );
   	for (QStringList::Iterator it = files.begin(); it != files.end(); ++it)
   		dir.remove((*it),false);			
  }
       	
  CBTConfig::set(CBTConfig::standardBible, m_settings.swords.standardBible->currentText());
  CBTConfig::set(CBTConfig::standardCommentary, m_settings.swords.standardCommentary->currentText());
  CBTConfig::set(CBTConfig::standardLexicon, m_settings.swords.standardLexicon->currentText());
  CBTConfig::set(CBTConfig::standardDailyDevotional, m_settings.swords.standardDailyDevotional->currentText());
  CBTConfig::set(CBTConfig::standardHebrewStrongsLexicon, m_settings.swords.standardHebrewStrong->currentText());
  CBTConfig::set(CBTConfig::standardGreekStrongsLexicon, m_settings.swords.standardGreekStrong->currentText() );  	
  CBTConfig::set(CBTConfig::standardHebrewMorphLexicon, m_settings.swords.standardHebrewMorph->currentText());
  CBTConfig::set(CBTConfig::standardGreekMorphLexicon, m_settings.swords.standardGreekMorph->currentText() );  	

 	const QString currentText = m_settings.swords.localeCombo->currentText();
 	list <string> locales =sword:: LocaleMgr::systemLocaleMgr.getAvailableLocales();
 	QString localeName = QString::null;
 	for (list <string>::iterator it = locales.begin(); it != locales.end(); it++) {
 		if ( i18n(sword::LocaleMgr::systemLocaleMgr.getLocale((*it).c_str())->getDescription()) == currentText ) {
 			localeName = (*it).c_str();	//we found the abbrevation for the current language
 			break;
 		}
 	}
 	if (!localeName.isEmpty())
 		CBTConfig::set(CBTConfig::language, localeName);
 	else
 		CBTConfig::set(CBTConfig::language, currentText);

 	CBTConfig::set(CBTConfig::scroll, m_settings.swords.useDownArrow->isChecked());
 	CBTConfig::set(CBTConfig::lineBreaks, m_settings.swords.lineBreaks->isChecked());
 	CBTConfig::set(CBTConfig::verseNumbers, m_settings.swords.verseNumbers->isChecked());
 	CBTConfig::set(CBTConfig::footnotes, m_settings.swords.footnotes->isChecked());
 	CBTConfig::set(CBTConfig::strongNumbers, m_settings.swords.strongNumbers->isChecked());
 	CBTConfig::set(CBTConfig::headings, m_settings.swords.headings->isChecked());
 	CBTConfig::set(CBTConfig::morphTags, m_settings.swords.morphTags->isChecked());
 	CBTConfig::set(CBTConfig::lemmas, m_settings.swords.lemmas->isChecked());
 	CBTConfig::set(CBTConfig::hebrewPoints, m_settings.swords.hebrewPoints->isChecked());
 	CBTConfig::set(CBTConfig::hebrewCantillation, m_settings.swords.hebrewCantillation->isChecked());
 	CBTConfig::set(CBTConfig::greekAccents, m_settings.swords.greekAccents->isChecked());
 	CBTConfig::set(CBTConfig::textualVariants, m_settings.swords.textualVariants->isChecked()); 		 	
}

/** This slot is called when the "Use own font for language" bo was clicked. */
void COptionsDialog::useOwnFontClicked( bool isOn){

  m_settings.fonts.fontChooser->setEnabled(isOn);
  m_settings.fonts.fontMap[ m_settings.fonts.usage->currentText() ].first = isOn;

  if (isOn) //show font icon
	 		m_settings.fonts.usage->changeItem(SmallIcon("fonts"), m_settings.fonts.usage->currentText(), m_settings.fonts.usage->currentItem() );
		else    //don't show
	 		m_settings.fonts.usage->changeItem(m_settings.fonts.usage->currentText(), m_settings.fonts.usage->currentItem() );
}
