/***************************************************************************
                          cstyleeditordialog.h  -  description
                             -------------------
    begin                : Sun Aug 20 2000
    copyright            : (C) 2000 by The BibleTime team
    email                : info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef CSTYLEEDITORDIALOG_H
#define CSTYLEEDITORDIALOG_H

//BibleTime includes
#include "cstyle.h"

//QT includes
#include <qwidget.h>

//KDE inludes
#include <kdialogbase.h>
#include "cstyle.h"


/**The editor for the printing styles.
  *@author The BibleTime team
  */
class QLineEdit;
class KComboBox;
class QRadioButton;
class QSpinBox;
class KFontDialog;
class KColorButton;
class QVButtonGroup;
class QHButtonGroup;
class QCheckBox;
class QComboBox;
class QGroupBox;

class CStyleEditorDialog : public KDialogBase  {
   Q_OBJECT
public: 
	CStyleEditorDialog(CStyle* style, QWidget *parent=0, const char *name=0);
	~CStyleEditorDialog();

protected: // Protected methods
  /**
  * Saves settings to config file.
  */
  void saveSettings();
  /**
  * Reads settings from config file
  */
  void readSettings();
	/**
  * Initializes te view.
  */
  void initView();

protected slots:
  /**
  * Opens the font chooser dialog.
  */
  void showFontChooser();
  /**
  *
  */
  void useFrameClicked();
  /**
 	* Setups the font widgets using the parameter.
 	*/
  void setupFontWidgets( QFont& font );
  /**
  * Sets up the states of the child widgets using the styl format given as parameter.
  */
  void applySettingsToFormat( CStyle::Format* const format );  	
  /**
  * Sets the properties of the cuzrrent format which are changed in the editor.
  */
  void setupWithFormat( CStyle::Format* const format );
  /**
  * Is called when the enablePart box was clicked.
  */
  void enableBoxClicked();
  /**
  * Called when the type was changed in the combobox.
  */
  void styleTypeChanged( const QString& );

private:
	struct alignementRadios {
		QRadioButton*	leftRB;
		QRadioButton*	centerRB;
		QRadioButton*	rightRB;
		QVButtonGroup*	buttongroup;
	} m_alignRadios;
	
	struct colorButtons {
		KColorButton*	foregroundChooser;
		KColorButton*	backgroundChooser;
	} m_colors;
	
	struct frameWidgets {
		QCheckBox*	useFrame;
		KColorButton*	colorChooser;
		QSpinBox* lineThicknessChooser;
		QComboBox* lineStyleChooser;		
		QGroupBox*	groupbox;
	} m_frame;
	
	struct fontWidgets {
		QFont font;
		QPushButton* fontButton;
		QLabel*	fontDisplay;
	} m_font;
	

  QLineEdit*	m_styleNameEdit;
  KComboBox*	m_styleTypeChooser;
	CStyle*	m_style;
	CStyle::Format* m_currentFormat;
	QCheckBox*	m_setEnabledBox;


	bool m_formatEnabled;
};

#endif
