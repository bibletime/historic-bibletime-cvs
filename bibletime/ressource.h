/***************************************************************************
                          ressource.h  -  description
                             -------------------
    begin                : Wed Jan 19 2000
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

#include <kglobal.h>
#include <kiconloader.h>

//Menu and toolbar entries
#define ID_MENU_FILE 			1
#define ID_MENU_EDIT 			2
#define ID_MENU_VIEW 			3
#define ID_MENU_SETTINGS 	4
#define ID_MENU_WINDOW 		5
#define ID_MENU_HELP		 	6

#define ID_FILE_NEW_WINDOW		1001
#define ID_FILE_CLOSE					1099
#define ID_FILE_QUIT					1100

#define ID_VIEW_FOOTNOTES			3001
#define ID_VIEW_STRONGS				3002

#define ID_SETTINGS_OPTIONS		4001
#define ID_SETTINGS_RESTART_SWORD		4003

#define ID_WINDOW_CASCADE		  5001
#define ID_WINDOW_TILE		  	5002
#define ID_WINDOW_AUTO_TILE				5003
#define ID_WINDOW_AUTO_CASCADE		5004
#define ID_WINDOW_CLOSE_ALL				5005

#define ID_PRESENTER_SEARCH		6001

//toolbar IDs
#define ID_TOOLBAR_PRESENTER_EDIT 	1

//Groupmanager
#define ID_GM_PRESENTER_CREATE			1001
#define ID_GM_BOOKMARK_CREATE				1002
#define ID_GM_GROUP_CREATE					1003
#define ID_GM_ITEMS_DELETE					1004
#define ID_GM_BOOKMARK_CHANGE				1005
#define ID_GM_MODULES_SEARCH				1006
#define ID_GM_MODULE_UNLOCK					1007
#define ID_GM_MODULE_ABOUT					1008
#define ID_GM_BOOKMARKS_EXPORT			1009
#define ID_GM_BOOKMARKS_IMPORT			1010
#define ID_GM_GROUP_CHANGE					1011
#define ID_GM_RESET									1012
#define ID_GM_BOOKMARK_PRINT				1013

#define ID_PRESENTER_BOOK_COMBO					1001
#define	ID_PRESENTER_CHAPTER_COMBO			1002
#define	ID_PRESENTER_VERSE_COMBO				1003
#define ID_PRESENTER_LINED							1004
#define ID_PRESENTER_TOGGLE_FOOTNOTE		1005
#define ID_PRESENTER_TOGGLE_STRONGS			1006
#define ID_PRESENTER_TOOLBAR						1007
#define ID_PRESENTER_EDIT								1008
#define ID_PRESENTER_NEXT_VERSE					1009
#define ID_PRESENTER_PREVIOUS_VERSE			1010
#define ID_PRESENTER_NEXT_CHAPTER				1011
#define ID_PRESENTER_PREVIOUS_CHAPTER		1012
#define ID_PRESENTER_NEXT_BOOK					1013
#define ID_PRESENTER_PREVIOUS_BOOK			1014
#define ID_PRESENTER_ENTRY_COMBO				1015
#define ID_PRESENTER_SELECT_ALL					1016
#define ID_PRESENTER_CHOOSE_MODULES			1017
#define ID_PRESENTER_LOOKUP							1018

#define ID_PRESENTER_COPY_ONLY_KEY			1019
#define ID_PRESENTER_COPY_KEY_TEXT			1020
#define ID_PRESENTER_COPY_KEY						1021
#define ID_PRESENTER_COPY_CHAPTER				1022
#define ID_PRESENTER_COPY_SELECTED			1023

#define ID_PRESENTER_PRINT_KEY					1030
#define ID_PRESENTER_PRINT_CHAPTER			1031

#define ID_PRESENTER_SAVE_ONLY_KEY			1032
#define ID_PRESENTER_SAVE_KEY_TEXT			1033
#define ID_PRESENTER_SAVE_KEY						1034
#define ID_PRESENTER_SAVE_CHAPTER				1035
#define ID_PRESENTER_SAVE_CHAPTER_HTML	1036

#define ID_PRESENTER_COPY_POPUP					1050
#define ID_PRESENTER_PRINT_POPUP				1051
#define ID_PRESENTER_SAVE_POPUP					1052
//Noteseditor
#define ID_NE_SAVE		1001
#define ID_NE_DELETE	1002
#define ID_NE_RELOAD	1003
#define ID_NE_CLOSE		1004

// scope view
#define ID_SCOPE_REMOVE_ITEM					2001
#define ID_SCOPE_ADD_SCOPE						2002


//Key bindings of BibleTime
#define IDK_GM_RESET										CTRL+ALT+Key_R
#define IDK_GM_ITEMS_DELETE							CTRL+ALT+Key_D
#define IDK_GM_MODULES_SEARCH						CTRL+ALT+Key_M

#define IDK_VIEW_FOOTNOTES							CTRL+ALT+Key_F
#define IDK_VIEW_STRONGS								CTRL+ALT+Key_S
#define IDK_VIEW_HEADINGS               0
#define IDK_VIEW_MORPH_TAGS             0
#define IDK_VIEW_TOOLBAR								CTRL+Key_T
#define IDK_VIEW_GROUPMANAGER						0

#define IDK_PRESENTER_NEXT_VERSE				CTRL+Key_V
#define IDK_PRESENTER_PREVIOUS_VERSE		CTRL+SHIFT+Key_R
#define IDK_PRESENTER_NEXT_CHAPTER			CTRL+Key_H
#define IDK_PRESENTER_PREVIOUS_CHAPTER	CTRL+SHIFT+Key_H
#define IDK_PRESENTER_EDIT							CTRL+SHIFT+Key_E
#define IDK_PRESENTER_EDIT_DELETE				CTRL+Key_D
#define IDK_PRESENTER_EDIT_BOLD					CTRL+Key_B
#define IDK_PRESENTER_EDIT_ITALIC				CTRL+Key_I
#define IDK_PRESENTER_EDIT_UNDERLINE		CTRL+Key_U

#define IDK_WINDOW_CASCADE							CTRL+Key_A
#define IDK_WINDOW_TILE									CTRL+Key_T
#define IDK_WINDOW_AUTO_CASCADE					CTRL+ALT+Key_A
#define IDK_WINDOW_AUTO_TILE						CTRL+ALT+Key_T
#define IDK_WINDOW_CLOSE_ALL						CTRL+Key_K
#define IDK_WINDOW_LOAD_PROFILE					0
#define IDK_WINDOW_SAVE_PROFILE					0
#define IDK_WINDOW_EDIT_PROFILES				0
#define IDK_WINDOW_FULLSCREEN						0


#define IDK_HELP_DAILY_TIP							0

/* The Bibletime Sub-Mimetypes */
/* e.g. bible/reference is expanded to text/bible/reference,
  like text/plain or text/html */

#define REFERENCE    	"bt_reference"    // text/bt_reference
#define STRONGNUMBER 	"bt_strongnumber" // text/bt_strongnumber
#define BOOKMARK     	"bt_bookmark"     // text/bt_bookmark (fake)
#define MODULE     		"bt_module"     	// text/bt_module
#define GROUP     		"bt_group"     		// text/bt_group

#define TEXT         "plain"           // text/plain



#define LOAD_SMALL_ICON(name)	SmallIcon(name, 16)
#define LOAD_MC_ICON(name) BarIcon(name, 22) //MC == module chooser
#define LOAD_OD_ICON(name) KGlobal::instance()->iconLoader()->loadIcon(name, KIcon::NoGroup, KIcon::SizeLarge)
//#define LOAD_OD_ICON(name) SmallIcon(name,16)

//ICONS
#define ICON_FILE_CLEAR_QUEUE "queue"
#define ICON_FILE_NEW "filenew"
#define ICON_FILE_PRINT "fileprint"
#define ICON_FILE_SAVE "filesave"
#define ICON_FILE_DELETE "editdelete"

#define ICON_EDIT "pencil"
#define ICON_EDIT_COPY "editcopy"
#define ICON_EDIT_BOLD "text_bold"
#define ICON_EDIT_ITALIC "text_italic"
#define ICON_EDIT_UNDER "text_under"
#define ICON_EDIT_LEFT "text_left"
#define ICON_EDIT_CENTER "text_center"
#define ICON_EDIT_RIGHT "text_right"
#define ICON_EDIT_JUST "text_block"

#define ICON_MAININDEX_RESET "start"
#define ICON_MAININDEX_SEARCH "bt_search"
#define ICON_MAININDEX_DELETE_ITEMS ICON_FILE_DELETE

#define ICON_VIEW_MAININDEX "bt_mainindex"

#define ICON_WINDOW_TILE "bt_tile"
#define ICON_WINDOW_CASCADE "bt_cascade"
#define ICON_WINDOW_TILE_AUTO "bt_tile_auto"
#define ICON_WINDOW_CASCADE_AUTO "bt_cascade_auto"
#define ICON_WINDOW_CLOSE_ALL "fileclose"
#define ICON_WINDOW_SAVE_PROFILE "bt_save_profile"
#define ICON_WINDOW_LOAD_PROFILE "bt_load_profile"
#define ICON_WINDOW_EDIT_PROFILES "bt_edit_profiles"
#define ICON_WINDOW_FULLSCREEN "window_fullscreen"

#define ICON_HELP_DAILY_TIP "bt_daily_tip"
#define ICON_HELP_CONTENTS "contents"

#define ICON_SYNC	"bt_sync"

//CGroupManager icons
#define GROUP_ICON_SMALL	LOAD_SMALL_ICON("bt_folder")
#define GROUP_OPEN_ICON_SMALL	LOAD_SMALL_ICON("bt_folder_open")
#define GROUP_NEW_ICON_SMALL	LOAD_SMALL_ICON("bt_folder_new")
#define GROUP_CHANGE_ICON_SMALL	LOAD_SMALL_ICON("bt_folder_change")

#define BIBLE_ICON_SMALL	LOAD_SMALL_ICON("bt_bible")
#define BIBLE_ICON_MC	LOAD_MC_ICON("bt_bible")
#define BIBLE_LOCKED_ICON_SMALL	LOAD_SMALL_ICON("bt_bible_locked")
#define BIBLE_ADD_ICON_MC	LOAD_MC_ICON("bt_bible_add")

#define COMMENTARY_ICON_SMALL	LOAD_SMALL_ICON("bt_commentary")
#define COMMENTARY_ICON_MC LOAD_MC_ICON("bt_commentary")
#define COMMENTARY_LOCKED_ICON_SMALL	LOAD_SMALL_ICON("bt_commentary_locked")
#define COMMENTARY_ADD_ICON_MC	LOAD_SMALL_ICON("bt_commentary_add")

#define LEXICON_ICON_SMALL	LOAD_SMALL_ICON("bt_lexicon")
#define LEXICON_ICON_MC	LOAD_MC_ICON("bt_lexicon")
#define LEXICON_LOCKED_ICON_SMALL	LOAD_SMALL_ICON("bt_lexicon_locked")
#define LEXICON_ADD_ICON_MC	LOAD_MC_ICON("bt_lexicon_add")

#define PARALLEL_ICON_MC	LOAD_SMALL_ICON("bt_parallel")
#define SYNC_ICON_SMALL	LOAD_SMALL_ICON(ICON_SYNC)
#define ITEMS_DELETE_ICON_SMALL	LOAD_SMALL_ICON(ICON_MAININDEX_DELETE_ITEMS)

#define BOOKMARK_ICON_SMALL	LOAD_SMALL_ICON("bt_bookmark")
#define BOOKMARK_CHANGE_ICON_SMALL	LOAD_SMALL_ICON("bt_bookmark_change")
#define BOOKMARK_NEW_ICON_SMALL	LOAD_SMALL_ICON("bt_bookmark_new")
#define BOOKMARK_IMPORT_ICON_SMALL	LOAD_SMALL_ICON("bt_bookmark_import")
#define BOOKMARK_EXPORT_ICON_SMALL	LOAD_SMALL_ICON("bt_bookmark_export")
#define BOOKMARK_PRINT_ICON_SMALL	LOAD_SMALL_ICON(ICON_FILE_PRINT)

#define SHOW_GM_ICON_SMALL	LOAD_SMALL_ICON( ICON_MAININDEX )
#define RESET_GM_ICON_SMALL	LOAD_SMALL_ICON( ICON_MAININDEX_RESET )

#define MODULE_SEARCH_ICON_SMALL	LOAD_SMALL_ICON( ICON_MAININDEX_SEARCH )
#define MODULE_UNLOCK_ICON_SMALL	LOAD_SMALL_ICON("bt_module_unlock")
#define MODULE_ABOUT_ICON_SMALL	LOAD_SMALL_ICON("info")

//used for Drag&Drop
#define REFERENCE_ICON_SMALL	LOAD_SMALL_ICON("bt_verseref")

//Icons for the optionsdialog; OD == OptionsDialog
#define OD_ICON_GENERAL	LOAD_OD_ICON("go")
#define OD_ICON_KEY_BINDINGS LOAD_OD_ICON("key_bindings")
#define OD_ICON_FONTS LOAD_OD_ICON("fonts")
#define OD_ICON_FONTMANAGER LOAD_OD_ICON("fonts")
#define OD_ICON_COLORS	LOAD_OD_ICON("colorize")

//define HelpDialog aliases for defines
#define HELPDIALOG_MODULE_LOCKED							"bibletime/helpdialog/help-module-encrypted.html"
#define HELPDIALOG_FIRST_START								"bibletime/helpdialog/help-first-startup.html"
#define HELPDIALOG_NO_SWORD_MODULES						"bibletime/helpdialog/help-no_modules.html"
//#define HELPDIALOG_NO_SWORD_MODULES_DIR				"bibletime/helpdialog/help_no_sword_modules_dir.html"
#define HELPDIALOG_NO_SWORD_MODULE_CONFIG_DIR	"bibletime/helpdialog/help-no-mods-d.html"
//#define HELPDIALOG_NO_SWORD_CONFIG						"bibletime/helpdialog/help_no_sword_config.html"
#define HELPDIALOG_INITBACKEND_FAILED					"bibletime/helpdialog/help-initbackend-failed.html"
#define HELPDIALOG_FIRSTTIME_SEARCH						"bibletime/helpdialog/help-first-time-search.html"

