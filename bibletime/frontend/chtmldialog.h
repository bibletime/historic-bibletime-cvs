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

//Qt includes
#include <qwidget.h>

//KDE includes
#include <kdialogbase.h>
#include <khtml_part.h>
#include <kurl.h>
#include <kparts/part.h>
#include <kparts/browserextension.h>

//forward declarations
class KComboBox;

/** This is a general dialog to show long error messages or report and solutions
	* (Perhaps from an FAQ file).
  * @author The BibleTime Team
  */
class CHTMLDialog : public KDialogBase {
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
	KHTMLPart* m_htmlPart;
  /**
  * Initializes this widget.
  */
  void init(const bool enableHistory = true);

protected slots: // Protected slots
  void openURL( const KURL&, const KParts::URLArgs&);
};

#endif

