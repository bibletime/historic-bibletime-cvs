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
#include <klocale.h>
#include <kshortcut.h>

/**Provides static functions to easily access the Tooltip and WhatsThis texts for all the frontend parts.
  *@author The BibleTime team
  */
  
namespace CResMgr {
  inline const QString makeToolTip( const QString& text ) {
    return text;
  };
  inline const QString makeWhatsThis( const QString& title, const QString& description )  {
    return QString::fromLatin1("<center><B>%1</B></center><HR>%2").arg(title).arg(description);
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

    namespace mainIndex { //Main menu->Settings
      namespace search {
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
      namespace installation {
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
    namespace searchButton {
      extern const QString tooltip;
      extern const QString whatsthis;
    };
    namespace cancelSearchButton {
      extern const QString tooltip;
      extern const QString whatsthis;
    };
    
    namespace options {
      namespace moduleChooserDialog {
        extern const QString tooltip;
        extern const QString whatsthis;
      };
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
      };
      namespace foundItems {
        extern const QString tooltip;
        extern const QString whatsthis;
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
  };

  namespace printing {
  };
}

#endif
