Convenzioni file Docbook

Fred Saalbach - Aggiornato 10 Aprile 2001

I file dockbook hanno le seguenti convenzioni:

index.docbook questo file contiene le istruzioni ENTITY che definiscono
il percorso di tutti i capitoli del libro. Quasto file, inoltre,
contiene il titolo, i file info e i puntatori a tutti i file
***idx.docbook.

***idx.docbook uno di questi esiste per ogni libro. Contengono le
informazioni sul libro e i puntatori ai capitoli.

Tutti i file appartenenti a un libro devono iniziare con le stesse lettere. Dovrebbero anche essere in relazione con il nome del libro. Per esempio, hdbk significa handbook.

ID= tags
questi tag devono essere in relazione con il libro e la sezione. Jade usa gli ID tag per dare un nome ai file HTML. I nomi sono questi.

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

Immagini
Tutte le immagini sono messe nella directory en/HTML/common. Se vuoi
ricompilare questi file (kdb2html dockbook.index), come avviene in caso
di una traduzione, spostali altrove. Dovrai poi rimetteri a posto dopo
che kdb2html termina.
Questo accade perché kdb2html scarta il contenuto di HTML e
sottodirectory ogni volta che viene lanciato. Comunque, il codice HTML
generato cercherà sempre le immagini in en/HTML/common.

Ogni volta che esegui kde2html dovrai copiare la directory /common in
/html/common per visualizzare correttamente le immagini.

Queste sono le convenzioni sui nomi delle immagini:
logotp3.png è il nome di default per il logo in cima alla pagina. Questo
è definito dal DTD. Per questa volta ci stiamo inserendo il logo del K
Desktop Environment, che il DTD collega alla home page di KDE.

Come nel caso dei nomi agli ID di un documento, chiama le immagini in
accordo con l'ID del documento che contiene le immagini. Per esempio:

hdbk_op_module_bookmk_1.png
hdbk_op_module_select_1.png
hdbk_op_module_select_2.png
hdbk_op_module_tree_1.png

Per farove, usa immagini *.png in accordo alle raccomandazioni di
kde.org.

