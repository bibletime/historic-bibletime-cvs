/***************************************************************************
                          tooltipdef.h  -  description
                             -------------------
    begin                : Sat Jul 29 2000
    copyright            : (C) 2000 by The BibleTime team
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

//ToolTip entries for Mainwindow actions
#define TT_FILE_CLEAR_QUEUE  i18n("Clear the printing queue")
#define TT_FILE_PRINT  i18n("Open the printerdialog")
#define TT_FILE_QUIT  i18n("Close BibleTime")
	
#define TT_GM_SEARCH_MODULES i18n("Search in modules")
#define TT_GM_BACK_MODULES i18n("Previous item")
#define TT_GM_FORWARD_MODULES i18n("Next item")
#define TT_GM_DELETE_ITEMS i18n("Remove selected items")
#define TT_GM_RESET	i18n("Reset main index")

#define TT_VIEW_GROUPMANAGER i18n("Show main index")
#define TT_VIEW_TOOLBAR i18n("Show tooolbar")
#define TT_VIEW_FOOTNOTES i18n("Show footnotes")
#define TT_VIEW_STRONGS i18n("Show Strong's numbers")

#define TT_WINDOW_TILE i18n("Tile windows")
#define TT_WINDOW_AUTO_TILE  i18n("Automatically tile windows")
#define TT_WINDOW_CASCADE  i18n("Cascade windows")
#define TT_WINDOW_AUTO_CASCADE  i18n("Automatically cascade windows")
#define TT_WINDOW_CLOSE_ALL  i18n("Close all windows")
#define TT_WINDOW_SAVE_PROFILE  i18n("Save current profile")
#define TT_WINDOW_EDIT_PROFILES  i18n("Edit profiles")
#define TT_WINDOW_LOAD_PROFILE  i18n("Load a new profile")
#define TT_WINDOW_FULLSCREEN  i18n("Toggle fullscreen mode")

#define TT_SETTINGS_EDIT_TOOLBAR i18n("Edit toolbar")
#define TT_SETTINGS_OPTIONS i18n("Options")

#define TT_HELP_HANDBOOK i18n("BibleTime's handbook")
#define TT_HELP_INSTALLATION i18n("BibleTime's installation documentation")
#define TT_HELP_BIBLESTUDY i18n("BibleTime's Bible study HowTo")
#define TT_HELP_WHATSTHIS i18n("Enter \"What's this?\" mode")
#define TT_HELP_BUGREPORT i18n("Send bugreport")
#define TT_HELP_DAILY_TIP i18n("Show daily tips at startup")
#define TT_HELP_ABOUT i18n("About BibleTime")
#define TT_HELP_ABOUT_KDE i18n("About KDE")


//QWhatsThis defines for the groupmanager
#define TT_GM_WIDGET i18n("The main index")
#define TT_GM_NEW_GROUP i18n("Create a new folder")
#define TT_GM_CHANGE_GROUP i18n("Change this folder")
#define TT_GM_CHANGE_BOOKMARK i18n("Change this bookmark")
#define TT_GM_IMPORT_BOOKMARKS i18n("Import bookmarks")
#define TT_GM_EXPORT_BOOKMARKS i18n("Export bookmarks")
#define TT_GM_PRINT_BOOKMARK i18n("Add bookmark to printing queue")
#define TT_GM_UNLOCK_MODULE i18n("Unlock this module")
#define TT_GM_ABOUT_MODULE i18n("About this module")

//QWhatsThis defines for the presenters
#define TT_MDI_AREA_WIDGET i18n("The working area")

#define TT_PRESENTER_NEXT_ENTRY i18n("Show the next entry")
#define TT_PRESENTER_PREVIOUS_ENTRY i18n("Show the previous entry")
#define TT_PRESENTER_NEXT_VERSE i18n("Jump to the next verse")
#define TT_PRESENTER_PREVIOUS_VERSE i18n("Jump to the previous verse")
#define TT_PRESENTER_NEXT_CHAPTER i18n("Show the next chapter")
#define TT_PRESENTER_PREVIOUS_CHAPTER i18n("Show the previous chapter")
#define TT_PRESENTER_NEXT_BOOK i18n("Show the next book")
#define TT_PRESENTER_PREVIOUS_BOOK i18n("Show the previous book")
#define TT_PRESENTER_BOOK_COMBO i18n("List of books")
#define TT_PRESENTER_CHAPTER_COMBO i18n("List of chapters")
#define TT_PRESENTER_VERSE_COMBO i18n("List of verses")
#define TT_PRESENTER_ENTRY_COMBO i18n("List of entries")
#define TT_PRESENTER_SCROLL_BUTTON i18n("Scroll through the list")
#define TT_PRESENTER_EDIT i18n("Edit this entry")
#define TT_PRESENTER_EDIT_SAVE i18n("Save this comment")
#define TT_PRESENTER_EDIT_DELETE i18n("Delete this comment")
#define TT_PRESENTER_EDIT_COMMENT_UP_TO i18n("Comment up to")
#define TT_PRESENTER_EDIT_SET_BOLD i18n("Set bold")
#define TT_PRESENTER_EDIT_SET_ITALIC i18n("Set italic")
#define TT_PRESENTER_EDIT_SET_UNDERLINED i18n("Set underlined")
#define TT_PRESENTER_EDIT_SET_FONT i18n("Set font")
#define TT_PRESENTER_EDIT_COLOR_CHOOSER i18n("Set the color")
#define TT_PRESENTER_EDIT_FONTSIZE_CHOOSER i18n("Set the fontsize")

#define TT_PRESENTER_SYNC i18n("Synchronize with Bible windows")

//HTML widget related #defines
#define TT_HTML_SAVE_AS i18n("Save as HTML")
#define TT_HTML_SELECT_ALL i18n("Select all")
#define TT_HTML_COPY i18n("Copy to clipboard")

//searchdialog related #defines, SD means searchdialog; mc==Module chooser
#define TT_SD_MODULECHOOSER i18n("Choose the modules to search in")
#define TT_SD_MC_ADD_MODULE i18n("Add the selected module to the list")
#define TT_SD_MC_REMOVE_MODULE i18n("Remove the selected module from the list")
#define TT_SD_MC_MODULELIST i18n("List of modules used for the search")

#define TT_SD_SEARCH_TEXT_EDIT i18n("Search for this text")
#define TT_SD_SEARCH_MULTIPLE_WORDS i18n("Search multiple words")
#define TT_SD_SEARCH_EXACT_MATCH i18n("Exact match")
#define TT_SD_SEARCH_REGEXP i18n("Search with GNU regular expressions")
#define TT_SD_SEARCH_CASE_SENSITIVE i18n("Search case sensitive")

#define TT_SD_SCOPE_NO_SCOPE i18n("No scope")
#define TT_SD_SCOPE_LAST_RESULT i18n("Last search result")
#define TT_SD_SCOPE_OWN_SCOPE i18n("Use custom scope")
#define TT_SD_SCOPE_NEW_SCOPE i18n("Add a new scope")
#define TT_SD_SCOPE_DELETE_SCOPE i18n("Delete this search scope")
#define TT_SD_SCOPE_EDIT_SCOPE_BUTTON i18n("Edit and define your search scopes")
#define TT_SD_SCOPE_CHOOSER i18n("Choose a search scope from the list")
#define TT_SD_SCOPE_EDIT_LINE	i18n("Enter your own search scope")
#define TT_SD_SCOPE_PARSED_LIST	i18n("Parsed result")
#define TT_SD_SCOPE_CLOSE i18n("Close this dialog")

#define TT_SD_RESULT_MODULE_VIEW i18n("Used modules")
#define TT_SD_RESULT_RESULT_VIEW i18n("Items found")
#define TT_SD_RESULT_PREVIEW 	i18n("Preview")

#define TT_SD_ANALYSIS_VIEW i18n("Graphical Search Analysis")


//#defines for the settings dialog, OD means Optionsdialog
#define TT_OD_GENERAL_DAILY_TIP i18n("Show daily tip")
#define TT_OD_GENERAL_INTERNATIONAL_BOOKNAMES i18n("List of languages")
#define TT_OD_GENERAL_SHOW_STARTUPLOGO i18n("Show start-up logo")
#define TT_OD_GENERAL_SCROLL_PREVIOUS i18n("Use down arrow to scroll to next verse")
#define TT_OD_GENERAL_RESTORE_WORKSPACE i18n("Restore the windows in the workspace on startup")

#define TT_OD_SWORD_USE_LEXICON_CACHE i18n("Use cache files for the lexicon entries")
#define TT_OD_SWORD_STANDARD_BIBLE i18n("The default bible used in the workspace area")
#define TT_OD_SWORD_STANDARD_COMMENTARY i18n("The default commentary used in the workspace area")
#define TT_OD_SWORD_STANDARD_LEXICON i18n("The default lexicon used in the workspace area")
#define TT_OD_SWORD_STANDARD_HEBREW_STRONG 	i18n("The default Hebrew lexicon used in the workspace are")
#define TT_OD_SWORD_STANDARD_GREEK_STRONG i18n("The default Greek lexicon used in the workspace area")
#define TT_OD_SWORD_STANDARD_HEBREW_MORPH i18n("The default Morphological lexicon for Hebrew texts used in the workspace area")
#define TT_OD_SWORD_STANDARD_GREEK_MORPH i18n("The default Morphological lexicon for Greek texts used in the workspace area")


#define TT_OD_KEYS_CHOOSER i18n("Set keyboard accelerators")

#define TT_OD_FONTS_TYPE_CHOOSER i18n("Choose the area of application")
#define TT_OD_FONTS_CHOOSER i18n("Choose a font")

#define TT_OD_COLORS_BACKGROUND i18n("Color of the background")
#define TT_OD_COLORS_CURRENT_VERSE i18n("Color of the highlighted verse")

#define TT_OD_DISPLAY_WINDOW_KEYS_GENERAL i18n("Keyboard accelerators used by all display windows")
#define TT_OD_DISPLAY_WINDOW_KEYS_BIBLE i18n("Keyboard accelerators used by Bible windows")
#define TT_OD_DISPLAY_WINDOW_KEYS_COMMENTARY i18n("Keyboard accelerators used by commentary windows")
#define TT_OD_DISPLAY_WINDOW_KEYS_LEXICON i18n("Keyboard accelerators used by lexicon windows")

//defines for the printing dialog; PI == Print Item; PD=Printer Dialog
#define TT_PD_PRINT i18n("Start printing")
#define TT_PD_PREVIEW i18n("Preview print job")
#define TT_PD_CANCEL i18n("Cancel")
#define TT_PD_HELP i18n("Open help")

#define TT_PD_ENTRIES_STYLE_COMBO i18n("Styles")
#define TT_PD_ENTRIES_PI_LIST i18n("List of print items")
#define TT_PD_ENTRIES_PI_MOVE_UP i18n("Move item up")
#define TT_PD_ENTRIES_PI_MOVE_DOWN i18n("Move item down")
#define TT_PD_ENTRIES_PI_DELETE i18n("Delete items")
#define TT_PD_ENTRIES_PI_PAGE_BREAK i18n("Insert a page break")

#define TT_PD_LAYOUT_BORDER i18n("Page borders")
#define TT_PD_LAYOUT_BORDER_LEFT i18n("Left border")
#define TT_PD_LAYOUT_BORDER_RIGHT i18n("Right border")
#define TT_PD_LAYOUT_BORDER_TOP i18n("Upper border")
#define TT_PD_LAYOUT_BORDER_BOTTOM i18n("Lower border")
#define TT_PD_LAYOUT_STYLE_LIST i18n("List of print styles")
#define TT_PD_LAYOUT_STYLE_DELETE i18n("Delete the selected style")
#define TT_PD_LAYOUT_STYLE_EDIT i18n("Edit the selected style")
#define TT_PD_LAYOUT_STYLE_NEW i18n("Create a new printing style")
