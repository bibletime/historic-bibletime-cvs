//
// C++ Implementation: cinfodisplay
//
// Description:
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

//BibleTime includes
#include "cinfodisplay.h"

//Qt includes
#include <qlayout.h>
#include <qlabel.h>
#include <qscrollview.h>

//KDE includes
#include <khtml_part.h>
#include <khtmlview.h>
#include <klocale.h>

CInfoDisplay::CInfoDisplay(QWidget *parent, const char *name)
    : QWidget(parent, name)
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	QLabel* headingLabel = new QLabel(i18n("Info display"),this);
	m_htmlPart = new KHTMLPart(this);

	layout->addWidget(headingLabel);
	layout->addWidget(m_htmlPart->view());
}


CInfoDisplay::~CInfoDisplay() {
}


/*!
    \fn CInfoDisplay::setText(const QString& text)
 */
void CInfoDisplay::setText(const QString& text) {
  m_htmlPart->begin();
	m_htmlPart->write(text);
	m_htmlPart->end();
}
