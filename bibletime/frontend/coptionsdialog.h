/***************************************************************************
                          coptionsdialog.h  -  description
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

#ifndef COPTIONSDIALOG_H
#define COPTIONSDIALOG_H

#include "cprofilemgr.h"
#include "cbtconfig.h"
#include "util/cpointers.h"

//QT includes
#include <qwidget.h>
#include <qframe.h>
#include <qlabel.h>
#include <qstring.h>
#include <qmap.h>
#include <qdict.h>
#include <qmap.h>

//KDE includes
#include <kdialogbase.h>
#include <kfontdialog.h>
#include <kaccel.h>
#include <kcolorbutton.h>
#include <kapp.h>

class QHBox;
class QCheckBox;
class QComboBox;
class QRadioButton;
class KListBox;
class KKeyChooser;
class KTabCtl;
class KHTMLPart;

/**
	* The optionsdialog of BibleTime
  * @author The BibleTime Team
  */
class COptionsDialog : public KDialogBase, public CPointers  {
   Q_OBJECT

public:
	COptionsDialog(QWidget *parent=0, const char *name=0, KAccel* key_accel=0);

  enum Parts {
		Startup,
		Font,
		Color,
		ViewProfiles,
		Sword,
		accelerators
  };

  /** Opens the page which contaisn the given part ID. */
  const bool showPart( COptionsDialog::Parts ID );

private:
  struct settings {
	 	struct StartupSettings {
	 		QCheckBox* showTips;
	 		QCheckBox* showLogo;
	 		QCheckBox* restoreWorkspace;
	 	} startup;

		struct SwordSettings {
			QCheckBox* lexiconCache;
			QComboBox* standardBible;
			QComboBox* standardCommentary;
			QComboBox* standardLexicon;
			QComboBox* standardDailyDevotional;
			QComboBox* standardHebrewStrong;
			QComboBox* standardGreekStrong;
			QComboBox* standardHebrewMorph;
			QComboBox* standardGreekMorph;
			QComboBox* localeCombo;
			QCheckBox* useDownArrow;
			QCheckBox* lineBreaks;
			QCheckBox* verseNumbers;
  		QCheckBox* headings;
			QCheckBox* hebrewPoints;
			QCheckBox* hebrewCantillation;
			QCheckBox* greekAccents;
			QCheckBox* textualVariants;
      QCheckBox* scriptureReferences;
		} swords; // not: sword instead of sword -> namespace error

		struct KeySettings {
			struct WindowType {
				KKeyChooser* keyChooser;
				KAccel* accel;
			};
			WindowType application;
			WindowType general;
			WindowType bible;
			WindowType commentary;
			WindowType lexicon;
			WindowType book;
		} keys;

		struct DisplayStyleSettings {
			QComboBox* styleChooser;
			KHTMLPart* stylePreview;
		} displayStyle;

    struct FontSettings {
			KFontChooser* fontChooser;
			QComboBox* usage;
      QCheckBox* useOwnFontBox;
      //the pair os to check whether the standard font (bool == false) or whether an own font should be used (true)
      QMap<QString,CBTConfig::FontSettingsPair> fontMap;
		} fonts;

		struct ProfileSettings {
			QListBox* profiles;
			QPushButton* createProfile;
			QPushButton* deleteProfile;
			QPushButton* renameProfile;
			CProfileMgr mgr;
		} profiles;
  } m_settings;

protected slots: // Protected slots
  /**
  * Adds a new view profile to the list.
  */
  void addNewProfile();
  /**
  * Called when a new font in the fonts page was selected.
  */
  void newDisplayWindowFontSelected(const QFont &);
  /**
  * Called when the combobox contents is changed
  */
  void newDisplayWindowFontAreaSelected(const QString&);
	/**
	* Called if the OK button was clicked
	*/
  void slotOk();
  /**
 	* Called if the Apply button was clicked
 	* commented out for the time being.  ck
 	*/
	void slotApply();
  /**
  * Renames the currently selected profile.
  */
  void renameProfile();
  /**
  * Delete the selected profile.
  */
  void deleteProfile();
  /**
  * This slot is called when the "Use own font for language" bo was clicked.
  */
  void useOwnFontClicked(bool);
  /**
  * Update the style preview widget
  */
  void updateStylePreview();

protected: // Protected methods
  /**
  * Init Sword section.
  */
  void initSword();
  /**
  * Init accel key section.
  */
  void initAccelerators();
  /**
  * Init fonts section.
  */
  void initFonts();
  /**
  * Init profiles section.
  */
  void initProfiles();
  /**
  * Init color section.
  */
  void initDisplayStyle();
  /**
  * Initializes the startup section of the OD.
  */
  void initStartup();
  void saveAccelerators();
	void saveDisplayStyle();
	void saveFonts();
 	void saveProfiles();
 	void saveStartup();
 	void saveSword();

signals: // Signals
  void signalSettingsChanged();
};

#endif
