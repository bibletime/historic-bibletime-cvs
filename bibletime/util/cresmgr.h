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
  void init_i18n();
  
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
      namespace print { //a standard action
        extern QString tooltip;
        extern QString whatsthis;
      }
      namespace quit { //a standard action
        extern QString tooltip;
        extern QString whatsthis;
      }
    }

    namespace view { //Main menu->View
      namespace showMainIndex {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace showInfoDisplay {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace showToolBar { //a standard action
        extern QString tooltip;
        extern QString whatsthis;
      }
    }

    namespace mainIndex { //configuration for the main index and the view->search menu
      namespace search {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace searchdefaultbible {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
    };

    namespace window { //Main menu->Window
      namespace loadProfile {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const char* actionName;
      }
      namespace saveProfile {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const char* actionName;
      }
      namespace saveToNewProfile {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace setupProfiles {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace showFullscreen {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
			namespace arrangementMode {
				extern QString tooltip;
				extern QString whatsthis;
				extern const QString icon;
				extern const KShortcut accel;
				extern const char* actionName;
				
				namespace manual {
					extern QString tooltip;
					extern QString whatsthis;
					extern const QString icon;
					extern const KShortcut accel;
					extern const char* actionName;
				}
				namespace autoTileVertical {
					extern QString tooltip;
					extern QString whatsthis;
					extern const QString icon;
					extern const KShortcut accel;
					extern const char* actionName;
				}
				namespace autoTileHorizontal {
					extern QString tooltip;
					extern QString whatsthis;
					extern const QString icon;
					extern const KShortcut accel;
					extern const char* actionName;
				}
				namespace autoCascade {
					extern QString tooltip;
					extern QString whatsthis;
					extern const QString icon;
					extern const KShortcut accel;
					extern const char* actionName;
				}			
			}
      namespace tile {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace cascade {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
      namespace closeAll {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
    }

    namespace settings { //Main menu->Settings
      namespace editToolBar { // available as KStdAction
        extern QString tooltip;
        extern QString whatsthis;
      }
      namespace optionsDialog { // available as KStdAction
        extern QString tooltip;
        extern QString whatsthis;
      };
      namespace swordSetupDialog {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      }
    };

    namespace help { //Main menu->Help
      namespace handbook {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      };
      namespace bibleStudyHowTo {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      };
      namespace whatsThis { // available as KStdAction
        extern QString tooltip;
        extern QString whatsthis;
      };
      namespace bugreport { // available as KStdAction
        extern QString tooltip;
        extern QString whatsthis;
      };
      namespace dailyTip {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
        extern const char* actionName;
      };
      namespace aboutBibleTime { // available as KStdAction
        extern QString tooltip;
        extern QString whatsthis;
      };
      namespace aboutKDE { // available as KStdAction
        extern QString tooltip;
        extern QString whatsthis;
      };
    };
  }  //end of main menu

  namespace searchdialog {
    extern const QString icon;

    namespace searchButton {
      extern QString tooltip;
      extern QString whatsthis;
    };
    namespace cancelSearchButton {
      extern QString tooltip;
      extern QString whatsthis;
    };

    namespace options {
//      namespace moduleChooserDialog {
//        extern QString tooltip;
//        extern QString whatsthis;
//      };
      namespace moduleChooserButton {
        extern QString tooltip;
        extern QString whatsthis;
      };
      namespace searchedText {
        extern QString tooltip;
        extern QString whatsthis;
      };
      namespace searchType {
        namespace multipleWords_and {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace multipleWords_or {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace exactMatch {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace regExp {
          extern QString tooltip;
          extern QString whatsthis;
        };
      };

      namespace searchOptions {
        namespace caseSensitive {
          extern QString tooltip;
          extern QString whatsthis;
        };
      };
      namespace chooseScope {
        extern QString tooltip;
        extern QString whatsthis;
      };
      namespace scopeEditor {
        namespace rangeList {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace nameEdit {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace editRange {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace parsedResult {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace addNewRange {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace deleteCurrentRange {
          extern QString tooltip;
          extern QString whatsthis;
        };

      };
    }
    namespace result {
      namespace moduleList {
        extern QString tooltip;
        extern QString whatsthis;

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
        extern QString tooltip;
        extern QString whatsthis;

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
        extern QString tooltip;
        extern QString whatsthis;
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
        extern QString tooltip;
        extern QString whatsthis;
      };
			namespace search {
				extern QString tooltip;
				extern QString whatsthis;
				extern const QString icon;
				extern const KShortcut accel;
				extern const char* actionName;
			}			
    };

    namespace bibleWindow {
      namespace bookList {
        extern QString tooltip;
        extern QString whatsthis;
      }
      namespace nextBook {
        extern QString tooltip;
        extern QString whatsthis;
        extern const KShortcut accel;
      }
      namespace previousBook {
        extern QString tooltip;
        extern QString whatsthis;
        extern const KShortcut accel;
      }

      namespace chapterList {
        extern QString tooltip;
        extern QString whatsthis;
      }
      namespace nextChapter {
        extern QString tooltip;
        extern QString whatsthis;
        extern const KShortcut accel;
      }
      namespace previousChapter {
        extern QString tooltip;
        extern QString whatsthis;
        extern const KShortcut accel;
      }

      namespace verseList {
        extern QString tooltip;
        extern QString whatsthis;
      }
      namespace nextVerse {
        extern QString tooltip;
        extern QString whatsthis;
        extern const KShortcut accel;
      }
      namespace previousVerse {
        extern QString tooltip;
        extern QString whatsthis;
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
        extern QString tooltip;
        extern QString whatsthis;
        extern const KShortcut accel;
				extern const char* actionName;
      }

    };

    namespace lexiconWindow {
      namespace entryList {
        extern QString tooltip;
        extern QString whatsthis;
      }
      namespace nextEntry {
        extern QString tooltip;
        extern QString whatsthis;
        extern const KShortcut accel;
      }
      namespace previousEntry {
        extern QString tooltip;
        extern QString whatsthis;
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
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
				extern const char* actionName;
      };
      namespace restoreText {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
				extern const char* actionName;
      };
      namespace deleteEntry {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
				extern const char* actionName;
      };

    //formatting buttons
      namespace boldText {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
				extern const char* actionName;
      };
      namespace italicText {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
				extern const char* actionName;
      };
      namespace underlinedText {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
				extern const char* actionName;
      };

      namespace alignLeft {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
				extern const char* actionName;
      };
      namespace alignCenter {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
				extern const char* actionName;
      };
      namespace alignRight {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
				extern const char* actionName;
      };
      namespace alignJustify {
        extern QString tooltip;
        extern QString whatsthis;
        extern const QString icon;
        extern const KShortcut accel;
				extern const char* actionName;
      };

      namespace fontFamily {
        extern QString tooltip;
        extern QString whatsthis;
	      extern const KShortcut accel;
				extern const char* actionName;
      };
      namespace fontSize {
        extern QString tooltip;
        extern QString whatsthis;
	      extern const KShortcut accel;
				extern const char* actionName;
      };
      namespace fontColor {
        extern QString tooltip;
        extern QString whatsthis;
      };

    };
  };

  namespace settings {
    namespace startup {
      extern const QString icon;

      namespace dailyTip {
        extern QString tooltip;
        extern QString whatsthis;
      };
      namespace showLogo {
        extern QString tooltip;
        extern QString whatsthis;
      };
      namespace restoreWorkingArea {
        extern QString tooltip;
        extern QString whatsthis;
      };
    };
    namespace fonts {
      extern const QString icon;

      namespace typeChooser {
        extern QString tooltip;
        extern QString whatsthis;
      };
    };
    namespace colors {
      extern const QString icon;
    };
    namespace profiles {
      extern const QString icon;

      namespace list {
        extern QString tooltip;
        extern QString whatsthis;
      };
      namespace createNew {
        extern QString tooltip;
        extern QString whatsthis;
      };
      namespace deleteCurrent {
        extern QString tooltip;
        extern QString whatsthis;
      };
      namespace renameCurrent {
        extern QString tooltip;
        extern QString whatsthis;
      };
    };
    namespace sword {
      extern const QString icon;

      namespace general {
        namespace cacheKeys {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace scrolling {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace language {
          extern QString tooltip;
          extern QString whatsthis;
        };
      };
      namespace modules {
        namespace bible {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace commentary {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace lexicon {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace dailyDevotional {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace hebrewStrongs {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace greekStrongs {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace hebrewMorph {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace greekMorph {
          extern QString tooltip;
          extern QString whatsthis;
        };
      };
      namespace filters {
        namespace lineBreaks {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace verseNumbers {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace footnotes {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace strongsNumbers {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace morphTags {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace headings {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace lemmas {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace hebrewVowelPoints {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace hebrewCantillation {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace greekAccents {
          extern QString tooltip;
          extern QString whatsthis;
        };
        namespace textualVariants {
          extern QString tooltip;
          extern QString whatsthis;
        };
      };
    };
    namespace keys {
      extern const QString icon;
    };
  };

  namespace mainIndex { //configuration for the main index and the view->search menu
    namespace search {
      extern QString tooltip;
      extern QString whatsthis;
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
