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
#include <qdialog.h>

class QTextBrowser;

/** This is a general dialog to show long error messages or report and solutions
	* (Perhaps from an FAQ file).
  * @author The BibleTime Team
  */
class CHTMLDialog : public QDialog  {
  Q_OBJECT
public: 
	CHTMLDialog(QWidget* parent=0, const char *name=0);
	CHTMLDialog(const QString& url, QWidget* parent=0, const char *name=0);
	~CHTMLDialog();
  /**
 	* Sets the content of the widget. Not URL is used.
 	*/
  virtual void setText(const QString&);

protected:
	QTextBrowser* m_textBrowser;
  /**
  * Initializes this widget.
  */
  void init(const bool enableHistory = true);
};

#define HTML_DIALOG(filename) \
{ \
	CHTMLDialog *dlg = new CHTMLDialog( filename, 0, "HTML_DIALOG" ); \
	dlg->exec(); \
	delete dlg; \
};

#endif

