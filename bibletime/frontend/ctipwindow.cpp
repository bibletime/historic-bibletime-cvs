/***************************************************************************
                          ctipwindow.cpp  -  description
                             -------------------
    begin                : Thu Jul 27 2000
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
/*****************************************************************

Copyright (c) 2000, Matthias Hoelzer-Kluepfel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

******************************************************************/

//Qt include files
#include <qframe.h>
#include <qcheckbox.h>
#include <qpushbutton.h>
#include <qlayout.h>
#include <qlabel.h>
#include <qfile.h>
#include <qtextstream.h>

//KDE includes
#include <kapp.h>
#include <kuniqueapp.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <kglobal.h>
#include <klocale.h>
#include <kconfig.h>
#include <kmessagebox.h>
#include <kiconloader.h>
#include <kstddirs.h>

//Own include files
#include "thirdparty/qt3stuff/qtextview.h"
#include "ctipwindow.h"
#include "chtmlwidget.h"
#include "ctoolclass.h"
#include "../ressource.h"

CTipWindow::CTipWindow() : KDialog(0,0,true) {
  setCaption(i18n("Useful tips for BibleTime"));
  setIcon( LOAD_SMALL_ICON(ICON_HELP_DAILY_TIP) );

  QHBoxLayout *hbox = new QHBoxLayout(this, 4);
  QVBoxLayout *vbox = new QVBoxLayout(hbox, 4);

  QFrame *f = new QFrame(this);
  f->setFrameStyle(QFrame::VLine | QFrame::Sunken);
  hbox->addWidget(f);

  text = new Qt3::QTextView(this);
  QPixmap pix = locate("data", "bibletime/pics/cross.png");
  text->setFixedSize( pix.size() );
  text->setPaper( QBrush(Qt::white, pix) );
  text->mimeSourceFactory()->addFilePath(KGlobal::dirs()->findResourceDir("data", "bibletime/pics/")+"bibletime/pics/");
  QStringList icons = KGlobal::dirs()->resourceDirs("icon");
  QStringList::Iterator it;
  for (it = icons.begin(); it != icons.end(); ++it)
    text->mimeSourceFactory()->addFilePath(*it);
  hbox->addWidget(text);

  ok = new QPushButton(i18n("&OK"), this);
  ok->setDefault(true);
  vbox->addWidget(ok);
  vbox->addSpacing(15);

  next = new QPushButton(i18n("&Next"), this);
  vbox->addWidget(next);

  prev = new QPushButton(i18n("&Previous"), this);
  vbox->addWidget(prev);

  vbox->addStretch(3);
  startup = new QCheckBox(i18n("Run on startup"), this);
  vbox->addWidget(startup);

  connect(next, SIGNAL(clicked()), this, SLOT(nextTip()));
  connect(prev, SIGNAL(clicked()), this, SLOT(prevTip()));
  connect(ok, SIGNAL(clicked()), this, SLOT(accept()));
  connect(startup, SIGNAL(clicked()), this, SLOT(startupClicked()));

	resize( sizeHint() );
	setFixedSize( sizeHint() );

	current = 0;
  if ( loadTips() ) {
	  current = tips.count() ? kapp->random() % tips.count() : 0;

	  KConfig *config = KGlobal::config();
	  KConfigGroupSaver(config, "Startup");
	  startup->setChecked(config->readBoolEntry("show tips", true));
  }
  else {
		QString message = i18n("<DIV align=\"center\"><B>Missing tips!</B></DIV><BR><BR>Your installation is missing the daily tips.\
		Please make sure all files of BibleTime are\
		installed and that the permissions of $KDEDIR are correct!<BR><BR>\
		If you can't solve this problem please contact the team of BibleTime at\
		<A HREF=\"mailto:info@bibletime.de\">info@bibletime.de</A>!");
		tips.append(message);
		current=0;
		next->setEnabled(false);
		prev->setEnabled(false);
  }
	nextTip();
}


void CTipWindow::startupClicked() {
  KConfig *config = KGlobal::config();
	KConfigGroupSaver(config, "Startup");
  config->writeEntry("show tips", startup->isChecked());
}


bool CTipWindow::loadTips() {
  QString fname = CToolClass::locatehtml("bibletime/tipdatabase");
  if (fname.isEmpty())
    return false;
  tips.clear();

  QFile f(fname);
  f.setName(fname);
  if (f.open(IO_ReadOnly)) {	
		QTextStream ts(&f);

		QString line, tag, tip;
		bool inTip = false;
		while (!ts.eof()) {
	  	line = ts.readLine();
	  	tag = line.stripWhiteSpace().lower();
	
	  	if (tag == "<html>")
	   	{
	     	inTip = true;
	     	tip = QString::null;
	     	continue;
	   	}
	  	
	  	if (inTip) {
	     	if (tag == "</html>") {
		  		tips.append(tip);
		  		inTip = false;
				}
	     	else
					tip.append(line).append("\n");
	    }
    }
		f.close();
	}
	return true;
}


void CTipWindow::nextTip() {
  current++;
  if (current >= tips.count())
    current = 0;
  text->setText(QString::fromLatin1("<html>%1</html>").arg(tips[current]));
}

void CTipWindow::prevTip() {
  current--;
  if (current < 0)
    current = tips.count()-1;
  if (current < 0)	//are we still in negative numbers?
  	current = 0;
  text->setText(QString::fromLatin1("<html>%1</html>").arg(tips[current]));
}