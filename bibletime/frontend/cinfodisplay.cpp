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
#include "backend/cswordversekey.h"
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
//	qWarning("setInfo");
	QString text;
	
	ListInfoData::const_iterator end = list.end();
	for (ListInfoData::const_iterator it = list.begin(); it != end; ++it) {
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
	CSwordBackend::DisplayOptions dispOpts;
	dispOpts.lineBreaks = true;
	dispOpts.verseNumbers = true;
		
	CrossRefRendering renderer(dispOpts);
	CTextRendering::KeyTree tree;
		
	VerseKey vk;
	ListKey refs = vk.ParseVerseList((const char*)data.utf8(), "Gen 1:1", true);
	for (int i = 0; i < refs.Count(); ++i) {
	//TODO: check and render key ranges
	
		SWKey* key = refs.getElement(i);
		Q_ASSERT(key);
		
		CTextRendering::KeyTreeItem i(
			QString::fromUtf8(key->getText()),
			CPointers::backend()->findModuleByDescription(CBTConfig::get(CBTConfig::standardBible)), 
			CTextRendering::KeyTreeItem::Settings()
		);
		
		tree.append( i );
	}	
	
	return QString::fromLatin1("<div class=\"crossrefinfo\"><h3>%1</h3>%2</div>")
		.arg(i18n("Cross references"))
		.arg(renderer.renderKeyTree(tree));
}

/*!
    \fn CInfoDisplay::decodeFootnote( const QString& data )
 */
const QString CInfoDisplay::decodeFootnote( const QString& data ) {
	QStringList list = QStringList::split("/", data);
	Q_ASSERT(list.count() >= 3);
	if (!list.count()) {
		return QString::null;
	}
		
	const QString modulename = list[0];
	const QString keyname = list[1];
	const QString swordFootnote = list[2];
	
//	Q_ASSERT(!modulename.isEmpty());
//	Q_ASSERT(!keyname.isEmpty());
//	Q_ASSERT(!swordFootnote.isEmpty());

//	qWarning("data: %s, %s, %s", modulename.latin1(), keyname.latin1(), swordFootnote.latin1());
	
	CSwordModuleInfo* module = CPointers::backend()->findModuleByName(modulename);
// 	Q_ASSERT(module);
	if (!module) {
		return QString::null;
	}
	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );	
	key->key(keyname);
	key->renderedText(); //force entryAttributes
	
	QString text = QString::fromUtf8(module->module()->getEntryAttributes()["Footnote"][swordFootnote.latin1()]["body"].c_str());
	
	text = QString::fromUtf8( module->module()->RenderText( 
		module->isUnicode() ? (const char*)text.utf8() : (const char*)text.latin1()
	));
	
	return QString::fromLatin1("<div class=\"footnoteinfo\"><h3>%1</h3><p>%2</p></div>")
		.arg(i18n("Footnote"))
		.arg(text);
}

const QString CInfoDisplay::decodeLemma( const QString& data ) {
//	qWarning("decode lemma: %s", data.latin1());
	
	QStringList lemmas = QStringList::split("|", data);
	QString ret;
	
	QStringList::const_iterator end = lemmas.end();
	for (QStringList::const_iterator it = lemmas.begin(); it != end; ++it) {
		QString strongModuleDesc = CBTConfig::get((*it).left(1) == "H" ? 
			CBTConfig::standardHebrewStrongsLexicon : 
			CBTConfig::standardGreekStrongsLexicon
		);
	
		CSwordModuleInfo* module = CPointers::backend()->findModuleByDescription( strongModuleDesc );	
		if (!module) {
			continue;
		}
		
		
		util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
		key->key( (*it).mid(1) ); //skip H or G (language sign), will have to change later if we have better modules
		ret += QString::fromLatin1("<div class=\"lemmainfo\"><h3>%1: %2</h3><p>%3</p></div>")
			.arg(i18n("Lemma"))
			.arg(*it)
			.arg(key->renderedText());

	}
		
	return ret;
}

const QString CInfoDisplay::decodeMorph( const QString& data ) {
//	qWarning("decodeMorph");
	QStringList morphs = QStringList::split("|", data);
	QString ret;
		
	for (QStringList::iterator it = morphs.begin(); it != morphs.end(); ++it) {
		QString strongModuleDesc = CBTConfig::get((*it).left(1) == "H" ? 
			CBTConfig::standardHebrewMorphLexicon : 
			CBTConfig::standardGreekMorphLexicon
		);
		
		CSwordModuleInfo* module = CPointers::backend()->findModuleByDescription( strongModuleDesc );	
		if (!module) {
			continue;
		}
		
		util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
		key->key( (*it).mid(1) ); //skip H or G (language sign)
	
		ret += QString::fromLatin1("<div class=\"morphinfo\"><h3>%1: %2</h3><p>%3</p></div>")
			.arg(i18n("Morph number"))
			.arg(*it)
			.arg(key->renderedText());
	}
	
	return ret;	
}

const QString CInfoDisplay::getWordTranslation( const QString& data ) {
	const QString lexiconDescr = CBTConfig::get(CBTConfig::standardLexicon);
	
	CSwordModuleInfo* module = CPointers::backend()->findModuleByDescription( lexiconDescr );	
	if (!module) {
		return QString::null;
	}
	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
	key->key( data );
	if (key->key().upper() != data.upper()) { //key not present in the lexicon
		return QString::null;
	}
	
	QString ret = QString::fromLatin1("<div class=\"translationinfo\"><h3>%1: %2</h3><p>%3</p></div>")
		.arg(i18n("Word lookup"))
		.arg(data)
		.arg(key->renderedText());

	return ret;
}


/*!
    \fn CInfoDisplay::clearInfo()
 */
void CInfoDisplay::clearInfo() {
	CDisplayTemplateMgr tmgr;
	CDisplayTemplateMgr::Settings settings;
	settings.pageCSS_ID = "infodisplay";
	
	m_htmlPart->begin();
	m_htmlPart->write( tmgr.fillTemplate(CBTConfig::get(CBTConfig::displayStyle), QString::null, settings) );
	m_htmlPart->end();
}


/**
 * New class: CInfoDisplay::refRendering
 */
CInfoDisplay::CrossRefRendering::CrossRefRendering( CSwordBackend::DisplayOptions displayOptions, CSwordBackend::FilterOptions filterOptions) 
	: CHTMLExportRendering(Settings(), displayOptions, filterOptions)
{

}
 
const QString CInfoDisplay::CrossRefRendering::finishText( const QString& text, KeyTree& ) {

//	qWarning(text.latin1());
	return text;
}
