/***************************************************************************
                          whatsthisdef.h  -  Contains QWhatThis descriptions
	                          of widget
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

#include "tooltipdef.h"

#ifndef WHATSTHISDEF_H
#define WHATSTHISDEF_H

//the general format of QWhatsThis help texts
#define WT(title, description) QString("<center><B>%1</B></center><HR>%2").arg(title).arg(description)

//QWhatsThis entries for Mainwindow actions
#define WT_FILE_CLEAR_QUEUE WT( TT_FILE_CLEAR_QUEUE, i18n("Clear the printing queue of BibleTime. All items will be removed from the list, the print button will be disabled."))
#define WT_FILE_PRINT WT( TT_FILE_PRINT, i18n("Open the printerdialog of BibleTime where you can edit the printing queue and assign styles to the items."))
#define WT_FILE_QUIT WT( TT_FILE_QUIT, i18n("Close BibleTime and save the settings to disk."))
	
#define WT_GM_SEARCH_MODULES WT(TT_GM_SEARCH_MODULES, i18n("Open the search dialog to search text in one or more modules."))
#define WT_GM_DELETE_ITEMS WT(TT_GM_DELETE_ITEMS, i18n("Remove the selected items from the main index.<BR>If you remove a folder the subitems of it will be removed, too."))
#define WT_GM_RESET	WT(TT_GM_RESET, i18n("Reset the main index<BR>All bookmarks and folders will be deleted and the default folders will be restored."))

#define WT_VIEW_GROUPMANAGER WT(TT_VIEW_GROUPMANAGER, i18n("This will show or hide the main index depending on current state."))
#define WT_VIEW_TOOLBAR WT(TT_VIEW_TOOLBAR, i18n("This will show or hide the main toolbar depending on current state."))
#define WT_VIEW_FOOTNOTES WT(TT_VIEW_FOOTNOTES, i18n("This will show or hide the footnotes in the modules."))
#define WT_VIEW_STRONGS WT(TT_VIEW_STRONGS, i18n("This will show or hide Strong's numbers in the modules."))

#define WT_WINDOW_TILE WT(TT_WINDOW_TILE, i18n("This will arrange the opened display windows in a tile pattern."))
#define WT_WINDOW_AUTO_TILE WT( TT_WINDOW_AUTO_TILE, i18n("If this is enabled the opened display windows will automatically be tiled."))
#define WT_WINDOW_CASCADE WT( TT_WINDOW_CASCADE, i18n("This will arrange the opened display windows in a cascade pattern."))
#define WT_WINDOW_AUTO_CASCADE WT( TT_WINDOW_AUTO_CASCADE, i18n("If this is enabled opened display windows will automatically cascaded."))
#define WT_WINDOW_CLOSE_ALL WT( TT_WINDOW_CLOSE_ALL, i18n("This will close all display windows."))

#define WT_SETTINGS_EDIT_TOOLBAR WT(TT_SETTINGS_EDIT_TOOLBAR, i18n("This will open a dialog where you can edit the toolbar of BibleTime."))
#define WT_SETTINGS_OPTIONS WT(TT_SETTINGS_OPTIONS, i18n("This will open BibleTime's optionsdialog where you can set most of the preferences."))

#define WT_HELP_CONTENT WT(TT_HELP_CONTENT, i18n("This will open KDE's helpbrowser with BibleTime's handbook."))
#define WT_HELP_WHATSTHIS WT(TT_HELP_WHATSTHIS, i18n("This will switch into the <B>\"What's this?\"</B> mode."))
#define WT_HELP_BUGREPORT WT(TT_HELP_BUGREPORT, i18n("This will send a bugreport to the developers of BibleTime!"))
#define WT_HELP_ABOUT WT(TT_HELP_ABOUT, i18n("This will show about information about BibleTime."))
#define WT_HELP_ABOUT_KDE WT(TT_HELP_ABOUT_KDE, i18n("This will show the general about dialog of KDE 2."))


//QWhatsThis defines for the groupmanager
#define WT_GM_WIDGET WT(TT_GM_WIDGET, i18n("This is the main index of BibleTime.<BR>Use it to manage the installed modules, to work with bookmarks, to open display windows etc.."))
#define WT_GM_NEW_GROUP WT(TT_GM_NEW_GROUP, i18n("Create a new folder in the main index.<BR>If you pressed the right mouse button on a folder the new folder will be a sub-item of it.<BR>Otherwise you will create a toplevel folder."))
#define WT_GM_CHANGE_GROUP WT(TT_GM_CHANGE_GROUP, i18n("This will change the properties of the selected folder."))
#define WT_GM_CHANGE_BOOKMARK WT(TT_GM_CHANGE_BOOKMARK, i18n("This will change the properties of the selected bookmark."))
#define WT_GM_IMPORT_BOOKMARKS WT(TT_GM_IMPORT_BOOKMARKS, i18n("This will import bookmark from a file."))
#define WT_GM_EXPORT_BOOKMARKS WT(TT_GM_EXPORT_BOOKMARKS, i18n("This will export bookmarks to a file so you or other<BR>users can import them later."))
#define WT_GM_PRINT_BOOKMARK WT(TT_GM_PRINT_BOOKMARK, i18n("Use this to add this bookmark to BibleTime's printing queue. The key, the description and the text of the key will be used for printing."))
#define WT_GM_UNLOCK_MODULE WT(TT_GM_UNLOCK_MODULE, i18n("This will open a small dialog where you can unlock the selected module."))
#define WT_GM_ABOUT_MODULE WT(TT_GM_ABOUT_MODULE, i18n("Open a dialog to show information about the selected module."))

//QWhatsThis defines for the presenters
#define WT_MDI_AREA_WIDGET WT(TT_MDI_AREA_WIDGET, i18n("This part of BibleTime contains the display windows."))

#define WT_PRESENTER_BIBLE_WIDGET WT(TT_PRESENTER_BIBLE_WIDGET, i18n("This is the Bible display window"))
#define WT_PRESENTER_COMMENTARY_WIDGET WT(TT_PRESENTER_COMMENTARY_WIDGET, i18n("This is the commentary display window."))
#define WT_PRESENTER_LEXICON_WIDGET WT(TT_PRESENTER_LEXICON_WIDGET, i18n("This is the lexicon display window."))

#define WT_PRESENTER_NEXT_VERSE WT(TT_PRESENTER_NEXT_VERSE, i18n("If you view the text of a Bible the next verse will be highlighted. If you read a commentary the next entry will be shown."))
#define WT_PRESENTER_PREVIOUS_VERSE WT(TT_PRESENTER_PREVIOUS_VERSE, i18n("If you view the text of a Bible the previous verse will be highlighted. If you read a commentary the next entry will be shown."))
#define WT_PRESENTER_NEXT_CHAPTER WT(TT_PRESENTER_NEXT_CHAPTER, i18n("Show the next chapter of the module. The verse will be set to \"1\"."))
#define WT_PRESENTER_PREVIOUS_CHAPTER WT(TT_PRESENTER_PREVIOUS_CHAPTER, i18n("Show the previous chapter of the module. The verse will be set to \"1\""))
#define WT_PRESENTER_NEXT_BOOK WT(TT_PRESENTER_NEXT_BOOK, i18n("Show the next book of this module. The chapter and the verse will be set to \"1\"."))
#define WT_PRESENTER_PREVIOUS_BOOK WT(TT_PRESENTER_PREVIOUS_BOOK, i18n("Show the previous book of this module. The chapter and the verse will be set to \"1\"."))
#define WT_PRESENTER_BOOK_COMBO WT(TT_PRESENTER_BOOK_COMBO, i18n("This list contains the book which are available in this module."))
#define WT_PRESENTER_CHAPTER_COMBO WT(TT_PRESENTER_CHAPTER_COMBO, i18n("This list contains the chapters available in the current book."))
#define WT_PRESENTER_VERSE_COMBO WT(TT_PRESENTER_VERSE_COMBO, i18n("This list contains the verses available in the current chapter."))
#define WT_PRESENTER_ENTRY_COMBO WT(TT_PRESENTER_ENTRY_COMBO, i18n("This list contains the entries of the current module."))
#define WT_PRESENTER_SCROLL_BUTTON WT(TT_PRESENTER_SCROLL_BUTTON, i18n("This button is useful to scroll though the entries of the list. Press the button and move the mouse to increase or decrease the item."))
#define WT_PRESENTER_EDIT WT(TT_PRESENTER_EDIT, i18n("Toggle between read-write and read-only mode of writebale modules."))
#define WT_PRESENTER_EDIT_SAVE WT(TT_PRESENTER_EDIT_SAVE, i18n("This will save the personal comment to disk."))
#define WT_PRESENTER_EDIT_DELETE WT(TT_PRESENTER_EDIT_DELETE, i18n("Clear the text and delete the entry from disk."))
#define WT_PRESENTER_EDIT_SET_BOLD WT(TT_PRESENTER_EDIT_SET_BOLD, i18n("Switch between bold and non-italic formatting of the selected text."))
#define WT_PRESENTER_EDIT_SET_ITALIC WT(TT_PRESENTER_EDIT_SET_ITALIC, i18n("Switch between italic and non-italic formatting of the selected text."))
#define WT_PRESENTER_EDIT_SET_UNDERLINED WT(TT_PRESENTER_EDIT_SET_UNDERLINED, i18n("Switch between underlined and non-undelined formatting of the selected text."))
#define WT_PRESENTER_EDIT_SET_FONT WT(TT_PRESENTER_EDIT_SET_FONT, i18n("Choose a new font for the selected text."))
#define WT_PRESENTER_EDIT_COLOR_CHOOSER WT(TT_PRESENTER_EDIT_COLOR_CHOOSER, i18n("Choose a color for the selected text."))
#define WT_PRESENTER_EDIT_FONTSIZE_CHOOSER WT(TT_PRESENTER_EDIT_FONTSIZE_CHOOSER, i18n("Choose a fontsize for the selected text."))

#define WT_PRESENTER_SYNC WT(TT_PRESENTER_SYNC, i18n("Synchronize the displayed text of this module with the active Bible window."))

//HTML widget related #defines
#define WT_HTML_SAVE_AS WT(TT_HTML_SAVE_AS, i18n("Save the displayed chapter as a HTML file to disk."))
#define WT_HTML_SELECT_ALL WT(TT_HTML_SELECT_ALL, i18n("Select all text."))
#define WT_HTML_COPY WT(TT_HTML_COPY, i18n("Copy the displayed text into the clipboard."))

//searchdialog related #defines, SD means searchdialog
#define WT_SD_MODULECHOOSER WT(TT_SD_MODULECHOOSER, i18n("Choose the modules which will be used for the search.<BR>Use the two button on the right side to add or delete them to/from the list of used modules."))
#define WT_SD_MC_ADD_MODULE WT(TT_SD_MC_ADD_MODULE, i18n("Add the selected module to the list on the left."))
#define WT_SD_MC_REMOVE_MODULE WT(TT_SD_MC_REMOVE_MODULE, i18n("Remove the selected module from the list on the left side."))
#define WT_SD_MC_MODULELIST WT(TT_SD_MC_MODULELIST, i18n("The list of modules used for the search."))

#define WT_SD_SEARCH_TEXT_EDIT WT(TT_SD_SEARCH_TEXT_EDIT, i18n("The text entered in this edit box will be searched in all modules."))
#define WT_SD_SEARCH_MULTIPLE_WORDS WT(TT_SD_SEARCH_MULTIPLE_WORDS, i18n("Select this to search for multiple words.<BR>Each word in the edit line is seaeched as a single word."))
#define WT_SD_SEARCH_EXACT_MATCH WT(TT_SD_SEARCH_EXACT_MATCH, i18n("Select this to search for exactly the same text you entered in the edit line."))
#define WT_SD_SEARCH_REGEXP WT(TT_SD_SEARCH_REGEXP, i18n("Select this to search using standard GNU regular expressions.<BR>Read the handbook for a explanation how to use them."))
#define WT_SD_SEARCH_CASE_SENSITIVE WT(TT_SD_SEARCH_CASE_SENSITIVE, i18n("Activate this to use case sensitive search."))

#define WT_SD_SCOPE_NO_SCOPE WT(TT_SD_SCOPE_NO_SCOPE, i18n("Select this if you want to search through alls parts of the module(s)."))
#define WT_SD_SCOPE_LAST_RESULT WT(TT_SD_SCOPE_LAST_RESULT, i18n("Select this if you want to use the last search result of each module as search scope."))
#define WT_SD_SCOPE_NEW_SCOPE WT(TT_SD_SCOPE_NEW_SCOPE, i18n("Create a new scope."))
#define WT_SD_SCOPE_OWN_SCOPE WT(TT_SD_SCOPE_OWN_SCOPE, i18n("Use an own scope for the search.<BR>A standard set of scopes exists."))
#define WT_SD_SCOPE_DELETE_SCOPE WT(TT_SD_SCOPE_DELETE_SCOPE, i18n("Use this button to delete the chosen scope."))
#define WT_SD_SCOPE_EDIT_SCOPE_BUTTON WT(TT_SD_SCOPE_EDIT_SCOPE_BUTTON, i18n("Edit and define your scopes"))
#define WT_SD_SCOPE_CHOOSER WT(TT_SD_SCOPE_CHOOSER,i18n("Choose a scope from the combo box"))
#define WT_SD_SCOPE_EDIT_LINE WT(TT_SD_SCOPE_EDIT_LINE, i18n("Enter your own scope.<BR>To define a range use \"-\" (e.g. Genesis-Exodus),<BR>to seperate different ranges or keys use the semicolon (e.g. Genesis;Matthew)."))
#define WT_SD_SCOPE_PARSED_LIST WT(TT_SD_SCOPE_PARSED_LIST, i18n("This list contains the parsed result of your entered scope. This result will be used for the search."))
#define WT_SD_SCOPE_CLOSE WT(TT_SD_SCOPE_CLOSE, i18n("Close this dialog and save the changes to disk"))

#define WT_SD_RESULT_MODULE_VIEW WT(TT_SD_RESULT_MODULE_VIEW, i18n("List of modules used for the search.<BR>Click on a module to see the search result on the right side."))
#define WT_SD_RESULT_RESULT_VIEW WT(TT_SD_RESULT_RESULT_VIEW, i18n("This contains the items found in the selected module."))
#define WT_SD_RESULT_PREVIEW WT(TT_SD_RESULT_PREVIEW, i18n("This text box shows a preview of the chosen item."))

#define WT_SD_ANALYSIS_VIEW WT(TT_SD_ANALYSIS_VIEW, i18n("Graphical analysis"))

//#defines for the settings dialog, OD means Optionsdialog
#define WT_OD_GENERAL_DAILY_TIP WT(TT_OD_GENERAL_DAILY_TIP, i18n("Activate this box to see a daily tip on startup."))
#define WT_OD_GENERAL_INTERNATIONAL_BOOKNAMES WT(TT_OD_GENERAL_INTERNATIONAL_BOOKNAMES, i18n("Contains the languages which can be used for the international booknames."))
#define WT_OD_GENERAL_SHOW_STARTUPLOGO WT(TT_OD_GENERAL_SHOW_STARTUPLOGO, i18n("Activate this to see a startup logo on each startup of BibleTime."))

#define WT_OD_KEYS_CHOOSER WT(TT_OD_KEYS_CHOOSER, i18n("Select a action and change the key binding for the action."))
#define WT_OD_FONTS_TYPE_CHOOSER WT(TT_OD_FONTS_TYPE_CHOOSER, i18n("Select in this box how the font is used and change the font in the font chooser."))
#define WT_OD_FONTS_CHOOSER WT(TT_OD_FONTS_CHOOSER, i18n("Choose a font"))

#define WT_OD_FFM_FONTS_LIST WT(TT_OD_FFM_FONTS_LIST, i18n("This list contains the modules using foreign fonts."))

#define WT_OD_COLORS_BACKGROUND WT(TT_OD_COLORS_BACKGROUND, i18n("This is the background color used in display windows."))
#define WT_OD_COLORS_CURRENT_VERSE WT(TT_OD_COLORS_CURRENT_VERSE, i18n("This is the color of the highlighted verse used in display windows."))

//defines for the printing dialog; PI == Print Item; PD=Printer Dialog
#define WT_PD_PRINT WT(TT_PD_PRINT, i18n("Start the printing process.<BR>The items of the list will be printed using the assigned styles."))
#define WT_PD_PREVIEW WT(TT_PD_PREVIEW, i18n("This will print into a temporary file and open a postscript viewer to preview the printing result."))
#define WT_PD_CANCEL WT(TT_PD_CANCEL, i18n("Closes this dialog without doing anything with the items or the printer."))
#define WT_PD_HELP WT(TT_PD_HELP, i18n("Open the online help of BibleTime."))

#define WT_PD_GENERAL_PRINTER_LIST WT(TT_PD_GENERAL_PRINTER_LIST, i18n("This is the list of available printers of your system.<BR>Please select the printer you want to use for printing."))
#define WT_PD_GENERAL_PRINT_TO_FILE WT(TT_PD_GENERAL_PRINT_TO_FILE, i18n("Print into a postscript file on disk."))
#define WT_PD_GENERAL_BROWSE_FILE WT(TT_PD_GENERAL_BROWSE_FILE, i18n("Browse for the file you want to print to."))
#define WT_PD_GENERAL_COPIES WT(TT_PD_GENERAL_COPIES, i18n("The number of copies used for printing."))
#define WT_PD_GENERAL_ORIENTATION WT(TT_PD_GENERAL_ORIENTATION, i18n("Choose the orientation of the used paper."))
#define WT_PD_GENERAL_ORIENTATION_PORTRAIT WT(TT_PD_GENERAL_ORIENTATION_PORTRAIT, i18n("Print on the paper using \"Portrait\" as orientation."))
#define WT_PD_GENERAL_ORIENTATION_LANDSCAPE WT(TT_PD_GENERAL_ORIENTATION_LANDSCAPE, i18n("Print on the paper using \"Landscape\" as orientation."))
#define WT_PD_GENERAL_PAPER_TYPE WT(TT_PD_GENERAL_PAPER_TYPE, i18n("The type of the paper which is used for printing."))
#define WT_PD_GENERAL_PAPER_SIZE WT(TT_PD_GENERAL_PAPER_SIZE, i18n("The size of the used paper."))

#define WT_PD_ENTRIES_STYLE_COMBO WT(TT_PD_ENTRIES_STYLE_COMBO, i18n("This combo boy contains the available printing styles."))
#define WT_PD_ENTRIES_PI_LIST WT(TT_PD_ENTRIES_PI_LIST, i18n("This is the list which contains the available print items."))
#define WT_PD_ENTRIES_PI_MOVE_UP WT(TT_PD_ENTRIES_PI_MOVE_UP, i18n("Move the selected item one item above."))
#define WT_PD_ENTRIES_PI_MOVE_DOWN WT(TT_PD_ENTRIES_PI_MOVE_DOWN, i18n("Move the selected item one item below."))
#define WT_PD_ENTRIES_PI_DELETE WT(TT_PD_ENTRIES_PI_DELETE, i18n("Delete the selected items."))
#define WT_PD_ENTRIES_PI_PAGE_BREAK WT(TT_PD_ENTRIES_PI_PAGE_BREAK, i18n("Inserts a page break into the list.<BR>The printer will begin here a new page."))

#define WT_PD_LAYOUT_BORDER WT(TT_PD_LAYOUT_BORDER, i18n("This group contains the page borders of the page measured in millimeters."))
#define WT_PD_LAYOUT_BORDER_LEFT WT(TT_PD_LAYOUT_BORDER_LEFT, i18n("The size of the border on the left side of the paper sheet."))
#define WT_PD_LAYOUT_BORDER_RIGHT WT(TT_PD_LAYOUT_BORDER_RIGHT, i18n("The size of the border on the right side of the paper sheet."))
#define WT_PD_LAYOUT_BORDER_TOP WT(TT_PD_LAYOUT_BORDER_TOP, i18n("The size of the border on the top of the paper sheet."))
#define WT_PD_LAYOUT_BORDER_BOTTOM WT(TT_PD_LAYOUT_BORDER_BOTTOM, i18n("The size of the border on the bottom of the paper sheet."))
#define WT_PD_LAYOUT_STYLE_LIST WT(TT_PD_LAYOUT_STYLE_LIST, i18n("This list contains the available printing styles.<BR>Create your own one using the \"New style\" button."))
#define WT_PD_LAYOUT_STYLE_DELETE WT(TT_PD_LAYOUT_STYLE_DELETE, i18n("Remove the selected style from the list."))
#define WT_PD_LAYOUT_STYLE_EDIT WT(TT_PD_LAYOUT_STYLE_EDIT, i18n("Change the properties of the selected style."))
#define WT_PD_LAYOUT_STYLE_NEW WT(TT_PD_LAYOUT_STYLE_NEW, i18n("Create a new style.<BR>The new printing style will be added to the list."))

#endif

