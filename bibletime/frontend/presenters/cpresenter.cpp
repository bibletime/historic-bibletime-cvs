/***************************************************************************
                          cpresenter.cpp  -  description
                             -------------------
    begin                : Sun Mar 18 2001
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

#include "cpresenter.h"

CPresenter::CPresenter(QWidget *parent, const char *name, WFlags f)
	: KMainWindow(parent,name,f) {
		
	resize(350,350);
}

CPresenter::~CPresenter(){
}
