//
// C++ Implementation: cdisplaytemplatemgr
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
#include "cdisplaytemplatemgr.h"
#include "clanguagemgr.h"

#include "frontend/cbtconfig.h"

#include "util/cpointers.h"

#include "config.h"

//Qt includes
#include <qstringlist.h>

//KDE includes
#include <klocale.h>

//#include <iostream>

CDisplayTemplateMgr::CDisplayTemplateMgr() {
	init();
}

CDisplayTemplateMgr::~CDisplayTemplateMgr() {
}

const QString CDisplayTemplateMgr::defaultTemplate() {
	return i18n("Default");
}

/*!
    \fn CDisplayTemplateMgr::availableTemplates()
 */
const QStringList CDisplayTemplateMgr::availableTemplates() {
	QStringList names = m_templateMap.keys();
	/*QMap<QString, QString>::Iterator it;
	for ( it = m_templateMap.begin(); it != m_templateMap.end(); ++it ) {
		names << it.key();
	}*/
	
	return names;
}


/*!
    \fn CDisplayTemplateMgr::fillTemplate( const QString& name, const QString& title, const QString& content )
 */
const QString CDisplayTemplateMgr::fillTemplate( const QString& name, const QString& title, const QString& content, const QString& langAbbrev ) {
	if (!m_templateMap.contains(name))
		return QString::null;

	QString langCSS;
  CLanguageMgr::LangMap langMap = CPointers::languageMgr()->availableLanguages();
  CLanguageMgr::LangMap::Iterator it;
  for ( it = langMap.begin(); it != langMap.end(); ++it ) {
    //const QString name = it.data().translatedName().isEmpty() ? it.data().abbrev() : it.data().translatedName();
  	CLanguageMgr::Language lang = it.data();
		if (lang.isValid() && CBTConfig::get(lang).first) {
			QFont f = CBTConfig::get(lang).second;
			langCSS += QString::fromLatin1("*[lang=%1] { font-family:%2; font-size:%3pt; font-weight:%3; font-style: %4;}\n")
				.arg(lang.abbrev())
				.arg(f.family()).arg(f.pointSize())
				.arg(f.bold() ? "bold" : "normal")
				.arg(f.italic() ? "italic" : "normal");
		}
	}
			
	QString t = m_templateMap[ name ];
	t.replace("#TITLE#", title);
	t.replace("#LANG_CSS#", langCSS);
	t.replace("#LANG_ABBREV#", langAbbrev);
	t.replace("#CONTENT#", content);
	
	//std::cout << t.latin1() << std::endl;
	
	return t;
}

#include "../display-templates/template-init.cpp"
