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

#ifndef CHELPMGR_H
#define CHELPMGR_H

//Qt includes
#include <qstring.h>
#include <qkeysequence.h>

//KDE includes
#include <klocale.h>
#include <kshortcut.h>

/**Provides static functions to easily access the Tooltip and WhatsThis texts for all the frontend parts.
  *@author The BibleTime team
  */
  
namespace CHelpMgr {
  inline const QString makeToolTip( const QString& text ) {
    return text;
  };
  inline const QString makeWhatsThis( const QString& title, const QString& description )  {
    return QString::fromLatin1("<center><B>%1</B></center><HR>%2").arg(title).arg(description);
  };

  namespace mainMenu { //Main menu
    namespace file { //Main menu->File
      namespace clearQueue {
        const QString tooltip     = makeToolTip( i18n("Clear the printing queue") );
        const QString whatsthis   = makeWhatsThis( tooltip, i18n("Clear the print queue of BibleTime. All items will be removed from the list and the print button will be disabled.") );
        const QString icon        = QString::fromLatin1("queue");
        const KShortcut accel     = KShortcut();
        const char* actionName    = "fileClearQueue_action";
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
      namespace showFullscreen {  //a standard action
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
      namespace editToolBar {
        const QString tooltip     = makeToolTip(i18n("Edit toolbar"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open the BibleTime tool bar editor."));
      }
      namespace optionsDialog {
        const QString tooltip     = makeToolTip(i18n("Options"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open BibleTime's optionsdialog where you can set most of the preferences."));
      };
    };

    namespace help { //Main menu->Help
      namespace handbook {
        const QString tooltip     = makeToolTip(i18n("BibleTime's handbook"));
        const QString whatsthis   = makeWhatsThis(tooltip,i18n("Open KDE's helpbrowser with BibleTime's handbook.<BR>The handbook explains how to use BibleTime in the best way."));
      };
      namespace installation {
        const QString tooltip     = makeToolTip(i18n("BibleTime's installation documentation"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open KDE's helpbrowser with BibleTime's installation instructions.<BR>The installation instructions explain how to set up BibleTime and the text modules."));
      };
      namespace bibleStudyHowTo {
        const QString tooltip     = makeToolTip(i18n("BibleTime's Bible study HowTo"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Open KDE's helpbrowser with the Bible study HowTo included with BibleTime.<BR>This HowTo is an introduction how to study the Bible in an efficient way."));
      };
      namespace whatsThis {
        const QString tooltip     = makeToolTip(i18n("Enter \"What's this?\" mode"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Switch into the <B>\"What's this?\"</B> mode.<BR>All important elements of BibleTime's GUI offer a \"What's this?\" help message."));
      };
      namespace bugreport {
        const QString tooltip     = makeToolTip(i18n("Send bugreport"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Send a bugreport to the developers of BibleTime!"));
      };
      namespace dailyTip {
        const QString tooltip     = makeToolTip(i18n("Show daily tips at startup"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Show a daily tip.<BR>The tips contain important Bible quotations and helpful usage tips for BibleTime."));
      };
      namespace aboutBibleTime {
        const QString tooltip     = makeToolTip(i18n("About BibleTime"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Show detailed information about BibleTime."));
      };
      namespace aboutKDE {
        const QString tooltip     = makeToolTip(i18n("About KDE"));
        const QString whatsthis   = makeWhatsThis(tooltip, i18n("Show the \"about\" dialog of KDE."));
      };      
    };
  }  //end of main menu

  namespace searchdialog {

  };

  namespace workspace {

  };

  namespace displaywindows {


  };

  namespace printing {

  };
}

#endif
