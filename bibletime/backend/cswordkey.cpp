/***************************************************************************
                          cswordkey.cpp  -  description
                             -------------------
    begin                : Thu May 24 2001
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


//own includes
#include "cswordkey.h"
#include "cswordmoduleinfo.h"

#include "cswordversekey.h"
#include "cswordldkey.h"
#include "cswordtreekey.h"

#include "util/ctoolclass.h"

//Sword includes
#include <swmodule.h>
#include <swkey.h>
#include <versekey.h>
#include <treekey.h>
#include <treekeyidx.h>

//Qt includes
#include <qregexp.h>
#include <qstring.h>

CSwordKey::CSwordKey(CSwordModuleInfo* const module) : m_module(module) {
}

CSwordKey::CSwordKey(const CSwordKey& k) {
	m_module = k.m_module;
}

const QString CSwordKey::rawText() {
  if (!m_module) {
		return QString::null;
	}

	if (sword::SWKey* k = dynamic_cast<sword::SWKey*>(this)) {
    m_module->module()->SetKey(k);
	}
  if (key().isNull()) {
    return QString::null;
  }
	
  return QString::fromUtf8(m_module->module()->getRawEntry());
}

const QString CSwordKey::renderedText( const CSwordKey::TextRenderType mode) {
  if (!m_module)
		return QString::null;

	if (sword::SWKey* k = dynamic_cast<sword::SWKey*>(this)) {
    m_module->module()->SetKey(k);
	}
  
	if (!key().isNull()) { //we have valid text
    const QString text = QString::fromUtf8( m_module->module()->RenderText() );
    
		if (mode == HTMLEscaped) {
      //we have to encode all UTF-8 in HTML escapes
      // go though every character and write down the escaped HTML unicode entity
      // form is &#<decimal unicode value here>;
      QString ret;
      QChar c;
      const unsigned int length = text.length();
      for (unsigned int i = 0; i < length; ++i) {
        c = text.at(i);
        if (c.latin1()) { //normal latin1 character
          ret.append(c);
				}
        else {//unicode character, needs to be escaped
          ret.append("&#")
						 .append(c.unicode())
						 .append(";");
				}
      };
      return ret;
    }
    else {
			return text;
		}
  }
  return QString::null;
}

const QString CSwordKey::strippedText() {
  if (!m_module) {
		return QString::null;
	}
	
	if (sword::SWKey* k = dynamic_cast<sword::SWKey*>(this)) {
		m_module->module()->SetKey(k);
  }
	return QString::fromUtf8( m_module->module()->StripText() );
}

/** This will create a proper key object from a given module */
CSwordKey* CSwordKey::createInstance( CSwordModuleInfo* const module ){
	if (!module)
		return 0;

	switch( module->type() ){
		case CSwordModuleInfo::Bible://fall through
		case CSwordModuleInfo::Commentary:
			return new CSwordVerseKey( (sword::VerseKey *) ( (sword::SWKey *)(*module->module()) ), module );
		case CSwordModuleInfo::Lexicon:
			return new CSwordLDKey( (sword::SWKey *)(*module->module()), module);
		case CSwordModuleInfo::GenericBook:
			return new CSwordTreeKey( (sword::TreeKeyIdx*)((sword::SWKey *)(*module->module())), module );
		default:
			return 0;
	}
}
