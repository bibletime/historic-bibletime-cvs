/***************************************************************************
                          creferencemanager.cpp  -  description
                             -------------------
    begin                : Sun Aug 19 2001
    copyright            : (C) 2001 by The BibleTime team
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

#include "creferencemanager.h"
#include "cswordversekey.h"

#include "../frontend/cbtconfig.h"

//QT includes
#include <qregexp.h>

/** Returns a hyperlink used to be imbedded in the display windows. At the moment the format is sword://module/key */
const QString CReferenceManager::encodeHyperlink( const QString& moduleName, const QString& key, const CReferenceManager::Type& type){
	QString ret = QString::null;
	switch (type) {
		case Bible:				
			ret = QString::fromLatin1("sword://Bible/");
			break;
		case Commentary:
			ret = QString::fromLatin1("sword://Commentary/");
			break;
		case Lexicon:
			ret = QString::fromLatin1("sword://Lexicon/");
			break;
		case GenericBook:
			ret = QString::fromLatin1("sword://Book/");
			break;			
		case MorphHebrew:				
			ret = QString::fromLatin1("morph://Hebrew/");		
			break;
		case MorphGreek:
			ret = QString::fromLatin1("morph://Greek/");				
			break;
		case StrongsHebrew:				
			ret = QString::fromLatin1("strongs://Hebrew/");				
			break;
		case StrongsGreek:				
			ret = QString::fromLatin1("strongs://Greek/");					
			break;
		default:
			break;
	}

	if (!moduleName.isEmpty()) {
		ret += moduleName + QString::fromLatin1("/");
  }
	else { //if module is empty use fallback module
		ret += preferredModule(type) + QString::fromLatin1("/");
	}	
	
	if (type == GenericBook) {
		const QString s = (!key.isEmpty() ? key : QString::null);
		QString newKey = QString::null;	
	  //replace all / of the key (e.g. of a CSwordTreeKey) with
	  // the escape sequence \/ so we know it's a link internal divider (e.g. of CSwordTreeKey)!

    QChar c;
		for(unsigned int i = 0; i < s.length(); ++i) {
      c = s.at(i);
			if (c == '/') {
				newKey += "\\/";
      }
			else {
				newKey += c;
      }
		}
		ret += newKey;		
	}
	else { //slashes do not appear in verses and dictionary entries
   	switch (type) {
      case Bible: //bibles or commentary keys need parsing
  		case Commentary: {
        CSwordVerseKey vk(0);
        vk = key;
        //vk.setLocale("en");

        ret += vk.key(); //we add the english key, so drag and drop will work in all cases
        break;
      }
      default:
        ret += key; //use the standard key, no parsing required
        break;
    }
	}
	return ret;
}

/** Decodes the given hyperlink to module and key. */
const bool CReferenceManager::decodeHyperlink( const QString& hyperlink, QString& module, QString& key, CReferenceManager::Type& type ){
	/**
	* We have to decide between three types of URLS: sword://Type/Module/Key, morph://Testament/key and strongs://Testament/Key
	*/	
  module = QString::null;
  key = QString::null;
  	
	type = Unknown; //not yet known
	QString ref = hyperlink;
	//remove the trailing slash
	if (ref.right(1)=="/" && ref.right(2) != "\\/") //triling slash, but not escaped
		ref = ref.left(ref.length()-1);
	
	//find out which type we have by looking at the beginning (protocoll section of URL)
	if (ref.left(8) == "sword://") { //Bible, Commentary or Lexicon
		ref = ref.mid(8);
		if (ref.left(5) == "Bible") { //a bible hyperlink
			type = CReferenceManager::Bible;			
			ref = ref.mid(6); //inclusive trailing slash
		}
		else if (ref.left(10) == "Commentary") { // a Commentary hyperlink
			type = CReferenceManager::Commentary;			
			ref = ref.mid(11); //inclusive trailing slash
		}
		else if (ref.left(7) == "Lexicon") { // a Lexicon hyperlink
			type = CReferenceManager::Lexicon;
			ref = ref.mid(8); //inclusive trailing slash
		}
		else if (ref.left(4) == "Book") { // a Book hyperlink
			type = CReferenceManager::GenericBook;
			ref = ref.mid(5); //inclusive trailing slash
		}		
		// string up to next slash is the modulename
		if (ref.at(0) != '/' ) { //we have a module given
      while (true) {
  			const int pos = ref.find("/");
  			if (pos>0 && ref.at(pos-1) != '\\') { //found a slash which is not escaped
  				module = ref.mid(0,pos);
  				ref = ref.mid(pos+1);
  				break;
  			}
        else if (pos == -1) {
          break;
        }
  		}
  		// the rest is the key
   		key = ref;
    }
    else {
      key = ref.mid(1);
    }
 		//replace \/ escapes with /
 		key.replace(QRegExp("\\\\/"), "/");
	}
	else if (ref.left(8) == "morph://" || ref.left(10) == "strongs://") { //strongs or morph URL have the same format
		enum PreType {IsMorph, IsStrongs};
		PreType preType = IsMorph;
		if (ref.left(8) == "morph://") { //morph code hyperlink
			ref = ref.mid(8);
			preType = IsMorph;
		}
		else if (ref.left(10) == "strongs://") {
			ref = ref.mid(10);		
			preType = IsStrongs;			
		}
		//part up to next slash is the language
		const int pos = ref.find("/");
		if (pos>0) { //found
			const QString language = ref.mid(0,pos);
			if (language == "Hebrew") {
				switch (preType) {
					case IsMorph:
						type = CReferenceManager::MorphHebrew;
						break;
					case IsStrongs:
						type = CReferenceManager::StrongsHebrew;
						break;								
				}			
			}
			else if (language == "Greek") {
				switch (preType) {
					case IsMorph:
						type = CReferenceManager::MorphGreek;
						break;
					case IsStrongs:
						type = CReferenceManager::StrongsGreek;
						break;								
				}
			}
			ref = ref.mid(pos+1);
			key = ref; //the remaining part is the key
			
			module = preferredModule(type);
		}
	}

  if (key.isEmpty() && module.isEmpty())
  	return false;
	return true;
}

const QString CReferenceManager::encodeReference(const QString &module, const QString &reference){
	return QString::fromLatin1("(%1)%2").arg(module).arg(reference);
}

void CReferenceManager::decodeReference(QString &dragreference, QString &module, QString &reference){
  const int pos = dragreference.find(")");
	const QString fallbackModule = dragreference.mid( 1, pos - 1);
  dragreference = dragreference.mid(pos+1);

  module = fallbackModule;
  reference = dragreference;
}

/** Returns true if the parameter is a hyperlink. */
const bool CReferenceManager::isHyperlink( const QString& hyperlink ){
	return (    hyperlink.left(8)  == "sword://")
					|| (hyperlink.left(10) == "strongs://")
					|| (hyperlink.left(8)  == "morph://");
}

/** Returns the preferred module name for the given type. */
const QString CReferenceManager::preferredModule( const CReferenceManager::Type type ){
	QString moduleName = QString::null;
	CSwordModuleInfo* module = 0;
	
	switch (type) {
		case CReferenceManager::Bible:
			module = CBTConfig::get( CBTConfig::standardBible );
			break;
		case CReferenceManager::Commentary:
			module = CBTConfig::get( CBTConfig::standardCommentary );
			break;
		case CReferenceManager::Lexicon:
			module = CBTConfig::get( CBTConfig::standardLexicon );
			break;
		case CReferenceManager::StrongsHebrew:
			module = CBTConfig::get( CBTConfig::standardHebrewStrongsLexicon );
			break;
		case CReferenceManager::StrongsGreek:
			module = CBTConfig::get( CBTConfig::standardGreekStrongsLexicon );
			break;
		case CReferenceManager::MorphHebrew:
			module = CBTConfig::get( CBTConfig::standardHebrewMorphLexicon );
			break;
		case CReferenceManager::MorphGreek:
			module = CBTConfig::get( CBTConfig::standardGreekMorphLexicon );
			break;
		default:
			module = 0;
			break;
	}
	
	return module ? module->name() : QString::null;
}

/** No descriptions */
CReferenceManager::Type CReferenceManager::typeFromModule( const CSwordModuleInfo::ModuleType type){
	switch (type) {
		case CSwordModuleInfo::Bible:
			return CReferenceManager::Bible;
		case CSwordModuleInfo::Commentary:
			return CReferenceManager::Commentary;
		case CSwordModuleInfo::Lexicon:
			return CReferenceManager::Lexicon;
    case CSwordModuleInfo::GenericBook:
      return CReferenceManager::GenericBook;
		default:
			return CReferenceManager::Unknown;
	}
}

/** Parses the given verse references using the given language and the module.*/
const QString CReferenceManager::parseVerseReference( const QString ref, const QString& lang, const QString& newLang){
	CSwordVerseKey key(0);
	if (!lang.isEmpty()) {
		key.setLocale( lang.latin1() );
  }
	
	key.key(ref);
	
	if (!lang.isEmpty() && lang != newLang) {
		key.setLocale(newLang.latin1());
  }
	return key.key();
}
