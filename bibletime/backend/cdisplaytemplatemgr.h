//
// C++ Interface: cdisplaytemplatemgr
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef CDISPLAYTEMPLATEMGR_H
#define CDISPLAYTEMPLATEMGR_H

//BibleTime include
#include "cswordmoduleinfo.h"

//Qt includes
#include <qmap.h>
#include <qstring.h>
#include <qstringlist.h>

/**
Manages the display templates used in the filters and display classes.

@author The BibleTime team
*/
class CDisplayTemplateMgr{
public:
    struct Settings {
			Settings() {
				title = QString::null;
				langAbbrev = QString::null;
			};
			
			ListCSwordModuleInfo modules;
			QString title;
			QString langAbbrev;
		};
		
		CDisplayTemplateMgr();
    ~CDisplayTemplateMgr();
		
    const QStringList availableTemplates();
    const QString fillTemplate( const QString& name, const QString& content, Settings& settings);
    
		static const QString defaultTemplate();

private:
    void init();
		QMap<QString, QString> m_templateMap;
};

#endif
