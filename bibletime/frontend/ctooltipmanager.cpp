/***************************************************************************
                          ctooltipmanager.cpp  -  description
                             -------------------
    begin                : Sam Mai 11 2002
    copyright            : (C) 2002 by The BibleTime team
    email                : info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "ctooltipmanager.h"
#include "frontend/cbtconfig.h"

#include "backend/creferencemanager.h"
#include "backend/cswordkey.h"

#include "util/scoped_resource.h"

//KDE includes
#include <klocale.h>

CTooltipManager::CTooltipManager(){
}

CTooltipManager::~CTooltipManager(){
}

/** Returns the tooltip text for the given hyperlink. */
const QString CTooltipManager::textForHyperlink( const QString& link ){
  QString moduleName;
  QString keyName;
	CReferenceManager::Type type;

 	CReferenceManager::decodeHyperlink(link, moduleName, keyName, type);
  if (moduleName.isEmpty()) {
    moduleName = CReferenceManager::preferredModule( type );
  }

  return QString::fromLatin1("<B>%1</B><HR>%2").arg(keyText(keyName)).arg(moduleText(moduleName, keyName));
}

/** Returns the tooltip text for the given hyperlink. */
const QString CTooltipManager::textForReference( const QString& moduleName, const QString& keyName, const QString& description){

  return QString::fromLatin1("<B>%1 %2</B>%3<HR>%4")
  	.arg(i18n("Bookmark to"))
  	.arg(keyText(keyName))
   	.arg(!description.isEmpty() ? QString::fromLatin1("<FONT color=\"#800000\">(%1)</FONT><BR>").arg(description.stripWhiteSpace()) : QString::null )
    .arg(moduleText(moduleName, keyName));
}


/** Returns the text for the given moduleName and key name. */
const QString CTooltipManager::moduleText( const QString& moduleName, const QString& keyName){
  QString text = QString::null;

  if (CSwordModuleInfo* module = backend()->findModuleByName(moduleName)) {
    backend()->setFilterOptions( CBTConfig::getFilterOptionDefaults() );
 		util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
 		key->key( keyName );
   	text = key->renderedText();

		if (module->type() == CSwordModuleInfo::Bible || module->type() == CSwordModuleInfo::Commentary) {
      text = QString::null;
      ListKey verses = VerseKey().ParseVerseList((const char*)keyName.local8Bit(), "Genesis 1:1", true);
			
    	for (int i = 0; i < verses.Count(); ++i) {
    		VerseKey* element = dynamic_cast<VerseKey*>(verses.GetElement(i));
    		if (element) {
     			VerseKey lowerBound = element->LowerBound();
					VerseKey upperBound = element->UpperBound();
        	
         	while (lowerBound < upperBound) {
      			key->key( (const char*)lowerBound );
		    		text += QString::fromLatin1("<B>%1</B> %2<BR>").arg(lowerBound.Verse()).arg(key->renderedText());
						lowerBound++;
          }
          if (lowerBound == upperBound) {
      			key->key( (const char*)lowerBound );
		    		text += QString::fromLatin1("<B>%1</B> %2").arg(lowerBound.Verse()).arg(key->renderedText());
          }
				}
    		else {
	  			key->key( (const char*)(*verses.GetElement(i))  );
					text += key->renderedText();
        }
    	}
		}
	}
 	return text;
}

/** Returns the text for the tooltip beginning. */
const QString CTooltipManager::keyText( const QString& keyName ){
	QString text = QString::null;
 	ListKey keys = VerseKey().ParseVerseList((const char*)keyName.local8Bit(),"Genesis 1:1", true);

 	VerseKey* element = dynamic_cast<VerseKey*>(keys.GetElement(0));
  if (element) {
		text = QString::fromLatin1("%1-%2").arg((const char*)element->LowerBound()).arg((const char*)element->UpperBound());
  }
  else
  	text = QString::fromLatin1("%1").arg((const char*)(*keys.GetElement(0)));

  return text;
}
