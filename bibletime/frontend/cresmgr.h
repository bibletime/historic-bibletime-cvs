/***************************************************************************
                          chelpmgr.h  -  description
                             -------------------
    begin                : Mon Sep 16 2002
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

#ifndef CRESMGR_H
#define CRESMGR_H

//Qt includes
#include <qstring.h>
#include <qkeysequence.h>

//KDE includes
#include <kshortcut.h>

/** Provides static functions to easily access the Tooltip and WhatsThis texts for all the frontend parts.
  * @author The BibleTime team
  */  
namespace CResMgr {
//public:
  namespace modules {
    namespace bible {
      extern const QString icon_unlocked;
      extern const QString icon_locked;
      extern const QString icon_add;
    };
    namespace commentary {
      extern const QString icon_unlocked;
      extern const QString icon_locked;
      extern const QString icon_add;
    };
    namespace lexicon {
      extern const QString icon_unlocked;
      extern const QString icon_locked;
      extern const QString icon_add;
    };
    namespace book {
      extern const QString icon_unlocked;
      extern const QString icon_locked;
      extern const QString icon_add;
    };
  };  
  
  namespace mainMenu { //Main menu
    namespace file { //Main menu->File
      namespace clearQueue {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace print { //a standard action
        extern const QString tooltip;
        extern const QString whatsthis;
      }      
      namespace quit { //a standard action
        extern const QString tooltip;
        extern const QString whatsthis;
      }
    }
    
    namespace view { //Main menu->View
      namespace showMainIndex {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace showToolBar { //a standard action
        extern const QString tooltip;
        extern const QString whatsthis;
      }
    }

    namespace mainIndex { //configuration for the main index and the view->search menu
      namespace search {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace searchdefaultbible {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
    };

    namespace window { //Main menu->Window
      namespace loadProfile {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const char* actionName;
      }
      namespace saveProfile {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const char* actionName;
      }
      namespace saveToNewProfile {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace setupProfiles {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace showFullscreen {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace tile {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace cascade {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace autoTile {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace autoCascade {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace closeAll {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
    }

    namespace settings { //Main menu->Settings
      namespace editToolBar { // available as KStdAction
        extern const QString tooltip;
        extern const QString whatsthis;
      }
      namespace optionsDialog { // available as KStdAction
        extern const QString tooltip;
        extern const QString whatsthis;
      };
    };

    namespace help { //Main menu->Help
      namespace handbook {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      };
      namespace bibleStudyHowTo {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      };
      namespace whatsThis { // available as KStdAction
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace bugreport { // available as KStdAction
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace dailyTip {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      };
      namespace aboutBibleTime { // available as KStdAction
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace aboutKDE { // available as KStdAction
        extern const QString tooltip;
        extern const QString whatsthis;
      };      
    };
  }  //end of main menu

  namespace searchdialog {
    extern const QString icon;

    namespace searchButton {
      extern const QString tooltip;
      extern const QString whatsthis;
    };
    namespace cancelSearchButton {
      extern const QString tooltip;
      extern const QString whatsthis;
    };
    
    namespace options {
//      namespace moduleChooserDialog {
//        extern const QString tooltip;
//        extern const QString whatsthis;
//      };
      namespace moduleChooserButton {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace searchedText {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace searchType {
        namespace multipleWords_and {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace multipleWords_or {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace exactMatch {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace regExp {
          extern const QString tooltip;
          extern const QString whatsthis;
        };        
      };
      
      namespace searchOptions {
        namespace caseSensitive {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
      };
      namespace chooseScope {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace scopeEditor {
        namespace rangeList {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace nameEdit {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace editRange {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace parsedResult {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace addNewRange {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace deleteCurrentRange {
          extern const QString tooltip;
          extern const QString whatsthis;
        };

      };
    }
    namespace result {
      namespace moduleList {
        extern const QString tooltip;
        extern const QString whatsthis;

        namespace copyMenu {
          extern const QString icon;
        };
        namespace saveMenu {
          extern const QString icon;
        };
        namespace printMenu {
          extern const QString icon;
        };
      };
      namespace foundItems {
        extern const QString tooltip;
        extern const QString whatsthis;

        namespace copyMenu {
          extern const QString icon;
        };
        namespace saveMenu {
          extern const QString icon;
        };
        namespace printMenu {
          extern const QString icon;
        };
        
      };
      namespace textPreview {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
    };    
  };

  namespace workspace {
  };

  namespace displaywindows {
    namespace transliteration {
      extern const QString icon;
    };
    namespace displaySettings {
      extern const QString icon;
    };

    namespace general {
      namespace scrollButton{
        extern const QString tooltip;
        extern const QString whatsthis;
      };
    };
    
    namespace bibleWindow {
      namespace bookList {
        extern const QString tooltip;
        extern const QString whatsthis;
      }
      namespace nextBook {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const KShortcut accel;
      }
      namespace previousBook {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const KShortcut accel;
      }

      namespace chapterList {
        extern const QString tooltip;
        extern const QString whatsthis;
      }
      namespace nextChapter {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const KShortcut accel;
      }
      namespace previousChapter {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const KShortcut accel;
      }

      namespace verseList {
        extern const QString tooltip;
        extern const QString whatsthis;
      }
      namespace nextVerse {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const KShortcut accel;
      }
      namespace previousVerse {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const KShortcut accel;
      }

      namespace copyMenu {
        extern const QString icon;
      };
      namespace saveMenu {
        extern const QString icon;
      }
      namespace printMenu {
        extern const QString icon;
      }
    };
    namespace commentaryWindow {
      namespace syncWindow {
        extern const QString icon;
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const KShortcut accel;
      }
      
    };

    namespace lexiconWindow {
      namespace entryList {
        extern const QString tooltip;
        extern const QString whatsthis;
      }
      namespace nextEntry {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const KShortcut accel;
      }
      namespace previousEntry {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const KShortcut accel;
      }

      namespace copyMenu {
        extern const QString icon;
      }
      namespace saveMenu {
        extern const QString icon;
      }
      namespace printMenu {
        extern const QString icon;
      }      
    };
    namespace bookWindow {
      namespace toggleTree {
        extern const QString icon;
        extern const KShortcut accel;
      };
    };

    
    namespace writeWindow {
      namespace saveText {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
      };
      namespace restoreText {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
      };
      namespace deleteEntry {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
      };

    //formatting buttons
      namespace boldText {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
      };
      namespace italicText {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
      };
      namespace underlinedText {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
      };

      namespace alignLeft {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
      };
      namespace alignCenter {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
      };
      namespace alignRight {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
      };
      namespace alignJustify {
        extern const QString tooltip;
        extern const QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
      };


      namespace fontFamily {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace fontSize {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace fontColor {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
            
    };
  };
  
   namespace printing {
    namespace itemPage {
      namespace styleChooser {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace itemList {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace itemUp {
        extern const QString icon;
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace itemDown {
        extern const QString icon;
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace itemDelete {
        extern const QString icon;
        extern const QString tooltip;
        extern const QString whatsthis;
      };
    };

    namespace styleList {
      namespace newStyle {
        extern const QString icon;
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace deleteStyle {
        extern const QString icon;
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace editStyle {
        extern const QString icon;
        extern const QString tooltip;
        extern const QString whatsthis;
      };
    };
    namespace printerSettings {
      namespace margins {
        extern const QString whatsthis;

        namespace upper {
          extern const QString tooltip;
          extern const QString whatsthis;          
        };
        namespace lower {
          extern const QString tooltip;
          extern const QString whatsthis;          
        };
        namespace left {
          extern const QString tooltip;
          extern const QString whatsthis;          
        };
        namespace right {
          extern const QString tooltip;
          extern const QString whatsthis;          
        };
      };
    };
  };

  namespace settings {
    namespace startup {
      extern const QString icon;
      
      namespace dailyTip {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace showLogo {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace restoreWorkingArea {
        extern const QString tooltip;
        extern const QString whatsthis;
      };      
    };
    namespace fonts {
      extern const QString icon;

      namespace typeChooser {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
    };
    namespace colors {
      extern const QString icon;

      namespace text {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace background {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace links {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace footnotes {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace strongNumbers {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace highlightedVerse {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace jesusWords {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace morphTags {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
    };    
    namespace profiles {
      extern const QString icon;
      
      namespace list {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace createNew {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace deleteCurrent {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
      namespace renameCurrent {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
    };
    namespace sword {
      extern const QString icon;

      namespace general {
        namespace cacheKeys {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace scrolling {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace language {
          extern const QString tooltip;
          extern const QString whatsthis;
        };        
      };
      namespace modules {
        namespace bible {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace commentary {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace lexicon {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace dailyDevotional {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace hebrewStrongs {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace greekStrongs {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace hebrewMorph {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace greekMorph {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
      };
      namespace filters {
        namespace lineBreaks {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace verseNumbers {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace footnotes {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace strongsNumbers {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace morphTags {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace headings {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace lemmas {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace hebrewVowelPoints {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace hebrewCantillation {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace greekAccents {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
        namespace textualVariants {
          extern const QString tooltip;
          extern const QString whatsthis;
        };
      };
    };
    namespace keys {
      extern const QString icon;
    };    
  };

  namespace mainIndex { //configuration for the main index and the view->search menu
    namespace search {
      extern const QString tooltip;
      extern const QString whatsthis;
      extern const QString icon;
      extern const KShortcut accel;
      extern const char* actionName;
    }
    namespace newFolder {
      extern const QString icon;
    }
    namespace changeFolder {
      extern const QString icon;
    }
    namespace openedFolder {
      extern const QString icon;
    }
    namespace closedFolder {
      extern const QString icon;
    }    

    namespace bookmark {
      extern const QString icon;
    }
    namespace changeBookmark {
      extern const QString icon;
    }
    namespace importBookmarks {
      extern const QString icon;
    }
    namespace exportBookmarks {
      extern const QString icon;
    }
    namespace printBookmarks {
      extern const QString icon;
    }
    namespace deleteItems {
      extern const QString icon;
    }

    namespace editModuleMenu {
      extern const QString icon;
    }
    namespace editModulePlain {
      extern const QString icon;
    }
    namespace editModuleHTML {
      extern const QString icon;
    }

    namespace unlockModule {
      extern const QString icon;
    }
    namespace aboutModule {
      extern const QString icon;
    }    
  };
  
  namespace helpDialog {
    extern const QString moduleLocked;
    extern const QString firstStart;
    extern const QString noSwordModules;    
    extern const QString noSwordModuleConfigDir;
    extern const QString initBackendFailed;
    extern const QString firstTimeSearchDialog;
  };
}

#endif
