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

#include "bt_basicfilter.h"
#include "../frontend/optionsdialog/coptionsdialog.h"

BT_BASICFILTER::BT_BASICFILTER(){
}

char BT_BASICFILTER::ProcessText (char *text, int maxlen = -1){
// This updates the settings (colors etc.) which are used in ProcessText
// (HandleToken is reimplemented in the children)
	updateSettings();
	SWBasicFilter::ProcessText(text, maxlen);

}

void BT_BASICFILTER::updateSettings(void){
	footnote_color 		= COptionsDialog::getBTColor(COptionsDialog::footnote		).name();
	strongs_color  		= COptionsDialog::getBTColor(COptionsDialog::strongs		).name();
	morph_color    		= COptionsDialog::getBTColor(COptionsDialog::morph			).name();
	jesuswords_color 	= COptionsDialog::getBTColor(COptionsDialog::jesuswords	).name();
}
