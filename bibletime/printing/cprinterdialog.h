/***************************************************************************
                          cprinterdialog.h  -  description
                             -------------------
    begin                : Sat Aug 5 2000
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

/* $ID$ */

#ifndef CPRINTERDIALOG_H
#define CPRINTERDIALOG_H

#include <qwidget.h>
#include <qstringlist.h>
#include <qtoolbutton.h>

#include <kdialogbase.h>
#include <klistview.h>

#include "cprinter.h"

class QCheckBox;
class QComboBox;
class QLabel;
class QRadioButton;
class QPushButton;
class QSpinBox;
class QHButtonGroup;

class KLineEdit;
class KAction;

class CStyleList;
class CStyle;
class CPrintItemList;

/**
	* This is the dialog where th eprinting
	* specific settings are set.
  * @author The BibleTime team
  */
class CPrinterDialog : public KDialogBase  {
   Q_OBJECT

public:
	CPrinterDialog(CPrinter* printer, QWidget *parent=0, const char *name=0);
	~CPrinterDialog();
  /**
 	* Initializes the layout page of the printerdialog.
 	*/
  void initLayoutPage();
  /**
 	* Initialitzes the page which contains the two lists of CStyles and PrintItems.
 	*/
  void initListPage();

protected: // Protected methods
  struct GeneralWidgets {
		QLabel*	printerListLabel;
		KListView*	printerList;
		QCheckBox*	fileCheck;
		KLineEdit*	fileInput;
		QPushButton*	browseButton;
		QHButtonGroup *oriGroup;
		QRadioButton *portraitRadio;
		QRadioButton *landscapeRadio;
		QSpinBox     *pageSpin;
		QLabel       *paperSizeLabel;
		QComboBox    *paperSelector;
  };
  struct LayoutWidgets {
		QSpinBox     *marginSpin[4];
		CStyleList*	styleList;		
		QToolButton* deleteStyleButton;		
		QToolButton* newStyleButton;		
		QToolButton* editStyleButton;		
  };
  struct EntryWidgets {
		CPrintItemList* printItemList;
		QComboBox* styleComboBox;
		QToolButton* moveUpButton;
		QToolButton* moveDownButton;
		QToolButton* deleteButton;		
		QToolButton* newPageButton;
  };
	GeneralWidgets	m_general;
	LayoutWidgets m_layout;
	EntryWidgets m_entryWidgets;
	
	CPrinter*	m_printer;
  /**
  * Initializes the general printing page
  */
  void initGeneralPage();
  void addPrinterName( const QString &printer );
  /**
  * Initializes the widgets.
  */
  void initView();
  /**
  * parses the printcap file!
  */
  const bool parsePrintcap();
	void setSelectedPrinter( const QString &name );
  /**
 	* Saves the states of the widgets to config file.
 	*/
  void saveSettings();
  /**
 	* Initializes the widgets from config file.
 	*/
  void readSettings();
	/**
	* Inserts known paper types into the list
	*/
	void paperType( QStringList &list );
	const CPrinter::CPageSize paperSize( QPrinter::PageSize pageSize );
  /**
 	* Calls the CPrinter methods to set settings.
 	*/
  const bool applySettingsToPrinter( const bool preview );
	
protected slots: // Protected slots
  /**
 	* Is called when "Browse for file was clicked."
 	*/
  void slotFileBrowserClicked();
  /**
 	* Is called when the print to file checkbox was clicked.
 	*/
  void slotPrintFileCheck();
  /**
 	* Is called when the papertype was changed.
 	*/
  void paperTypeChanged(const int);
  /**
 	*
 	*/
  void slotUser2();
  /**
 	*
 	*/
  void slotUser1();
  /**
 	* Applies the given styles to the print item list.
 	*/
  void slotListApplyStyle(const QString&);

private slots: // Private slots
  /**
  * Is called when the style selection changed in the syle list box.
  */
  void currentStyleChanged(QListViewItem*);
};

#endif
