//
// C++ Implementation: cchapterdisplay
//
// Description: 
//
//
// Author: The BibleTime team <info@bibletime.info>, (C) 2004
//
// Copyright: See COPYING file that comes with this distribution
//
//

//Backend
#include "cchapterdisplay.h"
#include "cdisplayrendering.h"
#include "cswordversekey.h"


namespace Rendering {

const QString CChapterDisplay::text( const ListCSwordModuleInfo& modules, const QString& keyName, const CSwordBackend::DisplayOptions displayOptions, const CSwordBackend::FilterOptions filterOptions ) {  
  CSwordVerseKey key(0);
  key = keyName;
	
	const int currentTestament = key.Testament();
	const int currentBook = key.Book();
	const int currentChapter = key.Chapter();

  CSwordModuleInfo* module = modules.first();
  bool ok = true;
	
	if (modules.count() == 1) {
		key.module(module);
		module->module()->setSkipConsecutiveLinks( true ); //skip empty, linked verses
	}

	CTextRendering::KeyTree tree;
	CTextRendering::KeyTreeItem::Settings settings;
	

	for (	key.Verse(1); 
			  (key.Testament() == currentTestament)
				&& (key.Book() == currentBook)
				&& (key.Chapter() == currentChapter)
				&& ok 
				&& !module->module()->Error();
				
			  ok = (key.next(CSwordVerseKey::UseVerse) && !key.Error()) )  //error 1 means not sucessful
	{
		settings.highlight = (key.key() == keyName);
		settings.keyRenderingFace = 
			displayOptions.verseNumbers 
			? CTextRendering::KeyTreeItem::Settings::SimpleKey
			: CTextRendering::KeyTreeItem::Settings::NoKey;
			
		tree.append( new CTextRendering::KeyTreeItem( key.key(), modules, settings ) );
	}

	CDisplayRendering render(displayOptions, filterOptions);
	return render.renderKeyTree( tree );
}

};
