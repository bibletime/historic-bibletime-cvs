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
	~COptionsDialog();
  /**
  * Returns an integer with ORed feature enum
  * entries of the changed settings.
  */
  const int getChangedSettings() const;

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
		};
		StartupSettings startup;
		
		struct KeySettings {
			KKeyChooser* keyChooser;
			KKeyEntryMap dict;			
			KAccel* accel;
		};	
		KeySettings keys;
	};	
	struct DisplayWindowSettings {
		struct GeneralSettings {
			QCheckBox* useDownArrow;
			QComboBox* localeCombo;
		};
		GeneralSettings general;
		
		struct ColorSettings {
			KColorButton* background;
			KColorButton* highlightedVerse;			
		};
		ColorSettings colors;
		
		struct FontSettings {		
			KFontChooser* fontChooser;
			QComboBox* usage;
			QMap<QString,QFont> fontMap;			
		};
		FontSettings fonts;
		
		struct ModuleFontSettings {
			KListBox* modules;
			KFontChooser* fonts;
		};
		ModuleFontSettings module_fonts;
		
		struct ViewProfileSettings {
			QListBox* profiles;
			QPushButton* createProfile;			
			QPushButton* deleteProfile;
			QPushButton* renameProfile;
			CProfileMgr mgr;
		};
		ViewProfileSettings profiles;
		
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
		};	
		KeySettings keys;		
	};

	GeneralSettings m_general;
	DisplayWindowSettings m_displayWindows;

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
 	* Is called when a new font was selected in the
	* foreign font manager dialog.
	*/
  void newForeignFontSelected( const QFont& );
  /**
 	* Is called when the user select a new module in te foreign font management dialog.
 	*/
  void foreignFontModuleChanged( QListBoxItem* );
	
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
   No descriptions
  */
  void deleteProfile();
signals: // Signals
  /** No descriptions */
  void signalSettingsChanged(const int changedSettings);
};

#endif
