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

#include "backend/cswordmoduleinfo.h"
#include "backend/cswordkey.h"

#include "frontend/cbtconfig.h"

#include "util/scoped_resource.h"

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


void CInfoDisplay::setInfo(const InfoType type, const QString& data) {
	QString text;
  switch (type) {
		case Footnote:
			text = decodeFootnote(data);
			break;
		case WordTranslation:
			text = getWordTranslation(data);
			break;
		case StrongNumber:
			text = decodeStrongNumber(data);
			break;
		case MorphCode:
			text = decodeMorphCode(data);
			break;
		default:
			text = "unknown tag";
			break;
	}

	m_htmlPart->begin();
	m_htmlPart->write(text);
	m_htmlPart->end();
}


/*!
    \fn CInfoDisplay::decodeFootnote( const QString& data )
 */
const QString CInfoDisplay::decodeFootnote( const QString& data ) {
	QStringList list = QStringList::split("/", data);
	Q_ASSERT(list.count() >= 3);
	if (!list.count()) {
		return "List invalid";
	}
		
	const QString modulename = list[0];
	const QString keyname = list[1];
	const QString swordFootnote = list[2];
	
	Q_ASSERT(!modulename.isEmpty());
	Q_ASSERT(!keyname.isEmpty());
	Q_ASSERT(!swordFootnote.isEmpty());

//	qWarning("data: %s, %s, %s", modulename.latin1(), keyname.latin1(), swordFootnote.latin1());
	
	CSwordModuleInfo* module = CPointers::backend()->findModuleByName(modulename);
	Q_ASSERT(module);
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );	
	
	key->key(keyname);
	key->renderedText(); //fotce entryAttributes
	
	QString ret = QString::fromUtf8(module->module()->getEntryAttributes()["Footnote"][swordFootnote.latin1()]["body"].c_str());
	
	return ret;
}

const QString CInfoDisplay::decodeStrongNumber( const QString& data ) {
	QString strongDesc = CBTConfig::get(CBTConfig::standardGreekStrongsLexicon);
	CSwordModuleInfo* module = CPointers::backend()->findModuleByDescription( strongDesc );
	
	Q_ASSERT(module);
	if (!module) {
		return QString::null;
	}
	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
	key->key( data.mid(1) ); //skip H or G (language sign)
	
	return key->renderedText();
	
}

const QString CInfoDisplay::decodeMorphCode( const QString& data ) {
	return QString::null;
}

const QString CInfoDisplay::getWordTranslation( const QString& data ) {
	const QString lexiconName = CBTConfig::get(CBTConfig::standardLexicon);
	CSwordModuleInfo* module = CPointers::backend()->findModuleByDescription( lexiconName );
	
	Q_ASSERT(module);
	if (!module)
		return QString("module %1 not found").arg(lexiconName);
		
	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
	key->key( data );
	
	return data + "<br/>" + key->renderedText();
}


/*!
    \fn CInfoDisplay::clearInfo()
 */
void CInfoDisplay::clearInfo() {
	m_htmlPart->begin();
	m_htmlPart->write(QString::null);
	m_htmlPart->end();		
}
