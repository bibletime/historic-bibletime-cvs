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
#include <treekey.h>

/** BibleTime's implementation of Sword's TreeKeyIdx class.
  * @author The BibleTime team
  */
class CSwordTreeKey : public TreeKey, public CSwordKey  {
public:
//	CSwordTreeKey( CSwordModuleInfo* module );

	CSwordTreeKey( TreeKey *k, CSwordModuleInfo* module );

	~CSwordTreeKey();
};

#endif
