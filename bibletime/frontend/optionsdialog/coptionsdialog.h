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

//QT includes
#include <qwidget.h>
#include <qframe.h>
#include <qlabel.h>
#include <qstring.h>
#include <qmap.h>
#include <qcombobox.h>
#include <qdict.h>

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
  int getChangedSettings();

protected: // Protected methods
  virtual void initGeneralPage();
  virtual void initKeyPage(KAccel* key_accel);
  virtual void initFontPage();
  virtual void initFontManagerPage();
  virtual void initColorsPage();
  virtual void saveGeneralOptions();
  virtual void saveFontOptions();

  virtual void saveKeyOptions();
  virtual void saveColorsOptions();

	QHBox* keyaccel_page;
	QWidget* general_page;	
	QHBox* font_page;
	QHBox* foreign_font_page;
	QHBox* colors_page;

  KConfig* config;
	  	
	//items of general configuration page
	QCheckBox*	tipCheckBox;	
	QCheckBox*	logoCheckBox;
	QCheckBox*	localeCheckBox;
	QFont currentFonts[1];
	QComboBox*	localeComboBox;

	// Font dialog definitions
	KFontChooser* fonts;
	QFont presenterFont;
	QButtonGroup* fontButtonGroup;
	QRadioButton* font1vButton;
//	QRadioButton* font1fButton;
//	QRadioButton* font2fButton;
//	QRadioButton* font2vButton;
	int currentFont;
//	bool fixedFonts;
  CImportantClasses* m_important;
	
	//Key binding definitions
#if KDE_VERSION >= 193
	KKeyEntryMap dict;
#else
	QDict<KKeyEntry> dict;
#endif

	KAccel* key_accel;
	
	//Foreign font dialog definitions
	KListBox* fontModuleList;
	KFontChooser* foreignFonts;
	
	//Color dialog definitions
	QButtonGroup* colorGroup;
	KColorButton* backgroundButton;
	KColorButton* normalTextButton;
	KColorButton* URLLinkButton;
	KColorButton* highlightedVerseButton;
	
	QString* hexString;
  /**
  	*
  	*/
  CBackEnd* backend;

protected slots: // Protected slots
	void localeStateChanged(int);
  void fontButtonClicked(int);
  void newFontSelected(const QFont &);

  /**
  	* Is called when a new font was selected in the
		* foreign font manager dialog.
		*/
  void newForeignFontSelected( const QFont& );
	/**
		* Called if the OK button was clicked
		*/
  void slotOk();
  /**
  	* Called if the Apply button was clicked
  	*/
  void slotApply();
  /**
  	* Is called when the user select a new module in te foreign font management dialog.
  	*/
  void foreignFontModuleChanged( QListBoxItem* );

private:
	int m_changedSettings;
};

#endif
