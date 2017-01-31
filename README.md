An experiment in elevator management and test of my GUI toolkit.


##Updates made to DXTKGui that have not been pushed to DXTKGui project
>- changed how mouse gets used by gui controls
>- dialog title and text positions now moving properly after a drag event
>- dialog properly updates when text changes to blank text
>- createLine in PrimitiveShapes.h updated and usuable
>- createTriangleFrame in PrimitiveShapes.h
>- added code to main.cpp to pause game when titlebar clicked
>- allows the creation of imagebuttons with an existing sprite
>- added getHitArea function to GUIControl
>- fixed a bug where height was getting value of width in ImageButton constructor
>- added a layer depth control to GUIControls and Sprites; set GUIControl default to 1.0f and Sprite to 0.0f
>- added spinner control
>- fixed issue where dialog button area not re-sizing properly when pre-made button added


## Need to do
>- allow GUIcontrols to use depth attribute in spritebatch (Implemented; not tested)
>- ~~dialog/title text does not change tint~~ (fixed)
>- ~~dialog text bg color doesn't match text bg if tint not set manually~~ (fixed)
>- dialog parser to handle a word that doesn't fit on a line (fixed?)
>- dialog parser to add hyphen to word that gets broken (and option to turn on/off)
>- ~~dialog text area should expand to fill all dialog if title and button area not used~~
>- force minimum size for dialog box
>- ~~tearing on panel texture~~ (changed buffer in GUIFactory::createTextureFromIElement2D() to 20 from 5)
>- ~~dialog able to print non-centerd text~~
>- ImageButtons do not go back to hoverstate after press release