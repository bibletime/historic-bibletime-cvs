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

#include "../../structdef.h"
#include "../cprofilemgr.h"

//QT includes
#include <qwidget.h>
#include <qframe.h>
#include <qlabel.h>
#include <qstring.h>
#include <qmap.h>
#include <qcombobox.h>
#include <qdict.h>
#include <qmap.h>

//KDE includes
#include <kdialogbase.h>
#include <kfontdialog.h>
#include <kaccel.h>
#include <kcolorbtn.h>
#include <kapp.h>

class QHBox;
class QCheckBox;
class QRadioButton;
class KListBox;
class KKeyChooser;
class CBackEnd;

/**
	* The optionsdialog of BibleTime
  * @author The BibleTime Team
  */
class COptionsDialog : public KDialogBase  {
   Q_OBJECT

public:
	COptionsDialog(CImportantClasses* importantClasses, QWidget *parent=0, const char *name=0, KAccel* key_accel=0);
  /**
  * Returns an integer with ORed feature enum
  * entries of the changed settings.
  */
  const int getChangedSettings() const;

  enum BTFont {
		standard,
		unicode
	};
  enum BTColor {
		text,
		background,
		highlighted_verse,
		footnote,
		strongs,
		morph,
		jesuswords
	};
	enum BTBool {
		downArrow,
		lineBreaks,
		verseNumbers
	};
  enum Parts {
		Startup,
		Sword,
		GeneralKeys,
		General,
		Color,
		Font,
		DisplayKeys,
		ViewProfiles
  };


	static QFont 	getBTFont ( BTFont which );
	static QColor getBTColor( BTColor which );
	static bool		getBTBool ( BTBool which );
  /** Opens the page which contaisn the given part ID. */
  const bool showPart( COptionsDialog::Parts ID );

  static CSwordBackend::moduleOptionsBool getAllModuleOptionDefaults( void );

  static CSwordBackend::displayOptionsBool getAllDisplayOptionDefaults( void );

private:
  void initGeneral();
  void saveGeneral();

  void initDisplayWindow();
  void saveDisplayWindow();


  KConfig* m_config;
  CImportantClasses* m_important;
  int m_changedSettings;

	struct GeneralSettings {
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
			QComboBox* standardHebrewStrong;
			QComboBox* standardGreekStrong;
		} sword;
		struct KeySettings {
			KKeyChooser* keyChooser;
			KKeyEntryMap dict;			
			KAccel* accel;
		} keys;
	} m_general;	

	struct DisplayWindowSettings {
		struct GeneralSettings {
			QComboBox* localeCombo;
			QCheckBox* useDownArrow;
			QCheckBox* lineBreaks;
			QCheckBox* verseNumbers;
			QCheckBox* footnotes;
  		QCheckBox* strongNumbers;
  		QCheckBox* headings;
  		QCheckBox* morphTags;
			QCheckBox* lemmas;
			QCheckBox* hebrewPoints;
			QCheckBox* hebrewCantillation;
			QCheckBox* greekAccents;
		} general;
		
		struct ColorSettings {
			KColorButton* text;
			KColorButton* background;
			KColorButton* highlightedVerse;
			KColorButton* footnotes;
			KColorButton* strongs;
			KColorButton* morph;
			KColorButton* jesuswords;
		} colors;
		
		struct FontSettings {		
			KFontChooser* fontChooser;
			QComboBox* usage;
			QMap<QString,QFont> fontMap;			
		} fonts;
		
		struct ViewProfileSettings {
			QListBox* profiles;
			QPushButton* createProfile;			
			QPushButton* deleteProfile;
			QPushButton* renameProfile;
			CProfileMgr mgr;
		} profiles;
		
		struct KeySettings {
			struct WindowType{
				KKeyChooser* keyChooser;
				KKeyEntryMap dict;			
				KAccel* accel;		
			};
			WindowType general;
			WindowType bible;
			WindowType commentary;
			WindowType lexicon;
		} keys;		
	} m_displayWindows;

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
  void deleteProfile();

signals: // Signals
  void signalSettingsChanged(const int changedSettings);
};

#endif
