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

//KDE includes
#include <klocale.h>

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
        const QString tooltip = makeToolTip( i18n("Clear the printing queue") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Clear the print queue of BibleTime. All items will be removed from the list and the print button will be disabled.") );
      }
      namespace print {
        const QString tooltip = makeToolTip( i18n("Open the printerdialog") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Open the printer dialog of BibleTime.  Here you can edit the print queue and assign styles to the items.") );
      }      
      namespace quit {
        const QString tooltip = makeToolTip( i18n("Close BibleTime") );
        const QString whatsthis = makeWhatsThis( tooltip, i18n("Close BibleTime and save the settings.") );
      }
    }
    namespace view { //Main menu->View
      namespace showMainIndex {
        const QString tooltip = makeToolTip(i18n("Show main index"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Toggle the main index view."));

      }
      namespace showToolBar {
        const QString tooltip = makeToolTip(i18n("Show toolbar"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Toggle the main toolbar view."));
      }
    }
    namespace window { //Main menu->Window
      namespace loadProfile {
        const QString tooltip = makeToolTip( i18n("Load a new profile") );
        const QString whatsthis = makeWhatsThis(tooltip,i18n("Load a new profile") );
      }
      namespace saveProfile {
        const QString tooltip = makeToolTip(i18n("Save current profile"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Save current profile"));
      }
      namespace setupProfiles {
        const QString tooltip = makeToolTip(i18n("Edit profiles"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Create new and delete or rename existing profiles."));
      }
      namespace showFullscreen {
        const QString tooltip = makeToolTip(i18n("Toggle fullscreen mode"));
        const QString whatsthis = makeWhatsThis(tooltip,i18n("Toggle fullscreen mode"));
      }      
      namespace tile {
        const QString tooltip = makeToolTip(i18n("Tile windows"));
        const QString whatsthis = makeWhatsThis(tooltip,i18n("Tile the open display windows."));
      }
      namespace cascade {
        const QString tooltip = makeToolTip(i18n("Cascade windows"));
        const QString whatsthis = makeWhatsThis(tooltip,i18n("Cascade the open display windows."));
      }
      namespace autoTile {
        const QString tooltip = makeToolTip(i18n("Automatically tile windows"));
        const QString whatsthis = makeWhatsThis(tooltip,i18n("Automatically tile the display windows."));
      }
      namespace autoCascade {
        const QString tooltip = makeToolTip(i18n("Automatically cascade windows"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Automatically cascade the opened display windows."));
      }
      namespace closeAll {
        const QString tooltip = makeToolTip(i18n("Close all windows"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Close all open display windows."));
      }
    }

    namespace settings { //Main menu->Settings
      namespace editToolBar {
        const QString tooltip = makeToolTip(i18n("Edit toolbar"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Open the BibleTime tool bar editor."));
      }
      namespace optionsDialog {
        const QString tooltip = makeToolTip(i18n("Options"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Open BibleTime's optionsdialog where you can set most of the preferences."));
      };
    };

    namespace help { //Main menu->Help
      namespace handbook {
        const QString tooltip = makeToolTip(i18n("BibleTime's handbook"));
        const QString whatsthis = makeWhatsThis(tooltip,i18n("Open KDE's helpbrowser with BibleTime's handbook.<BR>The handbook explains how to use BibleTime in the best way."));
      };
      namespace installation {
        const QString tooltip = makeToolTip(i18n("BibleTime's installation documentation"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Open KDE's helpbrowser with BibleTime's installation instructions.<BR>The installation instructions explain how to set up BibleTime and the text modules."));
      };
      namespace bibleStudyHowTo {
        const QString tooltip = makeToolTip(i18n("BibleTime's Bible study HowTo"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Open KDE's helpbrowser with the Bible study HowTo included with BibleTime.<BR>This HowTo is an introduction how to study the Bible in an efficient way."));
      };
      namespace whatThis {
        const QString tooltip = makeToolTip(i18n("Enter \"What's this?\" mode"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Switch into the <B>\"What's this?\"</B> mode.<BR>All important elements of BibleTime's GUI offer a \"What's this?\" help message."));
      };
      namespace bugreport {
        const QString tooltip = makeToolTip(i18n("Send bugreport"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Send a bugreport to the developers of BibleTime!"));
      };
      namespace dailyTip {
        const QString tooltip = makeToolTip(i18n("Show daily tips at startup"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Show a daily tip.<BR>The tips contain important Bible quotations and helpful usage tips for BibleTime."));
      };
      namespace aboutBibleTime {
        const QString tooltip = makeToolTip(i18n("About BibleTime"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Show detailed information about BibleTime."));
      };
      namespace aboutKDE {
        const QString tooltip = makeToolTip(i18n("About KDE"));
        const QString whatsthis = makeWhatsThis(tooltip, i18n("Show the \"about\" dialog of KDE."));
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
