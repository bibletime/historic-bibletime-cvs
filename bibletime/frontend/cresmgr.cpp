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


namespace CResMgr {
  namespace mainMenu { //Main menu
    namespace file { //Main menu->File
      namespace clearQueue {
        const QString tooltip    = CResMgr::makeToolTip( i18n("Clear the printing queue") );
        const QString whatsthis  = CResMgr::makeWhatsThis( tooltip, i18n("Clear the print queue of BibleTime. All items will be removed from the list and the print button will be disabled.") );
        const QString icon       = QString::fromLatin1("queue");
        const KShortcut accel    = KShortcut();
        const char* actionName   = "fileClearQueue_action";
      }
      namespace print { //a standard action
        const QString tooltip     = makeToolTip( i18n("Open the printerdialog") );
        const QString whatsthis   = makeWhatsThis( tooltip, i18n("Open the printer dialog of BibleTime.  Here you can edit the print queue and assign styles to the items.") );
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
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Opens the searchdialog to search in the currently opened modules."));
        const QString icon        = "find";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_M;
        const char* actionName    = "GMsearch_action";
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
      namespace setupProfiles {
        const QString tooltip     = makeToolTip(i18n("Edit profiles"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Create new and delete or rename existing profiles."));
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
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open BibleTime's optionsdialog where you can set most of the preferences."));
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
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open KDE's helpbrowser with the Bible study HowTo included with BibleTime.<BR>This HowTo is an introduction how to study the Bible in an efficient way."));
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
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Show a daily tip.<BR>The tips contain important Bible quotations and helpful usage tips for BibleTime."));
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
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Treats the search text as multiple words. A text must contain all of the words to match! The order of the words is unimportant."));
        };
        namespace multipleWords_or {
          const QString tooltip     = makeToolTip(i18n("Multiple words (OR)"));
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Treats the search text as multiple words. A text must contain only one or more words of the search text. The order is unimportant."));
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
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("If you choose this option the search will decide between upper- and lowercase characters. Refer to BibleTime's handbook for more help on this option."));
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
          const QString whatsthis   = makeWhatsThis(tooltip, i18n("Add a new search scope. At first enter a good name, then edit the search ranges."));
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
      };
      namespace foundItems {
        const QString tooltip     = makeToolTip(i18n("Found items of selected module"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("This list contains the search result of the selected module."));
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
    namespace biblewindow {

    };
    namespace commentarywindow {

    };
    namespace lexiconwindow {

    };
    namespace writewindow {
      namespace saveText {
        const QString tooltip   = makeToolTip( i18n("Save the current text") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Save the curent text into the module. The old text will get lost.") );
        const QString icon      = "filesave";
        const KShortcut accel   = KShortcut();
      };
      namespace restoreText {
        const QString tooltip   = makeToolTip( i18n("Restore the old text") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Loads the old text from the module and loads it into the edit area. The unsaved text will get lost.") );
        const QString icon      = "undo";
        const KShortcut accel   = KShortcut();
      };
      namespace deleteEntry {
        const QString tooltip   = makeToolTip( i18n("Delete the current entry") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Deletes the current entry out of the module. The text will get lost.") );
        const QString icon      = "editdelete";
        const KShortcut accel   = KShortcut();
      };
    };
  };

  namespace printing {
  };
}
