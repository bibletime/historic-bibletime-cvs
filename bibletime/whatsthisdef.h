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
#define WT(title, description) QString::fromLatin1("<center><B>%1</B></center><HR>%2").arg(title).arg(description)

//QWhatsThis defines for the groupmanager
#define WT_GM_WIDGET WT(TT_GM_WIDGET, i18n("This is the main index of BibleTime.<BR>Use it to manage the installed modules, to work with bookmarks, to open display windows etc.."))
#define WT_GM_NEW_GROUP WT(TT_GM_NEW_GROUP, i18n("Create a new folder in the main index.<BR>If you press the right mouse button on a folder, the new folder will become a sub-folder of it.<BR>Otherwise you will create a toplevel folder."))
#define WT_GM_CHANGE_GROUP WT(TT_GM_CHANGE_GROUP, i18n("Change the properties of the selected folder."))
#define WT_GM_CHANGE_BOOKMARK WT(TT_GM_CHANGE_BOOKMARK, i18n("Change the properties of the selected bookmark."))
#define WT_GM_IMPORT_BOOKMARKS WT(TT_GM_IMPORT_BOOKMARKS, i18n("Import bookmarks from a file."))
#define WT_GM_EXPORT_BOOKMARKS WT(TT_GM_EXPORT_BOOKMARKS, i18n("Export bookmarks to a file so you or other<BR>users can import them later."))
#define WT_GM_PRINT_BOOKMARK WT(TT_GM_PRINT_BOOKMARK, i18n("Use this to add this bookmark to BibleTime's print queue. The key, the description, and the text of the key will be used for printing."))
#define WT_GM_UNLOCK_MODULE WT(TT_GM_UNLOCK_MODULE, i18n("Open a small dialog where you can unlock the selected module."))
#define WT_GM_ABOUT_MODULE WT(TT_GM_ABOUT_MODULE, i18n("Open a dialog to show information about the selected module."))

//QWhatsThis defines for the presenters
#define WT_MDI_AREA_WIDGET WT(TT_MDI_AREA_WIDGET, i18n("This area is the main workspace of Bibletime.<BR>All display windows are opened in this window."))

#define WT_PRESENTER_NEXT_ENTRY WT(TT_PRESENTER_NEXT_ENTRY, i18n("The next entry of the module will be shown."))
#define WT_PRESENTER_PREVIOUS_ENTRY WT(TT_PRESENTER_PREVIOUS_ENTRY, i18n("The previous entry of the module will be shown."))
#define WT_PRESENTER_NEXT_VERSE WT(TT_PRESENTER_NEXT_VERSE, i18n("For Bible texts, the next verse will be highlighted.<BR>In commentaries, the next entry will be shown."))
#define WT_PRESENTER_PREVIOUS_VERSE WT(TT_PRESENTER_PREVIOUS_VERSE, i18n("For Bible texts, the previous verse will be highlighted. In commentaries,  the previous entry will be shown."))
#define WT_PRESENTER_NEXT_CHAPTER WT(TT_PRESENTER_NEXT_CHAPTER, i18n("Show the next chapter of the module. The verse will be set to \"1\"."))
#define WT_PRESENTER_PREVIOUS_CHAPTER WT(TT_PRESENTER_PREVIOUS_CHAPTER, i18n("Show the previous chapter of the module. The verse will be set to \"1\""))
#define WT_PRESENTER_NEXT_BOOK WT(TT_PRESENTER_NEXT_BOOK, i18n("Show the next book of this module. The chapter and the verse will be set to \"1\"."))
#define WT_PRESENTER_PREVIOUS_BOOK WT(TT_PRESENTER_PREVIOUS_BOOK, i18n("Show the previous book of this module. The chapter and the verse will be set to \"1\"."))
#define WT_PRESENTER_BOOK_COMBO WT(TT_PRESENTER_BOOK_COMBO, i18n("This list contains the books which are available in this module."))
#define WT_PRESENTER_CHAPTER_COMBO WT(TT_PRESENTER_CHAPTER_COMBO, i18n("This list contains the chapters which are available in the current book."))
#define WT_PRESENTER_VERSE_COMBO WT(TT_PRESENTER_VERSE_COMBO, i18n("This list contains the verses which are available in the current chapter."))
#define WT_PRESENTER_ENTRY_COMBO WT(TT_PRESENTER_ENTRY_COMBO, i18n("This list contains the entries of the current module."))
#define WT_PRESENTER_SCROLL_BUTTON WT(TT_PRESENTER_SCROLL_BUTTON, i18n("This button is useful to scroll through the entries of the list. Press the button and move the mouse to increase or decrease the item."))
#define WT_PRESENTER_EDIT WT(TT_PRESENTER_EDIT, i18n("Toggle between read-write and read-only mode of writeable modules."))
#define WT_PRESENTER_EDIT_SAVE WT(TT_PRESENTER_EDIT_SAVE, i18n("Save new personal comments."))
#define WT_PRESENTER_EDIT_DELETE WT(TT_PRESENTER_EDIT_DELETE, i18n("Clear the text and delete the entry from the module."))
#define WT_PRESENTER_EDIT_SET_BOLD WT(TT_PRESENTER_EDIT_SET_BOLD, i18n("Toggle bold formatting of the selected text."))
#define WT_PRESENTER_EDIT_SET_ITALIC WT(TT_PRESENTER_EDIT_SET_ITALIC, i18n("Toggle italic formatting of the selected text."))
#define WT_PRESENTER_EDIT_SET_UNDERLINED WT(TT_PRESENTER_EDIT_SET_UNDERLINED, i18n("Toggle underlined formatting of the selected text."))
#define WT_PRESENTER_EDIT_SET_FONT WT(TT_PRESENTER_EDIT_SET_FONT, i18n("Choose a new font for the selected text."))
#define WT_PRESENTER_EDIT_COLOR_CHOOSER WT(TT_PRESENTER_EDIT_COLOR_CHOOSER, i18n("Choose a new color for the selected text."))
#define WT_PRESENTER_EDIT_FONTSIZE_CHOOSER WT(TT_PRESENTER_EDIT_FONTSIZE_CHOOSER, i18n("Choose a new fontsize for the selected text."))

#define WT_PRESENTER_SYNC WT(TT_PRESENTER_SYNC, i18n("Synchronize the displayed verse of this module with the active Bible window."))

//HTML widget related #defines
#define WT_HTML_SAVE_AS WT(TT_HTML_SAVE_AS, i18n("Save the displayed chapter as an HTML file."))
#define WT_HTML_SELECT_ALL WT(TT_HTML_SELECT_ALL, i18n("Select all text."))
#define WT_HTML_COPY WT(TT_HTML_COPY, i18n("Copy the displayed text to the clipboard."))

//searchdialog related #defines, SD means searchdialog
#define WT_SD_MODULECHOOSER WT(TT_SD_MODULECHOOSER, i18n("Choose the modules which will be used for the search.<BR>Use the two buttons on the right side to add or delete the modules to/from the list of used ones."))
#define WT_SD_MC_ADD_MODULE WT(TT_SD_MC_ADD_MODULE, i18n("Add the selected module to the list on the right."))
#define WT_SD_MC_REMOVE_MODULE WT(TT_SD_MC_REMOVE_MODULE, i18n("Remove the selected module from the list on the right side."))
#define WT_SD_MC_MODULELIST WT(TT_SD_MC_MODULELIST, i18n("The list of modules used for the search."))

#define WT_SD_SEARCH_TEXT_EDIT WT(TT_SD_SEARCH_TEXT_EDIT, i18n("The text entered in this edit box will be searched for in all selected modules."))
#define WT_SD_SEARCH_MULTIPLE_WORDS WT(TT_SD_SEARCH_MULTIPLE_WORDS, i18n("Select this to search for multiple words.<BR>Each word in the edit box above is searched as a single word."))
#define WT_SD_SEARCH_EXACT_MATCH WT(TT_SD_SEARCH_EXACT_MATCH, i18n("Select this to search for exactly the same text you entered in the edit box."))
#define WT_SD_SEARCH_REGEXP WT(TT_SD_SEARCH_REGEXP, i18n("Select this to search using standard GNU regular expressions.<BR>Read the handbook for a explanation how to use them."))
#define WT_SD_SEARCH_CASE_SENSITIVE WT(TT_SD_SEARCH_CASE_SENSITIVE, i18n("Activate this to use a case sensitive search."))

#define WT_SD_SCOPE_NO_SCOPE WT(TT_SD_SCOPE_NO_SCOPE, i18n("Select this if you want to search through all parts of the module(s)."))
#define WT_SD_SCOPE_LAST_RESULT WT(TT_SD_SCOPE_LAST_RESULT, i18n("Select this if you want to use the last search result as the search scope."))
#define WT_SD_SCOPE_NEW_SCOPE WT(TT_SD_SCOPE_NEW_SCOPE, i18n("Create a new search scope."))
#define WT_SD_SCOPE_OWN_SCOPE WT(TT_SD_SCOPE_OWN_SCOPE, i18n("Use a custom scope for the search.<BR>A standard set of search scopes exists."))
#define WT_SD_SCOPE_DELETE_SCOPE WT(TT_SD_SCOPE_DELETE_SCOPE, i18n("Use this button to delete the chosen search scope."))
#define WT_SD_SCOPE_EDIT_SCOPE_BUTTON WT(TT_SD_SCOPE_EDIT_SCOPE_BUTTON, i18n("Edit and define your search scopes"))
#define WT_SD_SCOPE_CHOOSER WT(TT_SD_SCOPE_CHOOSER,i18n("Choose a search scope from the combo box"))
#define WT_SD_SCOPE_EDIT_LINE WT(TT_SD_SCOPE_EDIT_LINE, i18n("Enter your own search scope.<BR>To define a range use \"-\" (e.g. Genesis-Exodus),<BR>to seperate different ranges or keys use the semicolon (e.g. Genesis;Matthew)."))
#define WT_SD_SCOPE_PARSED_LIST WT(TT_SD_SCOPE_PARSED_LIST, i18n("This list contains the parsed result of your entered scope. This result will be used for the search."))
#define WT_SD_SCOPE_CLOSE WT(TT_SD_SCOPE_CLOSE, i18n("Close this dialog and save the changes."))

#define WT_SD_RESULT_MODULE_VIEW WT(TT_SD_RESULT_MODULE_VIEW, i18n("List of modules used for the search.<BR>Click on a module to see the search result on the right side."))
#define WT_SD_RESULT_RESULT_VIEW WT(TT_SD_RESULT_RESULT_VIEW, i18n("This contains the items found in the selected module."))
#define WT_SD_RESULT_PREVIEW WT(TT_SD_RESULT_PREVIEW, i18n("This text box shows a preview of the chosen item."))

#define WT_SD_ANALYSIS_VIEW WT(TT_SD_ANALYSIS_VIEW, i18n("Graphical analysis of your search result.<BR>The bars show the number of items found in each book of the Bible or commentary."))

//#defines for the settings dialog, OD means Optionsdialog
#define WT_OD_GENERAL_DAILY_TIP WT(TT_OD_GENERAL_DAILY_TIP, i18n("Activate this box to see a daily tip on startup."))
#define WT_OD_GENERAL_INTERNATIONAL_BOOKNAMES WT(TT_OD_GENERAL_INTERNATIONAL_BOOKNAMES, i18n("Contains the languages which can be used for the international booknames."))
#define WT_OD_GENERAL_SHOW_STARTUPLOGO WT(TT_OD_GENERAL_SHOW_STARTUPLOGO, i18n("Activate this to see the BibleTime logo on startup."))
#define WT_OD_GENERAL_SCROLL_PREVIOUS WT(TT_OD_GENERAL_SCROLL_PREVIOUS, i18n("Check this to use the down arrow to scroll to the next verse."))
#define WT_OD_GENERAL_RESTORE_WORKSPACE WT(TT_OD_GENERAL_RESTORE_WORKSPACE, i18n("Save the windows in the workspace if BibleTime is closed<BR>and restore them on the next startup. "))

#define WT_OD_SWORD_USE_LEXICON_CACHE WT(TT_OD_SWORD_USE_LEXICON_CACHE, i18n("Activate this to let BibleTime create index files for the lexicon entries. This will speed up opening the lexicons a lot, but consume some disk space. Recommended."))
#define WT_OD_SWORD_STANDARD_BIBLE WT(TT_OD_SWORD_STANDARD_BIBLE, i18n("The default Bible is used when a hyperlink into a Bible was clicked"))
#define WT_OD_SWORD_STANDARD_COMMENTARY WT(TT_OD_SWORD_STANDARD_BIBLE, i18n("The default commentary is used when a hyperlink into a commentary was clicked"))
#define WT_OD_SWORD_STANDARD_LEXICON WT(TT_OD_SWORD_STANDARD_LEXICON, i18n("The default lexicon is used when a hyperlink into a lexicon was clicked"))
#define WT_OD_SWORD_STANDARD_HEBREW_STRONG WT(TT_OD_SWORD_STANDARD_HEBREW_STRONG, i18n("The default Hebrew lexicon is used when a hyperlink into a Hebrew lexicon was clicked"))
#define WT_OD_SWORD_STANDARD_GREEK_STRONG WT(TT_OD_SWORD_STANDARD_GREEK_STRONG, i18n("The default Greek lexicon is used when a hyperlink into a Greek lexicon was clicked"))
#define WT_OD_SWORD_STANDARD_HEBREW_MORPH WT(TT_OD_SWORD_STANDARD_HEBREW_MORPH, i18n("The standard morphological lexicon for Hebrew texts is used when a hyperlink of a morphological tag in a Hebrew text was clicked."))
#define WT_OD_SWORD_STANDARD_GREEK_MORPH WT(TT_OD_SWORD_STANDARD_HEBREW_MORPH, i18n("The standard morphological lexicon for Greek texts is used when a hyperlink of a morphological tag in a Greek text was clicked."))

#define WT_OD_KEYS_CHOOSER WT(TT_OD_KEYS_CHOOSER, i18n("Select an action and change the key binding for the action."))
#define WT_OD_FONTS_TYPE_CHOOSER WT(TT_OD_FONTS_TYPE_CHOOSER, i18n("Use this box to select how the font is used and change the font in the font chooser."))
#define WT_OD_FONTS_CHOOSER WT(TT_OD_FONTS_CHOOSER, i18n("Choose a font"))

#define WT_OD_COLORS_BACKGROUND WT(TT_OD_COLORS_BACKGROUND, i18n("This is the background color used in display windows."))
#define WT_OD_COLORS_CURRENT_VERSE WT(TT_OD_COLORS_CURRENT_VERSE, i18n("This is the color of the highlighted verse used in display windows."))

#define WT_OD_DISPLAY_WINDOW_KEYS_GENERAL WT(TT_OD_DISPLAY_WINDOW_KEYS_GENERAL, i18n("Set the keyboard accelerators used in all display windows"))
#define WT_OD_DISPLAY_WINDOW_KEYS_BIBLE WT(TT_OD_DISPLAY_WINDOW_KEYS_BIBLE, i18n("Set keyboard accelerators used in Bible windows"))
#define WT_OD_DISPLAY_WINDOW_KEYS_COMMENTARY WT(TT_OD_DISPLAY_WINDOW_KEYS_COMMENTARY, i18n("Set the keyboard accelerators used in commentary windows"))
#define WT_OD_DISPLAY_WINDOW_KEYS_LEXICON WT(TT_OD_DISPLAY_WINDOW_KEYS_LEXICON, i18n("Set the keyboard accelerators used in lexicon windows"))


//defines for the printing dialog; PI == Print Item; PD=Printer Dialog
#define WT_PD_PRINT WT(TT_PD_PRINT, i18n("Start the printing process.<BR>The items of the list will be printed using the assigned styles."))
#define WT_PD_PREVIEW WT(TT_PD_PREVIEW, i18n("Print into a temporary file and open a postscript viewer to preview the printing result."))
#define WT_PD_CANCEL WT(TT_PD_CANCEL, i18n("Closes this dialog without doing anything else."))
#define WT_PD_HELP WT(TT_PD_HELP, i18n("Open the online help of BibleTime."))

#define WT_PD_ENTRIES_STYLE_COMBO WT(TT_PD_ENTRIES_STYLE_COMBO, i18n("This combo box contains the available print styles."))
#define WT_PD_ENTRIES_PI_LIST WT(TT_PD_ENTRIES_PI_LIST, i18n("This is the list which contains the available print items."))
#define WT_PD_ENTRIES_PI_MOVE_UP WT(TT_PD_ENTRIES_PI_MOVE_UP, i18n("Move the selected item up one item."))
#define WT_PD_ENTRIES_PI_MOVE_DOWN WT(TT_PD_ENTRIES_PI_MOVE_DOWN, i18n("Move the selected item down one item."))
#define WT_PD_ENTRIES_PI_DELETE WT(TT_PD_ENTRIES_PI_DELETE, i18n("Delete the selected items."))
#define WT_PD_ENTRIES_PI_PAGE_BREAK WT(TT_PD_ENTRIES_PI_PAGE_BREAK, i18n("Inserts a page break into the list.<BR>The printer will start a new page."))

#define WT_PD_LAYOUT_BORDER WT(TT_PD_LAYOUT_BORDER, i18n("This group contains the page borders of the page measured in millimeters."))
#define WT_PD_LAYOUT_BORDER_LEFT WT(TT_PD_LAYOUT_BORDER_LEFT, i18n("The size of the border on the left side of the paper."))
#define WT_PD_LAYOUT_BORDER_RIGHT WT(TT_PD_LAYOUT_BORDER_RIGHT, i18n("The size of the border on the right side of the paper."))
#define WT_PD_LAYOUT_BORDER_TOP WT(TT_PD_LAYOUT_BORDER_TOP, i18n("The size of the border on the top of the paper."))
#define WT_PD_LAYOUT_BORDER_BOTTOM WT(TT_PD_LAYOUT_BORDER_BOTTOM, i18n("The size of the border on the bottom of the paper."))
#define WT_PD_LAYOUT_STYLE_LIST WT(TT_PD_LAYOUT_STYLE_LIST, i18n("This list contains the available printing styles.<BR>Create your own one using the \"New style\" button."))
#define WT_PD_LAYOUT_STYLE_DELETE WT(TT_PD_LAYOUT_STYLE_DELETE, i18n("Remove the selected printing style from the list."))
#define WT_PD_LAYOUT_STYLE_EDIT WT(TT_PD_LAYOUT_STYLE_EDIT, i18n("Change the properties of the selected printing style."))
#define WT_PD_LAYOUT_STYLE_NEW WT(TT_PD_LAYOUT_STYLE_NEW, i18n("Create a new printing style.<BR>The new style will be added to the list."))

#endif

