/***************************************************************************
                          cswordtreekeyidx.h  -  description
                             -------------------
    begin                : Thu Jan 24 2002
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

#ifndef CSWORDTREEKEYIDX_H
#define CSWORDTREEKEYIDX_H

//BibleTime includes
#include "cswordkey.h"
#include "cswordmoduleinfo.h"

//Sword includes
#include <treekeyidx.h>

/** BibleTime's implementation of Sword's TreeKeyIdx class.
  * @author The BibleTime team
  */
class CSwordTreeKey : public TreeKeyIdx, public CSwordKey  {
public:
	CSwordTreeKey( const CSwordTreeKey& k );
	CSwordTreeKey( const TreeKeyIdx *k, CSwordModuleInfo* module );
	CSwordModuleInfo* module( CSwordModuleInfo* newModule );
	virtual CSwordTreeKey* copy() const;
  /**
  * Set/get the key. If the parameter is not set (means equal to QString::null)
  * the used key is returned. Otherwise the key is set and the new on ei returned.
  */
  virtual const QString key( const QString& key = QString::null );
  /**
  * Set/get the key. If the parameter is not set (means equal to QString::null)
  * the used key is returned. Otherwise the key is set and the new on ei returned.
  */
	virtual void key( const char* key );
};

#endif
