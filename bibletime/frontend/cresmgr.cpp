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
      namespace clearQueue {
        const QString tooltip    = makeToolTip( i18n("Clear print queue") );
        const QString whatsthis  = makeWhatsThis( tooltip, i18n("Clear the print queue of BibleTime.") );
        const QString icon       = QString::fromLatin1("queue");
        const KShortcut accel    = KShortcut();
        const char* actionName   = "fileClearQueue_action";
      }
      namespace print { //a standard action
        const QString tooltip     = makeToolTip( i18n("Open the printer dialog") );
        const QString whatsthis   = makeWhatsThis( tooltip, i18n("Open the printer dialog of BibleTime, where you can edit the print queue, assign styles to the items and print them.") );
      }
      namespace quit { //a standard action
        const QString tooltip     = makeToolTip( i18n("Close BibleTime") );
        const QString whatsthis   = makeWhatsThis( tooltip, i18n("Close BibleTime and save the settings.") );
      }
    }

    namespace view { //Main menu->View
      namespace showMainIndex {
        const QString tooltip     = makeToolTip(i18n("Show main index"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Toggle the main index view."));
        const QString icon        = "view_sidetree";
        const KShortcut accel     = Qt::Key_F9;
        const char* actionName    = "viewMainIndex_action";
      }
      namespace showToolBar { //a standard action
        const QString tooltip     = makeToolTip(i18n("Show main toolbar"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Toggle the main toolbar view."));
      }
    }

    namespace mainIndex { //Main menu->Settings
      namespace search {
        const QString tooltip     = makeToolTip(i18n("Search in open modules"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the search dialog to search in all modules that are currently open."));
        const QString icon        = "find";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_M;
        const char* actionName    = "GMsearch_action";
      }
      namespace searchdefaultbible {
        const QString tooltip     = makeToolTip(i18n("Search default bible"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the search dialog to search in the default bible."));
        const QString icon        = "find";
        const KShortcut accel     = Qt::CTRL + Qt::Key_F;
        const char* actionName    = "GMsearchdefaultbible_action";
      }
    };

    namespace window { //Main menu->Window
      namespace loadProfile {
        const QString tooltip     = makeToolTip( i18n("Load a new session") );
        const QString whatsthis   = makeWhatsThis(tooltip,i18n("Restore a saved BibleTime session.") );
        const QString icon        = "view_sidetree";
        const char* actionName    = "windowLoadProfile_action";
      }
      namespace saveProfile {
        const QString tooltip     = makeToolTip(i18n("Save current session"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Save current BibleTime session so that it can be reused later."));
        const QString icon        = "view_sidetree";
        const char* actionName    = "windowSaveProfile_action";
      }
      namespace saveToNewProfile {
        const QString tooltip     = makeToolTip(i18n("Save as new session"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Create and save a new session."));
        const QString icon        = "view_sidetree";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_S;
        const char* actionName    = "windowSaveToNewProfile_action";
      }
      namespace setupProfiles {
        const QString tooltip     = makeToolTip(i18n("Edit sessions"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Create new, delete or rename existing sessions."));
        const QString icon        = "view_sidetree";
        const KShortcut accel     = KShortcut();
        const char* actionName    = "windowEditProfiles_action";
      }
      namespace showFullscreen {
        const QString tooltip     = makeToolTip(i18n("Toggle fullscreen mode"));
        const QString whatsthis   = makeWhatsThis(tooltip,i18n("Toggle fullscreen mode of the main window."));
        const QString icon        = "window_fullscreen";
        const KShortcut accel     = KShortcut();
        const char* actionName    = "windowFullscreen_action";
      }
      namespace tile {
        const QString tooltip     = makeToolTip(i18n("Tile windows"));
        const QString whatsthis   = makeWhatsThis(tooltip,i18n("Tile the open display windows."));
        const QString icon        = "bt_tile";
        const KShortcut accel     = Qt::CTRL + Qt::Key_T;
        const char* actionName    = "windowTile_action";
      }
      namespace cascade {
        const QString tooltip     = makeToolTip(i18n("Cascade windows"));
        const QString whatsthis   = makeWhatsThis(tooltip,i18n("Cascade the open display windows."));
        const QString icon        = "bt_cascade";
        const KShortcut accel     = Qt::CTRL + Qt::Key_A;
        const char* actionName    = "windowCascade_action";
      }
      namespace autoTile {
        const QString tooltip     = makeToolTip(i18n("Automatically tile windows"));
        const QString whatsthis   = makeWhatsThis(tooltip,i18n("Automatically tile the open display windows."));
        const QString icon        = "bt_tile_auto";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_T;
        const char* actionName    = "windowAutoTile_action";
      }
      namespace autoCascade {
        const QString tooltip     = makeToolTip(i18n("Automatically cascade windows"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Automatically cascade the open display windows."));
        const QString icon        = "bt_cascade_auto";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_A;
        const char* actionName    = "windowAutoCascade_action";
      }
      namespace closeAll {
        const QString tooltip     = makeToolTip(i18n("Close all windows"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Close all open display windows."));
        const QString icon        = "fileclose";
        const KShortcut accel     = Qt::CTRL + Qt::Key_K;
        const char* actionName    = "windowCloseAll_action";
      }
    }

    namespace settings { //Main menu->Settings
      namespace editToolBar { // available as KStdAction
        const QString tooltip     = makeToolTip(i18n("Edit toolbar"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open BibleTime's tool bar editor."));
      }
      namespace optionsDialog { // available as KStdAction
        const QString tooltip     = makeToolTip(i18n("Configure BibleTime"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the dialog to set most of BibleTime's preferences."));
      };
    };

    namespace help { //Main menu->Help
      namespace handbook {
        const QString tooltip     = makeToolTip(i18n("BibleTime's handbook"));
        const QString whatsthis   = makeWhatsThis(tooltip,i18n("Open BibleTime's handbook in the KDE helpbrowser.<BR>The handbook explains how to use BibleTime in the best way."));
        const QString icon        = "contents";
        const KShortcut accel     = Qt::Key_F1;
        const char* actionName    = "helpHandbook_action";
      };
      namespace bibleStudyHowTo {
        const QString tooltip     = makeToolTip(i18n("BibleTime's Bible study HowTo"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the Bible study HowTo included with BibleTime in the KDE helpbrowser.<BR>This HowTo is an introduction on how to study the Bible in an efficient way."));
        const QString icon        = "contents";
        const KShortcut accel     = Qt::Key_F2;
        const char* actionName    = "helpHowTo_action";
      };
      namespace whatsThis { // available as KStdAction
        const QString tooltip     = makeToolTip(i18n("Enter \"What's this?\" mode"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Switch into the <B>\"What's this?\"</B> mode.<BR>All important elements of BibleTime's GUI offer a \"What's this?\" help message."));
      };
      namespace bugreport { // available as KStdAction
        const QString tooltip     = makeToolTip(i18n("Send bugreport"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Send a bugreport to the developers of BibleTime."));
      };
      namespace dailyTip {
        const QString tooltip     = makeToolTip(i18n("Show daily tips at startup"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Show a daily tip each time BibleTime starts.<BR>The tips contain important Bible quotations and helpful tips for using BibleTime."));
        const QString icon        = "idea";
        const KShortcut accel     = KShortcut();
        const char* actionName    = "helpDailyTip_action";
      };
      namespace aboutBibleTime { // available as KStdAction
        const QString tooltip     = makeToolTip(i18n("About BibleTime"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Show detailed information about BibleTime."));
      };
      namespace aboutKDE { // available as KStdAction
        const QString tooltip     = makeToolTip(i18n("About KDE"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Show detailed information about the KDE project."));
      };
    };
  }  //end of main menu

  namespace searchdialog {
    const QString icon = "find";
    
    namespace searchButton {
      const QString tooltip     = makeToolTip(i18n("Start the search"));
      const QString whatsthis   = makeWhatsThis(tooltip, i18n("Start to search the text in each of the chosen modules."));
    };
    namespace cancelSearchButton {
      const QString tooltip     = makeToolTip(i18n("Stop search"));
      const QString whatsthis   = makeWhatsThis(tooltip, i18n("Stop the active search."));
    };

    namespace options {
      namespace moduleChooserButton {
        const QString tooltip     = makeToolTip(i18n("Choose modules"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open a dialog to choose modules for the search."));
      };
      namespace searchedText {
        const QString tooltip     = makeToolTip(i18n("Search text"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Enter the text you want to search in the chosen modules here."));
      };
      namespace searchType {
        namespace multipleWords_and {
          const QString tooltip     = makeToolTip(i18n("Multiple words (AND)"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Treat the search text as multiple words. A text must contain all of the words to match. The order of the words is unimportant."));
        };
        namespace multipleWords_or {
          const QString tooltip     = makeToolTip(i18n("Multiple words (OR)"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Treat the search text as multiple words. A text must contain one or more words of to match. The order is unimportant."));
        };
        namespace exactMatch {
          const QString tooltip     = makeToolTip(i18n("Exact match"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("The search text will be used exactly as entered."));
        };
        namespace regExp {
          const QString tooltip     = makeToolTip(i18n("Regular expression"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Treat the search string as a GNU regular expression. The BibleTime handbook contains an introduction to regular expressions."));
        };
      };

      namespace searchOptions {
        namespace caseSensitive {
          const QString tooltip     = makeToolTip(i18n("Case sensitive matching"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("If you choose this option the search will distinguish between upper and lowercase characters."));
        };
      };
      namespace chooseScope {
        const QString tooltip     = makeToolTip(i18n("Choose a search scope"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Choose a scope from the list. Select the first item to use no scope, the second one is to use each module's last search result as search scope. The others are user defined search scopes."));
      };
      namespace scopeEditor {
        namespace rangeList {
          const QString tooltip     = makeToolTip(i18n("Available search scopes"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Select an item from the list to edit the search scope."));
        };
        namespace nameEdit {
          const QString tooltip     = makeToolTip(i18n("Search scope name"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Change the name of the selected search scope."));
        };
        namespace editRange {
          const QString tooltip     = makeToolTip(i18n("Edit search scope ranges"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Change the search ranges of the selected search scope item. Have a look at the predefined search scopes to see how search ranges are constructed."));
        };
        namespace parsedResult {
          const QString tooltip     = makeToolTip(i18n("Parsed search ranges"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Contains the search ranges which will be used for the search."));
        };
        namespace addNewRange {
          const QString tooltip     = makeToolTip(i18n("Add a new search scope"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Add a new search scope. First enter an appropriate name, then edit the search ranges."));
        };
        namespace deleteCurrentRange {
          const QString tooltip     = makeToolTip(i18n("Delete current search scope"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Deletes the selected search scope. If you close the dialog using Cancel the settings won't be saved."));
        };

      };
    }
    namespace result {
      namespace moduleList {
        const QString tooltip     = makeToolTip(i18n("Used modules"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("The list of modules chosen for the search."));

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
        const QString tooltip     = makeToolTip(i18n("Found items of selected module"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("This list contains the search result of the selected module."));

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
        const QString tooltip     = makeToolTip(i18n("Text preview"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("The text preview of the selected search result item."));
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
        const QString tooltip = makeToolTip( i18n("Scroll through the list") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("This button is useful to scroll through the entries of the list. Press the button and move the mouse to increase or decrease the item."));

      };
    };
    namespace bibleWindow {
      namespace bookList {
        const QString tooltip = makeToolTip( i18n("List of books") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("This list contains the books which are available in this module."));
      };      
      namespace nextBook {
        const QString tooltip = makeToolTip( i18n("Show the next book") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Show the next book of this module."));
        const KShortcut accel = Qt::CTRL + Qt::Key_B;
       }
      namespace previousBook {
        const QString tooltip = makeToolTip( i18n("Show the previous book") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Show the previous book of this module."));
        const KShortcut accel = Qt::CTRL + Qt::SHIFT + Qt::Key_B;
      }


      namespace chapterList {
        const QString tooltip = makeToolTip( i18n("List of chapters") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("This list contains the chapters which are available in the current book."));
      };
      namespace nextChapter {
        const QString tooltip = makeToolTip( i18n("Show the next chapter") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Show the previous chapter of the module."));
        const KShortcut accel = Qt::CTRL + Qt::Key_H;
      }
      namespace previousChapter {
        const QString tooltip = makeToolTip( i18n("Show the previous chapter") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Show the next chapter of the module."));
        const KShortcut accel = Qt::CTRL + Qt::SHIFT + Qt::Key_H;
      }

      namespace verseList {
        const QString tooltip = makeToolTip( i18n("List of verses") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("This list contains the verses which are available in the current chapter."));
      };
      namespace nextVerse {
        const QString tooltip = makeToolTip( i18n("Jump to the next verse") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("In  Bible texts, the next verse will be highlighted. In commentaries, the next entry will be shown."));
        const KShortcut accel = Qt::CTRL + Qt::Key_V;
      }
      namespace previousVerse {
        const QString tooltip = makeToolTip( i18n("Jump to the previous verse") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("In Bible texts, the previous verse will be highlighted. In commentaries, the previous entry will be shown."));
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
         const QString tooltip = makeToolTip(i18n("Synchronize with Bible windows"));
         const QString whatsthis = makeWhatsThis(tooltip, i18n("Synchronize the displayed entry of this module with the active Bible window."));
         const QString icon = "bt_sync";         
         const KShortcut accel = KShortcut();//CResMgr::displaywindows::biblewindow::nextBook::accel;
       }
       
    };
    namespace lexiconWindow {
      namespace entryList {
        const QString tooltip = makeToolTip( i18n("List of entries") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("This list contains the entries of the current module."));
      };
      namespace nextEntry {
        const QString tooltip = makeToolTip( i18n("Show the next entry") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("The next entry of the module will be shown."));
        const KShortcut accel = KShortcut();
      }
      namespace previousEntry {
        const QString tooltip = makeToolTip( i18n("Show the previous entry") );
        const QString whatsthis = makeWhatsThis(tooltip, i18n("The previous entry of the module will be shown."));
        const KShortcut accel = KShortcut();
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
        const KShortcut accel = KShortcut();
      };
    };
    
    namespace writeWindow {
      namespace saveText {
        const QString tooltip   = makeToolTip( i18n("Save the current text") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Save the curent text into the module. The old text will be overwritten.") );
        const QString icon      = "filesave";
        const KShortcut accel   = KShortcut();
      };
      namespace restoreText {
        const QString tooltip   = makeToolTip( i18n("Restore the old text") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Loads the old text from the module and loads it into the edit area. The unsaved text will be lost.") );
        const QString icon      = "undo";
        const KShortcut accel   = KShortcut();
      };
      namespace deleteEntry {
        const QString tooltip   = makeToolTip( i18n("Delete the current entry") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Deletes the current entry out of the module. The text will be lost.") );
        const QString icon      = "editdelete";
        const KShortcut accel   = KShortcut();
      };

      //formatting buttons
      namespace boldText {
        const QString tooltip   = makeToolTip( i18n("Bold text") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Toggle bold formatting of the selected text.") );
        const QString icon      = "text_bold";
        const KShortcut accel   = KShortcut();
      };
      namespace italicText {
        const QString tooltip   = makeToolTip( i18n("Italic text") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Toggle italic formatting of the selected text.") );
        const QString icon      = "text_italic";
        const KShortcut accel   = KShortcut();
      };
      namespace underlinedText {
        const QString tooltip   = makeToolTip( i18n("Underline text") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Toggle underlined formatting of the selected text.") );
        const QString icon      = "text_under";
        const KShortcut accel   = KShortcut();
      };      

      namespace alignLeft {
        const QString tooltip   = makeToolTip( i18n("Align left") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("The text will be aligned on the left side of the page.") );
        const QString icon      = "text_left";
        const KShortcut accel   = KShortcut();
      };
      namespace alignCenter {
        const QString tooltip   = makeToolTip( i18n("Center") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Centers the text horizontally.") );
        const QString icon      = "text_center";
        const KShortcut accel   = KShortcut();
      };
      namespace alignRight {
        const QString tooltip   = makeToolTip( i18n("Align right") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Aligns the text on the right side of the page.") );
        const QString icon      = "rightjust";
        const KShortcut accel   = KShortcut();
      };
      namespace alignJustify {
        const QString tooltip   = makeToolTip( i18n("Justify") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Justifies the text on the page.") );
        const QString icon      = "text_block";
        const KShortcut accel   = KShortcut();
      };

      namespace fontFamily {
        const QString tooltip   = makeToolTip( i18n("Set the font") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Choose a new font for the selected text.") );
      };
      namespace fontSize {
        const QString tooltip   = makeToolTip( i18n("Set the font size") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Choose a new font size for the selected text.") );
      };
      namespace fontColor {
        const QString tooltip   = makeToolTip( i18n("Set the color") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Choose a new color for the selected text.") );
      };

    };
  };

  namespace printing {
    namespace itemPage {
      namespace styleChooser {
        const QString tooltip = makeToolTip(i18n("Styles"));
        const QString whatsthis = makeWhatsThis(tooltip,i18n("This list contains the available print styles."));
      };     
      namespace itemList {
        const QString tooltip = makeToolTip(i18n("List of print items"));
        const QString whatsthis = makeWhatsThis(tooltip,i18n("This is the list which contains the items to be printed."));
      };
      namespace itemUp {
        const QString icon = "up";
        const QString tooltip = makeToolTip(i18n("Move item up"));
        const QString whatsthis = makeWhatsThis(tooltip,i18n("Move the selected item up one line."));
      };
      namespace itemDown {
        const QString icon = "down";
        const QString tooltip = makeToolTip(i18n("Move item down"));
        const QString whatsthis = makeWhatsThis(tooltip,i18n("Move the selected item down one line."));
      };
      namespace itemDelete {
        const QString icon = "edittrash";
        const QString tooltip = makeToolTip(i18n("Delete items"));
        const QString whatsthis = makeWhatsThis(tooltip,i18n("Delete the selected items."));
      };
    };
    
    namespace styleList {
      namespace newStyle {
        const QString tooltip = makeToolTip( i18n("New printing style") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Create a new printing style.") );
        const QString icon = "filenew";
      };
      namespace deleteStyle {
        const QString tooltip = makeToolTip( i18n("Delete printing style") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Remove the selected printing style from the list.") );
        const QString icon = "editdelete";
      };
      namespace editStyle {
        const QString tooltip = makeToolTip( i18n("Edit printing style") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Change the properties of the selected printing style.") );
        const QString icon = "pencil";
      };
    };
    namespace printerSettings {
      namespace margins {
        const QString whatsthis = makeWhatsThis( makeToolTip(i18n("Size of page borders")), i18n("This group contains the page borders of the page measured in millimeters.") );

        namespace upper {
          const QString tooltip = makeToolTip( i18n("Upper border") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The size of the border on the top of the paper.") );
        };
        namespace lower {
          const QString tooltip = makeToolTip( i18n("Lower border") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The size of the border on the bottom of the paper.") );
        };
        namespace left {
          const QString tooltip = makeToolTip( i18n("Left border") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The size of the border on the left side of the paper.") );
        };
        namespace right {
          const QString tooltip = makeToolTip( i18n("Right border") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The size of the border on the right side of the paper.") );
        };
      };
    };    
  };
  namespace settings {
    namespace startup {
      const QString icon = "bt_startconfig";
      namespace dailyTip {
        const QString tooltip   = makeToolTip( i18n("Show daily tip") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Activate this box to see a daily tip on startup."));
      };
      namespace showLogo {
        const QString tooltip   = makeToolTip( i18n("Show startup logo") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Activate this to see the BibleTime logo on startup."));
      };
      namespace restoreWorkingArea {
        const QString tooltip   = makeToolTip( i18n("Restore the last BibleTime session on startup") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Save the user's session when BibleTime is closed and restore it on the next startup."));
      };
    };
    namespace fonts {
      const QString icon = "fonts";
            
      namespace typeChooser {
        const QString tooltip   = makeToolTip( i18n("Choose a language") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("The font selection below will apply to all texts in this language."));
      };
    };
    namespace colors {
      const QString icon = "bt_displayconfig";
            
      namespace text {
        const QString tooltip = makeToolTip( i18n("Text color") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("The color used for normal text in the display windows."));
      };
      namespace background {
        const QString tooltip = makeToolTip( i18n("Color of the background") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("This is the background color used in display windows."));
      };
      namespace links {
        const QString tooltip   = makeToolTip( i18n("Link color") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("The color used for all references into Sword modules."));
      };
      namespace footnotes {
        const QString tooltip   = makeToolTip( i18n("Footnote color") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("This color will be used for all footnotes in the display windows."));
      };
      namespace strongNumbers {
        const QString tooltip   = makeToolTip( i18n("Strong's Numbers") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("This color will be used for every Strong's Number in the display windows."));
      };
      namespace highlightedVerse {
        const QString tooltip   = makeToolTip( i18n("Color of the highlighted verse") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("This is the color of the highlighted verse used in display windows."));
      };
      namespace jesusWords {
        const QString tooltip   = makeToolTip( i18n("Words of Jesus") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("All words spoken by Jesus will appear in this color."));
      };
      namespace morphTags {
        const QString tooltip = makeToolTip( i18n("Morphological tags") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("All morphological tags will appear in this color."));
      };
    };
    namespace profiles {
      const QString icon = "view_sidetree";
            
      namespace list {
        const QString tooltip   = makeToolTip( i18n("List of sessions") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("The list of available sessions."));
      };
      namespace createNew {
        const QString tooltip   = makeToolTip( i18n("Create new session") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Create a new, empty session with default settings."));
      };
      namespace deleteCurrent {
        const QString tooltip   = makeToolTip( i18n("Delete session") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Delete the selected session."));
      };
      namespace renameCurrent {
        const QString tooltip   = makeToolTip( i18n("Rename session") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Rename the selected session."));
      };
    };
    namespace sword {
      const QString icon = "bt_swordconfig";
            
      namespace general {
        namespace cacheKeys {
          const QString tooltip   = makeToolTip( i18n("Use cache files for the lexicon entries") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Activate this to let BibleTime create index files for the lexicon entries. This increases the speed of opening large lexicon modules significantly, but does consume some disk space. Recommended."));
        };
        namespace scrolling {
          const QString tooltip   = makeToolTip( i18n("Use down arrow to scroll to next verse") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Check this to use the down arrow to scroll to the next verse."));
        };
        namespace language {
          const QString tooltip   = makeToolTip( i18n("List of languages") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Contains the languages which can be used for the biblical booknames."));
        };
      };
      namespace modules {
        namespace bible {
          const QString tooltip   = makeToolTip( i18n("The default bible used in the workspace area") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The default Bible is used when a hyperlink into a Bible is clicked."));
        };
        namespace commentary {
          const QString tooltip   = makeToolTip( i18n("The default commentary used in the workspace area") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The default commentary is used when a hyperlink into a commentary is clicked."));
        };
        namespace lexicon {
          const QString tooltip   = makeToolTip( i18n("The default lexicon used in the workspace area") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The default lexicon is used when a hyperlink into a lexicon is clicked."));
        };
        namespace dailyDevotional {
          const QString tooltip   = makeToolTip( i18n("The default daily devotional module") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The default devotional will be used to display a short start up devotional."));
        };
        namespace hebrewStrongs {
          const QString tooltip   = makeToolTip( i18n("The default Hebrew lexicon used in the workspace area") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The default Hebrew lexicon is used when a hyperlink into a Hebrew lexicon is clicked."));
        };
        namespace greekStrongs {
          const QString tooltip   = makeToolTip( i18n("The default Greek lexicon used in the workspace area") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The default Greek lexicon is used when a hyperlink into a Greek lexicon is clicked."));
        };
        namespace hebrewMorph {
          const QString tooltip   = makeToolTip( i18n("The default Morphological lexicon for Hebrew texts used in the workspace area") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The standard morphological lexicon for Hebrew texts is used when a hyperlink of a morphological tag in a Hebrew text is clicked."));
        };
        namespace greekMorph {
          const QString tooltip   = makeToolTip( i18n("The default Morphological lexicon for Greek texts used in the workspace area") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The standard morphological lexicon for Greek texts is used when a hyperlink of a morphological tag in a Greek text is clicked."));
        };
      };
      namespace filters {
        namespace lineBreaks {
          const QString tooltip   = makeToolTip( i18n("Line breaks") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Insert line breaks after each verse."));
        };
        namespace verseNumbers {
          const QString tooltip   = makeToolTip( i18n("Verse numbers") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Show the verse number before each verse in Bibles."));
        };
        namespace footnotes {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::footnotes) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Show footnotes embedded in the module's text."));
        };
        namespace strongsNumbers {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::strongNumbers) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Show Strong's Numbers embedded in the text of modules which support this feature."));
        };
        namespace morphTags {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::morphTags) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Show tags providing morphological information in the text of modules which support this feature."));
        };
        namespace headings {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::headings) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Show section headings in the text in the text of modules which support this feature."));
        };
        namespace lemmas {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::lemmas) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Show root words."));
        };
        namespace hebrewVowelPoints {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::hebrewPoints) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Show hebrew vowel points in the text."));
        };
        namespace hebrewCantillation {
           const QString tooltip  = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::hebrewCantillation) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Show hebrew cantillation marks in the text."));
        };
        namespace greekAccents {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::greekAccents) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Show greek accents in the text."));
        };
        namespace textualVariants {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::textualVariants) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Use different variants from the textual sources."));
        };
      };
    };
    namespace keys {
      const QString icon = "key_bindings";
    };
  };

    namespace mainIndex { //Main menu->Settings
      namespace search {
        const QString tooltip     = makeToolTip(i18n("Search in open modules"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Opens the search dialog to search in the modules that are currently open."));
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

