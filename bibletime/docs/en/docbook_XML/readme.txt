Docbook file conventions

Fred Saalbach - Updated 10 April 2001

The dockbook files have the following convention

index.docbook
this file contains the ENTITY statements that define the path of all chapters
in the bookset. this file also contains the set title and info files and the
pointers to all ***idx.docbook files.

***idx.docbook
one of these exists for each book.  These contain the book info and pointers to
the book chapters.

All files belonging to a book need to start with the same letters.  they should
have some relationship to the name of the book.  For example hdbk stands for
handbook.


ID= tags
these tags need to have relationship to the book and section. Jade uses the ID
tags to name HTML files.  The file names are as follows now.

hdbk_idx.docbook
hdbk_intro.docbook
hdbk_operation.docbook
hdbk_start.docbook
hdbk_toolbars.docbook
help_first_startup.docbook
help_first_time_search.docbook
help_idx.docbook
help_initbackend_failed.docbook
help_module_encrypted.docbook
help_no_mods_d.docbook
help_no_modules.docbook
how2_basics.docbook
how2_idx.docbook
how2_importance.docbook
how2_interpretation.docbook
index.docbook
inst_bin.docbook
inst_dvel.docbook
inst_idx.docbook
inst_preq.docbook
inst_sources.docbook
inst_user.docbook
readme.txt

Pictures
All pictures are stored in the en/HTML/common folder. If you want to recompile
these files (kdb2html dockbook.index) such as when translating them, please move
the pictures to another location.  You wlll need to move them back after kdb2html finishes.
The reason for this is that kdb2html discardes the contents of HTML and
subdirectories every time it is run.  However the generated HTML always look for
pictures in en/HTML/common.

Every time you run kde2html you will need to copy the /common folder to
/html/common for pictures to display correctly.

Picture naming convention is as follows:
logotp3.png is the default name for the logo at the top of the page.  This is
defined by the DTD.  For the time being we are placing the Kdesktop Environment
logo here.  The DTD links this logo to the KDE home.

As in the case for naming document ID's please name pictures according to the
document ID's of the section they belong in.  For example:

hdbk_op_module_bookmk_1.png
hdbk_op_module_select_1.png
hdbk_op_module_select_2.png
hdbk_op_module_tree_1.png

Please use *.png picture files in accordance with the recomendation of kde.org.




