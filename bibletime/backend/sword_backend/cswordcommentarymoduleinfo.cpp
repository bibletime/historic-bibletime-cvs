/***************************************************************************
                          cswordcommentarymoduleinfo.cpp  -  description
                             -------------------
    begin                : Thu Oct 5 2000
    copyright            : (C) 2000 by The BibleTime team
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

#include "cswordcommentarymoduleinfo.h"

CSwordCommentaryModuleInfo::CSwordCommentaryModuleInfo( CSwordBackend* backend, SWModule* module)
	:CSwordBibleModuleInfo(backend, module) {
	
}

CSwordCommentaryModuleInfo::~CSwordCommentaryModuleInfo(){

}

/** No descriptions */
CSwordModuleInfo::type CSwordCommentaryModuleInfo::getType(){
	return CSwordModuleInfo::Commentary;
}
