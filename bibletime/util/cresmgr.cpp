/***************************************************************************
                          cresmgr.cpp  -  description
                             -------------------
    begin                : Tue Jan 25 2000
    copyright            : (C) 2000 by The BibleTime Team
    email                : Info@bibletime.de
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

//own includes
#include "cresmgr.h"

#include "backend/cswordbackend.h"

//KDE includes
#include <kglobal.h>
#include <klocale.h>

inline const QString makeToolTip( const QString& text ) {
  return text;
};

inline const QString makeWhatsThis( const QString& title, const QString& description )  {
  return QString::fromLatin1("<center><B>%1</B></center><HR>%2").arg(title).arg(description);
};



namespace CResMgr {
  namespace modules {
    namespace bible {
      const QString icon_unlocked  = "bt_bible";
      const QString icon_locked    = "bt_bible_locked";
      const QString icon_add       = "bt_bible_add";
    };
    namespace commentary {
      const QString icon_unlocked  = "bt_commentary";
      const QString icon_locked    = "bt_commentary_locked";
      const QString icon_add       = "bt_commentary_add";
    };
    namespace lexicon {
      const QString icon_unlocked  = "bt_lexicon";
      const QString icon_locked    = "bt_lexicon_locked";
      const QString icon_add       = "bt_lexicon_add";
    };
    namespace book {
      const QString icon_unlocked  = "bt_book";
      const QString icon_locked    = "bt_book_locked";
      const QString icon_add       = "bt_book_add";
    };
  };

  namespace mainMenu { //Main menu
    namespace file { //Main menu->File
      namespace print { //a standard action
        QString tooltip;
        QString whatsthis;
      }
      namespace quit { //a standard action
        QString tooltip;
        QString whatsthis;
      }
    }

    namespace view { //Main menu->View
      namespace showMainIndex {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "view_sidetree";
        const KShortcut accel     = Qt::Key_F9;
        const char* actionName    = "viewMainIndex_action";
      }
      namespace showInfoDisplay {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "view_sidetree";
        const KShortcut accel     = Qt::Key_F8;
        const char* actionName    = "viewInfoDisplay_action";
      }
      namespace showToolBar { //a standard action
        QString tooltip;
        QString whatsthis;
      }
    }

    namespace mainIndex { //Main menu->Settings
      namespace search {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "find";
        const KShortcut accel     = Qt::CTRL + Qt::Key_G;
        const char* actionName    = "groupmanager_search_action";
      }
      namespace searchdefaultbible {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "find";
        const KShortcut accel     = Qt::CTRL + Qt::Key_F;
        const char* actionName    = "groupmanager_searchdefaultbible_action";
      }
    };

    namespace window { //Main menu->Window
      namespace loadProfile {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "view_sidetree";
        const char* actionName    = "windowLoadProfile_action";
      }
      namespace saveProfile {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "view_sidetree";
        const char* actionName    = "windowSaveProfile_action";
      }
      namespace saveToNewProfile {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "view_sidetree";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_S;
        const char* actionName    = "windowSaveToNewProfile_action";
      }
      namespace setupProfiles {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "view_sidetree";
        const KShortcut accel     = Qt::Key_F5;
        const char* actionName    = "windowEditProfiles_action";
      }
      namespace showFullscreen {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "window_fullscreen";
        const KShortcut accel     = Qt::CTRL + Qt::SHIFT + Qt::Key_F;
        const char* actionName    = "windowFullscreen_action";
      }
      namespace arrangementMode {
				QString tooltip;
				QString whatsthis;
				const QString icon        = "bt_tile";
				const KShortcut accel     = Qt::CTRL + Qt::Key_A;
				const char* actionName    = "windowArrangementMode_action";
				
				namespace manual {
					QString tooltip;
					QString whatsthis;
					const QString icon        = "bt_tile_manual";
					const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_M;
					const char* actionName    = "windowArrangementManual_action";
				}
				namespace autoTileHorizontal {
					QString tooltip;
					QString whatsthis;
					const QString icon        = "bt_tile_auto";
					const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_T;
					const char* actionName    = "windowAutoTileHorizontal_action";
				}
				namespace autoTileVertical {
					QString tooltip;
					QString whatsthis;
					const QString icon        = "bt_tile_auto";
					const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_T;
					const char* actionName    = "windowAutoTileVertical_action";
				}
				namespace autoCascade {
					QString tooltip;
					QString whatsthis;
					const QString icon        = "bt_cascade_auto";
					const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_A;
					const char* actionName    = "windowAutoCascade_action";
				}
      }
      namespace tile {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "bt_tile";
        const KShortcut accel     = Qt::CTRL + Qt::Key_T;
        const char* actionName    = "windowTile_action";
      }
      namespace cascade {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "bt_cascade";
        const KShortcut accel     = Qt::CTRL + Qt::Key_A;
        const char* actionName    = "windowCascade_action";
      }
      namespace closeAll {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "fileclose";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_W;
        const char* actionName    = "windowCloseAll_action";
      }
    }

    namespace settings { //Main menu->Settings
      namespace editToolBar { // available as KStdAction
        QString tooltip;
        QString whatsthis;
      }
      namespace optionsDialog { // available as KStdAction
        QString tooltip;
        QString whatsthis;
      };
      namespace swordSetupDialog {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "bt_swordconfig";
        const KShortcut accel     = Qt::Key_F4;
        const char* actionName    = "options_sword_setup";
      }

    };

    namespace help { //Main menu->Help
      namespace handbook {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "contents";
        const KShortcut accel     = Qt::Key_F1;
        const char* actionName    = "helpHandbook_action";
      };
      namespace bibleStudyHowTo {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "contents";
        const KShortcut accel     = Qt::Key_F2;
        const char* actionName    = "helpHowTo_action";
      };
      namespace whatsThis { // available as KStdAction
        QString tooltip;
        QString whatsthis;
      };
      namespace bugreport { // available as KStdAction
        QString tooltip;
        QString whatsthis;
      };
      namespace dailyTip {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "idea";
        const KShortcut accel     = Qt::Key_F3;
        const char* actionName    = "helpDailyTip_action";
      };
      namespace aboutBibleTime { // available as KStdAction
        QString tooltip;
        QString whatsthis;
      };
      namespace aboutKDE { // available as KStdAction
        QString tooltip;
        QString whatsthis;
      };
    };
  }  //end of main menu

  namespace searchdialog {
    const QString icon = "find";

    namespace searchButton {
      QString tooltip;
      QString whatsthis;
    };
    namespace cancelSearchButton {
      QString tooltip;
      QString whatsthis;
    };

    namespace options {
      namespace moduleChooserButton {
        QString tooltip;
        QString whatsthis;
      };
      namespace searchedText {
        QString tooltip;
        QString whatsthis;
      };
      namespace searchType {
        namespace multipleWords_and {
          QString tooltip;
          QString whatsthis;
        };
        namespace multipleWords_or {
          QString tooltip;
          QString whatsthis;
        };
        namespace exactMatch {
          QString tooltip;
          QString whatsthis;
        };
        namespace regExp {
          QString tooltip;
          QString whatsthis;
        };
      };

      namespace searchOptions {
        namespace caseSensitive {
          QString tooltip;
          QString whatsthis;
        };
      };
      namespace chooseScope {
        QString tooltip;
        QString whatsthis;
      };
      namespace scopeEditor {
        namespace rangeList {
          QString tooltip;
          QString whatsthis;
        };
        namespace nameEdit {
          QString tooltip;
          QString whatsthis;
        };
        namespace editRange {
          QString tooltip;
          QString whatsthis;
        };
        namespace parsedResult {
          QString tooltip;
          QString whatsthis;
        };
        namespace addNewRange {
          QString tooltip;
          QString whatsthis;
        };
        namespace deleteCurrentRange {
          QString tooltip;
          QString whatsthis;
        };

      };
    }
    namespace result {
      namespace moduleList {
        QString tooltip;
        QString whatsthis;

        namespace copyMenu {
          const QString icon = "editcopy";
        };
        namespace saveMenu {
          const QString icon = "filesave";
        };
        namespace printMenu {
          const QString icon = "fileprint";
        };
      };
      namespace foundItems {
        QString tooltip;
        QString whatsthis;

        namespace copyMenu {
          const QString icon = "editcopy";
        };
        namespace saveMenu {
          const QString icon = "filesave";
        };
        namespace printMenu {
          const QString icon = "fileprint";
        };
      };
      namespace textPreview {
        QString tooltip;
        QString whatsthis;
      };
    };
  };

  namespace workspace {
  };

  namespace displaywindows {
    namespace transliteration {
      const QString icon = "bt_displaytranslit";
    };
    namespace displaySettings {
      const QString icon = "bt_displayconfig";
    };

    namespace general {
      namespace scrollButton {
        QString tooltip;
        QString whatsthis;
      };
      
			namespace search {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "find";
        const KShortcut accel     = Qt::CTRL + Qt::Key_L;
        const char* actionName    = "window_search_action";
      }
    };
    namespace bibleWindow {
      namespace bookList {
        QString tooltip;
        QString whatsthis;
      };
      namespace nextBook {
        QString tooltip;
        QString whatsthis;
        const KShortcut accel = Qt::CTRL + Qt::Key_B;
       }
      namespace previousBook {
        QString tooltip;
        QString whatsthis;
        const KShortcut accel = Qt::CTRL + Qt::SHIFT + Qt::Key_B;
      }


      namespace chapterList {
        QString tooltip;
        QString whatsthis;
      };
      namespace nextChapter {
        QString tooltip;
        QString whatsthis;
        const KShortcut accel = Qt::CTRL + Qt::Key_H;
      }
      namespace previousChapter {
        QString tooltip;
        QString whatsthis;
        const KShortcut accel = Qt::CTRL + Qt::SHIFT + Qt::Key_H;
      }

      namespace verseList {
        QString tooltip;
        QString whatsthis;
      };
      namespace nextVerse {
        QString tooltip;
        QString whatsthis;
        const KShortcut accel = Qt::CTRL + Qt::Key_V;
      }
      namespace previousVerse {
        QString tooltip;
        QString whatsthis;
        const KShortcut accel = Qt::CTRL + Qt::SHIFT + Qt::Key_V;
      }

      namespace copyMenu {
        const QString icon = "editcopy";//CResMgr::displaywindows::lexiconWindow::copyMenu::icon;
      };
      namespace saveMenu {
        const QString icon = "filesave";//CResMgr::displaywindows::lexiconWindow::saveMenu::icon;
      }
      namespace printMenu {
        const QString icon = "fileprint";//CResMgr::displaywindows::lexiconWindow::printMenu::icon;
      }
    };
    namespace commentaryWindow {
      namespace syncWindow {
         QString tooltip;
         QString whatsthis;
         const QString icon 		= "bt_sync";
         const KShortcut accel 	= KKeySequence(); //CResMgr::displaywindows::biblewindow::nextBook::accel;
		 const char* actionName	= "commentary_syncWindow";
       }

    };
    namespace lexiconWindow {
      namespace entryList {
        QString tooltip;
        QString whatsthis;
      };
      namespace nextEntry {
        QString tooltip;
        QString whatsthis;
        const KShortcut accel = Qt::CTRL + Qt::Key_V;
      }
      namespace previousEntry {
        QString tooltip;
        QString whatsthis;
        const KShortcut accel = Qt::CTRL + Qt::SHIFT + Qt::Key_V;
      }

      namespace copyMenu {
        const QString icon = "editcopy";
      };
      namespace saveMenu {
        const QString icon = "filesave";
      }
      namespace printMenu {
        const QString icon = "fileprint";
      }
    };
    namespace bookWindow {
      namespace toggleTree {
        const QString icon = "view_sidetree";
        const KShortcut accel = KKeySequence();
      };
    };

    namespace writeWindow {
      namespace saveText {
        QString tooltip;
        QString whatsthis;
        const QString icon      = "filesave";
        const KShortcut accel   = KKeySequence();
				const char* actionName 	= "writeWindow_saveText";
      };
      namespace restoreText {
        QString tooltip;
        QString whatsthis;
        const QString icon      = "undo";
        const KShortcut accel   = KKeySequence();
				const char* actionName 	= "writeWindow_restoreText";
      };
      namespace deleteEntry {
        QString tooltip;
        QString whatsthis;
        const QString icon      = "editdelete";
        const KShortcut accel   = KKeySequence();
				const char* actionName 	= "writeWindow_deleteEntry";
      };

      //formatting buttons
      namespace boldText {
        QString tooltip;
        QString whatsthis;
        const QString icon      = "text_bold";
        const KShortcut accel   = KKeySequence();
				const char* actionName 	= "writeWindow_boldText";
      };
      namespace italicText {
        QString tooltip;
        QString whatsthis;
        const QString icon      = "text_italic";
        const KShortcut accel   = KKeySequence();
				const char* actionName 	= "writeWindow_italicText";
      };
      namespace underlinedText {
        QString tooltip;
        QString whatsthis;
        const QString icon      = "text_under";
        const KShortcut accel   = KKeySequence();
				const char* actionName 	= "writeWindow_underlineText";
      };

      namespace alignLeft {
        QString tooltip;
        QString whatsthis;
        const QString icon      = "text_left";
        const KShortcut accel   = KKeySequence();
				const char* actionName 	= "writeWindow_alignLeft";
      };
      namespace alignCenter {
        QString tooltip;
        QString whatsthis;
        const QString icon      = "text_center";
        const KShortcut accel   = KKeySequence();
				const char* actionName 	= "writeWindow_alignCenter";
      };
      namespace alignRight {
        QString tooltip;
        QString whatsthis;
        const QString icon      = "rightjust";
        const KShortcut accel   = KKeySequence();
				const char* actionName 	= "writeWindow_alignRight";
      };
      namespace alignJustify {
        QString tooltip;
        QString whatsthis;
        const QString icon      = "text_block";
        const KShortcut accel   = KKeySequence();
				const char* actionName	= "writeWindow_alignJustify";
      };

      namespace fontFamily {
        QString tooltip;
        QString whatsthis;
	      const KShortcut accel   = KKeySequence();
				const char* actionName	= "writeWindow_fontFamily";
      };
      namespace fontSize {
        QString tooltip;
        QString whatsthis;
	      const KShortcut accel   = KKeySequence();
				const char* actionName	= "writeWindow_fontSize";
      };
      namespace fontColor {
        QString tooltip;
        QString whatsthis;
      };

    };
  };

  namespace settings {
    namespace startup {
      const QString icon = "bt_startconfig";
      namespace dailyTip {
        QString tooltip;
        QString whatsthis;
      };
      namespace showLogo {
        QString tooltip;
        QString whatsthis;
      };
      namespace restoreWorkingArea {
        QString tooltip;
        QString whatsthis;
      };
    };
    namespace fonts {
      const QString icon = "fonts";

      namespace typeChooser {
        QString tooltip;
        QString whatsthis;
      };
    };
    namespace colors {
      const QString icon = "colorize";

      namespace text {
        QString tooltip;
        QString whatsthis;
      };
      namespace background {
        QString tooltip;
        QString whatsthis;
      };
      namespace links {
        QString tooltip;
        QString whatsthis;
      };
      namespace footnotes {
        QString tooltip;
        QString whatsthis;
      };
      namespace strongNumbers {
        QString tooltip;
        QString whatsthis;
      };
      namespace highlightedVerse {
        QString tooltip;
        QString whatsthis;
      };
      namespace jesusWords {
        QString tooltip;
        QString whatsthis;
      };
      namespace morphTags {
        QString tooltip;
        QString whatsthis;
      };
    };
    namespace profiles {
      const QString icon = "view_sidetree";

      namespace list {
        QString tooltip;
        QString whatsthis;
      };
      namespace createNew {
        QString tooltip;
        QString whatsthis;
      };
      namespace deleteCurrent {
        QString tooltip;
        QString whatsthis;
      };
      namespace renameCurrent {
        QString tooltip;
        QString whatsthis;
      };
    };
    namespace sword {
      const QString icon = "bt_swordconfig";

      namespace general {
        namespace cacheKeys {
          QString tooltip;
          QString whatsthis;
        };
        namespace scrolling {
          QString tooltip;
          QString whatsthis;
        };
        namespace language {
          QString tooltip;
          QString whatsthis;
        };
      };
      namespace modules {
        namespace bible {
          QString tooltip;
          QString whatsthis;
        };
        namespace commentary {
          QString tooltip;
          QString whatsthis;
        };
        namespace lexicon {
          QString tooltip;
          QString whatsthis;
        };
        namespace dailyDevotional {
          QString tooltip;
          QString whatsthis;
        };
        namespace hebrewStrongs {
          QString tooltip;
          QString whatsthis;
        };
        namespace greekStrongs {
          QString tooltip;
          QString whatsthis;
        };
        namespace hebrewMorph {
          QString tooltip;
          QString whatsthis;
        };
        namespace greekMorph {
          QString tooltip;
          QString whatsthis;
        };
      };
      namespace filters {
        namespace lineBreaks {
          QString tooltip;
          QString whatsthis;
        };
        namespace verseNumbers {
          QString tooltip;
          QString whatsthis;
        };
        namespace footnotes {
          QString tooltip;
          QString whatsthis;
        };
        namespace strongsNumbers {
          QString tooltip;
          QString whatsthis;
        };
        namespace morphTags {
          QString tooltip;
          QString whatsthis;
        };
        namespace headings {
          QString tooltip;
          QString whatsthis;
        };
        namespace lemmas {
          QString tooltip;
          QString whatsthis;
        };
        namespace hebrewVowelPoints {
          QString tooltip;
          QString whatsthis;
        };
        namespace hebrewCantillation {
           QString tooltip;
          QString whatsthis;
        };
        namespace greekAccents {
          QString tooltip;
          QString whatsthis;
        };
        namespace textualVariants {
          QString tooltip;
          QString whatsthis;
        };
      };
    };
    namespace keys {
      const QString icon = "key_bindings";
    };
  };

    namespace mainIndex { //Main menu->Settings
      namespace search {
        QString tooltip;
        QString whatsthis;
        const QString icon        = "find";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_M;
        const char* actionName    = "GMsearch_action";
      }
      namespace newFolder {
        const QString icon = "folder_new";
      }
      namespace changeFolder {
        const QString icon = "folder";
      }
      namespace openedFolder {
        const QString icon = "folder_open";
      }
      namespace closedFolder {
        const QString icon = "folder";
      }

      namespace bookmark {
        const QString icon = "bookmark";
      }
      namespace changeBookmark {
        const QString icon = "bookmark";
      }
      namespace importBookmarks {
        const QString icon = "bookmark";
      }
      namespace exportBookmarks {
        const QString icon = "bookmark";
      }
      namespace printBookmarks {
        const QString icon = "fileprint";
      }
      namespace deleteItems {
        const QString icon = "filedelete";
      }

      namespace editModuleMenu {
        const QString icon = "pencil";
      }
      namespace editModulePlain {
        const QString icon = "pencil";
      }
      namespace editModuleHTML {
        const QString icon = "pencil";
      }

      namespace unlockModule {
        const QString icon = "unlock";
      }
      namespace aboutModule {
        const QString icon = "info";
      }
    };
  
  namespace helpDialog {
    const QString moduleLocked = "bibletime/helpdialog/help-module-encrypted.html";
    const QString firstStart = "bibletime/helpdialog/help-first-startup.html";
    const QString noSwordModules = "bibletime/helpdialog/help-no-modules.html";
    const QString noSwordModuleConfigDir = "bibletime/helpdialog/help-no-mods-d.html";
    const QString initBackendFailed = "bibletime/helpdialog/help-initbackend-failed.html";
    const QString firstTimeSearchDialog = "bibletime/helpdialog/help-first-time-search.html";
  };
  
};



namespace CResMgr {
  void init_i18n() {
    using namespace CResMgr;
    {
      using namespace mainMenu;
      {
        using namespace file;
        {
          using namespace print;
          tooltip     = makeToolTip( i18n("Open the printer dialog") );
          whatsthis   = makeWhatsThis( tooltip, i18n("Open the printer dialog of BibleTime, where you can edit the print queue, assign styles to the items and print them.") );
        };
        {
          using namespace quit;
          tooltip     = makeToolTip( i18n("Close BibleTime") );
          whatsthis   = makeWhatsThis( tooltip, i18n("Close BibleTime and save the settings.") );
        };

        {
          using namespace view;
          {
            using namespace showMainIndex;
            tooltip     = makeToolTip(i18n("Show main index"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Toggle the main index view."));
          }
          {
            using namespace showToolBar;
            tooltip     = makeToolTip(i18n("Show main toolbar"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Toggle the main toolbar view."));
          }
        }

        {
          using namespace mainMenu::mainIndex;
          {
            using namespace search;
            tooltip     = makeToolTip(i18n("Search in open modules"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Open the search dialog to search in all modules that are currently open."));
          }
          {
            using namespace searchdefaultbible;
            tooltip     = makeToolTip(i18n("Search default bible"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Open the search dialog to search in the default bible."));
          }
        };

        {
          using namespace window;
          {
            using namespace loadProfile;
            tooltip     = makeToolTip( i18n("Load a session") );
            whatsthis   = makeWhatsThis(tooltip,i18n("Restore a saved BibleTime session.") );
          }
          {
            using namespace saveProfile;
            tooltip     = makeToolTip(i18n("Save current session"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Save current BibleTime session so that it can be reused later."));
          }
          {
            using namespace saveToNewProfile;
            tooltip     = makeToolTip(i18n("Save as new session"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Create and save a new session."));
          }
          {
            using namespace setupProfiles;
            tooltip     = makeToolTip(i18n("Edit sessions"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Create new, delete or rename existing sessions."));
          }
          {
            using namespace showFullscreen;
            tooltip     = makeToolTip(i18n("Toggle fullscreen mode"));
            whatsthis   = makeWhatsThis(tooltip,i18n("Toggle fullscreen mode of the main window."));
          }
          {
            using namespace tile;
            tooltip     = makeToolTip(i18n("Tile windows"));
            whatsthis   = makeWhatsThis(tooltip,i18n("Tile the open display windows."));
          }
          {
            using namespace cascade;
            tooltip     = makeToolTip(i18n("Cascade windows"));
            whatsthis   = makeWhatsThis(tooltip,i18n("Cascade the open display windows."));
          }
          {
						{
							using namespace arrangementMode;
						
            	tooltip     = makeToolTip(i18n("Choose the window arrangement mode"));
            	whatsthis   = makeWhatsThis(tooltip,i18n("Choose the way that is used to arrange the display windows."));
						}
						{
            	using namespace arrangementMode::autoTileVertical;
            	tooltip     = makeToolTip(i18n("Automatically tile windows vertical"));
            	whatsthis   = makeWhatsThis(tooltip,i18n("Automatically tile the open display windows."));
						}
						{
            	using namespace arrangementMode::autoTileHorizontal;
            	tooltip     = makeToolTip(i18n("Automatically tile windows horizontal"));
            	whatsthis   = makeWhatsThis(tooltip,i18n("Automatically tile the open display windows."));
						}
						{
							using namespace arrangementMode::autoCascade;
							tooltip     = makeToolTip(i18n("Automatically cascade windows"));
							whatsthis   = makeWhatsThis(tooltip, i18n("Automatically cascade the open display windows."));
						}
          }
					{
            using namespace closeAll;
            tooltip     = makeToolTip(i18n("Close all windows"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Close all open display windows."));
          }
        }

        {
          using namespace mainMenu::settings;
          {
            using namespace editToolBar;
            tooltip     = makeToolTip(i18n("Edit toolbar"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Open BibleTime's toolbar editor."));
          }
          {
            using namespace optionsDialog;
            tooltip     = makeToolTip(i18n("Configure BibleTime"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Open the dialog to set most of BibleTime's preferences."));
          };
          {
            using namespace swordSetupDialog;
            tooltip     = makeToolTip(i18n("Configure Sword"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Open the dialog to configure Sword and install/update/remove Sword modules."));
          };

        };

        {
          using namespace help;
          {
            using namespace handbook;
            tooltip     = makeToolTip(i18n("BibleTime's handbook"));
            whatsthis   = makeWhatsThis(tooltip,i18n("Open BibleTime's handbook in the KDE helpbrowser.<BR>The handbook explains how to use BibleTime in the best way."));
          };
          {
            using namespace bibleStudyHowTo;
            tooltip     = makeToolTip(i18n("BibleTime's Bible study HowTo"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Open the Bible study HowTo included with BibleTime in the KDE helpbrowser.<BR>This HowTo is an introduction on how to study the Bible in an efficient way."));
          };
          {
            using namespace whatsThis;
            tooltip     = makeToolTip(i18n("Enter \"What's this?\" mode"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Switch into the <B>\"What's this?\"</B> mode.<BR>All important elements of BibleTime's GUI offer a \"What's this?\" help message."));
          };
          {
            using namespace bugreport;
            tooltip     = makeToolTip(i18n("Send bugreport"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Send a bugreport to the developers of BibleTime."));
          };
          {
            using namespace dailyTip;
            tooltip     = makeToolTip(i18n("Show daily tips at startup"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Show a daily tip each time BibleTime starts.<BR>The tips contain important Bible quotations and helpful tips for using BibleTime."));
          };
          {
            using namespace aboutBibleTime;
            tooltip     = makeToolTip(i18n("About BibleTime"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Show detailed information about BibleTime."));
          };
          {
            using namespace aboutKDE;
            tooltip     = makeToolTip(i18n("About KDE"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Show detailed information about the KDE project."));
          };
        };
      };
    };

    {
      using namespace searchdialog;
      {
        using namespace searchButton;
        tooltip     = makeToolTip(i18n("Start the search"));
        whatsthis   = makeWhatsThis(tooltip, i18n("Start to search the text in each of the chosen modules."));
      };
      {
        using namespace cancelSearchButton;
        tooltip     = makeToolTip(i18n("Stop search"));
        whatsthis   = makeWhatsThis(tooltip, i18n("Stop the active search."));
      };

      {
        using namespace options;
        {
          using namespace moduleChooserButton;
          tooltip     = makeToolTip(i18n("Choose modules"));
          whatsthis   = makeWhatsThis(tooltip, i18n("Open a dialog to choose modules for the search."));
        };
        {
          using namespace searchedText;
          tooltip     = makeToolTip(i18n("Search text"));
          whatsthis   = makeWhatsThis(tooltip, i18n("Enter the text you want to search in the chosen modules here."));
        };
        {
          using namespace searchType;
          {
            using namespace multipleWords_and;
            tooltip     = makeToolTip(i18n("Multiple words (AND)"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Treat the search text as multiple words. A text must contain all of the words to match. The order of the words is unimportant."));
          };
          {
            using namespace multipleWords_or;
            tooltip     = makeToolTip(i18n("Multiple words (OR)"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Treat the search text as multiple words. A text must contain one or more words of to match. The order is unimportant."));
          };
          {
            using namespace exactMatch;
            tooltip     = makeToolTip(i18n("Exact match"));
            whatsthis   = makeWhatsThis(tooltip, i18n("The search text will be used exactly as entered."));
          };
          {
            using namespace regExp;
            tooltip     = makeToolTip(i18n("Regular expression"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Treat the search string as a GNU regular expression. The BibleTime handbook contains an introduction to regular expressions."));
          };
        };

        {
          using namespace searchOptions;
          {
            using namespace caseSensitive;
            tooltip     = makeToolTip(i18n("Case sensitive matching"));
            whatsthis   = makeWhatsThis(tooltip, i18n("If you choose this option the search will distinguish between upper and lowercase characters."));
          };
        };
        {
          using namespace chooseScope;
          tooltip     = makeToolTip(i18n("Choose a search scope"));
          whatsthis   = makeWhatsThis(tooltip, i18n("Choose a scope from the list. Select the first item to use no scope, the second one is to use each module's last search result as search scope. The others are user defined search scopes."));
        };
        {
          using namespace scopeEditor;
          {
            using namespace rangeList;
            tooltip     = makeToolTip(i18n("Available search scopes"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Select an item from the list to edit the search scope."));
          };
          {
            using namespace nameEdit;
            tooltip     = makeToolTip(i18n("Search scope name"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Change the name of the selected search scope."));
          };
          {
            using namespace editRange;
            tooltip     = makeToolTip(i18n("Edit search scope ranges"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Change the search ranges of the selected search scope item. Have a look at the predefined search scopes to see how search ranges are constructed."));
          };
          {
            using namespace parsedResult;
            tooltip     = makeToolTip(i18n("Parsed search ranges"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Contains the search ranges which will be used for the search."));
          };
          {
            using namespace addNewRange;
            tooltip     = makeToolTip(i18n("Add a new search scope"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Add a new search scope. First enter an appropriate name, then edit the search ranges."));
          };
          {
            using namespace deleteCurrentRange;
            tooltip     = makeToolTip(i18n("Delete current search scope"));
            whatsthis   = makeWhatsThis(tooltip, i18n("Deletes the selected search scope. If you close the dialog using Cancel the settings won't be saved."));
          };
        };
      }
      {
        using namespace result;
        {
          using namespace moduleList;
          tooltip     = makeToolTip(i18n("Used modules"));
          whatsthis   = makeWhatsThis(tooltip, i18n("The list of modules chosen for the search."));
        };
        {
          using namespace foundItems;
          tooltip     = makeToolTip(i18n("Found items of selected module"));
          whatsthis   = makeWhatsThis(tooltip, i18n("This list contains the search result of the selected module."));
        };
        {
          using namespace textPreview;
          tooltip     = makeToolTip(i18n("Text preview"));
          whatsthis   = makeWhatsThis(tooltip, i18n("The text preview of the selected search result item."));
        };
      };
    };

    {
      using namespace displaywindows;
      {
        using namespace general;
        {
					{
	          using namespace scrollButton;
						tooltip = makeToolTip( i18n("Scroll through the list") );
						whatsthis = makeWhatsThis(tooltip, i18n("This button is useful to scroll through the entries of the list. Press the button and move the mouse to increase or decrease the item."));
					}
					{
	          using namespace search;
						tooltip = makeToolTip( i18n("Search in modules") );
						whatsthis = makeWhatsThis(tooltip, i18n("This button open the search dialog with the module of this window."));
						
					}
        };
        using namespace bibleWindow;
        {
          using namespace bookList;
          tooltip = makeToolTip( i18n("List of books") );
          whatsthis = makeWhatsThis(tooltip, i18n("This list contains the books which are available in this module."));
        };
        {
          using namespace nextBook;
          tooltip = makeToolTip( i18n("Show the next book") );
          whatsthis = makeWhatsThis(tooltip, i18n("Show the next book of this module."));
         }
        {
          using namespace previousBook;
          tooltip = makeToolTip( i18n("Show the previous book") );
          whatsthis = makeWhatsThis(tooltip, i18n("Show the previous book of this module."));
        }
        {
          using namespace chapterList;
          tooltip = makeToolTip( i18n("List of chapters") );
          whatsthis = makeWhatsThis(tooltip, i18n("This list contains the chapters which are available in the current book."));
        };
        {
          using namespace nextChapter;
          tooltip = makeToolTip( i18n("Show the next chapter") );
          whatsthis = makeWhatsThis(tooltip, i18n("Show the previous chapter of the module."));
        }
        {
         using namespace previousChapter;
          tooltip = makeToolTip( i18n("Show the previous chapter") );
          whatsthis = makeWhatsThis(tooltip, i18n("Show the next chapter of the module."));
        }
        {
         using namespace verseList;
          tooltip = makeToolTip( i18n("List of verses") );
          whatsthis = makeWhatsThis(tooltip, i18n("This list contains the verses which are available in the current chapter."));
        };
        {
         using namespace nextVerse;
         tooltip = makeToolTip( i18n("Jump to the next verse") );
         whatsthis = makeWhatsThis(tooltip, i18n("In  Bible texts, the next verse will be highlighted. In commentaries, the next entry will be shown."));

        }
        { using namespace previousVerse;
          tooltip = makeToolTip( i18n("Jump to the previous verse") );
          whatsthis = makeWhatsThis(tooltip, i18n("In Bible texts, the previous verse will be highlighted. In commentaries, the previous entry will be shown."));
        }
      };
      {
        using namespace commentaryWindow;
        {
          using namespace syncWindow;
           tooltip = makeToolTip(i18n("Synchronize with Bible windows"));
           whatsthis = makeWhatsThis(tooltip, i18n("Synchronize the displayed entry of this module with the active Bible window."));
         }
      };
      {
        using namespace lexiconWindow;
        {
          using namespace entryList;
          tooltip = makeToolTip( i18n("List of entries") );
          whatsthis = makeWhatsThis(tooltip, i18n("This list contains the entries of the current module."));
        };
        {
          using namespace nextEntry;
          tooltip = makeToolTip( i18n("Show the next entry") );
          whatsthis = makeWhatsThis(tooltip, i18n("The next entry of the module will be shown."));
        };
        {
          using namespace previousEntry;
          tooltip = makeToolTip( i18n("Show the previous entry") );
          whatsthis = makeWhatsThis(tooltip, i18n("The previous entry of the module will be shown."));
        };
      };

    {
      using namespace writeWindow;
      {
        using namespace saveText;
        tooltip   = makeToolTip( i18n("Save the current text") );
        whatsthis = makeWhatsThis( tooltip, i18n("Save the curent text into the module. The old text will be overwritten.") );
      };
      {
        using namespace restoreText;
        tooltip   = makeToolTip( i18n("Restore the old text") );
        whatsthis = makeWhatsThis( tooltip, i18n("Loads the old text from the module and loads it into the edit area. The unsaved text will be lost.") );
      };
      {
        using namespace deleteEntry;
        tooltip   = makeToolTip( i18n("Delete the current entry") );
        whatsthis = makeWhatsThis( tooltip, i18n("Deletes the current entry out of the module. The text will be lost.") );
      };

      //formatting buttons
      {
        using namespace boldText;
        tooltip   = makeToolTip( i18n("Bold text") );
        whatsthis = makeWhatsThis( tooltip, i18n("Toggle bold formatting of the selected text.") );
      };
      {
        using namespace italicText;
        tooltip   = makeToolTip( i18n("Italic text") );
          whatsthis = makeWhatsThis( tooltip, i18n("Toggle italic formatting of the selected text.") );
        };
        {
          using namespace underlinedText;
          tooltip   = makeToolTip( i18n("Underline text") );
          whatsthis = makeWhatsThis( tooltip, i18n("Toggle underlined formatting of the selected text.") );
        };

        {
          using namespace alignLeft;
          tooltip   = makeToolTip( i18n("Align left") );
          whatsthis = makeWhatsThis( tooltip, i18n("The text will be aligned on the left side of the page.") );
        };
        {
          using namespace alignCenter;
          tooltip   = makeToolTip( i18n("Center") );
          whatsthis = makeWhatsThis( tooltip, i18n("Centers the text horizontally.") );
        };
        {
          using namespace alignRight;
          tooltip   = makeToolTip( i18n("Align right") );
          whatsthis = makeWhatsThis( tooltip, i18n("Aligns the text on the right side of the page.") );
        };
        {
          using namespace alignJustify;
          tooltip   = makeToolTip( i18n("Justify") );
          whatsthis = makeWhatsThis( tooltip, i18n("Justifies the text on the page.") );
        };

        {
          using namespace fontFamily;
          tooltip   = makeToolTip( i18n("Set the font") );
          whatsthis = makeWhatsThis( tooltip, i18n("Choose a new font for the selected text.") );
        };
        { using namespace fontSize;
          tooltip   = makeToolTip( i18n("Set the font size") );
          whatsthis = makeWhatsThis( tooltip, i18n("Choose a new font size for the selected text.") );
        };
        { using namespace fontColor;
          tooltip   = makeToolTip( i18n("Set the color") );
          whatsthis = makeWhatsThis( tooltip, i18n("Choose a new color for the selected text.") );
        };
      };
    };
    {
      using namespace settings;
      {
        using namespace startup;
        {
          using namespace dailyTip;
          tooltip   = makeToolTip( i18n("Show daily tip") );
          whatsthis = makeWhatsThis( tooltip, i18n("Activate this box to see a daily tip on startup."));
        };
        {
          using namespace showLogo;
          tooltip   = makeToolTip( i18n("Show startup logo") );
          whatsthis = makeWhatsThis( tooltip, i18n("Activate this to see the BibleTime logo on startup."));
        };
        {
          using namespace restoreWorkingArea;
          tooltip   = makeToolTip( i18n("Restore the last BibleTime session on startup") );
          whatsthis = makeWhatsThis( tooltip, i18n("Save the user's session when BibleTime is closed and restore it on the next startup."));
        };
      };
      {
        using namespace fonts;
        {
          using namespace typeChooser;
          tooltip   = makeToolTip( i18n("Choose a language") );
          whatsthis = makeWhatsThis( tooltip, i18n("The font selection below will apply to all texts in this language."));
        };
      };
      {
        using namespace colors;
        {
          using namespace text;
          tooltip = makeToolTip( i18n("Text color") );
          whatsthis = makeWhatsThis( tooltip, i18n("The color used for normal text in the display windows."));
        };
        {
          using namespace background;
          tooltip = makeToolTip( i18n("Color of the background") );
          whatsthis = makeWhatsThis( tooltip, i18n("This is the background color used in display windows."));
        };
        {
          using namespace links;
          tooltip   = makeToolTip( i18n("Link color") );
          whatsthis = makeWhatsThis( tooltip, i18n("The color used for all references into Sword modules."));
        };
        {
          using namespace footnotes;
          tooltip   = makeToolTip( i18n("Footnote color") );
          whatsthis = makeWhatsThis( tooltip, i18n("This color will be used for all footnotes in the display windows."));
        };
        {
          using namespace strongNumbers;
          tooltip   = makeToolTip( i18n("Strong's Numbers") );
          whatsthis = makeWhatsThis( tooltip, i18n("This color will be used for every Strong's Number in the display windows."));
        };
        {
          using namespace highlightedVerse;
          tooltip   = makeToolTip( i18n("Color of the highlighted verse") );
          whatsthis = makeWhatsThis( tooltip, i18n("This is the color of the highlighted verse used in display windows."));
        };
        {
          using namespace jesusWords;
          tooltip   = makeToolTip( i18n("Words of Jesus") );
          whatsthis = makeWhatsThis( tooltip, i18n("All words spoken by Jesus will appear in this color."));
        };
        {
          using namespace morphTags;
          tooltip = makeToolTip( i18n("Morphological tags") );
          whatsthis = makeWhatsThis( tooltip, i18n("All morphological tags will appear in this color."));
        };
      };
      {
        using namespace profiles;
        {
          using namespace list;
          tooltip   = makeToolTip( i18n("List of sessions") );
          whatsthis = makeWhatsThis( tooltip, i18n("The list of available sessions."));
        };
        {
          using namespace createNew;
          tooltip   = makeToolTip( i18n("Create new session") );
          whatsthis = makeWhatsThis( tooltip, i18n("Create a new, empty session with default settings."));
        };
        {
          using namespace deleteCurrent;
          tooltip   = makeToolTip( i18n("Delete session") );
          whatsthis = makeWhatsThis( tooltip, i18n("Delete the selected session."));
        };
        {
          using namespace renameCurrent;
          tooltip   = makeToolTip( i18n("Rename session") );
          whatsthis = makeWhatsThis( tooltip, i18n("Rename the selected session."));
        };
      };
      {
        using namespace settings::sword;
        {
          using namespace general;
          {
            using namespace cacheKeys;
            tooltip   = makeToolTip( i18n("Use cache files for the lexicon entries") );
            whatsthis = makeWhatsThis( tooltip, i18n("Activate this to let BibleTime create index files for the lexicon entries. This increases the speed of opening large lexicon modules significantly, but does consume some disk space. Recommended."));
          };
          {
            using namespace scrolling;
            tooltip   = makeToolTip( i18n("Use down arrow to scroll to next verse") );
            whatsthis = makeWhatsThis( tooltip, i18n("Check this to use the down arrow to scroll to the next verse."));
          };
          {
            using namespace language;
            tooltip   = makeToolTip( i18n("List of languages") );
            whatsthis = makeWhatsThis( tooltip, i18n("Contains the languages which can be used for the biblical booknames."));
          };
        };
        {
          using namespace settings::sword::modules;
          {
            using namespace bible;
            tooltip   = makeToolTip( i18n("The default bible used in the workspace area") );
            whatsthis = makeWhatsThis( tooltip, i18n("The default Bible is used when a hyperlink into a Bible is clicked."));
          };
          {
            using namespace commentary;
            tooltip   = makeToolTip( i18n("The default commentary used in the workspace area") );
            whatsthis = makeWhatsThis( tooltip, i18n("The default commentary is used when a hyperlink into a commentary is clicked."));
          };
          {
            using namespace lexicon;
            tooltip   = makeToolTip( i18n("The default lexicon used in the workspace area") );
            whatsthis = makeWhatsThis( tooltip, i18n("The default lexicon is used when a hyperlink into a lexicon is clicked."));
          };
          {
            using namespace dailyDevotional;
            tooltip   = makeToolTip( i18n("The default daily devotional module") );
            whatsthis = makeWhatsThis( tooltip, i18n("The default devotional will be used to display a short start up devotional."));
          };
          {
            using namespace hebrewStrongs;
            tooltip   = makeToolTip( i18n("The default Hebrew lexicon used in the workspace area") );
            whatsthis = makeWhatsThis( tooltip, i18n("The default Hebrew lexicon is used when a hyperlink into a Hebrew lexicon is clicked."));
          };
          {
            using namespace greekStrongs;
            tooltip   = makeToolTip( i18n("The default Greek lexicon used in the workspace area") );
            whatsthis = makeWhatsThis( tooltip, i18n("The default Greek lexicon is used when a hyperlink into a Greek lexicon is clicked."));
          };
          {
            using namespace hebrewMorph;
            tooltip   = makeToolTip( i18n("The default Morphological lexicon for Hebrew texts used in the workspace area") );
            whatsthis = makeWhatsThis( tooltip, i18n("The standard morphological lexicon for Hebrew texts is used when a hyperlink of a morphological tag in a Hebrew text is clicked."));
          };
          {
            using namespace greekMorph;
            tooltip   = makeToolTip( i18n("The default Morphological lexicon for Greek texts used in the workspace area") );
            whatsthis = makeWhatsThis( tooltip, i18n("The standard morphological lexicon for Greek texts is used when a hyperlink of a morphological tag in a Greek text is clicked."));
          };
        };
        {
          using namespace filters;
          {
            using namespace lineBreaks;
            tooltip   = makeToolTip( i18n("Line breaks") );
            whatsthis = makeWhatsThis( tooltip, i18n("Insert line breaks after each verse."));
          };
          {
            using namespace verseNumbers;
            tooltip   = makeToolTip( i18n("Verse numbers") );
            whatsthis = makeWhatsThis( tooltip, i18n("Show the verse number before each verse in Bibles."));
          };
          {
            using namespace footnotes;
            tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordModuleInfo::footnotes) );
            whatsthis = makeWhatsThis( tooltip, i18n("Show footnotes embedded in the module's text."));
          };
          {
            using namespace strongsNumbers;
            tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordModuleInfo::strongNumbers) );
            whatsthis = makeWhatsThis( tooltip, i18n("Show Strong's Numbers embedded in the text of modules which support this feature."));
          };
          {
            using namespace morphTags;
            tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordModuleInfo::morphTags) );
            whatsthis = makeWhatsThis( tooltip, i18n("Show tags providing morphological information in the text of modules which support this feature."));
          };
          {
            using namespace headings;
            tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordModuleInfo::headings) );
            whatsthis = makeWhatsThis( tooltip, i18n("Show section headings in the text in the text of modules which support this feature."));
          };
          {
            using namespace lemmas;
            tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordModuleInfo::lemmas) );
            whatsthis = makeWhatsThis( tooltip, i18n("Show root words."));
          };
          {
            using namespace hebrewVowelPoints;
            tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordModuleInfo::hebrewPoints) );
            whatsthis = makeWhatsThis( tooltip, i18n("Show hebrew vowel points in the text."));
          };
          {
            using namespace hebrewCantillation;
            tooltip  = makeToolTip( CSwordBackend::translatedOptionName(CSwordModuleInfo::hebrewCantillation) );
            whatsthis = makeWhatsThis( tooltip, i18n("Show hebrew cantillation marks in the text."));
          };
          {
            using namespace greekAccents;
            tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordModuleInfo::greekAccents) );
            whatsthis = makeWhatsThis( tooltip, i18n("Show greek accents in the text."));
          };
          {
            using namespace textualVariants;
            tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordModuleInfo::textualVariants) );
            whatsthis = makeWhatsThis( tooltip, i18n("Use different variants from the textual sources."));
          };
        };
      };
    };
    {
      using namespace mainIndex;
      {
        using namespace search;
        tooltip     = makeToolTip(i18n("Search in open modules"));
        whatsthis   = makeWhatsThis(tooltip, i18n("Opens the search dialog to search in the modules that are currently open."));
      };
    };
  };
};
