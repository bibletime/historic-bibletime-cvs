/***************************************************************************
                          chtmldialog.h  -  description
                             -------------------
    begin                : Tue Jan 25 2000
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

#ifndef CHTMLDIALOG_H
#define CHTMLDIALOG_H

#include <qwidget.h>
#include <kdialogbase.h>

class CHTMLWidget;

/** This is a general dialog to show long error messages or report and solutions
	* (Perhaps from an FAQ file).
  * @author The BibleTime Team
  */
class CHTMLDialog : public KDialogBase  {
  Q_OBJECT
public: 
	CHTMLDialog(QWidget* parent=0, const char *name=0);
	CHTMLDialog(const QString url, QWidget* parent=0, const char *name=0);
	~CHTMLDialog();
  /**
  	* Sets the content of the widget
  	*/
  virtual void setText(const QString&);

protected: // Protected attributes
 	CHTMLWidget* html_widget;
};

#endif
