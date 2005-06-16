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
#include "cswordbiblemoduleinfo.h"


namespace Rendering {

const QString CChapterDisplay::text( const ListCSwordModuleInfo& modules, const QString& keyName, const CSwordBackend::DisplayOptions displayOptions, const CSwordBackend::FilterOptions filterOptions ) {  
  Q_ASSERT( modules.count() >= 1 );
	Q_ASSERT( !keyName.isEmpty() );
	
  CSwordModuleInfo* module = modules.first();
	if (modules.count() == 1) {
		module->module()->setSkipConsecutiveLinks( true ); //skip empty, linked verses
	}
  
// 	bool ok = true;
	
/*	CSwordVerseKey key(modules.count() == 1 ? module : 0);
	key.Headings(1);
  key.key( keyName );*/
	
/*	const int currentTestament = key.Testament();
	const int currentBook = key.Book();
	const int currentChapter = key.Chapter();
*/
// 	CTextRendering::KeyTree tree;
 	CTextRendering::KeyTreeItem::Settings settings;
	settings.keyRenderingFace =
		displayOptions.verseNumbers
 			? CTextRendering::KeyTreeItem::Settings::SimpleKey
 			: CTextRendering::KeyTreeItem::Settings::NoKey;

	QString startKey = keyName;
	QString endKey = startKey;
	
	//check whether there's an intro we have to include
 	if (module->type() == CSwordModuleInfo::Bible) {
 		((VerseKey*)(module->module()->getKey()))->Headings(1); //HACK: enable headings for VerseKeys

		CSwordBibleModuleInfo* bible = dynamic_cast<CSwordBibleModuleInfo*>(module);
		Q_ASSERT(bible);
 			
		CSwordVerseKey k1(module); 
		k1.Headings(true);
		k1.key(keyName);
		k1.Verse(0);
		
 		QString raw( k1.rawText() );
 		//we need to work around a bug: osis2mod also puts Preverse titles into the chapter intro (verse 0)
		if ((raw.length() == 0) || ((raw.find("<title ") == 0) && (raw.find("</title>")+8 == int(raw.length())) )) {
			k1.Verse(1);
		}
		
		startKey = k1.key();
		
		k1.Verse(bible->verseCount(k1.book(), k1.Chapter()));
		endKey = k1.key();
 	}
 		
 	CDisplayRendering render(displayOptions, filterOptions);
	return render.renderKeyRange( startKey, endKey, modules, keyName, settings );

// 	CTextRendering::KeyTree* rootItem = &tree;
// 	Q_ASSERT(rootItem);
// 	
// 	if (modules.count() == 1) {
// 		CTextRendering::KeyTreeItem::Settings settings;
// 		
// 		CTextRendering::KeyTreeItem* headingItem = new CTextRendering::KeyTreeItem("Test", settings);
// 		rootItem = headingItem->childList();
// 	}
// 
// 	Q_ASSERT(rootItem);
// 	
// 	key = keyName;
// 	for (key.Verse(startVerse); 
// 			     (key.Testament() == currentTestament)
// 				&& (key.Book() == currentBook)
// 				&& (key.Chapter() == currentChapter)
// 				&& ok
// 				&& !module->module()->Error();	
// 			 ok = (key.next(CSwordVerseKey::UseVerse) && !key.Error()) )  //error 1 means not sucessful
// 	{
// 		settings.highlight = (key.key() == keyName);
// 		settings.keyRenderingFace = 
// 			displayOptions.verseNumbers 
// 			? CTextRendering::KeyTreeItem::Settings::SimpleKey
// 			: CTextRendering::KeyTreeItem::Settings::NoKey;
// 			
// 		rootItem->append( new CTextRendering::KeyTreeItem( key.key(), modules, settings ) );
// 	}
// 
// 	CDisplayRendering render(displayOptions, filterOptions);
// 	return render.renderKeyTree( tree );
}

};
