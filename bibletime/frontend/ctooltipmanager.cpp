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
#include "backend/centrydisplay.h"

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

  qWarning("link was %s, key is %s", link.latin1(), keyName.latin1());
  if (moduleName.isEmpty()) {
    QString typeName = QString::null;
    switch (type) {
      case CReferenceManager::Bible:
        typeName = i18n("Bible");
        break;
      case CReferenceManager::Commentary:
        typeName = i18n("Commentary");
        break;
      case CReferenceManager::Lexicon:
        typeName = i18n("Lexicon");
        break;
      case CReferenceManager::GenericBook:
        typeName = i18n("Generic Book");
        break;
  		case CReferenceManager::MorphHebrew:
        typeName = i18n("Hebrew Morphological lexicon");
        break;
  		case CReferenceManager::MorphGreek:
        typeName = i18n("Greek Morphological lexicon");
        break;
  		case CReferenceManager::StrongsHebrew:
        typeName = i18n("Hebrew Strong's Lexicon");
        break;
  		case CReferenceManager::StrongsGreek:
        typeName = i18n("Greek Strong's Lexicon");
        break;
  		default:
        break;
    }

  	return QString::fromLatin1("<FONT COLOR=\"red\"><CENTER><B>%1</B></CENTER></FONT><HR>%2</FONT>")
      .arg(i18n("Configuration problem!"))
      .arg(i18n("Please make sure the default module for the type <FONT COLOR=\"blue\"><I>%1</I></FONT> is set properly in the optionsdialog!")
          .arg(typeName)
      );
  };

  if (CSwordModuleInfo* m = backend()->findModuleByName(moduleName)) {
    return QString::fromLatin1("<HEAD><STYLE type=\"text/css\">%1</STYLE></HEAD><B>%1</B><HR>%2")
      .arg(tooltipCSS(m))
      .arg(headingText(m /*? m->type() : CSwordModuleInfo::Unknown*/, keyName))
      .arg(moduleText(moduleName, keyName)
    );
  }
  else {
    return QString::fromLatin1("<FONT COLOR=\"red\"><CENTER><B>%1</B></CENTER>%2</FONT><HR>")
            .arg(i18n("Configuration problem!"))
            .arg(i18n("The module <FONT COLOR=\"blue\"><I>%1</I></FONT> was not found on your system! Install the module to make this tooltip working!")
              .arg(moduleName)
           );
  }
}

/** Returns the tooltip text for the given hyperlink. */
const QString CTooltipManager::textForReference( const QString& moduleName, const QString& keyName, const QString& description){
	CSwordModuleInfo* const module = backend()->findModuleByName(moduleName);
  return QString::fromLatin1("<HEAD><STYLE type=\"text/css\">%1</STYLE></HEAD><B>%1 %2</B>%3<HR>%4")
    .arg(tooltipCSS(module))
  	.arg(i18n("Bookmark to"))
  	.arg(headingText(module/* ? module->type() : CSwordModuleInfo::Unknown*/, keyName))
   	.arg(!description.isEmpty() ? QString::fromLatin1("<FONT color=\"#800000\">(%1)</FONT><BR>").arg(description.stripWhiteSpace()) : QString::null )
    .arg(moduleText(moduleName, keyName));
}


/** Returns the text for the given moduleName and key name. */
const QString CTooltipManager::moduleText( const QString& moduleName, const QString& keyName){
  QString text = QString::null;
  if (CSwordModuleInfo* module = backend()->findModuleByName(moduleName)) {
  	util::scoped_ptr<CSwordKey> key( CSwordKey::createInstance(module) );
  	key->key( keyName );

    backend()->setFilterOptions( CBTConfig::getFilterOptionDefaults() );
   	text = key->renderedText();

		if (module->type() == CSwordModuleInfo::Bible || module->type() == CSwordModuleInfo::Commentary) {
      text = QString::null;
      sword::ListKey verses = sword::VerseKey().ParseVerseList((const char*)keyName.local8Bit(), "Genesis 1:1", true);
			
    	for (int i = 0; i < verses.Count(); ++i) {
    		sword::VerseKey* element = dynamic_cast<sword::VerseKey*>(verses.GetElement(i));
    		if (element) {
     			sword::VerseKey lowerBound = element->LowerBound();
          lowerBound.Headings(false);

					sword::VerseKey upperBound = element->UpperBound();
          upperBound.Headings(false);
        	
         	while (lowerBound < upperBound) {
      			key->key( (const char*)lowerBound );
		    		text += QString::fromLatin1("<B>%1:%2</B> %3<BR>")
              .arg(lowerBound.Chapter())
              .arg(lowerBound.Verse())
              .arg(key->renderedText());
						lowerBound++;
          }
          if (lowerBound == upperBound) {
      			key->key( (const char*)lowerBound );
		    		text += QString::fromLatin1("<B>%1:%2</B> %3<BR>")
              .arg(lowerBound.Chapter())
              .arg(lowerBound.Verse())
              .arg(key->renderedText());
          }
				}
    		else {
	  			key->key( (const char*)(*verses.GetElement(i))  );
					text += key->renderedText();
        }
    	}
		}


//    if (module->isUnicode()) {
      text = QString::fromLatin1("<DIV %1 STYLE=\"font-family:%2; font-size:%3pt;\">")
              .arg(module->textDirection() == CSwordModuleInfo::RightToLeft ? "dir=\"rtl\"" : 0)
              .arg(CBTConfig::get(module->language()).family())
              .arg(CBTConfig::get(module->language()).pointSize())
             + text + QString::fromLatin1("</DIV>");
//    }
	}
 	return text;
}

/** Returns the text for the tooltip beginning. */
const QString CTooltipManager::headingText( CSwordModuleInfo* module, const QString& keyName ){
  const QString defaultEnding = module ? QString::fromLatin1("  (<SMALL>%1 \"%2\"</SMALL>)").arg(i18n("Module")).arg(module->name()) : i18n("module not set!");
	if (module->type() == CSwordModuleInfo::Bible || module->type() == CSwordModuleInfo::Commentary) {
	  QString text = QString::null;
    qWarning("want to parse the key %s", keyName.latin1());
    sword::ListKey keys = sword::VerseKey().ParseVerseList((const char*)keyName.local8Bit(), sword::VerseKey("Genesis 1:1"), true);
    qWarning("parsed the key %s", keyName.latin1());
	  if (sword::VerseKey* element = dynamic_cast<sword::VerseKey*>(keys.GetElement(0)))
			text = QString::fromLatin1("%1-%2").arg((const char*)element->LowerBound()).arg((const char*)element->UpperBound());
	  else
	  	text = QString::fromLatin1("%1").arg((const char*)(*keys.GetElement(0)));
	  return text + defaultEnding;
  }
  else { //non-versekeys are not localized
		return keyName + defaultEnding;
  };
  return QString::null;
}

/** Returns the CSS data used for the tooltips. */
const QString CTooltipManager::tooltipCSS(CSwordModuleInfo* module){
  CEntryDisplay* display = module ? module->getDisplay() : 0;
//  Q_ASSERT(display);
  if (!display)
    return QString::null;

  QString css = QString::null;
  for (int i = CEntryDisplay::MinType; i <= CEntryDisplay::MaxType; ++i) {
    CEntryDisplay::StyleType type = static_cast<CEntryDisplay::StyleType>(i);
    if (type != CEntryDisplay::Body && type != CEntryDisplay::Background) {
      css += display->cssString( type );
    }
  }
  return css;
}
