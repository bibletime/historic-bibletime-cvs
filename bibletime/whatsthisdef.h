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

////QWhatsThis defines for the groupmanager
//#define WT_GM_WIDGET WT(TT_GM_WIDGET, i18n("This is the main index of BibleTime.<BR>Use it to manage the installed modules, to work with bookmarks, to open display windows etc.."))
//#define WT_GM_NEW_GROUP WT(TT_GM_NEW_GROUP, i18n("Create a new folder in the main index.<BR>If you press the right mouse button on a folder, the new folder will become a sub-folder of it.<BR>Otherwise you will create a toplevel folder."))
//#define WT_GM_CHANGE_GROUP WT(TT_GM_CHANGE_GROUP, i18n("Change the properties of the selected folder."))
//#define WT_GM_CHANGE_BOOKMARK WT(TT_GM_CHANGE_BOOKMARK, i18n("Change the properties of the selected bookmark."))
//#define WT_GM_IMPORT_BOOKMARKS WT(TT_GM_IMPORT_BOOKMARKS, i18n("Import bookmarks from a file."))
//#define WT_GM_EXPORT_BOOKMARKS WT(TT_GM_EXPORT_BOOKMARKS, i18n("Export bookmarks to a file so you or other<BR>users can import them later."))
//#define WT_GM_PRINT_BOOKMARK WT(TT_GM_PRINT_BOOKMARK, i18n("Use this to add this bookmark to BibleTime's print queue. The key, the description, and the text of the key will be used for printing."))
//#define WT_GM_UNLOCK_MODULE WT(TT_GM_UNLOCK_MODULE, i18n("Open a small dialog where you can unlock the selected module."))
//#define WT_GM_ABOUT_MODULE WT(TT_GM_ABOUT_MODULE, i18n("Open a dialog to show information about the selected module."))

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

#define WT_OD_KEYS_CHOOSER WT(TT_OD_KEYS_CHOOSER, i18n("Select an action and change the key binding for the action."))
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
//#define WT_PD_LAYOUT_STYLE_LIST WT(TT_PD_LAYOUT_STYLE_LIST, i18n("This list contains the available printing styles.<BR>Create your own one using the \"New style\" button."))
//#define WT_PD_LAYOUT_STYLE_DELETE WT(TT_PD_LAYOUT_STYLE_DELETE, i18n("Remove the selected printing style from the list."))
//#define WT_PD_LAYOUT_STYLE_EDIT WT(TT_PD_LAYOUT_STYLE_EDIT, i18n("Change the properties of the selected printing style."))
//#define WT_PD_LAYOUT_STYLE_NEW WT(TT_PD_LAYOUT_STYLE_NEW, i18n("Create a new printing style.<BR>The new style will be added to the list."))

#endif

