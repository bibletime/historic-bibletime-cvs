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
#include "backend/cdisplaytemplatemgr.h"
#include "backend/cdisplayrendering.h"

#include "frontend/displaywindow/cbiblereadwindow.h"
#include "frontend/displaywindow/ccommentaryreadwindow.h"
#include "frontend/displaywindow/clexiconreadwindow.h"
#include "frontend/displaywindow/cbookreadwindow.h"
#include "frontend/displaywindow/creadwindow.h"

#include "util/cresmgr.h"
#include "util/ctoolclass.h"

#include <stdio.h>
#include <stdlib.h>

//QT includes
#include <qhbox.h>
#include <qlayout.h>
#include <qvbox.h>
#include <qdict.h>
#include <qcheckbox.h>
#include <qcombobox.h>
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
#include <khtml_part.h>
#include <khtmlview.h>
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
	initDisplayStyle();
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
	saveDisplayStyle();
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
	saveDisplayStyle();
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
	QString name = QInputDialog::getText(i18n("Create new session"), i18n("Please enter a name for the new session."), QLineEdit::Normal, QString::null, &ok);
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
	const QString newName = QInputDialog::getText(i18n("Rename session"), i18n("Please enter a new name for the session."), QLineEdit::Normal, profile->name(), &ok);
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
		m_settings.startup.restoreWorkspace->setText(i18n("Restore windows from the last BibleTime session"));
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
      i18n("Select custom fonts per-language"),
      i18n("Here you find a list of all languages of the installed works. \
You can specify a custom font for each language that needs a special font \
to be displayed correctly.")
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

  for ( CLanguageMgr::LangMapIterator it( langMap ); it.current(); ++it ) {
    const QString name = it.current()->translatedName().isEmpty() ? it.current()->abbrev() : it.current()->translatedName();
    
		m_settings.fonts.fontMap.insert(name, CBTConfig::get(it.current()) );
  }

  for( QMap<QString, CBTConfig::FontSettingsPair>::Iterator it = m_settings.fonts.fontMap.begin(); it != m_settings.fonts.fontMap.end(); ++it ) {
		if ( m_settings.fonts.fontMap[it.key()].first ) { //show font icon
	 		m_settings.fonts.usage->insertItem(SmallIcon("fonts"), it.key() );
    }
		else {	//don't show icon for font
	 		m_settings.fonts.usage->insertItem(it.key());
    }
  }


  m_settings.fonts.useOwnFontBox = new QCheckBox(i18n("Use custom font"), page, "font checkbox");
  connect(m_settings.fonts.useOwnFontBox, SIGNAL(toggled(bool)), SLOT(useOwnFontClicked(bool)));
 	hLayout->addWidget(m_settings.fonts.useOwnFontBox);


  layout->addLayout(hLayout);
//#warning TODO: remember the last selected font and jump there.

 	m_settings.fonts.fontChooser = new KFontChooser(page, "fonts", false, QStringList(), true, 5);
 	m_settings.fonts.fontChooser->setSampleText(i18n("The quick brown fox jumps over the lazy dog."));
 	layout->addWidget(m_settings.fonts.fontChooser);

  connect(m_settings.fonts.fontChooser, SIGNAL(fontSelected(const QFont&)), SLOT(newDisplayWindowFontSelected(const QFont&)));
  connect(m_settings.fonts.usage, SIGNAL(activated(const QString&)), SLOT(newDisplayWindowFontAreaSelected(const QString&)));

 	m_settings.fonts.fontChooser->setFont( m_settings.fonts.fontMap[m_settings.fonts.usage->currentText()].second );
  useOwnFontClicked( m_settings.fonts.fontMap[m_settings.fonts.usage->currentText()].first );
	m_settings.fonts.useOwnFontBox->setChecked( m_settings.fonts.fontMap[m_settings.fonts.usage->currentText()].first );
 	m_settings.fonts.fontChooser->setMinimumSize(m_settings.fonts.fontChooser->sizeHint());
}

/** Init color section. */
void COptionsDialog::initDisplayStyle(){
	QFrame* page = addPage(i18n("Display templates"), QString::null, DesktopIcon(CResMgr::settings::colors::icon,32));
	QGridLayout* gridLayout = new QGridLayout(page,8,5,5,5);
  gridLayout->setResizeMode(QLayout::Minimum);

	gridLayout->addMultiCellWidget(
		CToolClass::explanationLabel(page,
			i18n("Display templates"),
			i18n("Display templates define how text is displayed. Please choose a template you like.")
		),
		0,0,0,-1
	);

	m_settings.displayStyle.styleChooser = new QListBox(page);
	connect(
		m_settings.displayStyle.styleChooser, SIGNAL(selectionChanged()), 
		this, SLOT(updateStylePreview())
	);
	
	m_settings.displayStyle.stylePreview = new KHTMLPart(page);

	QLabel* label = new QLabel(m_settings.displayStyle.styleChooser, i18n("Available display styles:"), page);
	gridLayout->addMultiCellWidget(label, 1, 1, 0, 1);
	gridLayout->addMultiCellWidget(m_settings.displayStyle.styleChooser, 2, 2, 0, 1);

	label = new QLabel(m_settings.displayStyle.stylePreview->view(), i18n("Style preview:"), page);
	gridLayout->addMultiCellWidget(label, 1, 1, 2, 4);
	gridLayout->addMultiCellWidget(m_settings.displayStyle.stylePreview->view(), 2, 2, 2, 4);

	CDisplayTemplateMgr* tMgr = CPointers::displayTemplateManager();
	m_settings.displayStyle.styleChooser->insertStringList( tMgr->availableTemplates() );
	QListBoxItem*  i = m_settings.displayStyle.styleChooser->findItem( CBTConfig::get(CBTConfig::displayStyle), Qt::CaseSensitive );
	
	if ( i ) {
		m_settings.displayStyle.styleChooser->setCurrentItem( i );
	}
}

/** Init profiles section. */
void COptionsDialog::initProfiles(){
	QFrame* page = addPage(i18n("Sessions"),QString::null, DesktopIcon(CResMgr::settings::profiles::icon,32));
	QGridLayout* gridLayout = new QGridLayout(page, 3,3,5,5);

	gridLayout->addMultiCellWidget(
		CToolClass::explanationLabel(page,
			i18n("Manage sessions"),
			i18n("Sessions define the appereance of your desk, \
				for example which windows are open and which texts should displayed in these windows. \
				New sessions only work after you've saved something into them.")
		),
		0,0,0,-1
	);

	m_settings.profiles.profiles = new QListBox(page);
	gridLayout->addMultiCellWidget(m_settings.profiles.profiles, 1,1,0,-1);
	gridLayout->setRowStretch(1,10);

	m_settings.profiles.createProfile = new QPushButton(i18n("Create new session"), page);
	connect(m_settings.profiles.createProfile, SIGNAL(clicked()), SLOT(addNewProfile()));
  gridLayout->addWidget(m_settings.profiles.createProfile,2,0);

	m_settings.profiles.deleteProfile = new QPushButton(i18n("Delete selected session"), page);
	connect(m_settings.profiles.deleteProfile, SIGNAL(clicked()), SLOT(deleteProfile()));
  gridLayout->addWidget(m_settings.profiles.deleteProfile,2,1);

	m_settings.profiles.renameProfile = new QPushButton(i18n("Rename selected session"), page);
	connect(m_settings.profiles.renameProfile, SIGNAL(clicked()), SLOT(renameProfile()));
  gridLayout->addWidget(m_settings.profiles.renameProfile,2,2);

  //fill the profile list box
	QPtrList<CProfile> profiles = m_settings.profiles.mgr.profiles();
	if (CProfile* p = profiles.first()) { //better than count
		for (; p; p = profiles.next()) {
			m_settings.profiles.profiles->insertItem(p->name());
		}
	}
	else {
		m_settings.profiles.profiles->setEnabled(false);
	}
}

/** Init accel key section. */
void COptionsDialog::initAccelerators(){
  QVBox* page = addVBoxPage(i18n("HotKeys"),QString::null, DesktopIcon(CResMgr::settings::keys::icon,32));

  KTabCtl* tabCtl = new KTabCtl(page);

// ----- new tab: All display windows ------ //
  QFrame* currentTab = new QVBox(tabCtl);
	currentTab->setMargin(3);
  tabCtl->addTab(currentTab, i18n("Application-wide"));

	CBTConfig::setupAccel( CBTConfig::application, m_settings.keys.application.accel  );
//	CSwordPresenter::insertKeyboardActions( m_settings.keys.application.accel );
	m_settings.keys.application.accel->readSettings();

 	m_settings.keys.application.keyChooser = new KKeyChooser( m_settings.keys.application.accel, currentTab, false );
	
	
// 	QToolTip::add(m_settings.keys.application.keyChooser, TT_OD_KEYS_CHOOSER);
//	QWhatsThis::add(m_settings.keys.application.keyChooser, WT_OD_KEYS_CHOOSER);

// ----- new tab: All display windows ------ //
	currentTab = new QVBox(tabCtl);
	currentTab->setMargin(3);
	tabCtl->addTab(currentTab, i18n("All read windows"));

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
	QVBox* page = addVBoxPage(i18n("Desk"),QString::null, DesktopIcon(CResMgr::settings::sword::icon,32));
  KTabCtl* tabCtl = new KTabCtl(page);
  QFrame* currentTab = new QFrame(tabCtl);
  tabCtl->addTab(currentTab, i18n("Navigation"));

  QGridLayout* gridLayout = new QGridLayout(currentTab,7,2,5,5);
	gridLayout->setResizeMode(QLayout::Minimum);

  gridLayout->addMultiCellWidget(
  	CToolClass::explanationLabel(currentTab, i18n("Use key cache for lexicons"),
			i18n("BibleTime can create a key cache for lexicons. \
This increases the speed of opening large lexicon modules significantly. \
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
booknames of the bible. You can specify which locale to use. If you want to \
create a new locale, see http://www.crosswire.org/sword/develop for details.")),
		4,4,0,-1
  );

 	m_settings.swords.localeCombo = new QComboBox(currentTab);
 	QLabel* label = new QLabel(m_settings.swords.localeCombo, i18n("Language for biblical booknames"), currentTab);
 	QToolTip::add(m_settings.swords.localeCombo, CResMgr::settings::sword::general::language::tooltip);
 	QWhatsThis::add(m_settings.swords.localeCombo, CResMgr::settings::sword::general::language::whatsthis);
 	gridLayout->addWidget(label, 5,0);
 	gridLayout->addWidget(m_settings.swords.localeCombo, 5,1);

 	gridLayout->setRowStretch(6,5); //eat up remaining space :)

	QStringList languageNames;
	languageNames.append( i18n("English") );
	
 	const list<sword::SWBuf> locales = sword::LocaleMgr::getSystemLocaleMgr()->getAvailableLocales();
 	for (list<sword::SWBuf>::const_iterator it = locales.begin(); it != locales.end(); it++) {
		const CLanguageMgr::Language* const l = CPointers::languageMgr()->languageForAbbrev(
			sword::LocaleMgr::getSystemLocaleMgr()->getLocale((*it).c_str())->getName()
		);
		
		if (l->isValid()) {
			languageNames.append( l->translatedName() );
		}
		else {
			languageNames.append(
				sword::LocaleMgr::getSystemLocaleMgr()->getLocale((*it).c_str())->getDescription() 
			);
		}
 	}
	
	languageNames.sort();
	m_settings.swords.localeCombo->insertStringList( languageNames );
	
	const CLanguageMgr::Language* const l = CPointers::languageMgr()->languageForAbbrev( 
		CBTConfig::get(CBTConfig::language)
	);
	QString currentLanguageName;
	if ( l->isValid() && languageNames.contains(l->translatedName()) ) { //tranlated language name is in the box
		currentLanguageName = l->translatedName();
	}
	else { //a language like "German Abbrevs" might be the language to set
		sword::SWLocale* locale = sword::LocaleMgr::LocaleMgr::getSystemLocaleMgr()->getLocale(
			CBTConfig::get(CBTConfig::language).local8Bit()
		);
		if (locale) {
			currentLanguageName = QString::fromLatin1(locale->getDescription());
		}
	}
	
	if (currentLanguageName.isEmpty()) { // set english as default if nothing was chosen
		currentLanguageName = i18n("English");
	}
	
	//now set the item with the right name as current item
	for (int i = 0; i < m_settings.swords.localeCombo->count(); ++i) {
		if (currentLanguageName == m_settings.swords.localeCombo->text(i)) {
			m_settings.swords.localeCombo->setCurrentItem(i);
			break; //item found, finish the loop
		}
	}
	

// ---------- new tab: Standard modules -------- //
  currentTab = new QFrame(tabCtl);
  tabCtl->addTab(currentTab, i18n("Standard works"));
	gridLayout = new QGridLayout(currentTab,10,2, 5,5); //the last row is for stretching available space
	gridLayout->setResizeMode(QLayout::Minimum);

  gridLayout->addMultiCellWidget(
  	CToolClass::explanationLabel(currentTab, i18n("Standard works"),
			i18n("Standard works are used when no particular work is specified, \
for example when a hyperlink into a Bible or lexicon was clicked .")),
  	0,0,0,-1 /*fill the horizontal space*/
  );

 	m_settings.swords.standardBible = new QComboBox(currentTab);
  label = new QLabel(m_settings.swords.standardBible, i18n("Standard Bible"), currentTab);
  label->setAutoResize(true);
 	QToolTip::add(m_settings.swords.standardBible, CResMgr::settings::sword::modules::bible::tooltip);
 	QWhatsThis::add(m_settings.swords.standardBible, CResMgr::settings::sword::modules::bible::whatsthis);
 	gridLayout->addWidget(label,1,0);
 	gridLayout->addWidget(m_settings.swords.standardBible,1,1);

 	m_settings.swords.standardCommentary = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardCommentary, i18n("Standard Commentary"), currentTab);
  label->setAutoResize(true);
 	QToolTip::add(m_settings.swords.standardCommentary, CResMgr::settings::sword::modules::commentary::tooltip);
 	QWhatsThis::add(m_settings.swords.standardCommentary, CResMgr::settings::sword::modules::commentary::whatsthis);
 	gridLayout->addWidget(label,2,0);
 	gridLayout->addWidget(m_settings.swords.standardCommentary,2,1);


 	m_settings.swords.standardLexicon = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardLexicon, i18n("Standard Lexicon"), currentTab);
  label->setAutoResize(true);
 	QToolTip::add(m_settings.swords.standardLexicon, CResMgr::settings::sword::modules::lexicon::tooltip);
 	QWhatsThis::add(m_settings.swords.standardLexicon, CResMgr::settings::sword::modules::lexicon::whatsthis);
 	gridLayout->addWidget(label,3,0);
 	gridLayout->addWidget(m_settings.swords.standardLexicon,3,1);

 	m_settings.swords.standardDailyDevotional = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardDailyDevotional, i18n("Standard Daily Devotional"), currentTab);
  label->setAutoResize(true);
 	QToolTip::add(m_settings.swords.standardDailyDevotional, CResMgr::settings::sword::modules::dailyDevotional::tooltip);
 	QWhatsThis::add(m_settings.swords.standardDailyDevotional, CResMgr::settings::sword::modules::dailyDevotional::whatsthis);
 	gridLayout->addWidget(label,4,0);
 	gridLayout->addWidget(m_settings.swords.standardDailyDevotional,4,1);

 	m_settings.swords.standardHebrewStrong = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardHebrewStrong, i18n("Standard Hebrew Strong's Lexicon"), currentTab);
  label->setAutoResize(true);
 	QToolTip::add(m_settings.swords.standardHebrewStrong, CResMgr::settings::sword::modules::hebrewStrongs::tooltip);
 	QWhatsThis::add(m_settings.swords.standardHebrewStrong, CResMgr::settings::sword::modules::hebrewStrongs::whatsthis);
 	gridLayout->addWidget(label,5,0);
 	gridLayout->addWidget(m_settings.swords.standardHebrewStrong,5,1);

 	m_settings.swords.standardGreekStrong = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardGreekStrong, i18n("Standard Greek Strong's Lexicon"), currentTab);
  label->setAutoResize(true);
 	QToolTip::add(m_settings.swords.standardGreekStrong, CResMgr::settings::sword::modules::greekStrongs::tooltip);
 	QWhatsThis::add(m_settings.swords.standardGreekStrong, CResMgr::settings::sword::modules::greekStrongs::whatsthis);
 	gridLayout->addWidget(label,6,0);
 	gridLayout->addWidget(m_settings.swords.standardGreekStrong,6,1);
		
 	m_settings.swords.standardHebrewMorph = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardHebrewMorph, i18n("Standard Hebrew Morphological Lexicon"), currentTab);
 	QToolTip::add(m_settings.swords.standardHebrewMorph, CResMgr::settings::sword::modules::hebrewMorph::tooltip);
 	QWhatsThis::add(m_settings.swords.standardHebrewMorph, CResMgr::settings::sword::modules::hebrewMorph::whatsthis);
 	gridLayout->addWidget(label,7,0);
 	gridLayout->addWidget(m_settings.swords.standardHebrewMorph,7,1);

 	m_settings.swords.standardGreekMorph = new QComboBox(currentTab);
 	label = new QLabel(m_settings.swords.standardGreekMorph, i18n("Standard Greek Morphological Lexicon"), currentTab);
  label->setAutoResize(true); 	 	
 	QToolTip::add(m_settings.swords.standardGreekMorph, CResMgr::settings::sword::modules::greekMorph::tooltip);
 	QWhatsThis::add(m_settings.swords.standardGreekMorph, CResMgr::settings::sword::modules::greekMorph::whatsthis);
 	gridLayout->addWidget(label,8,0);
 	gridLayout->addWidget(m_settings.swords.standardGreekMorph,8,1);

 	gridLayout->setRowStretch(9,5);							
		
 	//fill the comboboxes with the right modules
 	ListCSwordModuleInfo& modules = backend()->moduleList();
	QString modDescript;
/*  for ( modules.first(); modules.current(); modules.next() ) {*/
	ListCSwordModuleInfo::iterator end_it = modules.end();
	for (ListCSwordModuleInfo::iterator it(modules.begin()); it != end_it; ++it) {
		modDescript = (*it)->config(CSwordModuleInfo::Description);
		
 		switch ((*it)->type()) {
 			case CSwordModuleInfo::Bible:
 				m_settings.swords.standardBible->insertItem(modDescript);
 				break;
 			case CSwordModuleInfo::Commentary:
 				m_settings.swords.standardCommentary->insertItem(modDescript);				
 				break;
 			case CSwordModuleInfo::Lexicon:
 			{
        bool inserted = false;
 				if ((*it)->has(CSwordModuleInfo::HebrewDef)) {
					m_settings.swords.standardHebrewStrong->insertItem(modDescript);
          inserted = true;          
 				}
 				if ((*it)->has(CSwordModuleInfo::GreekDef)) {
					m_settings.swords.standardGreekStrong->insertItem(modDescript);
          inserted = true;
 				}
 				if ((*it)->has(CSwordModuleInfo::HebrewParse)) {
					m_settings.swords.standardHebrewMorph->insertItem(modDescript);				
          inserted = true;
        }
 				if ((*it)->has(CSwordModuleInfo::GreekParse)) {       
					m_settings.swords.standardGreekMorph->insertItem(modDescript);
          inserted = true;
 				}
 				if ((*it)->category() == CSwordModuleInfo::DailyDevotional) {
					m_settings.swords.standardDailyDevotional->insertItem(modDescript);
          inserted = true;
 				}
				
				if (!inserted) {//daily dvotionals, striong lexicons etc. are not very useful for word lookups
          m_settings.swords.standardLexicon->insertItem(modDescript);
				}
				
 				break;
 			} 				
 			default://unknown type
 				break;
 		}
  }

//using two lists and one loop is better than six loops with almost the same code :)
 	QPtrList<QComboBox> comboList;
 	comboList.setAutoDelete(false);//don't delete the combos accidentally
 	QStringList moduleList;
	
	for (int i = 0; i < (int)CBTConfig::lastModuleType; ++i) {
		//fill the combobox list in the right order (i.e. same order as the CBTConfig::module enum list)
  	CBTConfig::modules moduleType = (CBTConfig::modules)(i);
		switch (moduleType) {
			case CBTConfig::standardBible:
				comboList.append(m_settings.swords.standardBible);
				break;
			case CBTConfig::standardCommentary:
				comboList.append(m_settings.swords.standardCommentary);
				break;
			case CBTConfig::standardLexicon:
				comboList.append(m_settings.swords.standardLexicon);
				break;
			case CBTConfig::standardDailyDevotional:
				comboList.append(m_settings.swords.standardDailyDevotional); 
				break;
			case CBTConfig::standardHebrewStrongsLexicon:
				comboList.append(m_settings.swords.standardHebrewStrong);
				break;
			case CBTConfig::standardGreekStrongsLexicon:
				comboList.append(m_settings.swords.standardGreekStrong);
				break;
			case CBTConfig::standardHebrewMorphLexicon:
				comboList.append(m_settings.swords.standardHebrewMorph);
				break;
			case CBTConfig::standardGreekMorphLexicon:
				comboList.append(m_settings.swords.standardGreekMorph);
				break;
		};
		
		//fill the module list
		CSwordModuleInfo* const m = CBTConfig::get( (CBTConfig::modules)(i) );
		if (m) {
			moduleList << m->config(CSwordModuleInfo::Description);
		}
		else {
			moduleList << QString::null;
		}
	}

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

  layout->addWidget( CToolClass::explanationLabel(currentTab, i18n("Filter settings"),
		i18n("Filters control the appearance of text. \
			Here you can specify default settings for all filters. \
			You can change the filter settings in each display window, of course.")) );

  layout->addSpacing(5);
  		
 	m_settings.swords.lineBreaks = new QCheckBox(currentTab);
 	m_settings.swords.lineBreaks->setText(i18n("Insert line break after each verse"));
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

/* 	m_settings.swords.footnotes = new QCheckBox(currentTab);
 	m_settings.swords.footnotes->setText(i18n("Show footnotes"));
 	m_settings.swords.footnotes->setChecked(CBTConfig::get(CBTConfig::footnotes));		
  QToolTip::add(m_settings.swords.footnotes, CResMgr::settings::sword::filters::footnotes::tooltip);
  QWhatsThis::add(m_settings.swords.footnotes, CResMgr::settings::sword::filters::footnotes::whatsthis);
 	layout->addWidget(m_settings.swords.footnotes);
*/
 	
	m_settings.swords.headings = new QCheckBox(currentTab);
 	m_settings.swords.headings->setText(i18n("Show section headings"));
 	m_settings.swords.headings->setChecked(CBTConfig::get(CBTConfig::headings));
  QToolTip::add(m_settings.swords.headings, CResMgr::settings::sword::filters::headings::tooltip);
  QWhatsThis::add(m_settings.swords.headings, CResMgr::settings::sword::filters::headings::whatsthis);
 	layout->addWidget(m_settings.swords.headings);

//  	m_settings.swords.strongNumbers = new QCheckBox(currentTab);
//  	m_settings.swords.strongNumbers->setText(i18n("Show Strong's numbers"));
//  	m_settings.swords.strongNumbers->setChecked(CBTConfig::get(CBTConfig::strongNumbers));
//   QToolTip::add(m_settings.swords.strongNumbers, CResMgr::settings::sword::filters::strongsNumbers::tooltip);
//   QWhatsThis::add(m_settings.swords.strongNumbers, CResMgr::settings::sword::filters::strongsNumbers::whatsthis);  
//  	layout->addWidget(m_settings.swords.strongNumbers);

 	m_settings.swords.scriptureReferences = new QCheckBox(currentTab);
 	m_settings.swords.scriptureReferences->setText(i18n("Show scripture cross-references"));
 	m_settings.swords.scriptureReferences->setChecked(CBTConfig::get(CBTConfig::scriptureReferences));
//Enable this in 1.4, because we're in message freeze for 1.3
//   QToolTip::add(m_settings.swords.scriptureReferences, CResMgr::settings::sword::filters::crossReferences::tooltip);
//   QWhatsThis::add(m_settings.swords.scriptureReferences, CResMgr::settings::sword::filters::crossReferences::whatsthis);
 	layout->addWidget(m_settings.swords.scriptureReferences);

/*  m_settings.swords.morphTags = new QCheckBox(currentTab);
 	m_settings.swords.morphTags->setText(i18n("Show morphologic tags"));
 	m_settings.swords.morphTags->setChecked(CBTConfig::get(CBTConfig::morphTags));
  QToolTip::add(m_settings.swords.morphTags, CResMgr::settings::sword::filters::morphTags::tooltip);
  QWhatsThis::add(m_settings.swords.morphTags, CResMgr::settings::sword::filters::morphTags::whatsthis);
 	layout->addWidget(m_settings.swords.morphTags);
 	
	m_settings.swords.lemmas = new QCheckBox(currentTab);
 	m_settings.swords.lemmas->setText(i18n("Show lemmas"));
 	m_settings.swords.lemmas->setChecked(CBTConfig::get(CBTConfig::lemmas));
  QToolTip::add(m_settings.swords.lemmas, CResMgr::settings::sword::filters::lemmas::tooltip);
  QWhatsThis::add(m_settings.swords.lemmas, CResMgr::settings::sword::filters::lemmas::whatsthis);
 	layout->addWidget(m_settings.swords.lemmas);
*/
		
  m_settings.swords.greekAccents = new QCheckBox(currentTab);
 	m_settings.swords.greekAccents->setText(i18n("Show Greek accents"));
 	m_settings.swords.greekAccents->setChecked(CBTConfig::get(CBTConfig::greekAccents));
  QToolTip::add(m_settings.swords.greekAccents, CResMgr::settings::sword::filters::greekAccents::tooltip);
  QWhatsThis::add(m_settings.swords.greekAccents, CResMgr::settings::sword::filters::greekAccents::whatsthis);
 	layout->addWidget(m_settings.swords.greekAccents);

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
void COptionsDialog::saveDisplayStyle(){
	CBTConfig::set(CBTConfig::displayStyle, m_settings.displayStyle.styleChooser->currentText());
}

/** No descriptions */
void COptionsDialog::saveFonts(){
	for(QMap<QString, CBTConfig::FontSettingsPair>::Iterator it = m_settings.fonts.fontMap.begin(); it != m_settings.fonts.fontMap.end(); ++it ) {
    const CLanguageMgr::Language* const lang = languageMgr()->languageForTranslatedName(it.key());
    if (!lang->isValid()) { //we probably use a language, for which we have only the abbrev
      CLanguageMgr::Language l(it.key(), it.key(), it.key());
	    CBTConfig::set(&l, it.data());

    }
		else {
    	CBTConfig::set(lang, it.data());
		}
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

	for (int i = 0; i < (int)CBTConfig::lastModuleType; ++i) {
		QString moduleDescription = QString::null;
  	
		CBTConfig::modules moduleType = (CBTConfig::modules)(i);
		switch (moduleType) {
			case CBTConfig::standardBible:
				moduleDescription = m_settings.swords.standardBible->currentText();
				break;
			case CBTConfig::standardCommentary:
				moduleDescription = m_settings.swords.standardCommentary->currentText();
				break;
			case CBTConfig::standardLexicon:
				moduleDescription = m_settings.swords.standardLexicon->currentText();
				break;
			case CBTConfig::standardDailyDevotional:
				moduleDescription = m_settings.swords.standardDailyDevotional->currentText();
				break;
			case CBTConfig::standardHebrewStrongsLexicon:
				moduleDescription = m_settings.swords.standardHebrewStrong->currentText();
				break;
			case CBTConfig::standardGreekStrongsLexicon:
				moduleDescription = m_settings.swords.standardGreekStrong->currentText();
				break;
			case CBTConfig::standardHebrewMorphLexicon:
				moduleDescription = m_settings.swords.standardHebrewMorph->currentText();
				break;
			case CBTConfig::standardGreekMorphLexicon:
				moduleDescription = m_settings.swords.standardGreekMorph->currentText();
				break;
		};
		
		CSwordModuleInfo* const module = backend()->findModuleByDescription(moduleDescription);
		CBTConfig::set(moduleType, module);
	}

	
	QString languageAbbrev;
 	
	const QString currentLanguageName = m_settings.swords.localeCombo->currentText();
	const CLanguageMgr::Language* const l = CPointers::languageMgr()->languageForTranslatedName( currentLanguageName );
	
	if (l && l->isValid()) {
		languageAbbrev = l->abbrev();
	}
	else { //it can be the lang abbrev like de_abbrev or the Sword description
		list <sword::SWBuf> locales = sword::LocaleMgr::getSystemLocaleMgr()->getAvailableLocales();
		
		for (list <sword::SWBuf>::iterator it = locales.begin(); it != locales.end(); it++) {
			SWLocale* locale = LocaleMgr::getSystemLocaleMgr()->getLocale((*it).c_str());
			Q_ASSERT(locale);
			
			if ( locale && (QString::fromLatin1(locale->getDescription()) == currentLanguageName) ) {
				languageAbbrev = QString::fromLatin1(locale->getName());	//we found the abbrevation for the current language
				break;
			}
		}
		
		if (languageAbbrev.isEmpty()) {
			languageAbbrev = currentLanguageName; //probably a non-standard locale name like de_abbrev
		}
	}
	
 	if (!languageAbbrev.isEmpty()) {
  	CBTConfig::set(CBTConfig::language, languageAbbrev);
 	}

 	CBTConfig::set(CBTConfig::scroll, m_settings.swords.useDownArrow->isChecked());
 	CBTConfig::set(CBTConfig::lineBreaks, m_settings.swords.lineBreaks->isChecked());
 	CBTConfig::set(CBTConfig::verseNumbers, m_settings.swords.verseNumbers->isChecked());
 	CBTConfig::set(CBTConfig::headings, m_settings.swords.headings->isChecked());
 	CBTConfig::set(CBTConfig::scriptureReferences, m_settings.swords.scriptureReferences->isChecked());
 	CBTConfig::set(CBTConfig::hebrewPoints, m_settings.swords.hebrewPoints->isChecked());
 	CBTConfig::set(CBTConfig::hebrewCantillation, m_settings.swords.hebrewCantillation->isChecked());
 	CBTConfig::set(CBTConfig::greekAccents, m_settings.swords.greekAccents->isChecked());
 	CBTConfig::set(CBTConfig::textualVariants, m_settings.swords.textualVariants->isChecked());
}

/** This slot is called when the "Use own font for language" bo was clicked. */
void COptionsDialog::useOwnFontClicked( bool isOn ){
  m_settings.fonts.fontChooser->setEnabled(isOn);
  m_settings.fonts.fontMap[ m_settings.fonts.usage->currentText() ].first = isOn;

  if (isOn) { //show font icon
    m_settings.fonts.usage->changeItem(
			SmallIcon("fonts"),
			m_settings.fonts.usage->currentText(),
			m_settings.fonts.usage->currentItem() 
		);
  }
  else {    //don't show
    m_settings.fonts.usage->changeItem(
			m_settings.fonts.usage->currentText(), 
			m_settings.fonts.usage->currentItem() 
		);
  }
}

void COptionsDialog::updateStylePreview() {
	//update the style preview widget
 	using namespace Rendering;
	
	const QString styleName = m_settings.displayStyle.styleChooser->currentText();
	
	CSwordModuleInfo* const module = CBTConfig::get(CBTConfig::standardBible);
	if (!module) {
		return; //WARNING: return already here!
	}
		
 	CTextRendering::KeyTreeItem::Settings settings;
	
	CTextRendering::KeyTree tree;
	tree.append( new CTextRendering::KeyTreeItem("Gen 1:1", "Gen 1:3", module, settings) );
	settings.highlight = true;
	tree.append( new CTextRendering::KeyTreeItem("Joh 3:16", module, settings) );
	settings.highlight = false;
	tree.append( new CTextRendering::KeyTreeItem("Joh 16:4", module, settings) );
	
	CDisplayRendering render;
	m_settings.displayStyle.stylePreview->begin();
	
	const QString oldStyleName = CBTConfig::get(CBTConfig::displayStyle);
	CBTConfig::set(CBTConfig::displayStyle, styleName);
	
	m_settings.displayStyle.stylePreview->write( render.renderKeyTree(tree) );
	
	m_settings.displayStyle.stylePreview->end();

	CBTConfig::set(CBTConfig::displayStyle, oldStyleName);
}
