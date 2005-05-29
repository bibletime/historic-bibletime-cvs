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
  Q_ASSERT( modules.count() >= 1 );
	Q_ASSERT( !keyName.isEmpty() );
	
  CSwordModuleInfo* module = modules.first();
	if (modules.count() == 1) {
		module->module()->setSkipConsecutiveLinks( true ); //skip empty, linked verses
	}
  
	bool ok = true;
	
	CSwordVerseKey key(modules.count() == 1 ? module : 0);
	key.Headings(1);	
  key.key( keyName );
	
	const int currentTestament = key.Testament();
	const int currentBook = key.Book();
	const int currentChapter = key.Chapter();

	CTextRendering::KeyTree tree;
	CTextRendering::KeyTreeItem::Settings settings;

	int startVerse = 0;
	
	//check whether there's an intro we have to include
 	if (module->type() == CSwordModuleInfo::Bible) {
 		((VerseKey*)(module->module()->getKey()))->Headings(1); //enable headings for VerseKeys
 		
		CSwordVerseKey k1(module); 
		k1.Headings(true);
		k1.key(key.key());
		k1.Verse(0);
		
 		QString raw( k1.rawText() );
 		//we need to work around a bug: osis2mod also puts Preverse titles into the chapter intro (verse 0)
		if (((raw.find("<title") == 0) && (raw.find("</title>")+8 == int(raw.length())) ) || (raw.length() == 0)) {
			startVerse = 1;
		}
 	}
	
	key = keyName;
	for (key.Verse(startVerse); 
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
