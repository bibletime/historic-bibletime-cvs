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
#define TT_FILE_CLEAR_QUEUE  i18n("Clear printing queue")
#define TT_FILE_PRINT  i18n("Open the printerdialog")
#define TT_FILE_QUIT  i18n("Quit")
	
#define TT_GM_SEARCH_MODULES i18n("Search in selected modules")
#define TT_GM_DELETE_ITEMS i18n("Remove selected items")
#define TT_GM_RESET	i18n("Reset main index")

#define TT_VIEW_GROUPMANAGER i18n("Show / hide main index")
#define TT_VIEW_TOOLBAR i18n("Show / hide tooolbar")
#define TT_VIEW_FOOTNOTES i18n("Show / hide footnotes")
#define TT_VIEW_STRONGS i18n("Show / hide Strong's numbers")

#define TT_WINDOW_TILE i18n("Tile windows")
#define TT_WINDOW_AUTO_TILE  i18n("Automatically tile windows")
#define TT_WINDOW_CASCADE  i18n("Cascade windows")
#define TT_WINDOW_AUTO_CASCADE  i18n("Automatically cascade windows")
#define TT_WINDOW_CLOSE_ALL  i18n("Close all windows")

#define TT_SETTINGS_EDIT_TOOLBAR i18n("Edit main toolbar")
#define TT_SETTINGS_OPTIONS i18n("Options")

#define TT_HELP_CONTENT i18n("Content")
#define TT_HELP_WHATSTHIS i18n("What's this?")
#define TT_HELP_BUGREPORT i18n("Send bugreport")
#define TT_HELP_ABOUT i18n("About BibleTime")
#define TT_HELP_ABOUT_KDE i18n("About KDE")


//QWhatsThis defines for the groupmanager
#define TT_GM_WIDGET i18n("The main index")
#define TT_GM_NEW_PRESENTER i18n("New display window")
#define TT_GM_NEW_GROUP i18n("Create a new folder")
#define TT_GM_CHANGE_GROUP i18n("Change folder")
#define TT_GM_NEW_BOOKMARK i18n("New bookmark")
#define TT_GM_CHANGE_BOOKMARK i18n("Change bookmark")
#define TT_GM_IMPORT_BOOKMARKS i18n("Import bookmarks")
#define TT_GM_EXPORT_BOOKMARKS i18n("Export bookmarks")
#define TT_GM_PRINT_BOOKMARK i18n("Add bookmark to printing queue")
#define TT_GM_UNLOCK_MODULE i18n("Unlock module")
#define TT_GM_ABOUT_MODULE i18n("About module")

//QWhatsThis defines for the presenters
#define TT_MDI_AREA_WIDGET i18n("The working area")

#define TT_PRESENTER_BIBLE_WIDGET i18n("The bible window")
#define TT_PRESENTER_COMMENTARY_WIDGET i18n("The commentary window")
#define TT_PRESENTER_LEXICON_WIDGET i18n("The lexicon window")

#define TT_PRESENTER_NEXT_VERSE i18n("Next verse")
#define TT_PRESENTER_PREVIOUS_VERSE i18n("Previous verse")
#define TT_PRESENTER_NEXT_CHAPTER i18n("Next chapter")
#define TT_PRESENTER_PREVIOUS_CHAPTER i18n("Previous chapter")
#define TT_PRESENTER_NEXT_BOOK i18n("Next book")
#define TT_PRESENTER_PREVIOUS_BOOK i18n("Previous book")
#define TT_PRESENTER_BOOK_COMBO i18n("Book list")
#define TT_PRESENTER_CHAPTER_COMBO i18n("Chapter list")
#define TT_PRESENTER_VERSE_COMBO i18n("Verse list")
#define TT_PRESENTER_ENTRY_COMBO i18n("Entry list")
#define TT_PRESENTER_SCROLL_BUTTON i18n("Scroll through the combobox")
#define TT_PRESENTER_EDIT i18n("Edit comment")
#define TT_PRESENTER_EDIT_SAVE i18n("Save comment")
#define TT_PRESENTER_EDIT_DELETE i18n("Delete comment")
#define TT_PRESENTER_EDIT_COMMENT_UP_TO i18n("Comment up to")
#define TT_PRESENTER_EDIT_SET_BOLD i18n("Set bold")
#define TT_PRESENTER_EDIT_SET_ITALIC i18n("Set italic")
#define TT_PRESENTER_EDIT_SET_UNDERLINED i18n("Set underlined")
#define TT_PRESENTER_EDIT_SET_FONT i18n("Set font")
#define TT_PRESENTER_EDIT_COLOR_CHOOSER i18n("Choose a color")
#define TT_PRESENTER_EDIT_FONTSIZE_CHOOSER i18n("Choose a fontsize")

#define TT_PRESENTER_SYNC i18n("Synchronize with Bibles")

//HTML widget related #defines
#define TT_HTML_SAVE_AS i18n("Save as HTML")
#define TT_HTML_SELECT_ALL i18n("Select all")
#define TT_HTML_COPY i18n("Copy to clipboard")

//searchdialog related #defines, SD means searchdialog; mc==Module chooser
#define TT_SD_MODULECHOOSER i18n("Choose the modules for the search")
#define TT_SD_MC_ADD_MODULE i18n("Add the selected module to the list")
#define TT_SD_MC_REMOVE_MODULE i18n("Remove the selected module from the list")
#define TT_SD_MC_MODULELIST i18n("List of modules used for the search")

#define TT_SD_SEARCH_TEXT_EDIT i18n("Search this text")
#define TT_SD_SEARCH_MULTIPLE_WORDS i18n("Search multiple words")
#define TT_SD_SEARCH_EXACT_MATCH i18n("Exact match")
#define TT_SD_SEARCH_REGEXP i18n("Search with GNU regexp")
#define TT_SD_SEARCH_CASE_SENSITIVE i18n("Search case sensitive")

#define TT_SD_SCOPE_NO_SCOPE i18n("No scope")
#define TT_SD_SCOPE_LAST_RESULT i18n("Last search result")
#define TT_SD_SCOPE_SET_BOUNDS i18n("Set bounds")
#define TT_SD_SCOPE_ADD_SCOPE i18n("Add scope")
#define TT_SD_SCOPE_DELETE_SCOPE i18n("Delete scope")

#define TT_SD_RESULT_MODULE_VIEW i18n("Used modules")
#define TT_SD_RESULT_RESULT_VIEW i18n("Found items")
#define TT_SD_RESULT_PREVIEW i18n("Preview")

#define TT_SD_ANALYSIS_VIEW i18n("Graphical analysis")

//#defines for the settings dialog, OD means Optionsdialog
#define TT_OD_GENERAL_DAILY_TIP i18n("Show daily tip")
#define TT_OD_GENERAL_INTERNATIONAL_BOOKNAMES i18n("List of languages")
#define TT_OD_GENERAL_SHOW_STARTUPLOGO i18n("Show startuplogo")

#define TT_OD_KEYS_CHOOSER i18n("Set keyboard accelerators")

#define TT_OD_FONTS_PRESENTER i18n("Font of presenter")
#define TT_OD_FONTS_CHOOSER i18n("Choose a font")

#define TT_OD_FFM_FONTS_LIST i18n("List of modules")

#define TT_OD_COLORS_BACKGROUND i18n("Background color")
#define TT_OD_COLORS_NORMAL_TEXT i18n("Normal text")
#define TT_OD_COLORS_VERSENUMBER i18n("Versenumber")
#define TT_OD_COLORS_CURRENT_VERSE i18n("Highlighted verse")

//defines for the printing dialog; PI == Print Item; PD=Printer Dialog
#define TT_PD_PRINT i18n("Start printing")
#define TT_PD_PREVIEW i18n("Open preview")
#define TT_PD_CANCEL i18n("Cancel")
#define TT_PD_HELP i18n("Open help")

#define TT_PD_GENERAL_PRINTER_LIST i18n("The list of printers")
#define TT_PD_GENERAL_PRINT_TO_FILE i18n("Print into file")
#define TT_PD_GENERAL_BROWSE_FILE i18n("Search file")
#define TT_PD_GENERAL_COPIES i18n("Copies")
#define TT_PD_GENERAL_ORIENTATION i18n("Orientation")
#define TT_PD_GENERAL_ORIENTATION_PORTAIT i18n("Portrait")
#define TT_PD_GENERAL_ORIENTATION_LANDSCAPE i18n("Landscape")
#define TT_PD_GENERAL_PAPER_TYPE i18n("Paper type")
#define TT_PD_GENERAL_PAPER_SIZE i18n("Paper size")

#define TT_PD_ENTRIES_STYLE_COMBO i18n("Styles")
#define TT_PD_ENTRIES_PI_LIST i18n("List of print items")
#define TT_PD_ENTRIES_PI_MOVE_UP i18n("Move item up")
#define TT_PD_ENTRIES_PI_MOVE_DOWN i18n("Move item down")
#define TT_PD_ENTRIES_PI_DELETE i18n("Delete items")
#define TT_PD_ENTRIES_PI_PAGE_BREAK i18n("Insert page break")

#define TT_PD_LAYOUT_BORDER i18n("Page borders")
#define TT_PD_LAYOUT_BORDER_LEFT i18n("Left border")
#define TT_PD_LAYOUT_BORDER_RIGHT i18n("Right border")
#define TT_PD_LAYOUT_BORDER_TOP i18n("Upper border")
#define TT_PD_LAYOUT_BORDER_BOTTOM i18n("Lower border")
#define TT_PD_LAYOUT_STYLE_LIST i18n("List of styles")
#define TT_PD_LAYOUT_STYLE_DELETE i18n("Delete style")
#define TT_PD_LAYOUT_STYLE_EDIT i18n("Edit style")
#define TT_PD_LAYOUT_STYLE_NEW i18n("Create new style")
