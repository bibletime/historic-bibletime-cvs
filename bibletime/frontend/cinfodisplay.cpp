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
#include "backend/cdisplaytemplatemgr.h"

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
	ListInfoData list;
	list.append( qMakePair(type, data) );
	
	setInfo(list);
}


void CInfoDisplay::setInfo(const ListInfoData& list) {
	QString text;
	for (ListInfoData::const_iterator it = list.begin(); it != list.end(); ++it) {
	  switch ( (*it).first ) {
			case Lemma:
				text += decodeLemma( (*it).second );
				continue;
			case Morph:
				text += decodeMorph( (*it).second );
				continue;
			case CrossReference:
				text += decodeCrossReference( (*it).second );
				continue;
			case Footnote:
				text += decodeFootnote( (*it).second );
				continue;
			case WordTranslation:
				text += getWordTranslation( (*it).second );
				continue;
			case WordGloss:
				//text += getWordTranslation( (*it).second );
				continue;
			default:
				continue;
		};
	}

	//text = QString::fromLatin1("<div class\"infodisplay\">%1</div>").arg(text);
	CDisplayTemplateMgr mgr;
	CDisplayTemplateMgr::Settings settings;
	settings.pageCSS_ID = "infodisplay";
	text = mgr.fillTemplate(CBTConfig::get(CBTConfig::displayStyle), text, settings);
	
	m_htmlPart->begin();
	m_htmlPart->write(text);
	m_htmlPart->end();
}


const QString CInfoDisplay::decodeCrossReference( const QString& data ) {
	return data;
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
	
	QString ret = QString::fromLatin1("<div class=\"footnoteinfo\"><h3>%1</h3><p>%2</p></div>")
		.arg(i18n("Footnote"))
		.arg(QString::fromUtf8(module->module()->getEntryAttributes()["Footnote"][swordFootnote.latin1()]["body"].c_str()));
	
	return ret;
}

const QString CInfoDisplay::decodeLemma( const QString& data ) {
	//qWarning("decode lemma: %s", data.latin1());
	
	QString strongModuleDesc = CBTConfig::get(data.left(1) == "H" ? 
		CBTConfig::standardHebrewStrongsLexicon : 
		CBTConfig::standardGreekStrongsLexicon
	);
	
	CSwordModuleInfo* module = CPointers::backend()->findModuleByDescription( strongModuleDesc );	
	Q_ASSERT(module);	
	if (!module) {
		return QString::null;
	}
	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
	key->key( data.mid(1) ); //skip H or G (language sign)
	
		QString ret = QString::fromLatin1("<div class=\"lemmainfo\"><h3>%1: %2</h3><p>%3</p></div>")
		.arg(i18n("Lemma"))
		.arg(data)
		.arg(key->renderedText());
	
	return ret;
	
}

const QString CInfoDisplay::decodeMorph( const QString& data ) {
	QString strongModuleDesc = CBTConfig::get(data.left(1) == "H" ? 
		CBTConfig::standardHebrewMorphLexicon : 
		CBTConfig::standardGreekMorphLexicon
	);
	
	CSwordModuleInfo* module = CPointers::backend()->findModuleByDescription( strongModuleDesc );	
	Q_ASSERT(module);	
	if (!module) {
		return QString::null;
	}
	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
	key->key( data.mid(1) ); //skip H or G (language sign)

	QString ret = QString::fromLatin1("<div class=\"morphinfo\"><h3>%1: %2</h3><p>%3</p></div>")
		.arg(i18n("Morph number"))
		.arg(data)
		.arg(key->renderedText());

	return ret;	
}

const QString CInfoDisplay::getWordTranslation( const QString& data ) {
	const QString lexiconName = CBTConfig::get(CBTConfig::standardLexicon);
	CSwordModuleInfo* module = CPointers::backend()->findModuleByDescription( lexiconName );
	
	Q_ASSERT(module);
	if (!module) {
		return QString("module %1 not found").arg(lexiconName);
	}
	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
	key->key( data );
	module->snap();
	
	QString ret = QString::fromLatin1("<div class=\"translationinfo\"><h3>%1: %2</h3><p>%3</p></div>")
		.arg(i18n("Word translation"))
		.arg(data)
		.arg(key->renderedText());

	return ret;
}


/*!
    \fn CInfoDisplay::clearInfo()
 */
void CInfoDisplay::clearInfo() {
	m_htmlPart->begin();
	m_htmlPart->write(QString::null);
	m_htmlPart->end();		
}
