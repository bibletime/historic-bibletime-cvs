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

#include "../frontend/cbtconfig.h"

/** Returns a hyperlink used to be imbedded in the display windows. At the moment the format is sword://module/key */
const QString CReferenceManager::encodeHyperlink( const QString& module, const QString& key, const CReferenceManager::Type& type){
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
		case Morph_OT:				
			ret = QString::fromLatin1("morph://OT/");		
			break;
		case Morph_NT:
			ret = QString::fromLatin1("morph://NT/");				
			break;
		case Strongs_OT:				
			ret = QString::fromLatin1("strongs://OT/");				
			break;
		case Strongs_NT:				
			ret = QString::fromLatin1("strongs://NT/");					
			break;
		default:
			break;
	}

	if (!module.isEmpty())
		ret += module+QString::fromLatin1("/");
	if (!key.isEmpty())
		ret += key;
	ret += QString::fromLatin1("/");
	
//	qWarning(ret.local8Bit());
	return ret;
//	return QString::fromLatin1("sword://%1/%2/%3").arg(module/*.replace("/", "\\/")*/).arg(key/*.replace("/", "\\/")*/);
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
	if (ref.right(1)=="/")
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
			ref = ref.mid(11);			//inclusive trailing slash
		}
		else if (ref.left(7) == "Lexicon") { // a Lexicon hyperlink
			type = CReferenceManager::Lexicon;
			ref = ref.mid(8); //inclusive trailing slash
		}
		// string up to next slash is the modulename
		const int pos = ref.find("/");
		if (pos>0) { //found
			module = ref.mid(0,pos);
			ref = ref.mid(pos+1);			
		}
		// the rest is the key
		key = ref;		
	}
	else if (ref.left(8) == "morph://" || ref.left(10) == "strongs://") { //strongs or morph URL have the same format
		enum PreType {IsMorph, IsStrongs};
		PreType preType;
		if (ref.left(8) == "morph://") { //morph code hyperlink
			ref = ref.mid(8);
			preType = IsMorph;
		}
		else if (ref.left(10) == "strongs://") {
			ref = ref.mid(10);		
			preType = IsStrongs;			
		}
		//part up to next slash is the testament
//		qWarning("string before testament: %s", ref.latin1());
		const int pos = ref.find("/");
		if (pos>0) { //found
			const QString testament = ref.mid(0,pos); //pos or pos-1 ??
			qWarning("testament is %s", testament.latin1());
			if (testament == "OT") {
				switch (preType) {
					case IsMorph:
						type = CReferenceManager::Morph_OT;
						break;
					case IsStrongs:
						type = CReferenceManager::Strongs_OT;
						break;								
				}			
			}
			else if (testament == "NT") {
				switch (preType) {
					case IsMorph:
						type = CReferenceManager::Morph_NT;
						break;
					case IsStrongs:
						type = CReferenceManager::Strongs_NT;
						break;								
				}
			}
			ref = ref.mid(pos+1);
			key = ref; //the remaining part is the key
		}
	}

//  qWarning("decodeHyperlink: module %s key %s", module.latin1(), key.latin1());
  if (key.isEmpty() && module.isEmpty())
  	return false;
	return true;
}

const QString CReferenceManager::encodeReference(const QString &module, const QString &reference){
	return QString::fromLatin1("(%1)%2").arg(module).arg(reference);
}

void CReferenceManager::decodeReference(QString &dragreference, QString &module, QString &reference){
  const int pos = dragreference.find(")");
	const QString fallbackModule    = dragreference.mid( 1, pos - 1);
  dragreference = dragreference.mid(pos+1);

//  if (dragreference.right(1) == "/") {
//		dragreference = dragreference.left(dragreference.length()-1);		
//  }

//  if (dragreference.left(8) == "sword://") { //remove sword:// and trailing /
//		dragreference = dragreference.mid(8);
//  }
//  else if (dragreference.left(10) == "strongs://") { //remove sword:// and trailing /
//		dragreference = dragreference.mid(10);
//  }
//  else if (dragreference.left(8) == "morph://") { //remove sword:// and trailing /
//		dragreference = dragreference.mid(8);
//  }

	Type type;
//	QString mod;	
  CReferenceManager::decodeHyperlink(dragreference, module, reference, type);
  if (module.isEmpty())
  	module = preferredModule(type);
  if (module.isEmpty())
  	module = fallbackModule;
//  reference = dragreference.right( dragreference.length() - dragreference.find(")") - 1 );
}

/** Returns true if the parameter is a hyperlink. */
const bool CReferenceManager::isHyperlink( const QString& hyperlink ){
//	return ( (hyperlink.left(8) == "sword://") && hyperlink.mid(8).contains("/"));
	return (hyperlink.left(8) == "sword://") || (hyperlink.left(10) == "strongs://") || (hyperlink.left(8) == "morph://");
}

/** Returns the preferred module name for the given type. */
const QString CReferenceManager::preferredModule( const CReferenceManager::Type type ){
	QString description = QString::null;
	switch (type) {
		case CReferenceManager::Bible:
			description = CBTConfig::get( CBTConfig::standardBible );
			break;
		case CReferenceManager::Commentary:
			description = CBTConfig::get( CBTConfig::standardCommentary );
			break;			
		case CReferenceManager::Lexicon:
			description = CBTConfig::get( CBTConfig::standardLexicon );
			break;			
		case CReferenceManager::Strongs_OT:
			description = CBTConfig::get( CBTConfig::standardHebrewLexicon );
			break;			
		case CReferenceManager::Strongs_NT:
			description = CBTConfig::get( CBTConfig::standardGreekLexicon );
			break;			
		case CReferenceManager::Morph_OT:
			description = CBTConfig::get( CBTConfig::standardHebrewMorphLexicon );
			break;			
		case CReferenceManager::Morph_NT:
			description = CBTConfig::get( CBTConfig::standardGreekMorphLexicon );
			break;			
		default:
			qWarning("unknwon type");
			description = QString::null;
			break;			
	}
	if (!description.isEmpty())
		return CSwordBackend::findModuleNameByDescription(description);
	else
		return QString::null;
}

/** No descriptions */
CReferenceManager::Type CReferenceManager::typeFromModule( const CSwordModuleInfo::type type){
	switch (type) {
		case CSwordModuleInfo::Bible:
			return CReferenceManager::Bible;
		case CSwordModuleInfo::Commentary:
			return CReferenceManager::Commentary;
		case CSwordModuleInfo::Lexicon:
			return CReferenceManager::Lexicon;
		default:
			return CReferenceManager::Unknown;
	}
}
