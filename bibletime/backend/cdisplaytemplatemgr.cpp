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

#include "config.h"

//Qt includes
#include <qstringlist.h>

CDisplayTemplateMgr::CDisplayTemplateMgr() {
	init();
}


CDisplayTemplateMgr::~CDisplayTemplateMgr() {
}


/*!
    \fn CDisplayTemplateMgr::init()
 */
void CDisplayTemplateMgr::init() {
	m_templateMap["Default"] = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\
<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Strict//EN\"\ \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd\">\
<html xmlns=\"http://www.w3.org/1999/xhtml\">\
<head>\
	<title>#TITLE#</title>\
	<meta name=\"GENERATOR\" content=\"BibleTime " VERSION "\" />\
	<meta name=\"AUTHOR\" content=\"BibleTime " VERSION "\" />\
	<meta http-equiv=\"Content-Type\" content=\"text/html; charset=UTF-8\" />\
	<style type=\"text/css\">\
	/* <![CDATA[ */\
body {\
	margin:20px;\
}\
#content {\
	background-color:white;\
}\
#content table {\
	margin:0;\
	padding:0;\
	border-spacing:0;\
	border-collapse:collapse;\
	vertical-align:top;\
}\
\
a {\
	text-decoration:none;\
	font-weight:normal;\
	color:blue;\
	padding:2px;\
}\
a:hover {\
	background-color:#FBFBFB;\
	padding:2px;\
}\
div.verse, td.verse {\
	padding:5px;\
	vertical-align:top;\
}\
span.verse {\
}\
div.currentverse, td.currentverse {\
	padding:5px;\
	border:1px solid black;\
	background-color:#F6FFF7;\
	vertical-align:top;\
}\
td.verse + td.verse, td.currentverse  + td.currentverse { \
} \
span.currentverse {\
	background-color:blue;\
	color:darkRed;\
}\
.versenum {\
}\
.versenum a {\
	padding-right:5px;\
	padding-left:5px;\
	font-size: 60%;\
	vertical-align:top;\
}\
	/* ]]> */\
	</style>\
</head>\
<body>\
<div id=\"content\">\
#CONTENT#\
</div>\
</body>\
</html>";

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
const QString CDisplayTemplateMgr::fillTemplate( const QString& name, const QString& title, const QString& content ) {
	if (!m_templateMap.contains(name))
		return QString::null;
	
	QString t = m_templateMap[ name ];
	t.replace("#TITLE#", title);
	t.replace("#CONTENT#", content);
	
	return t;
}
