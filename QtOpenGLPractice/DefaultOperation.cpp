#include "Operation.h"
#include "DemoOperation.h"
#include "EditVectorFontOperation.h"
#include "UnicodeOutlineOperation.h"

Operation* createDefaultOperation(const std::string &name)
{
    if (name == "FirstTriangle")
    {
        FirstTriangle* ft = new FirstTriangle;
        return ft;
    }
    else if (name == "FontOperation")
    {
        FontOperation* font = new FontOperation;
        return font;
    }
    else if (name == "MillionPrimitiveOperation")
    {
        MillionPrimitiveOperation* million = new MillionPrimitiveOperation;
        return million;
    }
    else if (name == "EditVectorFont")
    {
        EditVectorFont* edit_font = new EditVectorFont;
        return edit_font;
    }
    else if (name == "UnicodeOutlineOperation")
    {
        UnicodeOutlineOperation* font_outline = new UnicodeOutlineOperation;
        return font_outline;
    }

    return new FirstTriangle;

}