/***************************************************************************
                          ctipwindow.h  -  description
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
/**
  * This class is from KTip (kdebase), we renamed it from TipWindow to CTipWindow to
  * fit into our naming conventions.
  * We also made some modifications to the code. Copyright remains to the original author.
  * Added by Joachim
	*/

#ifndef __ctipwindow_h__
#define __ctipwindow_h__


#include <qstringlist.h>
#include <kdialog.h>


namespace Qt3 {
	class QTextView;
};

class QCheckBox;
class QPushButton;


class CTipWindow : public KDialog
{
  Q_OBJECT

public:
  CTipWindow();

public slots:
  void nextTip();
  void prevTip();
  void startupClicked();

private:
  bool loadTips();

  Qt3::QTextView *text;
  QCheckBox    *startup;
  QPushButton  *next, *prev, *ok;
  QStringList  tips;
  int current;
};

#endif
