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
        const QString tooltip    = makeToolTip( i18n("Clear the printing queue") );
        const QString whatsthis  = makeWhatsThis( tooltip, i18n("Clear the print queue of BibleTime. All items will be removed from the list and the print button will be disabled.") );
        const QString icon       = QString::fromLatin1("queue");
        const KShortcut accel    = KShortcut();
        const char* actionName   = "fileClearQueue_action";
      }
      namespace print { //a standard action
        const QString tooltip     = makeToolTip( i18n("Open the printerdialog") );
        const QString whatsthis   = makeWhatsThis( tooltip, i18n("Open the printer dialog of BibleTime. Here you can edit the print queue and assign styles to the items.") );
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
        const KShortcut accel     = KShortcut();
        const char* actionName    = "viewGroupManager_action";
      }
      namespace showToolBar { //a standard action
        const QString tooltip     = makeToolTip(i18n("Show toolbar"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Toggle the main toolbar view."));
      }
    }

    namespace mainIndex { //Main menu->Settings
      namespace search {
        const QString tooltip     = makeToolTip(i18n("Search in open modules"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Opens the search dialog to search in the currently opened modules."));
        const QString icon        = "find";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_M;
        const char* actionName    = "GMsearch_action";
      }
      namespace searchdefaultbible {
        const QString tooltip     = makeToolTip(i18n("Search default bible"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Opens the search dialog to search your default bible."));
        const QString icon        = "find";
        const KShortcut accel     = Qt::CTRL + Qt::Key_F;
        const char* actionName    = "GMsearchdefaultbible_action";
      }
    };

    namespace window { //Main menu->Window
      namespace loadProfile {
        const QString tooltip     = makeToolTip( i18n("Load a new profile") );
        const QString whatsthis   = makeWhatsThis(tooltip,i18n("Load a new profile") );
        const QString icon        = "view_sidetree";
        const char* actionName    = "windowLoadProfile_action";
      }
      namespace saveProfile {
        const QString tooltip     = makeToolTip(i18n("Save current profile"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Save current profile"));
        const QString icon        = "view_sidetree";
        const char* actionName    = "windowSaveProfile_action";
      }
      namespace saveToNewProfile {
        const QString tooltip     = makeToolTip(i18n("Save as a new profile"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Save the current settings as a new profile."));
        const QString icon        = "view_sidetree";
        const KShortcut accel     = KShortcut();        
        const char* actionName    = "windowSaveToNewProfile_action";
      }
      namespace setupProfiles {
        const QString tooltip     = makeToolTip(i18n("Edit profiles"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Create new, delete or rename existing profiles."));
        const QString icon        = "view_sidetree";
        const KShortcut accel     = Qt::CTRL + Qt::Key_A;
        const char* actionName    = "windowEditProfiles_action";
      }
      namespace showFullscreen {
        const QString tooltip     = makeToolTip(i18n("Toggle fullscreen mode"));
        const QString whatsthis   = makeWhatsThis(tooltip,i18n("Toggle fullscreen mode"));
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
        const QString whatsthis   = makeWhatsThis(tooltip,i18n("Automatically tile the display windows."));
        const QString icon        = "bt_tile_auto";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_T;
        const char* actionName    = "windowAutoTile_action";
      }
      namespace autoCascade {
        const QString tooltip     = makeToolTip(i18n("Automatically cascade windows"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Automatically cascade the opened display windows."));
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
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the BibleTime tool bar editor."));
      }
      namespace optionsDialog { // available as KStdAction
        const QString tooltip     = makeToolTip(i18n("Options"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open BibleTime's options dialog where you can set most of the preferences."));
      };
    };

    namespace help { //Main menu->Help
      namespace handbook {
        const QString tooltip     = makeToolTip(i18n("BibleTime's handbook"));
        const QString whatsthis   = makeWhatsThis(tooltip,i18n("Open KDE's helpbrowser with BibleTime's handbook.<BR>The handbook explains how to use BibleTime in the best way."));
        const QString icon        = "contents";
        const KShortcut accel     = KShortcut();
        const char* actionName    = "helpHandbook_action";
      };
      namespace installation {
        const QString tooltip     = makeToolTip(i18n("BibleTime's installation documentation"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open KDE's helpbrowser with BibleTime's installation instructions.<BR>The installation instructions explain how to set up BibleTime and the text modules."));
        const QString icon        = "contents";
        const KShortcut accel     = KShortcut();
        const char* actionName    = "helpInstallation_action";
      };
      namespace bibleStudyHowTo {
        const QString tooltip     = makeToolTip(i18n("BibleTime's Bible study HowTo"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open KDE's helpbrowser with the Bible study HowTo included with BibleTime.<BR>This HowTo is an introduction on how to study the Bible in an efficient way."));
        const QString icon        = "contents";
        const KShortcut accel     = KShortcut();
        const char* actionName    = "helpHowTo_action";
      };
      namespace whatsThis { // available as KStdAction
        const QString tooltip     = makeToolTip(i18n("Enter \"What's this?\" mode"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Switch into the <B>\"What's this?\"</B> mode.<BR>All important elements of BibleTime's GUI offer a \"What's this?\" help message."));
      };
      namespace bugreport { // available as KStdAction
        const QString tooltip     = makeToolTip(i18n("Send bugreport"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Send a bugreport to the developers of BibleTime!"));
      };
      namespace dailyTip {
        const QString tooltip     = makeToolTip(i18n("Show daily tips at startup"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Show a daily tip.<BR>The tips contain important Bible quotations and helpful tips for using BibleTime."));
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
      const QString whatsthis   = makeWhatsThis(tooltip, i18n("Starts to search the text in each of the chosen modules."));
    };
    namespace cancelSearchButton {
      const QString tooltip     = makeToolTip(i18n("Cancel searching"));
      const QString whatsthis   = makeWhatsThis(tooltip, i18n("Stops the active search."));
    };

    namespace options {
      namespace moduleChooserDialog {
        const QString tooltip     = makeToolTip(i18n(""));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n(""));
      };
      namespace moduleChooserButton {
        const QString tooltip     = makeToolTip(i18n("Choose modules"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Opens a dialog to choose modules for the search."));
      };
      namespace searchedText {
        const QString tooltip     = makeToolTip(i18n("Search text"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Enter here the text you want to search in the chosen modules."));
      };
      namespace searchType {
        namespace multipleWords_and {
          const QString tooltip     = makeToolTip(i18n("Multiple words (AND)"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Treats the search text as multiple words. A text must contain all of the words to match. The order of the words is unimportant."));
        };
        namespace multipleWords_or {
          const QString tooltip     = makeToolTip(i18n("Multiple words (OR)"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Treats the search text as multiple words. A text must contain one or more words of to match. The order is unimportant."));
        };
        namespace exactMatch {
          const QString tooltip     = makeToolTip(i18n("Exact match"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("The search text will be exactly used as entered."));
        };
        namespace regExp {
          const QString tooltip     = makeToolTip(i18n("Regular expression"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("The search string is treated as a GNU regular expression. See the BibleTime handbook for an explanation of regular expressions."));
        };
      };

      namespace searchOptions {
        namespace caseSensitive {
          const QString tooltip     = makeToolTip(i18n("Case sensitive matching"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("If you choose this option the search will distinguish between upper and lowercase characters. Refer to BibleTime's handbook for more help on this option."));
        };
      };
      namespace chooseScope {
        const QString tooltip     = makeToolTip(i18n("Choose a search scope"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Choose a scope from the list of available ones. Select the first item to use no scope, the second one is to use each module's last search result as search scope. The others are user defined search scopes."));
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
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Change the search ranges of the selected search scope item. Have a look at the predefined search scopes to see how search ranges are build."));
        };
        namespace parsedResult {
          const QString tooltip     = makeToolTip(i18n("Parsed search ranges"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Contains the search ranges which will be used by BibleTime for the search."));
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
          const QString icon = "filesace";
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
          const QString icon = "filesace";
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
      const QString icon = "bt_displayconfig";
    };
    namespace displaySettings {
      const QString icon = "bt_displaytranslit";
    };
    
    namespace biblewindow {
     namespace nextBook {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = Qt::CTRL + Qt::Key_B;
      }
      namespace previousBook {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = Qt::CTRL + Qt::SHIFT + Qt::Key_B;
      }
      namespace nextChapter {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = Qt::CTRL + Qt::Key_H;
      }
      namespace previousChapter {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = Qt::CTRL + Qt::SHIFT + Qt::Key_H;
      }
      namespace nextVerse {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = Qt::CTRL + Qt::Key_V;
      }
      namespace previousVerse {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = Qt::CTRL + Qt::SHIFT + Qt::Key_V;
      }
      namespace copyMenu {
        const QString icon = "";//CResMgr::displaywindows::lexiconWindow::copyMenu::icon;
      };
      namespace saveMenu {
        const QString icon = "";//CResMgr::displaywindows::lexiconWindow::saveMenu::icon;
      }
      namespace printMenu {
        const QString icon = "";//CResMgr::displaywindows::lexiconWindow::printMenu::icon;
      }
    };
    namespace commentaryWindow {
     namespace nextBook {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = KShortcut();//CResMgr::displaywindows::biblewindow::nextBook::accel;
      }
      namespace previousBook {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = KShortcut();//CResMgr::displaywindows::biblewindow::previousBook::accel;
      }
      namespace nextChapter {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = KShortcut(); //CResMgr::displaywindows::biblewindow::nextChapter::accel;
      }
      namespace previousChapter {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = KShortcut();;//CResMgr::displaywindows::biblewindow::previousChapter::accel;
      }
      namespace nextVerse {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = KShortcut();//CResMgr::displaywindows::biblewindow::nextVerse::accel;
      }
      namespace previousVerse {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = KShortcut();//CResMgr::displaywindows::biblewindow::previousVerse::accel;
      }

     namespace syncWindow {
        const QString icon = "bt_sync";
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = KShortcut();//CResMgr::displaywindows::biblewindow::nextBook::accel;
      }
      
    };
    namespace lexiconWindow {
      namespace nextEntry {
        const QString tooltip = "";
        const QString whatsthis = "";
        const KShortcut accel = KShortcut();
      }
      namespace previousEntry {
        const QString tooltip = "";
        const QString whatsthis = "";
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
      namespace toggleTree{
        const QString icon = "view_sidetree";
        const KShortcut accel = KShortcut();
      };
    };
    
    namespace writewindow {
      namespace saveText {
        const QString tooltip   = makeToolTip( i18n("Save the current text") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Save the curent text into the module. The old text will be lost.") );
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
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Toggles the bold status of the selected text.") );
        const QString icon      = "text_bold";
        const KShortcut accel   = KShortcut();
      };
      namespace italicText {
        const QString tooltip   = makeToolTip( i18n("Italic text") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Toggles the italic status of the selected text.") );
        const QString icon      = "text_italic";
        const KShortcut accel   = KShortcut();
      };
      namespace underlinedText {
        const QString tooltip   = makeToolTip( i18n("Underline text") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Toggles the underline status of the selected text.") );
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
        const QString tooltip   = makeToolTip( i18n("Align center") );
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
        const QString tooltip   = makeToolTip( i18n("Justification") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Justifies the text on the page.") );
        const QString icon      = "text_block";
        const KShortcut accel   = KShortcut();
      };

    };
  };

  namespace printing {
    namespace styleList {
      namespace newStyle {
        const QString tooltip = makeToolTip( i18n("New printing style") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Create a new printing style.<BR>The new style will be added to the list.") );
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
  };
  namespace settings {
    namespace startup {
      const QString icon = "bt_startconfig";
      namespace dailyTip {
        const QString tooltip   = makeToolTip( i18n("Show daily tip") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Activate this box to see a daily tip on startup."));
      };
      namespace showLogo {
        const QString tooltip   = makeToolTip( i18n("Show start-up logo") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Activate this to see the BibleTime logo on startup."));
      };
      namespace restoreWorkingArea {
        const QString tooltip   = makeToolTip( i18n("Restore the windows in the workspace on startup") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Save the windows in the workspace when BibleTime is closed<BR>and restore them on the next startup."));
      };
    };
    namespace fonts {
      const QString icon = "fonts";
            
      namespace typeChooser {
        const QString tooltip   = makeToolTip( i18n("Choose the area of application") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Use this box to select how the font is used and change the font in the font chooser."));
      };
    };
    namespace colors {
      const QString icon = "bt_displayconfig";
            
      namespace text {
        const QString tooltip = makeToolTip( i18n("Text color") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("The color used for all normal text in the display windows."));
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
        const QString tooltip   = makeToolTip( i18n("Strongs numbers") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("This color will be used for every Strong number in the display windows."));
      };
      namespace highlightedVerse {
        const QString tooltip   = makeToolTip( i18n("Color of the highlighted verse") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("This is the color of the highlighted verse used in display windows."));
      };
      namespace jesusWords {
        const QString tooltip   = makeToolTip( i18n("Jesus' words") );
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
        const QString tooltip   = makeToolTip( i18n("") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n(""));
      };
      namespace createNew {
        const QString tooltip   = makeToolTip( i18n("") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n(""));
      };
      namespace deleteCurrent {
        const QString tooltip   = makeToolTip( i18n("") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n(""));
      };
      namespace renameCurrent {
        const QString tooltip   = makeToolTip( i18n("") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n(""));
      };
    };
    namespace sword {
      const QString icon = "bt_swordconfig";
            
      namespace general {
        namespace cacheKeys {
          const QString tooltip   = makeToolTip( i18n("Use cache files for the lexicon entries") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Activate this to let BibleTime create index files for the lexicon entries. This significantly increases the speed of opening large lexicon modules, but does consume some disk space. Recommended."));
        };
        namespace scrolling {
          const QString tooltip   = makeToolTip( i18n("Use down arrow to scroll to next verse") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Check this to use the down arrow to scroll to the next verse."));
        };
        namespace language {
          const QString tooltip   = makeToolTip( i18n("List of languages") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Contains the languages which can be used for the international booknames."));
        };
      };
      namespace modules {
        namespace bible {
          const QString tooltip   = makeToolTip( i18n("The default bible used in the workspace area") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The default Bible is used when a hyperlink into a Bible is clicked"));
        };
        namespace commentary {
          const QString tooltip   = makeToolTip( i18n("The default commentary used in the workspace area") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The default commentary is used when a hyperlink into a commentary is clicked"));
        };
        namespace lexicon {
          const QString tooltip   = makeToolTip( i18n("The default lexicon used in the workspace area") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("The default lexicon is used when a hyperlink into a lexicon is clicked"));
        };
        namespace dailyDevotional {
          const QString tooltip   = makeToolTip( i18n("") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n(""));
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
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Show line breaks in the module's texts."));
        };
        namespace verseNumbers {
          const QString tooltip   = makeToolTip( i18n("Verse numbers") );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Show verse numbers before each verse in Bibles."));
        };
        namespace footnotes {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::footnotes) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Shows footnotes embedded in the module's text."));
        };
        namespace strongsNumbers {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::strongNumbers) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n("Show strongs numbers embedded in the text of modules which support this feature."));
        };
        namespace morphTags {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::morphTags) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n(""));
        };
        namespace headings {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::headings) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n(""));
        };
        namespace lemmas {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::lemmas) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n(""));
        };
        namespace hebrewVowelPoints {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::hebrewPoints) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n(""));
        };
        namespace hebrewCantillation {
           const QString tooltip  = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::hebrewCantillation) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n(""));
        };
        namespace greekAccents {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::greekAccents) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n(""));
        };
        namespace textualVariants {
          const QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::textualVariants) );
          const QString whatsthis = makeWhatsThis( tooltip, i18n(""));
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
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Opens the search dialog to search in the currently opened modules."));
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

