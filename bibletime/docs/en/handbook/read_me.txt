The dockbook XML files in this directory can be processed into HTML pages by the
command   kdb2html bt_hdbk.dockbook

 bt_hdbk.dockbook contains document headder information and calls the other
sections.

The remaining .dockbook files contain the text and <dockbook tags> that will
generate the handbook.

Pictures are placed in the /common folder.  after generating the html code, copy
/common to /HTML/common to view the resulting code.  Everytime kdb2html is run,
a new copy of /common will need to be draged to /HTML/common

Please use the section id as part of the picture name to make it easier to
identify them later.

Fred Saalbach