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

//static KLocale* locale = KGlobal::locale();
const QString makeToolTip( const QString& text ) {
//  qWarning("translation of %s is %s", text.latin1(), i18n(text.latin1()).latin1());

  return text;
};
const QString makeWhatsThis( const QString& title, const QString& description )  {
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
      namespace clearQueue {
        QString tooltip;
//        QString tooltip    = makeToolTip( i18n("Clear print queue") );
        QString whatsthis  = makeWhatsThis( tooltip, i18n("Clear the print queue of BibleTime.") );
        const QString icon       = QString::fromLatin1("queue");
        const KShortcut accel    = KShortcut();
        const char* actionName   = "fileClearQueue_action";
      }
      namespace print { //a standard action
        QString tooltip     = makeToolTip( i18n("Open the printer dialog") );
        QString whatsthis   = makeWhatsThis( tooltip, i18n("Open the printer dialog of BibleTime, where you can edit the print queue, assign styles to the items and print them.") );
      }
      namespace quit { //a standard action
        QString tooltip     = makeToolTip( i18n("Close BibleTime") );
        QString whatsthis   = makeWhatsThis( tooltip, i18n("Close BibleTime and save the settings.") );
      }
    }

    namespace view { //Main menu->View
      namespace showMainIndex {
        QString tooltip     = makeToolTip(i18n("Show main index"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Toggle the main index view."));
        const QString icon        = "view_sidetree";
        const KShortcut accel     = Qt::Key_F9;
        const char* actionName    = "viewMainIndex_action";
      }
      namespace showToolBar { //a standard action
        QString tooltip     = makeToolTip(i18n("Show main toolbar"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Toggle the main toolbar view."));
      }
    }

    namespace mainIndex { //Main menu->Settings
      namespace search {
        QString tooltip     = makeToolTip(i18n("Search in open modules"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the search dialog to search in all modules that are currently open."));
        const QString icon        = "find";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_M;
        const char* actionName    = "GMsearch_action";
      }
      namespace searchdefaultbible {
        QString tooltip     = makeToolTip(i18n("Search default bible"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the search dialog to search in the default bible."));
        const QString icon        = "find";
        const KShortcut accel     = Qt::CTRL + Qt::Key_F;
        const char* actionName    = "GMsearchdefaultbible_action";
      }
    };

    namespace window { //Main menu->Window
      namespace loadProfile {
        QString tooltip     = makeToolTip( i18n("Load a session") );
        QString whatsthis   = makeWhatsThis(tooltip,i18n("Restore a saved BibleTime session.") );
        const QString icon        = "view_sidetree";
        const char* actionName    = "windowLoadProfile_action";
      }
      namespace saveProfile {
        QString tooltip     = makeToolTip(i18n("Save current session"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Save current BibleTime session so that it can be reused later."));
        const QString icon        = "view_sidetree";
        const char* actionName    = "windowSaveProfile_action";
      }
      namespace saveToNewProfile {
        QString tooltip     = makeToolTip(i18n("Save as new session"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Create and save a new session."));
        const QString icon        = "view_sidetree";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_S;
        const char* actionName    = "windowSaveToNewProfile_action";
      }
      namespace setupProfiles {
        QString tooltip     = makeToolTip(i18n("Edit sessions"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Create new, delete or rename existing sessions."));
        const QString icon        = "view_sidetree";
        const KShortcut accel     = KShortcut();
        const char* actionName    = "windowEditProfiles_action";
      }
      namespace showFullscreen {
        QString tooltip     = makeToolTip(i18n("Toggle fullscreen mode"));
        QString whatsthis   = makeWhatsThis(tooltip,i18n("Toggle fullscreen mode of the main window."));
        const QString icon        = "window_fullscreen";
        const KShortcut accel     = KShortcut();
        const char* actionName    = "windowFullscreen_action";
      }
      namespace tile {
        QString tooltip     = makeToolTip(i18n("Tile windows"));
        QString whatsthis   = makeWhatsThis(tooltip,i18n("Tile the open display windows."));
        const QString icon        = "bt_tile";
        const KShortcut accel     = Qt::CTRL + Qt::Key_T;
        const char* actionName    = "windowTile_action";
      }
      namespace cascade {
        QString tooltip     = makeToolTip(i18n("Cascade windows"));
        QString whatsthis   = makeWhatsThis(tooltip,i18n("Cascade the open display windows."));
        const QString icon        = "bt_cascade";
        const KShortcut accel     = Qt::CTRL + Qt::Key_A;
        const char* actionName    = "windowCascade_action";
      }
      namespace autoTile {
        QString tooltip     = makeToolTip(i18n("Automatically tile windows"));
        QString whatsthis   = makeWhatsThis(tooltip,i18n("Automatically tile the open display windows."));
        const QString icon        = "bt_tile_auto";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_T;
        const char* actionName    = "windowAutoTile_action";
      }
      namespace autoCascade {
        QString tooltip     = makeToolTip(i18n("Automatically cascade windows"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Automatically cascade the open display windows."));
        const QString icon        = "bt_cascade_auto";
        const KShortcut accel     = Qt::CTRL + Qt::ALT + Qt::Key_A;
        const char* actionName    = "windowAutoCascade_action";
      }
      namespace closeAll {
        QString tooltip     = makeToolTip(i18n("Close all windows"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Close all open display windows."));
        const QString icon        = "fileclose";
        const KShortcut accel     = Qt::CTRL + Qt::Key_K;
        const char* actionName    = "windowCloseAll_action";
      }
    }

    namespace settings { //Main menu->Settings
      namespace editToolBar { // available as KStdAction
        QString tooltip     = makeToolTip(i18n("Edit toolbar"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Open BibleTime's toolbar editor."));
      }
      namespace optionsDialog { // available as KStdAction
        QString tooltip     = makeToolTip(i18n("Configure BibleTime"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the dialog to set most of BibleTime's preferences."));
      };
    };

    namespace help { //Main menu->Help
      namespace handbook {
        QString tooltip     = makeToolTip(i18n("BibleTime's handbook"));
        QString whatsthis   = makeWhatsThis(tooltip,i18n("Open BibleTime's handbook in the KDE helpbrowser.<BR>The handbook explains how to use BibleTime in the best way."));
        const QString icon        = "contents";
        const KShortcut accel     = Qt::Key_F1;
        const char* actionName    = "helpHandbook_action";
      };
      namespace bibleStudyHowTo {
        QString tooltip     = makeToolTip(i18n("BibleTime's Bible study HowTo"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the Bible study HowTo included with BibleTime in the KDE helpbrowser.<BR>This HowTo is an introduction on how to study the Bible in an efficient way."));
        const QString icon        = "contents";
        const KShortcut accel     = Qt::Key_F2;
        const char* actionName    = "helpHowTo_action";
      };
      namespace whatsThis { // available as KStdAction
        QString tooltip     = makeToolTip(i18n("Enter \"What's this?\" mode"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Switch into the <B>\"What's this?\"</B> mode.<BR>All important elements of BibleTime's GUI offer a \"What's this?\" help message."));
      };
      namespace bugreport { // available as KStdAction
        QString tooltip     = makeToolTip(i18n("Send bugreport"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Send a bugreport to the developers of BibleTime."));
      };
      namespace dailyTip {
        QString tooltip     = makeToolTip(i18n("Show daily tips at startup"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Show a daily tip each time BibleTime starts.<BR>The tips contain important Bible quotations and helpful tips for using BibleTime."));
        const QString icon        = "idea";
        const KShortcut accel     = KShortcut();
        const char* actionName    = "helpDailyTip_action";
      };
      namespace aboutBibleTime { // available as KStdAction
        QString tooltip     = makeToolTip(i18n("About BibleTime"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Show detailed information about BibleTime."));
      };
      namespace aboutKDE { // available as KStdAction
        QString tooltip     = makeToolTip(i18n("About KDE"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Show detailed information about the KDE project."));
      };
    };
  }  //end of main menu

  namespace searchdialog {
    const QString icon = "find";

    namespace searchButton {
      QString tooltip     = makeToolTip(i18n("Start the search"));
      QString whatsthis   = makeWhatsThis(tooltip, i18n("Start to search the text in each of the chosen modules."));
    };
    namespace cancelSearchButton {
      QString tooltip     = makeToolTip(i18n("Stop search"));
      QString whatsthis   = makeWhatsThis(tooltip, i18n("Stop the active search."));
    };

    namespace options {
      namespace moduleChooserButton {
        QString tooltip     = makeToolTip(i18n("Choose modules"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Open a dialog to choose modules for the search."));
      };
      namespace searchedText {
        QString tooltip     = makeToolTip(i18n("Search text"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Enter the text you want to search in the chosen modules here."));
      };
      namespace searchType {
        namespace multipleWords_and {
          QString tooltip     = makeToolTip(i18n("Multiple words (AND)"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("Treat the search text as multiple words. A text must contain all of the words to match. The order of the words is unimportant."));
        };
        namespace multipleWords_or {
          QString tooltip     = makeToolTip(i18n("Multiple words (OR)"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("Treat the search text as multiple words. A text must contain one or more words of to match. The order is unimportant."));
        };
        namespace exactMatch {
          QString tooltip     = makeToolTip(i18n("Exact match"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("The search text will be used exactly as entered."));
        };
        namespace regExp {
          QString tooltip     = makeToolTip(i18n("Regular expression"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("Treat the search string as a GNU regular expression. The BibleTime handbook contains an introduction to regular expressions."));
        };
      };

      namespace searchOptions {
        namespace caseSensitive {
          QString tooltip     = makeToolTip(i18n("Case sensitive matching"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("If you choose this option the search will distinguish between upper and lowercase characters."));
        };
      };
      namespace chooseScope {
        QString tooltip     = makeToolTip(i18n("Choose a search scope"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Choose a scope from the list. Select the first item to use no scope, the second one is to use each module's last search result as search scope. The others are user defined search scopes."));
      };
      namespace scopeEditor {
        namespace rangeList {
          QString tooltip     = makeToolTip(i18n("Available search scopes"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("Select an item from the list to edit the search scope."));
        };
        namespace nameEdit {
          QString tooltip     = makeToolTip(i18n("Search scope name"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("Change the name of the selected search scope."));
        };
        namespace editRange {
          QString tooltip     = makeToolTip(i18n("Edit search scope ranges"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("Change the search ranges of the selected search scope item. Have a look at the predefined search scopes to see how search ranges are constructed."));
        };
        namespace parsedResult {
          QString tooltip     = makeToolTip(i18n("Parsed search ranges"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("Contains the search ranges which will be used for the search."));
        };
        namespace addNewRange {
          QString tooltip     = makeToolTip(i18n("Add a new search scope"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("Add a new search scope. First enter an appropriate name, then edit the search ranges."));
        };
        namespace deleteCurrentRange {
          QString tooltip     = makeToolTip(i18n("Delete current search scope"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("Deletes the selected search scope. If you close the dialog using Cancel the settings won't be saved."));
        };

      };
    }
    namespace result {
      namespace moduleList {
        QString tooltip     = makeToolTip(i18n("Used modules"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("The list of modules chosen for the search."));

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
        QString tooltip     = makeToolTip(i18n("Found items of selected module"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("This list contains the search result of the selected module."));

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
        QString tooltip     = makeToolTip(i18n("Text preview"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("The text preview of the selected search result item."));
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
        QString tooltip = makeToolTip( i18n("Scroll through the list") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("This button is useful to scroll through the entries of the list. Press the button and move the mouse to increase or decrease the item."));

      };
    };
    namespace bibleWindow {
      namespace bookList {
        QString tooltip = makeToolTip( i18n("List of books") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("This list contains the books which are available in this module."));
      };
      namespace nextBook {
        QString tooltip = makeToolTip( i18n("Show the next book") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("Show the next book of this module."));
        const KShortcut accel = Qt::CTRL + Qt::Key_B;
       }
      namespace previousBook {
        QString tooltip = makeToolTip( i18n("Show the previous book") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("Show the previous book of this module."));
        const KShortcut accel = Qt::CTRL + Qt::SHIFT + Qt::Key_B;
      }


      namespace chapterList {
        QString tooltip = makeToolTip( i18n("List of chapters") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("This list contains the chapters which are available in the current book."));
      };
      namespace nextChapter {
        QString tooltip = makeToolTip( i18n("Show the next chapter") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("Show the previous chapter of the module."));
        const KShortcut accel = Qt::CTRL + Qt::Key_H;
      }
      namespace previousChapter {
        QString tooltip = makeToolTip( i18n("Show the previous chapter") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("Show the next chapter of the module."));
        const KShortcut accel = Qt::CTRL + Qt::SHIFT + Qt::Key_H;
      }

      namespace verseList {
        QString tooltip = makeToolTip( i18n("List of verses") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("This list contains the verses which are available in the current chapter."));
      };
      namespace nextVerse {
        QString tooltip = makeToolTip( i18n("Jump to the next verse") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("In  Bible texts, the next verse will be highlighted. In commentaries, the next entry will be shown."));
        const KShortcut accel = Qt::CTRL + Qt::Key_V;
      }
      namespace previousVerse {
        QString tooltip = makeToolTip( i18n("Jump to the previous verse") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("In Bible texts, the previous verse will be highlighted. In commentaries, the previous entry will be shown."));
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
         QString tooltip = makeToolTip(i18n("Synchronize with Bible windows"));
         QString whatsthis = makeWhatsThis(tooltip, i18n("Synchronize the displayed entry of this module with the active Bible window."));
         const QString icon = "bt_sync";
         const KShortcut accel = KShortcut();//CResMgr::displaywindows::biblewindow::nextBook::accel;
       }

    };
    namespace lexiconWindow {
      namespace entryList {
        QString tooltip = makeToolTip( i18n("List of entries") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("This list contains the entries of the current module."));
      };
      namespace nextEntry {
        QString tooltip = makeToolTip( i18n("Show the next entry") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("The next entry of the module will be shown."));
        const KShortcut accel = KShortcut();
      }
      namespace previousEntry {
        QString tooltip = makeToolTip( i18n("Show the previous entry") );
        QString whatsthis = makeWhatsThis(tooltip, i18n("The previous entry of the module will be shown."));
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
        QString tooltip   = makeToolTip( i18n("Save the current text") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Save the curent text into the module. The old text will be overwritten.") );
        const QString icon      = "filesave";
        const KShortcut accel   = KShortcut();
      };
      namespace restoreText {
        QString tooltip   = makeToolTip( i18n("Restore the old text") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Loads the old text from the module and loads it into the edit area. The unsaved text will be lost.") );
        const QString icon      = "undo";
        const KShortcut accel   = KShortcut();
      };
      namespace deleteEntry {
        QString tooltip   = makeToolTip( i18n("Delete the current entry") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Deletes the current entry out of the module. The text will be lost.") );
        const QString icon      = "editdelete";
        const KShortcut accel   = KShortcut();
      };

      //formatting buttons
      namespace boldText {
        QString tooltip   = makeToolTip( i18n("Bold text") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Toggle bold formatting of the selected text.") );
        const QString icon      = "text_bold";
        const KShortcut accel   = KShortcut();
      };
      namespace italicText {
        QString tooltip   = makeToolTip( i18n("Italic text") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Toggle italic formatting of the selected text.") );
        const QString icon      = "text_italic";
        const KShortcut accel   = KShortcut();
      };
      namespace underlinedText {
        QString tooltip   = makeToolTip( i18n("Underline text") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Toggle underlined formatting of the selected text.") );
        const QString icon      = "text_under";
        const KShortcut accel   = KShortcut();
      };

      namespace alignLeft {
        QString tooltip   = makeToolTip( i18n("Align left") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("The text will be aligned on the left side of the page.") );
        const QString icon      = "text_left";
        const KShortcut accel   = KShortcut();
      };
      namespace alignCenter {
        QString tooltip   = makeToolTip( i18n("Center") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Centers the text horizontally.") );
        const QString icon      = "text_center";
        const KShortcut accel   = KShortcut();
      };
      namespace alignRight {
        QString tooltip   = makeToolTip( i18n("Align right") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Aligns the text on the right side of the page.") );
        const QString icon      = "rightjust";
        const KShortcut accel   = KShortcut();
      };
      namespace alignJustify {
        QString tooltip   = makeToolTip( i18n("Justify") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Justifies the text on the page.") );
        const QString icon      = "text_block";
        const KShortcut accel   = KShortcut();
      };

      namespace fontFamily {
        QString tooltip   = makeToolTip( i18n("Set the font") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Choose a new font for the selected text.") );
      };
      namespace fontSize {
        QString tooltip   = makeToolTip( i18n("Set the font size") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Choose a new font size for the selected text.") );
      };
      namespace fontColor {
        QString tooltip   = makeToolTip( i18n("Set the color") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Choose a new color for the selected text.") );
      };

    };
  };

  namespace printing {
    namespace itemPage {
      namespace styleChooser {
        QString tooltip = makeToolTip(i18n("Styles"));
        QString whatsthis = makeWhatsThis(tooltip,i18n("This list contains the available print styles."));
      };
      namespace itemList {
        QString tooltip = makeToolTip(i18n("List of print items"));
        QString whatsthis = makeWhatsThis(tooltip,i18n("This is the list which contains the items to be printed."));
      };
      namespace itemUp {
        const QString icon = "up";
        QString tooltip = makeToolTip(i18n("Move item up"));
        QString whatsthis = makeWhatsThis(tooltip,i18n("Move the selected item up one line."));
      };
      namespace itemDown {
        const QString icon = "down";
        QString tooltip = makeToolTip(i18n("Move item down"));
        QString whatsthis = makeWhatsThis(tooltip,i18n("Move the selected item down one line."));
      };
      namespace itemDelete {
        const QString icon = "edittrash";
        QString tooltip = makeToolTip(i18n("Delete items"));
        QString whatsthis = makeWhatsThis(tooltip,i18n("Delete the selected items."));
      };
    };

    namespace styleList {
      namespace newStyle {
        QString tooltip = makeToolTip( i18n("New printing style") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Create a new printing style.") );
        const QString icon = "filenew";
      };
      namespace deleteStyle {
        QString tooltip = makeToolTip( i18n("Delete printing style") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Remove the selected printing style from the list.") );
        const QString icon = "editdelete";
      };
      namespace editStyle {
        QString tooltip = makeToolTip( i18n("Edit printing style") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Change the properties of the selected printing style.") );
        const QString icon = "pencil";
      };
    };
    namespace printerSettings {
      namespace margins {
        QString whatsthis = makeWhatsThis( makeToolTip(i18n("Size of page borders")), i18n("This group contains the page borders of the page measured in millimeters.") );

        namespace upper {
          QString tooltip = makeToolTip( i18n("Upper border") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The size of the border on the top of the paper.") );
        };
        namespace lower {
          QString tooltip = makeToolTip( i18n("Lower border") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The size of the border on the bottom of the paper.") );
        };
        namespace left {
          QString tooltip = makeToolTip( i18n("Left border") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The size of the border on the left side of the paper.") );
        };
        namespace right {
          QString tooltip = makeToolTip( i18n("Right border") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The size of the border on the right side of the paper.") );
        };
      };
    };
  };
  namespace settings {
    namespace startup {
      const QString icon = "bt_startconfig";
      namespace dailyTip {
        QString tooltip   = makeToolTip( i18n("Show daily tip") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Activate this box to see a daily tip on startup."));
      };
      namespace showLogo {
        QString tooltip   = makeToolTip( i18n("Show startup logo") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Activate this to see the BibleTime logo on startup."));
      };
      namespace restoreWorkingArea {
        QString tooltip   = makeToolTip( i18n("Restore the last BibleTime session on startup") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Save the user's session when BibleTime is closed and restore it on the next startup."));
      };
    };
    namespace fonts {
      const QString icon = "fonts";

      namespace typeChooser {
        QString tooltip   = makeToolTip( i18n("Choose a language") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("The font selection below will apply to all texts in this language."));
      };
    };
    namespace colors {
      const QString icon = "bt_displayconfig";

      namespace text {
        QString tooltip = makeToolTip( i18n("Text color") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("The color used for normal text in the display windows."));
      };
      namespace background {
        QString tooltip = makeToolTip( i18n("Color of the background") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("This is the background color used in display windows."));
      };
      namespace links {
        QString tooltip   = makeToolTip( i18n("Link color") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("The color used for all references into Sword modules."));
      };
      namespace footnotes {
        QString tooltip   = makeToolTip( i18n("Footnote color") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("This color will be used for all footnotes in the display windows."));
      };
      namespace strongNumbers {
        QString tooltip   = makeToolTip( i18n("Strong's Numbers") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("This color will be used for every Strong's Number in the display windows."));
      };
      namespace highlightedVerse {
        QString tooltip   = makeToolTip( i18n("Color of the highlighted verse") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("This is the color of the highlighted verse used in display windows."));
      };
      namespace jesusWords {
        QString tooltip   = makeToolTip( i18n("Words of Jesus") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("All words spoken by Jesus will appear in this color."));
      };
      namespace morphTags {
        QString tooltip = makeToolTip( i18n("Morphological tags") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("All morphological tags will appear in this color."));
      };
    };
    namespace profiles {
      const QString icon = "view_sidetree";

      namespace list {
        QString tooltip   = makeToolTip( i18n("List of sessions") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("The list of available sessions."));
      };
      namespace createNew {
        QString tooltip   = makeToolTip( i18n("Create new session") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Create a new, empty session with default settings."));
      };
      namespace deleteCurrent {
        QString tooltip   = makeToolTip( i18n("Delete session") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Delete the selected session."));
      };
      namespace renameCurrent {
        QString tooltip   = makeToolTip( i18n("Rename session") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Rename the selected session."));
      };
    };
    namespace sword {
      const QString icon = "bt_swordconfig";

      namespace general {
        namespace cacheKeys {
          QString tooltip   = makeToolTip( i18n("Use cache files for the lexicon entries") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Activate this to let BibleTime create index files for the lexicon entries. This increases the speed of opening large lexicon modules significantly, but does consume some disk space. Recommended."));
        };
        namespace scrolling {
          QString tooltip   = makeToolTip( i18n("Use down arrow to scroll to next verse") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Check this to use the down arrow to scroll to the next verse."));
        };
        namespace language {
          QString tooltip   = makeToolTip( i18n("List of languages") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Contains the languages which can be used for the biblical booknames."));
        };
      };
      namespace modules {
        namespace bible {
          QString tooltip   = makeToolTip( i18n("The default bible used in the workspace area") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The default Bible is used when a hyperlink into a Bible is clicked."));
        };
        namespace commentary {
          QString tooltip   = makeToolTip( i18n("The default commentary used in the workspace area") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The default commentary is used when a hyperlink into a commentary is clicked."));
        };
        namespace lexicon {
          QString tooltip   = makeToolTip( i18n("The default lexicon used in the workspace area") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The default lexicon is used when a hyperlink into a lexicon is clicked."));
        };
        namespace dailyDevotional {
          QString tooltip   = makeToolTip( i18n("The default daily devotional module") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The default devotional will be used to display a short start up devotional."));
        };
        namespace hebrewStrongs {
          QString tooltip   = makeToolTip( i18n("The default Hebrew lexicon used in the workspace area") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The default Hebrew lexicon is used when a hyperlink into a Hebrew lexicon is clicked."));
        };
        namespace greekStrongs {
          QString tooltip   = makeToolTip( i18n("The default Greek lexicon used in the workspace area") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The default Greek lexicon is used when a hyperlink into a Greek lexicon is clicked."));
        };
        namespace hebrewMorph {
          QString tooltip   = makeToolTip( i18n("The default Morphological lexicon for Hebrew texts used in the workspace area") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The standard morphological lexicon for Hebrew texts is used when a hyperlink of a morphological tag in a Hebrew text is clicked."));
        };
        namespace greekMorph {
          QString tooltip   = makeToolTip( i18n("The default Morphological lexicon for Greek texts used in the workspace area") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The standard morphological lexicon for Greek texts is used when a hyperlink of a morphological tag in a Greek text is clicked."));
        };
      };
      namespace filters {
        namespace lineBreaks {
          QString tooltip   = makeToolTip( i18n("Line breaks") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Insert line breaks after each verse."));
        };
        namespace verseNumbers {
          QString tooltip   = makeToolTip( i18n("Verse numbers") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Show the verse number before each verse in Bibles."));
        };
        namespace footnotes {
          QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::footnotes) );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Show footnotes embedded in the module's text."));
        };
        namespace strongsNumbers {
          QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::strongNumbers) );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Show Strong's Numbers embedded in the text of modules which support this feature."));
        };
        namespace morphTags {
          QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::morphTags) );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Show tags providing morphological information in the text of modules which support this feature."));
        };
        namespace headings {
          QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::headings) );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Show section headings in the text in the text of modules which support this feature."));
        };
        namespace lemmas {
          QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::lemmas) );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Show root words."));
        };
        namespace hebrewVowelPoints {
          QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::hebrewPoints) );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Show hebrew vowel points in the text."));
        };
        namespace hebrewCantillation {
           QString tooltip  = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::hebrewCantillation) );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Show hebrew cantillation marks in the text."));
        };
        namespace greekAccents {
          QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::greekAccents) );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Show greek accents in the text."));
        };
        namespace textualVariants {
          QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::textualVariants) );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Use different variants from the textual sources."));
        };
      };
    };
    namespace keys {
      const QString icon = "key_bindings";
    };
  };

    namespace mainIndex { //Main menu->Settings
      namespace search {
        QString tooltip     = makeToolTip(i18n("Search in open modules"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Opens the search dialog to search in the modules that are currently open."));
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
          using namespace clearQueue;
          tooltip    = makeToolTip( i18n("Clear print queue") );
          whatsthis  = makeWhatsThis( tooltip, i18n("Clear the print queue of BibleTime.") );
        };
        {
          using namespace print;
          tooltip     = makeToolTip( i18n("Open the printer dialog") );
          whatsthis   = makeWhatsThis( tooltip, i18n("Open the printer dialog of BibleTime, where you can edit the print queue, assign styles to the items and print them.") );
        };
        {
          using namespace quit;
          QString tooltip     = makeToolTip( i18n("Close BibleTime") );
          QString whatsthis   = makeWhatsThis( tooltip, i18n("Close BibleTime and save the settings.") );
        };

        {
          using namespace view;
          {
            using namespace showMainIndex;
            QString tooltip     = makeToolTip(i18n("Show main index"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Toggle the main index view."));
          }
          {
            using namespace showToolBar;
            QString tooltip     = makeToolTip(i18n("Show main toolbar"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Toggle the main toolbar view."));
          }
        }

        {
          using namespace mainMenu::mainIndex;
          {
            using namespace search;
            QString tooltip     = makeToolTip(i18n("Search in open modules"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the search dialog to search in all modules that are currently open."));
          }
          {
            using namespace searchdefaultbible;
            QString tooltip     = makeToolTip(i18n("Search default bible"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the search dialog to search in the default bible."));
          }
        };

        {
          using namespace window;
          {
            using namespace loadProfile;
            QString tooltip     = makeToolTip( i18n("Load a session") );
            QString whatsthis   = makeWhatsThis(tooltip,i18n("Restore a saved BibleTime session.") );
          }
          {
            using namespace saveProfile;
            QString tooltip     = makeToolTip(i18n("Save current session"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Save current BibleTime session so that it can be reused later."));
          }
          {
            using namespace saveToNewProfile;
            QString tooltip     = makeToolTip(i18n("Save as new session"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Create and save a new session."));
          }
          {
            using namespace setupProfiles;
            QString tooltip     = makeToolTip(i18n("Edit sessions"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Create new, delete or rename existing sessions."));
          }
          {
            using namespace showFullscreen;
            QString tooltip     = makeToolTip(i18n("Toggle fullscreen mode"));
            QString whatsthis   = makeWhatsThis(tooltip,i18n("Toggle fullscreen mode of the main window."));
          }
          {
            using namespace tile;
            QString tooltip     = makeToolTip(i18n("Tile windows"));
            QString whatsthis   = makeWhatsThis(tooltip,i18n("Tile the open display windows."));
          }
          {
            using namespace cascade;
            QString tooltip     = makeToolTip(i18n("Cascade windows"));
            QString whatsthis   = makeWhatsThis(tooltip,i18n("Cascade the open display windows."));
          }
          {
            using namespace autoTile;
            QString tooltip     = makeToolTip(i18n("Automatically tile windows"));
            QString whatsthis   = makeWhatsThis(tooltip,i18n("Automatically tile the open display windows."));
          }
          {
            using namespace autoCascade;
            QString tooltip     = makeToolTip(i18n("Automatically cascade windows"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Automatically cascade the open display windows."));
          }
          {
            using namespace closeAll;
            QString tooltip     = makeToolTip(i18n("Close all windows"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Close all open display windows."));
          }
        }

        {
          using namespace mainMenu::settings;
          {
            using namespace editToolBar;
            QString tooltip     = makeToolTip(i18n("Edit toolbar"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Open BibleTime's toolbar editor."));
          }
          {
            using namespace optionsDialog;
            QString tooltip     = makeToolTip(i18n("Configure BibleTime"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the dialog to set most of BibleTime's preferences."));
          };
        };

        {
          using namespace help;
          {
            using namespace handbook;
            QString tooltip     = makeToolTip(i18n("BibleTime's handbook"));
            QString whatsthis   = makeWhatsThis(tooltip,i18n("Open BibleTime's handbook in the KDE helpbrowser.<BR>The handbook explains how to use BibleTime in the best way."));
          };
          {
            using namespace bibleStudyHowTo;
            QString tooltip     = makeToolTip(i18n("BibleTime's Bible study HowTo"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the Bible study HowTo included with BibleTime in the KDE helpbrowser.<BR>This HowTo is an introduction on how to study the Bible in an efficient way."));
          };
          {
            using namespace whatsThis;
            QString tooltip     = makeToolTip(i18n("Enter \"What's this?\" mode"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Switch into the <B>\"What's this?\"</B> mode.<BR>All important elements of BibleTime's GUI offer a \"What's this?\" help message."));
          };
          {
            using namespace bugreport;
            QString tooltip     = makeToolTip(i18n("Send bugreport"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Send a bugreport to the developers of BibleTime."));
          };
          {
            using namespace dailyTip;
            QString tooltip     = makeToolTip(i18n("Show daily tips at startup"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Show a daily tip each time BibleTime starts.<BR>The tips contain important Bible quotations and helpful tips for using BibleTime."));
          };
          {
            using namespace aboutBibleTime;
            QString tooltip     = makeToolTip(i18n("About BibleTime"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Show detailed information about BibleTime."));
          };
          {
            using namespace aboutKDE;
            QString tooltip     = makeToolTip(i18n("About KDE"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Show detailed information about the KDE project."));
          };
        };
      };
    };

    {
      using namespace searchdialog;
      {
        using namespace searchButton;
        QString tooltip     = makeToolTip(i18n("Start the search"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Start to search the text in each of the chosen modules."));
      };
      {
        using namespace cancelSearchButton;
        QString tooltip     = makeToolTip(i18n("Stop search"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Stop the active search."));
      };

      {
        using namespace options;
        {
          using namespace moduleChooserButton;
          QString tooltip     = makeToolTip(i18n("Choose modules"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("Open a dialog to choose modules for the search."));
        };
        {
          using namespace searchedText;
          QString tooltip     = makeToolTip(i18n("Search text"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("Enter the text you want to search in the chosen modules here."));
        };
        {
          using namespace searchType;
          {
            using namespace multipleWords_and;
            QString tooltip     = makeToolTip(i18n("Multiple words (AND)"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Treat the search text as multiple words. A text must contain all of the words to match. The order of the words is unimportant."));
          };
          {
            using namespace multipleWords_or;
            QString tooltip     = makeToolTip(i18n("Multiple words (OR)"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Treat the search text as multiple words. A text must contain one or more words of to match. The order is unimportant."));
          };
          {
            using namespace exactMatch;
            QString tooltip     = makeToolTip(i18n("Exact match"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("The search text will be used exactly as entered."));
          };
          {
            using namespace regExp;
            QString tooltip     = makeToolTip(i18n("Regular expression"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Treat the search string as a GNU regular expression. The BibleTime handbook contains an introduction to regular expressions."));
          };
        };

        {
          using namespace searchOptions;
          {
            using namespace caseSensitive;
            QString tooltip     = makeToolTip(i18n("Case sensitive matching"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("If you choose this option the search will distinguish between upper and lowercase characters."));
          };
        };
        {
          using namespace chooseScope;
          QString tooltip     = makeToolTip(i18n("Choose a search scope"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("Choose a scope from the list. Select the first item to use no scope, the second one is to use each module's last search result as search scope. The others are user defined search scopes."));
        };
        {
          using namespace scopeEditor;
          {
            using namespace rangeList;
            QString tooltip     = makeToolTip(i18n("Available search scopes"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Select an item from the list to edit the search scope."));
          };
          {
            using namespace nameEdit;
            QString tooltip     = makeToolTip(i18n("Search scope name"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Change the name of the selected search scope."));
          };
          {
            using namespace editRange;
            QString tooltip     = makeToolTip(i18n("Edit search scope ranges"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Change the search ranges of the selected search scope item. Have a look at the predefined search scopes to see how search ranges are constructed."));
          };
          {
            using namespace parsedResult;
            QString tooltip     = makeToolTip(i18n("Parsed search ranges"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Contains the search ranges which will be used for the search."));
          };
          {
            using namespace addNewRange;
            QString tooltip     = makeToolTip(i18n("Add a new search scope"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Add a new search scope. First enter an appropriate name, then edit the search ranges."));
          };
          {
            using namespace deleteCurrentRange;
            QString tooltip     = makeToolTip(i18n("Delete current search scope"));
            QString whatsthis   = makeWhatsThis(tooltip, i18n("Deletes the selected search scope. If you close the dialog using Cancel the settings won't be saved."));
          };
        };
      }
      {
        using namespace result;
        {
          using namespace moduleList;
          QString tooltip     = makeToolTip(i18n("Used modules"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("The list of modules chosen for the search."));
        };
        {
          using namespace foundItems;
          QString tooltip     = makeToolTip(i18n("Found items of selected module"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("This list contains the search result of the selected module."));
        };
        {
          using namespace textPreview;
          QString tooltip     = makeToolTip(i18n("Text preview"));
          QString whatsthis   = makeWhatsThis(tooltip, i18n("The text preview of the selected search result item."));
        };
      };
    };

    {
      using namespace displaywindows;
      {
        using namespace general;
        {
          using namespace scrollButton;
          QString tooltip = makeToolTip( i18n("Scroll through the list") );
          QString whatsthis = makeWhatsThis(tooltip, i18n("This button is useful to scroll through the entries of the list. Press the button and move the mouse to increase or decrease the item."));
        };
        using namespace bibleWindow;
        {
          using namespace bookList;
          QString tooltip = makeToolTip( i18n("List of books") );
          QString whatsthis = makeWhatsThis(tooltip, i18n("This list contains the books which are available in this module."));
        };
        {
          using namespace nextBook;
          QString tooltip = makeToolTip( i18n("Show the next book") );
          QString whatsthis = makeWhatsThis(tooltip, i18n("Show the next book of this module."));
         }
        {
          using namespace previousBook;
          QString tooltip = makeToolTip( i18n("Show the previous book") );
          QString whatsthis = makeWhatsThis(tooltip, i18n("Show the previous book of this module."));
        }
        {
          using namespace chapterList;
          QString tooltip = makeToolTip( i18n("List of chapters") );
          QString whatsthis = makeWhatsThis(tooltip, i18n("This list contains the chapters which are available in the current book."));
        };
        {
          using namespace nextChapter;
          QString tooltip = makeToolTip( i18n("Show the next chapter") );
          QString whatsthis = makeWhatsThis(tooltip, i18n("Show the previous chapter of the module."));
        }
        {
         using namespace previousChapter;
          QString tooltip = makeToolTip( i18n("Show the previous chapter") );
          QString whatsthis = makeWhatsThis(tooltip, i18n("Show the next chapter of the module."));
        }
        {
         using namespace verseList;
          QString tooltip = makeToolTip( i18n("List of verses") );
          QString whatsthis = makeWhatsThis(tooltip, i18n("This list contains the verses which are available in the current chapter."));
        };
        {
         using namespace nextVerse;
         QString tooltip = makeToolTip( i18n("Jump to the next verse") );
         QString whatsthis = makeWhatsThis(tooltip, i18n("In  Bible texts, the next verse will be highlighted. In commentaries, the next entry will be shown."));

        }
        { using namespace previousVerse;
          QString tooltip = makeToolTip( i18n("Jump to the previous verse") );
          QString whatsthis = makeWhatsThis(tooltip, i18n("In Bible texts, the previous verse will be highlighted. In commentaries, the previous entry will be shown."));
        }
      };
      {
        using namespace commentaryWindow;
        {
          using namespace syncWindow;
           QString tooltip = makeToolTip(i18n("Synchronize with Bible windows"));
           QString whatsthis = makeWhatsThis(tooltip, i18n("Synchronize the displayed entry of this module with the active Bible window."));
         }
      };
      {
        using namespace lexiconWindow;
        {
          using namespace entryList;
          QString tooltip = makeToolTip( i18n("List of entries") );
          QString whatsthis = makeWhatsThis(tooltip, i18n("This list contains the entries of the current module."));
        };
        {
          using namespace nextEntry;
          QString tooltip = makeToolTip( i18n("Show the next entry") );
          QString whatsthis = makeWhatsThis(tooltip, i18n("The next entry of the module will be shown."));
        };
        {
          using namespace previousEntry;
          QString tooltip = makeToolTip( i18n("Show the previous entry") );
          QString whatsthis = makeWhatsThis(tooltip, i18n("The previous entry of the module will be shown."));
        };
      };

    {
      using namespace writeWindow;
      {
        using namespace saveText;
        QString tooltip   = makeToolTip( i18n("Save the current text") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Save the curent text into the module. The old text will be overwritten.") );
      };
      {
        using namespace restoreText;
        QString tooltip   = makeToolTip( i18n("Restore the old text") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Loads the old text from the module and loads it into the edit area. The unsaved text will be lost.") );
      };
      {
        using namespace deleteEntry;
        QString tooltip   = makeToolTip( i18n("Delete the current entry") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Deletes the current entry out of the module. The text will be lost.") );
      };

      //formatting buttons
      {
        using namespace boldText;
        QString tooltip   = makeToolTip( i18n("Bold text") );
        QString whatsthis = makeWhatsThis( tooltip, i18n("Toggle bold formatting of the selected text.") );
      };
      {
        using namespace italicText;
        QString tooltip   = makeToolTip( i18n("Italic text") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Toggle italic formatting of the selected text.") );
        };
        {
          using namespace underlinedText;
          QString tooltip   = makeToolTip( i18n("Underline text") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Toggle underlined formatting of the selected text.") );
        };

        {
          using namespace alignLeft;
          QString tooltip   = makeToolTip( i18n("Align left") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The text will be aligned on the left side of the page.") );
        };
        {
          using namespace alignCenter;
          QString tooltip   = makeToolTip( i18n("Center") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Centers the text horizontally.") );
        };
        {
          using namespace alignRight;
          QString tooltip   = makeToolTip( i18n("Align right") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Aligns the text on the right side of the page.") );
        };
        {
          using namespace alignJustify;
          QString tooltip   = makeToolTip( i18n("Justify") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Justifies the text on the page.") );
        };

        {
          using namespace fontFamily;
          QString tooltip   = makeToolTip( i18n("Set the font") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Choose a new font for the selected text.") );
        };
        { using namespace fontSize;
          QString tooltip   = makeToolTip( i18n("Set the font size") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Choose a new font size for the selected text.") );
        };
        { using namespace fontColor;
          QString tooltip   = makeToolTip( i18n("Set the color") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Choose a new color for the selected text.") );
        };
      };
    };

    {
      using namespace printing;
      {
        using namespace itemPage;
        {
          using namespace styleChooser;
          QString tooltip = makeToolTip(i18n("Styles"));
          QString whatsthis = makeWhatsThis(tooltip,i18n("This list contains the available print styles."));
        };
        {
          using namespace itemList;
          QString tooltip = makeToolTip(i18n("List of print items"));
          QString whatsthis = makeWhatsThis(tooltip,i18n("This is the list which contains the items to be printed."));
        };
        {
          using namespace itemUp;
          QString tooltip = makeToolTip(i18n("Move item up"));
          QString whatsthis = makeWhatsThis(tooltip,i18n("Move the selected item up one line."));
        };
        {
          using namespace itemDown;
          QString tooltip = makeToolTip(i18n("Move item down"));
          QString whatsthis = makeWhatsThis(tooltip,i18n("Move the selected item down one line."));
        };
        {
          using namespace itemDelete;
          QString tooltip = makeToolTip(i18n("Delete items"));
          QString whatsthis = makeWhatsThis(tooltip,i18n("Delete the selected items."));
        };
      };

      {
        using namespace styleList;
        {
          using namespace newStyle;
          QString tooltip = makeToolTip( i18n("New printing style") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Create a new printing style.") );

        };
        {
          using namespace deleteStyle;
          QString tooltip = makeToolTip( i18n("Delete printing style") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Remove the selected printing style from the list.") );

        };
        {
          using namespace editStyle;
          QString tooltip = makeToolTip( i18n("Edit printing style") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Change the properties of the selected printing style.") );

        };
      };
      {
        using namespace printerSettings;
        {
          using namespace margins;
          QString whatsthis = makeWhatsThis( makeToolTip(i18n("Size of page borders")), i18n("This group contains the page borders of the page measured in millimeters.") );
          {
            using namespace upper;
            QString tooltip = makeToolTip( i18n("Upper border") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("The size of the border on the top of the paper.") );
          };
          {
            using namespace lower;
            QString tooltip = makeToolTip( i18n("Lower border") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("The size of the border on the bottom of the paper.") );
          };
          {
            using namespace left;
            QString tooltip = makeToolTip( i18n("Left border") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("The size of the border on the left side of the paper.") );
          };
          {
            using namespace right;
            QString tooltip = makeToolTip( i18n("Right border") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("The size of the border on the right side of the paper.") );
          };
        };
      };
    };
    {
      using namespace settings;
      {
        using namespace startup;
        {
          using namespace dailyTip;
          QString tooltip   = makeToolTip( i18n("Show daily tip") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Activate this box to see a daily tip on startup."));
        };
        {
          using namespace showLogo;
          QString tooltip   = makeToolTip( i18n("Show startup logo") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Activate this to see the BibleTime logo on startup."));
        };
        {
          using namespace restoreWorkingArea;
          QString tooltip   = makeToolTip( i18n("Restore the last BibleTime session on startup") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Save the user's session when BibleTime is closed and restore it on the next startup."));
        };
      };
      {
        using namespace fonts;
        {
          using namespace typeChooser;
          QString tooltip   = makeToolTip( i18n("Choose a language") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The font selection below will apply to all texts in this language."));
        };
      };
      {
        using namespace colors;
        {
          using namespace text;
          QString tooltip = makeToolTip( i18n("Text color") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The color used for normal text in the display windows."));
        };
        {
          using namespace background;
          QString tooltip = makeToolTip( i18n("Color of the background") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("This is the background color used in display windows."));
        };
        {
          using namespace links;
          QString tooltip   = makeToolTip( i18n("Link color") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The color used for all references into Sword modules."));
        };
        {
          using namespace footnotes;
          QString tooltip   = makeToolTip( i18n("Footnote color") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("This color will be used for all footnotes in the display windows."));
        };
        {
          using namespace strongNumbers;
          QString tooltip   = makeToolTip( i18n("Strong's Numbers") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("This color will be used for every Strong's Number in the display windows."));
        };
        {
          using namespace highlightedVerse;
          QString tooltip   = makeToolTip( i18n("Color of the highlighted verse") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("This is the color of the highlighted verse used in display windows."));
        };
        {
          using namespace jesusWords;
          QString tooltip   = makeToolTip( i18n("Words of Jesus") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("All words spoken by Jesus will appear in this color."));
        };
        {
          using namespace morphTags;
          QString tooltip = makeToolTip( i18n("Morphological tags") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("All morphological tags will appear in this color."));
        };
      };
      {
        using namespace profiles;
        {
          using namespace list;
          QString tooltip   = makeToolTip( i18n("List of sessions") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("The list of available sessions."));
        };
        {
          using namespace createNew;
          QString tooltip   = makeToolTip( i18n("Create new session") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Create a new, empty session with default settings."));
        };
        {
          using namespace deleteCurrent;
          QString tooltip   = makeToolTip( i18n("Delete session") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Delete the selected session."));
        };
        {
          using namespace renameCurrent;
          QString tooltip   = makeToolTip( i18n("Rename session") );
          QString whatsthis = makeWhatsThis( tooltip, i18n("Rename the selected session."));
        };
      };
      {
        using namespace settings::sword;
        {
          using namespace general;
          {
            using namespace cacheKeys;
            QString tooltip   = makeToolTip( i18n("Use cache files for the lexicon entries") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Activate this to let BibleTime create index files for the lexicon entries. This increases the speed of opening large lexicon modules significantly, but does consume some disk space. Recommended."));
          };
          {
            using namespace scrolling;
            QString tooltip   = makeToolTip( i18n("Use down arrow to scroll to next verse") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Check this to use the down arrow to scroll to the next verse."));
          };
          {
            using namespace language;
            QString tooltip   = makeToolTip( i18n("List of languages") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Contains the languages which can be used for the biblical booknames."));
          };
        };
        {
          using namespace settings::sword::modules;
          {
            using namespace bible;
            QString tooltip   = makeToolTip( i18n("The default bible used in the workspace area") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("The default Bible is used when a hyperlink into a Bible is clicked."));
          };
          {
            using namespace commentary;
            QString tooltip   = makeToolTip( i18n("The default commentary used in the workspace area") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("The default commentary is used when a hyperlink into a commentary is clicked."));
          };
          {
            using namespace lexicon;
            QString tooltip   = makeToolTip( i18n("The default lexicon used in the workspace area") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("The default lexicon is used when a hyperlink into a lexicon is clicked."));
          };
          {
            using namespace dailyDevotional;
            QString tooltip   = makeToolTip( i18n("The default daily devotional module") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("The default devotional will be used to display a short start up devotional."));
          };
          {
            using namespace hebrewStrongs;
            QString tooltip   = makeToolTip( i18n("The default Hebrew lexicon used in the workspace area") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("The default Hebrew lexicon is used when a hyperlink into a Hebrew lexicon is clicked."));
          };
          {
            using namespace greekStrongs;
            QString tooltip   = makeToolTip( i18n("The default Greek lexicon used in the workspace area") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("The default Greek lexicon is used when a hyperlink into a Greek lexicon is clicked."));
          };
          {
            using namespace hebrewMorph;
            QString tooltip   = makeToolTip( i18n("The default Morphological lexicon for Hebrew texts used in the workspace area") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("The standard morphological lexicon for Hebrew texts is used when a hyperlink of a morphological tag in a Hebrew text is clicked."));
          };
          {
            using namespace greekMorph;
            QString tooltip   = makeToolTip( i18n("The default Morphological lexicon for Greek texts used in the workspace area") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("The standard morphological lexicon for Greek texts is used when a hyperlink of a morphological tag in a Greek text is clicked."));
          };
        };
        {
          using namespace filters;
          {
            using namespace lineBreaks;
            QString tooltip   = makeToolTip( i18n("Line breaks") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Insert line breaks after each verse."));
          };
          {
            using namespace verseNumbers;
            QString tooltip   = makeToolTip( i18n("Verse numbers") );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Show the verse number before each verse in Bibles."));
          };
          {
            using namespace footnotes;
            QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::footnotes) );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Show footnotes embedded in the module's text."));
          };
          {
            using namespace strongsNumbers;
            QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::strongNumbers) );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Show Strong's Numbers embedded in the text of modules which support this feature."));
          };
          {
            using namespace morphTags;
            QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::morphTags) );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Show tags providing morphological information in the text of modules which support this feature."));
          };
          {
            using namespace headings;
            QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::headings) );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Show section headings in the text in the text of modules which support this feature."));
          };
          {
            using namespace lemmas;
            QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::lemmas) );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Show root words."));
          };
          {
            using namespace hebrewVowelPoints;
            QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::hebrewPoints) );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Show hebrew vowel points in the text."));
          };
          {
            using namespace hebrewCantillation;
            QString tooltip  = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::hebrewCantillation) );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Show hebrew cantillation marks in the text."));
          };
          {
            using namespace greekAccents;
            QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::greekAccents) );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Show greek accents in the text."));
          };
          {
            using namespace textualVariants;
            QString tooltip   = makeToolTip( CSwordBackend::translatedOptionName(CSwordBackend::textualVariants) );
            QString whatsthis = makeWhatsThis( tooltip, i18n("Use different variants from the textual sources."));
          };
        };
      };
    };
    {
      using namespace mainIndex;
      {
        using namespace search;
        QString tooltip     = makeToolTip(i18n("Search in open modules"));
        QString whatsthis   = makeWhatsThis(tooltip, i18n("Opens the search dialog to search in the modules that are currently open."));
      };
    };
  };
};
