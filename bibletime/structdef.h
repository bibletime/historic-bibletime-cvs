/***************************************************************************
                     structdef.h -- some important struct definitions
                             -------------------                                         

    begin                : 20 Jul 1998                                        
    copyright            : (C) 1998 by The Team of BibleTime
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

#ifndef STRUCTDEF_H
#define STRUCTDEF_H

#include <qlist.h>
#include <qstring.h>
#include <qfont.h>
#include <qguardedptr.h>

class QListViewItem;
class CModuleInfo;
class CPrintItem;
class CStyle;
class CSwordBackend;
class CPrinter;

typedef QList<CPrintItem> printItemList;
typedef QList<CStyle> styleList;

struct CImportantClasses {
	CSwordBackend* swordBackend;
 	CPrinter* printer;
};

/** This is used to show the selection in the searchscope dialog
 	*/
enum SearchScope 		{ NoScope, LastSearch, Bounds	};

/** This is used to so show the selection of the searchdialog
 	*/
enum SearchType 		{ MultiWord, ExactPhrase, RegEx	};

#endif


