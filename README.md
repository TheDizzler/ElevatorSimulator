An experiment in elevator management and test of my GUI toolkit.


##Updates made to DXTKGui that have not been pushed to DXTKGui project
>- changed how mouse gets used by gui controls
>- dialog title and text positions not moving properly after a drag event
>- createLine in PrimitiveShapes.h updated and usuable

## Need to do
>- ~~dialog/title text does not change tint~~ (fixed)
>- ~~dialog text bg color doesn't match text bg if tint not set manually~~ (fixed)
>- dialog parser to handle a word that doesn't fit on a line (fixed?)
>- dialog parser to add hyphen to word that gets broken (and option to turn on/off)
>- ~~dialog text area should expand to fill all dialog if title and button area not used~~
>- force minimum size for dialog box
>- ~~tearing on panel texture~~ (changed buffer in GUIFactory::createTextureFromIElement2D() to 20 from 5)
>- ~~dialog able to print non-centerd text~~