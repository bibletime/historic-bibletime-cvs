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

//KDE includes
#include <kglobal.h>
#include <kconfig.h>
#include <kinstance.h>


/** Returns a hyperlink used to be imbedded in the display windows. At the moment the format is sword://module/key */
const QString CReferenceManager::encodeHyperlink( const QString& module, const QString& key){
//	switch (type) {
//		case morphCode:
//			return QString::fromLatin1("sword://morph_/%2").arg(key);			
//			break;
//		case strongsNumber:
//			break;		
//		default:
//			break;		
//	}
	return QString::fromLatin1("sword://%1/%2").arg(module/*.replace("/", "\\/")*/).arg(key/*.replace("/", "\\/")*/);
}

/** Decodes the given hyperlink to module and key. */
const bool CReferenceManager::decodeHyperlink( const QString& hyperlink, QString& module, QString& key){
	//remove the sword:// at the beginning (e.g. sword://WEB/Genesis 1:1/)
	QString ref = hyperlink;
	QString dummy;
	if (ref.left(8) == "sword://") { //remove sword:// and trailing /
		ref = ref.mid(8);
  }
  //remove trailing slash (now it's for example "WEB/Genesis 1:1/")
  if (ref.right(1) == "/") {
		ref = ref.mid(0, ref.length()-1);		
  }

  //the string up to the first slash is the module name, the rest is the key
  const int pos = ref.find("/");
  if (pos == -1) //not found
  	return false;

	dummy = ref.left(pos);
	if (dummy.left(8) == "strongs_") {
		module = preferedModule(CReferenceManager::strongsNumbers);
	}
	else if (dummy.left(6) == "morph_") {
		module = preferedModule(CReferenceManager::morphCode);	
	}
	else
		module = dummy;//no special type

//  module = ref.left(pos);
  qWarning("decodeHyperlink: %s", module.latin1());

  key = ref.mid(pos+1);
  qWarning("decodeHyperlink: %s", key.latin1());
  if (key.isEmpty() && module.isEmpty())
  	return false;
	return true;
}
const QString CReferenceManager::encodeReference(const QString &module, const QString &reference){
	return QString::fromLatin1("(%1)%2").arg(module).arg(reference);
}

void CReferenceManager::decodeReference(QString &dragreference, QString &module, QString &reference){
  if (dragreference.left(8) == "sword://") { //remove sword:// and trailing /
		dragreference = dragreference.mid(8, dragreference.length()-8);
  }
  if (dragreference.right(1) == "/") {
		dragreference = dragreference.mid(0, dragreference.length()-1);		
  }

  reference= dragreference.right( dragreference.length() - dragreference.find(")") - 1 );
  module   = dragreference.mid( 1, dragreference.find(")") - 1);
}

/** Returns true if the parameter is a hyperlink. */
const bool CReferenceManager::isHyperlink( const QString& hyperlink ){
	return ( (hyperlink.left(8) == "sword://") && hyperlink.mid(8).contains("/"));
}

/** Returns the name of the module prefered for the set module type. */
const QString CReferenceManager::preferedModule(CReferenceManager::Type type){
	KConfig* config = KGlobal::instance()->config();
	KConfigGroupSaver gs(config, "SWORD");
	switch (type) {
		case strongsNumbers:
			return config->readEntry("strongs module");
		case morphCode:
			return config->readEntry("morph code module");		
		default:
			return QString::null;
	}
}
