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

#ifndef WHATSTHISDEF_H
#define WHATSTHISDEF_H

//the general format of QWhatsThis help texts
#define WT(title, description) QString("<center><B>%1</B></center><BR>%2").arg(title).arg(description)

//QWhatsThis entries for Mainwindow actions
#define WT_FILE_CLEAR_QUEUE WT( i18n("Clear printing queue"), i18n("This will clear the printing queue of BibleTime."))
#define WT_FILE_PRINT WT( i18n("Open the printerdialog"), i18n("This will open the printerdialog of BibleTime where you can edit the queue and assign styles to the items."))
#define WT_FILE_QUIT WT( i18n("Quit"), i18n("This will terminate BibleTime."))
	
#define WT_GM_SEARCH_MODULES WT(i18n("Search in selected modules"), i18n("This will open the search dialog to search in selected modules"))
#define WT_GM_DELETE_ITEMS WT(i18n("Delete selected items"), i18n("This will delete the selected items from the groupmanager."))
#define WT_GM_RESET	WT(i18n("Reset groupmanager"), i18n("This will reset the groupmanager<BR>All bookmarks and groups will be deleted and the default groups will be restored."))

#define WT_VIEW_GROUPMANAGER WT(i18n("Show / hide groupmanager"), i18n("This will show or hide the groupmanager depending on current state."))
#define WT_VIEW_TOOLBAR WT(i18n("Show / hide tooolbar"), i18n("This will show or hide the main toolbar depending on current state."))
#define WT_VIEW_FOOTNOTES WT(i18n("Show / hide footnotes"), i18n("This will show or hide the footnotes in texts depending on current state."))
#define WT_VIEW_STRONGS WT(i18n("Show / hide strong numbers"), i18n("This will show or hide the strong numbers in texts depending on current state."))

#define WT_WINDOW_TILE WT(i18n("Tile windows"), i18n("This will arrange the opened presenter windows in a tile pattern."))
#define WT_WINDOW_AUTO_TILE WT( i18n("Automatically tile"), i18n("If this is enabled the opened presenter windows will automatically tiled."))
#define WT_WINDOW_CASCADE WT( i18n("Cascade windows"), i18n("This will arrange the opened presenter windows in a cascade pattern."))
#define WT_WINDOW_AUTO_CASCADE WT( i18n("Automatically cascade"), i18n("If this is enabled opened presenter windows will automatically cascaded."))
#define WT_WINDOW_CLOSE_ALL WT( i18n("Close all windows"), i18n("This will close all opened presenter windows."))

#define WT_SETTINGS_EDIT_TOOLBAR WT(i18n("Edit main toolbar"), i18n("This will open a dialog where you can edit the toolbar of BibleTime."))
#define WT_SETTINGS_OPTIONS WT(i18n("Options"), i18n("This will open BibleTime's optionsdialog where you can set most of the preferences."))

#define WT_HELP_CONTENT WT(i18n("Content"), i18n("This will open KDE's helpbrowser with BibleTime's handbook."))
#define WT_HELP_WHATSTHIS WT(i18n("Whats this"), i18n("This will switch into the \"Whats this?\" mode."))
#define WT_HELP_BUGREPORT WT(i18n("Send bugreport"), i18n("This will send a bugreport to the team of BibleTime!"))
#define WT_HELP_ABOUT WT(i18n("About BibleTime"), i18n("This will show information about BibleTime."))
#define WT_HELP_ABOUT_KDE WT(i18n("About KDE"), i18n("This will show the general about dialog of KDE 2."))


//QWhatsThis defines for the groupmanager
#define WT_GM_WIDGET WT(i18n("The groupmanager"), i18n("This is the groupmanager of BibleTime.<BR>Use it to manage the installed modules, work with bookmarks, to open presenters etc.."))
#define WT_GM_NEW_PRESENTER WT(i18n("New presenter"), i18n("This will open a new presenter window for the selected module."))
#define WT_GM_NEW_GROUP WT(i18n("Create a new group"), i18n("This will create a new group.<BR> If you pressed the right mouse button on a group the new group will be the subitem of it.<BR>Otherwise you will create a toplevel group."))
#define WT_GM_CHANGE_GROUP WT(i18n("Change group"), i18n("This will change the properties of the selected group."))
#define WT_GM_NEW_BOOKMARK WT(i18n("New bookmark"), i18n("This will create a new bookmark. The current key of the activated presenter<BR>will be used to craete the bookmark."))
#define WT_GM_CHANGE_BOOKMARK WT(i18n("Change bookmark"), i18n("This will change the properties of the selected bookmark."))
#define WT_GM_IMPORT_BOOKMARKS WT(i18n("Import bookmarks"), i18n("This will import bookmark from a file."))
#define WT_GM_EXPORT_BOOKMARKS WT(i18n("Export bookmarks"), i18n("This will export bookmarks to a file so you or other<BR>users can import them later."))
#define WT_GM_PRINT_BOOKMARK WT(i18n("Print bookmark"), i18n("Use this to print the bookmark. The key, the description and the text of the key will be printed."))
#define WT_GM_UNLOCK_MODULE WT(i18n("Unlock module"), i18n("This will open a small dialog where you can unlock the selected module."))
#define WT_GM_ABOUT_MODULE WT(i18n("About module"), i18n("This opens a dialog to show information about the selected module."))

//QWhatsThis defines for the presenters
#define WT_MDI_AREA_WIDGET WT(i18n("The working area"), i18n("This is the area where you work with the texts."))

#define WT_PRESENTER_BIBLE_WIDGET WT(i18n("The bible presenter"), i18n("This is the presenter where bibles are shown."))
#define WT_PRESENTER_COMMENTARY_WIDGET WT(i18n("The commentary presenter"), i18n("This is the presenter where commentaries are shown."))
#define WT_PRESENTER_LEXICON_WIDGET WT(i18n("The lexicon presenter"), i18n("This is the presenter where lexicons are shown."))

#define WT_PRESENTER_NEXT_VERSE WT(i18n("Next verse"), i18n("This will show or highlight the next verse depending on the type of presenter."))
#define WT_PRESENTER_PREVIOUS_VERSE WT(i18n("Previous verse"), i18n("This will show or highlight the previous verse depending on the type of presenter."))
#define WT_PRESENTER_NEXT_CHAPTER WT(i18n("Next chapter"), i18n("This will show or highlight the next chapter depending on the type of presenter."))
#define WT_PRESENTER_PREVIOUS_CHAPTER WT(i18n("Previous chapter"), i18n("This will show or highlight the previous chapter depending on the type of presenter."))
#define WT_PRESENTER_BOOK_COMBO WT(i18n("Book list"), i18n("This list contains the books available in the current module."))
#define WT_PRESENTER_CHAPTER_COMBO WT(i18n("Chapter list"), i18n("This list contains the chapters available in the current book."))
#define WT_PRESENTER_VERSE_COMBO WT(i18n("Verse list"), i18n("This list contains the verses available in the current verse."))
#define WT_PRESENTER_ENTRY_COMBO WT(i18n("Entry list"), i18n("This list contains the entries of the current module."))
#define WT_PRESENTER_AUTO_SCROLL WT(i18n("Automatic scrolling"), i18n("If this is activated the current presenter will scroll with the other autoscrolling-enabled presenters."))
#define WT_PRESENTER_EDIT WT(i18n("Edit personal comment"), i18n("This button will toggle between edit and read mode of writebale modules."))
#define WT_PRESENTER_EDIT_SAVE WT(i18n("Save personal comment"), i18n("This will save the current personal comment to disk."))
#define WT_PRESENTER_EDIT_DELETE WT(i18n("Delete personal comment"), i18n("This will clear the currently enetered text but it will not save to disk."))
#define WT_PRESENTER_EDIT_COMMENT_UP_TO WT(i18n("Comment up to"), i18n("This is the upper bound of the commented range of verses."))
#define WT_PRESENTER_EDIT_SET_BOLD WT(i18n("Set bold"), i18n("This will make the selected text bold or not bold, depending on current state."))
#define WT_PRESENTER_EDIT_SET_ITALIC WT(i18n("Set italic"), i18n("This will make the selected text italic or not bold, depending on current state."))
#define WT_PRESENTER_EDIT_SET_UNDERLINED WT(i18n("Set underlined"), i18n("This will make the selected text underlined or not bold, depending on current state."))
#define WT_PRESENTER_EDIT_SET_FONT WT(i18n("Set font"), i18n("This will change the font of the selected text."))
#define WT_PRESENTER_EDIT_COLOR_CHOOSER WT(i18n("Choose a color"), i18n("Use this to change the color of the selected text."))
#define WT_PRESENTER_EDIT_FONTSIZE_CHOOSER WT(i18n("Choose a fontsize"), i18n("use this to change the fontsize of the selected text."))

//HTML widget related #defines
#define WT_HTML_SAVE_AS WT(i18n("Save as HTML"), i18n("This will save the displayed text as a HTML page to disk."))
#define WT_HTML_SELECT_ALL WT(i18n("Select all"), i18n("This will select all displayed text."))
#define WT_HTML_COPY WT(i18n("Copy to clipboard"), i18n("This will copy the selected text to the clipboard."))

//searchdialog related #defines, SD means searchdialog
#define WT_SD_SEARCH_TEXT_EDIT WT(i18n("Search this text"), i18n("The text entered in this box will be searched in all modules."))
#define WT_SD_SEARCH_MULTIPLE_WORDS WT(i18n("Search multiple words"), i18n("Select this to search more than one word."))
#define WT_SD_SEARCH_EXACT_MATCH WT(i18n("Exact match"), i18n("Select this to search exactly the text you entered."))
#define WT_SD_SEARCH_REGEXP WT(i18n("Search with GNU regexp"), i18n("Select this to search with standard GNU regular expressions."))
#define WT_SD_SEARCH_CASE_SENSITIVE WT(i18n("Search case sensitive"), i18n("Activate this to use case sensitive search."))

#define WT_SD_SCOPE_NO_SCOPE WT(i18n("No scope"), i18n("Select this if you want to search the whole module(s)."))
#define WT_SD_SCOPE_LAST_RESULT WT(i18n("Last search result"), i18n("Select this if you want to use last search result of a module<BR>for the search scope of the module."))
#define WT_SD_SCOPE_SET_BOUNDS WT(i18n("Set bounds"), i18n("Select this if you want to specify own search scopes."))
#define WT_SD_SCOPE_LOWER_BOUND WT(i18n("Lower bound"), i18n("This editbox contains the lower bound of the currently edited search scope."))
#define WT_SD_SCOPE_UPPER_BOUND WT(i18n("Upper bound"), i18n("This editbox contains the upper bound of the currently edited search scope."))
#define WT_SD_SCOPE_ADD_SCOPE WT(i18n("Add scope"), i18n("Use this button to add the edited scope to the selected module."))
#define WT_SD_SCOPE_DELETE_SCOPE WT(i18n("Delete scope"), i18n("Use this button o delete the currently selected scope."))

#define WT_SD_RESULT_RESULT_VIEW WT(i18n("Found items"), i18n("This contains the found items of the searched module(s)"))
#define WT_SD_RESULT_PREVIEW WT(i18n("Verse preview"), i18n("This text box contains the text of the selected verse."))

#define WT_SD_ANALYSIS_SAVE_AS_IMAGE WT(i18n("Save as image"), i18n("Use this button to save the graphical search analysis as an image to disk."))
#define WT_SD_ANALYSIS_GRAPH_VIEW WT(i18n("Graphical analysis"), i18n("This area contains the graphical search analysis."))

//#defines for the settings dialog, OD means Optionsdialog
#define WT_OD_GENERAL_DAILY_TIP WT(i18n("Show daily tip"), i18n("Activate this box to see a daily tip on startup."))
#define WT_OD_GENERAL_INTERNATIONAL_BOOKNAMES WT(i18n("International booknames"), i18n("Activate this to have international booknames."))
#define WT_OD_GENERAL_INTERNATIONAL_BOOKNAMES_LIST WT(i18n("List of languages"), i18n("This list contains the available<BR>languages used for international booknames."))
#define WT_OD_GENERAL_SHOW_STARTUPLOGO WT(i18n("Show startuplogo"), i18n("Activate this to see a startup logo on each startup of BibleTime."))

#define WT_OD_FONTS_PRESENTER WT(i18n("Font of presenter"), i18n("This font will be used as general font in all presenters."))
#define WT_OD_FONTS_PRESENTER_FIXED WT(i18n("Fixed font of presenters"), i18n("This font will be used as fixed font in all presenters."))
#define WT_OD_FONTS_PRINTING WT(i18n("Font for printing"), i18n("This font will be used as general font for printing."))
#define WT_OD_FONTS_PRINTING_FIXED WT(i18n("Fixed font for printing"), i18n("This font will be used as general fixed font for printing."))

//FFM does mean "Foreign font management"
#define WT_OD_FFM_FONTS_LIST WT(i18n("List of modules"), i18n("This list contains the modules using foreign fonts."))

#define WT_OD_COLORS_BACKGROUND WT(i18n("Background color"), i18n("This is the background color used in presenters."))
#define WT_OD_COLORS_NORMAL_TEXT WT(i18n("Normal text"), i18n("This is the color of normal text used in presenters."))
#define WT_OD_COLORS_VERSENUMBER WT(i18n("Versenumber"), i18n("This is the color of versenumbers used in presenters."))
#define WT_OD_COLORS_CURRENT_VERSE WT(i18n("Highlighted verse"), i18n("This is the color of the highlighted verse used in presenters."))

//defines for the printing dialog; PI == Print Item; PD=Printer Dialog
#define WT_PD_PRINT WT(i18n("Start printing"), i18n("Start the printing process. It will print the items in the list using<BR>the styles and the selected options (printer, paper size ...)"))
#define WT_PD_PREVIEW WT(i18n("Open preview"), i18n("This will print into a temporary file and open a postscript viewer to show the file."))
#define WT_PD_CANCEL WT(i18n("Cancel"), i18n("Closes this dialog without printing the items."))
#define WT_PD_HELP WT(i18n("Open help"), i18n("Open the online help of BibleTime."))

#define WT_PD_GENERAL_PRINTER_LIST WT(i18n("The list of printers"), i18n("This is the list of available printers on your system.<BR>Please select the printer you want to use."))
#define WT_PD_GENERAL_PRINT_TO_FILE WT(i18n("Print into file"), i18n("Print into a postrscript file on disk."))
#define WT_PD_GENERAL_BROWSE_FILE WT(i18n("Search file"), i18n("Browse for the file you want to print print to."))
#define WT_PD_GENERAL_COPIES WT(i18n("Copies"), i18n("The number of copies used for printing."))
#define WT_PD_GENERAL_ORIENTATION WT(i18n("Orientation"), i18n("Choose the type of Orientation"))
#define WT_PD_GENERAL_ORIENTATION_PORTAIT WT(i18n("Portrait"), i18n("Print on the paper using Portait orientation."))
#define WT_PD_GENERAL_ORIENTATION_LANDSCAPE WT(i18n("Landscape"), i18n("Print on the paper using Landscape orientation."))
#define WT_PD_GENERAL_PAPER_TYPE WT(i18n("Paper type"), i18n("The type of the paper on which you want to print."))
#define WT_PD_GENERAL_PAPER_SIZE WT(i18n("Paper size"), i18n("Select the paper size of your printer. The size will be used fo printing."))

#define WT_PD_ENTRIES_STYLE_COMBO WT(i18n("Styles"), i18n("This combo boy contains the available styles."))
#define WT_PD_ENTRIES_PI_LIST WT(i18n("List of print items"), i18n("This is the list which contains the available print items."))
#define WT_PD_ENTRIES_PI_MOVE_UP WT(i18n("Move item up"), i18n("Move the selected item one item above."))
#define WT_PD_ENTRIES_PI_MOVE_DOWN WT(i18n("Move item down"), i18n("Move the selected item one item below."))
#define WT_PD_ENTRIES_PI_DELETE WT(i18n("Delete items"), i18n("Delete the selected items."))
#define WT_PD_ENTRIES_PI_PAGE_BREAK WT(i18n("Insert page break"), i18n("Inserts a page break into the list.<BR>The printer will begin here a new page."))

#define WT_PD_LAYOUT_BORDER WT(i18n("Page borders"), i18n("This group contains the page borders of the page."))
#define WT_PD_LAYOUT_BORDER_LEFT WT(i18n("Left border"), i18n("The size of the border on the left side of the paper sheet."))
#define WT_PD_LAYOUT_BORDER_RIGHT WT(i18n("Right border"), i18n("The size of the border on the left side of the paper sheet."))
#define WT_PD_LAYOUT_BORDER_TOP WT(i18n("Upper border"), i18n("The size of the border on the bottom of the paper sheet."))
#define WT_PD_LAYOUT_BORDER_BOTTOM WT(i18n("Lower border"), i18n("The size of the border on the top of the paper sheet."))
#define WT_PD_LAYOUT_STYLE_LIST WT(i18n("List of styles"), i18n("This list contains the available styles. You can create your own ones."))
#define WT_PD_LAYOUT_STYLE_DELETE WT(i18n("Delete style"), i18n("Remove the selected style from the list."))
#define WT_PD_LAYOUT_STYLE_EDIT WT(i18n("Edit style"), i18n("Change the properties of the selected style."))
#define WT_PD_LAYOUT_STYLE_NEW WT(i18n("Create new style"), i18n("Create a new style.<BR>The new style will be added to the list."))

#endif

