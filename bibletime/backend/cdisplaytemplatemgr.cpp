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
#include "cswordmoduleinfo.h"
#include "clanguagemgr.h"

#include "frontend/cbtconfig.h"

#include "util/cpointers.h"

#include "config.h"

//Qt includes
#include <qstringlist.h>

//KDE includes
#include <klocale.h>

#include <iostream>

CDisplayTemplateMgr::CDisplayTemplateMgr() {
	init();
}

CDisplayTemplateMgr::~CDisplayTemplateMgr() {
}


/*!
    \fn CDisplayTemplateMgr::fillTemplate( const QString& name, const QString& title, const QString& content )
 */
const QString CDisplayTemplateMgr::fillTemplate( const QString& name, const QString& content, Settings& settings ) {
// 	if (!m_templateMap.contains(name)) {
// 		
// 	}

	const QString templateName = 	m_templateMap.contains(name) ? name : defaultTemplate();

	QString langCSS;
  CLanguageMgr::LangMap langMap = CPointers::languageMgr()->availableLanguages();
	
  for ( CLanguageMgr::LangMapIterator it( langMap ); it.current(); ++it ) {
  	const CLanguageMgr::Language* lang = it.current();
		if (lang->isValid() && CBTConfig::get(lang).first) {
			const QFont f = CBTConfig::get(lang).second;
			langCSS += QString::fromLatin1("*[lang=%1] { font-family:%2; font-size:%3pt; font-weight:%3; font-style: %4;}\n")
				.arg(lang->abbrev())
				.arg(f.family()).arg(f.pointSize())
				.arg(f.bold() ? "bold" : "normal")
				.arg(f.italic() ? "italic" : "normal");
		}
	}
	
	QString displayTypeString;
	if (!settings.pageCSS_ID.isEmpty()) {
		displayTypeString = settings.pageCSS_ID;
	}
	else {
		if (settings.modules.count()) {
			switch (settings.modules.first()->type()) {
				case CSwordModuleInfo::Bible:
					displayTypeString = "bible";
					break;
				case CSwordModuleInfo::GenericBook:
					displayTypeString = "book";
					break;
				
				case CSwordModuleInfo::Commentary:
				case CSwordModuleInfo::Lexicon:
				default:
					displayTypeString = "singleentry";
					break;
			};
		};
	}
		
	QString newContent = content;
	const int moduleCount = settings.modules.count();
	if (moduleCount >= 2) {
		//create header for the modules
		QString header;
		for (CSwordModuleInfo* m = settings.modules.first(); m; m = settings.modules.next()) {
			header += QString::fromLatin1("<th style=\"width:%1%;\">%2</th>")
				.arg( int( 100.0 / (float)moduleCount ) )
				.arg( m->name() );
		}
		
		newContent = QString::fromLatin1("<table><tr>%1</tr>%2</table>")
			.arg(header)
			.arg(content);
	}
	
	QString t = m_templateMap[ templateName ];
	t.replace("#TITLE#", settings.title);
	t.replace("#LANG_CSS#", langCSS);
	t.replace("#LANG_ABBREV#", settings.langAbbrev);
 	t.replace("#DISPLAYTYPE#", displayTypeString);
	t.replace("#CONTENT#", newContent);
	
//	std::cout << t.latin1() << std::endl;
	
	return t;
}

//Include the HTML templates which were put into a cpp file by a Perl script
#include "../display-templates/template-init.cpp"
