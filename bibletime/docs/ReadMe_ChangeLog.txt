-----------------------------------
22 Sept 2001

KDE 2.0 DTD eliminates the use of the graphic element,

<graphic fileref="common/hdbk_op_module_tree_1.png" format="png"></graphic>


and replaces it with the inline media object.

<inlinemediaobject>
<imageobject> <imagedata fileref="rectangles.eps" format="eps"> </imageobject>
<imageobject> <imagedata fileref="rectangles.png" format="png"> </imageobject>
<textobject> <phrase>A bunch of rectangles</phrase> </textobject>
</inlinemediaobject>

Inline media object needs to be inside <para></para> tags

As the screenshot tag was causing trouble, it has been deleted.  The screen info has been moved to the textobject phrase.

I have fixed this in the english version of the handbook.

Fix applied to the following files:

hdbk_operation.docbook
hdbk_start.docbook
hdbk_toolbars.docbook

Other files do not have this error, and were not changed

Fred Saalbach
-------------