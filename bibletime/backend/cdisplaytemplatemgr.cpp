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

#include <iostream>

CDisplayTemplateMgr::CDisplayTemplateMgr() {
	init();
}


CDisplayTemplateMgr::~CDisplayTemplateMgr() {
}

const QString CDisplayTemplateMgr::defaultTemplate() {
	return i18n("Default");
}

/*!
    \fn CDisplayTemplateMgr::init()
 */
void CDisplayTemplateMgr::init() {
	m_templateMap[i18n("Default")] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\n\
<html xmlns=\"http://www.w3.org/1999/xhtml\">\n\
<head>\n\
	<title>#TITLE#</title>\n\
	<meta name=\"GENERATOR\" content=\"BibleTime " VERSION "\" />\n\
	<meta name=\"AUTHOR\" content=\"BibleTime " VERSION "\" />\n\
	<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\n\
	<style type=\"text/css\">\n\
	/* <![CDATA[ */\n\
body {\n\
	margin:15px;\
}\n\
#content {\n\
	background-color:white;\
}\n\
#content table {\n\
	margin:0;\
	padding:0;\
	border-spacing:15px;;\
	border-collapse:collapse;\
	vertical-align:top;\
}\n\
#content table th {\n\
	padding: 3px; \
	text-align: center; \
	font-weight: bold;\
	font-siye: 1.1em;\
	border-bottom: 1px solid darkGray;\
}\n\
a {\n\
	text-decoration:none;\
	font-weight:normal;\
	color:blue;\
	padding:2px;\
}\n\
a:hover {\n\
	background-color:#FBFBFB;\
	color:darkRed;\
	padding:2px;\
}\n\
div.verse, td.verse, div.entry {\n\
	padding: 5px;\
	vertical-align: top;\
}\n\
span.verse, span.entry {\n\
	padding: 5px;\
}\n\
div.currentverse, td.currentverse, div.currententry {\n\
	padding: 5px;\
	border: 1px solid black;\
	background-color: #F6FFF7;\
	vertical-align: top;\
}\n\
td.verse + td.verse, td.entry + td.entry { \
	margin-left:5px;\
}\n\
span.currentverse, span.currententry {\n\
	background-color: #F6FFF7;\
	vertical-align: middle;\
	padding: 5px;\
}\n\
.versenum, .entryname {\n\
}\n\
\
.versenum a , .entryname a  {\n\
	font-size: 70%;\
	vertical-align: top;\
}\
.versenum[dir=ltr] a , .entryname[dir=ltr] a  {\n\
	padding-right: 2px;\
	padding-left: 10px;\
}\n\
.versenum[dir=rtl] a, .entryname[dir=rtl] a {\n\
	padding-right: 10px;\
	padding-left: 2px;\
}\n\
\
.footnotepre {\n\
	font-color: gray;\
	font-size: 0.9em;\
	font-style: italic;\
}\n\
.footnote {\n\
	font-color: gray;\
	font-size: 0.9em;\
	font-style: italic;\
}\n\
.strongnumber {\n\
	vertical-align:top;\
	font-size:50%;\
	color:gray;\
\
}\n\
.morphcode {\n\
	vertical-align:top;\
	font-size:60%;\
	color:gray;\
}\n\
.lemma {\n\
}\n\
.strongnumber:hover, .morphcode:hover {\n\
	color:darkRed;\
}\
.sectiontitle {\n\
	font-weight:bold;\
	font-size:120%;\
}\n\
.booktitle {\n\
	font-weight:bold;\
	font-size:130%;\
}\n\
.foreign {\n\
}\n\
.jesuswords {\n\
	color:#AC0508;\
	font-size:0.9em;\
}\n\
.quotation {\n\
}\n\
.poetry {\n\
}\n\
.sup {\n\
	vertical-align:sup;\
}\n\
.sub {\n\
	vertical-align:sub;\
}\n\
.right {\n\
	text-align:right;\
}\n\
.center {\n\
	text-align:center;\
}\n\
\n#LANG_CSS#\n\
	/* ]]> */\n\
	</style>\n\
</head>\n\
<body>\n\
<div id=\"content\" lang=\"#LANG_ABBREV#\">\
\n#CONTENT#\n\
</div>\n\
</body>\n\
</html>\n";
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
