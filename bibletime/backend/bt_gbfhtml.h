/***************************************************************************
                          gbfhtml.h  -  description
                             -------------------
    begin                : Tue Jun 15 15:34:26 CEST 1999

    copyright            : (C) 1999 by Torsten Uhlmann
    email                : TUhlmann@gmx.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/* $Header$ */
/* $Revision$ */

#ifndef GBFHTML_H
#define GBFHTML_H

#include "bt_basicfilter.h"

class sword::SWKey;
class sword::SWModule;

using namespace sword;

/**
* This filter converts GBF Text into HTML
*/
class BT_GBFHTML : public BT_BASICFILTER{

public:
  BT_GBFHTML ();
  virtual bool handleToken(char **buf, const char *token, DualStringMap &userData);
  /** No descriptions */
  virtual char ProcessText(char * text, int maxlen, const SWKey *, const SWModule * = 0);

protected: // Protected methods
  /**
  * Update the tokens which use changeable setttings.
  */
  virtual void updateTokens();
};

#endif
