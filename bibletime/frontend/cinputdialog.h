/***************************************************************************
                          cinputdialog.h  -  description
                             -------------------
    begin                : Sun May 13 2001
    copyright            : (C) 2001 by The BibleTime team
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

#ifndef CINPUTDIALOG_H
#define CINPUTDIALOG_H

//KDE includes
#include <kdialog.h>

class QMultiLineEdit;
class QWidget;

/** This is a small input dialog with
	* a multiline edit for the text input.
  * @author The BibleTime team
  */
class CInputDialog : public KDialog  {
   Q_OBJECT
public: 	
	CInputDialog(const QString& caption, const QString& description, const QString& text, QWidget *parent=0, const char *name=0, const bool modal = true);
  /**
 	* A static function to get some using CInputDialog.
 	*/
  static const QString getText( const QString& caption, const QString& description, const QString& text = QString::null, bool* ok = 0, QWidget* parent = 0, bool modal = true);
  /**
 	* Returns the text entered at the moment.
 	*/
  const QString text();
	~CInputDialog();

private:
	QMultiLineEdit* m_editWidget;
};

#endif
