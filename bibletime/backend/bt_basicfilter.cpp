/***************************************************************************
                          bt_basicfilter.cpp  -  description
                             -------------------
    begin                : Thu Nov 1 2001
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
#include <stdlib.h>

#include "bt_basicfilter.h"
#include "../frontend/cbtconfig.h"

BT_BASICFILTER::BT_BASICFILTER(){
}

char BT_BASICFILTER::ProcessText (char *text, int maxlen = -1){
	updateSettings();
	SWBasicFilter::ProcessText(text, maxlen);
}

void BT_BASICFILTER::updateSettings(void){
	strcpy(footnote_color,	CBTConfig::get(CBTConfig::footnotesColor	).name().utf8());
	strcpy(strongs_color,		CBTConfig::get(CBTConfig::strongsColor		).name().utf8());
	strcpy(morph_color,			CBTConfig::get(CBTConfig::morphsColor			).name().utf8());
	strcpy(jesuswords_color,CBTConfig::get(CBTConfig::jesuswordsColor	).name().utf8());
}
