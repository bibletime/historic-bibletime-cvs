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
#include "backend/creferencemanager.h"
#include "backend/cdisplaytemplatemgr.h"

#include "frontend/cbtconfig.h"
#include "frontend/crossrefrendering.h"

#include "frontend/display/cdisplay.h"
#include "frontend/display/creaddisplay.h"

#include "util/scoped_resource.h"

//Qt includes
#include <qlayout.h>
#include <qlabel.h>
#include <qscrollview.h>

//KDE includes
#include <klocale.h>


using namespace Rendering;

CInfoDisplay::CInfoDisplay(QWidget *parent, const char *name)
    : QWidget(parent, name)
{
	QVBoxLayout* layout = new QVBoxLayout(this);
	QLabel* headingLabel = new QLabel(i18n("Info display"),this);
	
	m_htmlPart = CDisplay::createReadInstance(0, this);
	m_htmlPart->setMouseTracking(false); //we don't want strong/lemma/note mouse infos
	
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
// 	qWarning("we have %i list items", list.count());
	
	ListInfoData::const_iterator end = list.end();
	for (ListInfoData::const_iterator it = list.begin(); it != end; ++it) {
	  switch ( (*it).first ) {
			case Lemma:
				text += decodeStrongs( (*it).second );
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
	
	CDisplayTemplateMgr* mgr = CPointers::displayTemplateManager();
	CDisplayTemplateMgr::Settings settings;
	settings.pageCSS_ID = "infodisplay";
	QString content = mgr->fillTemplate(CBTConfig::get(CBTConfig::displayStyle), text, settings);
	
	m_htmlPart->setText(content);
}


const QString CInfoDisplay::decodeCrossReference( const QString& data ) {
	if (data.isEmpty()) {
		return QString::fromLatin1("<div class=\"crossrefinfo\"><h3>%1</h3></div>")
			.arg(i18n("Cross references"));
	}

	CSwordBackend::DisplayOptions dispOpts;
	dispOpts.lineBreaks 	= false;
	dispOpts.verseNumbers = true;
	
	CSwordBackend::FilterOptions filterOpts;
	filterOpts.headings 			= false;
	filterOpts.strongNumbers 	= false;
	filterOpts.morphTags 			= false;
	filterOpts.lemmas 				= false;
	filterOpts.footnotes			= false;
			
	CrossRefRendering renderer(dispOpts, filterOpts);
	CTextRendering::KeyTree tree;
		
	VerseKey vk;
	ListKey refs = vk.ParseVerseList((const char*)data.utf8(), "Gen 1:1", true);
	
	for (int i = 0; i < refs.Count(); ++i) {
		SWKey* key = refs.getElement(i);
		Q_ASSERT(key);
		VerseKey* vk = dynamic_cast<VerseKey*>(key);		
		
		CTextRendering::KeyTreeItem::Settings settings(
			false, 
			CTextRendering::KeyTreeItem::Settings::CompleteShort
		);
		
		CTextRendering::KeyTreeItem* i = 0;
		if (vk && vk->isBoundSet()) { //render a range of keys
			i = new CTextRendering::KeyTreeItem(
				QString::fromUtf8(vk->LowerBound().getText()),
				QString::fromUtf8(vk->UpperBound().getText()),
				CBTConfig::get(CBTConfig::standardBible), 
				settings
			);
		}
		else {
			i = new CTextRendering::KeyTreeItem(
				QString::fromUtf8(key->getText()),
				QString::fromUtf8(key->getText()),
				CBTConfig::get(CBTConfig::standardBible), 
				settings
			);
		}
		
		tree.append( i );
	}	
	
	return QString::fromLatin1("<div class=\"crossrefinfo\"><h3>%1</h3><p>%2</p></div>")
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

	CSwordModuleInfo* module = CPointers::backend()->findModuleByName(modulename);
	if (!module) {
		return QString::null;
	}
	
	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );	
	key->key(keyname);
	key->renderedText(); //force entryAttributes
	
	const char* note = module->module()->getEntryAttributes()["Footnote"][swordFootnote.latin1()]["body"].c_str();
	
	QString text = module->isUnicode() ? QString::fromUtf8(note) : QString::fromLatin1(note);
	text = QString::fromUtf8( module->module()->RenderText( 
		module->isUnicode() ? (const char*)text.utf8() : (const char*)text.latin1()
	));
	
	return QString::fromLatin1("<div class=\"footnoteinfo\"><h3>%1</h3><p>%2</p></div>")
		.arg(i18n("Footnote"))
		.arg(text);
}

const QString CInfoDisplay::decodeStrongs( const QString& data ) {
	QStringList strongs = QStringList::split("|", data);
// 	qWarning("%s, we have %i srongs", data.latin1(), strongss.count());
	QString ret;
	
	QStringList::const_iterator end = strongs.end();
	for (QStringList::const_iterator it = strongs.begin(); it != end; ++it) {
		CSwordModuleInfo* const module = CBTConfig::get( 
			(*it).left(1) == "H" ? 
			CBTConfig::standardHebrewStrongsLexicon : 
			CBTConfig::standardGreekStrongsLexicon
 		);
		
		QString text;
		if (module) { 
			util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
			key->key( (*it).mid(1) ); //skip H or G (language sign), will have to change later if we have better modules
			text = key->renderedText();
		}		
		//if the module could not be found just display an empty lemma info
				
		ret += QString::fromLatin1("<div class=\"strongsinfo\"><h3>%1: %2</h3><p>%3</p></div>")
			.arg(i18n("Strongs"))
			.arg(*it)
			.arg(text);
	}
		
	return ret;
}

const QString CInfoDisplay::decodeMorph( const QString& data ) {
//	qWarning("decodeMorph");
	QStringList morphs = QStringList::split("|", data);
	QString ret;
		
	for (QStringList::iterator it = morphs.begin(); it != morphs.end(); ++it) {
		CSwordModuleInfo* const module = CBTConfig::get(
			(*it).left(1) == "H" ? 
			CBTConfig::standardHebrewMorphLexicon : 
			CBTConfig::standardGreekMorphLexicon
		);
		
		QString text;
		if (module) {
			util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
			if ((key->key().at(0) == 'G') || (key->key().at(0) == 'H')) {
				key->key( (*it).mid(1) ); //skip H or G (language sign)
			}
			else {
				key->key( *it );
			}
			
			text = key->renderedText();
		}
		//if the module wasn't found just display an empty morph info
		
		ret += QString::fromLatin1("<div class=\"morphinfo\"><h3>%1: %2</h3><p>%3</p></div>")
			.arg(i18n("Morphology"))
			.arg(*it)
			.arg(text);
	}
	
	return ret;	
}

const QString CInfoDisplay::getWordTranslation( const QString& data ) {
	CSwordModuleInfo* const module = CBTConfig::get(CBTConfig::standardLexicon);
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
	CDisplayTemplateMgr* tmgr = CPointers::displayTemplateManager();
	CDisplayTemplateMgr::Settings settings;
	settings.pageCSS_ID = "infodisplay";
	
	m_htmlPart->setText( tmgr->fillTemplate(CBTConfig::get(CBTConfig::displayStyle), QString::null, settings) );
}

